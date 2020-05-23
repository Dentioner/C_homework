#include "ip.h"
#include "icmp.h"
#include "packet.h"
#include "arpcache.h"
#include "rtable.h"
#include "arp.h"

#include "mospf_proto.h"
#include "mospf_daemon.h"

#include "log.h"

#include <stdlib.h>
#include <assert.h>

// handle ip packet
//
// If the packet is ICMP echo request and the destination IP address is equal to
// the IP address of the iface, send ICMP echo reply; otherwise, forward the
// packet.
void handle_ip_packet(iface_info_t *iface, char *packet, int len)
{
	struct iphdr *ip = packet_to_ip_hdr(packet);
	u32 daddr = ntohl(ip->daddr);
	if (daddr == iface->ip) 
	{ // �����������Ǹ��Լ���
		if (ip->protocol == IPPROTO_ICMP) // �����icmp�� 
		{
			struct icmphdr *icmp = (struct icmphdr *)IP_DATA(ip);
			if (icmp->type == ICMP_ECHOREQUEST) // �����ping���İ� 
			{
				icmp_send_packet(packet, len, ICMP_ECHOREPLY, 0);
			}
		}
		else if (ip->protocol == IPPROTO_MOSPF) // �����mOSPF�İ�
		{
			handle_mospf_packet(iface, packet, len);
		}

		free(packet);
	}
	else if (ip->daddr == htonl(MOSPF_ALLSPFRouters))  // ������鲥��mOSPF���������Լ����ڵ����֣�
	{
		assert(ip->protocol == IPPROTO_MOSPF);
		handle_mospf_packet(iface, packet, len);

		free(packet);
	}
	else // ���������Ҫ���ݸ���һ���ڵ�İ�
	{
		ip_forward_packet(daddr, packet, len);
	}
}
