#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "DriverInterface.h"
#include "MygSensor.h"

#ifdef SUPPORT_MY_GSENSOR

#if defined(SUPPORT_PLATFORM_MTK)
extern void bsp_sensor_peripheral_init(void);
extern void mc3410_eint_enable(void);
extern void mc3410_eint_disable(void);
extern I8 mc3410_read_accel_xyz(I16 *x,I16 *y, I16 *z);
extern void mcube_int_reg_set_by_mode(U8 sensitivity);
#endif
int trackimo_get_gsensor_sensitivity_middle(void);
int trackimo_get_gsensor_sensitivity_top(void);

void gSensor_driver_init(void){
	gsensor_printf("gSensor_driver_init\r\n");
#if defined(SUPPORT_PLATFORM_MTK)
	bsp_sensor_peripheral_init();
#endif				
}
void gSensor_on(void){
	gsensor_printf("gSensor_on\r\n");
#if defined(SUPPORT_PLATFORM_MTK)
	mc3410_eint_enable();
#endif
}
void gSensor_off(void){
	gsensor_printf("gSensor_off\r\n");
#if defined(SUPPORT_PLATFORM_MTK)
	mc3410_eint_disable();
#endif
}

void gSensor_get_data(I16 * pX, I16 * pY , I16 * pZ){

	if(!pX || !pY || !pZ)return;
#if defined(SUPPORT_PLATFORM_MTK)
        mc3410_read_accel_xyz(pX, pY, pZ);
#endif		 
	gsensor_printf("gSensor_get_data:%d,%d,%d\r\n", *pX, *pY, *pZ);
}
void gSensor_set_sensitivity(int type){
	U8 value = 0x20;
	gsensor_printf("gSensor_set_sensitivity:%d\r\n", value);
#if defined(SUPPORT_PLATFORM_MTK)
	switch(type){
		case GSENSOR_SENSITIVITY_TOP:
			value = trackimo_get_gsensor_sensitivity_top();
			if(value == 0)value = 0x10;
			break;
		case GSENSOR_SENSITIVITY_MIDDLE:
			value = trackimo_get_gsensor_sensitivity_middle();
			if(value == 0)value = 0x20;
			break;
	}
	mcube_int_reg_set_by_mode(value);
#endif
}

#endif
