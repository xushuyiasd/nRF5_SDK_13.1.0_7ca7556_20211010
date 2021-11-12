#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DriverInterface.h"

#ifdef SUPPORT_MY_MEMORY

#if defined(SUPPORT_PLATFORM_MTK) || defined(SUPPORT_PLATFORM_NORDIC)
#include "FreeRTOS.h"
#endif

#ifdef SUPPORT_PLATFORM_VS
void *pvPortMalloc(unsigned int num_bytes){
	return malloc(num_bytes);
}
void *pvPortRealloc(void *mem_address, unsigned int newsize){
	return realloc(mem_address, newsize);
}
void *pvPortCalloc(unsigned int num, unsigned int size){
	return calloc(num, size);
}
void vPortFree(void *ptr){
	free(ptr);
}
int xPortGetFreeHeapSize(void){
	return 0;
}
#endif

void *myMalloc(unsigned int num_bytes){
	return pvPortMalloc(num_bytes);
}
void myFree(void *ptr){
	vPortFree(ptr);
}
void * myRealloc(void *mem_address, unsigned int newsize){
#ifndef SUPPORT_PLATFORM_NORDIC
	return pvPortRealloc(mem_address, newsize);
#else
	return NULL;
#endif
}
void * myCalloc(unsigned int num, unsigned int size){
#ifndef SUPPORT_PLATFORM_NORDIC
	return pvPortCalloc(num, size);
#else
	return NULL;
#endif
}
#endif

