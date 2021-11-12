#ifndef MY_TIMER_H_20190903
#define MY_TIMER_H_20190903

#include "CommonInterface.h"
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
#ifdef SUPPORT_MY_TIMER

typedef enum{
TIMER_ID_CLOCK,
TIMER_ID_SYSTEM,
TIMER_ID_RR,
TIMER_ID_LED,
TIMER_ID_KEY,
TIMER_ID_BUZZER,
TIMER_ID_WAKEUP,
TIMER_ID_POWERON,
TIMER_MAX
}enumMyTimer;


/*
	��������:��ʼ����ʱ��
	����˵��:��
	�� �� ֵ:��
	������Ա:����ǿ
*/
void initMyTimer(void);
void deinitMyTimer(void);
/*
	��������:������ʱ��
	����˵��:id		��ʱ��id
			 time	��Ҫ�ȴ��Ĵ���ʱ��(����)
			 cb		�ص�����
			 user_data	�ص�ʱ���û�����
	�� �� ֵ:��
	������Ա:����ǿ
*/
void startMyTimer(int id, int time, pfDefaultFunction cb);
void startMyTimerFromISR(int id, int time, pfDefaultFunction cb);
/*
	��������:�رն�ʱ��
	����˵��:id		��ʱ��id
	�� �� ֵ:��
	������Ա:����ǿ
*/
void stopMyTimer(int id);
void stopMyTimerFromISR(int id);
int getWaitTimeForMyTimer(void);
#ifdef SUPPORT_TIMER_EXCUTE_IN_TASK
void startMyTimerInTask(int id, int time, pfDefaultFunction cb, int task);
#define startMyMainTaskTimer(id, time, cb) \
    startMyTimerInTask(id, time, cb, MYTASK_MAIN)
#define stopMyMainTaskTimer stopMyTimer

#define startMyNGTaskTimer(id, time, cb) \
    startMyTimerInTask(id, time, cb, MYTASK_NG)
#define stopMyNGTaskTimer stopMyTimer


#endif

#endif

#if defined(__cplusplus)
}
#endif 

#endif //MY_TIMER_H_20190903



