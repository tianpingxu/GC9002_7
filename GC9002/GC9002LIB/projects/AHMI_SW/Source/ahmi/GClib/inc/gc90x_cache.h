#ifndef __GC90x_CACHE_H
#define __GC90x_CACHE_H

#ifdef __cplusplus
 extern "C" {
#endif
     
//#include "CPU1_CMSDK_CM0.h"
#include "gc90xx_hal.h"

void DCache_Flush(void);
void DCache_Invalid(void);
     
#ifdef __cplusplus
}
#endif

#endif /* __GC90x_CACHE_H */
