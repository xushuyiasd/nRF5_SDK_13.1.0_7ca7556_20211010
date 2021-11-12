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
	功能作用:初始化定时器
	参数说明:无
	返 回 值:无
	开发人员:付华强
*/
void initMyTimer(void);
void deinitMyTimer(void);
/*
	功能作用:开启定时器
	参数说明:id		定时器id
			 time	需要等待的触发时间(毫秒)
			 cb		回调函数
			 user_data	回调时的用户数据
	返 回 值:无
	开发人员:付华强
*/
void startMyTimer(int id, int time, pfDefaultFunction cb);
void startMyTimerFromISR(int id, int time, pfDefaultFunction cb);
/*
	功能作用:关闭定时器
	参数说明:id		定时器id
	返 回 值:无
	开发人员:付华强
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



