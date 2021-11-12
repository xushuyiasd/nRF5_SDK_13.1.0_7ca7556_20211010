#ifndef __MY_I2C_FHQ_20210506_H__
#define __MY_I2C_FHQ_20210506_H__


#if defined(__cplusplus)
extern "C" {     
#endif

void my_i2c_init (void);
void my_i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t * pData, int iLen);
void my_i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t * pData, int iLen);


int my_i2c_read_int8(uint8_t addr, uint8_t reg);
int my_i2c_read_int16(uint8_t addr, uint8_t reg);
int my_i2c_read_int32(uint8_t addr, uint8_t reg);
void my_i2c_write_int8(uint8_t addr, uint8_t reg, uint8_t value);
void my_i2c_write_int16(uint8_t addr, uint8_t reg, uint8_t value);
void my_i2c_write_int32(uint8_t addr, uint8_t reg, uint8_t value);
	
#if defined(__cplusplus)
}
#endif 

#endif //__MY_I2C_FHQ_20210506_H__

