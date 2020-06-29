#include "tcp.h"
#include "tcp_sock.h"
#include "tcp_timer.h"

#include "log.h"
#include "ring_buffer.h"

#include <stdlib.h>
// update the snd_wnd of tcp_sock
//
// if the snd_wnd before updating is zero, notify tcp_sock_send (wait_send)
static inline void tcp_update_window(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	u16 old_snd_wnd = tsk->snd_wnd;
	tsk->snd_wnd = cb->rwnd;
	if (old_snd_wnd == 0)
		wake_up(tsk->wait_send);
}

// update the snd_wnd safely: cb->ack should be between snd_una and snd_nxt
static inline void tcp_update_window_safe(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	if (less_or_equal_32b(tsk->snd_una, cb->ack) && less_or_equal_32b(cb->ack, tsk->snd_nxt))
		tcp_update_window(tsk, cb);
}

#ifndef max
#	define max(x,y) ((x)>(y) ? (x) : (y))
#endif

// check whether the sequence number of the incoming packet is in the receiving
// window
static inline int is_tcp_seq_valid(struct tcp_sock *tsk, struct tcp_cb *cb)
{
	u32 rcv_end = tsk->rcv_nxt + max(tsk->rcv_wnd, 1);
	if (less_than_32b(cb->seq, rcv_end) && less_or_equal_32b(tsk->rcv_nxt, cb->seq_end)) {
		return 1;
	}
	else {
		log(ERROR, "received packet with invalid seq, drop it.");
		return 0;
	}
}

// Process the incoming packet according to TCP state machine. 
void tcp_process(struct tcp_sock *tsk, struct tcp_cb *cb, char *packet)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	char c_flags[32];
	memset((void *)c_flags, 0, 32);

	if(tsk == NULL) // ����hash list�ǿյ�
	{
		return;
	}

	// ����RSTֱ�ӽ������ӣ�û�����������
	if(cb->flags & TCP_RST)
	{
		printf("debug: get RST pac.\n");
		tcp_set_state(tsk, TCP_CLOSED);
		tcp_unhash(tsk);
		return;
	}

	//printf("debug:before:%s, ", tcp_state_str[tsk->state]);
	


	switch (tsk->state)
	{
		case TCP_CLOSED:
			{
				printf("Error. TCP_CLOSED.\n");
				exit(-1);
				break;
			}	
		case TCP_LISTEN: // �������״̬˵���Ƿ��������������У���ʱ���ܻ����3������
			{
				if(cb->flags & TCP_SYN) // �Է�����ĵ�һ������
				{
					struct tcp_sock *csk = alloc_tcp_sock(); // ���������������ʼ���ˣ��Ͳ����ֶ���ʼ����
					struct sock_addr skaddr;

					csk->sk_sip = cb->daddr; // cb �ڳ�ʼ��tcp_cb_init��ʱ���Ѿ�ת���ֽ����ˣ�����Ͳ���ת����
					csk->sk_dip = cb->saddr;
					csk->sk_sport = cb->dport;
					csk->sk_dport = cb->sport;

					csk->parent = tsk;
					csk->state = TCP_LISTEN;
					//csk->ref_cnt = 0;

					//init_list_head(&(csk->hash_list));
					//init_list_head(&(csk->bind_hash_list));

					// timewait�ṹ����һ�����̻��ʼ���������������

					tcp_set_state(csk, TCP_SYN_RECV);

					csk->iss = tcp_new_iss();
					//csk->snd_una =  tsk->snd_una;
					csk->snd_nxt = csk->iss;
					csk->rcv_nxt = cb->seq_end;

					skaddr.ip = htonl(csk->sk_sip);
					skaddr.port = htons(csk->sk_sport);

					tcp_hash(csk);
					tcp_sock_bind(csk, &skaddr);
					

					tcp_send_control_packet(csk, (TCP_SYN | TCP_ACK)); // �ڶ���������ҪSYN��ACK
					list_add_tail(&(csk->list), &(tsk->listen_queue));

				

					
				}
				break;
			}
		case TCP_SYN_RECV: // ���״̬�Ƿ������˷��͵ڶ��������źŵȴ����������ֵ�״̬
			{
				if(cb->flags & TCP_ACK) // �����tskʵ������֮ǰ��csk
				{
					tsk->snd_una = cb->ack;
					tsk->rcv_nxt = cb->seq_end;
					tsk->adv_wnd = cb->rwnd;
					tcp_update_window_safe(tsk, cb);
					if(tcp_update_send_buf(tsk, cb)) // prj13 added
					{
						tcp_reset_retrans_timer(tsk); // prj13 added
					}
					tcp_sock_accept_enqueue(tsk); // �˺����Դ��˳�listen queue��parent ac_backlog+1�Ĺ���
					tcp_set_state(tsk, TCP_ESTABLISHED);
					if(tsk->parent->wait_accept->sleep)
					{
						wake_up(tsk->parent->wait_accept);
					}
					
				}
				break;
			}
		case TCP_SYN_SENT: // ���״̬�ǿͻ��˷��͵�һ�����ֵȴ�����ظ���״̬
			{
				if((cb->flags & (TCP_SYN | TCP_ACK)) == (TCP_SYN | TCP_ACK)) // ��������λ��Ϊ1���У����������if(cb->flags & (TCP_SYN | TCP_ACK))����ֻ��1��λ��1ҲΪ��
				{
					tsk->snd_una = cb->ack;
					tsk->rcv_nxt = cb->seq_end;
					tsk->adv_wnd = cb->rwnd;
					tcp_update_window_safe(tsk, cb);
					if(tcp_update_send_buf(tsk, cb)) // prj13 added
					{
						tcp_reset_retrans_timer(tsk); // prj13 added
					}

					tcp_send_control_packet(tsk, TCP_ACK);
					tcp_set_state(tsk, TCP_ESTABLISHED);
					if(tsk->wait_connect->sleep)
					{
						wake_up(tsk->wait_connect);
					}

				}
				break;
			}
			
		
		case TCP_ESTABLISHED: // ��������
			{

				// printf("debug: in the front of case TCP_ESTABLISHED.\n");

				if(cb->flags & TCP_FIN) // ���������յ��˵�һ�λ�����Ϣ
				{
					tsk->snd_una = cb->ack;
					tsk->rcv_nxt = cb->seq_end;
					tsk->adv_wnd = cb->rwnd;



					tcp_update_window_safe(tsk, cb);

					tcp_send_control_packet(tsk, TCP_ACK);
					tcp_set_state(tsk, TCP_CLOSE_WAIT);

					if(tsk->wait_recv->sleep)
					{
						// printf("debug: before wake up wait_recv in case ESTABLISHED.\n");
						wake_up(tsk->wait_recv);
					}

				}
				else if(cb->flags & TCP_ACK) // ��������»��ǻ���ACK��־λ��
				{
					// printf("debug: before wake up wait_recv.\n");
					
					//printf("debug: seq=%u, ack=%u\n", (cb->seq), (cb->ack - ));

					int is_new_packet = 0;
					
					//if(less_than_32b(tsk->rcv_nxt, cb->seq_end)) // �°�
					if(less_or_equal_32b(tsk->rcv_nxt, cb->seq_end))
					{
						tsk->snd_una = cb->ack;
						
						tsk->adv_wnd = cb->rwnd;
						is_new_packet = 1;

					}
										

					if(tcp_update_send_buf(tsk, cb)) // prj13 added
					{
						tcp_reset_retrans_timer(tsk); // prj13 added
					}

					if(cb->pl_len != 0) // ������tcp�����ģ��Ͱ���������rbuf���棬�ٻ���wait recv
					{
						/*
						pthread_mutex_lock(&(tsk->rcv_buf->lock));
						write_ring_buffer(tsk->rcv_buf, cb->payload, cb->pl_len);
						
						tsk->rcv_wnd -= cb->pl_len; // ���մ�����С

						if(tsk->wait_recv->sleep)
						{
							// printf("debug: before wake up wait_recv.\n");
							wake_up(tsk->wait_recv);
						}
						pthread_mutex_unlock(&(tsk->rcv_buf->lock));
						*/
						if(is_new_packet)
						{
							write_ofo_buffer(tsk, cb);
							tsk->rcv_wnd -= cb->pl_len; // ���մ�����С
							// tsk->rcv_nxt = cb->seq_end;
						}
						
						tcp_send_control_packet(tsk, TCP_ACK);
					}
					

					// if(is_new_packet)
					{
						tcp_update_window_safe(tsk, cb);
					}
					
				}
				// �ͻ��˷����һ�λ��֣���tcp_sock_close����ʵ����
				break;
			}
			
		
		/*
		case TCP_CLOSE_WAIT: // �����˵ڶ��λ��֣�׼�������λ���
			{
				tcp_send_control_packet(TCP_FIN | TCP_ACK);
				tcp_set_state(TCP_LAST_ACK);
				break;
			}
		*/ 
		// case TCP_CLOSE_WAIT ��tcp_sock_close����ʵ����
		case TCP_LAST_ACK:
			{
				if(cb->flags & TCP_ACK) // �������յ����Ĵλ��ֵ��ź�
				{
					tsk->snd_una = cb->ack;
					tsk->rcv_nxt = cb->seq_end;
					tsk->adv_wnd = cb->rwnd;
					tcp_update_window_safe(tsk, cb);
					tcp_update_send_buf(tsk, cb); // prj13 added
					tcp_unset_retrans_timer(tsk); // prj13 added

					tcp_set_state(tsk, TCP_CLOSED);
					tcp_unhash(tsk);
				}
				break;
			}
			
		
		case TCP_FIN_WAIT_1: // �ͻ����յ��������������ĵڶ��λ��ֵ��ź�
			{
				if((cb->flags & (TCP_FIN | TCP_ACK)) == (TCP_FIN | TCP_ACK)) // ���������PPT˵���������ݰ��ϲ���2/3�λ��ֺ�һ����
				{
					tsk->snd_una = cb->ack;
					tsk->rcv_nxt = cb->seq_end;
					tsk->adv_wnd = cb->rwnd;
					tcp_update_window_safe(tsk, cb);
					if(tcp_update_send_buf(tsk, cb)) // prj13 added
					{
						tcp_reset_retrans_timer(tsk); // prj13 added
					}

					tcp_send_control_packet(tsk, TCP_ACK);
					tcp_set_state(tsk, TCP_TIME_WAIT);
					tcp_set_timewait_timer(tsk);

					printf("debug: server merges packets.\n");
				}
				else if(cb->flags & TCP_ACK) // ������4�λ�������
				{
					tsk->snd_una = cb->ack;
					tsk->rcv_nxt = cb->seq_end;
					tsk->adv_wnd = cb->rwnd;
					tcp_update_window_safe(tsk, cb);
					if(tcp_update_send_buf(tsk, cb)) // prj13 added
					{
						tcp_reset_retrans_timer(tsk); // prj13 added
					}

					tcp_set_state(tsk, TCP_FIN_WAIT_2);
				}
				break;
			}
			
		case TCP_FIN_WAIT_2:// �ͻ����յ��������������ĵ����λ��ֵ��ź�
			{
				// printf("debug: in case TCP_FIN_WAIT_2\n");

				if((cb->flags & (TCP_FIN | TCP_ACK)) == (TCP_FIN | TCP_ACK))
				{
					tsk->snd_una = cb->ack;
					tsk->rcv_nxt = cb->seq_end;
					tsk->adv_wnd = cb->rwnd;
					tcp_update_window_safe(tsk, cb);
					tcp_update_send_buf(tsk, cb); // prj13 added
					

					tcp_send_control_packet(tsk, TCP_ACK);
					tcp_set_state(tsk, TCP_TIME_WAIT);
					
					
					// printf("debug: case WAIT2 before set timer.\n");
					
					
					tcp_set_timewait_timer(tsk);
					tcp_unset_retrans_timer(tsk); // prj13 added
				}
				break;
			}
			

		//case TCP_CLOSING:
			//break;
			// ���״̬����Ҫʵ��
		
		//case TCP_TIME_WAIT:
		//	break;
		// tcp_scan_timer_listʵ�������״̬��CLOSED״̬��ת��

		default:
		{
			printf("Error. Impossible case:%s\n", tcp_state_str[tsk->state]);
			// exit(-1);
			break;
		}
			
	}

	//printf("after:%s\n", tcp_state_str[tsk->state]);
}


