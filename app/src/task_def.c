#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "FreeRTOS.h"
#include "task.h"
#include "task_def.h"
#include "DriverInterface.h"
#include "MainTask.h"

extern void log_task(void * arg);
extern size_t xPortGetFreeHeapSize( void );

typedef struct tasks_list_ {
    TaskFunction_t      pvTaskCode;
    char                *pcName;
    U16            usStackDepth;
    void                *pvParameters;
    UBaseType_t         uxPriority;
} tasks_list_t;


static const tasks_list_t tasks_list[] = {
	{ main_task,         "main",      1024,     NULL,   2 },
};

void task_def_init(void){
	driver_init();
}

void task_def_create(void)
{
	int           i;
	BaseType_t          x;
	const tasks_list_t  *t;
	int iCount = countof(tasks_list);

	for (i = 0; i < iCount; i++) {
		t = &tasks_list[i];


		x = xTaskCreate(t->pvTaskCode,
		t->pcName,
		t->usStackDepth,
		t->pvParameters,
		t->uxPriority,
		NULL);

		myprintf("xCreate task %d, pri %d : %d\r\n", i, (int)t->uxPriority, x);
	}
	myprintf("Free Heap size:%d bytes\r\n", xPortGetFreeHeapSize());
}

