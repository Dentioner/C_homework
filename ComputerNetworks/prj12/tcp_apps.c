#include "tcp_sock.h"

#include "log.h"

#include <stdlib.h>
#include <unistd.h>



int file_mode = 1;

// tcp server application, listens to port (specified by arg) and serves only one
// connection request
void *tcp_server(void *arg)
{
	u16 port = *(u16 *)arg;
	struct tcp_sock *tsk = alloc_tcp_sock();

	struct sock_addr addr;
	addr.ip = htonl(0);
	addr.port = port;
	if (tcp_sock_bind(tsk, &addr) < 0) {
		log(ERROR, "tcp_sock bind to port %hu failed", ntohs(port));
		exit(1);
	}

	if (tcp_sock_listen(tsk, 3) < 0) {
		log(ERROR, "tcp_sock listen failed");
		exit(1);
	}

	log(DEBUG, "listen to port %hu.", ntohs(port));

	struct tcp_sock *csk = tcp_sock_accept(tsk);

	log(DEBUG, "accept a connection.");



	if(file_mode)
	{
		FILE *fp = fopen("server-output.dat", "w");
		if(fp == NULL)
		{
			printf("Error in fopen.\n");
			exit(-1);
		}

		int rlen;
		char tmp_buf[1000];


		while(1)
		{
			// printf("debug: current rcv wnd:%d\n", csk->rcv_wnd);

			rlen = 0;
			memset((void *)tmp_buf, 0, 1000);

			rlen = tcp_sock_read(csk, tmp_buf, 1000);

			if (rlen == 0) // 读到流结尾，对方关闭连接
			{
				log(DEBUG, "tcp_sock_read return 0, finish transmission.");
				break;
			}
			else if(rlen > 0)
			{
				fwrite((void *)tmp_buf, sizeof(char), rlen, fp);
				// tcp_send_control_packet(csk, TCP_ACK);
			} 
			else
			{
				log(DEBUG, "tcp_sock_read return negative value, something goes wrong.");
				exit(1);
			}
			

			
		}

		fclose(fp);
		
		
		
	}
	else
	{

		// 从这里开始是prj12修改的

		char rbuf[1001];
		char wbuf[1024];
		int rlen = 0;
		while (1) 
		{
			// printf("debug: in the front of while(1)\t");

			rlen = tcp_sock_read(csk, rbuf, 1000);

			// printf("rlen = %d\t", rlen);

			if (rlen == 0) // 读到流结尾，对方关闭连接
			{
				log(DEBUG, "tcp_sock_read return 0, finish transmission.");
				break;
			} 
			else if (rlen > 0) // 正常情况
			{
				rbuf[rlen] = '\0';
				sprintf(wbuf, "server echoes: %s", rbuf); // 复读机
				if (tcp_sock_write(csk, wbuf, strlen(wbuf)) < 0)
				{
					log(DEBUG, "tcp_sock_write return negative value, something goes wrong.");
					exit(1);
				}
			}
			else {
				log(DEBUG, "tcp_sock_read return negative value, something goes wrong.");
				exit(1);
			}

			// printf("in while(1)\n");
		}


	}
	
	log(DEBUG, "close this connection.");

	// printf("debug: server before tcp_sock_close.\n");

	tcp_sock_close(csk);
	
	return NULL;
}

// tcp client application, connects to server (ip:port specified by arg), each
// time sends one bulk of data and receives one bulk of data 
void *tcp_client(void *arg)
{

	struct sock_addr *skaddr = arg;

	struct tcp_sock *tsk = alloc_tcp_sock();

	if (tcp_sock_connect(tsk, skaddr) < 0) {
		log(ERROR, "tcp_sock connect to server ("IP_FMT":%hu)failed.", \
				NET_IP_FMT_STR(skaddr->ip), ntohs(skaddr->port));
		exit(1);
	}


	if(file_mode)
	{
		FILE *fp = fopen("client-input.dat", "r");
		if(fp == NULL)
		{
			printf("Error in fopen.\n");
			exit(-1);
		}

		long long unsigned int file_len, sent_len, data_len;
		char tmp_buf[1000];

		fseek(fp, 0, SEEK_END);
		file_len = ftell(fp); // 先将fp放到末尾以计算文件长度

		fseek(fp, 0, SEEK_SET); // 然后再回来
		sent_len = 0;
		int sleep_counter = 0;

		while(1)
		{
			// printf("debug: current peer rcv wnd = %d\n", tsk->adv_wnd);
			// printf("debug: file len=%llu\tsent_len=%llu\n", file_len, sent_len);
			memset(tmp_buf, 0, 1000);
			// data_len = min(1000, (file_len - sent_len));
			// fread((void *)tmp_buf, sizeof(char), (size_t)data_len, fp);		
			// data_len = fread((void *)tmp_buf, sizeof(char), 1000, fp);
			data_len = 0;
			while((feof(fp) == 0) && (data_len < 1000))
			{
				fscanf(fp, "%c", tmp_buf + (int)data_len);
				data_len++;
			}

			if(feof(fp))
			{
				data_len--;
				tcp_sock_write(tsk, tmp_buf, data_len);
				break;
			}
			else
			{
				data_len = 1000;
			}
			
			if (tcp_sock_write(tsk, tmp_buf, data_len) < 0)
			{
				printf("Error. client write error.\n");
			}

			sent_len += data_len;

			sleep_counter++;
			/*if(sleep_counter == 100)
			{
				sleep(1);
				sleep_counter = 0;
			}*/
			
		}

		printf("debug: file len=%llu\tsent_len=%llu\n", file_len, sent_len);

		fclose(fp);
		
		// sleep(1);
	}
	else
	{


		char *wbuf = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		int wlen = strlen(wbuf);
		char rbuf[1001];
		int rlen = 0;

		int n = 10;
		for (int i = 0; i < n; i++) 
		{

			// printf("debug: before tcp_sock_write. %dth cycle.\n", i);


			if (tcp_sock_write(tsk, wbuf + i, wlen - n) < 0)
				break;

			// printf("debug: before tcp_sock_read. %dth cycle.\n", i);

			rlen = tcp_sock_read(tsk, rbuf, 1000);

			// printf("debug: after  tcp_sock_read. %dth cycle.\n", i);

			if (rlen == 0) {
				log(DEBUG, "tcp_sock_read return 0, finish transmission.");
				break;
			}
			else if (rlen > 0) {
				rbuf[rlen] = '\0';
				fprintf(stdout, "%s\n", rbuf);
			}
			else {
				log(DEBUG, "tcp_sock_read return negative value, something goes wrong.");
				exit(1);
			}
			sleep(1);
		}

	}

	printf("debug: before tcp_sock_close in client.\n");

	tcp_sock_close(tsk);

	return NULL;
}
