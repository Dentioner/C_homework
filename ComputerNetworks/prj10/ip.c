#include "ip.h"
#include "icmp.h"
#include "packet.h"
#include "arpcache.h"
#include "rtable.h"
#include "arp.h"
#include "nat.h"

#include <stdlib.h>

void handle_ip_packet(iface_info_t *iface, char *packet, int len)
{
	struct iphdr *ip = packet_to_ip_hdr(packet);
	u32 daddr = ntohl(ip->daddr);
	if (daddr == iface->ip && ip->protocol == IPPROTO_ICMP) // 如果收到的是ICMP的包
	{
		struct icmphdr *icmp = (struct icmphdr *)IP_DATA(ip);
		if (icmp->type == ICMP_ECHOREQUEST) {
			icmp_send_packet(packet, len, ICMP_ECHOREPLY, 0);
		}

		free(packet);
	}
	else // 其余的包都是这个函数处理
	{
		nat_translate_packet(iface, packet, len);
	}
}

