#ifndef __MY_COMMON_FHQ_20190722_H__
#define __MY_COMMON_FHQ_20190722_H__
#include "CommonInterface.h"
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
#define FATFS_BLOCK_SIZE 4096



BOOL isSystemSleep(void);
void setSystemSleep(BOOL value);

int getSystemPowerStep(void);
void setSystemPowerStep(int value);

U8 hex2Byte(U8 c1, U8 c2);
U16 hex2Short(const U8 * pString);


BOOL hexString2Bin(U8 * pDest, U8 * pSrc, int iSrcLen);
BOOL Bin2hexString(U8 * pDest, U8 * pSrc, int iSrcLen);
BOOL isHexText(char * pString);
char *rtrim(char *str);
char *ltrim(char *str);
char *trim(char *str);
char * deleteQuotation(char * pData);
char * deleteTitle(char * pData, const char * pTitle);
char * getATValidContent(char * pData, const char * pTitle);

U16 VBE_B2L_16(U16 value);
U32 VBE_B2L_32(U32 value);

BOOL isMemZero(char * pBuffer, int iLen);
void myMemDump(const U8 * pBuffer, int iLen);

void my_nvram_load_settings(void);
void my_nvram_save_settings(void);
#if defined(__cplusplus)
}
#endif 
#endif //__MY_COMMON_FHQ_20190722_H__

