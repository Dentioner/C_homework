/*
 * =====================================================================================
 *
 *       Filename:  t1.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月19日 16时12分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


//__uint32_t ar[65536];
__uint32_t *ar;

struct sigaction act;

int index1;

void sigsegvHandler(int sig, siginfo_t *info, void *secret)
{
//	printf("in handler\n");
//	index1++;
	printf("Got SIGSEGV at address: 0x%lx\n", (long)info->si_addr);
	mprotect(ar, 0x40000, PROT_WRITE);
        act.sa_flags = SA_SIGINFO;
        sigemptyset(&act.sa_mask);
        act.sa_sigaction = sigsegvHandler;
        sigaction(SIGSEGV, &act, NULL);
	//index1++;
	//sigemptyset(&act.sa_mask);


	//ar += index1*1000
	

}


int main()
{

	//int index1;
	__uint32_t tmp;

	

	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = sigsegvHandler;
	sigaction(SIGSEGV, &act, NULL);

/*	memset((void *)(&act), 0, sizeof(act));
	act.sa_handler = &sigsegvHandler;
	sigaction(SIGSEGV, &act, NULL);
*/	

	//ar = (__uint32_t *)malloc(0x40000 * sizeof(char));
	ar = memalign(0x10000, 4*0x10000);
	mprotect(ar, 0x40000, PROT_READ);

	for(index1 =0; index1 < 64; index1++)
	{
		tmp = index1 + 1;
		ar[index1*1000] = tmp;
	
		//mprotect((void *)(&ar[index1*1000]), 0x40000-index1*1000, PROT_READ);
	
	//	mprotect((void *)(&ar[index1*1000]), 0x40000, PROT_READ);
	mprotect(ar, 0x40000, PROT_READ);
	}

	printf("*******************\n");	
	return 0;
}
