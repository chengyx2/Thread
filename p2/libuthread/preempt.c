#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

//used to set up enabling and disabling preempt
static sigset_t sig;

//signal handler
void signal_handler(int signum)
{
	//if signal called -> switch from kernel/user
	uthread_yield();
}

//stop preempt
void preempt_disable(void)
{
	sigprocmask(SIG_BLOCK, &sig, NULL);
}

//start preempt
void preempt_enable(void)
{
	sigprocmask(SIG_UNBLOCK, &sig, NULL);
}

void preempt_start(void)
{

	//Install a signal handler that receives alarm signals (of type SIGVTALRM)
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &signal_handler;
	sa.sa_mask = sa.sa_mask;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGVTALRM, &sa, NULL);

	//Configure a timer which will fire an alarm (through a SIGVTALRM signal)
	// a hundred times per second (i.e. 100 Hz)
	struct itimerval alarm;
	alarm.it_interval.tv_sec = 0;
	alarm.it_interval.tv_usec = 1000000/HZ;
	alarm.it_value.tv_sec = 0;
	alarm.it_value.tv_usec = 1000000/HZ;
	setitimer(ITIMER_REAL, &alarm, NULL);

	//blocking and unblocking set up
	sigemptyset(&sig);
	sigaddset(&sig, SIGVTALRM);
}
