#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "DriverInterface.h"
#include "MyClock.h"

#if defined(SUPPORT_PLATFORM_MTK)
#include "hal.h"
#elif defined(SUPPORT_PLATFORM_NORDIC)
extern int nrf_drv_clock_init(void);
#endif

#ifdef SUPPORT_MY_CLOCK
static structTimeStamp static_baseTimeStamp = 0;
static U32 static_baseClick = 0;
static U32 static_iSystemOnTime = 0;
#define RTC_BASE_YEAR 2020

U32 getMySystemTimeTick(void){
	return xTaskGetTickCount();
}
static void clock_refresh_base(U32 click, structTimeStamp stamp){
	static_iSystemOnTime += (click - static_baseClick) / 1000;
	static_baseClick = click;
	static_baseTimeStamp = stamp;
}
static void clock_read_rtc(void){
	structTimeStamp timeStamp;
	
#if defined(SUPPORT_PLATFORM_VS)
{
	time_t ltime;
	struct tm *tp = NULL;  


	ltime = time(NULL);
	tp = localtime(&ltime); 
	if(!tp)return;
	timeStamp = getTimeStampByPara(tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec, 0);
	timeStamp = getUTCTimeOfStamp(timeStamp);
	if(static_baseTimeStamp)timeStamp = static_baseTimeStamp;
}
#elif defined(SUPPORT_PLATFORM_MTK)
{
	hal_rtc_time_t rtc;

	hal_rtc_get_time(&rtc);
	timeStamp = getTimeStampByPara(rtc.rtc_year + RTC_BASE_YEAR, rtc.rtc_mon, rtc.rtc_day, rtc.rtc_hour, rtc.rtc_min, rtc.rtc_sec, 0);
}
#endif		
	clock_refresh_base(getMySystemTimeTick(), timeStamp);
}
static void clock_write_rtc(structTimeStamp stampUTC){
#if defined(SUPPORT_PLATFORM_MTK)
	hal_rtc_time_t rtc;
	int year,month,day;

	getDateOfTimeStamp(stampUTC, &year, &month, &day);

	hal_rtc_get_time(&rtc);
	rtc.rtc_year = year - RTC_BASE_YEAR;
	rtc.rtc_mon = month;
	rtc.rtc_day = day;
	rtc.rtc_hour = getHourOfTime(stampUTC);
	rtc.rtc_min = getMinuteOfTime(stampUTC);
	rtc.rtc_sec = getSecondOfTime(stampUTC);
	hal_rtc_set_time(&rtc);
#endif
}
void clock_init(void){
#if defined(SUPPORT_PLATFORM_NORDIC)
	nrf_drv_clock_init();
#endif	
	clock_read_rtc();
}

structTimeStamp getCurrentHardwareTime(void){
	return addTimeStamp(static_baseTimeStamp, getMySystemTimeTick() - static_baseClick);
}


void setCurrentHardwareTime(structTimeStamp stamp){
#if defined(SUPPORT_PLATFORM_MTK)
	clock_write_rtc(stamp);
	clock_read_rtc();
#elif defined(SUPPORT_PLATFORM_VS)
	clock_refresh_base(getMySystemTimeTick(), stamp);
#endif
}

U32 getTotalSystemOnTime(void){
	return static_iSystemOnTime + (getMySystemTimeTick() - static_baseClick) / 1000;
}
#if defined(SUPPORT_PLATFORM_VS)
void tickless_handler(U32 xExpectedIdleTime ){
}
void vPortSuppressTicksAndSleep(U32 xExpectedIdleTime){
}
#endif
#endif
