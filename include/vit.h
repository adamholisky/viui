#ifndef VIOS_VIT_INCLUDED
#define VIOS_VIT_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

/**************************************/
/* Header Config                      */
/**************************************/
#include <stdint.h>
#include <stdbool.h>

#ifdef VI_ENV_OS
	#include <kernel_common.h>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#endif

#include "lib/bitmap.h"

/**************************************/
/* System API Macro Config            */
/**************************************/

#ifdef VI_ENV_OS
	#define vit_malloc kmalloc
#else
	#define vit_malloc malloc
#endif

#ifdef __cplusplus
}
#endif
#endif