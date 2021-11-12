#ifndef __MY_CACHE_BUFFER_H_20200308_FHQ__
#define __MY_CACHE_BUFFER_H_20200308_FHQ__

//#include "MyCommonInnerInclude.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#define TEMP_BUF_SIZE 288
#define LARGE_BUF_SIZE 4096
#define HUGE_BUF_SIZE (4)

#define FILELIST_BUF_SIZE (4096)
#define FILEIO_BUF_SIZE (4096)


char * getMainTaskTempBuf(void);
char * getMainTaskLargeBuf(void);
char * getMainTaskHugeBuf(void);
char * getFileIOBuf(void);
char * getFileListBuf(void);
int getMainTaskTempBufSize(void);
int getMainTaskLargeBufSize(void);
int getMainTaskHugeBufSize(void);
int getFileIOBufSize(void);
int getFileListBufSize(void);

#if defined(__cplusplus)
}
#endif 

#endif //__MY_CACHE_BUFFER_H_20200308_FHQ__



