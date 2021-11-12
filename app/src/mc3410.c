#include "mc3410.h"
#include "boards.h"
#include "MyI2C.h"
#include "nrf_delay.h"

#include "DriverInterface.h"

#define MC3410_ADDR 0x4C
static void mc3410_write_reg(uint8_t reg, uint8_t value){
	my_i2c_write_int8(MC3410_ADDR, reg, value);
}
static int mc3410_read_reg(uint8_t reg){
	return my_i2c_read_int8(MC3410_ADDR, reg);
}
int mc3410_read_reg_03(void){
	return mc3410_read_reg(0x03);
}
void mc3410_entry_sleep(void){
	mc3410_write_reg(0x07,0x00);	//standby
}
void mc3410_wake_up(void){
	mc3410_write_reg(0x07,0xc3);	//standby
}
void mc3410_set_sensitivity(int sensitivity){
mc3410_write_reg(0x07,0xc3);	//standby
mc3410_write_reg(0x20,0x29);	// 8g
mc3410_write_reg(0x08,0x05);  	// 1024 high
mc3410_write_reg(0x43,sensitivity);  //anymotion 阀值，调节灵敏度 低8位
mc3410_write_reg(0x44,0x00);   //anymotion 阀值，调节灵敏度  高8位 
mc3410_write_reg(0x45,0x08);   //debounce 有多少个大于阀值才会产生中断 from 0x0A，越小越灵敏
mc3410_write_reg(0x09,0x04); //enable ANYMOTION EINT  from 0x0c, 只开anymotion
mc3410_write_reg(0x06,0x44); //enable ANYMOTION EINT  from 0x0c, 只开anymotion
mc3410_write_reg(0x07,0xc1);
	
}
int mc3410_get_id(void){
	return mc3410_read_reg(0x3b);
}
