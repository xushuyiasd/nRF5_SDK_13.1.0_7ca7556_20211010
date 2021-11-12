#ifndef __MY_GSENSOR_FHQ_20200203_H__
#define __MY_GSENSOR_FHQ_20200203_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif
typedef enum{
GSENSOR_SENSITIVITY_LOW,
GSENSOR_SENSITIVITY_MIDDLE,
GSENSOR_SENSITIVITY_TOP,
GSENSOR_SENSITIVITY_MAX
}enumGSensorSensitivity;
typedef enum{
GSENSOR_EVENT_MOVE,
GSENSOR_EVENT_SILENT,
GSENSOR_EVENT_FALL,
GSENSOR_EVENT_MAX
}enumGSensorEvent;

void gSensor_driver_init(void);
void gSensor_on(void);
void gSensor_off(void);
void gSensor_get_data(I16 * pX, I16 * pY, I16 * pZ);
void gSensor_set_sensitivity(int type);

#if defined(__cplusplus)
}
#endif 

#endif //__MY_GSENSOR_FHQ_20200203_H__



