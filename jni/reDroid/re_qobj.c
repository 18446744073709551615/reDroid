#include "dlog.h"
#include "re_abbrevtypes.h"
#include "re_qobj.h"
#include "malloc.h"
#include <string.h>

struct sig_slt {
    int signature, parameters, type, tag, flags;
};
struct qmetaobjectdata {
    int revision, classname, n_classinfo, o_classinfo, n_methods, o_methods, n_properties, o_properties, n_enums_sets , o_enums_sets, n_ctor, o_ctor, flags, n_signals;
    struct sig_slt signals;
    struct sig_slt slots;
    int eod; //0
};
struct QMetaObject {
        const struct QMetaObject *superdata;
        const char *stringdata;
        const struct qmetaobjectdata /*unsigned int*/ *data;
        const struct QMetaObject **extradata;
};
/*
struct QMetaObjectPrivate
{
    int revision;
    int className;
    int classInfoCount, classInfoData;
    int methodCount, methodData;
    int propertyCount, propertyData;
    int enumeratorCount, enumeratorData;
};*/
enum MethodFlags  {
    AccessPrivate = 0x00,
    AccessProtected = 0x01,
    AccessPublic = 0x02,
    AccessMask = 0x03, //mask

    MethodMethod = 0x00,
    MethodSignal = 0x04,
    MethodSlot = 0x08,
    MethodTypeMask = 0x0c,

    MethodCompatibility = 0x10,
    MethodCloned = 0x20,
    MethodScriptable = 0x40
};

int isValidQMetaObject(void*p) {
    struct QMetaObject* pm = p;
    return isValidPtr(pm,sizeof(struct QMetaObject))
        && isValidOrNullPtr(pm->superdata,sizeof(struct QMetaObject))
        && isValidPtr(pm->stringdata,0x40)
        && isValidPtr(pm->data,sizeof(struct qmetaobjectdata))
        ;
}
void logQMetaObject(void* p) {
DLOG("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~ logQMetaObject(%p)",p);
    struct QMetaObject* pm = p;
    if (isValidQMetaObject(p)) {
        DLOG("QMetaObject name:[%s]",pm->stringdata);
        struct QMetaObject const* q = pm->superdata;
        while(q) { DLOG("==q==> %s",q->stringdata); q = q->superdata;}
        log_dumpf("QMetaObject: %s",pm,0x80, 0x80);
        log_dumpf("QMO stringdata: %s",pm->stringdata,0x80,0x80);
    } else {
        DLOG("Not a QMetaObject: %p",p);
        log_dumpf("no QMetaObject: %s",pm,0x80, 0x80);
    }
//    DLOG("",pm->stringdata);
}


void*getMetaObject(DW o) {
    typedef void* func();
    if(isVftPtr(o)) {
//DLOG("=== getMetaObject o=%x",o);
//log_o(o);
//log_rdumpf("=== getMetaObject %s",o,0,4*4,2,64);
//printVfts("=== getMetaObject o: ", o, -8, 0x400);
//DLOG("return ((func***)o)[0][0](o);");
        void* res =  ((func***)o)[0][0](o);
//DLOG("=== getMetaObject res=%x",res);
        return res;
        //return ((func***)o)[13][0](o);
    } else {
        return 0;
    }
}

void qlog(DW pp) {
DLOG("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~ o=%lx",pp);
    void* p = getMetaObject(pp);
DLOG("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~ mo=%p",p);
//    log_rdumpf("qlog %s",p,-8,0x20,2,0x40);
    logQMetaObject(p);
}
