#ifndef __MY_POWER_MANAGER_FHQ_20200204_H__
#define __MY_POWER_MANAGER_FHQ_20200204_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#ifdef SUPPORT_MY_POWERMANAGER
void power_manager_init(void);
int getBatteryPercent(void);
int getBatteryVoltage(void);
int getBatteryLevel(void);
BOOL isCharging(void);

#endif

#if defined(__cplusplus)
}
#endif 

#endif //__MY_POWER_MANAGER_FHQ_20200204_H__



