#ifndef __MC3410_FHQ_20210506_H__
#define __MC3410_FHQ_20210506_H__

#include "CommonInterface.h"
#if defined(__cplusplus)
extern "C" {     
#endif

void mc3410_set_sensitivity(int sensitivity);
int mc3410_read_reg_03(void);
int mc3410_get_id(void);
void mc3410_entry_sleep(void);
void mc3410_wake_up(void);

	
#if defined(__cplusplus)
}
#endif 

#endif //__MC3410_FHQ_20210506_H__

