#ifndef __MY_NVRAM_20210814_FHQ_H__
#define __MY_NVRAM_20210814_FHQ_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

void my_nvram_init(void);
BOOL my_nvram_erase(void);
BOOL my_nvram_write(U8 * pBuffer, int iLen);
BOOL my_nvram_read(U8 * pBuffer, int iLen);


#if defined(__cplusplus)
}
#endif 

#endif //__MY_NVRAM_20210814_FHQ_H__



