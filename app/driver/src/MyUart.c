#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "DriverInterface.h"
#include "MyNotifyQueue.h"
#include "MyAT.h"
#include "MyUart.h"

#ifdef SUPPORT_MY_UART
#include "nordic_common.h"
#include "nrf.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp.h"

#define UART_TX_BUF_SIZE                MAX_AT_LEN    
#define UART_RX_BUF_SIZE                MAX_AT_LEN  

static U8 static_rxbuffer[UART_RX_BUF_SIZE];
static int static_iWriteIndex = 0;
static BOOL static_bWorking = FALSE;

static void uart_event_handle(app_uart_evt_t * p_event){
U8 * p = static_rxbuffer + static_iWriteIndex;

    switch (p_event->evt_type){
        case APP_UART_DATA_READY:
		case APP_UART_DATA:
		app_uart_get((uint8_t *)p);
		if(*p >= 0x7F){
			static_iWriteIndex = 0;
			break;
		}
		if(((*p) == '\r') || ((*p) == '\n') || ((*p) == '\0')){
			*p  = 0;
			if(static_rxbuffer[0]){
				my_notify_queue_write((char *)static_rxbuffer);
				main_task_post_message(MAIN_MSG_AT_EVENT, 0, 0);
			}
			static_iWriteIndex = 0;
		}else{
			static_iWriteIndex = (static_iWriteIndex + 1) % UART_RX_BUF_SIZE;
		}
            break;

        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;
        default:
           break;
    }
}

static void nordic_uart_init(void){
    uint32_t                     err_code;
    const app_uart_comm_params_t comm_params =  {
        .rx_pin_no    = RX_PIN_NUMBER,
        .tx_pin_no    = TX_PIN_NUMBER,
        .rts_pin_no   = RTS_PIN_NUMBER,
        .cts_pin_no   = CTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity   = false,
        .baud_rate    = UART_BAUDRATE_BAUDRATE_Baud115200
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);
    APP_ERROR_CHECK(err_code);
}

void uart_send_data(const U8 * pData, int iLen){
	int i;
	U8 * p = (U8 *)pData;
	
	if(!pData)return;
	for(i = 0; i < iLen; i++, p++){
		while (app_uart_put(*p) != NRF_SUCCESS);
	}
}
void uart_send_string_with_zero(const char * pString){
	if(!pString)return;
	uart_send_data((const U8 *)pString, strlen(pString) + 1);
}
void uart_send_string_without_zero(const char * pString){
	if(!pString)return;
	uart_send_data((const U8 *)pString, strlen(pString));
}
void uart_send_cmd_string(const char * pString){
	uart_send_string_without_zero(pString);
	uart_send_string_without_zero("\n");
}

void uart_driver_init(void){
#if NRF_LOG_ENABLED
	return;
#endif
	nordic_uart_init();
	myprintf("uart_driver_init\r\n");
	static_bWorking = TRUE;
}
void uart_on(void){
#if NRF_LOG_ENABLED
	return;
#endif	
	if(static_bWorking)return;
	uart_driver_init();
}
void uart_off(void){
#if NRF_LOG_ENABLED
	return;
#endif	
	myprintf("uart_off\r\n");
	if(static_bWorking)app_uart_close();
	static_bWorking = FALSE;
//	nrf_gpio_cfg_output(RX_PIN_NUMBER);
//	nrf_gpio_pin_clear(RX_PIN_NUMBER);
	nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
}

void uart_entry_sleep(void){
	uart_printf("uart_entry_sleep\r\n");
}
void uart_exit_sleep(void){
	uart_printf("uart_exit_sleep\r\n");
}
BOOL uart_is_working(void){
	return static_bWorking;
}



#endif
