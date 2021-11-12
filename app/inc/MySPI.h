#ifndef __MY_SPI_FHQ_20210506_H__
#define __MY_SPI_FHQ_20210506_H__

#include "DriverInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

void my_spi_init(void);
void spi_write(const uint8_t * data, size_t size);
void spi_read(uint8_t * data, size_t size);



#if defined(__cplusplus)
}
#endif 

#endif //__MY_SPI_FHQ_20210506_H__

