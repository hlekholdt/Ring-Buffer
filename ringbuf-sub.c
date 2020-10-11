#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "ringbuf.h"

/*
 * author: Hanna Ekholdt 
 * date: 9/6/20
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change "author" and continue work
 *
 * update: 9/11/20
 */


/* static variables (zeroed) */

int g_debug = 0 ;
int is_verbose = 0 ;
int is_full = 0;
int is_empty = 1;
struct RingBuf rb;

int rb_ioctl(int op) {
	switch (op) {
		case RB_Q_SIZE:
			return RINGBUF_SIZE ;
		case RB_IS_EMPTY:
			return is_empty;
		case RB_IS_FULL:
			return is_full;
		case RB_Q_COUNT:
			if(is_full){
				return RINGBUF_SIZE; //if array is full then it must be the size of array
			}
			if(is_empty){
				return 0;
			}
			return (RINGBUF_SIZE - rb.tail + rb.head) % RINGBUF_SIZE;		
		default:
			return 0;
		}
	return 0;
}

int rb_enqueue(int ele) {
	if(!rb_ioctl(RB_IS_FULL)){ //it will only add if ringbuf is not full	

		rb.ringbuf[rb.head] = ele;
		rb.head = (rb.head + 1) % RINGBUF_SIZE; //wraps around RINGBUF_SIZE (16)		
		is_empty = 0; //if anything has been added, it can't be empty
		if(rb.head == rb.tail){
			is_full = 1; //if the head is the same as the tail then it is full
		}
		return ele;
	}
	return -1;
}

int rb_dequeue(void) {
	int tailValue;

	if(!rb_ioctl(RB_IS_EMPTY)){
		tailValue = rb.ringbuf[rb.tail]; //save in a temporary int variable to return it later
		rb.tail = (rb.tail + 1) % RINGBUF_SIZE; //wrap around RINGBUF_SIZE (16)
		is_full = 0; //if something has been dequeued, it can't be full
		if(rb.head == rb.tail){
			is_empty = 1; //if head is the same as tail after popping something, it must be empty
			rb.head = 0;
			rb.tail = 0;
		}
		return tailValue;
	}
	return -1;
}
