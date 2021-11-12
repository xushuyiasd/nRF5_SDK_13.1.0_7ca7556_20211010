#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "nrf_drv_spis.h"

#include "FreeRTOS.h"
#include "DriverInterface.h"
#include "MyGenerateInfo.h"
#include "MyAT.h"
#include "MainTaskClock.h"
#include "MyNotifyQueue.h"
#include "MainTask.h"

#define MAIN_TASK_EVENT_QUEUE_LENGTH 10
static const int task = MYTASK_MAIN;
#define MY_FIREWARE_VERSION "TRACKIMO_V01"

#if defined(SUPPORT_PLATFORM_NORDIC)
extern void intern_softdevice_events_execute(void);
extern void my_ble_init(void);
extern void my_ant_init(void);

#endif

BOOL main_task_post_message(int msg, int wPara, int lpPara){
	return MyMessageSystemPostMessage(task, task, msg, wPara, lpPara);
}

char * getTrackimoVersion(void){
	return MY_FIREWARE_VERSION;
}

#ifdef SUPPORT_MY_GSENSOR
static U32 static_arrayGSensorMove[TRACKIMO_GSENSOR_SILENT_TIME];
static int static_iGSensorWriteIndex = 0;

static void trackimo_reset_gsensor_status(void){
	memset(static_arrayGSensorMove, 0, sizeof(static_arrayGSensorMove));
	static_iGSensorWriteIndex = 0;
}
static int trackimo_add_gsensor_move_count(int time){
	int time2 = time -getGSensorTime() * 1000;
	int i;
	int iIndex;
	int timeCache;
	if(time2 < 0)time2 = 0;
	
	static_arrayGSensorMove[static_iGSensorWriteIndex] = time;
	static_iGSensorWriteIndex = (static_iGSensorWriteIndex + 1) % TRACKIMO_GSENSOR_SILENT_TIME;
	for(i = 0; i < TRACKIMO_GSENSOR_SILENT_TIME; i++){
		iIndex = (static_iGSensorWriteIndex + i) % TRACKIMO_GSENSOR_SILENT_TIME;
		timeCache = static_arrayGSensorMove[iIndex];
		if(!timeCache)continue;
		if(timeCache >= time2){
			return TRACKIMO_GSENSOR_SILENT_TIME - i;
		}
	}
	return 0;
}

static void ongSensorEventMove(void){
	int iCount  = 0;
	int iLimit = getGSensorCount();
	
	iCount = trackimo_add_gsensor_move_count(getMySystemTimeTick());
//	iCount = trackimo_get_gsensor_move_count_in_period();
	maintask_printf("trackimo_is_gsensor_moving:%d,%d\r\n", iCount, iLimit);
	if(iCount < iLimit)return;

	maintask_printf("GSensor Start Move\r\n");
	main_task_post_message(MAIN_MSG_SYSTEM_EVENT, SYSTEM_EVENT_START_MOVING, 0);
}
static void ongSensorEvent(int event, int para){
	switch(event){
		case GSENSOR_EVENT_MOVE:
			ongSensorEventMove();
			break;
		case GSENSOR_EVENT_SILENT:
			break;
	}
}
#endif
#ifdef SUPPORT_MY_BLE

static void onBLEEvent(int event, int para){
	switch(event){
		case BLE_EVENT_INTERNAL:
			intern_softdevice_events_execute();
			break;
		case BLE_EVENT_CONNECTED:
			break;
		case BLE_EVENT_DISCONNECTED:
			break;
	}	
}
#endif
static void onSystemEvent(int event, int para){
	switch(event){
		case SYSTEM_EVENT_LOW_POWER:
			break;
		case SYSTEM_EVENT_SHUTDOWN:
			break;
		case SYSTEM_EVENT_REBOOT:
			break;
		case SYSTEM_EVENT_START_MOVING:
			system_poweron_mcu();
			break;
	}
}
static void onMainTaskClockEvent(int wPara, int lpPara){
	switch(wPara){
		case CLOCK_EVENT_SECOND_CHANGE:
			onMainTaskSecondChange();
			break;
		case CLOCK_EVENT_MINUTE_CHANGE:
			onMainTaskMinuteChange();
			break;
		case CLOCK_EVENT_UTC_HOUR_CHANGE:
			onMainTaskUTCHourChange();
			break;
		case CLOCK_EVENT_UTC_DAY_CHANGE:
			onMainTaskUTCDayChange();
			break;
		case CLOCK_EVENT_UTC_MONTH_CHANGE:
			onMainTaskUTCMonthChange();
			break;
		case CLOCK_EVENT_UTC_YEAR_CHANGE:
			onMainTaskUTCYearChange();
			break;
		case CLOCK_EVENT_LOCAL_HOUR_CHANGE:
			onMainTaskLocaleHourChange();
			break;
		case CLOCK_EVENT_LOCAL_DAY_CHANGE:
			onMainTaskLocaleDayChange();
			break;
		case CLOCK_EVENT_LOCAL_MONTH_CHANGE:
			onMainTaskLocaleMonthChange();
			break;
		case CLOCK_EVENT_LOCAL_YEAR_CHANGE:
			onMainTaskLocaleYearChange();
			break;
		case CLOCK_EVENT_TIMEZONE_CHANGE:
			onMainTaskZoneChange();
			break;
	}	
}

static void onPowerStepPoweron(void){
	maintask_printf("onPowerStepPoweron\r\n");
	setSystemPowerStep(POWERSTEP_POWERON);
}
static void onPowerStepWelcom(void){
//	maintask_printf("onPowerStepWelcom:%s_%s\r\n", MY_FIREWARE_VERSION, getVersionTime());
	maintask_printf("onPowerStepWelcom\r\n");
	setSystemPowerStep(POWERSTEP_WELCOM);
	main_task_clear_all_second_timer();
	//onMainTaskClockMessageInit();
	main_task_post_message(MAIN_MSG_POWERSTEP_EVENT, POWERSTEP_POWERON, 0);
}

static void onPowerStep(int iStep, int iPara){
	switch(iStep){
		case POWERSTEP_WELCOM:
			onPowerStepWelcom();
			break;
		case POWERSTEP_POWERON:
			onPowerStepPoweron();
			break;	
	}
}
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
static void onMainTaskTimerEvent(int wPara, int lpPara){
	pfDefaultFunction cb = (pfDefaultFunction)lpPara;
	if(cb)cb();
}
#endif

static void main_task_process(MYMSG * pMsg){
	if(!pMsg)return;
	main_printf("main_task_process:%d,%d,%d\r\n", (int)pMsg->id, (int)pMsg->wParam, pMsg->lParam);
	switch(pMsg->id){
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
		case MAIN_MSG_INTASK_TIMER_CB:
			onMainTaskTimerEvent(pMsg->wParam, pMsg->lParam);
			break;
#endif
		case MAIN_MSG_SYSTEM_EVENT:
			onSystemEvent(pMsg->wParam, pMsg->lParam);
			break;
		case MAIN_MSG_POWERSTEP_EVENT:
			onPowerStep(pMsg->wParam, pMsg->lParam);
			break;			

		case MAIN_MSG_BLE_EVENT:
#ifdef SUPPORT_MY_BLE
			onBLEEvent(pMsg->wParam, pMsg->lParam);
#endif
			break;
		case MAIN_MSG_GSENSOR_EVENT:
#ifdef SUPPORT_MY_GSENSOR
			ongSensorEvent(pMsg->wParam, pMsg->lParam);
#endif
			break;
		case MAIN_MSG_CLOCK_EVENT:
			onMainTaskClockEvent(pMsg->wParam, pMsg->lParam);
			break;
		case MAIN_MSG_AT_EVENT:
			onProcessATCmdLine(my_notify_queue_read());
			break;
	}
}


static void onMyTimerSystemInit(void){
	main_printf("onMyTimerSystemInit\r\n");
	trackimo_reset_gsensor_status();
	main_task_post_message(MAIN_MSG_POWERSTEP_EVENT, POWERSTEP_WELCOM, 0);
}
void main_task( void *pvParameters ){
	MyMessageSystemInitTaskWithStartCB(task, MAIN_TASK_EVENT_QUEUE_LENGTH, main_task_process, onMyTimerSystemInit);
}
	
