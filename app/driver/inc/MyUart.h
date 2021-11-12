#ifndef __MY_UART_FHQ_20200204_H__
#define __MY_UART_FHQ_20200204_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#ifdef SUPPORT_MY_UART

void uart_driver_init(void);
void uart_on(void);
void uart_off(void);
void uart_entry_sleep(void);
void uart_exit_sleep(void);
void uart_send_char(U8 c);
void uart_send_data(const U8 * pData, int iLen);
void uart_send_string_with_zero(const char * pString);
void uart_send_string_without_zero(const char * pString);
void uart_send_cmd_string(const char * pString);
BOOL uart_is_working(void);

#endif

#if defined(__cplusplus)
}
#endif 

#endif //__MY_UART_FHQ_20200204_H__



