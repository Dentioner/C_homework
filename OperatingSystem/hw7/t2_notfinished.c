/*
 * =====================================================================================
 *
 *       Filename:  t2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年11月03日 15时50分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<semaphore.h>

#define N 2
#define M 8
void *teller(void *);

void *customer(void *);

sem_t full, empty, mutex, mutex2;

static int queue_number;


int main()
{
	pthread_t s[N];
	pthread_t c[M];
	sem_init(&remain, 0, N);
	sem_init(&mutex, 0 ,1);
	sem_init(&mutex2, 0 ,1);
	int number[N+1];
	

	
	for(int i = 0; i < N; i++)
	{
		pthread_create(&s[i], NULL, &teller, NULL);
	}

	for(int i = 0; i < 3*N; i++)
	{
		number[i] = i;
		pthread_create(&c[i], NULL, &customer, (void*)&number[i]);
	}

//	sleep(3);
	while(1);
//	for(int i = 0; i < N + 1; i++)
//	{
//		pthread_join(&c[i], NULL);
//	}
	return 0;
}


void *customer(void *arg)
{
	int * num = (int *)arg;
	int my_queue_num;
	
	sem_wait(&mutex);
	my_queue_num = queue_number++;
	sem_post(&mutex);


	printf("(%d)I'm waiting for a free teller. I get number %d\n", *num, my_queue_num);

	sem_wait(&remain);
	sem_wait(&mutex2);	
	printf("I'm finished using this teller.(%d)\n", *num);
	sem_post(&mutex2);
	sem_post(&remain);

//	}
}


void *teller(void *arg)
{
	while(1)
	{
		sem_wait(&empty);
		sem_wait(&mutex);

