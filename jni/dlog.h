// Android logging

#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "~~~~~~", __VA_ARGS__)
#define DLOG(...) __android_log_print(ANDROID_LOG_DEBUG  , "~~~~~~", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "~~~~~~", __VA_ARGS__)
#define ELOG(...) __android_log_print(ANDROID_LOG_ERROR  , "~~~~~~", __VA_ARGS__)
//#define printf printfIMPL

//#define LOGD1(x) __android_log_print(ANDROID_LOG_DEBUG  , "~~~~~~", x)

#ifdef __cplusplus
extern "C" {
#endif

//int printfIMPL(__const char *__restrict, ...);

void log_wsf(const char*pref, const unsigned short*ws);
void log_ws(const unsigned short*ws);

void* pfetch(void*p);
unsigned long ufetch(void*p);


//void test_rdump();

#ifdef __cplusplus
}
#endif

#define log_dumpf2(fmt,addr) log_dumpf(fmt,addr,64,32)
#define log_dumpf2s(fmt,expr) log_dumpf(fmt,(expr),sizeof(*(expr)),32)
#define log_dumpf3s(fmt,expr,num) log_dumpf(fmt,(expr),sizeof(*(expr))*(num),32)
#define log_dumpf3(fmt,addr,len) log_dumpf(fmt,addr,len,32)


