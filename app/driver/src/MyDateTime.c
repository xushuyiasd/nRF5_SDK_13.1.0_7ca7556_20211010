#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DriverInterface.h"
#include "MyDateTime.h"


#ifdef SUPPORT_MY_DATETIME

#define BASE_YEAR 0
#define BASE_DAY_WEEK 6

static const int MillSecondsOneDay = 1000 * 3600 * 24;

static const U8 days_of_month[]={
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

int getTimeDiffByZone(int zone){
	int hour = zone / 100;
	int minute = zone % 100;

	return (hour * 3600   + minute * 60) * 1000;
}
int getTimeDiffByTimeZone(void){
//	return getTimeDiffByZone(getTimeZone());
	return 0;
}
structTimeStamp getUTCTimeOfStamp(structTimeStamp stamp){
	return addTimeStamp(stamp, -getTimeDiffByTimeZone());
}
structTimeStamp getLocalTimeOfUTC(structTimeStamp stamp){
	return addTimeStamp(stamp, getTimeDiffByTimeZone());
}
structTimeStamp getCurrentTime(void){
	return getCurrentHardwareTime();
}
structTimeStamp computerTime2MyTime(structTimeStamp time){
	structTimeStamp time2 = getTimeStampByPara(1970, 1, 1, 0, 0, 0, 0);
	return time2 + time;
}
structTimeStamp getCurrentLocalTime(void){
	return getLocalTimeOfUTC(getCurrentTime());
}
static int getValueOfPartString(const char * pBuf, int iStart, int iLen){
	char temp[20];

	if(!pBuf)return 0;
	memset(temp, 0, sizeof(temp));
	strncpy(temp, pBuf + iStart, iLen);
	return atoi(temp);
}
static int getYearOfDateString(const char * pBuf){
	return getValueOfPartString(pBuf, 0, 4);
}
static int getMonthOfDateString(const char * pBuf){
	return getValueOfPartString(pBuf, 4, 2);
}
static int getDayOfDateString(const char * pBuf){
	return getValueOfPartString(pBuf, 6, 2);
}
static int getHourOfTimeString(const char * pBuf){
	return getValueOfPartString(pBuf, 0, 2);
}
static int getMinuteOfTimeString(const char * pBuf){
	return getValueOfPartString(pBuf, 2, 2);
}
static int getSecondOfTimeString(const char * pBuf){
	return getValueOfPartString(pBuf, 4, 2);
}
static int getYearByDays(int days){
	int iYear = 0;
	int iDays = abs(days);
	
	iYear += iDays / 146097 * 400;
	iDays %= 146097;
	iYear += iDays / 36524 * 100;
	iDays %= 36524;
	iYear += iDays / 1461 * 4;
	iDays %= 1461;
	if(days > 0){
		if(iDays)iDays--;
	}else{
//		if(!iDays)iDays--;
	}
	iYear += iDays / 365;
	if(days < 0){
		iYear = -iYear;
		if(iDays % 365)iYear--;
	}
	return iYear;
}
int getDaysOfMonth(int month, BOOL bLeap){
	int iDays;

	if((month <= 0) || (month > 12))return 0;
	iDays = days_of_month[month - 1];
	if(month == 2)iDays += bLeap ? 1 : 0;
	
	return iDays;
}

BOOL isTimeStampEmpty(structTimeStamp stamp){
	return stamp == 0;
}
BOOL isTimeStampDiffEmpty(structTimeStampDiff diff){
	return diff == 0;
}
void resetTimeStamp(structTimeStamp * pStamp){
	if(!pStamp)return;
	memset(pStamp, 0, sizeof(structTimeStamp));
}
BOOL isLeapYear(int year){
	return (((year % 4) == 0) && ((year % 100) != 0)) || (year % 400 == 0);
}

int getDaysFromStartForYear(int year){
	int days = 0;
	int iYear = abs(year);
	

	days = iYear * 365;
	days += iYear / 4;
	days -= iYear / 100;
	days += iYear / 400;
	if(year > 0){
		if(iYear % 4)days++;
	}else{
//		if(iYear % 4 == 0)days++;
	}
	if(year < 0)days =- days;
	return days;
}
int getDaysFormThisYearForMonth(int year, int month){
	BOOL leap = isLeapYear(year);
	int days = 0;
	int i;

	for(i = 1; i < month; i++){
		days += getDaysOfMonth(i, leap);
	}
	return days;
}
structTimeStamp setTimeStampDays(structTimeStamp time, int days){
	time %= MillSecondsOneDay;
	time += (I64)MillSecondsOneDay * days;
	return time;
}
structTimeStamp setTimeStampMillSeconds(structTimeStamp time, int millSeconds){
	time = time / MillSecondsOneDay * MillSecondsOneDay +  millSeconds;
	return time;
}

int getDaysFromThisYearForDay(int year, int month, int day){
	return getDaysFormThisYearForMonth(year, month) + (day - 1);
}

int getDaysFromStart(structTimeStamp time){
	int days = (int)(time / MillSecondsOneDay);
	
	if((time < 0) && (time % MillSecondsOneDay) )days--;
	return days;
}
int getMillSeconsThatDay(structTimeStamp time){
	return (int)(time % MillSecondsOneDay);
}

BOOL isZeroClockOfThatDay(structTimeStamp time){
	return getMillSeconsThatDay(time) == 0;
}
BOOL isInSameWeek(structTimeStamp time1, structTimeStamp time2){
	int days1 = getDaysFromStart(time1);
	int days2 = getDaysFromStart(time2);
	return  ((BASE_DAY_WEEK + days1) / 7) == ((BASE_DAY_WEEK + days2) / 7);
}

BOOL isInSameMonth(structTimeStamp time1, structTimeStamp time2){
	int iYear1,iMonth1,iDay1;
	int iYear2,iMonth2,iDay2;

	if(!getDateOfTimeStamp(time1, &iYear1, &iMonth1, &iDay1))return FALSE;
	if(!getDateOfTimeStamp(time2, &iYear2, &iMonth2, &iDay2))return FALSE;

	return (iYear1 == iYear2) && (iMonth1 == iMonth2);
}
BOOL isTheSameDay(structTimeStamp time1, structTimeStamp time2){
	return getDaysFromStart(time1) == getDaysFromStart(time2);
}
BOOL isUTCTimeToday(structTimeStamp time){
	return isTheSameDay(time, getCurrentTime());
}
BOOL isUTCTimeThisMonth(structTimeStamp time){
	return isInSameMonth(time, getCurrentTime());
}
BOOL isUTCTimeThisWeek(structTimeStamp time){
	return isInSameWeek(time, getCurrentTime());
}

BOOL getDateOfTimeStamp(structTimeStamp time, int *pYear, int *pMonth, int *pDay){
	int iYear = 0;
	int iMonth = 1;
	int days = getDaysFromStart(time);
	BOOL bLeap;
	int iTempDays = days;

	if((!pYear) || (!pMonth) || (!pDay))return FALSE;
	iYear = getYearByDays(days);
	bLeap = isLeapYear(iYear);
	days -= getDaysFromStartForYear(iYear);
	 iTempDays = days;
	while(1){
		iTempDays -= getDaysOfMonth(iMonth, bLeap);
		if(iTempDays < 0)break;
		days = iTempDays;
		iMonth++;
	}

	*pYear = iYear;
	*pMonth = iMonth;
	*pDay = days + 1;
	return TRUE;

}
int getYearOfTime(structTimeStamp time){
	int iYear, iMonth, iDay;

	if(!getDateOfTimeStamp(time, &iYear, &iMonth, &iDay))return 0;
	return iYear;
}
int getMonthOfTime(structTimeStamp time){
	int iYear, iMonth, iDay;

	if(!getDateOfTimeStamp(time, &iYear, &iMonth, &iDay))return 0;
	return iMonth;
}

int getDayOfTime(structTimeStamp time){
	int iYear, iMonth, iDay;

	if(!getDateOfTimeStamp(time, &iYear, &iMonth, &iDay))return 0;
	return iDay;
}

int getWeekOfTime(structTimeStamp time){
	int days = getDaysFromStart(time);
	
	return  (BASE_DAY_WEEK + (days % 7)) % 7;
}
BOOL getClockOfTimeStamp(structTimeStamp time, int *pHour, int *pMinute, int *pSecond){
	int iSecond = getMillSeconsThatDay(time) / 1000;
	
	if(!pHour || !pMinute || !pSecond)return FALSE;
	*pHour = iSecond / 3600;
	*pMinute = (iSecond % 3600) / 60;
	*pSecond = iSecond % 60;
	return TRUE;
}
int getHourOfTime(structTimeStamp time){
	return (getMillSeconsThatDay(time) / 1000 / 3600);
}

int getMinuteOfTime(structTimeStamp time){
	return ((getMillSeconsThatDay(time) / 1000) % 3600) / 60;
}

int getSecondOfTime(structTimeStamp time){
	return (getMillSeconsThatDay(time) / 1000) % 60;
}

int getMillSecondOfTime(structTimeStamp time){
	return (getMillSeconsThatDay(time) % 1000);
}

structTimeStamp getTimeStamp(int days, int millSecons){
	return (I64)days * MillSecondsOneDay + millSecons;
}

int getDaysForTimeStamp(int year, int month, int day){
	return getDaysFromStartForYear(year) + getDaysFromThisYearForDay(year, month, day);
}
int getMillSecondsForTimeStamp(int hour, int minute, int second, int millSecons){
	return ((hour * 60 + minute) * 60 + second) * 1000 + millSecons;
}
structTimeStamp getTimeStampByPara(int year, int month, int day, int hour, int minute, int second, int millSecons){
	int days = getDaysForTimeStamp(year, month, day);
	int millSeconds = getMillSecondsForTimeStamp(hour, minute, second, millSecons);

	return getTimeStamp(days, millSeconds);
}

int getDaysForTimeStampByString(char * pString){
	int year, month, day;

	if(!pString)return 0;
	if(strlen(pString) < 8)return 0;
	year = getYearOfDateString(pString);
	month = getMonthOfDateString(pString);
	day = getDayOfDateString(pString);
	return getDaysForTimeStamp(year, month, day);
}

int getMillSecondsForTimeStampByString(char * pString){
	int hour, minute, second;

	if(!pString)return 0;
	if(strlen(pString) < 6)return 0;
	hour = getHourOfTimeString(pString);
	minute = getMinuteOfTimeString(pString);
	second = getSecondOfTimeString(pString);
	return getMillSecondsForTimeStamp(hour, minute, second, 0);
}
structTimeStamp getTimeStampByString(char * pString){
	char temp[10];
	int days,millSeconds;

	if(!pString)return 0;
	memset(temp, 0, sizeof(temp));
	strncpy(temp, pString, 8);
	days = getDaysForTimeStampByString(temp);

	memset(temp, 0, sizeof(temp));
	strncpy(temp, pString + 8, 6);
	millSeconds = getMillSecondsForTimeStampByString(temp);
	return getTimeStamp(days, millSeconds);

}
BOOL getDateStringForTimeStamp(structTimeStamp stamp, char * pBuffer, int iBufLen){
	int year, month, day;

	if(!pBuffer)return FALSE;
	if(iBufLen <= 8)return FALSE;
	if(!getDateOfTimeStamp(stamp, &year, &month, &day))return FALSE;

	sprintf(pBuffer, "%04d%02d%02d", year, month, day);
	pBuffer[8] = 0;
	return TRUE;
}
BOOL getTimeStringForTimeStamp(structTimeStamp stamp, char * pBuffer, int iBufLen){
	int hour, minute, second;

	if(!pBuffer)return FALSE;
	if(iBufLen <= 6)return FALSE;
	if(!getClockOfTimeStamp(stamp, &hour, &minute, &second))return FALSE;
	sprintf(pBuffer, "%02d%02d%02d", hour, minute, second);
	pBuffer[6] = 0;
	return TRUE;
}
BOOL getDateTimeStringForTimeStamp(structTimeStamp stamp, char * pBuffer, int iBufLen){
	int year, month, day;
	int hour, minute, second;

	if(!pBuffer)return FALSE;
	if(iBufLen <= 14)return FALSE;
	if(!getDateOfTimeStamp(stamp, &year, &month, &day))return FALSE;
	if(!getClockOfTimeStamp(stamp, &hour, &minute, &second))return FALSE;

	sprintf(pBuffer, "%04d%02d%02d%02d%02d%02d", year, month, day, hour, minute, second);
	pBuffer[14] = 0;
	return TRUE;
}

structTimeStamp addTimeStamp(structTimeStamp stamp, int millSecons){
	return stamp + millSecons;
}
structTimeStamp getNextDayTimeStamp(structTimeStamp stamp){
	return addTimeStamp(stamp, MillSecondsOneDay);
}
structTimeStamp getLastDayTimeStamp(structTimeStamp stamp){
	return addTimeStamp(stamp, -MillSecondsOneDay);
}
structTimeStamp getThisWeekStartTimeStamp(structTimeStamp stamp){
	stamp = setTimeStampMillSeconds(stamp, 0);
	return addTimeStamp(stamp, -MillSecondsOneDay * getWeekOfTime(stamp));
}
int compareTimeStamp(structTimeStamp time1, structTimeStamp time2){
	if(time1 == time2)return 0;
	return (time1 > time2) ? 1 : -1;
}

structTimeStampDiff getTimeDiffByPara(int days, int millSecons){
	structTimeStampDiff diff = days;

	diff *= MillSecondsOneDay;
	diff += millSecons;
	return diff;
}

structTimeStampDiff getTimeDiff(structTimeStamp stamp1, structTimeStamp stamp2){
	return stamp1 - stamp2;

}

structTimeStampDiff addTimeStampDiff(structTimeStampDiff diff1, structTimeStampDiff diff2){
	return diff1 + diff2;
}
structTimeStampDiff addTimeStampDiffByMillSeconds(structTimeStampDiff diff, int millSecons){
	return diff + millSecons;
}
int getYearOfCurrentUTCTime(void){
	return getYearOfTime(getCurrentTime());
}

int getMonthOfCurrentUTCTime(void){
	return getMonthOfTime(getCurrentTime());
}
int getDayOfCurrentUTCTime(void){
	return getDayOfTime(getCurrentTime());	
}
int getWeekOfCurrentUTCTime(void){
	return getWeekOfTime(getCurrentTime());	
}
int getHourOfCurrentUTCTime(void){
	return getHourOfTime(getCurrentTime());	
}
int getMinuteOfCurrentUTCTime(void){
	return getMinuteOfTime(getCurrentTime());	
}
int getSecondOfCurrentUTCTime(void){
	return getSecondOfTime(getCurrentTime());	
}
int getMillSecondOfCurrentUTCTime(void){
	return (int)(getCurrentTime() % 1000);	
}
int getDaysOfDiff(structTimeStampDiff diff){
	return (int)(diff / MillSecondsOneDay);
}
int getHourOfDiff(structTimeStampDiff diff){
	return (int)((diff % MillSecondsOneDay) / 1000) / 3600;
}
int getMinuteOfDiff(structTimeStampDiff diff){
	return (((int)(diff % MillSecondsOneDay) / 1000) % 3600) / 60;
}
int getSecondOfDiff(structTimeStampDiff diff){
	return (((int)(diff % MillSecondsOneDay) / 1000) % 3600) % 60;
}
int getMillSecondOfDiff(structTimeStampDiff diff){
	return (int)(diff % 1000);
}
int getTotalMillSecondsOfDiff(structTimeStampDiff diff){
	return (int)diff;
}
BOOL getClockOfTimeDiff(structTimeStampDiff diff, int *pHour, int *pMinute, int *pSecond, int *pMillSecond){
	int iMillSecond = (int)(diff % MillSecondsOneDay);
	int iSecond = iMillSecond / 1000;
	
	if(!pHour || !pMinute || !pSecond || !pMillSecond)return FALSE;
	*pHour = iSecond / 3600;
	*pMinute = (iSecond % 3600) / 60;
	*pSecond = iSecond % 60;
	*pMillSecond = iMillSecond % 1000;
	return TRUE;
}
/*
	功能作用:根据生日计算年龄(周岁)
	参数说明:birthday	生日
	返 回 值:如题
	开发人员:付华强
*/
int getAgeByBirthday(structTimeStamp birthday){
	structTimeStamp now = getCurrentLocalTime();
	
	int iYear = getYearOfTime(now) - getYearOfTime(birthday);
	int iMonth = getMonthOfTime(now) - getMonthOfTime(birthday);
	int iDay = getDayOfTime(now) - getDayOfTime(birthday);

	if(iDay < 0)iMonth--;
	if(iMonth < 0)iYear--;
	if(iYear < 0)iYear = 0;
	return iYear;
}
#endif
