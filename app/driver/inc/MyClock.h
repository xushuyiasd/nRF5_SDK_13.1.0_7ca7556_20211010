#ifndef __MY_CLOCK_FHQ_20200204_H__
#define __MY_CLOCK_FHQ_20200204_H__

#if defined(__cplusplus)
extern "C" {     
#endif
#include "CommonInterface.h"

#ifdef SUPPORT_MY_CLOCK
void clock_init(void);
structTimeStamp getCurrentHardwareTime(void);
void setCurrentHardwareTime(structTimeStamp stamp);
//millseccond
U32 getMySystemTimeTick(void);
//second
U32 getTotalSystemOnTime(void);
#endif

#if defined(__cplusplus)
}
#endif 

#endif //__MY_CLOCK_FHQ_20200204_H__



