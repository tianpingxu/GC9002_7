
//#include "csi_hal_core.h"
#include <soc.h>
#include <csi_core.h>
#include <csi_config.h>
#include "gc90xx_hal_def.h"
#include "csi_irq.h"
#include "gc90xx_hal.h"

#ifndef __CSI_HAL_DRIVER_FLASH_H
#define __CSI_HAL_DRIVER_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

void csi_hal_internal_flash_init(void);
void csi_hal_internal_flash_SetWriteProtect(void);
void csi_hal_internal_flash_ClearWriteProtect(void);
uint32_t csi_hal_internal_flash_read(uint32_t addr, uint32_t len, uint8_t* buffer);
void csi_hal_internal_flash_write(uint32_t addr, uint32_t len, uint8_t* buffer);
void csi_hal_interal_flash_sector_erase(uint32_t addr);
void csi_hal_internal_flash_block_erase(uint32_t addr);


void csi_hal_extern_flash_init(void);
void csi_hal_extern_flash_SetWriteProtect(void);
void csi_hal_extern_flash_ClearWriteProtect(void);
uint32_t csi_hal_extern_flash_read(uint32_t addr, uint32_t len, uint8_t* buffer);
void csi_hal_extern_flash_write(uint32_t addr, uint32_t len, uint8_t* buffer);
void csi_hal_extern_flash_sector_erase(uint32_t addr);
void csi_hal_extern_flash_block_erase(uint32_t addr);

#ifdef __cplusplus
}
#endif

#endif
