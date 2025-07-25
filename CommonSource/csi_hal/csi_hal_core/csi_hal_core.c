
#include "platform.h"
#include "gc90x_cache.h"

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include <stdint.h>
#include <csi_config.h>
#include "gc90xx_hal.h"
#include "csi_hal_core.h"



void csi_hal_enable_irq(void)
{
	__enable_irq();	
}


void csi_hal_disable_irq(void)
{
	__disable_irq();
}

void csi_hal_enable_excp_irq(void)
{
#if defined(CHIP_GC9002)
	__enable_excp_irq();
#else
	__enable_irq();
#endif
}

void csi_hal_disable_excp_irq(void)
{
#if defined(CHIP_GC9002)
	__disable_excp_irq();
#else
	__disable_irq();
#endif
}

void csi_hal_irq_enable(uint32_t irq_num)
{
#if defined(CHIP_GC9002)
	csi_irq_enable(irq_num);
#else
	ECLIC_EnableIRQ(irq_num);
#endif
}


void csi_hal_irq_disable(uint32_t irq_num)
{
#if defined(CHIP_GC9002)
	csi_irq_disable(irq_num);
#else
	ECLIC_DisableIRQ(irq_num);
#endif
}

void csi_hal_irq_register(uint32_t irq_num, void *irq_handler)
{
#if defined(CHIP_GC9002)
	#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
	csi_clic_register_irq(irq_num, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, irq_handler);
	#else
	csi_clic_register_irq(irq_num, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, irq_handler);
	#endif
#else
    #if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    ECLIC_Register_IRQ(irq_num, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 1, irq_handler);
    #else
    ECLIC_Register_IRQ(irq_num, ECLIC_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 1, irq_handler);
    #endif
#endif
}

void csi_hal_irq_unregister(uint32_t irq_num)
{
//	drv_irq_unregister(irq_num);
}

#endif
void csi_hal_DCache_Flush(void)
{
	DCache_Flush();
}
