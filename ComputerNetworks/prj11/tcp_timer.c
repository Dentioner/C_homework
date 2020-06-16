#include "tcp.h"
#include "tcp_timer.h"
#include "tcp_sock.h"

#include <stdio.h>
#include <unistd.h>

static struct list_head timer_list;

// scan the timer_list, find the tcp sock which stays for at 2*MSL, release it
void tcp_scan_timer_list()
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);
	struct tcp_timer *timer_p, *timer_q;
	timer_p = NULL;
	timer_q = NULL;

	list_for_each_entry_safe(timer_p, timer_q, &(timer_list), list)
	{
		timer_p->timeout += TCP_TIMER_SCAN_INTERVAL; // 因为每次主函数睡这么久，每次执行的间隔就是这么长
		if(timer_p->timeout > TCP_TIMEWAIT_TIMEOUT)
		{
			list_delete_entry(&(timer_p->list));
			// free(timer_p);
			struct tcp_sock *tmp_tsk = timewait_to_tcp_sock(timer_p); // 要把这个timer所在的整个sock搞掉
			tcp_bind_unhash(tmp_tsk);
			tcp_set_state(tmp_tsk, TCP_CLOSED);
			free_tcp_sock(tmp_tsk);
		}
	}
	
	return;
}

// set the timewait timer of a tcp sock, by adding the timer into timer_list
void tcp_set_timewait_timer(struct tcp_sock *tsk)
{
	// fprintf(stdout, "TODO: implement %s please.\n", __FUNCTION__);

	tsk->timewait.enable = 1;
	tsk->timewait.timeout = 0;
	tsk->timewait.type = 0;
	list_add_tail(&(tsk->timewait.list), &(timer_list));

	return;
}

// scan the timer_list periodically by calling tcp_scan_timer_list
void *tcp_timer_thread(void *arg)
{
	init_list_head(&timer_list);
	while (1) {
		usleep(TCP_TIMER_SCAN_INTERVAL);
		tcp_scan_timer_list();
	}

	return NULL;
}
