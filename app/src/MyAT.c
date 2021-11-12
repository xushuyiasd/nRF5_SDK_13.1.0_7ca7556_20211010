#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "DriverInterface.h"
#include "mc3410.h"
#include "MyAT.h"


typedef enum{
AT_CMD_MODE_TESTING,
AT_CMD_MODE_READ,
AT_CMD_MODE_ACTIVE,
AT_CMD_MODE_EXECUTION,
}enumATCmdMode;

typedef enum{
COMMAND_AT,
COMMAND_WORKMODE,
COMMAND_GSENSOR_SENSITIVITY,
COMMAND_GSENSOR_COUNT,
COMMAND_GSENSOR_TIME,
COMMAND_BT_NAME,
COMMAND_BT_ADDR,
COMMAND_DFU,
COMMAND_VERSION,
COMAND_MAX
}enumATCommand;

static const char * static_atCommands[]={
"",
"+WORKMODE",
"+GSENSOR_SENSITIVITY",
"+GSENSOR_COUNT",
"+GSENSOR_TIME",
"+BT_NAME",
"+BT_ADDR",
"+DFU",
"+VERSION",
};
extern char * getTrackimoVersion(void);
static const char * getATCommandString(int command){
	if((command < 0) || (command >= COMAND_MAX))return NULL;
	return static_atCommands[command];
}

static void myat_send_string(const char * pString){
	system_wakeup_mcu();
	uart_send_cmd_string(pString);
}
static void sendATOK(void){
	myat_send_string("OK");
}
static void sendATError(void){
	myat_send_string("ERROR");
}
static void onSetWorkmode(int value){
	if((value >= 0) && (value < SYSTEM_WORKMODE_MAX)){
		setSystemWorkMode(value);
		sendATOK();
		system_enable_gsensor(!value);
	}else{
		sendATError();	
	}
}
static void onSetGSensorSensitivity(int value){
	if(value <= 0){
		sendATError();	
	}else{
		setGSensorSensitivity(value);
		mc3410_set_sensitivity(value);
		my_nvram_save_settings();
		sendATOK();
	}
}
static void onSetGSensorCount(int value){
	if(value <= 0){
		sendATError();	
	}else{
		setGSensorCount(value);
		my_nvram_save_settings();
		sendATOK();
	}
}
static void onSetGSensorTime(int value){
	if((value <= 0) || (value > TRACKIMO_GSENSOR_SILENT_TIME)){
		sendATError();	
	}else{
		setGSensorTime(value);
		my_nvram_save_settings();
		sendATOK();
	}
}
static void onSetBTName(char * pName){
	BOOL bOK = FALSE;
	
	if(pName)bOK = strlen(pName) < MAX_BT_NAME;
	if(bOK){
		strcpy(getBTName(), pName);
		my_nvram_save_settings();
		refreshBTName();
		sendATOK();	
	}else{
		sendATError();
	}
}
static void onATRead(int cmd, const char * pValue){
	char temp[MAX_AT_LEN];
	
	if(!pValue)return;
	sprintf(temp, "%s: %s", getATCommandString(cmd), pValue);
	myat_send_string(temp);
	sendATOK();
}
static void onATReadByIntValue(int cmd, int value){
	char temp[20];
	
	sprintf(temp, "%d", value);
	onATRead(cmd, temp);
}

static BOOL getCommandOfBuf(char * pBuf, int * pCmd, int * pMode, char ** ppValue){
	char * p;
	int i;
	int iCount = countof(static_atCommands);
	int iLen = 0;
	char * pCmdString = NULL;
	BOOL bOK = FALSE;

	if(!pBuf || !pCmd || !pMode || !ppValue)return FALSE;
	if(iCount > COMAND_MAX)iCount = COMAND_MAX;
	*pCmd = -1;
	*pMode = -1;
	*ppValue = NULL;
	
	if(strncmp(pBuf, "AT", 2) != 0)return FALSE;
	p = pBuf + 2;
	
	if(!(*p)){
		*pCmd = COMMAND_AT;
		*pMode = AT_CMD_MODE_ACTIVE;
		return TRUE;
	}
	for(i = 1; i < iCount; i++){
		pCmdString = (char *)getATCommandString(i);
		iLen = strlen(pCmdString);
		if(strncmp(p, pCmdString, iLen) == 0){
			p += iLen;
			bOK = TRUE;
			*pCmd = i;
			switch(*p){
				case '?':
					*pMode = AT_CMD_MODE_READ;
					break;
				case 0:
					*pMode = AT_CMD_MODE_ACTIVE;
					break;
				case '=':
					if(*(p + 1) == '?')*pMode = AT_CMD_MODE_TESTING;
					else{
						*pMode = AT_CMD_MODE_EXECUTION;
						*ppValue = p + 1;
					}
					break;
				default:
					bOK = FALSE;
					break;
			}
			if(bOK)return TRUE;
		}
	}
	return FALSE;
}


static void onProcessATTesting(int cmd){
	sendATError();	
}
static void onProcessATRead(int cmd){
	switch(cmd){
		case COMMAND_WORKMODE:
			onATReadByIntValue(cmd, getSystemWorkMode());
			break;
		case COMMAND_GSENSOR_SENSITIVITY:
			onATReadByIntValue(cmd, getGSensorSensitivity());
			break;
		case COMMAND_GSENSOR_COUNT:
			onATReadByIntValue(cmd, getGSensorCount());
			break;
		case COMMAND_GSENSOR_TIME:
			onATReadByIntValue(cmd, getGSensorTime());
			break;
		case COMMAND_BT_NAME:
			onATRead(cmd, getBTName());
			break;
		case COMMAND_BT_ADDR:
			onATRead(cmd, getBTAddr());
			break;
		case COMMAND_VERSION:
			onATRead(cmd, getTrackimoVersion());
			break;
	}
}
static void onProcessATActive(int cmd){
	switch(cmd){
		case COMMAND_AT:
			sendATOK();
			break;
		case COMMAND_DFU:
			sendATOK();
			EnterDfu();
			break;
		default:
			sendATError();	
			break;
	}
}

static void onProcessATExecute(int cmd, char * pValue){
	int iValue = 0;
	
	if(!pValue)return;
	iValue = atoi(pValue);
	switch(cmd){
		case COMMAND_WORKMODE:
			onSetWorkmode(iValue);
			break;
		case COMMAND_GSENSOR_SENSITIVITY:
			onSetGSensorSensitivity(iValue);
			break;
		case COMMAND_GSENSOR_COUNT:
			onSetGSensorCount(iValue);
			break;
		case COMMAND_GSENSOR_TIME:
			onSetGSensorTime(iValue);
			break;
		case COMMAND_BT_NAME:
			onSetBTName(pValue);
			break;
		default:
			sendATError();	
			break;
	}
}
void onProcessATCmdLine(char * pCmdLine){
	int cmd = 0;
	int mode = 0;
	char *pValue = NULL;
	
	if(!pCmdLine)return;
	if(!getCommandOfBuf(pCmdLine, &cmd, &mode, &pValue)){
		sendATError();
		return;
	}
	switch (mode) {
		case AT_CMD_MODE_TESTING:    // rec: AT+TESTCMD=?
			onProcessATTesting(cmd);
			break;
		case AT_CMD_MODE_READ:    // rec: AT+TESTCMD?
			onProcessATRead(cmd);
			break;
		case AT_CMD_MODE_ACTIVE:  // rec: AT+TESTCMD
			onProcessATActive(cmd);
			break;
		case AT_CMD_MODE_EXECUTION: // rec: AT+TESTCMD=<p1> 
			onProcessATExecute(cmd, pValue);
			break;
	}
}


