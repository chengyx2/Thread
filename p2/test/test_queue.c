#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include <queue.h>

/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg)
{
	int *a = (int*)data;
	int inc = (int)(long)arg;

	*a += inc;

	return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
	int *a = (int*)data;
	int match = (int)(long)arg;

	if (*a == match)
		return 1;

	return 0;
}

//checking if queue is created
void test_create(void) {
	queue_t q;

	q = queue_create();
    assert(q != NULL);

}

//checking if enqueue and dequeue works
void test_enqueue_dequeue(void) {

	queue_t q;
	int data = 3;
	int *ptr;

	q = queue_create();
	//checkinf if enqueue and dequeue works
	assert(queue_enqueue(q, &data) == 0);
	assert(queue_dequeue(q, (void**)&ptr) == 0);
	
	//checking if correct data was dequeued
	assert(ptr == &data);

}

//checking if destory works
void test_destroy(void) {

	queue_t q;
	int data = 3, *ptr;
	
	//creating queue
	q = queue_create();
	
	//enqueing once and dequeing once 
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	
	//should be empty queue
	assert(queue_destroy(q) == 0);		

}

//checking if delete works
void test_delete() {

	queue_t q;
	int data1[] = {1, 2, 3, 4, 5};		//data with 5 separate numbers

	q = queue_create();
	
	//only enqueing number 2 and 4 into queue
	queue_enqueue(q, &data1[1]);		
	queue_enqueue(q, &data1[3]);

	//checking if only 2 and 4 is in queue and deleting
	assert(queue_delete(q, &data1[0]) == -1);
	assert(queue_delete(q, &data1[1]) == 0);
	assert(queue_delete(q, &data1[2]) == -1);
	assert(queue_delete(q, &data1[3]) == 0);
	assert(queue_delete(q, &data1[4]) == -1);

}

//checking if iterate works
void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
}

//checking if length works
void test_getLength(void) {

	queue_t q;
	int data[] = {1, 2, 3};		//creating array with 3 data

	//enqueing 3 data into queue
	q = queue_create();
	queue_enqueue(q, &data[0]);
	queue_enqueue(q, &data[1]);
	queue_enqueue(q, &data[2]);
	
	//checking if length is 3
	assert(queue_length(q) == 3);

}

int main(void) {	

	//testing in main
	test_create();
	test_enqueue_dequeue();
	test_destroy();
	test_delete();
	test_iterator();
	test_getLength();

	return 0;
}
