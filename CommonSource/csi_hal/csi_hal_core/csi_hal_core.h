
#include <stdint.h>
#include "platform.h"
#ifndef __CSI_HAL_CORE_H
#define __CSI_HAL_CORE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <csi_config.h>
#if defined(CHIP_GC9002)
#include <soc.h>
#include <csi_core.h>
#include "csi_irq.h"
#endif
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "gc90xx_hal_def.h"
#include "gc90xx_hal.h"
#endif

#if !defined(CHIP_GC9002) && !defined(CHIP_GC9005)
#include "publicType.h"
#endif


void csi_hal_enable_irq(void);

void csi_hal_disable_irq(void);

void csi_hal_enable_excp_irq(void);

void csi_hal_disable_excp_irq(void);

void csi_hal_irq_enable(uint32_t irq_num);

void csi_hal_irq_disable(uint32_t irq_num);

void csi_hal_irq_register(uint32_t irq_num, void *irq_handler);

void csi_hal_irq_unregister(uint32_t irq_num);

void csi_hal_DCache_Flush(void);

#ifdef __cplusplus
}
#endif

#endif
