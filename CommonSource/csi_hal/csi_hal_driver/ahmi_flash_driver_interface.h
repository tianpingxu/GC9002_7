

#include "csi_hal_driver_flash.h"


#ifndef __AHMI_FLASH_DRIVER_H
#define __AHMI_FLASH_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

void ahmi_internal_flash_SetWriteProtect(uint32_t intflag);
void ahmi_internal_flash_ClearWriteProtect(uint32_t intflag);
uint32_t ahmi_internal_flash_read(uint32_t addr, uint32_t len, uint8_t* buffer, uint32_t intflag);
void ahmi_internal_flash_write(uint32_t addr, uint32_t len, uint8_t* buffer, uint32_t intflag);
void ahmi_interal_flash_sector_erase(uint32_t addr, uint32_t intflag);
void ahmi_internal_flash_block_erase(uint32_t addr, uint32_t intflag);


#ifdef __cplusplus
}
#endif

#endif
