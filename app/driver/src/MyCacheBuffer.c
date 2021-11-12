#include <stddef.h>
#include <stdlib.h>

//#include "MyDataStruct.h"
#include "MyMemory.h"
#include "MyCacheBuffer.h"




static char ATTRIBUTE_ALIGN static_bufTemp[TEMP_BUF_SIZE];
static char ATTRIBUTE_ALIGN static_largeBuffer[LARGE_BUF_SIZE];
static char ATTRIBUTE_ALIGN static_hugeBuffer[HUGE_BUF_SIZE];


char * getMainTaskTempBuf(void){
	return static_bufTemp;
}
char * getMainTaskLargeBuf(void){
	return static_largeBuffer;
}
char * getMainTaskHugeBuf(void){
	return static_hugeBuffer;
}


int getMainTaskTempBufSize(void){
	return TEMP_BUF_SIZE;
}
int getMainTaskLargeBufSize(void){
	return LARGE_BUF_SIZE;
}
int getMainTaskHugeBufSize(void){
	return HUGE_BUF_SIZE;
}
#ifdef SUPPORT_MY_FILEIO
static char ATTRIBUTE_ALIGN static_fileListBuffer[FILELIST_BUF_SIZE];
static char ATTRIBUTE_ALIGN static_FileIOBuffer[FILEIO_BUF_SIZE];

char * getFileIOBuf(void){
	return static_FileIOBuffer;
}
char * getFileListBuf(void){
	return static_fileListBuffer;
}
int getFileIOBufSize(void){
	return FILEIO_BUF_SIZE;
}
int getFileListBufSize(void){
	return FILELIST_BUF_SIZE;
}
#endif
