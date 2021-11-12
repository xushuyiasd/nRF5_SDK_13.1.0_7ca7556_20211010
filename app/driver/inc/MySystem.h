#ifndef __MY_SYSTEM_FHQ_20191127_H__
#define __MY_SYSTEM_FHQ_20191127_H__

#if defined(__cplusplus)
extern "C" {     
#endif

void driver_init(void);
void main_entry_sleep(void);
void main_4g_exit_sleep(void);
void system_feed_dog(void);
void system_enable_download(void);
void system_disable_download(void);
void system_disable_dog(void);
void system_shutdown(void);
void system_reset(void);
void system_lock_sleep(void);
void system_unlock_sleep(void);
void system_shutdown_with_myng(void);
void system_reboot_with_myng(void);
void system_enable_gsensor(BOOL bEnable);
void system_poweron_mcu(void);
void system_wakeup_mcu(void);


#if defined(__cplusplus)
}
#endif 

#endif	//__MY_SYSTEM_FHQ_20191127_H__

