#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "DriverInterface.h"
#include "MyTimer.h"

#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
#include "MyMessageSystem.h"
#endif
#ifdef SUPPORT_MY_TIMER
typedef struct{
U32 tickOver;
pfDefaultFunction cb;
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
int task;
#endif
}structMyTimer;


static void onMyTimerCB(TimerHandle_t pxTimer);


static structMyTimer static_arrayTimer[TIMER_MAX];
static TimerHandle_t static_hTimer = NULL;

static structMyTimer * getMyTimerByID(int id){
	if((id < 0) || (id >= TIMER_MAX))return NULL;
	return static_arrayTimer + id;
}

int getWaitTimeForMyTimer(void){
	int i;
	U32 iMin = 0;
	structMyTimer * p = static_arrayTimer;
	U32 now;

	for(i = 0; i < TIMER_MAX; i++, p++){
		if(p->tickOver == 0)continue;
		if(iMin == 0)iMin = p->tickOver;
		else if(iMin > p->tickOver)iMin = p->tickOver;
	}
	if(iMin == 0)return -1;	// no  timer
	now = getMySystemTimeTick();
	if(iMin <= now)return 0;	 // timeout
	return iMin - now;	//wait time
}

static void restartMyTimer(BOOL bFromISR){
	int time = getWaitTimeForMyTimer();
	TimerHandle_t handle = static_hTimer;
	if(time < 0){
		if(bFromISR)xTimerStopFromISR(handle, 0);
		else xTimerStop(handle, 0);
		return;
	}
	else if(time < 5)time = 5;
	if(bFromISR){
		xTimerStopFromISR(handle, 0);
		xTimerChangePeriodFromISR(handle, time, 0);
		xTimerStartFromISR(handle, 0);
	}else{
		xTimerStop(handle, 0);
		xTimerChangePeriod(handle, time, 0);
		xTimerStart(handle, 0);
	}
}
static void startMyTimerByMode(int id, int time, pfDefaultFunction cb, BOOL bFromISR){
	structMyTimer * p;

	p = getMyTimerByID(id);
	if(!p)return;
	p->tickOver = time + getMySystemTimeTick();
	p->cb = cb;
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
	p->task = -1;
#endif
	restartMyTimer(bFromISR);
}

void startMyTimer(int id, int time, pfDefaultFunction cb){
	startMyTimerByMode(id, time, cb, FALSE);
}
void startMyTimerFromISR(int id, int time, pfDefaultFunction cb){
	startMyTimerByMode(id, time, cb, TRUE);
}
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
void startMyTimerInTask(int id, int time, pfDefaultFunction cb, int task){
	structMyTimer * p;

	p = getMyTimerByID(id);
	if(!p)return;
	p->tickOver = time + getMySystemTimeTick();
	p->cb = cb;
	p->task = task;
	restartMyTimer(FALSE);
}
#endif
static void stopMyTimerByMode(int id, BOOL bFromISR){
	structMyTimer * p;

	p = getMyTimerByID(id);
	if(!p)return;
	memset(p, 0, sizeof(structMyTimer));
	restartMyTimer(bFromISR);
}
void stopMyTimer(int id){
	stopMyTimerByMode(id, FALSE);
}
void stopMyTimerFromISR(int id){
	stopMyTimerByMode(id, TRUE);
}
static void executeMyTimerOver(void){
	int i;
	structMyTimer * p;
	U32 now = getMySystemTimeTick();
	pfDefaultFunction cb;
	int task = -1;

	p = static_arrayTimer;
	for(i = 0; i < TIMER_MAX; i++, p++){
		if(p->tickOver == 0)continue;
		if(p->tickOver <= now){
			cb = p->cb;
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
			task = p->task;
#endif
			memset(p, 0, sizeof(structMyTimer));

			if(!cb)continue;
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
			if(task >= 0)MyMessageSystemPostMessage(-1, task, 0, 0, (int)cb);
			else
#endif
			cb();
		}
	}
}
static void onMyTimerCB(TimerHandle_t pxTimer){
	executeMyTimerOver();
	restartMyTimer(FALSE);
}
void initMyTimer(void){
	memset(static_arrayTimer, 0, sizeof(static_arrayTimer));
	static_hTimer  = xTimerCreate("myTimer", 1000, FALSE, NULL, onMyTimerCB);
}
void deinitMyTimer(void){
	if(static_hTimer){
		xTimerStop(static_hTimer, 0);
		xTimerDelete(static_hTimer, 0);
		static_hTimer = 0;
	}
	memset(static_arrayTimer, 0, sizeof(static_arrayTimer));
	
}
#endif
