#include "MyPath.h"


#ifdef SUPPORT_VS_FILE

#define ROOT_DIR _T("d:\\Temp\\")
#define PATH_EXT _T("\\")
#else
#define ROOT_DIR _T("0:/")
#define PATH_EXT _T("/")
#endif


#define LOGS_DIR _T("Logs")

#define UPGRADE_FILE _T("FOTA.bin")
#define UPGRADE_FLAG_FILE _T("update")
#define EPO_FILE _T("EPO.dat")
#define EPHEMERIS_FILE _T("Ephemeris.dat")

#define LOG_FILE _T("log.txt")
#define INI_FILE _T("Settings.ini")
#define SETTING_FILE _T("Settings.dat")
#define EXCEPTION_FILE _T("Exception.txt")
#define FACTORY_TEST_FILE _T("test")
#define NVDM_FILE _T("nvdm_%s.ini")

const MYSTRING getRootDir(void){
	return ROOT_DIR;
}
const MYSTRING getFirmwareUpgradeFile(void){
	return ROOT_DIR  UPGRADE_FILE;
}
const MYSTRING getFirmwareUpgradeFlagFile(void){
	return ROOT_DIR  UPGRADE_FLAG_FILE;
}
const MYSTRING getEphemerisFile(void){
	return ROOT_DIR  EPHEMERIS_FILE;
}
const MYSTRING getEPOFile(void){
	return ROOT_DIR EPO_FILE;
} 
const MYSTRING getLogFile(void){
	return ROOT_DIR LOG_FILE;
}
const MYSTRING getExceptionFile(void){
	return ROOT_DIR  EXCEPTION_FILE;
}
const MYSTRING getIniFile(void){
	return ROOT_DIR  INI_FILE;
}
const MYSTRING getSettingFile(void){
	return ROOT_DIR  SETTING_FILE;
}
const MYSTRING getFactoryTestFile(void){
	return ROOT_DIR  FACTORY_TEST_FILE;
}


