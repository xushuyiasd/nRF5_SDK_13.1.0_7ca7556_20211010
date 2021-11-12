#ifndef __MAIN_TASK_CLOCK_H_20200424__
#define __MAIN_TASK_CLOCK_H_20200424__


#if defined(__cplusplus)
extern "C" {     
#endif

void main_task_set_second_timer(int id, int time);
void main_task_clear_second_timer(int id);
void main_task_clear_all_second_timer(void);
int main_task_get_second_timer(int id);

void restartClockMessage(void);
void onMainTaskClockMessageInit(void);
void onMainTaskSecondChange(void);
void onMainTaskMinuteChange(void);
void onMainTaskUTCHourChange(void);
void onMainTaskLocaleHourChange(void);
void onMainTaskUTCDayChange(void);
void onMainTaskLocaleDayChange(void);
void onMainTaskUTCMonthChange(void);
void onMainTaskLocaleMonthChange(void);
void onMainTaskUTCYearChange(void);
void onMainTaskLocaleYearChange(void);
void onMainTaskZoneChange(void);


#if defined(__cplusplus)
}
#endif 

#endif //__MAIN_TASK_CLOCK_H_20200424__



