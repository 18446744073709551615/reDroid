
#include "narg.h"
#include "re_abbrevtypes.h"

// check the arguments if any of them is an object of interest
// if any of them is, crash() will be performed
// you can specify how many times such objects must be encountered before crash() executed, see the source
// the variable nocrash must be defined (int or boolean), true = no crash now
#define encounter(...) (_encounter_(nocrash,__func__,PP_NARG(__VA_ARGS__),__VA_ARGS__))

void _encounter_(int nobreak, const char* name, DW n, ...);

// specify an object of interest
void toencounter(DW o);

