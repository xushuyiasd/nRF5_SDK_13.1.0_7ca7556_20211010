#ifndef __MY_PATH_H_20190918_FHQ__
#define __MY_PATH_H_20190918_FHQ__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif
const MYSTRING getRootDir(void);
const MYSTRING getFirmwareUpgradeFile(void);
const MYSTRING getFirmwareUpgradeFlagFile(void);
const MYSTRING getEphemerisFile(void);
const MYSTRING getEPOFile(void);
const MYSTRING getLogFile(void);
const MYSTRING getExceptionFile(void);
const MYSTRING getIniFile(void);
const MYSTRING getSettingFile(void);
const MYSTRING getFactoryTestFile(void);
const MYSTRING getNVDMFile(void);


#if defined(__cplusplus)
}
#endif 

#endif //__MY_PATH_H_20190918_FHQ__



