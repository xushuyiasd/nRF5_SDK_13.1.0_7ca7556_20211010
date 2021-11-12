#ifndef __MY_FILESYSTEM_FHQ_20200204_H__
#define __MY_FILESYSTEM_FHQ_20200204_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#ifdef SUPPORT_MY_FILESYSTEM

BOOL filesystem_disk_on(void);
BOOL filesystem_disk_off(void);
BOOL filesystem_disk_format(char * pBuf, int iBufSize);

#endif

#if defined(__cplusplus)
}
#endif 

#endif //__MY_FILESYSTEM_FHQ_20200204_H__



