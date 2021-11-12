#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "DriverInterface.h"
#include "MyCommon.h"
#include "MySettings.h"
#include "MyNVRam.h"

#define MAX_MEMORY_DUMP_SIZE 1024
static BOOL static_bSleepMode = FALSE;
static int static_iSystemPowerStep = 0;

static char static_memdum[MAX_MEMORY_DUMP_SIZE];

BOOL isSystemSleep(void){
	return static_bSleepMode;
}
void setSystemSleep(BOOL value){
	static_bSleepMode = value;
}


int getSystemPowerStep(void){
	return static_iSystemPowerStep;
}
void setSystemPowerStep(int value){
	static_iSystemPowerStep = value;
}


static U8 Hex2Value(U8 c){
	if(c >= '0' && c <= '9'){
		c -= '0';
	}else if(c >= 'a' && c <= 'f'){
		c -= 'a' - 10;
	}else if(c >= 'A' && c <= 'F'){
		c -= 'A' - 10;
	}else{
		c = 0;
	}
	return c;
}
U8 hex2Byte(U8 c1, U8 c2){
	return (U8)(Hex2Value(c1) << 4) | Hex2Value(c2);
}

BOOL hexString2Bin(U8 * pDest, U8 * pSrc, int iSrcLen){
	int i;
	if(!pDest || !pSrc)return  FALSE;
	if(iSrcLen % 2)return FALSE;
	for(i = 0; i < iSrcLen; i += 2, pSrc += 2, pDest++){
		*pDest = hex2Byte(*pSrc, *(pSrc + 1));
	}
	return TRUE;
}
static U8 value2Hex(U8 c){
	if(c <= 9){
		c += '0';
	}else if((c >= 10) && (c <= 15)){
		c = 'A' + c - 10;
	}else{
		c = '0';
	}
	return c;
}
BOOL Bin2hexString(U8 * pDest, U8 * pSrc, int iSrcLen){
	int i;

	if(!pDest || !pSrc)return  FALSE;
	for(i = 0; i < iSrcLen; i++, pDest += 2, pSrc++){
		*pDest = value2Hex((*pSrc) >> 4);
		*(pDest + 1) = value2Hex((*pSrc) & 0x0F);
	}
	return TRUE;
}
U16 hex2Short(const U8 * pString){
	if(!pString)return 0;
	if(strlen((const char *)pString) > 4)return 0;
	return (hex2Byte(*pString, *(pString + 1)) << 8) | hex2Byte(*(pString + 2), *(pString + 3));
}
char *rtrim(char *str){
	int len;
	char * p;
	
	if ((str == NULL) || (!*str))return str;
	len = strlen(str);
	p = str + len - 1;
	while ((len--)  && isspace((U8)(*p)))p--;
	*(p + 1) = 0;
	return str;
} 
char *ltrim(char *str){
	char * p = str;
	
	if ((str == NULL) || (!*str))return str;
	while (isspace((U8)(*p)))p++;
	return p;
} 
char *trim(char *str){
	str = rtrim(str);
	str = ltrim(str);
	return str;
}
char * deleteQuotation(char * pData){
	int iCount = 0;
	char * pStart = NULL;
	char * pEnd = NULL;
	
	if(!pData)return NULL;
	pStart = pData;
	while(*pStart == '\"')pStart++;
	
	iCount = strlen(pStart);
	if(iCount <= 0)return pStart;
	pEnd = pStart + iCount - 1;
	while((pEnd >= pStart) && (*pEnd == '\"')){
		pEnd--;
	}
	*(pEnd + 1) = 0;
	return pStart;	
}
char * deleteTitle(char * pData, const char * pTitle){
	int iLen;
	
	if(!pData || !pTitle)return pData;
	iLen = strlen(pTitle);
	if(memcmp(pData, pTitle, iLen) != 0)iLen = 0;
	return pData + iLen;
}
char * getATValidContent(char * pData, const char * pTitle){
	pData = deleteTitle(pData, pTitle);
	if (*pData == ':')pData++;
	pData = trim(pData);
//	pData = deleteQuotation(pData);
	return pData;
}

BOOL isHexText(char * pString){
	char * p = pString;
	char c;

	if(!pString)return FALSE;
	while(*p){
		c = *p++;
		if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))return FALSE;
	}
	return TRUE;
}
U16 VBE_B2L_16(U16 value){
	return (value << 8) | (value >> 8);
}
U32 VBE_B2L_32(U32 value){
 	value = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0x00FF00FF);
        return (value << 16) | (value >> 16);
}
BOOL isMemZero(char * pBuffer, int iLen){
	int i;
	char * p = pBuffer;
	for(i = 0; i < iLen; i++, p++){
		if(*p)return FALSE;
	}
	return TRUE;
}
void myMemDump(const U8 * pBuffer, int iLen){
	U8 * pSrc = (U8 *)pBuffer;
	U8 * pDest = (U8 *)static_memdum;
	int i;
	U8 c;

	if(!pBuffer)return;
	if(iLen >= MAX_MEMORY_DUMP_SIZE / 3){
		return;
		}
	
	for(i = 0; i < iLen; i++, pSrc++){
		c = (*pSrc) >> 4;
		if(c <= 9){
			*pDest = '0' + c;
		}else{
			*pDest = 'A' + c - 10;
		}

		pDest++;
		c = (*pSrc) & 0x0F;
		if( c <= 9){
			*pDest = '0' + c;
		}else{
			*pDest = 'A' + c - 10;
		}
		pDest++;

		*pDest++ = ' ';
	}
	*pDest++ = '\r';
	*pDest++ = '\n';
	*pDest = 0;
	main_printf("%s", (U32)static_memdum);
}
void my_nvram_load_settings(void){
#ifdef SUPPORT_MY_NVRAM
	memset(getMySetting(), 0, sizeof(structMySetting));
	my_nvram_read((U8 *)getMySetting(), sizeof(structMySetting));
#endif
}
void my_nvram_save_settings(void){
#ifdef SUPPORT_MY_NVRAM
	my_nvram_erase();
	my_nvram_write((U8 *)getMySetting(), sizeof(structMySetting));
#endif	
}
