#include "arpcache.h"
#include "arp.h"
#include "ether.h"
#include "packet.h"
#include "icmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

static arpcache_t arpcache;

// initialize IP->mac mapping, request list, lock and sweeping thread
void arpcache_init()
{
	bzero(&arpcache, sizeof(arpcache_t));

	init_list_head(&(arpcache.req_list));

	pthread_mutex_init(&arpcache.lock, NULL);

	pthread_create(&arpcache.thread, NULL, arpcache_sweep, NULL);
}

// release all the resources when exiting
void arpcache_destroy()
{
	pthread_mutex_lock(&arpcache.lock);

	struct arp_req *req_entry = NULL, *req_q;
	list_for_each_entry_safe(req_entry, req_q, &(arpcache.req_list), list) {
		struct cached_pkt *pkt_entry = NULL, *pkt_q;
		list_for_each_entry_safe(pkt_entry, pkt_q, &(req_entry->cached_packets), list) {
			list_delete_entry(&(pkt_entry->list));
			free(pkt_entry->packet);
			free(pkt_entry);
		}

		list_delete_entry(&(req_entry->list));
		free(req_entry);
	}

	pthread_kill(arpcache.thread, SIGTERM);

	pthread_mutex_unlock(&arpcache.lock);
}

// lookup the IP->mac mapping
//
// traverse the table to find whether there is an entry with the same IP
// and mac address with the given arguments
int arpcache_lookup(u32 ip4, u8 mac[ETH_ALEN])
{
	// fprintf(stderr, "TODO: lookup ip address in arp cache.\n");
	int index1;
	int find = 0;
	
	pthread_mutex_lock(&arpcache.lock);
	// ��ѯIP-MAC���Ǳ����������������
	for(index1 = 0; index1 < MAX_ARP_SIZE; index1++)
	{
		if((arpcache.entries[index1].valid) && (arpcache.entries[index1].ip4 == ip4))
		{
			memcpy(mac, arpcache.entries[index1].mac, ETH_ALEN);
			find = 1;
			break;
		}
	}


	pthread_mutex_unlock(&arpcache.lock);
	return find;
}

// append the packet to arpcache
//
// Lookup in the list which stores pending packets, if there is already an
// entry with the same IP address and iface (which means the corresponding arp
// request has been sent out), just append this packet at the tail of that entry
// (the entry may contain more than one packet); otherwise, malloc a new entry
// with the given IP address and iface, append the packet, and send arp request.
void arpcache_append_packet(iface_info_t *iface, u32 ip4, char *packet, int len)
{
	// fprintf(stderr, "TODO: append the ip address if lookup failed, and send arp request if necessary.\n");
	struct arp_req *pos, *q, *tmp_arp;
	// bool find = false;
	pos = NULL;
	q = NULL;
	tmp_arp = NULL;

	// struct ether_header * ether_p = (struct ether_header *)packet;
	struct cached_pkt * tmp_c_packet = (struct cached_pkt *)malloc(sizeof(struct cached_pkt));
	tmp_c_packet->packet = packet;
	init_list_head(&(tmp_c_packet->list));
	tmp_c_packet->len = len;

	pthread_mutex_lock(&arpcache.lock);

	list_for_each_entry_safe(pos, q, &(arpcache.req_list), list)
	{
		if((pos->ip4 == ip4) && (pos->iface == iface))
		{
			// find = true;
			tmp_arp = pos;
			break;
		}
	}

	if(tmp_arp != NULL) // append this packet at the tail of that entry
	{
		list_add_tail(&(tmp_c_packet->list), &(tmp_arp->cached_packets));
	}
	else // malloc a new entry with the given IP address and iface, append the packet, and send arp request.
	{
		struct arp_req * new_arp_req = (struct arp_req *)malloc(sizeof(struct arp_req));
		
		// init new arp_req entry
		init_list_head(&(new_arp_req->list));
		new_arp_req->iface = iface;
		new_arp_req->ip4 = ip4;
		new_arp_req->sent = time(NULL);
		new_arp_req->retries = 0;
		init_list_head(&(new_arp_req->cached_packets));

		// append the packet to the new entry & append the new entry to the arpcache
		list_add_tail(&(tmp_c_packet->list), &(new_arp_req->cached_packets));
		list_add_tail(&(new_arp_req->list), &(arpcache.req_list));
		
	}
	
	pthread_mutex_unlock(&arpcache.lock);
}

// insert the IP->mac mapping into arpcache, if there are pending packets
// waiting for this mapping, fill the ethernet header for each of them, and send
// them out
void arpcache_insert(u32 ip4, u8 mac[ETH_ALEN])
{
	// fprintf(stderr, "TODO: insert ip->mac entry, and send all the pending packets.\n");

	int index1;
	struct arp_req *tmp_arp_entry, *p;
	struct cached_pkt *tmp_pkt, *q;

	pthread_mutex_lock(&arpcache.lock);

	// insert the IP->mac mapping into arpcache

	for(index1 = 0; index1 < MAX_ARP_SIZE; index1++)
	{
		if(arpcache.entries[index1].valid == 0) // found an invalid entry
		{
			break;
		}
	}

	if(index1 == MAX_ARP_SIZE) // û�ҵ�
	{
		srand((unsigned)time(NULL));
		index1 = rand() % MAX_ARP_SIZE; // �����һ��������
	}

	arpcache.entries[index1].ip4 = ip4;
	memcpy((void *)(arpcache.entries[index1].mac), (void *)mac, ETH_ALEN);
	arpcache.entries[index1].added = time(NULL);
	arpcache.entries[index1].valid = 1;

	// �������
	list_for_each_entry_safe(tmp_arp_entry, p, &(arpcache.req_list), list) // ������һ������Ҳ����PPT12ҳ��IP����
	{
		if(tmp_arp_entry->ip4 == ip4)
		{
			list_for_each_entry_safe(tmp_pkt, q, &(tmp_arp_entry->cached_packets), list) // �����ڶ�������Ҳ����PPT12ҳ��IP�ڵ������packet����
			{
				struct ether_header *head = (struct ether_header *)(tmp_pkt->packet);
				memcpy((void *)(head->ether_dhost), (void *)mac, ETH_ALEN);
				iface_send_packet(tmp_arp_entry->iface, tmp_pkt->packet, tmp_pkt->len);

				list_delete_entry(&(tmp_pkt->list));
				free(tmp_pkt); // cached_pkt ������append��������malloc�����ģ�����Ҫfree��ȥ
			}

			list_delete_entry(&(tmp_arp_entry->list));
			free(tmp_arp_entry); // arp_req ������append��������malloc�ģ�ҲҪfree
		}
	}

	pthread_mutex_unlock(&arpcache.lock);
}

// sweep arpcache periodically
//
// For the IP->mac entry, if the entry has been in the table for more than 15
// seconds, remove it from the table.
// For the pending packets, if the arp request is sent out 1 second ago, while 
// the reply has not been received, retransmit the arp request. If the arp
// request has been sent 5 times without receiving arp reply, for each
// pending packet, send icmp packet (DEST_HOST_UNREACHABLE), and drop these
// packets.
void *arpcache_sweep(void *arg) 
{
	int index1;
	struct arp_req *tmp_arp_entry, *p;
	struct cached_pkt *tmp_pkt, *q;
	while (1) 
	{
		sleep(1);
		// fprintf(stderr, "TODO: sweep arpcache periodically: remove old entries, resend arp requests .\n");

		pthread_mutex_lock(&arpcache.lock);

		// ����Ƿ���ڹ��ڵ�IP-MAC��Ŀ
		for(index1 = 0; index1 < MAX_ARP_SIZE; index1++)
		{
			if((arpcache.entries[index1].valid) && // ��������ʧЧ����Ŀ���϶���ʱ�ˣ���Ҫ�����ù�
			((time(NULL) - arpcache.entries[index1].added) > ARP_ENTRY_TIMEOUT)) // ����15��
			{
				arpcache.entries[index1].valid = 0; // �������鲻��ɾԪ�أ�ֻ�ܸ�valid
			}
		}

		// ���ARP���󷢳�ʱ���Ƿ�ʱ
		list_for_each_entry_safe(tmp_arp_entry, p, &(arpcache.req_list), list)
		{
			if(tmp_arp_entry->retries > ARP_REQUEST_MAX_RETRIES) // ����ط�����5����δ�յ�ARPӦ��
			{
				list_for_each_entry_safe(tmp_pkt, q, &(tmp_arp_entry->cached_packets), list)
				{
					pthread_mutex_unlock(&arpcache.lock);
					icmp_send_packet(tmp_pkt->packet, tmp_pkt->len, ICMP_DEST_UNREACH, ICMP_HOST_UNREACH); // �Ըö����µ����ݰ����λظ�ICMP����
					pthread_mutex_lock(&arpcache.lock);

					// ɾ���ȴ������ݰ�
					list_delete_entry(&(tmp_pkt->list));
					free(tmp_pkt->packet); // ���ݰ�ҲҪɾ��
					free(tmp_pkt); // cached_pkt ������append��������malloc�����ģ�����Ҫfree��ȥ
				}

				list_delete_entry(&(tmp_arp_entry->list));
				free(tmp_arp_entry); // arp_req ������append��������malloc�ģ�ҲҪfree
			}
			else // �����ط�ARP REQUEST��Ϣ
			{
				arp_send_request(tmp_arp_entry->iface, tmp_arp_entry->ip4);
				tmp_arp_entry->retries++;
				tmp_arp_entry->sent = time(NULL);
				// ���Դ���+1
			}
			
		}

		pthread_mutex_unlock(&arpcache.lock);
	}

	return NULL;
}
