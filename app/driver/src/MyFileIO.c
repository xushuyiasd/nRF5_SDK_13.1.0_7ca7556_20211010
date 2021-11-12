#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "DriverInterface.h"
#include "MySystem.h"
#include "MyFileIO.h"

#ifdef SUPPORT_VS_FILE
#include <direct.h>
#include <windows.h>
#endif


#ifdef SUPPORT_MY_FILEIO

//static QueueHandle_t *xFileQueue = NULL;

void  MYGUI_File_Lock (void)
{ 
  //  	if(xFileQueue)xSemaphoreTake(xFileQueue, portMAX_DELAY);

}


void  MYGUI_File_Unlock (void)
{ 
//	if(xFileQueue)xSemaphoreGive(xFileQueue);
}

#ifndef SUPPORT_VS_FILE
#include "ff.h"

static	FIL static_file;
static FATFS static_fatfs;
static FILINFO static_fno;

static const MYSTRING static_pDisk = _T("0:");


FIL * getFatFile(void){
	return &static_file;

}
FATFS * getFatFS(void){
	return &static_fatfs;
}
FILINFO * getFatFileInfo(void){
	return &static_fno;
}
static const MYSTRING getFatDisk(void){
	return static_pDisk;
}

#endif
int getFileSectorSize(void){
	return FILE_SECTOR_SIZE;
}
int getDiskTotalSize(void){
#ifndef SUPPORT_VS_FILE
	FATFS * pFS = getFatFS();
	U32 clusters = 0;
	int tot_sect = 0;
	int result;
	
	result = f_getfree(getFatDisk(), (DWORD *)&clusters, &pFS);
	fileio_printf("f_getfree total:%d,%d,%d,%d\r\n", result, pFS->csize, (int)pFS->n_fatent, pFS->ssize);
	tot_sect = (pFS->n_fatent - 2) * pFS->csize;
	tot_sect *= pFS->ssize / 512;
	return tot_sect >> 1;
#else
	return 4096 * (getFileSectorSize() / 1024);
#endif
}
int getDiskFreeSize(void){
	U32 clusters = 0;

#ifndef SUPPORT_VS_FILE
	int result;
	FATFS * pFS = getFatFS();
	int fre_sect = 0;

	result = f_getfree(getFatDisk(), (DWORD *)&clusters, &pFS);
	fileio_printf("f_getfree free:%d,%d,%d,%d\r\n", result, pFS->csize, (int)clusters, pFS->ssize);
	fre_sect = clusters * pFS->csize;
	fre_sect *= pFS->ssize / 512;
	return fre_sect >> 1;
#else
	clusters = 4096;
#endif
	return clusters * (getFileSectorSize() / 1024);
}
#ifdef SUPPORT_VS_FILE

static MYSTRING getOpenMode(enumOpenMode mode){
	MYSTRING p = NULL;

	switch(mode){
		case OPENMODE_READ_TEXT:
		default:
			p = _T("r");
			break;
		case OPENMODE_READ_BIN:
			p = _T("rb");
			break;
		case OPENMODE_WRITE_TEXT:
			p = _T("w");
			break;
		case OPENMODE_WRITE_BIN:
			p = _T("wb");
			break;
		case OPENMODE_APPEND_BIN:
			p = _T("ab");
			break;
		case OPENMODE_CREATE_WRITE_BIN:
			p = _T("wb");
			break;
		case OPENMODE_RW:
			p = _T("rb+");
			break;
	}
	return p;
}

#else

static int getOpenMode(enumOpenMode mode){
	int value = 0;
	switch(mode){
		case OPENMODE_READ_TEXT:
		default:
			value = FA_READ | FA_OPEN_EXISTING;
			break;
		case OPENMODE_READ_BIN:
			value = FA_READ | FA_OPEN_EXISTING;
			break;
		case OPENMODE_WRITE_TEXT:
			value = FA_WRITE | FA_OPEN_EXISTING;
			break;
		case OPENMODE_WRITE_BIN:
			value = FA_WRITE | FA_OPEN_EXISTING;
			break;
		case OPENMODE_APPEND_BIN:
			value = FA_WRITE | FA_OPEN_EXISTING | FA_OPEN_ALWAYS;
			break;
		case OPENMODE_CREATE_WRITE_BIN:
			value = FA_WRITE | FA_CREATE_ALWAYS;
			break;
		case OPENMODE_RW:
			value = FA_READ | FA_WRITE | FA_OPEN_EXISTING;
			break;
	}
	return value;
}

#endif



static BOOL closeFileHandle(void * fileHandle){
	int result;

#ifdef SUPPORT_VS_FILE
	FILE  *fp;

	if(!fileHandle)return FALSE;
	fp = (FILE *)fileHandle;
	result = fclose(fp);
	return TRUE;
#else
	FIL *file;

	if(!fileHandle){
		MYGUI_File_Unlock();
		return FALSE;
	}
	file = (FIL *)fileHandle;
	result = f_close(file);
	fileio_printf("f_close:%d\r\n",  result);
	
	MYGUI_File_Unlock();
	return result == FR_OK;
#endif
}

static BOOL readContentByFileHandle(void * fileHandle, void * pBuffer, int iLen){
	int result;

#ifdef SUPPORT_VS_FILE
	FILE  *fp;

	if((!fileHandle) || (!pBuffer))return FALSE;
	
	fp = (FILE *)fileHandle;
	result = fread(pBuffer, iLen, 1, fp);
	return TRUE;
#else
	unsigned int read = 0;
	FIL *file;

	if((!fileHandle) || (!pBuffer))return FALSE;
	file = (FIL *)fileHandle;
	result = f_read(file, pBuffer, iLen, &read);
	fileio_printf("f_read:%d,%d\r\n", result, read);
	return result == FR_OK;
#endif
}

static BOOL writeContentByFileHandle(void * fileHandle, const void * pBuffer, int iLen){
	int result;

#ifdef SUPPORT_VS_FILE
	FILE  *fp;

	if((!fileHandle) || (!pBuffer))return FALSE;
	fp = (FILE *)fileHandle;
	result = fwrite(pBuffer, iLen, 1, fp);
	return TRUE;
#else
	unsigned int write = 0;
	FIL *file;

	if((!fileHandle) || (!pBuffer))return FALSE;

	file = (FIL *)fileHandle;
	result = f_write(file, pBuffer, iLen, &write);
	fileio_printf("f_write:%d,%d,%d\r\n", result, iLen, write);
	return result == FR_OK;
#endif
}

static BOOL seekByFileHandle(void * fileHandle, long lPos){
	int result;
#ifdef SUPPORT_VS_FILE
	FILE  *fp;

	if(!fileHandle)return FALSE;
	fp = (FILE *)fileHandle;
	result = fseek(fp, lPos, SEEK_SET);
	return TRUE;
#else
	FIL *file;

	if(!fileHandle)return FALSE;
	file = (FIL *)fileHandle;
	result = f_lseek(file, lPos);
	fileio_printf("f_lseek:%d,%d\r\n", result, (int)lPos);
	return result == FR_OK;
#endif
}
static int GetFileSizeByFileHandle(void * fileHandle){
	int size;

	if(!fileHandle)return 0;
#ifdef SUPPORT_VS_FILE
	if(fseek((FILE *)fileHandle, 0, SEEK_END) != 0)return 0;
	size = ftell((FILE *)fileHandle);
#else
	size = f_size((FIL *)fileHandle);
	fileio_printf("f_size:%d\r\n", size);
#endif
	return size;
}

static void * getFileHandle(const MYSTRING pFilename, enumOpenMode mode){
	void * handle = NULL;

#ifdef SUPPORT_VS_FILE
	FILE  *fp;

	if(!pFilename)return NULL;
#ifdef SUPPORT_UNICODE
	fp = MyVSFOpen(pFilename, getOpenMode(mode), _SH_DENYNO);
#else
	fp = MyVSFOpen(pFilename, getOpenMode(mode));
#endif
	handle = fp;
#else
	int result;
	FIL *file;

	if(!pFilename)return NULL;
	MYGUI_File_Lock();
	file = getFatFile();
	memset(file, 0, sizeof(FIL));
	result = f_open(file, pFilename, getOpenMode(mode));
	if(result != FR_OK){
		f_close(file);
		if(mode == OPENMODE_CREATE_WRITE_BIN)f_unlink(pFilename);
	}

	fileio_printf("f_open:%d,%d,%d\r\n",  result, mode, getOpenMode(mode));
	if(result != FR_OK){
		MYGUI_File_Unlock();
		return NULL;
	}
	handle = (void *)file;
	if(mode == OPENMODE_APPEND_BIN){
		int size = GetFileSizeByFileHandle(handle);
		if(size > 0){
			seekByFileHandle(handle, size);
		}
	}
#endif
	return handle;
}
int getFileLength(const MYSTRING filename){
	int size;
	void * handle;

	handle = getFileHandle(filename, OPENMODE_READ_BIN);
	if(!handle)return 0;
	size = GetFileSizeByFileHandle(handle);
	if(!closeFileHandle(handle))return 0;
	return size;
}


static MYSTRING GetFileStringByFileHandle(void * fileHandle, MYSTRING pBuffer, int iLen){
	MYSTRING p;

	if((!fileHandle) || (!pBuffer) || (iLen <= 0))return NULL;
#ifdef SUPPORT_VS_FILE
	p = (MYSTRING)fgets((char *)pBuffer, iLen, (FILE *)fileHandle);
#else
	p = f_gets(pBuffer, iLen, (FIL *)fileHandle);
#endif
	return p;
}

BOOL isFileExist(const MYSTRING filename){
	void * handle = getFileHandle(filename, OPENMODE_READ_BIN);

	if(!handle)return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}

static BOOL autoCreateParent(const MYSTRING filename){
	MYCHAR path[MAX_FILENAME_LENGTH];
	MYSTRING p = NULL;
	int i, iLen;

	if(!filename)return FALSE;

	MyStrcpy(path, filename);
	iLen = MyStrlen(path);
	p = path + iLen - 1;
	for(i = 0; i < iLen; i++, p--){
		if((*p == '\\') || (*p == '/')){
			*p = 0;
			if(((*(p - 1)) == ':') || (i == 0))return TRUE;
			if(!autoCreateParent(path))return FALSE;
			return CreateMyDirectory(path);
		}
	}
	return FALSE;
}

BOOL RenameFile(const MYSTRING pOld, const MYSTRING pNew){
	int result;

	if(!pOld || !pNew)return FALSE;
	if(!autoCreateParent(pNew))return FALSE;

#ifdef SUPPORT_VS_FILE
	result = MyVSRename(pOld, pNew);
	return TRUE;
#else
	MYGUI_File_Lock();
	result = f_rename(pOld, pNew);
	MYGUI_File_Unlock();

	fileio_printf("f_rename:%d\r\n", result);
	return result == 0;
#endif
}
BOOL CreateBinFileOfLength(const MYSTRING filename, int iLength, char value){
	char buf[64];
	int iLoop;
	int i, iSize = sizeof(buf);
	void * handle = NULL;

	if(isFileExist(filename))DeleteMyFile(filename);
	if(!autoCreateParent(filename))return FALSE;
	handle = getFileHandle(filename, OPENMODE_CREATE_WRITE_BIN);
	if(!handle)return FALSE;

	memset(buf, value, iSize);
	iLoop = iLength / iSize;
	for(i = 0; i < iLoop; i++){
		if(!writeContentByFileHandle(handle, buf, iSize)){
			closeFileHandle(handle);
			return FALSE;
		}
	}
	if(iLength % sizeof(buf))
		if(!writeContentByFileHandle(handle, buf, iLength % iSize)){
			closeFileHandle(handle);
			return FALSE;
		}

	if(!closeFileHandle(handle))return FALSE;

	return TRUE;
}


BOOL getTextFileContent(const MYSTRING filename, char * pBuffer, int iBufSize){
	int iSize;

	iSize = getFileLength(filename);
	if(!iSize)return FALSE;

	if((!pBuffer) || (iBufSize <= iSize))return FALSE;
	if(!ReadFileContentAtPos(filename, pBuffer, iSize, 0))return FALSE;
	pBuffer[iSize] = 0;
	return TRUE;
}

BOOL CreateEmptyFile(const MYSTRING filename){
	void * handle;

	if(isFileExist(filename))DeleteMyFile(filename);
	if(!autoCreateParent(filename))return FALSE;
	handle = getFileHandle(filename, OPENMODE_CREATE_WRITE_BIN);
	if(!handle)return FALSE;
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}

BOOL AppendContent(const MYSTRING filename, const void * pContent, int iLen){
	void * handle;

	if(iLen <= 0)return FALSE;
	handle = getFileHandle(filename, OPENMODE_APPEND_BIN);
	if(!handle)return FALSE;
	if(!writeContentByFileHandle(handle, pContent, iLen)){
		closeFileHandle(handle);
		return FALSE;
	}
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}


BOOL AppendString(const MYSTRING filename, const char * pString){
	return AppendContent(filename, pString, strlen(pString));
}

BOOL WriteFileContentAtPos(const MYSTRING filename, const void * pContent, int iLen, long lPos){
	void * handle = NULL;

	if(iLen <= 0)return FALSE;
	if(!pContent)return FALSE;
	handle = getFileHandle(filename, OPENMODE_RW);
	if(!handle)return FALSE;
	if(!seekByFileHandle(handle, lPos)){
		closeFileHandle(handle);
		return FALSE;
	}
	if(!writeContentByFileHandle(handle, pContent, iLen)){
		closeFileHandle(handle);
		return FALSE;
	}
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}

BOOL WriteFileContent(const MYSTRING filename, void * pContent, int iLen){
	void * handle = NULL;

	if(iLen <= 0)return FALSE;
	handle = getFileHandle(filename, OPENMODE_WRITE_BIN);
	if(!handle)return FALSE;
	if(!writeContentByFileHandle(handle, pContent, iLen)){
		closeFileHandle(handle);
		return FALSE;
	}
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;

}
BOOL CreateWriteFileContent(const MYSTRING filename, const void * pContent, int iLen){
	void * handle = NULL;

	if(!autoCreateParent(filename))return FALSE;
	handle = getFileHandle(filename, OPENMODE_CREATE_WRITE_BIN);

	if(!handle)return FALSE;
	if(!writeContentByFileHandle(handle, pContent, iLen)){
		closeFileHandle(handle);
		return FALSE;
	}
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;

}

BOOL ReadFileContentAtPos(const MYSTRING filename, void * pContent, int iLen, long lPos){
	void * handle = NULL;

	if(iLen <= 0)return FALSE;

	handle = getFileHandle(filename, OPENMODE_READ_BIN);
	if(!handle)return FALSE;
	if(!seekByFileHandle(handle, lPos)){
		closeFileHandle(handle);
		return FALSE;
	}
	if(!readContentByFileHandle(handle, pContent, iLen)){
		closeFileHandle(handle);
		return FALSE;
	}
	if(!closeFileHandle(handle))return FALSE;
	return TRUE;
}



int getLinesOfTextFile(const MYSTRING filename){
	TCHAR temp[LINE_CONTENT_MAX_CHARACTERS];
	MYSTRING p;
	int iCount = 0;
	void * handle;

	handle = getFileHandle(filename, OPENMODE_READ_TEXT);
	if(!handle)return 0;
	p = GetFileStringByFileHandle(handle, temp, sizeof(temp));
	while(p){
		iCount++;
		p = GetFileStringByFileHandle(handle, temp, sizeof(temp));
	}
	if(!closeFileHandle(handle))return 0;
	return iCount;
}


BOOL CreateMyDirectory(const MYSTRING path){
	int result;
	BOOL bOK = TRUE;

	if(!path)return FALSE;
#ifdef SUPPORT_VS_FILE
	result = MyVSMKDir(path);
	return TRUE;
#else
	MYGUI_File_Lock();
	result = f_mkdir(path);
	MYGUI_File_Unlock();

	fileio_printf("f_mkdir:%d", result);
	if(!(result == FR_OK || result == FR_EXIST || result == FR_DENIED)){
		bOK = FALSE;
		MYGUI_File_Lock();
		f_unlink(path);
		MYGUI_File_Unlock();
	}
	return bOK;
#endif

}

BOOL DeleteMyFile(const MYSTRING filename){
	int result;

#ifdef SUPPORT_VS_FILE
	if(!filename)return FALSE;
	result = MyVSRemove(filename);
	return TRUE;
#else

	if(!filename)return FALSE;
	MYGUI_File_Lock();
	result = f_unlink(filename);
	MYGUI_File_Unlock();

	fileio_printf("f_unlink:%d",  result);
	return (result == FR_OK) || (result == FR_NO_FILE);
#endif
	
}

BOOL ClearDisk(void){
	MYGUI_File_Lock();
	deleteTree(getRootDir());
	MYGUI_File_Unlock();
	return TRUE;
}
BOOL FormatFileSystem(char * pBuf, int iBufSize){
#ifndef SUPPORT_VS_FILE
	int result;
	const MYSTRING pDisk = getFatDisk();

	if(!pBuf)return FALSE;
	fileio_printf("FormatFileSystem:%d\r\n", iBufSize);
	system_disable_dog();
	MYGUI_File_Lock();
	result = f_mkfs(pDisk, FM_FAT, 0, pBuf, iBufSize);
	MYGUI_File_Unlock();
	system_feed_dog();
	fileio_printf("f_mkfs:%d\r\n", result);
	return result == FR_OK;
#else
	return TRUE;
#endif

}
BOOL MountFileSystem(BOOL bMount){
#ifndef SUPPORT_VS_FILE
	int result;
	FATFS * pFat = bMount ? getFatFS() : NULL;
	const MYSTRING pDisk = getFatDisk();
	int iLoop = 0;

	fileio_printf("MountFileSystem:%d\r\n", bMount);

  //  if(!xFileQueue)xFileQueue = xSemaphoreCreateMutex();
	
	do{
		result = f_mount(pFat, pDisk, 1);
		iLoop++;
	}while((result != FR_OK) && (iLoop < 3));

	fileio_printf("f_mount:%d,%d\r\n", result, bMount);
	return result == FR_OK;
#else
	return TRUE;
#endif
}

BOOL deleteTree(const MYSTRING path)  
{  
#ifndef SUPPORT_VS_FILE
	int res;  
	DIR   dir;    
	FILINFO *pFileInfo = getFatFileInfo();
	MYCHAR file[_MAX_LFN + 1] = {0};  

	if(!path)return TRUE;
	 
	res = f_opendir(&dir, path);  
	  
	while((res == FR_OK) && (FR_OK == f_readdir(&dir, pFileInfo))) 	{  
	    if(0 == MyStrlen(pFileInfo->fname))          break; 
	    if(0 == MyStrcmp(pFileInfo->fname, _T(".")))     continue; 
	    if(0 == MyStrcmp(pFileInfo->fname, _T("..")))    continue;  
	      
	    memset(file, 0, sizeof(file));
	    MyStrcpy(file, path);
	    MyStrcat(file, _T("/"));
	    MyStrcat(file, pFileInfo->fname);
		
//	    MySprintf(file, sizeof(file), _T("%s/%s"), path, fno.fname);   

	    if (pFileInfo->fattrib & AM_DIR)res = deleteTree(file);  
	    else  res = f_unlink(file);  
	}  
	if(res == FR_OK)    res = f_unlink(path);
	res = f_closedir(&dir);  	
	return res == FR_OK;  
#else
	WIN32_FIND_DATA  ffd;
	HANDLE hFind;
	BOOL ret = TRUE;
	MYCHAR filename[MAX_FILENAME_LENGTH + 1] = {0};  

	if(!path)return FALSE;
	MyStrcpy(filename, path);
	MyStrcat(filename, _T("\\*.*"));
	hFind = FindFirstFile(filename, &ffd);
	if(hFind == INVALID_HANDLE_VALUE)return FALSE;
	do
	{
		    if(0 == MyStrlen(ffd.cFileName))          break; 
		    if(0 == MyStrcmp(ffd.cFileName, _T(".")))     continue; 
		    if(0 == MyStrcmp(ffd.cFileName, _T("..")))    continue;  

	    memset(filename, 0, sizeof(filename));
	    MyStrcpy(filename, path);
	    MyStrcat(filename, _T("\\"));
	    MyStrcat(filename, ffd.cFileName);		
//	    MySprintf(filename, sizeof(filename), _T("%s\\%s"), path, ffd.cFileName);

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			ret = deleteTree(filename);
			MyVSRMDir(filename);
		}
	    else  ret = DeleteMyFile(filename);

	}while ((FindNextFile(hFind, &ffd) != 0) && ret);

	return ret;
#endif
}  



BOOL getFilenameBufferOfPath(const MYSTRING pPath, MYSTRING pBuf, int *pBufLen){
	int iLen, iTotalLen = 0;
	int iBufLen;
	MYCHAR filename[MAX_FILENAME_LENGTH];
	int iCount = 0;

#ifdef SUPPORT_VS_FILE
	WIN32_FIND_DATA  ffd;
	HANDLE hFind;

	if(!pPath || !pBuf || !pBufLen)return FALSE;
	iBufLen = *pBufLen;
	*pBufLen = 0;
	MyStrcpy(filename, pPath);
	MyStrcat(filename, _T("*.*"));
	hFind = FindFirstFile(filename, &ffd);
	if(hFind == INVALID_HANDLE_VALUE)return FALSE;
	do
	{
		if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
			iLen = MyStrlen(ffd.cFileName) + 1;
			if((iTotalLen + iLen) > iBufLen)break;
			MyStrcpy(pBuf + iTotalLen, ffd.cFileName);
			iTotalLen += iLen;
	   }
	}while (FindNextFile(hFind, &ffd) != 0);
	*pBufLen = iTotalLen;
	FindClose(hFind);
#else
	FRESULT res;
	DIR dir;
	FILINFO *pFileInfo = getFatFileInfo();

	if(!pPath || !pBuf || !pBufLen)return FALSE;
	iBufLen = *pBufLen;
	*pBufLen = 0;

	MyStrcpy(filename, pPath);
	iLen = MyStrlen(filename);
	if(filename[iLen - 1] == '/')filename[iLen - 1] = 0;
//	fileio_printf("f_opendir:%s\r\n", filename);
	res = f_opendir(&dir, filename);
	fileio_printf("f_opendir:%d\r\n", res);
	if(res != FR_OK)return FALSE;

    for (;;) {
        res = f_readdir(&dir, pFileInfo);                   
        if((res != FR_OK) || (pFileInfo->fname[0] == 0))break;
        if(!(pFileInfo->fattrib & AM_DIR)){
		iLen = MyStrlen(pFileInfo->fname) + 1;
		if((iTotalLen + iLen) * sizeof(MYCHAR) > (unsigned int)iBufLen)break;
		MyStrcpy(pBuf + iTotalLen, pFileInfo->fname);
		iTotalLen += iLen;
		iCount++;
        }
    }
 	*pBufLen = iTotalLen;
   res = f_closedir(&dir);
#endif
	fileio_printf("getFilenameBufferOfPath:%d,%d\r\n", iTotalLen, iCount);
	return TRUE;
}

int getFilenameListFromBuffer(MYSTRING pBuf, int iBufLen, MYSTRING*ppName, int iCount){
	int i = 0;
	int iLen = 0;
	MYSTRING p = pBuf;
	int iTempLen;

	if(!pBuf || !ppName)return 0;
	while((iLen < iBufLen) && (i < iCount)){
		*ppName++ = p;
		iTempLen = MyStrlen(p) + 1;
		p += iTempLen;
		iLen += iTempLen;
		i++;
	}
	fileio_printf("getFilenameListFromBuffer:%d\r\n", i);
	return i;
}
#endif
