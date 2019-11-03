/*
 * =====================================================================================
 *
 *       Filename:  t3.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年11月03日 20时26分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */

#define HAVE_STRUCT_TIMESPEC

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int random_array[16];
int random_array_index;
int result_array[8];
int result_array_index;




typedef class monitor
{
public:
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	void init();
	int getTask();
	void putResult(int sum);


}monitor;

void monitor::init()
{
	int i;
	for (i = 0; i < 16; i++)
	{
		random_array[i] = rand() % 100;
	}
	random_array_index = 0;
	result_array_index = 0;
	mutex = PTHREAD_MUTEX_INITIALIZER;
	cond = PTHREAD_COND_INITIALIZER;

}

int monitor::getTask()
{
	pthread_mutex_lock(&mutex);


	pthread_mutex_unlock(&mutex);
	return random_array[random_array_index++];

}

void monitor::putResult(int sum)
{
	pthread_mutex_lock(&mutex);

	result_array[result_array_index++] = sum;
	pthread_mutex_unlock(&mutex);
}

monitor ms;

void* thread_func(void*)
{
	int num1, num2;
	int result;
	int sleep_time;
	num1 = ms.getTask();
	num2 = ms.getTask();
	sleep_time = rand() % 9000 + 1000;
	usleep(sleep_time);
	result = num1 + num2;
	printf("%d + %d = %d\n", num1, num2, result);

	ms.putResult(result);
	return NULL;
}

int main()
{
	
	pthread_t t[8];
	ms.init();
	int final;
	printf("array: ");
	for (auto i : random_array)
	{
		printf("%d ", i);
	}
	printf("\n");
	for (int i = 0; i < 8; i++)
	{
		pthread_create(&t[i], NULL, thread_func, NULL);
	}
	
	for (int i = 0; i < 8; i++)
	{
		pthread_join(t[i], NULL);
	}
	
	final = 0;
	for (int i = 0; i < 8; i++)
	{
		final += result_array[i];
	}

	printf("final result is %d\n", final);
}
