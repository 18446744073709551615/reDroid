// prohibit the use of built-in wchar_t

#ifndef NO_wchar_t
#define NO_wchar_t

#include <stddef.h>


#define wchar_t struct _do_not_use_wchar_t_on_android___

#ifdef __cplusplus
extern "C" {
#endif

struct _do_not_use_wchar_t_on_android___ { int stuff; };
typedef unsigned short widechar_t;

widechar_t * wcscpy(widechar_t * s1, const widechar_t * s2);
size_t wcslen(const widechar_t * s);
widechar_t * wcsncpy(widechar_t * s1, const widechar_t * s2, size_t n);
char * wcs2csncpy(char * s1, const widechar_t * s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif
