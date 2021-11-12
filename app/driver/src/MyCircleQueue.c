#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "MyCircleQueue.h"

void my_circle_queue_clear(structMyCircleQueue *pQueue){
	if(!pQueue)return;
	pQueue->iRead = 0;
	pQueue->iWrite = 0;
}
void my_circle_queue_init(structMyCircleQueue *pQueue, char * pCache, int iCellSize, int iMaxCells){
	if(!pQueue)return;
	memset(pQueue, 0, sizeof(structMyCircleQueue));
	pQueue->pCache = pCache;
	pQueue->iCellSize = iCellSize;
	pQueue->iMaxCells = iMaxCells;
}
int my_circle_queue_get_count(structMyCircleQueue *pQueue){
	if(!pQueue)return 0;
	if(pQueue->iMaxCells <= 0)return 0;
	return (pQueue->iWrite - pQueue->iRead + pQueue->iMaxCells) % pQueue->iMaxCells;
}
int my_circle_queue_get_free(structMyCircleQueue *pQueue){
	if(!pQueue)return 0;
	if(pQueue->iMaxCells <= 0)return 0;
	return pQueue->iMaxCells - 1 - my_circle_queue_get_count(pQueue);
}
BOOL my_circle_queue_is_full(structMyCircleQueue *pQueue){
	if(!pQueue)return FALSE;
	return my_circle_queue_get_count(pQueue) == pQueue->iMaxCells - 1;
}
BOOL my_circle_queue_is_empty(structMyCircleQueue *pQueue){
	return my_circle_queue_get_count(pQueue) == 0;
}
char * my_circle_queue_read(structMyCircleQueue *pQueue){
	char * p;
	
	if(!pQueue)return NULL;
	if(my_circle_queue_is_empty(pQueue))return NULL;
	if(pQueue->iMaxCells <= 0)return NULL;
	p = pQueue->pCache + pQueue->iRead * pQueue->iCellSize;
	pQueue->iRead = (pQueue->iRead + 1) % pQueue->iMaxCells;
	return p;
}
char * my_circle_queue_get_current(structMyCircleQueue *pQueue){
	char * p;
	
	if(!pQueue)return NULL;
	if(my_circle_queue_is_empty(pQueue))return NULL;
	if(pQueue->iMaxCells <= 0)return NULL;
	p = pQueue->pCache + pQueue->iRead * pQueue->iCellSize;
	return p;	
}
BOOL  my_circle_queue_write(structMyCircleQueue *pQueue, char * pData, int iDataLen){
	if(!pQueue || !pData)return FALSE;
	if(pQueue->iCellSize < iDataLen)return FALSE;
	if(pQueue->iMaxCells <= 0)return FALSE;
	if(my_circle_queue_is_full(pQueue))return FALSE;
	memcpy(pQueue->pCache + pQueue->iWrite * pQueue->iCellSize, pData, iDataLen);
	pQueue->iWrite = (pQueue->iWrite + 1) % pQueue->iMaxCells;
	return TRUE;
}

