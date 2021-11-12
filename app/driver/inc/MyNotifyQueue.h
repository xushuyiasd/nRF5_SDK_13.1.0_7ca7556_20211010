#ifndef __MY_NOTIFY_QUEUE_FHQ_20210531_H__
#define __MY_NOTIFY_QUEUE_FHQ_20210531_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define MY_NOTIFY_QUEUE_SIZE 64
#define MY_NOTIFY_QUEUE_MAX_CELL_COUNT 20

void my_notify_queue_clear(void);
int my_notify_queue_get_count(void);
BOOL my_notify_queue_is_full(void);
BOOL my_notify_queue_is_empty(void);
char * my_notify_queue_read(void);
char * my_notify_queue_get_current(void);
BOOL  my_notify_queue_write(char * pData);


#if defined(__cplusplus)
}
#endif 

#endif	//__MY_NOTIFY_QUEUE_FHQ_20210531_H__
