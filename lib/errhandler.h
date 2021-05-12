#include "stdio.h"

#define __ERROR_CHECK(cond, erropt, errname, errmsg, args...)                  \
  if (cond) {                                                                  \
    printf("\x1b[1;31m[" errname "]> " errmsg "\x1b[0m\n", ##args);            \
    erropt;                                                                    \
  }

#define __HANDLE_ERROR(errvar, handle)                                         \
  if (errvar)                                                                  \
  handle\
