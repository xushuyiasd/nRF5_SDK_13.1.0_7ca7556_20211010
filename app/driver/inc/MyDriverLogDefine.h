#ifndef __MY_DRIVER_LOG_DEFINE_20191202_H__
#define __MY_DRIVER_LOG_DEFINE_20191202_H__

#if defined(__cplusplus)
extern "C" {     
#endif

#include "MyGlobalDefine.h"

#if defined(SUPPORT_PLATFORM_NORDIC)
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#if NRF_LOG_ENABLED
#define myprintf NRF_LOG_INFO
#else
extern void my_log_printf(char* strFormat, ...);
#ifdef SUPPORT_VERSION_DEBUG
#define myprintf my_log_printf
#else
#define myprintf(...)
#endif
#endif
#endif

#define SUPPORT_LOG_SYSTEM
#define SUPPORT_LOG_GPS
#define SUPPORT_LOG_MODULE_4G
#define SUPPORT_LOG_MAINTASK
#define SUPPORT_LOG_WATCHTASK
#define SUPPORT_LOG_BUZZER
#define SUPPORT_LOG_VIBURATOR
#define SUPPORT_LOG_WIFI
#define SUPPORT_LOG_TP
#define SUPPORT_LOG_LED
#define SUPPORT_LOG_BLE
#define SUPPORT_LOG_LCD
#define SUPPORT_LOG_BL
#define SUPPORT_LOG_PRESSURE
#define SUPPORT_LOG_TEMPERATURE
#define SUPPORT_LOG_HUMIDITY
#define SUPPORT_LOG_HEARTRATE
#define SUPPORT_LOG_COMPASS
#define SUPPORT_LOG_SENSORHUB
//#define SUPPORT_LOG_UART
#define SUPPORT_LOG_GYRO
#define SUPPORT_LOG_KEY
#define SUPPORT_LOG_FATFS
#define SUPPORT_LOG_GSENSOR



#ifdef SUPPORT_LOG_SYSTEM
#define system_printf myprintf
#else
#define system_printf(...)
#endif

#ifdef SUPPORT_LOG_BLE
#define ble_printf myprintf
#else
#define ble_printf(...)
#endif
#ifdef SUPPORT_LOG_BL
#define bl_printf myprintf
#else
#define bl_printf(...)
#endif
#ifdef SUPPORT_LOG_LCD
#define lcd_printf myprintf
#else
#define lcd_printf(...)
#endif

#ifdef SUPPORT_LOG_LED
#define led_printf myprintf
#else
#define led_printf(...)
#endif

#ifdef SUPPORT_LOG_GPS
#define gps_printf myprintf
#else
#define gps_printf(...)
#endif

#ifdef SUPPORT_LOG_MODULE_4G
#define module4g_printf myprintf
#else
#define module4g_printf(...)
#endif

#ifdef SUPPORT_LOG_COMPASS
#define compass_printf myprintf
#else
#define compass_printf(...)
#endif

#ifdef SUPPORT_LOG_SENSORHUB
#define sensorhub_printf myprintf
#else
#define sensorhub_printf(...)
#endif

#ifdef SUPPORT_LOG_MAINTASK
#define maintask_printf myprintf
#else
#define maintask_printf(...)
#endif

#ifdef SUPPORT_LOG_WATCHTASK
#define watchtask_printf myprintf
#else
#define watchtask_printf(...)
#endif


#ifdef SUPPORT_LOG_GPS
#define gps_printf myprintf
#else
#define gps_printf(...)
#endif

#ifdef SUPPORT_LOG_GYRO
#define gyro_printf myprintf
#else
#define gyro_printf(...)
#endif

#ifdef SUPPORT_LOG_GSENSOR
#define gsensor_printf myprintf
#else
#define gsensor_printf(...)
#endif

#ifdef SUPPORT_LOG_KEY
#define key_printf myprintf
#else
#define key_printf(...)
#endif

#ifdef SUPPORT_LOG_BUZZER
#define buzzer_printf myprintf
#else
#define buzzer_printf(...)
#endif

#ifdef SUPPORT_LOG_VIBURATOR
#define viburator_printf myprintf
#else
#define viburator_printf(...)
#endif

#ifdef SUPPORT_LOG_WIFI
#define wifi_printf myprintf
#else
#define wifi_printf(...)
#endif

#ifdef SUPPORT_LOG_TP
#define tp_printf myprintf
#else
#define tp_printf(...)
#endif

#ifdef SUPPORT_LOG_PRESSURE
#define pressure_printf myprintf
#else
#define pressure_printf(...)
#endif

#ifdef SUPPORT_LOG_TEMPERATURE
#define temperature_printf myprintf
#else
#define temperature_printf(...)
#endif

#ifdef SUPPORT_LOG_HUMIDITY
#define humidity_printf myprintf
#else
#define humidity_printf(...)
#endif

#ifdef SUPPORT_LOG_HEARTRATE
#define heartrate_printf myprintf
#else
#define heartrate_printf(...)
#endif

#ifdef SUPPORT_LOG_UART
#define uart_printf myprintf
#else
#define uart_printf(...)
#endif
#ifdef SUPPORT_LOG_FATFS
#define fileio_printf myprintf
#else
#define fileio_printf(...)
#endif

#define main_printf myprintf

#if defined(__cplusplus)
}
#endif 

#endif //__MY_DRIVER_LOG_DEFINE_20191202_H__



