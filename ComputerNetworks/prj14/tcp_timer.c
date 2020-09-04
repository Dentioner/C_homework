#include "tcp.h"
#include "tcp_timer.h"
#include "tcp_sock.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static struct list_head timer_list;

pthread_mutex_t timer_list_lock; // prj13 added

static int timer_list_inited; // prj13 added

// scan the timer_list, find the tcp sock which stays for at 2*MSL, release it
void tcp_scan_timer_list()
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	struct tcp_timer *timer_p, *timer_q;
	timer_p = NULL;
	timer_q = NULL;

	pthread_mutex_lock(&timer_list_lock);



/*
	if(!list_empty(&timer_list));
		printf("debug: before list for entry safe in timer.c\n");

	list_for_each_entry_safe(timer_p, timer_q, &(timer_list), list)
	{
		printf("debug: test.\n");
	}
*/
	list_for_each_entry_safe(timer_p, timer_q, &(timer_list), list)
	{
		timer_p->timeout -= TCP_TIMER_SCAN_INTERVAL; // 因为每次主函数睡这么久，每次执行的间隔就是这么长

		// printf("debug: type:%d, timeout:%d, en:%d\n", timer_p->type, timer_p->timeout, timer_p->enable);


		//if(timer_p->timeout > TCP_TIMEWAIT_TIMEOUT)
		if(timer_p->timeout <= 0)
		{
			if(timer_p->type == 0) // timewait
			{
				list_delete_entry(&(timer_p->list));
				// free(timer_p);
				struct tcp_sock *tmp_tsk = timewait_to_tcp_sock(timer_p); // 要把这个timer所在的整个sock搞掉
				tcp_bind_unhash(tmp_tsk);
				if(tmp_tsk->state != TCP_CLOSED)
				{
					tcp_set_state(tmp_tsk, TCP_CLOSED);
				}
					
				free_tcp_sock(tmp_tsk);
			}
			else if(timer_p->type == 1) // retrans
			{
				struct tcp_sock *tmp_tsk = retranstimer_to_tcp_sock(timer_p);
				if(timer_p->retry >= TCP_RETRANS_MAX_RETRY_TIMES)
				{
					list_delete_entry(&(timer_p->list));
					
					if(tmp_tsk->state != TCP_CLOSED)
					{
						tcp_send_control_packet(tmp_tsk, TCP_RST);
						tcp_set_state(tmp_tsk, TCP_CLOSED);
					}
					// printf("debug: retrans retries more than %d times, exit.\n", TCP_RETRANS_MAX_RETRY_TIMES);
				}
				else
				{
					timer_p->retry ++;
					timer_p->timeout = TCP_RETRANS_INTERVAL_INITIAL << (timer_p->retry); // 翻倍
					//重传

					//printf("debug: tsk snd buf:%x, snd buf next:%x, snd buf prev:%x\n", &(tmp_tsk->send_buf), tmp_tsk->send_buf.next, tmp_tsk->send_buf.prev);
					/*
					if(!list_empty(&(tmp_tsk->send_buf)))
					{						
						snd_buf_elem * retrans_elem = list_entry(tmp_tsk->send_buf.next, snd_buf_elem, list);
						char *packet = (char *)malloc(sizeof(char)*retrans_elem->len);
						
						//printf("debug: packet:%x, re->pac:%x, re->len:%d\n", packet, retrans_elem->packet, retrans_elem->len);
						//printf("debug: tskaddr:%x, re pac:%x, len:%d, seqend:%d, list:%x\n", tmp_tsk, retrans_elem->packet, retrans_elem->len, retrans_elem->seq_end, &(retrans_elem->list));

						memcpy((void *)packet, (void *)(retrans_elem->packet), retrans_elem->len);

						// tcp_send_packet(tmp_tsk, packet, retrans_elem->len);
						// 以下照搬tcp_send_packet中的内容
						struct iphdr *ip = packet_to_ip_hdr(packet);
						struct tcphdr *tcp = (struct tcphdr *)((char *)ip + IP_BASE_HDR_SIZE);

						tcp->ack = htonl(tmp_tsk->rcv_nxt);
						tcp->rwnd = htons(tmp_tsk->rcv_wnd);
						tcp->checksum = tcp_checksum(ip, tcp);
						ip->checksum = ip_checksum(ip);

						// printf("debug: resend seq:%u, retry:%d\n", ntohl(tcp->seq), timer_p->retry);

						ip_send_packet(packet, retrans_elem->len);
					}
					*/
					resend_send_buf(tmp_tsk);

					// prj14 added


					while(pthread_mutex_trylock(&(tmp_tsk->cwnd_lock)))
					{
						;
					}

					tmp_tsk->cwnd_state = CWND_OPEN;
					tmp_tsk->ssthresh = tmp_tsk->cwnd/2;
					if(tmp_tsk->ssthresh == 0)
					{
						tmp_tsk->ssthresh = 1;
					}
					tmp_tsk->cwnd = 1;
					
					pthread_mutex_unlock(&(tmp_tsk->cwnd_lock));

					// 记录此时的cwnd
					cwnd_record[record_index] = tmp_tsk->cwnd;
					record_index++;
					// debug
					if(record_index >= CWND_RECORD_NUM)
					{
						printf("Error. Do not have enough space to record cwnd size.\n");
						exit(-1);
					}



				}
				
			}
			else
			{
				printf("Error. Undefined timer type:%d\n", timer_p->type);
				exit(-1);
			}
		}
	}
	
	pthread_mutex_unlock(&timer_list_lock);

	return;
}

// set the timewait timer of a tcp sock, by adding the timer into timer_list
void tcp_set_timewait_timer(struct tcp_sock *tsk)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	while(timer_list_inited != 1)
	{
		;
	}

	pthread_mutex_lock(&timer_list_lock);

	tsk->timewait.enable = 1;
	// tsk->timewait.timeout = 0;
	tsk->timewait.timeout = TCP_TIMEWAIT_TIMEOUT;
	tsk->timewait.type = 0;
	list_add_tail(&(tsk->timewait.list), &(timer_list));

	pthread_mutex_unlock(&timer_list_lock);

	return;
}

// scan the timer_list periodically by calling tcp_scan_timer_list
void *tcp_timer_thread(void *arg)
{
	init_list_head(&timer_list);
	timer_list_inited = 1;

	pthread_mutex_init(&timer_list_lock, NULL); // prj13 added

	while (1) {
		usleep(TCP_TIMER_SCAN_INTERVAL);

		// printf("debug: before scan timer list.\n");

		tcp_scan_timer_list();

		// printf("debug: after  scan timer list.\n");
	}

	return NULL;
}

void tcp_set_retrans_timer(struct tcp_sock *tsk) // prj13 added
{

	while(timer_list_inited != 1)
	{
		;
	}	
	
	pthread_mutex_lock(&timer_list_lock);

	struct tcp_timer *retrans_timer;
	retrans_timer = &(tsk->retrans_timer);
	retrans_timer->type = 1;
	retrans_timer->timeout = TCP_RETRANS_INTERVAL_INITIAL;
	retrans_timer->enable = 1;
	retrans_timer->retry = 0;

	init_list_head(&(retrans_timer->list));

	// printf("debug: before add tail retrans.\n");
	// printf("debug: retimer list:%x, timerlist:%x.\n", &(retrans_timer->list), &(timer_list));
	// printf("debug: new:%x, head->prev:%x, head:%x\n", &(retrans_timer->list), timer_list.next, &(timer_list));
	list_add_tail(&(retrans_timer->list), &(timer_list));

	// printf("debug: after add tail retrans.\n");

	pthread_mutex_unlock(&timer_list_lock);
} 	

void tcp_unset_retrans_timer(struct tcp_sock *tsk) // prj13 added
{
	struct tcp_timer *retrans_timer;
	// struct tcp_timer *timer_p, *timer_q;

	retrans_timer = &(tsk->retrans_timer);

	pthread_mutex_lock(&timer_list_lock);

	retrans_timer->enable = 0;

	pthread_mutex_unlock(&timer_list_lock);

	
}

void tcp_reset_retrans_timer(struct tcp_sock *tsk) // prj13 added
{
	struct tcp_timer *retrans_timer;
	retrans_timer = &(tsk->retrans_timer);
	pthread_mutex_lock(&timer_list_lock);

	if(!list_empty(&(tsk->send_buf)))
	{
		retrans_timer->type = 1;
		retrans_timer->timeout = TCP_RETRANS_INTERVAL_INITIAL;
		retrans_timer->enable = 1;
		retrans_timer->retry = 0;
	}
	else
	{
		list_delete_entry(&(retrans_timer->list));
		retrans_timer->enable = 0;
	}
	


	pthread_mutex_unlock(&timer_list_lock);
}