#include "dlog.h"
#include <unistd.h>
#include <fcntl.h>
#include "re_tools.h"

int isValidPtr(const void*p, int len) {
    if (!p) {
        return 0;
    }
    int ret = 1;
    int nullfd = open("/dev/random", O_WRONLY);
    if (write(nullfd, p, len) < 0) {
        ret = 0;
        /* Not OK */
    }
    close(nullfd);
    return ret;
}
int isValidOrNullPtr(const void*p, int len) {
    return !p||isValidPtr(p, len);
}
int isValidPtru(unsigned long p, int len) {
    return isValidPtr((void*)p, len);
}

// true if a zero-terminated identifier is located at char* s
int isIdentifier(const char* s) {
    if(!isValidPtr(s,0x10)) { return 0; }
    if(!s[0]) { return 0; }
    int i;
    for (i=0; s[i] && i<512; i++) {
        if( i/0x10 && i%0x10 == 0 && !isValidPtr(s,0x10)) { return 0; }
        unsigned char c = s[i];
        if ('0'<=c && c<='9' || 'a'<=c && c <= 'z' || 'A'<=c && c <= 'Z' || '_' == c) {
        } else {
            return 0;
        }
    }
    return !s[i];
}


char* strrealloccat(char* buffer0, char *addition)
{
    char* buffer = realloc(buffer0, (buffer0 ? strlen(buffer0) : 0) + strlen(addition) + sizeof(char));
    if (!buffer) {
        return buffer;
    }
    if(!buffer0) {
        *buffer = 0;
    }
    return strcat(buffer, addition);
}

void crash() {
  LOGD("\n\n====== crash() ======");
  typedef int ifunc();
  ifunc*p = 0;
  (*p)();
}
