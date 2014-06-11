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

struct QMetaMethod {
 struct QMetaObject* mobj;
 unsigned int handle;
// int a,b,c,d;
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

// no different from _ZNK11QMetaObject12methodOffsetEv()
int xxMethodOffset(struct QMetaObject const* q) { // same as _ZNK11QMetaObject12methodOffsetEv
    int offset = 0;
    const struct QMetaObject *m = q->superdata;
    while (m) {
        offset += m->data->n_methods;
        m = m->superdata;
    }
    return offset;
}

void log_methods_x(struct QMetaObject const* q, int onlyNew) {
#if 0
    int i;
// does not really work
    int n,off;
log_dumpf("data: %s",q->data,0xC0,0x40);
log_dumpf("stringdata: %s",q->stringdata,0x4000,0x40);
    for(i=0, n=q->data->n_methods, off=q->data->n_methods; i<n; i++) {
        DLOG("-----------q--[%s]",q->stringdata+off);
        off += 1+strlen(q->stringdata+off);
    }
DLOG("######### [%s]",q->stringdata);
DLOG("######### methods: %x",q->data->n_methods);
DLOG("#########");
#endif
//    DLOG("\n\n\n#############################################\n\n");
    int nMethods = _ZNK11QMetaObject11methodCountEv(q);
    //int xmethodOffset = xxMethodOffset(q);
    int methodOffset = _ZNK11QMetaObject12methodOffsetEv(q);
    DLOG("### -- methodCount = 0x%x  methodOffset = 0x%x --",nMethods, methodOffset);
    int i;
    for (i = onlyNew ? methodOffset : 0; i< nMethods; i++) {
        struct QMetaMethod qmm = {0,0};
        _ZNK11QMetaObject6methodEi(&qmm, q, i);
        char* signature = (void*)_ZNK11QMetaMethod9signatureEv(&qmm);
        DLOG("### %i 0x%x [%s]",i,i,signature);
    }
//    DLOG("######### end =============");
}
void log_methods_all(struct QMetaObject const* q) {
    log_methods_x(q,0);
}
void log_methods_new(struct QMetaObject const* q) {
    log_methods_x(q,1);
}

void logQMetaObject(void* p) {
DLOG("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~ logQMetaObject(%p)",p);
    struct QMetaObject* pm = p;
    if (isValidQMetaObject(p)) {
        DLOG("QMetaObject name:[%s]",pm->stringdata);
        log_methods_new(pm);
        struct QMetaObject const* q = pm->superdata;
        while(q) {
            DLOG("==q==> %s",q->stringdata);
            log_methods_new(q);
            q = q->superdata;
        }
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
