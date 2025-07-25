#include <soc.h>
#include <csi_core.h>
#include <csi_config.h>
#include "gc90xx_hal_def.h"
#include "csi_irq.h"
#include "gc90xx_hal.h"

#ifndef GC9002_POWER_H
#define GC9002_POWER_H

#ifdef __cplusplus
extern "C" {
#endif

void ldo_calibration(void);
 
void init_ldo_and_hyper(void);
 
#ifdef __cplusplus
}
#endif
 
#endif