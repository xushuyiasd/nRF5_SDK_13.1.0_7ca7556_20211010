#ifndef __MY_GLOBAL_DEFINE_20200213_FHQ_H__
#define __MY_GLOBAL_DEFINE_20200213_FHQ_H__

#if defined(__cplusplus)
extern "C" {     
#endif

#ifdef WIN32
#define SUPPORT_PLATFORM_VS
#else
//#define SUPPORT_PLATFORM_MTK
#define SUPPORT_PLATFORM_NORDIC
#endif

#if defined(SUPPORT_PLATFORM_VS)
#define SUPPORT_VS_FILE
#define SUPPORT_FATFS
#elif defined(SUPPORT_PLATFORM_MTK)
#define SUPPORT_SENSOR_TASK
#endif
#define SUPPORT_TIMER_EXCUTE_IN_TASK
#define SUPPORT_MY_DATETIME

//#define TRACKIMO_SILENT_TRACKER
//#define SUPPORT_VERSION_DEBUG
#if defined(__cplusplus)
}
#endif 

#endif //__MY_GLOBAL_DEFINE_20200213_FHQ_H__



