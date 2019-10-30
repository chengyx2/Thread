#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"


// TCB contains TID, context, stack, state of the thread, and return value
typedef enum uthread_state {running, 
                            ready, 
                            blocked, 
                            zombie
  }uthread_state;

struct TCB
{
  uthread_t TID;
  uthread_ctx_t context;
  void * stack;
  uthread_state state;
  int retval;
};

int tid_thread = 1;
struct TCB * current_thread;
struct TCB * blocked_thread;


static queue_t ready_queue = NULL;
static queue_t blocked_queue = NULL;
static queue_t zombie_queue;


/*
 * Yield for other threads to execute 
 * from the currently active and running thread 
 */
void uthread_yield(void)
{
  struct TCB * next_thread = NULL;
  struct TCB *temp = current_thread;
  
  //If no thread to be excuted, current_thread continue running 
  if (ready_queue == NULL) {
	  printf("continue with current thread\n");
    return;
  }
  
  //Chage state of running and active current_thread into ready
  //If the current_therad state is blocked or zombie, yield does not happen
  //if(current_thread->state == running){
	  
    current_thread->state = ready;
  //}
  
  //Enqueue current_thread into ready_queue
  queue_enqueue(ready_queue,(void*)current_thread);
  
  //Dequeue next_thread from ready_queue
  queue_dequeue(ready_queue,(void*)&next_thread);
  
  
  // If nothing in queue after dequeue, deallocate and destry the queue.
  if (queue_length(ready_queue) == 0){
    queue_destroy(ready_queue);
    ready_queue = NULL;
  }
  
  //Chage state of next_thread into running
  //Now next_thread state becomes running
  next_thread->state = running;
 
 current_thread = next_thread;
 
  //Switch context between current_thread and next_thread
  uthread_ctx_switch(&temp->context,&next_thread->context);
  
  //Set next_thread as current_thread(running)
 
} 

//Get the TID of currently running thread
uthread_t uthread_self(void)
{
  return current_thread->TID;
}


/*This function creates a new thread running the function @func to which
 *argument @arg is passed, and returns the TID of this new thread.
 */
int uthread_create(uthread_func_t func, void *arg)
{

  struct TCB * new_thread = (struct TCB *)malloc(sizeof(struct TCB));
  
  //return -1 in case of failure of memory allocation
  if(!new_thread){
    perror("Failure of memory allocation");
    return -1;
  }
  
  // if there is no thread, create main thread
  if( (ready_queue == NULL && zombie_queue == NULL && blocked_queue == NULL)
       || (tid_thread < 1) ){
  
    struct TCB * main_thread = (struct TCB *)malloc(sizeof(struct TCB));
    
    if(!main_thread){
      perror("Failure of memory allocation");
      return -1;
    }
    
    //Initialize main thread TCB
    //Here, mian thread doesn't allocate stack
    main_thread->TID = 0;
    main_thread->state = running;
    current_thread = main_thread;
    
    //Allocate an empty queue to ready_queue, blocked_queue, and zombie_queue
    ready_queue = queue_create();
    blocked_queue = queue_create();
    zombie_queue = queue_create();
    
  }
  

  // initialize state and TID of new_thread
  new_thread->state = ready;
  new_thread->TID = tid_thread;
  tid_thread++;
  
  // Allocate stack segment
  new_thread->stack = uthread_ctx_alloc_stack();
  if (new_thread->stack == NULL){
    perror("Failure of stack allocation");
    return -1;
  }
  
  
  // return -1 if it was not properly initialized
  int initialization = uthread_ctx_init(&new_thread->context, &new_thread->stack, func, arg);
  
  // check properly initialized
  if (initialization == -1) {
    perror("Failure of initialization");
    return -1;
  }
  
  //Enqueue new thread into ready_queue
  queue_enqueue(ready_queue,new_thread);
  return new_thread->TID;
}

// Exit from currently running thread
void uthread_exit(int retval)
{
  struct TCB *temp = current_thread;
  //if thread is the main one, free and exit
  if(current_thread->TID == 0){
    queue_destroy(ready_queue);
    queue_destroy(blocked_queue);
    queue_destroy(zombie_queue);
    exit(EXIT_SUCCESS);
  }
  
  //Update current_thread retval and state
  current_thread->retval = retval;
  current_thread->state = zombie;
  
  
  if (zombie_queue == NULL){
    zombie_queue = queue_create();
  }
  //Enqueue current_thread into the zombie_queue
  queue_enqueue(zombie_queue,(void*)current_thread);
  
  /* blocked_thread_checker return -1 if @queue or @data are NULL,
   * or if the queue is empty. 
   * 0 if @data was set with the oldest item available in @queue.
  */
  if(ready_queue == NULL) {
	  int blocked_thread_checker = queue_dequeue(blocked_queue,(void*)&blocked_thread);
		
	  //if there is any waiting thread,
	  if(blocked_thread_checker != -1){
		
	  //Dequeue blocked_thread(waiting thread) from the blocked_queue
	  queue_dequeue(blocked_queue,(void*)&blocked_thread);
	  
	  //Set the blocked_thread as running current_thread
	  blocked_thread->state = running;
		 current_thread = blocked_thread;
	 uthread_ctx_switch(&temp->context,&blocked_thread->context);
	}
  }
  else {
	queue_dequeue(ready_queue,(void*)&blocked_thread);
	blocked_thread->state = running;
		 current_thread = blocked_thread;
	 uthread_ctx_switch(&temp->context,&blocked_thread->context);
  }
}

//check if the Tid of data(thread) is equal to arg's
int check_tid (void *data, void *arg)
{
  
	struct TCB*temp = malloc(sizeof(struct TCB));
	temp = data;
	
 
  // if thread's TID is equal to arg's, return 1
  if(temp->TID == (*(uthread_t*)arg) )
  {
    return 1;
  }
  // if not, return 0
	return 0;
  
}


int uthread_join(uthread_t tid, int *retval)
{

struct TCB * thread_to_join = malloc(sizeof(struct TCB));
struct TCB * thread_to_join1= malloc(sizeof(struct TCB));
struct TCB * thread_to_join2 = malloc(sizeof(struct TCB));  
  
  //queue_iterate : return -1 if queue or function are NULL, 0 otherwise
  
  
  int ready_check = queue_iterate(ready_queue, check_tid, &tid ,(void*)&thread_to_join);

int zombie_check = queue_iterate(zombie_queue, check_tid, &tid ,(void*)&thread_to_join1);   

 int blocked_check = queue_iterate(blocked_queue, check_tid, &tid ,(void*)&thread_to_join2);
         
      
  //if the thread to join state is active(ready state)
  if (ready_check == 0)
  {
	  struct TCB *temp = current_thread;
    
    //queue_iterate(ready_queue, check_tid, &tid ,(void*)&thread_to_join);
    
    if (thread_to_join){
    //current_thread must be blocked
    current_thread->state = blocked;

    //Enqueue current_thread into blocked_queue
    queue_enqueue(blocked_queue, (void*)current_thread);
    
    //Remove the oldest item of ready_queue and assign this item to thread_to_join
    queue_dequeue(ready_queue, (void*)&thread_to_join);
    
    //Chage state of the thread_to_join(thread to join) to running 
    thread_to_join->state = running;
    
	current_thread = thread_to_join;
    
    //Switch context between current_thread and thread_to_join(thread to join)
    uthread_ctx_switch(&temp->context,&thread_to_join->context);
    
    //Set thread_to_join as current_thread
    }
    return -1;
  }
  
  //if the thread to join is dead(zombie state),
 if (zombie_check == 0)
  {
    //queue_iterate(zombie_queue, check_tid, &tid ,(void*)&thread_to_join);   
    if (thread_to_join1){
      
    
    //Set the return value of thread_to_join(thread to join which is dead)
    if(retval){
      *retval = thread_to_join1->retval;
    }
      //Dequeue thread_to_join(thread to join which is on zombie state) from zombie_queue
      queue_dequeue(zombie_queue,(void*)&thread_to_join1);
      //Deallocate stack segment of thread_to_join
      uthread_ctx_destroy_stack(&thread_to_join1->stack);
      //thread_to_join can be finally freed
      free(thread_to_join1);
	}
    return 0;
  }

  
  //if the thread to join is blocked(waiting)
  if (blocked_check == 0)
  {
	  struct TCB *temp = current_thread;
    //queue_iterate(blocked_queue, check_tid, &tid ,(void*)&thread_to_join);
    
    if (thread_to_join2)
	{
      
    //current_thread must be blocked
    current_thread->state = blocked;
    
    //Enqueue current_thread into blocked_queue
    queue_enqueue(blocked_queue, (void*)current_thread);
    
    //Remove the oldest item of ready_queue and assign this item to thread_to_join
    queue_dequeue(ready_queue, (void*)&thread_to_join2);
    
    //Chage state of the thread_to_join(thread to join) to running 
    thread_to_join2->state = running;
   
 current_thread = thread_to_join2;
      
    //Switch context between current_thread and thread_to_join(thread to join)
    uthread_ctx_switch(&temp->context,&thread_to_join2->context);
    
    //Set thread_to_join as current_thread
   
	}
    return 0;
  }
  
  return -1;
  
}
