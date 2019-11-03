/*
 * =====================================================================================
 *
 *       Filename:  sample.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年11月03日 16时20分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/sem.h>

sem_t sem;
    
void *func1(void *arg)
{
    sem_wait(&sem);
    int *running = (int *)arg;
    printf("thread func1 running : %d\n", *running);
    
    pthread_exit(NULL);
}
    
void *func2(void *arg)
{
    printf("thread func2 running.\n");
    sem_post(&sem);
    
    pthread_exit(NULL);
}
    
int main(void)
{
    int a = 3;
    sem_init(&sem, 0, 0);
    pthread_t thread_id[2];
    
    pthread_create(&thread_id[0], NULL, func1, (void *)&a);
    printf("main thread running.\n");
    sleep(10);
    pthread_create(&thread_id[1], NULL, func2, (void *)&a);
    printf("main thread still running.\n");
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    sem_destroy(&sem);
    
    return 0;
}
