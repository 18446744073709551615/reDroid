//#include <stdio.h>
#include <dlog.h>
#include <alloca.h>
#include "nowchart.h"
#include <string.h>
#include "re_tools.h"
#include "re_dump.h"

inline char hdigit(int n){return "0123456789abcdef"[n&0xf];};

#define LEN_LIMIT 32
#define SUBSTITUTE_CHAR '`'

static const char* dumpline(char*dest, int linelen, const char*src, const char*srcend)
{
    if(src>=srcend) {
        return 0;
    }
    int i;
    unsigned long s = (unsigned long)src;
    for(i=0; i<8; i++) {
        dest[i] = hdigit(s>>(28-i*4));
    }
    dest[8] = ' ';
    dest += 9;
    if (isValidPtr(src,linelen/4)) {
      for(i=0; i<linelen/4 ; i++) {
        if(src+i<srcend) {
            dest[i*3] = hdigit(src[i]>>4);
            dest[i*3+1] = hdigit(src[i]);
            dest[i*3+2] = ((i+1+(unsigned long)src)&3)?':':' ';
            dest[linelen/4*3+i] = src[i] >= ' ' && src[i] < 0x7f ? src[i] : SUBSTITUTE_CHAR;
        }else{
            dest[i*3] = dest[i*3+1] = dest[i*3+2] = dest[linelen/4*3+i] = ' ';
        }
      }
    } else {
      strcpy(dest, "***cannot read***");
      i = linelen/4;
    }
    dest[linelen] = 0;
    return src+i;
}

void log_dumpf(const char*fmt,const void*addr,int len,int linelen)
{
#if LEN_LIMIT
    if(len>linelen*LEN_LIMIT) {
        len=linelen*LEN_LIMIT;
    }
#endif
    linelen *= 4;
    static char _buf[4096];
    char*buf = _buf;//(char*)alloca(linelen+1); // alloca() causes the initialization to fail!!!!
    buf[linelen]=0;
    const char*start = (char*)addr;
    const char*cur = start;
    const char*end = start+len;
    while(!!(cur = dumpline(buf,linelen,cur,start+len))){DLOG(fmt,buf);}
}

void log_dump(const void*addr,int len,int linelen)
{
    log_dumpf("%s\n",addr,len,linelen);
}

#if 0
int main()
{
    const char*x="qwertyuiopasdfghjklzxcvbnm1234567890";
    dump(x,16,8);
    dump(x,6,8);
    dump(x,17,8);
}
#endif


void log_wsf(const char*pref, const unsigned short*ws) {
    int len = wcslen(ws)+1;
    char * s = wcs2csncpy((char*)alloca(len*2),ws,len);
    DLOG("%s [%s]",pref,s);
}

void log_ws(const unsigned short*ws) { log_wsf("",ws) ;}

void* pfetch(void*p) {
    return isValidPtr(p,sizeof(void*)) ? *(void**)p : (void*)0;
}
unsigned long ufetch(void*p) {
    return isValidPtr(p,sizeof(unsigned long)) ? *(unsigned long*)p : 0;
}



void _rec_log_rdumpf(const char*fmt,const void*addr, int offset, int len, int level, int levels, int linelen) {
    if(len<0) { DLOG("this is a bug: len=0x%x",len); }
    if(strlen(fmt)<4) { DLOG("this is a bug: fmt=%p [%s]",fmt,fmt); }
//DLOG("_rec_log_rdumpf %p %i(%i)",addr,level,levels);
    addr = (void*)(-2&(long)addr);
//DLOG("addr=%p",addr);
    log_dumpf(fmt,(char*)addr+offset,len,linelen);
//DLOG("^^ log_dumpf([%s], %p, %x, %x)",fmt,addr+offset,len,linelen);
    if(level>=levels) {
    } else if(2&(long)addr) {
        DLOG(fmt,"*** address not aligned ***");
    } else if(isValidPtr((char*)addr+offset,len)) {
        void**p;
//DLOG("p=%p range %p %p",addr,addr-offset,addr-offset+len);
        for(p=(void**)((char*)addr+offset); (void*)p<(char*)addr+offset+len; p++) {
            _rec_log_rdumpf(fmt-2,*p,offset, len, level+1, levels, linelen);
        }
    } else {
        //DLOG("invalid address: %p %i",addr+offset,len);
    }
}
void log_rdumpf(const char*fmt,const void*addr, int offset, int len, int levels, int linelen) {
    DLOG("\n{{ log_rdumpf([%s], addr=%p, off=0x%x, len=0x%x; levels=0x%x, linelen=0x%x)",fmt,addr,offset,len,levels,linelen);
    static char buf[1024];
    char*p = buf;
    int i;
    for(i=0; i<levels*2+2; i+=2) {
        buf[i] = '+';
        buf[i+1] = ' ';
    }
    strcpy(buf+i,fmt);
//log_dumpf("buf: %s",buf,0x40,0x40);
    _rec_log_rdumpf(buf+i,addr,offset,len,0,levels,linelen);
    DLOG("}} log_rdumpf([%s], addr=%p, off=0x%x, len=0x%x; levels=0x%x, linelen=0x%x)\n",fmt,addr,offset,len,levels,linelen);
}

#if 0
int a[] = {1,2,3,4};
int b[] = {5,6,7,8};
int c[] = {11,12,13,14};
int d[] = {15,16,17,18};
int* x[] = {a,b,c,d};
int* y[] = {a+2,b+2,c+2,d+2};

void test_rdump() {
    DLOG("~~~~~~~~ test_rdump() {");
    log_rdumpf("test1: %s",x,0,0x10,2,64);
    log_rdumpf("test2: %s",y+2,-8,0x10,2,64);
    DLOG("}~~~~~~~ test_rdump()");
}


#endif

