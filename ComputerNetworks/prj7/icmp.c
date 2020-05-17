#include "icmp.h"
#include "ip.h"
#include "rtable.h"
#include "arp.h"
#include "base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// send icmp packet
// 四种调用本函数的情况：
// 1. 路由表查找失败，来自ip_send_packet的调用，ip_base.c
// 2. ARP查询失败，来自arpcache_sweep的调用，arpcache.c
// 3. TTL值减为0，来自handle_ip_packet的调用，ip.c
// 4. Ping包，来自handle_ip_packet的调用，ip.c
void icmp_send_packet(const char *in_pkt, int len, u8 type, u8 code)
{
	// fprintf(stderr, "TODO: malloc and send icmp packet.\n");
	struct ether_header *old_eth_head = (struct ether_header *)in_pkt;
	struct iphdr *old_ip_head = packet_to_ip_hdr(in_pkt);
	struct icmphdr * old_icmp_head = (struct icmphdr *)(in_pkt + ETHER_HDR_SIZE + IP_HDR_SIZE(old_ip_head));


	int new_packet_len;
	char * new_packet;

	

	if(type == ICMP_ECHOREPLY) // 检查是哪种类型的ICMP报文
	{
		new_packet_len = len - IP_HDR_SIZE(old_ip_head) + IP_BASE_HDR_SIZE;
	}
	else
	{
		new_packet_len = ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + ICMP_HDR_SIZE + IP_HDR_SIZE(old_ip_head) + 8;
	}

	// 创建新的ICMP数据包
	new_packet = (char *)malloc(new_packet_len);
	memset((void *)new_packet, 0, new_packet_len);
	struct ether_header *new_eth_head = (struct ether_header *)new_packet;
	struct iphdr *new_ip_head = packet_to_ip_hdr(new_packet);
	struct icmphdr *new_icmp_head = (struct icmphdr *)(new_packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);
	int icmp_tot_len;

	u32 new_daddr = ntohl(old_ip_head->saddr); // hton的工作在下面的ip_init_hdr函数中进行，因此这里需要先暂时变回原样
	rt_entry_t *rt_p = longest_prefix_match(new_daddr);
	u32 new_saddr = rt_p->iface->ip; // iface里面放的数据是正常的字节序

	// 初始化
	memcpy(new_eth_head->ether_dhost, old_eth_head->ether_shost, ETHER_HDR_SIZE);
	memcpy(new_eth_head->ether_shost, old_eth_head->ether_dhost, ETHER_HDR_SIZE);
	new_eth_head->ether_type = htons(ETH_P_IP);

	ip_init_hdr(new_ip_head, new_saddr, new_daddr, (new_packet_len - ETHER_HDR_SIZE), IPPROTO_ICMP);

	new_icmp_head->type = type;
	new_icmp_head->code = code;
	if(type == ICMP_ECHOREPLY)
	{
		icmp_tot_len = ntohs(old_ip_head->tot_len) - IP_HDR_SIZE(old_ip_head);
		memcpy((void *)(&(new_icmp_head->u)), (void *)(&(old_icmp_head->u)), (icmp_tot_len - ICMP_HDR_SIZE + 4));
	}
	else
	{
		icmp_tot_len = IP_HDR_SIZE(old_ip_head) + ICMP_HDR_SIZE + 8;
		memcpy(((void *)new_icmp_head + ICMP_HDR_SIZE), (void *)old_ip_head, (IP_HDR_SIZE(old_ip_head) + 8));
	}
	new_icmp_head->checksum = icmp_checksum(new_icmp_head, icmp_tot_len);
	
	ip_send_packet(new_packet, new_packet_len);
}
