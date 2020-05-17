#include "ip.h"
#include "icmp.h"

#include <stdio.h>
#include <stdlib.h>

// handle ip packet
//
// If the packet is ICMP echo request and the destination IP address is equal to
// the IP address of the iface, send ICMP echo reply; otherwise, forward the
// packet.
void handle_ip_packet(iface_info_t *iface, char *packet, int len)
{
	// fprintf(stderr, "TODO: handle ip packet.\n");
	struct iphdr *ip_head = packet_to_ip_hdr(packet);
	struct icmphdr * icmp_head = (struct icmphdr *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);

	ip_head->ttl--;

	if(ip_head->ttl <= 0)
	{
		// �ظ�ICMP��Ϣ
		icmp_send_packet(packet, len, ICMP_TIME_EXCEEDED, ICMP_EXC_TTL);
		// �������ݰ�
		free(packet);		
	}
	else if((ip_head->protocol == IPPROTO_ICMP) && // �ȼ���ǲ���ICMP���ģ��������ֱ��������������
			(icmp_head->type == ICMP_ECHOREQUEST) && // If the packet is ICMP echo request
			(ntohl(ip_head->daddr) == iface->ip))  // and the destination IP address is equal to the IP address of the iface
	{
		icmp_send_packet(packet, len, ICMP_ECHOREPLY, 0); 
		free(packet);
	}
	else // ����Ĭ��Ϊ��ͨIP��Ϣ
	{
		ip_send_packet(packet, len);
	}
	


}
