#include "icmp.h"
#include "ip.h"
#include "rtable.h"
#include "arp.h"
#include "base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// send icmp packet
// ���ֵ��ñ������������
// 1. ·�ɱ����ʧ�ܣ�����ip_send_packet�ĵ��ã�ip_base.c
// 2. ARP��ѯʧ�ܣ�����arpcache_sweep�ĵ��ã�arpcache.c
// 3. TTLֵ��Ϊ0������handle_ip_packet�ĵ��ã�ip.c
// 4. Ping��������handle_ip_packet�ĵ��ã�ip.c
void icmp_send_packet(const char *in_pkt, int len, u8 type, u8 code)
{
	// fprintf(stderr, "TODO: malloc and send icmp packet.\n");
	struct ether_header *old_eth_head = (struct ether_header *)in_pkt;
	struct iphdr *old_ip_head = packet_to_ip_hdr(in_pkt);
	struct icmphdr * old_icmp_head = (struct icmphdr *)(in_pkt + ETHER_HDR_SIZE + IP_HDR_SIZE(old_ip_head));


	int new_packet_len;
	char * new_packet;

	

	if(type == ICMP_ECHOREPLY) // ������������͵�ICMP����
	{
		new_packet_len = len - IP_HDR_SIZE(old_ip_head) + IP_BASE_HDR_SIZE;
	}
	else
	{
		new_packet_len = ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + ICMP_HDR_SIZE + IP_HDR_SIZE(old_ip_head) + 8;
	}

	// �����µ�ICMP���ݰ�
	new_packet = (char *)malloc(new_packet_len);
	memset((void *)new_packet, 0, new_packet_len);
	struct ether_header *new_eth_head = (struct ether_header *)new_packet;
	struct iphdr *new_ip_head = packet_to_ip_hdr(new_packet);
	struct icmphdr *new_icmp_head = (struct icmphdr *)(new_packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);
	int icmp_tot_len;

	u32 new_daddr = ntohl(old_ip_head->saddr); // hton�Ĺ����������ip_init_hdr�����н��У����������Ҫ����ʱ���ԭ��
	rt_entry_t *rt_p = longest_prefix_match(new_daddr);
	u32 new_saddr = rt_p->iface->ip; // iface����ŵ��������������ֽ���

	// ��ʼ��
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
