#ifndef WATCH_FILEIO_H
#define WATCH_FILEIO_H

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#ifdef SUPPORT_MY_FILEIO

#define FILE_SECTOR_SIZE 4096
#define LINE_CONTENT_MAX_CHARACTERS 100
#define MAX_FILENAME_LENGTH 64

#ifndef SEEK_SET
#define SEEK_SET        0       /* Seek from beginning of file.  */
#define SEEK_CUR        1       /* Seek from current position.  */
#define SEEK_END        2       /* Set file pointer to EOF plus "offset" */
#endif

#ifdef SUPPORT_VS_FILE
#define MF_FILE     FILE
#define mf_fclose   fclose
#define mf_fopen    fopen
#define mf_fread    fread
#define mf_write    fwrite
#define mf_fseek    fseek
#define mf_ftell    ftell
#else
int mf_close (void* fp);
int mf_fread(void *ptr, int size, int nmemb, void *stream);
int mf_write(void *ptr, int size, int nmemb, void *stream);
int mf_fseek(void *stream, long offset, int fromwhere);
void *mf_fopen(const void *filename, const char *mode);
int mf_ftell(void *stream);

#endif
#define MyStrcpy strcpy
#define MyStrlen strlen
#define MyStrcat strcat
#define MyStrcmp strcmp

typedef enum{
OPENMODE_READ_TEXT,		//0
OPENMODE_READ_BIN,
OPENMODE_WRITE_TEXT,
OPENMODE_WRITE_BIN,
OPENMODE_APPEND_BIN,
OPENMODE_CREATE_WRITE_BIN,	//5
OPENMODE_RW,
OPENMODE_MAX
}enumOpenMode;

#ifdef SUPPORT_UNICODE
#define MyVSRename _wrename
#define MyVSFOpen _wfsopen
#define MyVSMKDir _wmkdir
#define MyVSRemove _wremove
#define MyVSRMDir _wrmdir
#else
#define MyVSRename rename
#define MyVSFOpen fopen
#define MyVSMKDir _mkdir
#define MyVSRemove remove
#define MyVSRMDir _rmdir

#endif
	
/*
	功能作用:复位文件错误号
	参数说明:无
	返 回 值:无
	开发人员:付华强
*/
void resetCloseFileError(void);
/*
	功能作用:获取文件关闭错误号
	参数说明:无
	返 回 值:如题
	开发人员:付华强
*/
int getCloseFileError(void);
/*
	功能作用:获取磁盘块数
	参数说明:无
	返 回 值:如题
	开发人员:付华强
*/
int getFileSectorSize(void);
/*
	功能作用:获取磁盘总空间
	参数说明:无
	返 回 值:如题
	开发人员:付华强
*/
int getDiskTotalSize(void);
/*
	功能作用:获取磁盘剩余空间
	参数说明:无
	返 回 值:如题
	开发人员:付华强
*/
int getDiskFreeSize(void);
/*
	功能作用:文件是否存在
	参数说明:filename(in)	完整文件名
	返 回 值:如题
	开发人员:付华强
*/
BOOL isFileExist(const MYSTRING filename);
/*
	功能作用:建立二进制文件,已指定值和指定长度填充
	参数说明:filename(in)	完整文件名
			 iLength(in)	文件长度
			 value(in)		填充数据
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL CreateBinFileOfLength(const MYSTRING filename, int iLength, char value);
/*
	功能作用:获取文件长度
	参数说明:filename(in)	完整文件名
	返 回 值:如题
	开发人员:付华强
*/
int getFileLength(const MYSTRING filename);
/*
	功能作用:获取文本文件内容
	参数说明:filename(in)	完整文件名
			 pBuffer(out)	文件内容
			 iBufSize(in)	pBuffer长度
	返 回 值:如题
	开发人员:付华强
*/
BOOL getTextFileContent(const MYSTRING filename, char * pBuffer, int iBufSize);
/*
	功能作用:获取文件行数
	参数说明:filename(in)	完整文件名
	返 回 值:如题
	开发人员:付华强
*/
int getLinesOfTextFile(const MYSTRING filename);
/*
	功能作用:创建新文件
	参数说明:filename(in)	完整文件名
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL CreateEmptyFile(const MYSTRING filename);
/*
	功能作用:向文件追加内容
	参数说明:filename(in)	完整文件名
			 pContent(in)	追加内容
			 iLen(in)		pContent长度
	返 回 值:成功与否
	注意事项:文件必须存在才成功
	开发人员:付华强
*/
BOOL AppendContent(const MYSTRING filename, const void * pContent, int iLen);
/*
	功能作用:向文件追加字符串
	参数说明:filename(in)	完整文件名
			 pString(in)	追加字符串
	返 回 值:成功与否
	注意事项:文件必须存在才成功
	开发人员:付华强
*/
BOOL AppendString(const MYSTRING filename, const char * pString);
/*
	功能作用:指定位置读取文件内容
	参数说明:filename(in)	完整文件名
			 pBuf(out)		文件内容
			 iLen(in)		pBuf长度
			 lPos(in)		开始读的位置
	返 回 值:成功与否
	注意事项:文件必须存在才成功
	开发人员:付华强
*/
BOOL ReadFileContentAtPos(const MYSTRING filename, void * pBuf, int iLen, long lPos);
/*
	功能作用:指定位置写文件内容
	参数说明:filename(in)	完整文件名
			 pBuf(in)		文件内容
			 iLen(in)		pBuf长度
			 lPos(in)		开始写的位置
	返 回 值:成功与否
	注意事项:文件必须存在才成功
	开发人员:付华强
*/
BOOL WriteFileContentAtPos(const MYSTRING filename, const void * pBuf, int iLen, long lPos);
/*
	功能作用:更新文件内容
	参数说明:filename(in)	完整文件名
			 pContent(in)	文件内容
			 iLen(in)		pContent长度
	返 回 值:成功与否
	注意事项:文件必须存在才成功
	开发人员:付华强
*/
BOOL WriteFileContent(const MYSTRING filename, void * pContent, int iLen);
/*
	功能作用:新建并写文件内容
	参数说明:filename(in)	完整文件名
			 pContent(in)	文件内容
			 iLen(in)		pContent长度
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL CreateWriteFileContent(const MYSTRING filename, const void * pContent, int iLen);
/*
	功能作用:文件重命名
	参数说明:pOld(in)	旧文件名(完整路径)
			 pNew(in)	新文件名(完整路径)
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL RenameFile(const MYSTRING pOld, const MYSTRING pNew);
/*
	功能作用:获取指定路径下的文件名数组
	参数说明:pPath(in)		指定路径
			 pBuf(out)		文件名字符串(字符串依次排列)
			 pBufLen(in/out) pBuf大小(in时) pBuf使用长度(out时)
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL getFilenameBufferOfPath(const MYSTRING pPath, MYSTRING pBuf, int *pBufLen);
/*
	功能作用:解析文件名列表
	参数说明:pBuf(in)		文件名数组
			 iBufLen(in)	pBuf大小
			 ppName(out)	文件名
			 iCount(in)		ppName个数
	返 回 值:解析成功的文件个数
	开发人员:付华强
*/
int getFilenameListFromBuffer(MYSTRING pBuf, int iBufLen, MYSTRING *ppName, int iCount);
/*
	功能作用:建立目录
	参数说明:path(in)	完整路径
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL CreateMyDirectory(const MYSTRING path);
/*
	功能作用:删除文件
	参数说明:filename(in)	完整路径
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL DeleteMyFile(const MYSTRING filename);
/*
	功能作用:装载卸载文件系统
	参数说明:bMount	TRUE=装载 FALSE=卸载
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL MountFileSystem(BOOL bMount);
/*
	功能作用:格式化磁盘(文件系统损坏情况下使用)
	参数说明:无
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL FormatFileSystem(char * pBuf, int iBufSize);
/*
	功能作用:清空磁盘，删除所有文件(不格式化)
	参数说明:无
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL ClearDisk(void);
BOOL deleteTree(const MYSTRING path);

#endif

#if defined(__cplusplus)
}
#endif 

#endif //WATCH_FILEIO_H



