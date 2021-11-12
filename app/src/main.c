#include <stdint.h>
#include <string.h>
 #include <stdarg.h>

#include "nordic_common.h"
#include "nrf.h"
#include "boards.h"
#include "softdevice_handler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "nrf_drv_clock.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf52.h"
#include "system_nrf52.h"

#include "DriverInterface.h"
#include "task_def.h"
void vApplicationIdleHook(void){
}
static uint32_t softdevice_new_event_handler(void){
    main_task_post_message(MAIN_MSG_BLE_EVENT, BLE_EVENT_INTERNAL, 0);
    return NRF_SUCCESS;
}
static void my_softdevice_init(void){
    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, softdevice_new_event_handler);
}
static void log_init(void){
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
}
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}
void my_log_printf(char* strFormat, ...){
	char buffer[128];
	char * p = buffer;
	va_list args;
	
	*p = 0;
	if(strFormat){
		va_start (args, strFormat);
		vsprintf (p, strFormat, args);
		va_end (args);
	}
	if(!uart_is_working())return;
	uart_send_string_without_zero(buffer);
}

#define BOOTLOADER_DFU_START 0xB1
void EnterDfu(void) {
    
    U32       err_code;
    
	myprintf("EnterDfu\r\n");
    err_code = sd_power_gpregret_clr(0, 0xFFFFFFFF); 
    APP_ERROR_CHECK(err_code); 

    err_code = sd_power_gpregret_set(0, 0); 
    APP_ERROR_CHECK(err_code); 

    
    err_code = sd_power_gpregret_set(0, BOOTLOADER_DFU_START); 
    APP_ERROR_CHECK(err_code); 

    NVIC_SystemReset(); 
}

int main(void){
#if NRF_LOG_ENABLED
	log_init();
	NRF_LOG_INFO("xxxx\r\n");
#endif
	ble_on();
	task_def_init();
	task_def_create();
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	vTaskStartScheduler();
	while (1);
}

