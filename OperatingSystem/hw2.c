#include<stdio.h>
#include<sys/time.h>


int main()
{
	struct timeval tv1;
	struct timeval tv2;
	struct timeval tv3;
	struct timeval tv4;
	long answer, answer2;
	double answer3;
	gettimeofday(&tv1, NULL);//1
	for(long int i = 0; i < 100000000; i++)
		gettimeofday(&tv2, NULL);
	
	gettimeofday(&tv3, NULL);//1
	for(long int i = 0; i < 100000000; i++)
		;
	gettimeofday(&tv4, NULL);
	
	answer = (long)( tv2.tv_usec - tv1.tv_usec);
	//printf("executing time is %ld us\n", answer);
	answer2 = (long)( tv4.tv_usec - tv3.tv_usec);
	//printf("executing time is %ld us\n", answer2);
	answer3 = (double)(answer - answer2)/100000000;
	printf("executing time is %f us\n", answer3);
	return 0;
}
