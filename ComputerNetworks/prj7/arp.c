#include "arp.h"
#include "base.h"
#include "types.h"
#include "packet.h"
#include "ether.h"
#include "arpcache.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "log.h"

// send an arp request: encapsulate an arp request packet, send it out through 
// iface_send_packet
void arp_send_request(iface_info_t *iface, u32 dst_ip)
{
	// ��װARP�������ͳ�ȥ
	// ���ù㲥��ʽ
	//fprintf(stderr, "TODO: send arp request when lookup failed in arpcache.\n");
	int packet_len = ETHER_HDR_SIZE + sizeof(struct ether_arp); // ����PPT��15ҳ�Ľṹ�����ٺ����������ɷ�
	char * packet = (char *)malloc(sizeof(char) * packet_len);
	memset(packet, 0, (size_t)packet_len);

	// init packet	
	struct ether_header * ether_p = (struct ether_header *)packet;
	struct ether_arp * arp_p = (struct ether_arp *)(packet + ETHER_HDR_SIZE);

	memset((void *)(ether_p->ether_dhost), 0xff, ETH_ALEN); // dst ether addr = ff:ff:ff:ff:ff:ff
	memcpy((void *)(ether_p->ether_shost), (void *)(iface->mac), ETH_ALEN); // src ether addr = iface.mac
	ether_p->ether_type = htons(ETH_P_ARP); // proto type = 0x0806

	arp_p->arp_hrd = htons(ARPHRD_ETHER); 	// arp header = 0x01
	arp_p->arp_pro = htons(ETH_P_IP); 		// arp proto = 0x0800
	arp_p->arp_hln = ETH_ALEN;				// arp hw addr len = 6, ���ֽ�����ת���ֽ���
	arp_p->arp_pln = 4; 					// arp proto addr len = 4
	arp_p->arp_op  = htons(ARPOP_REQUEST);	// arp op type = 1
	//arp_p->arp_sha = 
	memcpy((void *)(arp_p->arp_sha), (void *)(iface->mac), ETH_ALEN); // arp sender hw addr = iface.mac
	arp_p->arp_spa = htonl(iface->ip);		// arp sender proto addr = iface.ip
	//arp_p->arp_tha = 						// ��arp������arp����ʱ���������ÿ�
	arp_p->arp_tpa = htonl(dst_ip);			// arp targert proto addr = dst ip

	// broadcast arp request
	iface_send_packet(iface, packet, packet_len);

	
}

// send an arp reply packet: encapsulate an arp reply packet, send it out
// through iface_send_packet
void arp_send_reply(iface_info_t *iface, struct ether_arp *req_hdr)
{
	// ���õ�����ʽ��Ӧ
	// fprintf(stderr, "TODO: send arp reply when receiving arp request.\n");
	int packet_len = ETHER_HDR_SIZE + sizeof(struct ether_arp); // ����PPT��15ҳ�Ľṹ�����ٺ����������ɷ�
	char * packet = (char *)malloc(sizeof(char) * packet_len);
	memset(packet, 0, (size_t)packet_len);

	// init packet	
	struct ether_header * ether_p = (struct ether_header *)packet;
	struct ether_arp * arp_p = (struct ether_arp *)(packet + ETHER_HDR_SIZE);

	memcpy((void *)(ether_p->ether_dhost), (void *)(req_hdr->arp_sha), ETH_ALEN); // dst ether addr = sender addr of received arp request
	memcpy((void *)(ether_p->ether_shost), (void *)(iface->mac), ETH_ALEN); // src ether addr = iface.mac
	ether_p->ether_type = htons(ETH_P_ARP); // proto type = 0x0806

	arp_p->arp_hrd = htons(ARPHRD_ETHER); 	// arp header = 0x01
	arp_p->arp_pro = htons(ETH_P_IP); 		// arp proto = 0x0800
	arp_p->arp_hln = ETH_ALEN;				// arp hw addr len = 6, ���ֽ�����ת���ֽ���
	arp_p->arp_pln = 4; 					// arp proto addr len = 4
	arp_p->arp_op  = htons(ARPOP_REPLY);	// arp op type = 2
	//arp_p->arp_sha = 
	memcpy((void *)(arp_p->arp_sha), (void *)(iface->mac), ETH_ALEN); // arp sender hw addr = iface.mac
	arp_p->arp_spa = htonl(iface->ip);		// arp sender proto addr = iface.ip
	//arp_p->arp_tha = 						
	memcpy((void *)(arp_p->arp_tha), (void *)(req_hdr->arp_sha), ETH_ALEN); // arp target hw addr = sender addr of received arp request
	arp_p->arp_tpa = req_hdr->arp_spa; // arp targert proto addr = sender ip addr of received arp request
	// ���ﲻҪʹ��htonl����Ϊ�������req_hdr��ֱ�Ӵ�arp�������ó����ģ�����ͬһ���ֽ��򣬲�Ҫת��


	// send arp reply
	iface_send_packet(iface, packet, packet_len);
}

void handle_arp_packet(iface_info_t *iface, char *packet, int len)
{
	// �˺���Ϊ����ARP����ͻ�Ӧ���ܵ��߼�
	// fprintf(stderr, "TODO: process arp packet: arp request & arp reply.\n");

	struct ether_arp * arp_p = (struct ether_arp *)(packet + ETHER_HDR_SIZE);

	// �����ж����ݰ�����ARP�������main������ʱ��ִ�й��ˣ�����ֻ���ж���arp request ����arp reply
	if(ntohs(arp_p->arp_op) == ARPOP_REQUEST)
	{
		if(iface->ip == ntohl(arp_p->arp_tpa))
		{
			arpcache_insert(ntohl(arp_p->arp_spa), arp_p->arp_sha);
			arp_send_reply(iface, arp_p);	
		}

	}
	else if(ntohs(arp_p->arp_op) == ARPOP_REPLY)
	{
		arpcache_insert(ntohl(arp_p->arp_spa), arp_p->arp_sha);
	}
	
	
	

}

// send (IP) packet through arpcache lookup ͨ����ѯARP���淢��IP���ݰ�
//
// Lookup the mac address of dst_ip in arpcache. If it is found, fill the
// ethernet header and emit the packet by iface_send_packet, otherwise, pending 
// this packet into arpcache, and send arp request.
void iface_send_packet_by_arp(iface_info_t *iface, u32 dst_ip, char *packet, int len)
{
	struct ether_header *eh = (struct ether_header *)packet;
	memcpy(eh->ether_shost, iface->mac, ETH_ALEN);
	eh->ether_type = htons(ETH_P_IP);

	u8 dst_mac[ETH_ALEN];
	int found = arpcache_lookup(dst_ip, dst_mac);
	if (found) {
		// log(DEBUG, "found the mac of %x, send this packet", dst_ip);
		memcpy(eh->ether_dhost, dst_mac, ETH_ALEN);
		iface_send_packet(iface, packet, len);
	}
	else {
		// log(DEBUG, "lookup %x failed, pend this packet", dst_ip);
		arpcache_append_packet(iface, dst_ip, packet, len);
	}
}
