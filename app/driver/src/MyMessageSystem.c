#include <stddef.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "MyMessageSystem.h"

#ifdef SUPPORT_MY_MESSAGE
typedef struct{
void * handleMsgPool;
pfProcessMsg process;
}structMsgPara;

static structMsgPara static_msgPool[MYTASK_MAX]={{0}};


static BOOL MyMessageSystemInit(int task){
	if((task < 0) || (task >= MYTASK_MAX))return FALSE;
	memset(&static_msgPool[task], 0, sizeof(structMsgPara));
	return TRUE;
}


static structMsgPara * MyMessageSystemGetPara(int task){
	if((task < 0) || (task >= MYTASK_MAX))return NULL;
	return static_msgPool + task;
}



static pfProcessMsg MyMessageSystemGetProcess(int task){
	structMsgPara * pPara = MyMessageSystemGetPara(task);

	if(!pPara)return NULL;
	return pPara->process;
}
void MyMessageSystemSetProcess(int task, pfProcessMsg process){
	structMsgPara * pPara = MyMessageSystemGetPara(task);

	if(!pPara)return;
	pPara->process = process;
}


static void * MyMessageSystemGetPoolHandle(int task){
	structMsgPara * pPara = MyMessageSystemGetPara(task);

	if(!pPara)return NULL;
	return pPara->handleMsgPool;
}
static void MyMessageSystemSetPoolHandle(int task, void *handle){
	structMsgPara * pPara = MyMessageSystemGetPara(task);

	if(!pPara)return;
	pPara->handleMsgPool = handle;
}


static BOOL MyMessageSystemProcess(int task, MYMSG * pMsg){
	pfProcessMsg fun;

	if(!pMsg)return FALSE;
	if((task < 0) || (task >= MYTASK_MAX))return FALSE;
	if(task != pMsg->to)return FALSE;
	fun = MyMessageSystemGetProcess(task);
	if(!fun)return FALSE;
	fun(pMsg);
	return TRUE;
}
int MyMessageSystemGetWaiting(int task){
	return uxQueueMessagesWaiting(MyMessageSystemGetPoolHandle(task));
}
BOOL MyMessageSystemIsQueueFull(int task){
	return uxQueueSpacesAvailable(MyMessageSystemGetPoolHandle(task)) == 0;
}
BOOL MyMessageSystemIsQueueEmpty(int task){
	return uxQueueMessagesWaiting(MyMessageSystemGetPoolHandle(task)) == 0;
}

BOOL MyMessageSystemPostMessage(int from, int to, U32 id, U32 wParam, int lParam){
	MYMSG msg;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	structMsgPara * pPara = MyMessageSystemGetPara(to);

	memset(&msg, 0, sizeof(msg));
	msg.from = from;
	msg.to = to;
	msg.id = id;
	msg.wParam = wParam;
	msg.lParam = lParam;
	if(!pPara)return FALSE;	
	if(!pPara->handleMsgPool)return FALSE;

	if(xQueueSendFromISR(pPara->handleMsgPool, (void*)&msg, &xHigherPriorityTaskWoken) != pdTRUE){
		return FALSE;
	}

	if (xHigherPriorityTaskWoken) portYIELD_FROM_ISR(pdTRUE);
	return TRUE;
	
}
BOOL MyMessageSystemSendMessage(int from, int to, U32 id, U32 wParam, int lParam){
	MYMSG msg;

	memset(&msg, 0, sizeof(msg));
	msg.from = from;
	msg.to = to;
	msg.id = id;
	msg.wParam = wParam;
	msg.lParam = lParam;

	{
		return MyMessageSystemProcess(to, &msg);
	}
	
}


static void MyMessageSystemLoop(int task){
	MYMSG msg;
	
	structMsgPara * pPara = MyMessageSystemGetPara(task);

	if(!pPara)return;	
	while(1){
		if (xQueueReceive(pPara->handleMsgPool, &msg, portMAX_DELAY)){
			MyMessageSystemProcess(task, &msg);
		}
	}
}
void MyMessageSystemInitTask(int iTask, int iMsgs, pfProcessMsg process)
{
	QueueHandle_t handle;

	handle = xQueueCreate(iMsgs, sizeof(MYMSG));	   
	MyMessageSystemInit(iTask);
	MyMessageSystemSetProcess(iTask, process);
	MyMessageSystemSetPoolHandle(iTask, handle);
	MyMessageSystemLoop(iTask);
}
void MyMessageSystemInitTaskWithStartCB(int iTask, int iMsgs, pfProcessMsg process, pfDefaultFunction fun)
{
	QueueHandle_t handle;

	handle = xQueueCreate(iMsgs, sizeof(MYMSG));	   
	MyMessageSystemInit(iTask);
	MyMessageSystemSetProcess(iTask, process);
	MyMessageSystemSetPoolHandle(iTask, handle);
	if(fun)fun();
	MyMessageSystemLoop(iTask);
}
#endif


