#include "stp.h"

#include "base.h"
#include "ether.h"
#include "utils.h"
#include "types.h"
#include "packet.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <signal.h>

stp_t *stp;

const u8 eth_stp_addr[] = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x01 };

static bool stp_is_root_switch(stp_t *stp) // 节点为根节点的判断条件
{
	return stp->designated_root == stp->switch_id;
}

static bool stp_port_is_designated(stp_port_t *p) // 端口为指定端口的判断条件
{
	return p->designated_switch == p->stp->switch_id &&
		p->designated_port == p->port_id;
}

static const char *stp_port_state(stp_port_t *p) // 推断端口状态
{
	if (p->stp->root_port && \
			p->port_id == p->stp->root_port->port_id)
		return "ROOT";
	else if (p->designated_switch == p->stp->switch_id &&
		p->designated_port == p->port_id)
		return "DESIGNATED";
	else
		return "ALTERNATE";
}

static void stp_port_send_packet(stp_port_t *p, void *stp_msg, int msg_len) // 输入参数p代表目的端口
{
	int pkt_len = ETHER_HDR_SIZE + LLC_HDR_SIZE + msg_len;
	char *pkt = malloc(pkt_len);

	// ethernet header
	struct ether_header *eth = (struct ether_header *)pkt;
	memcpy(eth->ether_dhost, eth_stp_addr, 6);
	memcpy(eth->ether_shost, p->iface->mac, 6);
	eth->ether_type = htons(pkt_len - ETHER_HDR_SIZE);

	// LLC header
	struct llc_header *llc = (struct llc_header *)(pkt + ETHER_HDR_SIZE);
	llc->llc_dsap = LLC_DSAP_SNAP;
	llc->llc_ssap = LLC_SSAP_SNAP;
	llc->llc_cntl = LLC_CNTL_SNAP;

	memcpy(pkt + ETHER_HDR_SIZE + LLC_HDR_SIZE, stp_msg, msg_len);

	iface_send_packet(p->iface, pkt, pkt_len);
}

static void stp_port_send_config(stp_port_t *p) // 发送stp配置信息
{
	stp_t *stp = p->stp;
	bool is_root = stp_is_root_switch(stp);
	if (!is_root && !stp->root_port)  // 不是根节点，也没记录根端口
	{
		return;
	}

	struct stp_config config; // PPT 20页的结构，注意，在这里config的信息经过了hton系列函数的转换，用的时候要变回来
	memset(&config, 0, sizeof(config));
	config.header.proto_id = htons(STP_PROTOCOL_ID);
	config.header.version = STP_PROTOCOL_VERSION;
	config.header.msg_type = STP_TYPE_CONFIG;
	config.flags = 0;
	config.root_id = htonll(stp->designated_root);
	config.root_path_cost = htonl(stp->root_path_cost);
	config.switch_id = htonll(stp->switch_id);
	config.port_id = htons(p->port_id);
	config.msg_age = htons(0);
	config.max_age = htons(STP_MAX_AGE);
	config.hello_time = htons(STP_HELLO_TIME);
	config.fwd_delay = htons(STP_FWD_DELAY);

	// log(DEBUG, "port %s send config packet.", p->port_name);
	stp_port_send_packet(p, &config, sizeof(config));
}

static void stp_send_config(stp_t *stp)
{
	for (int i = 0; i < stp->nports; i++) 
	{
		stp_port_t *p = &stp->ports[i];
		if (stp_port_is_designated(p)) // 如果本节点的这个端口是指定端口
		{
			stp_port_send_config(p); // 就发送config
		}
	}
}


static void stp_handle_hello_timeout(void *arg) // 定时器结束，发送config
{
	// log(DEBUG, "hello timer expired, now = %llx.", time_tick_now());

	stp_t *stp = arg;
	stp_send_config(stp);
	stp_start_timer(&stp->hello_timer, time_tick_now()); // 又开始计时
}

static void stp_port_init(stp_port_t *p) // 端口初始化
{
	stp_t *stp = p->stp;

	p->designated_root = stp->designated_root;
	p->designated_switch = stp->switch_id;
	p->designated_port = p->port_id;
	p->designated_cost = stp->root_path_cost;
}

void *stp_timer_routine(void *arg)
{
	while (true) {
		long long int now = time_tick_now();

		pthread_mutex_lock(&stp->lock);

		stp_timer_run_once(now);

		pthread_mutex_unlock(&stp->lock);

		usleep(100);
	}

	return NULL;
}


bool stp_cfg_prior_cmp(stp_port_t *self_p, struct stp_config *recv_cfg) // 比较端口自己的config和收到的config的优先级
// true表示自己的优先级高，false表示收到的优先级高
// 见stp_port_send_config函数，使用config的整型数据时需要用ntoh系列函数转换字节序变成正常的字节序
{
	// 如果根节点ID不同，小的ID优先级更高
	if(self_p->designated_root != ntohll(recv_cfg->root_id)) 
	{
		return (self_p->designated_root < ntohll(recv_cfg->root_id))? true : false;
	}
	// 根节点ID相同的情况下，比较开销，开销小的优先级更高
	else if(self_p->designated_cost != ntohl(recv_cfg->root_path_cost)) 
	{
		return (self_p->designated_cost < ntohl(recv_cfg->root_path_cost))? true : false;
	}
	// 根节点开销相同的情况下，比较两者到根节点上一跳的节点，上一跳节点ID小的优先级更高
	else if(self_p->designated_switch != ntohll(recv_cfg->switch_id))
	{
		return (self_p->designated_switch < ntohll(recv_cfg->switch_id))? true : false;
	}
	// 到根节点上一跳的节点相同的话，比较两者到根节点上一跳的端口，上一跳端口号小的优先级更高
	else // 根据PPT 16页的描述，这是最后default的情况了，没有更多的
	{
		return (self_p->designated_port < ntohs(recv_cfg->port_id))? true : false;
	}

	
}

bool stp_port_prior_cmp(stp_port_t *a, stp_port_t *b) // 比较端口的优先级
// true表示a的优先级高，false表示b的优先级高
// 比较逻辑模仿config的比较
{	
	// 如果根节点ID不同，小的ID优先级更高
	if(a->designated_root != b->designated_root) 
	{
		return (a->designated_root < b->designated_root)? true : false;
	}
	// 根节点ID相同的情况下，比较开销，开销小的优先级更高
	else if(a->designated_cost != b->designated_cost) 
	{
		return (a->designated_cost < b->designated_cost)? true : false;
	}
	// 根节点开销相同的情况下，比较两者到根节点上一跳的节点，上一跳节点ID小的优先级更高
	else if(a->designated_switch != b->designated_switch)
	{
		return (a->designated_switch < b->designated_switch)? true : false;
	}
	// 到根节点上一跳的节点相同的话，比较两者到根节点上一跳的端口，上一跳端口号小的优先级更高
	else // 根据PPT 16页的描述，这是最后default的情况了，没有更多的
	{
		return (a->designated_port < b->designated_port)? true : false;
	}
}

bool stp_stp_prior_cmp(stp_t *stp, stp_port_t *p) // 比较非指定端口本应持有的config和非指定端口目前config的优先级（用于更新非指定端口）
{
	// 如果根节点ID不同，小的ID优先级更高
	if(stp->designated_root != p->designated_root) 
	{
		return (stp->designated_root < p->designated_root)? true : false;
	}
	// 根节点ID相同的情况下，比较开销，开销小的优先级更高
	else if(stp->root_path_cost != p->designated_cost) 
	{
		return (stp->root_path_cost < p->designated_cost)? true : false;
	}
	// 根节点开销相同的情况下，比较两者到根节点上一跳的节点，上一跳节点ID小的优先级更高
	else if(stp->switch_id != p->designated_switch)
	{
		return (stp->switch_id < p->designated_switch)? true : false;
	}
	// 到根节点上一跳的节点相同的话，比较两者到根节点上一跳的端口，上一跳端口号小的优先级更高
	else // 根据PPT 16页的描述，这是最后default的情况了，没有更多的
	{
		return (p->port_id < p->designated_port)? true : false;
	}

}

static void stp_state_update(stp_t *stp)
{
	//bool no_root_port = false;
	stp_port_t * root_p = NULL;
	
	int index1;
	
	// 遍历所有端口，寻找根端口
	for (index1 = 0; index1 < stp->nports; index1++) 
	{
		stp_port_t *p = &stp->ports[index1];
		if (!stp_port_is_designated(p)) // 如果本节点的这个端口不是指定端口，且优先级是所有非指定端口里面最高的
		{
			if(root_p == NULL) // 如果指针此时为NULL，则说明目前还没有一个满足要求的端口
			{
				root_p = p; // 就不用比较优先级了，直接令这个端口为暂定的根端口
			}
			else if (stp_port_prior_cmp(p, root_p)) // 为真说明p的优先级高于root_p
			// 如果指针不为NULL，说明之前已经找到了至少一个满足要求的根端口，
			{// 现在本节点也满足条件，就需要和暂定的根端口进行比较
				/* code */
				root_p = p;
			}
			
		}
	}

	if (root_p == NULL) // 循环完了都没有1个，说明没有满足要求的端口，也就是没有根端口
	{
		// 则该节点设为根节点
		stp->root_port = NULL;
		stp->designated_root = stp->switch_id;
		stp->root_path_cost = 0;
	}
	else // 否则，选择通过root_port连接到根节点，更新节点状态
	{
		stp->root_port = root_p;
		stp->designated_root = root_p->designated_root;
		stp->root_path_cost = root_p->designated_cost + root_p->path_cost;
	}
	

}

static void stp_port_cfg_update(stp_t * stp) // 更新端口的config
{
	int index1;
	
	// 遍历所有端口
	for (index1 = 0; index1 < stp->nports; index1++)
	{
		stp_port_t *p = &stp->ports[index1];
		//判断该端口是否为指定端口
		if(stp_port_is_designated(p)) // 对于指定端口，更新其认为的根节点和路径开销
		{
			p->designated_root = stp->designated_root;
			p->designated_cost = stp->root_path_cost;
		}
		else if(stp_stp_prior_cmp(stp, p)) // 对于不是指定端口的端口，寻找潜在可能成为指定端口的端口
		{
			// 先将该端口改成指定端口
			p->designated_switch = stp->switch_id;
			p->designated_port = p->port_id;
			// 再按照上面if的方式修改它的参数
			p->designated_root = stp->designated_root;
			p->designated_cost = stp->root_path_cost;
		}
	}
}

static void stp_handle_config_packet(stp_t *stp, stp_port_t *p,
		struct stp_config *config)
// p：自己的端口；config：收到的config
{
	// TODO: handle config packet here
	// fprintf(stdout, "TODO: handle config packet here.\n");
	if(stp_cfg_prior_cmp(p, config)) // 自己的优先级更高，不改，并且自己返给对方一个config
	{
		stp_port_send_config(p); // 用自己的这个端口发送自己的config
	}
	else // 对方的config的优先级更高
	{
		// 将本端口的config替换为收到的config消息
		// 根据stp_port_send_config中设置config的方式，更新对应的部分:

		p->designated_root   = ntohll(config->root_id);
		p->designated_cost   = ntohl(config->root_path_cost);
		p->designated_switch = ntohll(config->switch_id);
		p->designated_port   = ntohs(config->port_id);


		// 更新节点状态
		stp_state_update(stp);

		// 更新剩余端口的config
		stp_port_cfg_update(stp);

		// 如果节点由根节点变为非根节点，停止hello定时器
		if(!stp_is_root_switch(stp))
		{
			stp_stop_timer(&(stp->hello_timer));
		}


		// 将更新后的Config从每个指定端口转发出去
		stp_send_config(stp);
	}
	
}

static void *stp_dump_state(void *arg)
{
#define get_switch_id(switch_id) (int)(switch_id & 0xFFFF) // 掩码操作，只拿最低16位
#define get_port_id(port_id) (int)(port_id & 0xFF) // 掩码操作，只拿最低8位

	pthread_mutex_lock(&stp->lock);

	bool is_root = stp_is_root_switch(stp);
	if (is_root) {
		log(INFO, "this switch is root."); 
	}
	else {
		log(INFO, "non-root switch, designated root: %04x, root path cost: %d.", \
				get_switch_id(stp->designated_root), stp->root_path_cost);
	}

	for (int i = 0; i < stp->nports; i++) {
		stp_port_t *p = &stp->ports[i];
		log(INFO, "port id: %02d, role: %s.", get_port_id(p->port_id), \
				stp_port_state(p));
		log(INFO, "\tdesignated ->root: %04x, ->switch: %04x, " \
				"->port: %02d, ->cost: %d.", \
				get_switch_id(p->designated_root), \
				get_switch_id(p->designated_switch), \
				get_port_id(p->designated_port), \
				p->designated_cost);
	}

	pthread_mutex_unlock(&stp->lock);

	exit(0);
}

static void stp_handle_signal(int signal)
{
	if (signal == SIGTERM) {
		log(DEBUG, "received SIGTERM, terminate this program.");
		
		pthread_t pid;
		pthread_create(&pid, NULL, stp_dump_state, NULL);
	}
}

void stp_init(struct list_head *iface_list)
{
	stp = malloc(sizeof(*stp));

	// set switch ID
	u64 mac_addr = 0;
	iface_info_t *iface = list_entry(iface_list->next, iface_info_t, list);
	for (int i = 0; i < sizeof(iface->mac); i++) {
		mac_addr <<= 8;
		mac_addr += iface->mac[i];
	}
	stp->switch_id = mac_addr | ((u64) STP_BRIDGE_PRIORITY << 48);

	stp->designated_root = stp->switch_id; // 节点认为自己是根节点
	stp->root_path_cost = 0;
	stp->root_port = NULL;

	stp_init_timer(&stp->hello_timer, STP_HELLO_TIME, \
			stp_handle_hello_timeout, (void *)stp);

	stp_start_timer(&stp->hello_timer, time_tick_now());

	stp->nports = 0;
	list_for_each_entry(iface, iface_list, list) // 初始化节点的各个端口
	{
		stp_port_t *p = &stp->ports[stp->nports];

		p->stp = stp;
		p->port_id = (STP_PORT_PRIORITY << 8) | (stp->nports + 1);
		p->port_name = strdup(iface->name);
		p->iface = iface;
		p->path_cost = 1;

		stp_port_init(p);

		// store stp port in iface for efficient access
		iface->port = p;

		stp->nports += 1;
	}

	pthread_mutex_init(&stp->lock, NULL);
	pthread_create(&stp->timer_thread, NULL, stp_timer_routine, NULL);

	signal(SIGTERM, stp_handle_signal);
}

void stp_destroy()
{
	pthread_kill(stp->timer_thread, SIGKILL);

	for (int i = 0; i < stp->nports; i++) {
		stp_port_t *port = &stp->ports[i];
		port->iface->port = NULL;
		free(port->port_name);
	}

	free(stp);
}

void stp_port_handle_packet(stp_port_t *p, char *packet, int pkt_len) // 端口处理数据包的逻辑
{
	stp_t *stp = p->stp;

	pthread_mutex_lock(&stp->lock);
	
	// protocol insanity check is omitted
	struct stp_header *header = (struct stp_header *)(packet + ETHER_HDR_SIZE + LLC_HDR_SIZE);

	// 根据报头判断报文种类，本实验只处理config类型
	if (header->msg_type == STP_TYPE_CONFIG) {
		stp_handle_config_packet(stp, p, (struct stp_config *)header);
	}
	else if (header->msg_type == STP_TYPE_TCN) {
		log(ERROR, "TCN packet is not supported in this lab.");
	}
	else {
		log(ERROR, "received invalid STP packet.");
	}

	pthread_mutex_unlock(&stp->lock);
}
