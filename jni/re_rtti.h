#ifdef __cplusplus
extern "C" {
#endif

void _log_obj(void* addr, char*s);
void _log_o(unsigned long a, char* s);
#define log_o(x) _log_o( x , #x)
#define log_obj(x) _log_obj( x , #x )
#ifdef __cplusplus
}
#endif
