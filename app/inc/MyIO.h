#ifndef __MY_IO_FHQ_20210506_H__
#define __MY_IO_FHQ_20210506_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#define HUB_MAX_MISO_SIZE 32
#define HUB_MAX_MOSI_SIZE 32

typedef enum{
HUB_REG_DATA_READY,
HUB_REG_VERSION,
HUB_REG_NOTIFY,
HUB_REG_SENSOR,
HUB_REG_COMPASS_CALIBRATION,
HUB_REG_WEARPOS,
HUB_REG_SPORTTYPE,
HUB_REG_SILENT_TIME,
HUB_REG_MOVING_ENERGY,
HUB_REG_UPGRADE_LENGTH,
HUB_REG_UPGRADE_CRC,
HUB_REG_UPGRADE_DATA,
}enumSensorHubReg;


typedef enum{
SENSOR_OPERATE_CLOSE,
SENSOR_OPERATE_OPEN,
SENSOR_OPERATE_OPEN_NOTIFY,
SENSOR_OPERATE_CLOSE_NOTIFY,
SENSOR_OPERATE_GETDATA,
SENSOR_OPERATE_MAX	
}enumSensorOperate;

typedef enum{
GSENSOR_NOTIFY_RAW,	
GSENSOR_NOTIFY_STEP,
GSENSOR_NOTIFY_SILENT,
GSENSOR_NOTIFY_GESTURE,
GSENSOR_NOTIFY_MAX
}enumGSensorNotify;

typedef enum{
GSENSOR_GETDATA_STEP,
GSENSOR_GETDATA_SILENT,	
GSENSOR_GETDATA_MAX
}enumGSensorData;

typedef enum{
COMPASS_NOTIFY_ANGLE,
COMPASS_NOTIFY_RAW,	
COMPASS_NOTIFY_MAX
}enumCompassNotify;

typedef enum {
HUB_SENSOR_GSENSOR,
HUB_SENSOR_COMPASS,
HUB_SENSOR_TEMPERATURE,
HUB_SENSOR_PRESSURE,
HUB_SENSOR_GYRO,
HUB_SENSOR_MAX
}enumHubSensor;
typedef enum{
HUB_NOTIFY_NULL,
HUB_NOTIFY_GSENSOR_STEP,
HUB_NOTIFY_GSENSOR_RAW,
HUB_NOTIFY_GSENSOR_SILENT,
HUB_NOTIFY_GSENSOR_GESTURE,
HUB_NOTIFY_COMPASS_ANGLE,
HUB_NOTIFY_COMPASS_RAW,
HUB_NOTIFY_COMPASS_CALIBRATION,
HUB_NOTIFY_GYRO_RAW,
HUB_NOTIFY_MULTI_AXIES_RAW,
HUB_NOTIFY_TEMPERATURE,
HUB_NOTIFY_PRESSURE,
HUB_NOTIFY_LOG,
HUB_NOTIFY_MAX
}enumHubNotify;


U8 * myio_get_buf_receive(void);
U8 * myio_get_buf_send(void);
int myio_get_buf_send_size(void);
int myio_get_buf_receive_size(void);
BOOL myio_is_sensor_open(int sensor);
void myio_set_sensor_open(int sensor, BOOL value);
BOOL myio_is_notify(int type);
void myio_set_notify(int type, BOOL value);
int myio_get_sport_type(void);
void myio_set_sport_type(int value);
int myio_get_wearpos(void);
void myio_set_wearpos(int value);
int myio_get_silent_time(void);
void myio_set_silent_time(int value);
int myio_get_moving_energy(void);
void myio_set_moving_energy(int value);
int myio_get_upgrade_length(void);
void myio_set_upgrade_length(int value);
int myio_get_upgrade_CRC(void);
void myio_set_upgrade_CRC(int value);

void myio_init(void);

void myio_process(void);
void myio_notify_temperature(void);
void myio_notify_pressure(void);
void myio_notify_compass_angle(void);
void myio_log_printf(char* strFormat, ...);

#if defined(__cplusplus)
}
#endif 

#endif //__MY_IO_FHQ_20210506_H__

