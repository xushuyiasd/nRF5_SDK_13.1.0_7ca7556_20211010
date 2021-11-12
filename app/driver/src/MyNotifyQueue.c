#include <stddef.h>
#include <string.h>
#include <stdio.h>

//#include "DriverInterface.h"
#include "MyCircleQueue.h"
#include "MyNotifyQueue.h"
static char static_array[MY_NOTIFY_QUEUE_MAX_CELL_COUNT][MY_NOTIFY_QUEUE_SIZE];
static structMyCircleQueue static_queue;

static structMyCircleQueue * my_notify_queue_get_ptr(void){
	return &static_queue;
}
void my_notify_queue_clear(void){
	my_circle_queue_init(my_notify_queue_get_ptr(), (char *)static_array, MY_NOTIFY_QUEUE_SIZE, countof(static_array));
}

int my_notify_queue_get_count(void){
	return my_circle_queue_get_count(my_notify_queue_get_ptr());
}
BOOL my_notify_queue_is_full(void){
	return my_circle_queue_is_full(my_notify_queue_get_ptr());
}
BOOL my_notify_queue_is_empty(void){
	return my_circle_queue_is_empty(my_notify_queue_get_ptr());
}

char * my_notify_queue_read(void){
	return my_circle_queue_read(my_notify_queue_get_ptr());
}
char * my_notify_queue_get_current(void){
	return my_circle_queue_get_current(my_notify_queue_get_ptr());
}

BOOL  my_notify_queue_write(char * pData){
	return my_circle_queue_write(my_notify_queue_get_ptr(), pData, MY_NOTIFY_QUEUE_SIZE);
}

