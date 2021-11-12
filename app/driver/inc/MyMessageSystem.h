#ifndef __MY_MESSAGE_SYSTEM_FHQ_20190722_H__
#define __MY_MESSAGE_SYSTEM_FHQ_20190722_H__

#include "DriverInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif
#ifdef SUPPORT_MY_MESSAGE
typedef enum{
MYTASK_MAIN,
#ifdef SUPPORT_MY_NG
MYTASK_NG,
#endif
MYTASK_MAX
}enumMyTask;

typedef struct{
int from;
int to;
int id;
int wParam;
int lParam;
int time;
}MYMSG;

typedef void (*pfProcessMsg)(MYMSG * pMsg);

/*
	��������:������Ϣ������
	����˵��:task		����
			 process	������
	�� �� ֵ:��
	������Ա:����ǿ
*/
void MyMessageSystemSetProcess(int task, pfProcessMsg process);
int MyMessageSystemGetWaiting(int task);
BOOL MyMessageSystemIsQueueFull(int task);
BOOL MyMessageSystemIsQueueEmpty(int task);

/*
	��������:����Ϣ�ʵݵ�����
	����˵��:from	���ĸ�����
			 to		���ĸ�����
			 id		��Ϣid
			 wParam	32λ����1
			lpParam	ָ�����
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL MyMessageSystemPostMessage(int from, int to, U32 id, U32 wParam, int lParam);
/*
	��������:����Ϣ���͵�����
	����˵��:from	���ĸ�����
			 to		���ĸ�����
			 id		��Ϣid
			 wParam	32λ����1
			lpParam	ָ�����
	�� �� ֵ:�ɹ����
	�ر�˵��:���ޱ�������
	������Ա:����ǿ
*/
BOOL MyMessageSystemSendMessage(int from, int to, U32 id, U32 wParam, int lParam);

void MyMessageSystemInitTask(int iTask, int iMsgs, pfProcessMsg process);
void MyMessageSystemInitTaskWithStartCB(int iTask, int iMsgs, pfProcessMsg process, pfDefaultFunction fun);
#endif
#if defined(__cplusplus)
}
#endif 

#endif //__MY_MESSAGE_SYSTEM_FHQ_20190722_H__



