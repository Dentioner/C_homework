#include<stdio.h>
#include<sys/time.h>


int main()
{
	struct timeval tv1;
	struct timeval tv2;
	struct timeval tv3;
	struct timeval tv4;
	struct timezone * tz;
	long answer, answer2;
	double answer3;
	tz = NULL;
	asm("int $0x80" ::"a"(78), "b"(&tv1), "c"(tz));
	for(long int i = 0; i < 100000; i++)
		asm("int $0x80" :: "a"(78), "b"(&tv2), "c"(tz));
	
	asm("int $0x80" :: "a"(78), "b"(&tv3), "c"(tz));
	for(long int i = 0; i < 100000; i++)
		;
	
	asm("int $0x80" :: "a"(78), "b"(&tv4), "c"(tz));
	answer = (long)( tv2.tv_usec - tv1.tv_usec);
	//printf("executing time is %ld us\n", answer);
	answer2 = (long)( tv4.tv_usec - tv3.tv_usec);
	//printf("executing time is %ld us\n", answer2);
	answer3 = (double)(answer - answer2)/100000;
	printf("executing time is %f us\n", answer3);
	return 0;
}
