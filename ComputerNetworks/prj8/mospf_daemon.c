#include "mospf_daemon.h"
#include "mospf_proto.h"
#include "mospf_nbr.h"
#include "mospf_database.h"

#include "ip.h"

#include "list.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

extern ustack_t *instance;

pthread_mutex_t mospf_lock;

void mospf_init()
{
	pthread_mutex_init(&mospf_lock, NULL);

	instance->area_id = 0;
	// get the ip address of the first interface
	iface_info_t *iface = list_entry(instance->iface_list.next, iface_info_t, list);
	instance->router_id = iface->ip;
	instance->sequence_num = 0;
	instance->lsuint = MOSPF_DEFAULT_LSUINT;

	iface = NULL;
	list_for_each_entry(iface, &instance->iface_list, list) {
		iface->helloint = MOSPF_DEFAULT_HELLOINT;
		init_list_head(&iface->nbr_list);
	}

	init_mospf_db();
}

void *sending_mospf_hello_thread(void *param);
void *sending_mospf_lsu_thread(void *param);
void *checking_nbr_thread(void *param);
void *checking_database_thread(void *param);

void send_lsu();

void mospf_run()
{
	pthread_t hello, lsu, nbr, db;
	pthread_create(&hello, NULL, sending_mospf_hello_thread, NULL);
	pthread_create(&lsu, NULL, sending_mospf_lsu_thread, NULL);
	pthread_create(&nbr, NULL, checking_nbr_thread, NULL);
	pthread_create(&db, NULL, checking_database_thread, NULL);
}

void *sending_mospf_hello_thread(void *param)
{
	//fprintf(stdout, "TODO: send mOSPF Hello message periodically.\n");
	int hello_pac_len = ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE + MOSPF_HELLO_SIZE;
	iface_info_t * iface_p, q;
	iface_p = NULL;
	q = NULL;

	while(1)
	{
		pthread_mutex_lock(&mospf_lock);

		list_for_each_entry_safe(iface_p, q, &(instance->iface_list), list)
		{
			char * packet = (char *)malloc(sizeof(char) * hello_pac_len);
			memset(packet, 0, hello_pac_len);

			struct ether_header * eth_hdr_p = (struct ether_header *)packet;
			struct iphdr * ip_hdr_p = (struct iphdr *)(packet + ETHER_HDR_SIZE);
			struct mospf_hdr * mospf_hdr_p = (struct mospf_hdr *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);
			struct mospf_hello * mospf_hello_p = (struct mospf_hello *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE);

			// init ether header
			memset((void *)(eth_hdr_p->ether_dhost), mospf_mac_addr_allrouters, ETH_ALEN);
			memset((void *)(eth_hdr_p->ether_shost), iface_p->mac, ETH_ALEN);
			eth_hdr_p->ether_type = htons(ETH_P_IP);

			// init ip header
			ip_init_hdr(ip_hdr_p, iface_p->ip, MOSPF_ALLSPFRouters, (IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE + MOSPF_HELLO_SIZE), IPPROTO_MOSPF);
			
			// init mOSPF header
			mospf_init_hdr(mospf_hdr_p, MOSPF_TYPE_HELLO, (MOSPF_HDR_SIZE + MOSPF_HELLO_SIZE), instance->router_id, instance->area_id);
		
			// init mOSPF hello packet
			mospf_init_hello(mospf_hello_p, iface_p->mask);

			// set checksum
			mospf_hdr_p->checksum = mospf_checksum(mospf_hdr_p);

			// send packet
			iface_send_packet(iface_p, packet, hello_pac_len);

		}
		pthread_mutex_unlock(&mospf_lock);
		sleep(MOSPF_DEFAULT_HELLOINT);
	}
	return NULL;
}

void *checking_nbr_thread(void *param)
{
	// fprintf(stdout, "TODO: neighbor list timeout operation.\n");

	iface_info_t * iface_p, *p;
	mospf_nbr_t * nbr_p, * q;
	int modified;

	while(1)
	{
		modified = 0;
		iface_p = NULL;
		p = NULL;
		nbr = NULL;
		q = NULL;

		pthread_mutex_lock(&mospf_lock);

		list_for_each_entry_safe(iface_p, p, &(instance->iface_list), list)
		{
			list_for_each_entry_safe(nbr_p, q, &(iface_p->nbr_list), list)
			{
				nbr_p->alive --;
				if(nbr_p->alive <= 0) // ����δ���¸��ھӵ�
				{
					list_delete_entry(&(nbr_p->list));
					free(nbr_p);
					iface_p->num_nbr--;
					modified = 1;
				}
			}
		}

		if(modified) // ���ڵ��ھ��б����䶯��ʱ��
		{
			// ��ÿ���ھӽڵ㷢����·״̬��Ϣ
			send_lsu();
		}

		
		pthread_mutex_unlock(&mospf_lock);

		sleep(1); // ÿһ�붼���һ�£������ŷ���alive����-1

	}
	return NULL;
}

void *checking_database_thread(void *param)
{
	// fprintf(stdout, "TODO: link state database timeout operation.\n");
	mospf_db_entry_t * db_entry_p, *q;
	while(1)
	{
		pthread_mutex_lock(&mospf_lock);

		list_for_each_entry_safe(db_entry_p, q, &mospf_db, list)
		{
			db_entry_p->alive --;
			if(db_entry_p->alive <= 0)
			{
				list_delete_entry(&(db_entry_p->list));
				free(db_entry_p);
				????????????
			}
		}

		pthread_mutex_unlock(&mospf_lock);
		sleep(1);// ÿһ�붼���һ�£������ŷ���alive����-1
	}

	return NULL;
}

void handle_mospf_hello(iface_info_t *iface, const char *packet, int len)
{
	// fprintf(stdout, "TODO: handle mOSPF Hello message.\n");
	// struct ether_header * eth_hdr_p = (struct ether_header *)packet;
	struct iphdr * ip_hdr_p = (struct iphdr *)(packet + ETHER_HDR_SIZE);
	struct mospf_hdr * mospf_hdr_p = (struct mospf_hdr *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);
	struct mospf_hello * mospf_hello_p = (struct mospf_hello *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE);

	mospf_nbr_t * nbr_p, * q;
	nbr_p = NULL; 
	q = NULL;
	int find = 0;


	pthread_mutex_lock(&mospf_lock);

	list_for_each_entry_safe(nbr_p, q, &(iface->nbr_list), list)
	{
		if(nbr_p->nbr_id == ntohl(mospf_hdr_p->rid))
		{
			find = 1;
			nbr_p->alive = 3 * MOSPF_DEFAULT_HELLOINT;
			break;
		}
	}

	if(find == 0) // not found
	{
		// init a new neighbor node
		mospf_nbr_t * new_nbr_p = (mospf_nbr_t *)malloc(sizeof(mospf_nbr_t));
		new_nbr_p->nbr_id = ntohl(mospf_hdr_p->rid);
		new_nbr_p->nbr_ip = ntohl(ip_hdr_p->saddr);
		new_nbr_p->nbr_mask = ntohl(mospf_hello_p->mask);
		new_nbr_p->alive = 3 * MOSPF_DEFAULT_HELLOINT;

		// add it to the neighbor list
		list_add_tail(new_nbr_p, &(iface->nbr_list));
		iface->num_nbr ++;

		// �����ھ��б����䶯����Ҫ����lsu��Ϣ
		send_lsu();
	}

	pthread_mutex_unlock(&mospf_lock);
}

void send_lsu()
{
	iface_info_t *iface_p, *iface_q;
	mospf_nbr_t *nbr_p, *nbr_q;
	char *packet_raw = NULL;
	char *packet_send = NULL;
	int len = 0;
	int neighbor_num = 0;
	int index1 = 0;
	iface_p = NULL;
	iface_q = NULL;
	nbr_p = NULL;
	nbr_q = NULL;

	list_for_each_entry_safe(iface_p, iface_q, &(instance->iface_list), list)
	{
		if(iface_p->num_nbr == 0) // û���ھ�Ҳ��1���ھӣ���ʱ�ھӽڵ�ID=0
		{
			neighbor_num += 1;
		}
		else
		{
			neighbor_num += iface_p->num_nbr;
		}
		
	}

	len = ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE + MOSPF_LSU_SIZE + MOSPF_LSA_SIZE * neighbor_num;
	packet_raw = (char *)malloc(len);
	struct mospf_lsa *lsa_arr = (struct mospf_lsa *)(packet_raw + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE + MOSPF_LSU_SIZE);

	// �ȳ�ʼ�����ݰ��ĺ�벿��
	list_for_each_entry_safe(iface_p, iface_q, &(instance->iface_list), list)
	{
		if(iface_p->num_nbr == 0) // û���ھ�Ҳ��1���ھӣ���ʱ�ھӽڵ�ID=0
		{
			lsa_arr[index1].network = htonl(iface_p->ip & iface_p->mask);
			lsa_arr[index1].mask    = htonl(iface_p->mask);
			lsa_arr[index1].rid     = 0;
		}
		else
		{
			list_for_each_entry_safe(nbr_p, nbr_q, &(iface_p->nbr_list), list)
			{
				lsa_arr[index1].network = htonl(nbr_p->nbr_ip & nbr_p->nbr_mask);
				lsa_arr[index1].mask    = htonl(nbr_p->mask);
				lsa_arr[index1].rid     = htonl(nbr_p->nbr_id);				
			}
		}
		index1++;
	}

	//Ȼ���ٳ�ʼ��ǰ�벿�ֲ���������
	list_for_each_entry_safe(iface_p, iface_q, &(instance->iface_list), list)
	{
		list_for_each_entry_safe(nbr_p, nbr_q, &(iface_p->nbr_list), list)
		{
			packet_send = (char *)malloc(len);
			memcpy((void *)packet_send, (void *)packet_raw, len);

			struct iphdr *ip_hdr_p        = (struct iphdr *    )(packet_send + ETHER_HDR_SIZE);
			struct mospf_hdr *mospf_hdr_p = (struct mospf_hdr *)(packet_send + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);
			struct mospf_lsu *mospf_lsu_p = (struct mospf_lsu *)(packet_send + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE);

			// init lsu
			mospf_init_lsu(mospf_lsu_p, neighbor_num);

			// init mospf_hdr
			mospf_init_hdr(mospf_hdr_p, MOSPF_TYPE_LSU, (u16)(len - ETHER_HDR_SIZE - IP_BASE_HDR_SIZE), instance->router_id, instance->area_id);
			mospf_hdr_p->checksum = mospf_checksum(mospf_hdr_p);

			// init ip_hdr
			ip_init_hdr(ip_hdr_p, iface_p->ip, nbr_p->nbr_ip, (u16)(len - ETHER_HDR_SIZE), IPPROTO_MOSPF);		
		
			ip_send_packet(packet_send, len);
		}
	}

	free(packet_raw); // ԭʼ��û����ȥ��û����
	instance->sequence_num ++; // ÿ��������·״̬��Ϣʱ+1
}

void *sending_mospf_lsu_thread(void *param)
{
	// fprintf(stdout, "TODO: send mOSPF LSU message periodically.\n");

	while(1)
	{
		pthread_mutex_lock(&mospf_lock);
		send_lsu();
		pthread_mutex_unlock(&mospf_unlock);

		sleep(MOSPF_DEFAULT_LSUINT);
	}

	return NULL;
}

void handle_mospf_lsu(iface_info_t *iface, char *packet, int len)
{
	// fprintf(stdout, "TODO: handle mOSPF LSU message.\n");
	mospf_db_entry_t * db_entry_p, *q;
	struct mospf_hdr *mospf_hdr_p = (struct mospf_hdr *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE); 
	struct mospf_lsu *lsu_pac_p = (struct mospf_lsu *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE);
	struct mospf_lsa *lsa_pac_p = (struct mospf_lsa *)(packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + MOSPF_HDR_SIZE + MOSPF_LSU_SIZE);

	int find = 0;
	int index1;
	db_entry_p = NULL;
	q = NULL;

	pthread_mutex_lock(&mospf_lock);

	list_for_each_entry_safe(db_entry_p, q, &mospf_db, list)
	{
		if(db_entry_p->rid == ntohl(mospf_hdr_p->rid))
		{
			find = 1;
			db_entry_p->alive = MOSPF_DATABASE_TIMEOUT;
			break;
		}
	}

	if(!find) // û�ҵ�
	{
		db_entry_p = (mospf_db_entry_t *)malloc(sizeof(mospf_db_entry_t));
		memset(db_entry_p, 0, sizeof(mospf_db_entry_t));
		list_add_tail(&(db_entry_p->list), &mospf_db);
	}
	else if(db_entry_p->seq < ntohs(lsu_pac_p->seq)) // �ҵ��ˣ��������ȶȱ��µĵ�
	{
		free(db_entry_p->array); // �����ɵĶ��У����֮��ֱ�������ݰ��еĶ����滻���ܻ����Ұָ��
	}

	if (!find || (db_entry_p->seq < ntohs(lsu_pac_p->seq)))
	{
		// copy lsu
		db_entry_p->rid   = ntohl(mospf_hdr_p->rid);
		db_entry_p->seq   = ntohs(lsu_pac_p->seq);
		db_entry_p->nadv  = ntohl(lsu_pac_p->nadv);
		db_entry_p->alive = MOSPF_DATABASE_TIMEOUT;

		// copy lsa
		db_entry_p->array = (struct mospf_lsa *)malloc(MOSPF_LSA_SIZE * db_entry_p->nadv);
		for(index1 = 0; index1 < db_entry_p->nadv; index1++)
		{
			db_entry_p->array[index1].network = ntohl(lsa_pac_p[index1].network);
			db_entry_p->array[index1].mask 	  = ntohl(lsa_pac_p[index1].mask);
			db_entry_p->array[index1].rid     = ntohl(lsa_pac_p[index1].rid);
		}
	}

	
	
	lsu_pac_p->ttl--;
	if(lsu_pac_p->ttl > 0) // ��������lsu��Ϣת��������
	{
		iface_info_t *iface_p, *iface_q;
		mospf_nbr_t *nbr_p, *nbr_q;
		char * new_packet = NULL;

		iface_p = NULL;
		iface_q = NULL;
		nbr_p = NULL;
		nbr_q = NULL;

		list_for_each_entry_safe(iface_p, iface_q, &(instance->iface_list), list)
		{
			if(iface_p->index == iface->index)
				continue;
			
			list_for_each_entry_safe(nbr_p, nbr_q, &(iface_p->nbr_list), list)
			{
				new_packet = (char *)malloc(len);
				memcpy((void *)new_packet, (void *)packet, len);
				
				// ֮ǰ��ip send packet��������Ӧ��ʵ������д��̫����ͷ�Ĺ��ܣ������������̫����ͷ�ĳ�ʼ����
				struct iphdr *new_ip_hdr_p = (struct iphdr *)(new_packet + ETHER_HDR_SIZE);
				struct mospf_hdr *new_mospf_hdr_p = (struct mospf_hdr *)(new_packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE);
				
				new_mospf_hdr_p->checksum = mospf_checksum(new_mospf_hdr_p);
				ip_init_hdr(new_ip_hdr_p, iface_p->ip, nbr_p->nbr_ip, (len - ETHER_HDR_SIZE), IPPROTO_MOSPF);
				
				ip_send_packet(new_packet, len);
			}
		}
	}

	?????????????????????????????????

	pthread_mutex_unlock(&mospf_lock);
}

void handle_mospf_packet(iface_info_t *iface, char *packet, int len)
{
	struct iphdr *ip = (struct iphdr *)(packet + ETHER_HDR_SIZE);
	struct mospf_hdr *mospf = (struct mospf_hdr *)((char *)ip + IP_HDR_SIZE(ip));

	if (mospf->version != MOSPF_VERSION) {
		log(ERROR, "received mospf packet with incorrect version (%d)", mospf->version);
		return ;
	}
	if (mospf->checksum != mospf_checksum(mospf)) {
		log(ERROR, "received mospf packet with incorrect checksum");
		return ;
	}
	if (ntohl(mospf->aid) != instance->area_id) {
		log(ERROR, "received mospf packet with incorrect area id");
		return ;
	}

	switch (mospf->type) {
		case MOSPF_TYPE_HELLO:
			handle_mospf_hello(iface, packet, len);
			break;
		case MOSPF_TYPE_LSU:
			handle_mospf_lsu(iface, packet, len);
			break;
		default:
			log(ERROR, "received mospf packet with unknown type (%d).", mospf->type);
			break;
	}
}
