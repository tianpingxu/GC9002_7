/**
  ******************************************************************************
  * @file    gc90x_flash.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    23-April-2019
  * @brief   This file contains all the functions prototypes for the FLASH 
  *          firmware library.
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC900x_FLASH_H
#define __GC900x_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------*/
//#include "CPU1_CMSDK_CM0.h"
#include "stdint.h"

/** @defgroup FLASH_Status 
  * @{
  */
	 
#define FLASHSTATUS_OK 				((uint8_t)0x00)
#define FLASHSTATUS_ERROR 		((uint8_t)0x01)

/**
  * @}
  */
	
	 
/** @defgroup FLASH_Exported_Functions
  * @{
  */

void INTERNAL_FLASH_SetWriteProtect(void);
void INTERNAL_FLASH_ClrWriteProtect(void);
void INTERNAL_FLASH_SectorErasePage(uint32_t Page_Address);
void INTERNAL_FLASH_BlockErasePage(uint32_t Page_Address);
uint32_t INTERNAL_FLASH_WriteData(uint32_t Address, uint32_t DataSize, uint32_t *buffer);
uint32_t INTERNAL_FLASH_ReadData(uint32_t Address, uint32_t DataSize, uint32_t *buffer);
    
/**
  * @}
  */ 
	 
#ifdef __cplusplus
}
#endif

#endif /* __GC900x_FLASH_H */
