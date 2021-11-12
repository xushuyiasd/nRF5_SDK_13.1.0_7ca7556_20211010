#include <stddef.h>
#include <string.h>
#include <stdio.h>


#include "DriverInterface.h"

#ifdef SUPPORT_MY_FILESYSTEM
#if defined(SUPPORT_PLATFORM_MTK)
#include "hal.h"
#endif


BOOL filesystem_disk_on(void){
	printf("filesystem_disk_on\r\n");
	return MountFileSystem(TRUE);
}
BOOL filesystem_disk_off(void){
	printf("filesystem_disk_off\r\n");
	return MountFileSystem(FALSE);
}

BOOL filesystem_disk_format(char * pBuf, int iBufSize){
	return FormatFileSystem(pBuf, iBufSize);
}

#endif
