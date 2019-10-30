## ECS 150 REPORT for HW2

## Phase 1

*Created a node(linked list) that contains the void data and node pointer to next node in linked list
*Created a struct to implement the front and last of queue
*Implemented the function "queue_create" to create a new queue
*Implemented the function "queue_destroy" to deallocate the maded queue
*Implemented the fcuntion "queue_enqueue" to add data into the queue
*Implemented the function "queue_dequeue" to remove the oldest data from the queue (FIFO Order)
*Implemented the function "queue_delete" to find and remove a certain data from oldest
*Implemented the function "queue_iterate" to go through each data in queue and apply a function to each data
*Implemented the function "queue_length" to return the length of queue
*Created test cases for the functions in "test_queue.c"

For our data structure,
1. Created a struct for the node, containing the data and node pointer to next node
2. Then created a struct for the queue list, containing the node pointer to the front and tail of queue and length

For the function "queue_create",
1. Allocated memeory for the queue
2. Allocated memory for for the head and tail node pointer
3. Set the value of both head and tail data to NULL
4. Set the value of the next pointer for both head nad tail to NULL
5. Set the length of queue to 0

For the function "queue_destroy",
1. Checked if queue was created and queue was empty
2. Deallocated the memeory of the pointer head and the queue itself

For the function "queue_enqueue",
1. Checked if queue was created and if the data to add was not NULL
2. Created a temporary node with data given and with next pointer set to NULL
3. Checked if first time enqueuing and if so, copied the temporary made node to both head and tail node pointer
4. If not first time, copy the temporary made node to next pointer in tail then set the node pointer of tail to be the temporary node
5. Increase the length by one

For the function "queue_dequeue", 
1. Checked if queue was created and queue is not empty
2. Saved the oldest data from node pointer Head 
3. Created a temporary node and copied the oldest node pointer head
4. Set the front of the queue to be the next pointer in head
5. Deallocated the old node pointer head

For the function "queue_delete",
1. Checked if queue was created and data to find was not NULL
2. Created a temporary node and copy original queue into it
3. Iterated through the queue to find data and once found, deleted the node pointer in that location

For the function "queue_iterate",
1. Checked if queue was created and data to find was not NULL
2. Created a temporary node and copy original queue into it
3. Iterated through the temporary queue and applied the function, which returns 0 if completed and found or 1 to stop
4. If found then data is saved and queue_iterate returns 0

For the function "queue_length", 
1. Checked if queue was created
2. Returned the length of queue saved inside the struct queue

For test cases in "test_queue.c"
1. Created 2 callback functions to use in "queue_iterate"
2. Tested if queue is created properly
3. Tested if queue is enquing and dequing properly
4. Tested deallocation by creating queue and enquing one data and dequting one data then calling "queue_destory"
5. Tested queue to use "queue_delete" by adding in certain data in a list and correctly finding those data
6. Tested iterate by incrementing all the data by one and finding the data that equals 5
7. Tested "queue_length" by enquing 3 data and seeing if 3 is returned 


## Phase 2
*Created a TCB struct to contain TID, context, stack, state of the thread, and return value
*Created int tid_thread which shows tid of thread
*Created a current_thread and blocked_thread
*Created ready, blocked, and zombie queue

For the function "uthread_yield",
(yield other threads to execute from the currently active and running thread)
1. Chage state of running and active current_thread into ready
2. Enqueue current_thread into ready_queue
3. Dequeue next_thread(other thread) from ready_queue if exists   
4. Chage state of next_thread into running 
5. Set next_thread as current_thread(running)
6. Switch context between current_thread and next_thread

For the function "uthread_t uthread_self",
1. get the TID of currently ruunning thread

For the function "uthread_create",
1. if there is no thread, create main thread
  - Set main thread state as running and TID as 0
  - Set main_thread as current_thread
  - Allocate an empty queue to ready_queue, blocked_queue, and zombie_queue
  
2. if there is main thread, create new thread
  - Set the new thread state as ready and TID as 1 which is tid_thread that was initialized as 0 and will be incrased by 1 later.
  - Allocate stack segment 
  - Enqeue new threaed into ready_queue

For the function "uthread_exit",
1. if thread is the main one, free and exit
2. Update current_thread retval and state as zombie
3. Enqueue current_thread into the zombie_queue
4. if ready_queue is null and there is any waiting thread, dequeue blocked_thread(waiting thread) from the blocked_queue and set the blocked_thread as running current_thread
5. if ready_queue is not null, dequeue blocked_thread(wating thread) from ready_queue, chage the state of blocked_thread into running, and set the blocked_thread as current_thread 

For the function "check_tid",
1. check if the Tid of data(thread) is equal to arg's
2. if thread's TID is equal to arg's, return 1 (if not return 0)

## Phase 3

*Implemented function "uthread_join" to make current_thread wait for the thread which has the given tid to complete

1. created ready_check, zombie_check, blocked_check to see if the state of the thread whose tid is same as the given tid
2. if the thread to join state is active(ready state)
  - Set current_thread as blocked
  - Enqueue current_thread into blocked_queue
  - Remove the oldest item of ready_queue and assign this item to thread_to_join
  - Chage state of the thread_to_join(thread to join) to running 
  - Set thread_to_join as current_thread
  - Switch context between current_thread and thread_to_join(thread to join)
3. if the thread to join is dead(zombie state),
  - Set the return value of thread_to_join(thread to join which is dead)
  - Dequeue thread_to_join(thread to join which is on zombie state) from zombie_queue
  - Deallocate stack segment of thread_to_join
  - thread_to_join can be finally freed
4. if the thread to join is blocked(waiting)
  - Did same process thing with active(ready state)

  
## Phase 4

*Created a sigaction struct to handle signals
*Created a itimerval struct to handle alarms
*Implemented function "signal_handler" to yield when signal occurs
*Implemented function "preempt_disable" to stop preempt in thread
*Implemented function "preempt_enable" to start preempt again in thread
*Implemented function "prement_start" to start preempt in thread 
*Created C file named "test_preempt" to test preempt

For function "signal_handler",
1. Called uthread_yield when signal is given

For function "preempt_disable",
1. Called sigprocmask with SIG_BLOCK option to stop signals

For function "preempt_enable", 
1. Called sigprocmask with SIG_UNBLOCK option to start signals again

For function "prement_start", 
1. Created the function sigaction and itimerval to handle signals and alarms
2. Set the alarm to signal every .01 second

For testing preempt,
1. In main, enabled preempt by calling function "preempt_start"
2. Created 3 threads to print their tid
3. Used uthread_join in main
4. Checked to see if every thread completed before main exited