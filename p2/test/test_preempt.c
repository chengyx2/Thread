#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

#include <queue.h>
#include <uthread.h>
#include <preempt.h>

/*
*	Testing preempt 
*	No uthread_yield() used
*	Only used timer to switch between threads
*/

int thread3(void* arg)
{
	printf("thread%d\n", uthread_self());
	return 0;
}

int thread2(void* arg)
{
	uthread_create(thread3, NULL);
	printf("thread%d\n", uthread_self());
	return 0;
}

int thread1(void* arg)
{
	uthread_create(thread2, NULL);
	printf("thread%d\n", uthread_self());
	return 0;
}

int main(void) {
	preempt_start();
	
	uthread_join(uthread_create(thread1, NULL), NULL);
}