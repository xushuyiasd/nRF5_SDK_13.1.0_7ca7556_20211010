#include <string.h>
#include "fstorage.h"

#include "DriverInterface.h"
#include "MyNVRam.h"

#ifdef SUPPORT_MY_NVRAM
#define MY_NVRAM_START_ADDRESS 0x7F000
#define MY_NVRAM_PAGES 1
#define MY_NVRAM_PAGE_SIZE_WORDS 1024
static volatile int static_iRet = -1;

//必须擦除后下载才起作用
static void fs_evt_handler(fs_evt_t const * const evt, fs_ret_t result){
	static_iRet = result;
    switch(evt->id){
        case FS_EVT_STORE:            
            break;
        case FS_EVT_ERASE:
            break;
    }
}
FS_REGISTER_CFG(fs_config_t fs_config) = {
//	.p_start_addr = (U32 const *)MY_NVRAM_START_ADDRESS,
//	.p_end_addr = ((U32 const *)MY_NVRAM_START_ADDRESS) + MY_NVRAM_PAGES * MY_NVRAM_PAGE_SIZE_WORDS,
		
	.callback  = fs_evt_handler, // Function for event callbacks.
	.num_pages = MY_NVRAM_PAGES,      // Number of physical flash pages required.
	.priority  = 0xFE            // Priority for flash usage.
};
static fs_config_t const * my_nvram_get_config(void){
	return &fs_config;
}
static U32 const * my_nvram_get_start_address(void){
	return my_nvram_get_config()->p_start_addr;
}
static int my_nvram_get_size(void){
	return (my_nvram_get_config()->p_end_addr - my_nvram_get_config()->p_start_addr) * sizeof(U32);
}
static int my_nvram_get_pages(void){
	return my_nvram_get_config()->num_pages;
}
void my_nvram_init(void){
	fs_init();
}

BOOL my_nvram_erase(void){
	static_iRet = -1;
	if(fs_erase(my_nvram_get_config(), my_nvram_get_start_address(), my_nvram_get_pages(), NULL) != FS_SUCCESS)return FALSE;
	while(static_iRet < 0);
	myprintf("my_nvram_erase:%d\r\n", static_iRet);
	return static_iRet == FS_SUCCESS;
}
BOOL my_nvram_write(U8 * pBuffer, int iLen){
	int iMaxLen = my_nvram_get_size();
	if((!pBuffer) || (iLen <= 0))return FALSE;
	if(iLen > iMaxLen)return FALSE;
	static_iRet = -1;
	if(fs_store(my_nvram_get_config(), my_nvram_get_start_address(), (U32 const * )pBuffer, iLen / sizeof(U32), NULL) != FS_SUCCESS)return FALSE;
	while(static_iRet < 0);
	myprintf("my_nvram_write:%d\r\n", static_iRet);
	return static_iRet == FS_SUCCESS;
}
BOOL my_nvram_read(U8 * pBuffer, int iLen){
	int iMaxLen = my_nvram_get_size();
	if((!pBuffer) || (iLen <= 0))return FALSE;
	if(iLen > iMaxLen)return FALSE;
	memcpy(pBuffer, my_nvram_get_start_address(), iLen);
	return TRUE;
}

#endif
