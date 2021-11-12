#ifndef __MY_MEMORY_FHQ_20200308_H__
#define __MY_MEMORY_FHQ_20200308_H__

#include "CommonInterface.h"
#if defined(__cplusplus)
extern "C" {     
#endif

#ifdef SUPPORT_MY_MEMORY

#if defined(SUPPORT_PLATFORM_VS)
#define ATTRIBUTE_ALIGN
#define ATTRIBUTE_NOCACHE
#elif defined(SUPPORT_PLATFORM_MTK)
#define ATTRIBUTE_ALIGN __attribute__((aligned(4)))
#define ATTRIBUTE_NOCACHE __attribute__((__section__(".noncached_zidata")))
#elif defined(SUPPORT_PLATFORM_NORDIC)
#define ATTRIBUTE_ALIGN
#define ATTRIBUTE_NOCACHE
#endif

#ifdef SUPPORT_PLATFORM_VS
void *pvPortMalloc(unsigned int num_bytes);
void *pvPortRealloc(void *mem_address, unsigned int newsize);
void *pvPortCalloc(unsigned int num, unsigned int size);
void vPortFree(void *ptr);
#endif

/*
	��������:�Զ����ͷ��ڴ溯��
	����˵��:ptr	��Ҫ�ͷŵ��ڴ��ָ��
	�� �� ֵ:��
	������Ա:����ǿ
*/

void myFree(void *ptr);
/*
	��������:�Զ�������ڴ溯��
	����˵��:num_bytes	�����ֽ���
	�� �� ֵ:����
	������Ա:����ǿ
*/

void *myMalloc(unsigned int num_bytes);

void * myRealloc(void *mem_address, unsigned int newsize);
void * myCalloc(unsigned int num, unsigned int size);

#endif
#if defined(__cplusplus)
}
#endif 

#endif //__MY_MEMORY_FHQ_20200308_H__



