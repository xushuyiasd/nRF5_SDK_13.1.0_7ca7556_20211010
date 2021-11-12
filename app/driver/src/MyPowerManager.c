#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "DriverInterface.h"

#ifdef SUPPORT_MY_POWERMANAGER

#if defined(SUPPORT_PLATFORM_MTK)
#include "hal.h"
#include "battery_management.h"
#endif

void power_manager_init(void){
	
}

int getBatteryPercent(void){
	int value = 50;
#if defined(SUPPORT_PLATFORM_MTK)
	value = battery_management_get_battery_property(BATTERY_PROPERTY_CAPACITY);
#endif
	return value;
}

int getBatteryVoltage(void){
	int value = 420;	//4//4.2v
#if defined(SUPPORT_PLATFORM_MTK)
	value = battery_management_get_battery_property(BATTERY_PROPERTY_VOLTAGE);
#endif
	return value;
}

int getBatteryLevel(void){
	int value = 0;
#if defined(SUPPORT_PLATFORM_MTK)
	value = battery_management_get_battery_property(BATTERY_PROPERTY_CAPACITY_LEVEL);
#endif
	return value;
}

BOOL isCharging(void){
	BOOL bCharging = FALSE;
#if defined(SUPPORT_PLATFORM_MTK)
	bCharging = battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_EXIST);
#endif
	return bCharging;
}



#endif
