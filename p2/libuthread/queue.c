#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

struct node {
	void *data;		//data that needs to be saved/used
	struct node *next;		//node pointer to next node in linked list
};

struct queue {
	struct node *head;		//the start or oldest in queue
	struct node *tail;		//the last and newest in queue
	int length;		//the length of queue
};

queue_t queue_create(void)
{
	//dynamically creating the queue 
	queue_t queue = (queue_t) malloc (sizeof(struct queue));

	//dynamically creating the front and back nodes
	queue->head = (struct node*) malloc (sizeof(struct node));
	queue->tail = (struct node*) malloc (sizeof(struct node));

	//settings the data and pointer to next as NULL pointer
	queue->head->data = queue->head->next = NULL;
	queue->tail->data = queue->tail->next = NULL;
	
	//setting the length of new queue as 0
	queue->length = 0;
	return queue;

}

int queue_destroy(queue_t queue)
{
	//checking if queue is created and that queue is not empty
	if(queue == NULL || queue->head->data != NULL) {
		return -1;
	}

	//deallocating
	free(queue->head);
	free(queue);

	return 0;		//success = 0, error = -1
}

int queue_enqueue(queue_t queue, void *data)
{
	//checking if queue is created and data to enqueue is not empty
	if(queue == NULL || data == NULL) {
		return -1;
	}

	//creating new node with data inside
	struct node *temp = (struct node*) malloc (sizeof(struct node));
	if(temp == NULL) { //memory allocation checker
			return -1;
		}
	temp->data = data;		
	temp->next = NULL;

	if(queue->tail->data == NULL) { //first enqueue
		queue->head = queue->tail = temp;		//copying the temp node into tail and head
		queue->length = 1;		//increasing length by one
	} else { //not first time enqueueing

		queue->tail->next = temp;		//adding the newly created node to last of list
		queue->tail = temp;		//setting the last node pointer to newly created node

		queue->length += 1;		//increasing length by one
	}

	return 0;		//success = 0, error = -1
}

int queue_dequeue(queue_t queue, void **data)
{
	//checking if queue is create and queue is not empty
	if(queue == NULL || data == NULL || queue->head->data == NULL) {
		return -1;
	}

	//saving the old data 
	*data = queue->head->data;

	//creating temporary node to deallocate
	struct node *temp = (struct node*) malloc (sizeof(struct node));
	if(temp == NULL) { //memory allocation checker
		return -1;
	}
	temp = queue->head;		//saving the queue into temporary node

	queue->length -= 1;		//decreasing the length of queue by one

	//if only one data to dequeue
	if(queue->length == 0) {
		queue->head->data = NULL;
		queue->tail->data = NULL;
	} else {
		//setting the queue to start from next in queue
		queue->head = queue->head->next;
		free(temp);		//deallocating the oldest queue
	}
	return 0;		//success = 0, error = -1
}

int queue_delete(queue_t queue, void *data)
{
	//checking if queue is created and data to delete in not empty
	if(queue == NULL || data == NULL) {
		return -1;
	}

	//creaing temporary node to iterate
	struct node *temp = (struct node*) malloc (sizeof(struct node));
	if(temp == NULL) { //memory allocation checker
		return -1;
	}
	temp = queue->head;

	
	//iterate through whole queue unless found
	while(temp != NULL) {
		//check if exist
		if(temp->data == data) { //found
			temp = temp->next;		//deleting current node pointer and letting it be the next node pointer instead
			queue->length -= 1;		//decrease length of queue
			return 0;
		}

		//go to next node
		temp = temp->next;
	}

	return -1; //could not find matching data
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	//checking if queue is empty and if function is empty
	if(queue == NULL || func == NULL){
		return -1;
	}

	//creating temporary node to iterate
	struct node *temp = (struct node*) malloc (sizeof(struct node));
	if(temp == NULL) { //memory allocation checker
		return -1;
	}
	temp = queue->head;


	//keep running till end of linked list
	while(temp != NULL) {

		//check if data is not empty
		if(temp->data != NULL) {
			//apply the function given, 1 = stop/found, 0 = keep going
			int end = func(temp->data, arg);
		
			//found 
			if(end == 1) {
				*data = temp->data;		//save the data found
				return 0;
			}
		}
		//go to next node
		temp = temp->next;
	}

	return 0;		//success = 0, error = -1
}

int queue_length(queue_t queue)
{
	//checking if queue is empty
	if(queue == NULL) {
		return -1;
	}

	return queue->length;		//returns the length of queue
}
