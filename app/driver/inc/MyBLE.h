#ifndef __MY_BLE_FHQ_20190722_H__
#define __MY_BLE_FHQ_20190722_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif
#ifdef SUPPORT_MY_BLE
typedef enum {
	BLE_EVENT_INTERNAL,
	BLE_EVENT_POWER_ON,
	BLE_EVENT_POWER_OFF,
	BLE_EVENT_CONNECTED,
	BLE_EVENT_DISCONNECTED,
	BLE_EVENT_MAX
} enumBLEEvent;

void refreshBTName(void);
void ble_driver_init(void);
void ble_on(void);
void ble_off(void);
BOOL ble_is_working(void);
BOOL isEnableBLE(void);
void setEnableBLE(BOOL value);
char * getBTAddr(void);

#endif
#if defined(__cplusplus)
}
#endif 

#endif //__MY_BLE_FHQ_20190722_H__



