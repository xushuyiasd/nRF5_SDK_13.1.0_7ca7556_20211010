#ifndef __MY_COMPILER_DEFINE_20190124_FHQ_H__
#define __MY_COMPILER_DEFINE_20190124_FHQ_H__

#include "MyGlobalDefine.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed int   /*   signed 32 bits. */
#define U32 unsigned int   /* unsigned 32 bits. */
#define I16P I16              /*   signed 16 bits OR MORE ! */
#define U16P U16              /* unsigned 16 bits OR MORE ! */

typedef I8 kal_int8;
typedef U8 kal_uint8;
typedef I16 kal_int16;
typedef U16 kal_uint16;
typedef I32 kal_int32;
typedef U32 kal_uint32;
typedef U8 kal_char;
typedef U16 kal_wchar;
typedef int kal_bool;
typedef int MMI_BOOL;

#ifdef SUPPORT_PLATFORM_VS
#define I64 __int64 /* signed 64 bits. */
#else
#define I64 long long int
#endif

#define S8 I8
#define S16 I16
#define S32 I32

#define PU16 U16 *
#define PU8 U8 *

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#define KAL_TRUE TRUE
#define KAL_FALSE FALSE

#define ISBOOL(value) (((value)==0)||((value)==1))

#if defined(SUPPORT_PLATFORM_VS)
#elif defined(SUPPORT_PLATFORM_MTK)
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal.h"
#include "hal_dcxo.h"
#include "hal_platform.h"
#include "hal_gpt.h"
#include "sensor_alg_interface.h"
#include "hal_keypad.h"
#include "keypad_custom.h"
#include "gnss_api.h"
#include "memory_attribute.h"
#include "hal_gpio.h"
#include "hal_pinmux_define.h"
#endif
#if defined(__cplusplus)
}
#endif 

#endif //__MY_COMPILER_DEFINE_20190124_FHQ_H__



