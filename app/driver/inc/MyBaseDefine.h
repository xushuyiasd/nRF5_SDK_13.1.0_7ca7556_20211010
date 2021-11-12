#ifndef __MY_BASE_DEFINE_20190129_FHQ_H__
#define __MY_BASE_DEFINE_20190129_FHQ_H__

#if defined(__cplusplus)
extern "C" {     
#endif

#define MY_BASE_YEAR 2000
#define countof(exp) (sizeof(exp)/sizeof(exp[0]))
#ifndef ABS
#define ABS(x) ((x)>0)?(x):(-(x))
#endif
//#ifndef MIN
//#define MIN(x,y) ((x)<=(y))?(x):(y)
//#endif
//#ifndef MAX
//#define MAX(x,y) ((x)>=(y))?(x):(y)
//#endif

typedef void  (*pfDefaultFunction)(void);
typedef void  (*pfnMessageProcess)(int wPara, int lPara);
#ifndef _T
#define _T(x)       x
#endif

#define MYCHAR char
#define MYSTRING char *

//typedef char MYCHAR;
//typedef char * MYSTRING;

#if defined(__cplusplus)
}
#endif 

#endif //__MY_BASE_DEFINE_20190129_FHQ_H__



