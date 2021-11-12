#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "sdk_config.h"
#include "MyI2C.h"
#include "DriverInterface.h"

#define TWI_INSTANCE_ID     0
#define MYI2C_SCL_PIN 14
#define MYI2C_SDA_PIN 15

#define MAX_WRITE_LEN 32

static volatile bool m_xfer_done = false;
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

static const nrf_drv_twi_t * my_i2c_get_handle(void){
	return &m_twi;
}


static void my_i2c_handler(nrf_drv_twi_evt_t const * p_event, void * p_context){
	if(!p_event)return;
	switch (p_event->type) {
	case NRF_DRV_TWI_EVT_DONE:
		m_xfer_done = true;
		break;
	default:
		break;
	}
}

void my_i2c_init (void){
	const nrf_drv_twi_config_t twi_config = {
		.scl                = MYI2C_SCL_PIN,
		.sda                = MYI2C_SDA_PIN,
		.frequency          = NRF_TWI_FREQ_100K,
		.interrupt_priority = APP_IRQ_PRIORITY_HIGH,
		.clear_bus_init     = true
	};
	ret_code_t err_code;
	const nrf_drv_twi_t * pHandle = my_i2c_get_handle();

	err_code = nrf_drv_twi_init(pHandle, &twi_config, my_i2c_handler, NULL);
	APP_ERROR_CHECK(err_code);

	nrf_drv_twi_enable(pHandle);
}

static void my_i2c_read(uint8_t addr, uint8_t * pData, int iLen){
	if((!pData) || (iLen <= 0))return;
    	m_xfer_done = false;
	nrf_drv_twi_rx(my_i2c_get_handle(), addr, pData, iLen);
    	while (m_xfer_done == false);
}
static void my_i2c_write(uint8_t addr, uint8_t * pData, int iLen){
	if((!pData) || (iLen <= 0))return;
    	m_xfer_done = false;
	nrf_drv_twi_tx(my_i2c_get_handle(), addr, pData, iLen, true);
    	while (m_xfer_done == false);
}
void my_i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t * pData, int iLen){
	my_i2c_write(addr, &reg, 1);
	my_i2c_read(addr, pData, iLen);
}
void my_i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t * pData, int iLen){
	uint8_t temp[MAX_WRITE_LEN];
	
	if((iLen >= MAX_WRITE_LEN) || (!pData))return;
	temp[0] = reg;
	memcpy(temp + 1, pData, iLen);
	my_i2c_write(addr, temp, iLen + 1);
}
int my_i2c_read_int8(uint8_t addr, uint8_t reg){
	int8_t value = 0;
	
	my_i2c_read_reg(addr, reg, (uint8_t *)&value, sizeof(value));
	return value;
}
int my_i2c_read_int16(uint8_t addr, uint8_t reg){
	int16_t value = 0;
	
	my_i2c_read_reg(addr, reg, (uint8_t *)&value, sizeof(value));
	return value;
}

int my_i2c_read_int32(uint8_t addr, uint8_t reg){
	int32_t value = 0;
	
	my_i2c_read_reg(addr, reg, (uint8_t *)&value, sizeof(value));
	return value;
}

void my_i2c_write_int8(uint8_t addr, uint8_t reg, uint8_t value){
	my_i2c_write_reg(addr, reg, (uint8_t *)&value, sizeof(value));
}
void my_i2c_write_int16(uint8_t addr, uint8_t reg, uint8_t value){
	my_i2c_write_reg(addr, reg, (uint8_t *)&value, sizeof(value));
}

void my_i2c_write_int32(uint8_t addr, uint8_t reg, uint8_t value){
	my_i2c_write_reg(addr, reg, (uint8_t *)&value, sizeof(value));
}
