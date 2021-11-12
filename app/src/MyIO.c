#include <string.h>
#include <stdarg.h>
#include "boards.h"
#include "nrf_delay.h"

#include "DriverInterface.h"
#include "MyNotifyQueue.h"
#include "MySPIS.h"
#include "myIO.h"

#define FIREWARE_VERSION "BC_V10"
static U8 static_bufReceive[HUB_MAX_MOSI_SIZE];
static U8 static_bufSend[HUB_MAX_MISO_SIZE];

static int static_iSportType = 0;
static int static_iWearPos = 0;
static int static_iSilentTime = 0;
static int static_iMovingEnergy = 0;
static int static_iUpgradeLength = 0;
static int static_iUpgradeCRC = 0;
static BOOL static_arrayOpen[HUB_SENSOR_MAX];
static BOOL static_arrayNotify[HUB_NOTIFY_MAX];

static void myio_set_read_reg(U8 value){
	static_bufSend[0] = value;
}
U8 * myio_get_buf_receive(void){
	return static_bufReceive;
}
U8 * myio_get_buf_send(void){
	return static_bufSend;
}
static U8 * myio_get_buf_send_data(void){
	return static_bufSend + 1;
//	return static_bufSend;
}

int myio_get_buf_send_size(void){
	return sizeof(static_bufSend);
}
int myio_get_buf_receive_size(void){
	return sizeof(static_bufReceive);
}
void myio_init(void){
	static_iSportType = 0;
	static_iWearPos = 0;
	static_iSilentTime = 0;
	static_iMovingEnergy = 0;
	static_iUpgradeLength = 0;
	static_iUpgradeCRC = 0;	
	memset(static_bufReceive, 0, sizeof(static_bufReceive));
	memset(static_bufSend, 0, sizeof(static_bufSend));
	memset(static_arrayOpen, 0, sizeof(static_arrayOpen));
	memset(static_arrayNotify, 0, sizeof(static_arrayNotify));
	static_arrayNotify[HUB_NOTIFY_LOG] = TRUE;
	static_arrayNotify[HUB_NOTIFY_COMPASS_CALIBRATION] = TRUE;
}
BOOL myio_is_sensor_open(int sensor){
	if(sensor < 0 || sensor >= HUB_SENSOR_MAX)return FALSE;
	return static_arrayOpen[sensor];
}
void myio_set_sensor_open(int sensor, BOOL value){
	if(sensor < 0 || sensor >= HUB_SENSOR_MAX)return;
	static_arrayOpen[sensor] = value;
}
BOOL myio_is_notify(int type){
	if(type < 0 || type >= HUB_NOTIFY_MAX)return FALSE;
	return static_arrayNotify[type];
}
void myio_set_notify(int type, BOOL value){
	if(type < 0 || type >= HUB_NOTIFY_MAX)return;
	static_arrayNotify[type] = value;
}
int myio_get_sport_type(void){
	return static_iSportType;
}
void myio_set_sport_type(int value){
	static_iSportType = value;
}
int myio_get_wearpos(void){
	return static_iWearPos;
}
void myio_set_wearpos(int value){
	static_iWearPos = value;
}
int myio_get_silent_time(void){
	return static_iSilentTime;
}
void myio_set_silent_time(int value){
	static_iSilentTime = value;
}
int myio_get_moving_energy(void){
	return static_iMovingEnergy;
}
void myio_set_moving_energy(int value){
	static_iMovingEnergy = value;
}
int myio_get_upgrade_length(void){
	return static_iUpgradeLength;
}
void myio_set_upgrade_length(int value){
	static_iUpgradeLength = value;
}
int myio_get_upgrade_CRC(void){
	return static_iUpgradeCRC;
}
void myio_set_upgrade_CRC(int value){
	static_iUpgradeCRC = value;
}
static void myio_respone_int32(int value){
	U8 * pSend = myio_get_buf_send_data();
	*((int *)pSend) = value;
}
static void myio_respone_success(void){
	myio_respone_int32(TRUE);
}
static void myio_on_version(void){
	U8 * pSend = myio_get_buf_send_data();

	strcpy((char *)pSend, FIREWARE_VERSION);
}
static void myio_on_notify(void){
	U8 * pSend = myio_get_buf_send_data();
	char * pData = my_notify_queue_read();
	
	if(!pData)return;
	memcpy(pSend, pData, myio_get_buf_send_size() - 1);
}
static void myio_on_sensor_close(int sensor){
	myio_set_sensor_open(sensor, FALSE);
	switch(sensor){
		case HUB_SENSOR_GSENSOR:
			break;
		case HUB_SENSOR_COMPASS:
			break;
		case HUB_SENSOR_TEMPERATURE:
		case HUB_SENSOR_PRESSURE:
			break;
		case HUB_SENSOR_GYRO:
			break;
	}
}
static void myio_on_sensor_open(int sensor){
	myio_set_sensor_open(sensor, TRUE);
	switch(sensor){
		case HUB_SENSOR_GSENSOR:
			break;
		case HUB_SENSOR_COMPASS:
			break;
		case HUB_SENSOR_TEMPERATURE:
		case HUB_SENSOR_PRESSURE:
			break;
		case HUB_SENSOR_GYRO:
			break;
	}
}
static int myio_get_sensor_notify_type(int sensor, int type){
	int notify = -1;
	switch(sensor){
		case HUB_SENSOR_GSENSOR:
			switch(type){
				case GSENSOR_NOTIFY_RAW:
					notify = HUB_NOTIFY_GSENSOR_RAW;
					break;
				case GSENSOR_NOTIFY_STEP:
					notify = HUB_NOTIFY_GSENSOR_STEP;
					break;
				case GSENSOR_NOTIFY_SILENT:
					notify = HUB_NOTIFY_GSENSOR_SILENT;
					break;
				case GSENSOR_NOTIFY_GESTURE:
					notify = HUB_NOTIFY_GSENSOR_GESTURE;
					break;
			}
			break;
		case HUB_SENSOR_COMPASS:
			switch(type){
				case COMPASS_NOTIFY_ANGLE:
					notify = HUB_NOTIFY_COMPASS_ANGLE;
					break;
				case COMPASS_NOTIFY_RAW:
					notify = HUB_NOTIFY_COMPASS_RAW;
					break;
			}
			
			break;
		case HUB_SENSOR_TEMPERATURE:
			notify = HUB_NOTIFY_TEMPERATURE;
			break;
		case HUB_SENSOR_PRESSURE:
			notify = HUB_NOTIFY_PRESSURE;
			break;
		case HUB_SENSOR_GYRO:
			notify = HUB_NOTIFY_GYRO_RAW;
			break;
	}
	return notify;
}
static void myio_on_sensor_open_notify(int sensor, int type){
	int notify = myio_get_sensor_notify_type(sensor, type);
	if(notify >= 0)myio_set_notify(notify, TRUE);
}
static void myio_on_sensor_close_notify(int sensor, int type){
	int notify = myio_get_sensor_notify_type(sensor, type);
	if(notify >= 0)myio_set_notify(notify, FALSE);
}
static void myio_on_sensor_getdata(int sensor, int type){
	switch(sensor){
		case HUB_SENSOR_GSENSOR:
			switch(type){
				case GSENSOR_GETDATA_STEP:
					break;
				case GSENSOR_GETDATA_SILENT:
					break;
			}
			break;
		case HUB_SENSOR_COMPASS:
			break;
		case HUB_SENSOR_TEMPERATURE:
			break;
		case HUB_SENSOR_PRESSURE:
			break;
	}
}

static void myio_on_sensor_request(int value){
	U8 sensor = value & 0xFF;
	U8 cmd = (value &0x0000FF00) >> 8;
	U8 para1 = (value &0x00FF0000) >> 16;

	switch(cmd){
		case SENSOR_OPERATE_CLOSE:
			myio_on_sensor_close(sensor);
			myio_respone_success();
			break;
		case SENSOR_OPERATE_OPEN:
			myio_on_sensor_open(sensor);
			myio_respone_success();
			break;
		case SENSOR_OPERATE_OPEN_NOTIFY:
			if(!myio_is_sensor_open(sensor))myio_on_sensor_open(sensor);
			myio_on_sensor_open_notify(sensor, para1);
			myio_respone_success();
			break;
		case SENSOR_OPERATE_CLOSE_NOTIFY:
			myio_on_sensor_close_notify(sensor, para1);
			myio_respone_success();
			break;
		case SENSOR_OPERATE_GETDATA:
			myio_on_sensor_getdata(sensor, para1);
			break;
	}
}
static void myio_on_request(U8 reg, int value){
	switch(reg){
		case HUB_REG_SENSOR:
			myio_on_sensor_request(value);
			break;
	}	
}

static void myio_on_write(U8 reg, U8 * pValue){
	 int value = 0;
	 
	 if(!pValue)return;
	 value = *((int *)pValue);
	switch(reg){
		case HUB_REG_DATA_READY:
			break;
		case HUB_REG_COMPASS_CALIBRATION:
			break;
		case HUB_REG_WEARPOS:
			myio_set_wearpos(value);
			break;
		case HUB_REG_SPORTTYPE:
			myio_set_sport_type(value);
			break;
		case HUB_REG_SILENT_TIME:
			myio_set_silent_time(value);
			break;
		case HUB_REG_MOVING_ENERGY:
			myio_set_moving_energy(value);
			break;
		case HUB_REG_UPGRADE_LENGTH:
			myio_set_upgrade_length(value);
			break;
		case HUB_REG_UPGRADE_CRC:
			myio_set_upgrade_CRC(value);
			break;
		case HUB_REG_UPGRADE_DATA:
			break;
	}
}
static void myio_on_read(U8 reg){
	switch(reg){
		case HUB_REG_DATA_READY:
			break;
		case HUB_REG_NOTIFY:
			myio_on_notify();
			break;
		case HUB_REG_VERSION:
			myio_on_version();
			break;
		case HUB_REG_WEARPOS:
			myio_respone_int32(myio_get_wearpos());
			break;
		case HUB_REG_SPORTTYPE:
			myio_respone_int32(myio_get_sport_type());
			break;
		case HUB_REG_SILENT_TIME:
			myio_respone_int32(myio_get_silent_time());
			break;
		case HUB_REG_MOVING_ENERGY:
			myio_respone_int32(myio_get_moving_energy());
			break;
	}
}
void myio_process(void){
	U8 * pRecive = myio_get_buf_receive();
	U8 reg = *pRecive;
	U8 * pValue = pRecive + 1;
	BOOL bRead = reg & 0x01;

	reg >>= 1;
//	main_printf("myio_process:%02x,%d\r\n", reg, bRead);
	
	if(bRead){
		memset(myio_get_buf_send(), 0, myio_get_buf_send_size());
		myio_set_read_reg(reg);
		if(reg == HUB_REG_SENSOR)myio_on_request(reg, *((int *)pValue));
		else myio_on_read(reg);
//		if(reg != HUB_REG_DATA_READY)myMemDump(myio_get_buf_send(), 7);
		my_spis_set_data_ready(reg != HUB_REG_DATA_READY);
	}else{
		myio_on_write(reg, pValue);
		my_spis_set_data_ready(FALSE);
	}
	my_spis_refresh();
}
static void my_notify_queue_prewrite(char * pBuffer){
	BOOL bWakeup = FALSE;
	
	if(!pBuffer)return;
	bWakeup = my_notify_queue_is_empty();
	bWakeup = 1;
	my_notify_queue_write(pBuffer);
	if(bWakeup)system_wakeup_main_mcu();
}
static void myio_notify_int32(int notify, int value){
	char temp[HUB_MAX_MISO_SIZE];
	U16 * p = (U16 *)temp;
	
	if(!my_spis_is_inited())return;
	*p++ = notify;
	*p++ = sizeof(int);
	*((int *)p) = value;
	my_notify_queue_prewrite(temp);
}
void myio_notify_string(int notify, char * pString){
	char temp[HUB_MAX_MISO_SIZE];
	U16 * p = (U16 *)temp;
	int iLen = 0;
	
	if(!pString)return;
	if(!my_spis_is_inited())return;
	memset(temp, 0, sizeof(temp));
	iLen = strlen(pString);
	if(iLen >= (HUB_MAX_MISO_SIZE - 5))return;
	*p++ = notify;
	*p++ = iLen + 1;
	strcpy((char *)p, pString); 
	my_notify_queue_prewrite(temp);
}

void myio_log_printf(char* strFormat, ...){
	char buffer[128];
	char * p = buffer;
	va_list args;
	
	*p = 0;
	if(strFormat){
		va_start (args, strFormat);
		vsprintf (p, strFormat, args);
		va_end (args);
	}
	myio_notify_string(HUB_NOTIFY_LOG, buffer);
}

