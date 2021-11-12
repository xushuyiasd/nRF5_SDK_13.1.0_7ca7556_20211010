#ifndef __MY_CIRCLE_QUEUE_FHQ_20201126_H__
#define __MY_CIRCLE_QUEUE_FHQ_20201126_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct{
	char * pCache;
	int iCellSize;
	int iMaxCells;
	int iWrite;
	int iRead;
} structMyCircleQueue;



void my_circle_queue_clear(structMyCircleQueue *pQueue);
void my_circle_queue_init(structMyCircleQueue *pQueue, char * pCache, int iCellSize, int iMaxCells);
int my_circle_queue_get_count(structMyCircleQueue *pQueue);
int my_circle_queue_get_free(structMyCircleQueue *pQueue);
BOOL my_circle_queue_is_full(structMyCircleQueue *pQueue);
BOOL my_circle_queue_is_empty(structMyCircleQueue *pQueue);
char * my_circle_queue_read(structMyCircleQueue *pQueue);
char * my_circle_queue_get_current(structMyCircleQueue *pQueue);
BOOL  my_circle_queue_write(structMyCircleQueue *pQueue, char * pData, int iDataLen);


#if defined(__cplusplus)
}
#endif 

#endif	//__MY_CIRCLE_QUEUE_FHQ_20201126_H__
