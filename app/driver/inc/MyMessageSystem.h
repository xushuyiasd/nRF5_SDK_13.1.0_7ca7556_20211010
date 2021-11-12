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
	功能作用:设置消息处理函数
	参数说明:task		任务
			 process	处理函数
	返 回 值:无
	开发人员:付华强
*/
void MyMessageSystemSetProcess(int task, pfProcessMsg process);
int MyMessageSystemGetWaiting(int task);
BOOL MyMessageSystemIsQueueFull(int task);
BOOL MyMessageSystemIsQueueEmpty(int task);

/*
	功能作用:将消息邮递到任务
	参数说明:from	从哪个任务
			 to		到哪个任务
			 id		消息id
			 wParam	32位参数1
			lpParam	指针参数
	返 回 值:成功与否
	开发人员:付华强
*/
BOOL MyMessageSystemPostMessage(int from, int to, U32 id, U32 wParam, int lParam);
/*
	功能作用:将消息发送到任务
	参数说明:from	从哪个任务
			 to		到哪个任务
			 id		消息id
			 wParam	32位参数1
			lpParam	指针参数
	返 回 值:成功与否
	特别说明:仅限本任务内
	开发人员:付华强
*/
BOOL MyMessageSystemSendMessage(int from, int to, U32 id, U32 wParam, int lParam);

void MyMessageSystemInitTask(int iTask, int iMsgs, pfProcessMsg process);
void MyMessageSystemInitTaskWithStartCB(int iTask, int iMsgs, pfProcessMsg process, pfDefaultFunction fun);
#endif
#if defined(__cplusplus)
}
#endif 

#endif //__MY_MESSAGE_SYSTEM_FHQ_20190722_H__



