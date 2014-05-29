
#include <stddef.h>
#include "re_encounter.h"
//#include <stdarg.h>
#include "dlog.h"

struct encounter_list {
  int n;
  int t; // в который раз должно встретиться
  DW x[20];
};
static struct encounter_list encounterList = {0, 0x7FFFffff, {0}};

void toencounter(DW o) {
  if (encounterList.n >= COUNT_OF(encounterList.x)) {
    DLOG("encounter list overflow: %i",encounterList.n);
    crash();
  } else {
    encounterList.x[encounterList.n++] = o;
  }
}
// TODO: function name
// TODO: macro, including res
void _encounter_1(int nobreak, const char* name, DW o) {
#define ENCOUNTER_STRING "\n\n\n\n==== *** ENCOUNTER ***\n\n==== encountered "
  int i;
  for(i=0; i<encounterList.n; i++) {
    if(encounterList.x[i] == o) {
      DLOG(ENCOUNTER_STRING "%lx in %s   t=%i nobreak=%i", o, name, encounterList.t, nobreak);
      if(!nobreak && !--encounterList.t) {
        crash();
      }
    } else if(encounterList.x[i] <= o && /*dlmalloc_usable_size(encounterList.x[i]) != 0 &&*/ o <= 2048 /*dlmalloc_usable_size(o)*/ + (DW)encounterList.x[i]) {
      DLOG(ENCOUNTER_STRING "%lx within %lx [%lx] in %s   t=%i nobreak=%i", o, encounterList.x[i], (DW)dlmalloc_usable_size(encounterList.x[i]), name, encounterList.t, nobreak);
      if(!nobreak && !--encounterList.t) {
        crash();
      }
    }
  }
}

void _encounter_(int nobreak, const char* name, DW n, ...) {
    va_list vl;
    va_start(vl, n);
    int i;
    for (i=0;i<n;i++)
    {
        _encounter_1(nobreak, name, va_arg(vl,DW));
    }
    va_end(vl);
}
