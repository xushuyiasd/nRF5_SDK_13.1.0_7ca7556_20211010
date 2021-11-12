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
	��������:��λ�ļ������
	����˵��:��
	�� �� ֵ:��
	������Ա:����ǿ
*/
void resetCloseFileError(void);
/*
	��������:��ȡ�ļ��رմ����
	����˵��:��
	�� �� ֵ:����
	������Ա:����ǿ
*/
int getCloseFileError(void);
/*
	��������:��ȡ���̿���
	����˵��:��
	�� �� ֵ:����
	������Ա:����ǿ
*/
int getFileSectorSize(void);
/*
	��������:��ȡ�����ܿռ�
	����˵��:��
	�� �� ֵ:����
	������Ա:����ǿ
*/
int getDiskTotalSize(void);
/*
	��������:��ȡ����ʣ��ռ�
	����˵��:��
	�� �� ֵ:����
	������Ա:����ǿ
*/
int getDiskFreeSize(void);
/*
	��������:�ļ��Ƿ����
	����˵��:filename(in)	�����ļ���
	�� �� ֵ:����
	������Ա:����ǿ
*/
BOOL isFileExist(const MYSTRING filename);
/*
	��������:�����������ļ�,��ָ��ֵ��ָ���������
	����˵��:filename(in)	�����ļ���
			 iLength(in)	�ļ�����
			 value(in)		�������
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL CreateBinFileOfLength(const MYSTRING filename, int iLength, char value);
/*
	��������:��ȡ�ļ�����
	����˵��:filename(in)	�����ļ���
	�� �� ֵ:����
	������Ա:����ǿ
*/
int getFileLength(const MYSTRING filename);
/*
	��������:��ȡ�ı��ļ�����
	����˵��:filename(in)	�����ļ���
			 pBuffer(out)	�ļ�����
			 iBufSize(in)	pBuffer����
	�� �� ֵ:����
	������Ա:����ǿ
*/
BOOL getTextFileContent(const MYSTRING filename, char * pBuffer, int iBufSize);
/*
	��������:��ȡ�ļ�����
	����˵��:filename(in)	�����ļ���
	�� �� ֵ:����
	������Ա:����ǿ
*/
int getLinesOfTextFile(const MYSTRING filename);
/*
	��������:�������ļ�
	����˵��:filename(in)	�����ļ���
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL CreateEmptyFile(const MYSTRING filename);
/*
	��������:���ļ�׷������
	����˵��:filename(in)	�����ļ���
			 pContent(in)	׷������
			 iLen(in)		pContent����
	�� �� ֵ:�ɹ����
	ע������:�ļ�������ڲųɹ�
	������Ա:����ǿ
*/
BOOL AppendContent(const MYSTRING filename, const void * pContent, int iLen);
/*
	��������:���ļ�׷���ַ���
	����˵��:filename(in)	�����ļ���
			 pString(in)	׷���ַ���
	�� �� ֵ:�ɹ����
	ע������:�ļ�������ڲųɹ�
	������Ա:����ǿ
*/
BOOL AppendString(const MYSTRING filename, const char * pString);
/*
	��������:ָ��λ�ö�ȡ�ļ�����
	����˵��:filename(in)	�����ļ���
			 pBuf(out)		�ļ�����
			 iLen(in)		pBuf����
			 lPos(in)		��ʼ����λ��
	�� �� ֵ:�ɹ����
	ע������:�ļ�������ڲųɹ�
	������Ա:����ǿ
*/
BOOL ReadFileContentAtPos(const MYSTRING filename, void * pBuf, int iLen, long lPos);
/*
	��������:ָ��λ��д�ļ�����
	����˵��:filename(in)	�����ļ���
			 pBuf(in)		�ļ�����
			 iLen(in)		pBuf����
			 lPos(in)		��ʼд��λ��
	�� �� ֵ:�ɹ����
	ע������:�ļ�������ڲųɹ�
	������Ա:����ǿ
*/
BOOL WriteFileContentAtPos(const MYSTRING filename, const void * pBuf, int iLen, long lPos);
/*
	��������:�����ļ�����
	����˵��:filename(in)	�����ļ���
			 pContent(in)	�ļ�����
			 iLen(in)		pContent����
	�� �� ֵ:�ɹ����
	ע������:�ļ�������ڲųɹ�
	������Ա:����ǿ
*/
BOOL WriteFileContent(const MYSTRING filename, void * pContent, int iLen);
/*
	��������:�½���д�ļ�����
	����˵��:filename(in)	�����ļ���
			 pContent(in)	�ļ�����
			 iLen(in)		pContent����
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL CreateWriteFileContent(const MYSTRING filename, const void * pContent, int iLen);
/*
	��������:�ļ�������
	����˵��:pOld(in)	���ļ���(����·��)
			 pNew(in)	���ļ���(����·��)
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL RenameFile(const MYSTRING pOld, const MYSTRING pNew);
/*
	��������:��ȡָ��·���µ��ļ�������
	����˵��:pPath(in)		ָ��·��
			 pBuf(out)		�ļ����ַ���(�ַ�����������)
			 pBufLen(in/out) pBuf��С(inʱ) pBufʹ�ó���(outʱ)
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL getFilenameBufferOfPath(const MYSTRING pPath, MYSTRING pBuf, int *pBufLen);
/*
	��������:�����ļ����б�
	����˵��:pBuf(in)		�ļ�������
			 iBufLen(in)	pBuf��С
			 ppName(out)	�ļ���
			 iCount(in)		ppName����
	�� �� ֵ:�����ɹ����ļ�����
	������Ա:����ǿ
*/
int getFilenameListFromBuffer(MYSTRING pBuf, int iBufLen, MYSTRING *ppName, int iCount);
/*
	��������:����Ŀ¼
	����˵��:path(in)	����·��
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL CreateMyDirectory(const MYSTRING path);
/*
	��������:ɾ���ļ�
	����˵��:filename(in)	����·��
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL DeleteMyFile(const MYSTRING filename);
/*
	��������:װ��ж���ļ�ϵͳ
	����˵��:bMount	TRUE=װ�� FALSE=ж��
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL MountFileSystem(BOOL bMount);
/*
	��������:��ʽ������(�ļ�ϵͳ�������ʹ��)
	����˵��:��
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL FormatFileSystem(char * pBuf, int iBufSize);
/*
	��������:��մ��̣�ɾ�������ļ�(����ʽ��)
	����˵��:��
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL ClearDisk(void);
BOOL deleteTree(const MYSTRING path);

#endif

#if defined(__cplusplus)
}
#endif 

#endif //WATCH_FILEIO_H



