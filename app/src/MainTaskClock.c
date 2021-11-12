#include <stddef.h>
#include <string.h>

#include "DriverInterface.h"
#include "MyIO.h"
#include "MainTaskClock.h"


static structTimeStamp static_oldUTCTime;

static int static_timerSecond[TIMER_SECOND_MAX];

void trackimo_process_gsensor_status_on_second(void);

extern void battery_level_update(int battery_level);
extern void hr_value_update(int value);
extern void on_sensor_contact_detected(BOOL bDetected);
extern void my_ant_update(U8 hr, int iTotal, int time);

void main_task_set_second_timer(int id, int time){
	if((id < 0) || (id >= TIMER_SECOND_MAX))return;
	if(time == 0)static_timerSecond[id] = 0;
	else static_timerSecond[id] = time + getTotalSystemOnTime();
}
void main_task_clear_second_timer(int id){
	static_timerSecond[id] = 0;
}
void main_task_clear_all_second_timer(void){
	memset(static_timerSecond, 0, sizeof(static_timerSecond));
}
int main_task_get_second_timer(int id){
	if((id < 0) || (id >= TIMER_SECOND_MAX))return 0;
	return static_timerSecond[id];
}

static void generateMainTaskClockMessages(void){
	structTimeStamp oldUTC, newUTC, oldLocale, newLocale;
	int second, iSleepTime = 1;
	BOOL bSecondChange = FALSE;
	BOOL bMinuteChange = FALSE;
	BOOL bSleep = FALSE;
	
	oldUTC = static_oldUTCTime;
	newUTC = getCurrentTime();
	oldLocale = getLocalTimeOfUTC(oldUTC);
	newLocale = getLocalTimeOfUTC(newUTC);
	second = getSecondOfTime(newUTC);
	bSecondChange = (getSecondOfTime(oldUTC) != second);
	bMinuteChange = (getMinuteOfTime(oldUTC) != getMinuteOfTime(newUTC));
	
	if(bSecondChange || bMinuteChange){
		main_task_post_message(MAIN_MSG_CLOCK_EVENT, CLOCK_EVENT_SECOND_CHANGE, 0);
	}
	if(bMinuteChange){
		main_task_post_message(MAIN_MSG_CLOCK_EVENT, CLOCK_EVENT_MINUTE_CHANGE, 0);
	}
	if(getHourOfTime(oldUTC) != getHourOfTime(newUTC)){
		main_task_post_message(MAIN_MSG_CLOCK_EVENT, CLOCK_EVENT_UTC_HOUR_CHANGE, 0);
	}
	if(getHourOfTime(oldLocale) != getHourOfTime(newLocale)){
		main_task_post_message(MAIN_MSG_CLOCK_EVENT, CLOCK_EVENT_LOCAL_HOUR_CHANGE, 0);
	}	
	if(getDaysFromStart(oldUTC) != getDaysFromStart(newUTC)){
		main_task_post_message(MAIN_MSG_CLOCK_EVENT, CLOCK_EVENT_UTC_DAY_CHANGE, 0);
	}
	if(getDaysFromStart(oldLocale) != getDaysFromStart(newLocale)){
		main_task_post_message(MAIN_MSG_CLOCK_EVENT, CLOCK_EVENT_LOCAL_DAY_CHANGE, 0);
	}
	static_oldUTCTime = newUTC;
#ifdef SUPPORT_WATCH_DOG			
	system_feed_dog();
#endif			
	
	if(bSleep){
		iSleepTime = (60 - second);
#ifdef SUPPORT_WATCH_DOG			
		if(iSleepTime > 20)iSleepTime = 20;
#endif		
	}
	iSleepTime = 1;
	startMyTimer(TIMER_ID_CLOCK, iSleepTime * 1000, generateMainTaskClockMessages);
}

void restartClockMessage(void){
	generateMainTaskClockMessages();
}


void onMainTaskClockMessageInit(void){
	static_oldUTCTime = getCurrentTime();
	restartClockMessage();
}
void onMainTaskSecondChange(void){
	//trackimo_process_gsensor_status_on_second();
}
void onMainTaskMinuteChange(void){
}
void onMainTaskUTCHourChange(void){
}
void onMainTaskLocaleHourChange(void){
}
void onMainTaskUTCDayChange(void){
}
void onMainTaskLocaleDayChange(void){
}
void onMainTaskUTCMonthChange(void){
}
void onMainTaskLocaleMonthChange(void){
}
void onMainTaskUTCYearChange(void){
}
void onMainTaskLocaleYearChange(void){
}
void onMainTaskZoneChange(void){
}


