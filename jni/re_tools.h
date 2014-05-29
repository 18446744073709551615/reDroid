
#ifdef __cplusplus
extern "C" {
#endif

// crash the program (jump to 0).
// You will see a lot of info in the logs!
void crash();

// check if (p) points to at least (len) readable bytes
int isValidPtr(const void*p, int len);
int isValidPtru(unsigned long p, int len);
int isValidOrNullPtr(const void*p, int len);

// true if (s) points to a null-terminated string with an identifier
int isIdentifier(const char* s);

char* strrealloccat(char* buffer0, char *addition);

#ifdef __cplusplus
}
#endif
