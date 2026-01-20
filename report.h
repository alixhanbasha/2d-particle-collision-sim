
#include <sys/types.h>

#ifndef __REPORTS
#define __REPORTS

typedef struct {
  uint whites;
  uint blues;
  uint reds;
  uint greens;
} Report;

Report *GetReport();
#endif
