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