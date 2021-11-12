#ifndef __MY_SETTINGS_20190918_FHQ_H__
#define __MY_SETTINGS_20190918_FHQ_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif
#define TRACKIMO_DEFAULT_WORK_MODE SYSTEM_WORKMODE_WORK
#define TRACKIMO_DEFAULT_GSENSOR_SENSITIVITY 32
#define TRACKIMO_DEFAULT_GSENSOR_COUNT 10
#define TRACKIMO_DEFAULT_GSENSOR_TIME 60
#define DEFAUT_NVRAM_FLAG 0xAAAAAAAA
#define MAX_BT_NAME 31
#define TRACKIMO_GSENSOR_SILENT_TIME 60

typedef enum{
SYSTEM_WORKMODE_WORK,
SYSTEM_WORKMODE_SLEEP,
SYSTEM_WORKMODE_MAX
}enumWorkMode;

typedef struct{
int iFlag;
int iGSensorSensitivity;
int iGSensorCount;
int iGSensorTime;
char btName[32];
}structMySetting;

structMySetting * getMySetting(void);
void initMySetting(void);
BOOL isSettingFlagCheckOK(void);

int getSystemWorkMode(void);
void setSystemWorkMode(int value);
int getGSensorSensitivity(void);
void setGSensorSensitivity(int value);
int getGSensorCount(void);
void setGSensorCount(int value);
int getGSensorTime(void);
void setGSensorTime(int value);
char * getBTName(void);
char * getBTAddr(void);

#if defined(__cplusplus)
}
#endif 

#endif //__MY_SETTINGS_20190918_FHQ_H__



