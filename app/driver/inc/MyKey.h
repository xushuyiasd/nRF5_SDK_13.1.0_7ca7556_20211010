#ifndef __MY_KEY_FHQ_20190722_H__
#define __MY_KEY_FHQ_20190722_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif
#ifdef SUPPORT_MY_KEY
#define SUPPORT_DOUBLE_CLICK

#ifdef SUPPORT_DOUBLE_CLICK
#define DOUBLE_CLICK_SPACE_TIME 100
#endif
typedef enum{
MYKEY_POWER,
MYKEY_SOS,
MYKEY_LEFT,
MYKEY_RIGHT,
MYKEY_REAL_MAX = MYKEY_RIGHT,
MYKEY_MAX	
}enumMyKey;
typedef enum{
MYKEYEVENT_UP,
MYKEYEVENT_DOWN,	
MYKEYEVENT_LONGPRESS,
MYKEYEVENT_REPEAT,
MYKEYEVENT_RELEASE,
#ifdef SUPPORT_DOUBLE_CLICK
MYKEYEVENT_DOUBLECLICK,	
#endif
MYKEYEVENT_MAX	
}enumMyKeyEvent;

typedef struct{
BOOL bDown;
BOOL bLongPress;
int iTime;
}structKeyState;

//�����ص�����
typedef pfDefaultFunction pfKeyCB;

/*
	��������:��������¼�
	����˵��:��
	�� �� ֵ:��
	������Ա:����ǿ
*/
void clearMyKeyEvents(void);
/*
	��������:���ð����¼���Ӧ����
	����˵��:key	ָ������
			 event	ָ�������¼�
			 func	��Ӧ����
	�� �� ֵ:�ɹ����
	������Ա:����ǿ
*/
BOOL setMyKeyEvent(int key, int event, pfKeyCB func);
/*
	��������:��ȡ�����¼���Ӧ����
	����˵��:key	ָ������
			 event	ָ�������¼�			 
	�� �� ֵ:����
	������Ա:����ǿ
*/
pfKeyCB getMyKeyEvent(int key, int event);
void my_key_driver_init(void);
int  mykey_process_double_click(int iKey, int iEvent);
void mykey_diagnose(void);
#ifdef KEYEVENT_INT_MODE
void onMyKeyInt(int type, int para);
#else
void on_mykey_control(void);
void clearMyKeyState(void);
#endif
#endif

#if defined(__cplusplus)
}
#endif 

#endif //__MY_KEY_FHQ_20190722_H__



