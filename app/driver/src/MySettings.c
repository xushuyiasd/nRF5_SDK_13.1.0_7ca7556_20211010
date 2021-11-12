#include <stddef.h>

#include "MySettings.h"



static structMySetting static_setting={
	.iFlag = DEFAUT_NVRAM_FLAG,
	.iGSensorSensitivity = TRACKIMO_DEFAULT_GSENSOR_SENSITIVITY,
	.iGSensorCount = TRACKIMO_DEFAULT_GSENSOR_COUNT,
	.iGSensorTime = TRACKIMO_DEFAULT_GSENSOR_TIME,
	.btName = {'T','R','A','C','K','I','M','O'},
};

structMySetting * getMySetting(void){
	return &static_setting;
}
static char static_btAddr[MAX_BT_NAME]={0};
static int static_iSystemWorkMode = TRACKIMO_DEFAULT_WORK_MODE;



void initMySetting(void){
	structMySetting * p = getMySetting();
	memset(p, 0, sizeof(structMySetting));
	p->iFlag = DEFAUT_NVRAM_FLAG;
	p->iGSensorSensitivity = TRACKIMO_DEFAULT_GSENSOR_SENSITIVITY,
	p->iGSensorCount = TRACKIMO_DEFAULT_GSENSOR_COUNT,
	p->iGSensorTime = TRACKIMO_DEFAULT_GSENSOR_TIME,
	strcpy(p->btName, "TRACKIMO");	
}
BOOL isSettingFlagCheckOK(void){
	return getMySetting()->iFlag == DEFAUT_NVRAM_FLAG;
}
int getSystemWorkMode(void){
	return static_iSystemWorkMode;
}
void setSystemWorkMode(int value){
	static_iSystemWorkMode = value;
}

int getGSensorSensitivity(void){
	return getMySetting()->iGSensorSensitivity;
}
void setGSensorSensitivity(int value){
	getMySetting()->iGSensorSensitivity = value;
}
int getGSensorCount(void){
	return getMySetting()->iGSensorCount;
}
void setGSensorCount(int value){
	getMySetting()->iGSensorCount = value;
}
int getGSensorTime(void){
	return getMySetting()->iGSensorTime;
}
void setGSensorTime(int value){
	getMySetting()->iGSensorTime = value;
}
char * getBTName(void){
	return getMySetting()->btName;
}
char * getBTAddr(void){
	return static_btAddr;
}
