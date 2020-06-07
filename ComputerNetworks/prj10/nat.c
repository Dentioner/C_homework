#include "nat.h"
#include "ip.h"
#include "icmp.h"
#include "tcp.h"
#include "rtable.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_FILE_LENGTH 1000

static struct nat_table nat;

// get the interface from iface name
static iface_info_t *if_name_to_iface(const char *if_name)
{
	iface_info_t *iface = NULL;
	list_for_each_entry(iface, &instance->iface_list, list) {
		if (strcmp(iface->name, if_name) == 0)
			return iface;
	}

	log(ERROR, "Could not find the desired interface according to if_name '%s'", if_name);
	return NULL;
}

// determine the direction of the packet, DIR_IN / DIR_OUT / DIR_INVALID
static int get_packet_direction(char *packet)
{
	// fprintf(stdout, "TODO: determine the direction of this packet.\n");
	// struct ether_header * eth_hdr = NULL;
	struct iphdr * ip_hdr = NULL;
	rt_entry_t *src_rtb_entry = NULL;
	rt_entry_t *dst_rtb_entry = NULL;

	u32 saddr, daddr;
	ip_hdr = packet_to_ip_hdr(packet);
	saddr = ntohl(ip_hdr->saddr);
	daddr = ntohl(ip_hdr->daddr);

	src_rtb_entry = longest_prefix_match(saddr);
	dst_rtb_entry = longest_prefix_match(daddr);

	if((src_rtb_entry->iface == nat.internal_iface) && (dst_rtb_entry->iface == nat.external_iface)) // ��Դ��ַΪ�ڲ���ַ����Ŀ�ĵ�ַΪ�ⲿ��ַʱ������ΪDIR_OUT
	{
		return DIR_OUT;
	}
	else if((src_rtb_entry->iface == nat.external_iface) && (daddr == nat.external_iface->ip)) // ��Դ��ַΪ�ⲿ��ַ����Ŀ�ĵ�ַΪexternal_iface��ַʱ������ΪDIR_IN
	{
		return DIR_IN;
	}
	else
	{
		return DIR_INVALID;
	}
	// return DIR_INVALID;
}

u16 assign_external_port()
{
	u16 index1;
	for(index1 = NAT_PORT_MIN; index1 < NAT_PORT_MAX; index1++)
	{
		if(nat.assigned_ports[index1] == 0)
		{
			nat.assigned_ports[index1] = 1;
			return index1;
		}
	}

	printf("Error. Cannot assign new port.\n");
	return 0;
}

// do translation for the packet: replace the ip/port, recalculate ip & tcp
// checksum, update the statistics of the tcp connection
void do_translation(iface_info_t *iface, char *packet, int len, int dir)
{
	// fprintf(stdout, "TODO: do translation for this packet.\n");
	struct iphdr *ip_hdr = NULL;
	struct tcphdr *tcp_hdr = NULL;
	struct nat_mapping *map_p, *map_q;
	struct dnat_rule *dnat_p, *dnat_q;

	u32 saddr, daddr;
	u16 sport, dport;
	u8 hash_index;
	u32 key;
	u8 fin, ack, rst;

	int map_find = 0;
	int rule_find = 0;

	ip_hdr = packet_to_ip_hdr(packet);
	tcp_hdr = packet_to_tcp_hdr(packet);

	saddr = ntohl(ip_hdr->saddr);
	daddr = ntohl(ip_hdr->daddr);
	sport = ntohs(tcp_hdr->sport);
	dport = ntohs(tcp_hdr->dport);

	pthread_mutex_lock(&(nat.lock));


	// ���ַ���ÿ����֧�ڲ�ѯnat�Ƿ�������ӳ���ϵ�������е�ַ����
	
	if(dir == DIR_IN) 
	{
		// DIR_IN��rmt ip/port��src
		key = saddr + sport;
		hash_index = hash8((char *)&key, sizeof(key));

		map_find = 0;
		list_for_each_entry_safe(map_p, map_q, &(nat.nat_mapping_list[hash_index]), list)
		{
			if((map_p->remote_ip == saddr) && (map_p->remote_port == sport) && (map_p->external_ip == daddr) && (map_p->external_port == dport))
			{
				map_find = 1;
				break;
			}
		}

		if(map_find == 0) // DNAT�����û�ҵ������չ������µ�����ӳ��
		{
			// �Ȳ�ѯ�����Ƿ��к��ʵ�
			rule_find = 0;
			
			list_for_each_entry_safe(dnat_p, dnat_q, &(nat.rules), list)
			{
				if((dnat_p->external_ip == daddr) && (dnat_p->external_port == dport))
				{
					rule_find = 1;
					break;
				}
			}

			if(rule_find == 0) // û�ҵ����ʵĹ���ģ��nat_translate_packet����
			{
				log(ERROR, "invalid packet direction, drop it.");
				icmp_send_packet(packet, len, ICMP_DEST_UNREACH, ICMP_HOST_UNREACH);
				free(packet); // �����double free��
				return ;
			}
			else // �ҵ��ˣ������ӵ�������
			{
				struct nat_mapping * new_map = (struct nat_mapping *)malloc(sizeof(struct nat_mapping));
				memset((void *)new_map, 0, sizeof(struct nat_mapping));
				
				new_map->remote_ip = saddr;
				new_map->remote_port = sport;
				new_map->external_ip = dnat_p->external_ip;
				new_map->external_port = dnat_p->external_port;
				new_map->internal_ip = dnat_p->internal_ip;
				new_map->internal_port = dnat_p->internal_port;
				new_map->update_time = TCP_ESTABLISHED_TIMEOUT;
				// new_map->conn = 0;
				list_add_tail(&(new_map->list), &(nat.nat_mapping_list[hash_index]));

				map_p = new_map; // �����ѭ����ָ��ָ������µ�
				nat.assigned_ports[dnat_p->external_port] = 1; // ����˿ڱ�������
			}

		}
		// else: ��ϣ�����ҵ��ˣ�map_find == 1
		
		// ��ַ����
		ip_hdr->daddr = htonl(map_p->internal_ip);
		tcp_hdr->dport = htons(map_p->internal_port);

		// ����tcp״̬
		fin = tcp_hdr->flags & TCP_FIN;
		ack = tcp_hdr->flags & TCP_ACK;
		rst = tcp_hdr->flags & TCP_RST;

		if(ack)
		{
			map_p->conn.external_ack = ntohl(tcp_hdr->ack);
		}

		if (fin)
		{
			map_p->conn.external_fin = 1;
		}

		if (rst)
		{
			map_p->conn.external_fin = 1;
			map_p->conn.internal_fin = 1;
		}

		// map_p->conn.external_ack = ack | rst;
		// map_p->conn.external_fin = fin | rst; 
		// map_p->conn.internal_ack = ;
		// map_p->conn.internal_fin = ;
		map_p->conn.external_seq_end = ntohl(tcp_hdr->seq);
		// map_p->conn.internal_seq_end = ;

	}
	else if(dir == DIR_OUT)
	{
		// DIR_OUT��rmt ip/port��dst
		key = daddr + dport;
		hash_index = hash8((char *)&key, sizeof(key));

		map_find = 0;
		list_for_each_entry_safe(map_p, map_q, &(nat.nat_mapping_list[hash_index]), list)
		{
			if((map_p->remote_ip == daddr) && (map_p->remote_port == dport) && (map_p->internal_ip == saddr) && (map_p->internal_port == sport))
			{
				map_find = 1;
				break;
			}
		}

		if(map_find == 0) // SNAT�����û�ҵ��������µĹ���port��
		{
			struct nat_mapping * new_map = (struct nat_mapping *)malloc(sizeof(struct nat_mapping));
			memset((void *)new_map, 0, sizeof(struct nat_mapping));
			
			new_map->remote_ip = daddr;
			new_map->remote_port = dport;
			new_map->external_ip = nat.external_iface->ip;
			new_map->external_port = assign_external_port();
			new_map->internal_ip = saddr;
			new_map->internal_port = sport;
			new_map->update_time = TCP_ESTABLISHED_TIMEOUT;
			// new_map->conn = 0;
			list_add_tail(&(new_map->list), &(nat.nat_mapping_list[hash_index]));

			map_p = new_map; // �����ѭ����ָ��ָ������µ�
		}
		// else: ��ϣ�����ҵ��ˣ�map_find == 1

		// ��ַ����
		ip_hdr->saddr = htonl(map_p->external_ip);
		tcp_hdr->sport = htons(map_p->external_port);

		// ����tcp״̬
		fin = tcp_hdr->flags & TCP_FIN;
		ack = tcp_hdr->flags & TCP_ACK;
		rst = tcp_hdr->flags & TCP_RST;

		if(ack)
		{
			map_p->conn.internal_ack = ntohl(tcp_hdr->ack);
		}

		if (fin)
		{
			map_p->conn.internal_fin = 1;
		}

		if (rst)
		{
			map_p->conn.external_fin = 1;
			map_p->conn.internal_fin = 1;
		}

		map_p->conn.internal_seq_end = ntohl(tcp_hdr->seq);
	}
	else // �ж��Ƿ�Ϊinvalid���߼��Ѿ���ĸ����nat_translate_packetִ���ˣ���������²��ᵽ����
	{
		printf("Error. Impossible branch.\n");
		pthread_mutex_unlock(&(nat.lock));
		exit(1);
	}
	

	// ���¼���У���
	tcp_hdr->checksum = tcp_checksum(ip_hdr, tcp_hdr);
	ip_hdr->checksum = ip_checksum(ip_hdr);

	// �������ݰ�
	ip_send_packet(packet, len);

	pthread_mutex_unlock(&(nat.lock));
}

void nat_translate_packet(iface_info_t *iface, char *packet, int len)
{
	int dir = get_packet_direction(packet);
	if (dir == DIR_INVALID)  // PPT��7ҳ��invalid��֧
	{
		log(ERROR, "invalid packet direction, drop it.");
		icmp_send_packet(packet, len, ICMP_DEST_UNREACH, ICMP_HOST_UNREACH);
		free(packet);
		return ;
	}

	struct iphdr *ip = packet_to_ip_hdr(packet);
	if (ip->protocol != IPPROTO_TCP) // �������TCPЭ��
	{
		log(ERROR, "received non-TCP packet (0x%0hhx), drop it", ip->protocol);
		free(packet);
		return ;
	}

	do_translation(iface, packet, len, dir);
}

// check whether the flow is finished according to FIN bit and sequence number
// XXX: seq_end is calculated by `tcp_seq_end` in tcp.h
static int is_flow_finished(struct nat_connection *conn)
{
    return (conn->internal_fin && conn->external_fin) && \
            (conn->internal_ack >= conn->external_seq_end) && \
            (conn->external_ack >= conn->internal_seq_end);
}

// nat timeout thread: find the finished flows, remove them and free port
// resource
void *nat_timeout()
{
	int index1;
	struct nat_mapping *map_p, *map_q;
	while (1) 
	{
		// fprintf(stdout, "TODO: sweep finished flows periodically.\n");
		sleep(1);
		pthread_mutex_lock(&(nat.lock));

		for(index1 = 0, map_p = NULL, map_q = NULL; index1 < HASH_8BITS; index1++)
		{
			list_for_each_entry_safe(map_p, map_q, &(nat.nat_mapping_list[index1]), list)
			{
				if(is_flow_finished(&(map_p->conn))) // �Ĵλ��ֻ���RST���½���
				{
					list_delete_entry(&(map_p->list));
					nat.assigned_ports[map_p->external_port] = 0; // ���մ˶˿�

					free(map_p);
				}
				else
				{
					map_p->update_time--;
					if(map_p <= 0) // ����60s
					{
						list_delete_entry(&(map_p->list));
						nat.assigned_ports[map_p->external_port] = 0; // ���մ˶˿�

						free(map_p);
					}
				}
				
			}
		}

		pthread_mutex_unlock(&(nat.lock));
	}

	return NULL;
}

int parse_config(const char *filename) // ����config�ļ���
{
	// fprintf(stdout, "TODO: parse config file, including i-iface, e-iface (and dnat-rules if existing).\n");
	FILE *cfg;
	char buffer[MAX_FILE_LENGTH];
	char in_iface_name[MAX_FILE_LENGTH], ex_iface_name[MAX_FILE_LENGTH];
	iface_info_t *in_iface, *ex_iface;
	memset((void *)buffer, 0, MAX_FILE_LENGTH);
	memset((void *)in_iface_name, 0, MAX_FILE_LENGTH);
	memset((void *)ex_iface_name, 0, MAX_FILE_LENGTH);
	in_iface = NULL;
	ex_iface = NULL;


	cfg = fopen(filename, "r");
	
	if(cfg == NULL)
	{
		printf("Error. Do not have such file.\n");
		exit(1);
	}

	
	if (!strncmp("exp1.conf", filename, strlen("exp1.conf"))) // ����config1
	{
		u32 internal_n, internal_eth;
		u32 external_n, external_eth;
		// internal-iface: XXXX
		fscanf(cfg, "internal-iface: n%u-eth%u\n", &internal_n, &internal_eth);
		sprintf(in_iface_name, "n%u-eth%u", internal_n, internal_eth);

		// external-iface: XXXX
		fscanf(cfg, "external-iface: n%u-eth%u", &external_n, &external_eth);
		sprintf(ex_iface_name, "n%u-eth%u", external_n, external_eth);

		// get iface
		in_iface = if_name_to_iface(in_iface_name);
		ex_iface = if_name_to_iface(ex_iface_name);

		if(in_iface == NULL || ex_iface == NULL)
		{
			printf("Error in if_name_to_iface.\n");
			exit(1);
		}

		// add iface information in NAT
		nat.internal_iface = in_iface;
        nat.external_iface = ex_iface;

	}
	else if ((!strncmp("exp2.conf", filename, strlen("exp2.conf"))) || 
			(!strncmp("exp3_n1.conf", filename, strlen("exp3_n1.conf"))) ||
			(!strncmp("exp3_n2.conf", filename, strlen("exp3_n2.conf")))) // ����config2���Լ�����3������config
	{
		u32 internal_n, internal_eth;
		u32 external_n, external_eth;
		u32 ret = 0;
		u32 rule_num = 0;
		// u32 rmt_ip3, rmt_ip2, rmt_ip1, rmt_ip0, rmt_port;
		u32 ext_ip3, ext_ip2, ext_ip1, ext_ip0, ext_port;
		u32 int_ip3, int_ip2, int_ip1, int_ip0, int_port;
		struct dnat_rule * dnat_rule_p = NULL;

		// internal-iface: XXXX
		fscanf(cfg, "internal-iface: n%u-eth%u\n", &internal_n, &internal_eth);
		sprintf(in_iface_name, "n%u-eth%u", internal_n, internal_eth);

		// external-iface: XXXX
		fscanf(cfg, "external-iface: n%u-eth%u\n\n", &external_n, &external_eth); // ����һ�������ͬ������external����֮����2���ո�
		sprintf(ex_iface_name, "n%u-eth%u", external_n, external_eth);

		// get iface
		in_iface = if_name_to_iface(in_iface_name);
		ex_iface = if_name_to_iface(ex_iface_name);

		if(in_iface == NULL || ex_iface == NULL)
		{
			printf("Error in if_name_to_iface.\n");
			exit(1);
		}

		// add iface information in NAT
		nat.internal_iface = in_iface;
        nat.external_iface = ex_iface;


		// read rules
		printf("read config:\n");
		rule_num = 0;
		while(1)
		{
			ret = fscanf(cfg, "dnat-rules: %u.%u.%u.%u:%u -> %u.%u.%u.%u:%u\n", 
									&ext_ip3, &ext_ip2, &ext_ip1, &ext_ip0, &ext_port, 
									&int_ip3, &int_ip2, &int_ip1, &int_ip0, &int_port);
			
			if(ret == EOF)
			{
				break;
			}
			
			dnat_rule_p = (struct dnat_rule *)malloc(sizeof(struct dnat_rule));
			dnat_rule_p->external_ip = (ext_ip3 << 24) | (ext_ip2 << 16) | (ext_ip1 << 8) | (ext_ip0);
			dnat_rule_p->external_port = (u16)ext_port;
			dnat_rule_p->internal_ip = (int_ip3 << 24) | (int_ip2 << 16) | (int_ip1 << 8) | (int_ip0);
			dnat_rule_p->internal_port = (u16)int_port;

			list_add_tail(&(dnat_rule_p->list), &(nat.rules));

			printf("dnat-rules: %u.%u.%u.%u:%u -> %u.%u.%u.%u:%u\n",
					ext_ip3, ext_ip2, ext_ip1, ext_ip0, ext_port,
					int_ip3, int_ip2, int_ip1, int_ip0, int_port);

			rule_num ++;
		} 
	}
	else
	{
		printf("Error. Do not have such file.\n");
		exit(1);
	}
	
	
	fclose(cfg);

	return 0;
}

// initialize
void nat_init(const char *config_file)
{
	memset(&nat, 0, sizeof(nat)); // ��ʼ��nat����ڴ�����

	for (int i = 0; i < HASH_8BITS; i++)
		init_list_head(&nat.nat_mapping_list[i]); // ��ʼ��hash��

	init_list_head(&nat.rules); // ��ʼ��rule����ͷ

	// seems unnecessary
	memset(nat.assigned_ports, 0, sizeof(nat.assigned_ports));

	parse_config(config_file); // ����������������в���config_file

	pthread_mutex_init(&nat.lock, NULL);

	pthread_create(&nat.thread, NULL, nat_timeout, NULL);
}

void nat_exit()
{
	int index1;
	struct dnat_rule * dnat_rule_p, *dnat_rule_q;
	struct nat_mapping *map_p, *map_q;
	// fprintf(stdout, "TODO: release all resources allocated.\n");
	pthread_mutex_lock(&(nat.lock));

	// �ͷ�parse_config������ӵ�rule
	list_for_each_entry_safe(dnat_rule_p, dnat_rule_q, &(nat.rules), list)
	{
		list_delete_entry(&(dnat_rule_p->list));
		free(dnat_rule_p);
	}

	// �ͷŹ�ϣ��
	for(index1 = 0, map_p = NULL, map_q = NULL; index1 < HASH_8BITS; index1++)
	{
		list_for_each_entry_safe(map_p, map_q, &(nat.nat_mapping_list[index1]), list)
		{
			list_delete_entry(&(map_p->list));
			free(map_p);
		}
	}

	// �ɵ��������
	pthread_kill(nat.thread, SIGTERM);

	pthread_mutex_unlock(&(nat.lock));
}
