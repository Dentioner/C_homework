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
	{ // 如果这个包就是给自己的
		if (ip->protocol == IPPROTO_ICMP) // 如果是icmp包 
		{
			struct icmphdr *icmp = (struct icmphdr *)IP_DATA(ip);
			if (icmp->type == ICMP_ECHOREQUEST) // 如果是ping发的包 
			{
				icmp_send_packet(packet, len, ICMP_ECHOREPLY, 0);
			}
		}
		else if (ip->protocol == IPPROTO_MOSPF) // 如果是mOSPF的包
		{
			handle_mospf_packet(iface, packet, len);
		}

		free(packet);
	}
	else if (ip->daddr == htonl(MOSPF_ALLSPFRouters))  // 如果是组播的mOSPF包（宣告自己存在的那种）
	{
		assert(ip->protocol == IPPROTO_MOSPF);
		handle_mospf_packet(iface, packet, len);

		free(packet);
	}
	else // 否则就是需要传递给下一个节点的包
	{
		ip_forward_packet(daddr, packet, len);
	}
}
