#include "mygl.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

MyGL *mygl = NULL;

MYGLSTRNFUNCS(32);
MYGLSTRNFUNCS(64);

#define MYGL_NO_STATIC_INLINE_VEC
#include "vecdefs.inc"
