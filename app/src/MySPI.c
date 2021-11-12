#include <stdio.h>
#include "boards.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "MySPI.h"

#if SPI_ENABLED

#define MYSPI_INSTANCE 1
#define MYSPI_SCK_PIN 4
#define MYSPI_MISO_PIN 5
#define MYSPI_MOSI_PIN 7
#define MYSPI_SS_PIN 8

static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(MYSPI_INSTANCE);

static const nrf_drv_spi_t * my_spi_get_handle(void){
	return &spi;
}
void my_spi_init(void){
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.sck_pin  = MYSPI_SCK_PIN;
    spi_config.miso_pin = MYSPI_MISO_PIN;
    spi_config.mosi_pin = MYSPI_MOSI_PIN;
    spi_config.ss_pin   = MYSPI_SS_PIN;
    spi_config.frequency   = NRF_DRV_SPI_FREQ_1M;

    nrf_drv_spi_init(my_spi_get_handle(), &spi_config, NULL, NULL);
}

void spi_write(const uint8_t * data, size_t size){
   APP_ERROR_CHECK(nrf_drv_spi_transfer(my_spi_get_handle(), data, size, NULL, 0));
}
void spi_read(uint8_t * data, size_t size){
    APP_ERROR_CHECK(nrf_drv_spi_transfer(my_spi_get_handle(), NULL, 0, data, size));
}
#endif
