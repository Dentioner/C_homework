#include "tcp.h"
#include "tcp_hash.h"
#include "tcp_sock.h"
#include "tcp_timer.h"
#include "ip.h"
#include "rtable.h"
#include "log.h"

// TCP socks should be hashed into table for later lookup: Those which
// occupy a port (either by *bind* or *connect*) should be hashed into
// bind_table, those which listen for incoming connection request should be
// hashed into listen_table, and those of established connections should
// be hashed into established_table.

struct tcp_hash_table tcp_sock_table;
#define tcp_established_sock_table	tcp_sock_table.established_table
#define tcp_listen_sock_table		tcp_sock_table.listen_table
#define tcp_bind_sock_table			tcp_sock_table.bind_table


// debug
// struct tcp_sock *debug_pointer;




inline void tcp_set_state(struct tcp_sock *tsk, int state)
{
	log(DEBUG, IP_FMT":%hu switch state, from %s to %s.", \
			HOST_IP_FMT_STR(tsk->sk_sip), tsk->sk_sport, \
			tcp_state_str[tsk->state], tcp_state_str[state]);
	tsk->state = state;
}

// init tcp hash table and tcp timer
void init_tcp_stack()
{
	for (int i = 0; i < TCP_HASH_SIZE; i++)
		init_list_head(&tcp_established_sock_table[i]);

	for (int i = 0; i < TCP_HASH_SIZE; i++)
		init_list_head(&tcp_listen_sock_table[i]);

	for (int i = 0; i < TCP_HASH_SIZE; i++)
		init_list_head(&tcp_bind_sock_table[i]);

	pthread_t timer;
	pthread_create(&timer, NULL, tcp_timer_thread, NULL);
}

// allocate tcp sock, and initialize all the variables that can be determined
// now
struct tcp_sock *alloc_tcp_sock()
{
	struct tcp_sock *tsk = malloc(sizeof(struct tcp_sock));

	memset(tsk, 0, sizeof(struct tcp_sock));

	tsk->state = TCP_CLOSED;
	tsk->rcv_wnd = TCP_DEFAULT_WINDOW;

	init_list_head(&tsk->list);
	init_list_head(&tsk->listen_queue);
	init_list_head(&tsk->accept_queue);

	tsk->rcv_buf = alloc_ring_buffer(tsk->rcv_wnd);

	tsk->wait_connect = alloc_wait_struct();
	tsk->wait_accept = alloc_wait_struct();
	tsk->wait_recv = alloc_wait_struct();
	tsk->wait_send = alloc_wait_struct();

	tsk->wait_ofo = alloc_wait_struct(); // prj13 added

	init_list_head(&tsk->send_buf); // prj13 added
	init_list_head(&tsk->rcv_ofo_buf); // prj13 added

	tsk->cwnd_state = CWND_OPEN; // prj14 added
	tsk->dupacks = 0; // prj14 added
	tsk->cwnd = 1; // prj14 added
	tsk->ssthresh = INIT_SSTHRESH; // prj14 added

	pthread_mutex_init(&(tsk->cwnd_lock), NULL);

	record_index = 0; // prj14 added

	return tsk;
}

// release all the resources of tcp sock
//
// To make the stack run safely, each time the tcp sock is refered (e.g. hashed), 
// the ref_cnt is increased by 1. each time free_tcp_sock is called, the ref_cnt
// is decreased by 1, and release the resources practically if ref_cnt is
// decreased to zero.
void free_tcp_sock(struct tcp_sock *tsk)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);

	tsk->ref_cnt--;

	if(tsk->ref_cnt <= 0) // 倒着把alloc_tcp_sock()执行一遍
	{
		free_wait_struct(tsk->wait_connect);
		free_wait_struct(tsk->wait_accept);
		free_wait_struct(tsk->wait_recv);
		free_wait_struct(tsk->wait_send);

		free_wait_struct(tsk->wait_ofo); // prj13 added

		free_ring_buffer(tsk->rcv_buf);

		// printf("debug: before free(tsk).\n");

		free(tsk);

		// printf("debug: after free(tsk).\n");
	}

	return;
}

// lookup tcp sock in established_table with key (saddr, daddr, sport, dport)
struct tcp_sock *tcp_sock_lookup_established(u32 saddr, u32 daddr, u16 sport, u16 dport)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	int hash = tcp_hash_function(saddr, daddr, sport, dport); 
	struct tcp_sock *tmp_p, *tmp_q;
	list_for_each_entry_safe(tmp_p, tmp_q, &(tcp_established_sock_table[hash]), hash_list)
	{
		if (saddr == tmp_p->sk_sip && daddr == tmp_p->sk_dip && sport == tmp_p->sk_sport && dport == tmp_p->sk_dport)
		{
			return tmp_p;
		}
	}

	return NULL;
}

// lookup tcp sock in listen_table with key (sport)
//
// In accordance with BSD socket, saddr is in the argument list, but never used.
struct tcp_sock *tcp_sock_lookup_listen(u32 saddr, u16 sport)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	int hash = tcp_hash_function(0, 0, sport, 0); 
	struct tcp_sock *tmp_p, *tmp_q;
	list_for_each_entry_safe(tmp_p, tmp_q, &(tcp_listen_sock_table[hash]), hash_list)
	{
		if (sport == tmp_p->sk_sport)
		{
			return tmp_p;
		}
	}

	// debug
	/*
	printf("debug:in lookup listen.\n");
	printf("debug:hash=%d\n", hash);
	printf("empty:%d\n", list_empty(&(tcp_listen_sock_table[hash])));
	printf("sip=%x, sport=%d\n", saddr, sport);
*/
	// debug
	/*
	printf("list addr:%x\n", &(tcp_listen_sock_table[hash]));
	list_for_each_entry_safe(tmp_p, tmp_q, &(tcp_listen_sock_table[hash]), hash_list)
	{
		// printf("%d\n", ((u64)(&(tmp_p->list)) - (u64)(tmp_p)));
		printf("tmp_p->sip=%x, tmp_p->sport=%d\n", tmp_p->sk_sip, tmp_p->sk_sport);
	}*/

	return NULL;
}

// lookup tcp sock in both established_table and listen_table
struct tcp_sock *tcp_sock_lookup(struct tcp_cb *cb)
{
	u32 saddr = cb->daddr,
		daddr = cb->saddr;
	u16 sport = cb->dport,
		dport = cb->sport;

	struct tcp_sock *tsk = tcp_sock_lookup_established(saddr, daddr, sport, dport);
	if (!tsk)
		tsk = tcp_sock_lookup_listen(saddr, sport);

	return tsk;
}

// hash tcp sock into bind_table, using sport as the key
static int tcp_bind_hash(struct tcp_sock *tsk)
{
	int bind_hash_value = tcp_hash_function(0, 0, tsk->sk_sport, 0);
	struct list_head *list = &tcp_bind_sock_table[bind_hash_value];
	list_add_head(&tsk->bind_hash_list, list);

	tsk->ref_cnt += 1;

	return 0;
}

// unhash the tcp sock from bind_table
void tcp_bind_unhash(struct tcp_sock *tsk)
{
	if (!list_empty(&tsk->bind_hash_list)) {
		list_delete_entry(&tsk->bind_hash_list);
		free_tcp_sock(tsk);
	}
}

// lookup bind_table to check whether sport is in use
static int tcp_port_in_use(u16 sport)
{
	int value = tcp_hash_function(0, 0, sport, 0);
	struct list_head *list = &tcp_bind_sock_table[value];
	struct tcp_sock *tsk;
	list_for_each_entry(tsk, list, bind_hash_list) {
		if (tsk->sk_sport == sport)
			return 1;
	}

	return 0;
}

// find a free port by looking up bind_table
static u16 tcp_get_port()
{
	for (u16 port = PORT_MIN; port < PORT_MAX; port++) {
		if (!tcp_port_in_use(port))
			return port;
	}

	return 0;
}

// tcp sock tries to use port as its source port
static int tcp_sock_set_sport(struct tcp_sock *tsk, u16 port)
{
	if ((port && tcp_port_in_use(port)) ||
			(!port && !(port = tcp_get_port())))
		return -1;

	tsk->sk_sport = port;

	tcp_bind_hash(tsk);

	return 0;
}

// hash tcp sock into either established_table or listen_table according to its
// TCP_STATE
int tcp_hash(struct tcp_sock *tsk)
{
	struct list_head *list;
	int hash;

	if (tsk->state == TCP_CLOSED)
		return -1;

	if (tsk->state == TCP_LISTEN) 
	{
		hash = tcp_hash_function(0, 0, tsk->sk_sport, 0);
		list = &tcp_listen_sock_table[hash];

		//debug
		/*
		static int debug_counter = 0;
		printf("tcp_hash listen times:%d\n", debug_counter);
		debug_counter++;
		printf("debug:hash=%d\n", hash);
		printf("empty:%d\n", list_empty(&(tcp_listen_sock_table[hash])));
		//printf("tsk_sip=%x, tsk_sport=%d\n", tsk->sk_sip, tsk->sk_sport);
*/
	}
	else 
	{
		int hash = tcp_hash_function(tsk->sk_sip, tsk->sk_dip, \
				tsk->sk_sport, tsk->sk_dport); 
		list = &tcp_established_sock_table[hash];

		struct tcp_sock *tmp;
		list_for_each_entry(tmp, list, hash_list) 
		{
			if (tsk->sk_sip == tmp->sk_sip &&
					tsk->sk_dip == tmp->sk_dip &&
					tsk->sk_sport == tmp->sk_sport &&
					tsk->sk_dport == tmp->sk_dport)
				return -1;
		}
	}

	list_add_head(&tsk->hash_list, list);
	tsk->ref_cnt += 1;


	// debug
	/*
	struct tcp_sock *tmp_p, *tmp_q;
	printf("list addr:%x\n", &(tcp_listen_sock_table[hash]));
	list_for_each_entry_safe(tmp_p, tmp_q, &(tcp_listen_sock_table[hash]), hash_list)
	{
		debug_pointer = tmp_p;
		printf("tsk->sip=%x, tsk->sport=%d\n", tmp_p->sk_sip, tmp_p->sk_sport);
	}
*/

	return 0;
}

// unhash tcp sock from established_table or listen_table
void tcp_unhash(struct tcp_sock *tsk)
{
	if (!list_empty(&tsk->hash_list)) {
		list_delete_entry(&tsk->hash_list);
		free_tcp_sock(tsk);
	}
}

// XXX: skaddr here contains network-order variables
int tcp_sock_bind(struct tcp_sock *tsk, struct sock_addr *skaddr)
{
	int err = 0;

	// omit the ip address, and only bind the port
	err = tcp_sock_set_sport(tsk, ntohs(skaddr->port));

	return err;
}

// connect to the remote tcp sock specified by skaddr
//
// XXX: skaddr here contains network-order variables
// 1. initialize the four key tuple (sip, sport, dip, dport);
// 2. hash the tcp sock into bind_table;
// 3. send SYN packet, switch to TCP_SYN_SENT state, wait for the incoming
//    SYN packet by sleep on wait_connect;
// 4. if the SYN packet of the peer arrives, this function is notified, which
//    means the connection is established.
int tcp_sock_connect(struct tcp_sock *tsk, struct sock_addr *skaddr)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);

	// 1. initialize the four key tuple (sip, sport, dip, dport);	
	tsk->sk_dip = ntohl(skaddr->ip);
	tsk->sk_dport = ntohs(skaddr->port);
	
	rt_entry_t *tmp_rentry = longest_prefix_match(tsk->sk_dip);
	if(tmp_rentry == NULL)
	{
		printf("Error in rtable search.\n");
		exit(-1);
	}

	tsk->sk_sip = tmp_rentry->iface->ip;

	if(tcp_sock_set_sport(tsk, tcp_get_port()) == -1)
	{
		printf("debug: Error in tcp sock set sport.\n");
		exit(-1);
	}

	// 2. hash the tcp sock into bind_table;
	//tcp_bind_hash(tsk); // 这一步在tcp_sock_set_sport()里面已经做了

	// 3. send SYN packet
	tsk->iss = tcp_new_iss(); // 发送之前把序列号弄好
	tsk->snd_nxt = tsk->iss;

	// printf("debug: client before send ctrl packet.\n");


	tcp_send_control_packet(tsk, TCP_SYN);

	// printf("debug: client after send ctrl packet.\n");



	// 3. switch to TCP_SYN_SENT state
	tcp_set_state(tsk, TCP_SYN_SENT);
	tcp_hash(tsk);

	// 3.wait for the incoming SYN packet by sleep on wait_connect
	sleep_on(tsk->wait_connect);

	// 4. if the SYN packet of the peer arrives, this function is notified, which means the connection is established.
	// tcp_send_control_packet(tsk, TCP_ACK);
	// tcp_set_state(tsk, TCP_ESTABLISHED);
	// 上面这两行在tcp_in.c里面实现

	//return -1;
	return 0;
}

// set backlog (the maximum number of pending connection requst), switch the
// TCP_STATE, and hash the tcp sock into listen_table
int tcp_sock_listen(struct tcp_sock *tsk, int backlog)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	// set backlog
	tsk->backlog = backlog;

	// switch the TCP_STATE
	tcp_set_state(tsk, TCP_LISTEN);

	// hash the tcp sock into listen_table
	// tcp_hash(tsk);

	//debug
	// printf("debug:in tcp_sock_listen.\n");

	return tcp_hash(tsk);

	// return -1;
}

// check whether the accept queue is full
inline int tcp_sock_accept_queue_full(struct tcp_sock *tsk)
{
	if (tsk->accept_backlog >= tsk->backlog) {
		log(ERROR, "tcp accept queue (%d) is full.", tsk->accept_backlog);
		return 1;
	}

	return 0;
}

// push the tcp sock into accept_queue
inline void tcp_sock_accept_enqueue(struct tcp_sock *tsk)
{
	if (!list_empty(&tsk->list))
		list_delete_entry(&tsk->list);
	list_add_tail(&tsk->list, &tsk->parent->accept_queue);
	tsk->parent->accept_backlog += 1;
}

// pop the first tcp sock of the accept_queue
inline struct tcp_sock *tcp_sock_accept_dequeue(struct tcp_sock *tsk)
{
	struct tcp_sock *new_tsk = list_entry(tsk->accept_queue.next, struct tcp_sock, list);
	list_delete_entry(&new_tsk->list);
	init_list_head(&new_tsk->list);
	tsk->accept_backlog -= 1;

	return new_tsk;
}

// if accept_queue is not emtpy, pop the first tcp sock and accept it,
// otherwise, sleep on the wait_accept for the incoming connection requests
struct tcp_sock *tcp_sock_accept(struct tcp_sock *tsk)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	struct tcp_sock *new_tsk = NULL;
	while(1)
	{
		if(!list_empty(&(tsk->accept_queue)))
		{
			new_tsk = tcp_sock_accept_dequeue(tsk);
			break;
		}
		else
		{
			sleep_on(tsk->wait_accept);
		}
	}

	return new_tsk;
	// return NULL;
}

// close the tcp sock, by releasing the resources, sending FIN/RST packet
// to the peer, switching TCP_STATE to closed
void tcp_sock_close(struct tcp_sock *tsk)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);

	// printf("debug: current state:%s\n", tcp_state_str[tsk->state]);

	if(tsk->state == TCP_ESTABLISHED) // 这种情况是客户端发起四次挥手中的第一次
	{
		tcp_send_control_packet(tsk, TCP_FIN|TCP_ACK); // 第一次挥手只有FIN
		tcp_set_state(tsk, TCP_FIN_WAIT_1);
	}
	else if(tsk->state == TCP_CLOSE_WAIT) // 这种情况是服务器端发起第三次挥手
	{
		tcp_send_control_packet(tsk, (TCP_FIN|TCP_ACK));
		tcp_set_state(tsk, TCP_LAST_ACK);
	}
	else
	{
		printf("Error. Impossible branch in tcp_sock_close.state=%s\n", tcp_state_str[tsk->state]);

		// exit(-1);
	}
	
	
	return;
}


// 将tsk的ring buffer里面的东西搞到buf里面，len是buf的可使用长度
int tcp_sock_read(struct tcp_sock *tsk, char *buf, int len)
{

	if(tsk == NULL)
	{
		printf("error in tcp_sock_read: tsk is NULL.\n");
		return -1;
	}
	else if (tsk->state == TCP_CLOSED)
	{
		printf("error in tcp_sock_read: do not have connect.\n");
		return -1;
	}

	

	int ret = 0;
	pthread_mutex_lock(&(tsk->rcv_buf->lock));
	// printf("debug: in the front of tcp_sock_read.\n");	
	while(ring_buffer_empty(tsk->rcv_buf)) // 等待rbuf不为空的时候
	{
		pthread_mutex_unlock(&(tsk->rcv_buf->lock));
		// printf("debug: before sleep in tcp_sock_read.\n");	
		sleep_on(tsk->wait_recv);
		//if((tsk->state == TCP_CLOSE_WAIT) && (ring_buffer_empty(tsk->rcv_buf)))
		if(tsk->state == TCP_CLOSE_WAIT)
		{
			// printf("debug: exit early. ring buffer:%d\n", ring_buffer_used(tsk->rcv_buf));

			return 0;
		}
		// printf("debug: after  sleep in tcp_sock_read.\n");	
		pthread_mutex_lock(&(tsk->rcv_buf->lock));
	}

	// printf("debug: check point in tcp read.\n");

	if (tsk->state == TCP_CLOSED)
	{
		printf("error in tcp_sock_read: do not have connect. part2.\n");
		pthread_mutex_unlock(&(tsk->rcv_buf->lock));
		return -1;
	}

	ret = read_ring_buffer(tsk->rcv_buf, buf, len); // 第一个参数不要加&，tsk的rcv_buf属性就是指针
	tsk->rcv_wnd += ret;
	// printf("debug: in the last of tcp_sock_read.\n");	
	pthread_mutex_unlock(&(tsk->rcv_buf->lock));

	wake_up(tsk->wait_ofo); // prj13 added

	// printf("debug: tcp_sock_read:ret:%d\n", ret);

	return ret;
}

int tcp_sock_write(struct tcp_sock *tsk, char *buf, int len)
{
	if(tsk == NULL)
	{
		printf("error in tcp_sock_write: tsk is NULL.\n");
		return -1;
	}
	else if (tsk->state == TCP_CLOSED)
	{
		printf("error in tcp_sock_write: do not have connect.\n");
		return -1;
	}

	// printf("debug: in the front of tcp_sock_write.\n");

	// int total_len = min(len, 1514 - ETHER_HDR_SIZE - IP_BASE_HDR_SIZE - TCP_BASE_HDR_SIZE);
	
	int len_sent = 0;
	int tcp_data_len = 0;
	int pac_len = 0;
	int inflight = 0; // prj14 added
	int snd_wnd_tmp = 0; // prj14 added


	while(len_sent < len)
	{
		snd_wnd_tmp = min(tsk->adv_wnd, (tsk->cwnd)*MSS);	// prj14 added
		
		/*
		while(tsk->snd_wnd == 0)
		{
			// printf("debug: before sleep.\n");
			sleep_on(tsk->wait_send);
			// printf("debug: after  sleep.\n")
		}*/

		inflight = (tsk->snd_nxt - tsk->snd_una) - (tsk->dupacks)*MSS ;	// prj14 added

		// 每次发的包的长度是“剩下数据长度”、“发送窗口”、“每个tcp包的限制长度1514”这三个中的最小值
		// tcp_data_len = min((len - len_sent), tsk->snd_wnd); 
		// tcp_data_len = min(tcp_data_len, 1514 - ETHER_HDR_SIZE - IP_BASE_HDR_SIZE - TCP_BASE_HDR_SIZE);
		tcp_data_len = min((len- len_sent), MSS); // prj14 added
		pac_len = tcp_data_len + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + TCP_BASE_HDR_SIZE;

		//while(tsk->snd_wnd == 0)
		//while((snd_wnd_tmp - inflight) <= 0)
		while(less_or_equal_32b(snd_wnd_tmp, inflight))
		{
			// printf("debug: before sleep.\n");
			sleep_on(tsk->wait_send);
			// printf("debug: after  sleep.\n")
		}


		char *packet = (char *)malloc(sizeof(char) * pac_len);
		char *tcp_data_p = packet + ETHER_HDR_SIZE + IP_BASE_HDR_SIZE + TCP_BASE_HDR_SIZE; // tcp包的正文指针
		char *buf_data_p = buf + len_sent; // buffer里面还没有发送的数据的指针
		memcpy((void *)tcp_data_p, (void *)buf_data_p, tcp_data_len); // 只写入正文部分，报头部分在后续调用的函数里面填写
		
		tcp_send_packet(tsk, packet, pac_len);
		len_sent += tcp_data_len;
	}
	// printf("debug: in the last of tcp_sock_write.\n");

	return len;
}



void tcp_add_send_buf(struct tcp_sock *tsk, char *packet, int len) // prj13 added
{
	char *packet_backup = (char *)malloc(sizeof(char) *len);
	memcpy((void *)packet_backup, (void *)packet, len);

	snd_buf_elem *new_snd_buf = (snd_buf_elem *)malloc(sizeof(snd_buf_elem));
	init_list_head(&(new_snd_buf->list));
	
	struct iphdr *ip_hdr = packet_to_ip_hdr(packet_backup);
	struct tcphdr *tcp_hdr = packet_to_tcp_hdr(packet_backup);
	
	new_snd_buf->len = len;
	new_snd_buf->packet = packet_backup;
	// seq_end 的计算方法参考tcp_cb_init()
	int tcp_len = ntohs(ip_hdr->tot_len) - IP_HDR_SIZE(ip_hdr) - TCP_HDR_SIZE(tcp_hdr);
	new_snd_buf->seq_end = ntohl(tcp_hdr->seq) + tcp_len + ((tcp_hdr->flags & (TCP_SYN|TCP_FIN)) ? 1 : 0);

	// printf("debug: seq=%d\n", tcp_hdr->seq);
	// printf("debug:head:%x, head->prev:%x\n", &(tsk->send_buf), tsk->send_buf.prev);

	list_add_tail(&(new_snd_buf->list), &(tsk->send_buf));

	// printf("debug: after list_add_tail.\n");
	// printf("debug: tskaddr: %x, new pac:%x, len:%d, seqend:%d, list:%x\n", tsk,  new_snd_buf->packet, new_snd_buf->len, new_snd_buf->seq_end, &(new_snd_buf->list));

// debug
/*
	snd_buf_elem * retrans_elem, *q;
	list_for_each_entry_safe(retrans_elem, q, &(tsk->send_buf), list)
	{
		printf("debug2: tskaddr:%x, re pac:%x, len:%d, seqend:%d, list:%x\n", tsk, retrans_elem->packet, retrans_elem->len, retrans_elem->seq_end, &(retrans_elem->list));
	}
*/	

}

int tcp_update_send_buf(struct tcp_sock *tsk, struct tcp_cb *cb) // 用来删除send buf里面的包的
{
	snd_buf_elem *p, *q;
	p = NULL;
	q = NULL;
	int has_updated = 0;

	// printf("debug: in update_send_buf:seq=%u, ack=%u\n", cb->seq, cb->ack);
	// printf("snd buf empty:%d\n", list_empty(&tsk->send_buf));

	list_for_each_entry_safe(p, q, &(tsk->send_buf), list)
	{
		// printf("seqend=%u\n", p->seq_end);
		if(less_or_equal_32b(p->seq_end, cb->ack)) // 如果某个包的最后一个字节都被确认了，那这个包肯定整个都被确认过了
		{
			has_updated = 1;
			list_delete_entry(&(p->list));
			free(p->packet);
			free(p);
			// printf("debug: delete entry\n");
		}
	}

	return has_updated;
}


void write_ofo_buffer(struct tcp_sock *tsk, struct tcp_cb *cb) // prj13 added
{
	ofo_elem *p, *q;	
	ofo_elem *new = (ofo_elem *)malloc(sizeof(ofo_elem));
	
	//debug
	/*
	static u32 init_seq = 0;
	if(init_seq == 0)
		init_seq = cb->seq;
	*/

	new->pl_len = cb->pl_len;
	new->seq = cb->seq;
	new->payload = (char *)malloc(sizeof(char)*cb->pl_len);
	memcpy((void *)(new->payload), (void *)(cb->payload), (size_t)(cb->pl_len));
	
	int find = 0;
	list_for_each_entry_safe(p, q, &(tsk->rcv_ofo_buf), list)
	{
		if(less_than_32b(new->seq, p->seq)) // 链表按升序排列
		{
			find = 1;
			break;
		}
	}

	if(find)
	{
		list_insert(&(new->list), p->list.prev, &(p->list)); // list.prev已经是指针了
	}
	else
	{
		list_add_tail(&(new->list), &(tsk->rcv_ofo_buf));
	}
	
	// 找一下是否可以把ofo里面的东西塞到ring buffer里面
	list_for_each_entry_safe(p, q, &(tsk->rcv_ofo_buf), list)
	{
		if(tsk->rcv_nxt == p->seq) // rcv_nxt == seq，此时，这个包的全部内容写进ring buffer
		{
			// lock?
			tsk->rcv_nxt += p->pl_len;
			// tsk->rcv_wnd -= p->pl_len; // 这个操作在外层函数case ESTABLISHED里面执行了

			pthread_mutex_lock(&(tsk->rcv_buf->lock));

			while(ring_buffer_free(tsk->rcv_buf) < p->pl_len) // 空间不够
			{
				// sleep_on(tsk->wait_recv);
				pthread_mutex_unlock(&(tsk->rcv_buf->lock));
				sleep_on(tsk->wait_ofo);
				pthread_mutex_lock(&(tsk->rcv_buf->lock));
			}


			write_ring_buffer(tsk->rcv_buf, p->payload, p->pl_len);
			list_delete_entry(&(p->list));
			free(p->payload);
			free(p);
			// unlock?
			pthread_mutex_unlock(&(tsk->rcv_buf->lock));

			if(tsk->wait_recv->sleep)
			{
				// printf("before wake up wait recv in write ofo.\n");
				wake_up(tsk->wait_recv);
			}
		}
		else if(greater_than_32b(tsk->rcv_nxt, p->seq)) // rcv_nxt > seq.这个情况还要细分
		{
			/*
			printf("Error. ofo has seq num less than rcv_nxt.\n");
			printf("debug: tsk rcv_nxt=%u, relative=%u\n", tsk->rcv_nxt, (tsk->rcv_nxt - init_seq));
			list_for_each_entry_safe(p, q, &(tsk->rcv_ofo_buf), list)
			{
				printf("debug: seq=%u, len=%u, rltvseq=%u\n", p->seq, p->pl_len, (p->seq - init_seq));
			}
			exit(-1);
			*/
			if(tsk->rcv_nxt < p->seq + p->pl_len) // 这种情况下，需要选择其中高于rcv_nxt的部分开始写
			{
				u32 old_rcv_nxt = tsk->rcv_nxt;
				u32 real_data_len = p->pl_len - (old_rcv_nxt - p->seq);
				char *real_payload_start = (char *)((p->payload) + (old_rcv_nxt - p->seq));
				tsk->rcv_nxt = p->seq + p->pl_len;

				// tsk->rcv_wnd -= p->pl_len; // 这个操作在外层函数case ESTABLISHED里面执行了

				pthread_mutex_lock(&(tsk->rcv_buf->lock));

				while(ring_buffer_free(tsk->rcv_buf) < real_data_len) // 空间不够
				{
					// sleep_on(tsk->wait_recv);
					pthread_mutex_unlock(&(tsk->rcv_buf->lock));
					sleep_on(tsk->wait_ofo);
					pthread_mutex_lock(&(tsk->rcv_buf->lock));
				}


				write_ring_buffer(tsk->rcv_buf, real_payload_start, real_data_len);
				list_delete_entry(&(p->list));
				free(p->payload);
				free(p);
				// unlock?
				pthread_mutex_unlock(&(tsk->rcv_buf->lock));

				if(tsk->wait_recv->sleep)
				{
					// printf("before wake up wait recv in write ofo.\n");
					wake_up(tsk->wait_recv);
				}
			}
			else // 这种情况下这个包彻底没用
			{
				list_delete_entry(&(p->list));
				free(p->payload);
				free(p);
				printf("debug: dropped a useless packet.\n");
			}
			

		}
		else // rcv_nxt <  seq，此时，这代表了这个包还不是写的时候
		{
			break;
		}
		
		
	}

}

// prj14 added
int tcp_update_cwnd(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	static u32 recovery_point;
	static u32 cwnd_counter;

	int need_resent = 0;

	while(pthread_mutex_trylock(&(tsk->cwnd_lock)))
	{
		;
	}


	switch (tsk->cwnd_state)
	{
		case CWND_OPEN:
			{
				// 更新cwnd状态
				if(tsk->snd_una == cb->ack) // dup ack
				{
					tsk->dupacks = 1;
					tsk->cwnd_state = CWND_DISORDER;
				}
				else if(less_than_32b(tsk->snd_una, cb->ack)) // 正常的ack包
				{
					tsk->snd_una = cb->ack;
					tsk->dupacks = 0;	
				}
				else
				{
					printf("Error: snd_una > cb->ack.\n");
					exit(-1);
				}
				
				// 更新cwnd大小
				if (tsk->cwnd < tsk->ssthresh) // 慢启动
				{
					cwnd_counter = 0;
					tsk->cwnd++;					
				}
				else // 拥塞避免
				{
					cwnd_counter++;
					if(greater_or_equal_32b(cwnd_counter, tsk->cwnd)) // cwnd += 1/cwnd
					{
						cwnd_counter = 0;
						tsk->cwnd++;
					}
				}
				
				break;
			}
		
		case CWND_DISORDER:
			{
				// 更新cwnd状态
				if(tsk->snd_una == cb->ack) // dup ack
				{
					tsk->dupacks++;
					if(tsk->dupacks >= 3)
					{
						tsk->cwnd_state = CWND_RECOVERY;
						recovery_point = tsk->snd_nxt;
						tsk->cwnd = tsk->cwnd /2;
						if(tsk->cwnd == 0)
						{
							tsk->cwnd = 1;
						}
						tsk->ssthresh = tsk->cwnd;

						// resend_send_buf(tsk);
						need_resent = 1;
					}
				}
				else if(less_than_32b(tsk->snd_una, cb->ack)) // 正常的ack包
				{
					tsk->snd_una = cb->ack;
					tsk->dupacks = 0;	
					tsk->cwnd_state = CWND_OPEN;
				}
				else
				{
					printf("Error: snd_una > cb->ack.\n");
					exit(-1);
				}


				// 更新cwnd大小
				if (tsk->cwnd < tsk->ssthresh) // 慢启动
				{
					cwnd_counter = 0;
					tsk->cwnd++;					
				}
				else // 拥塞避免
				{
					cwnd_counter++;
					if(greater_or_equal_32b(cwnd_counter, tsk->cwnd)) // cwnd += 1/cwnd
					{
						cwnd_counter = 0;
						tsk->cwnd++;
					}
				}

				break;
			}
		case CWND_RECOVERY:
			{
				if(tsk->snd_una == cb->ack) // 如果该ACK没有确认新数据，则说明inflight减一，cwnd允许发送一个新数据包
				{
					tsk->dupacks++;

				}
				else if(less_than_32b(tsk->snd_una, cb->ack)) // 如果该ACK确认了新数据
				{
					tsk->snd_una = cb->ack;
					tsk->dupacks = 0;	

					if(less_than_32b(cb->ack, recovery_point)) // partial ACK
					{
						// tsk->dupacks++;
						// resend_send_buf(tsk); // 重传对应的数据包
						need_resent = 1;
					}
					else // full ACK
					{
						// tsk->dupacks = 0;
						tsk->cwnd_state = CWND_OPEN;
					}
					
				}
				else
				{
					printf("Error: snd_una > cb->ack.\n");
					exit(-1);
				}
				
				
				break;
			}

		case CWND_LOSS:
			{
				printf("debug: in case CWND_LOSS.\n");
				break;		
			}

		default:
			{
				printf("Error: impossible cwnd state.\n");
				break;
			}
	}

	pthread_mutex_unlock(&(tsk->cwnd_lock));

	// 记录此时的cwnd
	cwnd_record[record_index] = tsk->cwnd;
	record_index++;
	// debug
	if(record_index >= CWND_RECORD_NUM)
	{
		printf("Error. Do not have enough space to record cwnd size.\n");
		exit(-1);
	}

	return need_resent;
}


void resend_send_buf(struct tcp_sock *tsk) // 按照timer里面的重传函数写的
{
	if(!list_empty(&(tsk->send_buf)))
	{						
		snd_buf_elem * retrans_elem = list_entry(tsk->send_buf.next, snd_buf_elem, list);
		char *packet = (char *)malloc(sizeof(char)*retrans_elem->len);
		
		//printf("debug: packet:%x, re->pac:%x, re->len:%d\n", packet, retrans_elem->packet, retrans_elem->len);
		//printf("debug: tskaddr:%x, re pac:%x, len:%d, seqend:%d, list:%x\n", tsk, retrans_elem->packet, retrans_elem->len, retrans_elem->seq_end, &(retrans_elem->list));

		memcpy((void *)packet, (void *)(retrans_elem->packet), retrans_elem->len);

		// tcp_send_packet(tsk, packet, retrans_elem->len);
		// 以下照搬tcp_send_packet中的内容
		struct iphdr *ip = packet_to_ip_hdr(packet);
		struct tcphdr *tcp = (struct tcphdr *)((char *)ip + IP_BASE_HDR_SIZE);

		tcp->ack = htonl(tsk->rcv_nxt);
		tcp->rwnd = htons(tsk->rcv_wnd);
		tcp->checksum = tcp_checksum(ip, tcp);
		ip->checksum = ip_checksum(ip);

		// printf("debug: resend seq:%u, retry:%d\n", ntohl(tcp->seq), timer_p->retry);

		ip_send_packet(packet, retrans_elem->len);
	}
}