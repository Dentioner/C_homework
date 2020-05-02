#include "packet.h"
#include "types.h"
#include "ether.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>

extern ustack_t *instance;

// send the packet out from iface
void iface_send_packet(iface_info_t *iface, char *packet, int len)
{
	struct sockaddr_ll addr;
	memset(&addr, 0, sizeof(struct sockaddr_ll));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = iface->index;
	addr.sll_halen = ETH_ALEN;
	addr.sll_protocol = htons(ETH_P_ARP);
	struct ether_header *eh = (struct ether_header *)packet;
	memcpy(addr.sll_addr, eh->ether_dhost, ETH_ALEN);

	if (sendto(iface->fd, packet, len, 0, (struct sockaddr *)&addr,
				sizeof(struct sockaddr_ll)) < 0) 
	{
		perror("Send raw packet failed");
	}
}

// broadcast the packet among all the interfaces except the one receiving the
// packet
void broadcast_packet(iface_info_t *rx_iface, char *packet, int len)
{
	// TODO: broadcast packet 
	//fprintf(stdout, "TODO: broadcast packet here.\n");

	iface_info_t *iface = NULL;
	iface_info_t *q = NULL;
	list_for_each_entry_safe(iface, q, &instance->iface_list, list)
	{
		if(iface != rx_iface)
		{
			iface_send_packet(iface, packet, len);
		}
	}

}

