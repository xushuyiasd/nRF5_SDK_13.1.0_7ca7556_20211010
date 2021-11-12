#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "DriverInterface.h"
#include "MyMessageSystem.h"
#include "MyKey.h"
#if defined(SUPPORT_PLATFORM_MTK)
#include "hal.h"
#include "keypad_custom.h"
#elif defined(SUPPORT_PLATFORM_VS)
#include <windows.h>
#endif


#ifdef SUPPORT_MY_KEY
static pfKeyCB static_arrayKeyEventsFunc[MYKEY_MAX][MYKEYEVENT_MAX];
pfKeyCB getMyKeyEvent(int key, int event){
	if((key < 0) || (key >= MYKEY_MAX))return NULL;
	if((event < 0) || (event >= MYKEYEVENT_MAX))return NULL;
	return static_arrayKeyEventsFunc[key][event];
}


void clearMyKeyEvents(void){
	memset(static_arrayKeyEventsFunc, 0, sizeof(static_arrayKeyEventsFunc));
}

BOOL setMyKeyEvent(int key, int event, pfKeyCB func){
	if((key < 0) || (key >= MYKEY_MAX))return FALSE;
	if((event < 0) || (event >= MYKEYEVENT_MAX))return FALSE;
	static_arrayKeyEventsFunc[key][event] = func;
	return TRUE;
}
#ifdef SUPPORT_DOUBLE_CLICK
static int static_iLastKeyUpTime = 0;
static int static_iLastKey = -1;
static BOOL static_bDoubleClickReady = FALSE;

static void mykey_double_click_reset(void){
	static_iLastKey = -1;
	static_bDoubleClickReady = FALSE;
	static_iLastKeyUpTime = 0;
}
static void mykey_delay_post_keyup(void){
	if(static_iLastKey < 0)return;
	main_task_post_message(MAIN_MSG_KEY_EVENT, static_iLastKey, MYKEYEVENT_UP);
	static_iLastKey = -1;
}
int  mykey_process_double_click(int iKey, int iEvent){
	int iOldKey = static_iLastKey;
	BOOL bReset = FALSE;
	int iNewEvent = iEvent;
	
	if((iEvent == MYKEYEVENT_UP) || (iEvent == MYKEYEVENT_RELEASE)){
		bReset = TRUE;
		if(static_bDoubleClickReady){
			iNewEvent = MYKEYEVENT_DOUBLECLICK;
		}else if(iEvent == MYKEYEVENT_UP){
			bReset = FALSE;
			static_iLastKey = iKey;
			static_iLastKeyUpTime =  getMySystemTimeTick();
			static_bDoubleClickReady = FALSE;
			iNewEvent = -1;
			startMyTimerFromISR(TIMER_ID_KEY, DOUBLE_CLICK_SPACE_TIME, mykey_delay_post_keyup);
		}
		if(bReset)mykey_double_click_reset();
		return iNewEvent;
	}
	if(iOldKey !=  iKey){
		if(iEvent == MYKEYEVENT_DOWN)mykey_delay_post_keyup();
	}else{
		if(iEvent == MYKEYEVENT_DOWN){
			if((getMySystemTimeTick() - static_iLastKeyUpTime) <= DOUBLE_CLICK_SPACE_TIME){
				stopMyTimerFromISR(TIMER_ID_KEY);
				static_bDoubleClickReady = TRUE;
			}else{
				static_bDoubleClickReady = FALSE;
			}
		}
		if(static_bDoubleClickReady)	iNewEvent = -1;
	}
	return iNewEvent;
}
#endif

#if defined(SUPPORT_PLATFORM_VS)

static const int static_arrayWKey[]={
'1',	
'2',
'3',
'4',
'5',
};
static int get_mykey_by_io_state(void){
	int key = -1;
	int i;
	int iCount = sizeof(static_arrayWKey) / sizeof(static_arrayWKey[0]);
	if(iCount > MYKEY_REAL_MAX) iCount = MYKEY_REAL_MAX; 

	for(i = 0; i < iCount; i++){
		if (GetKeyState(static_arrayWKey[i]) < 0) {
			key = i;
			break;
		}
	}
	return key;
}
void my_key_driver_init(void){
}
void mykey_diagnose(void){
}
#elif defined(SUPPORT_PLATFORM_MTK)
static BOOL static_bLongPress = FALSE;
static int getMyIntKeyEvent(int state){
	int iEvent = -1;

	switch(state){
		case  0:  //RELEASE
			if(static_bLongPress)iEvent = MYKEYEVENT_RELEASE;
			else iEvent = MYKEYEVENT_UP;
			static_bLongPress = FALSE;
			break;
		case  1:  //PRESS
			static_bLongPress = FALSE;
			iEvent = MYKEYEVENT_DOWN;
			break;
		case  2:  //LONG_PRESS
			static_bLongPress = TRUE;
			iEvent = MYKEYEVENT_LONGPRESS;
			break;
		case  3:  //REPEAT
			iEvent = MYKEYEVENT_REPEAT;
			break;
	}
	return iEvent;
}
static int getMyKeyValue(int hk){
	int key = -1;

	switch(hk){
		case 254:
			key = MYKEY_POWER;
			break;
		case 17:
			key = MYKEY_LEFT;
			break;
		case 18:
			key = MYKEY_RIGHT;
			break;
		case 13:
			key = MYKEY_SOS;
			break;
	}
	return key;
}
static void keypad_handler(void){
	hal_keypad_event_t          keypad_event;
	int iEvent = -1;
	int iKey = -1;

  	hal_keypad_get_key(&keypad_event);
	iKey = getMyKeyValue(keypad_event.key_data);
	iEvent = getMyIntKeyEvent(keypad_event.state);

	key_printf("keypad_handler:%d,%d\r\n", (int)keypad_event.key_data, keypad_event.state);
#ifdef SUPPORT_DOUBLE_CLICK
	iEvent = mykey_process_double_click(iKey, iEvent);
	if(iEvent < 0)return;
#endif
	main_task_post_message(MAIN_MSG_KEY_EVENT, iKey, iEvent);

}
static void powerkey_handler(void){
	hal_keypad_powerkey_event_t powekey_event;
	int iEvent = -1;
	
        hal_keypad_powerkey_get_key(&powekey_event);
	iEvent = getMyIntKeyEvent(powekey_event.state);
#ifdef SUPPORT_DOUBLE_CLICK
	iEvent = mykey_process_double_click(MYKEY_POWER, iEvent);
	if(iEvent < 0)return;
#endif
	
	key_printf("powerkey_handler:%d,%d\r\n", (int)MYKEY_POWER, powekey_event.state);
	main_task_post_message(MAIN_MSG_KEY_EVENT, MYKEY_POWER, iEvent);
}
void mykey_diagnose(void){
	hal_gpio_data_t ret1 = 0, ret2 = 0, ret3 = 0;
//col2	
	hal_gpio_init(HAL_GPIO_18);
	hal_pinmux_set_function(HAL_GPIO_18, HAL_GPIO_18_GPIO18); 
	hal_gpio_set_direction(HAL_GPIO_18, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_get_input(HAL_GPIO_18, &ret1);

//col0
	hal_gpio_init(HAL_GPIO_20);
	hal_pinmux_set_function(HAL_GPIO_20, HAL_GPIO_20_GPIO20); 
	hal_gpio_set_direction(HAL_GPIO_20, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_get_input(HAL_GPIO_20, &ret2);
//row1
	hal_gpio_init(HAL_GPIO_22);
	hal_pinmux_set_function(HAL_GPIO_22, HAL_GPIO_22_GPIO22); 
	hal_gpio_set_direction(HAL_GPIO_22, HAL_GPIO_DIRECTION_INPUT);
	hal_gpio_get_input(HAL_GPIO_22, &ret3);

	printf("row1:%d,colo:%d,col2:%d\r\n", ret3, ret2, ret1);
	
}
void my_key_driver_init(void){
	int ret = 0;

	if (!keypad_custom_init())return;

	ret = hal_keypad_register_callback((hal_keypad_callback_t)keypad_handler, NULL);
	key_printf("hal_keypad_register_callback ret=%d...\r\n",ret);

	ret = hal_keypad_enable();
	key_printf("hal_keypad_enable ret=%d...\r\n",ret);

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
	    ret = hal_keypad_powerkey_register_callback((hal_powerkey_callback_t)powerkey_handler, NULL);
#endif
}
#endif
#endif
