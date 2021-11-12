#include <stdio.h>


#include "DriverInterface.h"
#include "FreeRTOS.h"
#include "task.h"
#include "MyNotifyQueue.h"
#include "MyNVRam.h"
#if defined(SUPPORT_PLATFORM_MTK)
#include "hal.h"
#include "battery_management.h"
extern uint8_t main_sleep_handle;
#elif defined(SUPPORT_PLATFORM_NORDIC)
#include "boards.h"
#include "nrf_delay.h"
#include "nrf_drv_gpiote.h"
#include "mc3410.h"
#endif

extern void my_i2c_init (void);
static int static_iLastGSensorIntTime = 0;

#define GSENSOR_CS_PIN 18
#define GSENSOR_INT_PIN 16
#define MCU_POWERON_PIN 12
#define MCU_WAKEUP_PIN 7

#define MCU_STATUS_PIN 20
void system_enable_gsensor(BOOL bEnable){
//	nrf_gpio_pin_write(GSENSOR_CS_PIN, bEnable);
	return;
	if(bEnable)mc3410_wake_up();
	else mc3410_entry_sleep();
}
static void onPoweronTimeout(void){
	nrf_gpio_pin_write(MCU_POWERON_PIN, 0);
}
void system_poweron_mcu(void){
	nrf_gpio_pin_write(MCU_POWERON_PIN, 1);
#ifdef FREERTOS
	startMyTimer(TIMER_ID_POWERON, 10000, onPoweronTimeout);
#else
	nrf_delay_ms(10000);
	nrf_gpio_pin_write(MCU_POWERON_PIN, 0);
#endif
}
static void onWakupTimeout(void){
	nrf_gpio_pin_write(MCU_WAKEUP_PIN, 0);
}
void system_wakeup_mcu(void){
	nrf_gpio_pin_write(MCU_WAKEUP_PIN, 1);
#ifdef FREERTOS
	startMyTimer(TIMER_ID_WAKEUP, 20, onWakupTimeout);
#else
	nrf_delay_ms(20);
	nrf_gpio_pin_write(MCU_WAKEUP_PIN, 0);
#endif
}
static void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
	int time = 0;
	mc3410_read_reg_03();
	if(getSystemWorkMode() != SYSTEM_WORKMODE_WORK)return;
	time = getMySystemTimeTick();
	if((time - static_iLastGSensorIntTime) < 1000)return;
	static_iLastGSensorIntTime = time;
	main_task_post_message(MAIN_MSG_GSENSOR_EVENT, GSENSOR_EVENT_MOVE, 0);
}
static void gsensor_int_init(void){
	ret_code_t err_code;

	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
	in_config.pull = NRF_GPIO_PIN_PULLUP;

	err_code = nrf_drv_gpiote_in_init(GSENSOR_INT_PIN, &in_config, in_pin_handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_gpiote_in_event_enable(GSENSOR_INT_PIN, true);	
}
static void mcu_status_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
	myprintf("mcu_status_pin_handler:%d\r\n", action);
	switch(action){
		case NRF_GPIOTE_POLARITY_LOTOHI:
			uart_on();
			break;
		case NRF_GPIOTE_POLARITY_HITOLO:
			uart_off();
			break;
		case NRF_GPIOTE_POLARITY_TOGGLE:
			if(nrf_gpio_pin_read(MCU_STATUS_PIN)){
				uart_on();
			}else{
				uart_off();
			}
			break;
	}
}
static void mcu_int_init(void){
	ret_code_t err_code;

	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
	in_config.pull = NRF_GPIO_PIN_NOPULL;

	err_code = nrf_drv_gpiote_in_init(MCU_STATUS_PIN, &in_config, mcu_status_pin_handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_gpiote_in_event_enable(MCU_STATUS_PIN, true);

	if(nrf_gpio_pin_read(MCU_STATUS_PIN)){
		uart_on();
	}
}
static void driver_init_other(void){
//	nrf_gpio_cfg_output(GSENSOR_CS_PIN);
//	nrf_gpio_pin_write(GSENSOR_CS_PIN, 1);
#ifdef SUPPORT_MY_NVRAM
	my_nvram_init();
	my_nvram_load_settings();
	if(!isSettingFlagCheckOK()){
		initMySetting();
		my_nvram_save_settings();
		my_nvram_load_settings();
	}	
#endif

	refreshBTName();
	
	nrf_gpio_cfg_input(MCU_STATUS_PIN, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(GSENSOR_INT_PIN, GPIO_PIN_CNF_PULL_Pullup);

	nrf_gpio_cfg_output(MCU_POWERON_PIN);
	nrf_gpio_pin_write(MCU_POWERON_PIN, 0);

	nrf_gpio_cfg_output(MCU_WAKEUP_PIN);
	nrf_gpio_pin_write(MCU_WAKEUP_PIN, 0);

	my_notify_queue_clear();
	my_i2c_init();
	if (!nrf_drv_gpiote_is_init())nrf_drv_gpiote_init();
	gsensor_int_init();
	system_printf("mc3410 id:%d,%d\r\n", mc3410_get_id(), getGSensorSensitivity());
	mc3410_set_sensitivity(getGSensorSensitivity());
#ifndef SUPPORT_VERSION_DEBUG	
	mcu_int_init();
#endif
}
void driver_init(void){
	clock_init();
	#ifdef SUPPORT_MY_TIMER
	initMyTimer();
	#endif
	#ifdef SUPPORT_MY_KEY
	my_key_driver_init();
	#endif	
	#ifdef SUPPORT_MY_BLE
	ble_driver_init();
	#endif
	#ifdef SUPPORT_MY_GPS
	gps_driver_init();
	#endif

	#ifdef SUPPORT_MY_LED
	led_driver_init();
	#endif

	#ifdef SUPPORT_MY_VIBURATOR
	viburator_driver_init();
	#endif
	#ifdef SUPPORT_MY_WIFI
	wifi_driver_init();
	#endif
	#ifdef SUPPORT_MY_UART
#ifdef SUPPORT_VERSION_DEBUG	
	uart_driver_init();
#endif
	#endif	
	#ifdef SUPPORT_MY_GSENSOR
	gSensor_driver_init();
	gSensor_on();
	#endif
#ifdef SUPPORT_MY_NVRAM
	my_nvram_init();
#endif	
#ifdef SUPPORT_WATCH_DOG
{
	hal_wdt_config_t wdt_config;
	wdt_config.mode = HAL_WDT_MODE_RESET;
	wdt_config.seconds = 30;

	hal_wdt_init(&wdt_config);
	hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
}
#endif
	driver_init_other();
}


void system_task_delay(int iMillSecond){
	vTaskDelay(iMillSecond);
}
void system_feed_dog(void){
	system_printf("system_feed_dog\r\n");
	
#ifdef SUPPORT_WATCH_DOG			
	hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
	hal_wdt_feed(HAL_WDT_FEED_MAGIC);
#endif	
}
void system_disable_dog(void){
	system_printf("system_disable_dog\r\n");
	
#ifdef SUPPORT_WATCH_DOG			
	hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
#endif
}
static void rtc_wait_busy(void)
{
    uint32_t count = 0;
    while (count < 0x6EEEEE) {
        if (((*(volatile uint16_t *)0xA21e0000) & (0x40)) == 0) {
            break;
        }
        count++;
    }
}

static void rtc_write_trigger_wait(void)
{
    *(volatile uint16_t *)0xA21e0074 = 0x1;
    rtc_wait_busy();
}
void system_enable_download(void){
	system_printf("system_enable_download\r\n");
	
#if defined(SUPPORT_PLATFORM_MTK)
{
    uint16_t value = 0;
    value = *(volatile uint16_t *)0xA21e0078;
    value &= (~0x100);
    *(volatile uint16_t *)0xA21e0078 = value;
    rtc_write_trigger_wait();
}
#endif
}
void system_disable_download(void){
	system_printf("system_disable_download\r\n");
	
#if defined(SUPPORT_PLATFORM_MTK)
{
    uint16_t value = 0;

    *(volatile uint16_t *)0xA21e000c = 0x0;
    rtc_write_trigger_wait();

    value = *(volatile uint16_t *)0xA21e0078;
    value |= 0x100;
    *(volatile uint16_t *)0xA21e0078 = value;
    rtc_write_trigger_wait();

}
#endif
}
void system_shutdown(void){
#if defined(SUPPORT_PLATFORM_MTK)
    	hal_sleep_manager_enter_power_off_mode();
#endif
}
void system_lock_sleep(void){
	system_printf("system_lock_sleep\r\n");
#if defined(SUPPORT_PLATFORM_MTK)
hal_sleep_manager_lock_sleep(main_sleep_handle);
#endif
}
void system_unlock_sleep(void){
	system_printf("system_unlock_sleep\r\n");
#if defined(SUPPORT_PLATFORM_MTK)
hal_sleep_manager_unlock_sleep(main_sleep_handle);

#endif
}
void system_reset(void){
	system_printf("system_reset\r\n");
	
#if defined(SUPPORT_PLATFORM_MTK)
    hal_wdt_config_t wdt_config;
    
    wdt_config.mode = HAL_WDT_MODE_RESET;
    wdt_config.seconds = 1;
    hal_wdt_init(&wdt_config);
    hal_wdt_software_reset();
    while(1); /* waiting the system reboot */
#endif
}


