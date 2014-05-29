#include <stdlib.h>
#include "dlog.h"
#include "re_rtti.h"
#include "re_tools.h"

void _log_o(unsigned long a, char*s) {
    _log_obj((void*)a, s);
}

void _log_obj(void* addr, char* s) {
    if (!isValidPtr(addr,8)) {
        DLOG("%s obj:%p, not a valid address",s,addr);
        return;
    }
    unsigned int* vmtaddr = isValidPtr(addr,4) && 0 == (3 & *(int*)addr) && isValidPtr(*(int**)addr,4) ? *(int**)addr : (void*)0;
    DLOG("%s obj:%p [%x] vmt:%p", s, addr, dlmalloc_usable_size(addr), *(int**)addr);
    log_dumpf("data: %s",addr, 0x40, 0x40);
    if (vmtaddr) {
        if(isValidPtr(vmtaddr-2,0x20)) {
//        log_dumpf("vmt-8: %s",vmtaddr-2, 0x8, 0x40);
//        log_dumpf("-4 =>: %s",*(char**)(vmtaddr-1),0x40,0x40);
//        log_dumpf("-4 =>0: %s",((char***)vmtaddr)[-1][0],0x40,0x40);
//        log_dumpf("-4 =>1: %s",((char***)vmtaddr)[-1][1],0x40,0x40); // class name
//        log_dumpf("-4 =>2: %s",((char***)vmtaddr)[-1][2],0x40,0x40);
//        DLOG("class name: _Z%s",((char***)vmtaddr)[-1][1]);
//        log_dumpf("vmt:   %s",vmtaddr, 0x80, 0x40);
            char* lastType = (void*)0;
            char* classchain = strrealloccat((void*)0, "_Z");
            char**ptypeinfo = ((char***)vmtaddr)[-1];
            for (; isValidPtr(ptypeinfo,4)
                && isValidPtr((char***)ptypeinfo[0]-1,8)
                && isValidPtr(((char***)ptypeinfo[0])[-1],8)
                && isValidPtr(((char***)ptypeinfo[0])[-1][1],0x20)
                 ;  ptypeinfo = !strcmp(lastType=((char***)ptypeinfo[0])[-1][1], "N10__cxxabiv120__si_class_type_infoE") ? (char**)ptypeinfo[2] : 0
                ) {
//            DLOG("tinfo: %p",ptypeinfo);
//            DLOG("class: _Z%s   meta: _Z%s",ptypeinfo[1], ((char***)ptypeinfo[0])[-1][1]);
//            DLOG("meta : _Z%s",((char***)ptypeinfo[0])[-1][1]);
//DLOG("0chain: %p [%s]",classchain,classchain);
                classchain = strrealloccat(strrealloccat(classchain, ptypeinfo[1]), "  _Z");
//DLOG("1chain: %p [%s]",classchain,classchain);
//            classchain = strrealloccat(classchain," _Z");
//DLOG("2chain: %p [%s]",classchain,classchain);
            }
            DLOG("%s inheritance chain: %s",s,classchain);
            if(lastType) { DLOG("%s last seen RTTI type: %s",s,lastType); } else { DLOG("%s last seen RTTI type: %p",s,lastType); }
            if (ptypeinfo) {
                DLOG("%s %p is not a valid pointer to typeinfo",s,ptypeinfo);
            }
            free(classchain);
        } else {
            DLOG("%s %p is not a valid address",s,vmtaddr);
        }
    }

}

char* isVftPtr(void*addr) { // returns addr of mangled class name (prefix it with _Z to demangle with c++filt)
    unsigned int* vmtaddr = isValidPtr(addr,4)
                         && 0 == (3 & *(int*)addr)
                         && isValidPtr(*(int**)addr,4)
                         ? *(unsigned int**)addr
                         : (void*)0;
    if (vmtaddr
      &&isValidPtr(vmtaddr-2,0x20)
     ) {
            char**ptypeinfo = ((char***)vmtaddr)[-1];
            if (isValidPtr(ptypeinfo,4)
              &&isValidPtr((char***)ptypeinfo[0]-1,8)
              &&isValidPtr(((char***)ptypeinfo[0])[-1],8)
              &&isValidPtr(((char***)ptypeinfo[0])[-1][1],0x20)
              &&isIdentifier(ptypeinfo[1])
            ) {
                return !strncmp(((char***)ptypeinfo[0])[-1][1], "N10__cxxabiv",12) ? ptypeinfo[1] : 0;
            }
    }
    return 0;
}
void printVfts(const char*tag, void* addr, int from, int upto) {
    void** start = addr+from;
    void** end = addr+upto;
    DLOG("{ %s ====== printVfts %p (%p..%p)", tag, addr,start,end);
    void**p;
    char*n = 0;
    for(p=addr;p<end;p++) {
        if (n = isVftPtr(p)) {
            DLOG("vft at %p [off=0x%x] _Z%s",p,(unsigned)p - (unsigned)addr, n);
        }
    }
    DLOG("} %s ====== printVfts %p", tag, addr);
}
