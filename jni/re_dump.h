
#ifdef __cplusplus
extern "C" {
#endif

void log_dump(const void*addr,int len,int linelen);
void log_dumpf(const char*fmt,const void*addr,int len,int linelen);

void log_rdumpf(const char*fmt,const void*addr, int offset, int len, int levels, int linelen);

#ifdef __cplusplus
}
#endif
