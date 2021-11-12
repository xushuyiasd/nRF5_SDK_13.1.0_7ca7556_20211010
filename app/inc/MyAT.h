#ifndef __MY_AT_20200720_FHQ_H__
#define __MY_AT_20200720_FHQ_H__

#include "CommonInterface.h"

#if defined(__cplusplus)
extern "C" {     
#endif

#define MAX_AT_LEN 256

typedef enum{
AT_EVENT_NEW_LINE,
AT_EVENT_MAX
}enumATEvent;
void onProcessATCmdLine(char * pCmdLine);


#if defined(__cplusplus)
}
#endif 

#endif //__MY_AT_20200720_FHQ_H__



