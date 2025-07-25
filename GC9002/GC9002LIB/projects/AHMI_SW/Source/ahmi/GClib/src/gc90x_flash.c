/**
  ******************************************************************************
  * @file    gc90x_flash.c
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    23-April-2019
  * @brief   This file provides all the FLASH firmware functions.
  ******************************************************************************
	*/
	
/* Includes ------------------------------------------------------------------*/
#include "gc90x_flash.h"
#include "texture_flash.h"
//#include "ROMData.h"
//
//extern const unsigned int constROMData_ProgramData[];
//extern const unsigned int constROMData_InitFlash[];
//extern const unsigned int constROMData_CMDRead[];
//extern const unsigned int constROMData_CMDWrite[];
//extern const unsigned int constROMData_EraseSector[];
//extern const unsigned int constROMData_EraseBlock[];
//extern const unsigned int constROMData_ReadData[];
//extern const unsigned int constROMData_SetWriteProtect[];
//extern const unsigned int constROMData_ClrWriteProtect[];

/** @defgroup Flash_Private_Functions
  * @{
  */
	
/**
  * @brief  Protect whole internal Flash from WRITE and ERASE command for GD flash
  * @note   This function can be used for all gc90x devices.
  * @param  Page_Address: The page start address to be erased.
  */
void INTERNAL_FLASH_SetWriteProtect(void)
{
//   	uint32_t *pROMData;
//	uint32_t i;
//	uint32_t romdata[ROMDATA_SetWriteProtectSIZEW] = {0};
//	uint32_t offset = 0;
//    uint32_t interruptstatus = 0;
//    uint32_t systickinterruptstatus = 0;
//	pSetWriteProtect SetWriteProtect;
//	
//	/* load valid ReadData function */
//	pROMData	= (unsigned int *) constROMData_SetWriteProtect;
//    for(i = 0; i < ROMDATA_SetWriteProtectSIZEW; i++) {
//			romdata[i] = *(pROMData++);
//	}
//	offset = constROMData_SetWriteProtect[0] - ROM_BaseAddr;
//	romdata[0] = (uint32_t)romdata + offset;
//	
//	SetWriteProtect = *(pSetWriteProtect *)romdata; 
//    
//    /* close all nvic interrupt */
//	interruptstatus = NVIC->ISER[0];
//	NVIC->ICER[0] = interruptstatus;
//    
//    /* close systick interrupt */
//    systickinterruptstatus = SysTick->CTRL;
//    if(systickinterruptstatus & SysTick_CTRL_TICKINT_Msk)
//    {
//        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
//    }
//    
//    SetWriteProtect();
//    
//    /* enable interrupt had been open */
//	NVIC->ISER[0] = interruptstatus;
//    
//    /* enable systick interrupt if had been open */
//    if(systickinterruptstatus & SysTick_CTRL_TICKINT_Msk)
//    {
//        SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
//    }
}
	
/**
  * @brief  Unprotect whole internal Flash from WRITE and ERASE command for GD flash
  * @note   This function can be used for all gc90x devices.
  * @param  Page_Address: The page start address to be erased.
  */
void INTERNAL_FLASH_ClrWriteProtect(void)
{
//   	uint32_t *pROMData;
//	uint32_t i;
//	uint32_t romdata[ROMDATA_ClrWriteProtectSIZEW] = {0};
//	uint32_t offset = 0;
//    uint32_t interruptstatus = 0;
//    uint32_t systickinterruptstatus = 0;
//	pClrWriteProtect ClrWriteProtect;
//	
//	/* load valid ReadData function */
//	pROMData	= (unsigned int *) constROMData_ClrWriteProtect;
//    for(i = 0; i < ROMDATA_ClrWriteProtectSIZEW; i++) {
//			romdata[i] = *(pROMData++);
//	}
//	offset = constROMData_ClrWriteProtect[0] - ROM_BaseAddr;
//	romdata[0] = (uint32_t)romdata + offset;
//	
//	ClrWriteProtect = *(pClrWriteProtect *)romdata; 
//    
//        /* close all nvic interrupt */
//	interruptstatus = NVIC->ISER[0];
//	NVIC->ICER[0] = interruptstatus;
//    
//    /* close systick interrupt */
//    systickinterruptstatus = SysTick->CTRL;
//    if(systickinterruptstatus & SysTick_CTRL_TICKINT_Msk)
//    {
//        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
//    }
//    
//    ClrWriteProtect();
//    
//    /* enable interrupt had been open */
//	NVIC->ISER[0] = interruptstatus;
//    
//    /* enable systick interrupt if had been open */
//    if(systickinterruptstatus & SysTick_CTRL_TICKINT_Msk)
//    {
//        SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
//    }
}    

/**
  * @brief  Erases a specified Sector INTERNAL FLASH page.
  * @note   This function can be used for all gc90x devices.
  * @param  Page_Address: The page start address to be erased.
  */
void INTERNAL_FLASH_SectorErasePage(uint32_t Page_Address)
{
	DRAM_CACHE->REG90 = 0x06000001; //opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};
        
    DRAM_CACHE->REG94 = Page_Address;       //set erase sector/block address
    DRAM_CACHE->REG90 = 0x200B0001; //opcode 0xD8 for block erase
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};

    do
    {
        DRAM_CACHE->REG90 = 0x05800001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    return OK;
}

/**
  * @brief  Erases a specified Block INTERNAL FLASH page.
  * @note   This function can be used for all gc90x devices.
  * @param  Page_Address: The page start address to be erased.
  */
void INTERNAL_FLASH_BlockErasePage(uint32_t Page_Address)
{
//	uint32_t *pROMData;
//	uint32_t i;
//	uint32_t romdata[ROMDATA_EraseBlockSIZEW] = {0};
//	uint32_t offset = 0;
//	uint32_t interruptstatus = 0;
//    uint32_t systickinterruptstatus = 0;
//	pEraseBlockr EraseBlock;
//	
//	/* load valid EraseBlock function */
//	pROMData	= (unsigned int *) constROMData_EraseBlock;
//    for(i = 0; i < ROMDATA_EraseBlockSIZEW; i++) {
//			romdata[i] = *(pROMData++);
//	}
//	offset = constROMData_EraseBlock[0] - ROM_BaseAddr;
//	romdata[0] = (uint32_t)romdata + offset;
//	
//	/* get function ptr */
//	EraseBlock = *(pEraseBlockr *)(romdata);
//	
//	/* close all nvic interrupt */
//	interruptstatus = NVIC->ISER[0];
//	NVIC->ICER[0] = interruptstatus;
//    
//    /* close systick interrupt */
//    systickinterruptstatus = SysTick->CTRL;
//    if(systickinterruptstatus & SysTick_CTRL_TICKINT_Msk)
//    {
//        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
//    }
//    
//	EraseBlock(Page_Address);
//	/* enable interrupt had been open */
//	NVIC->ISER[0] = interruptstatus;
//    
//    /* enable systick interrupt if had been open */
//    if(systickinterruptstatus & SysTick_CTRL_TICKINT_Msk)
//    {
//        SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
//    }
}

/**
  * @brief  Programs buffer data at a specified address.
  * @note   This function can be used for all gc90x devices.
  * @param  Address: specifies the address to be writed.
  * @param  DataSize: specifies the data size to be writed.
  * @param  *buffer: specifies the data to be writed.
	* @retval FLASH Status: The returned value can be: 
	*					FLASHSTATUS_ERROR or FLASHSTATUS_OK. 
  */
uint32_t INTERNAL_FLASH_WriteData(uint32_t Address, uint32_t DataSize, uint32_t *buffer)
{
	DataSize = (DataSize + 3) & ~3;                                   // Adjust size for Words	
	while (DataSize) {		
		*(unsigned int *)(Address) = *((unsigned int *)buffer);
		Address += 4;                                           // Go to next Word
		buffer += 1;
		DataSize  -= 4;
	}
	
	return FLASHSTATUS_OK;
}

/**
  * @brief  Read a specified address in data buffer.
  * @note   This function can be used for all gc90x devices.
  * @param  Address: specifies the address to be read.
  * @param  DataSize: specifies the data size to be read.
  * @param  *buffer: specifies the data to be read.
  * @retval FLASH Status: The returned value can be: 
	*					FLASHSTATUS_ERROR or FLASHSTATUS_OK.   
  */
uint32_t INTERNAL_FLASH_ReadData(uint32_t Address, uint32_t DataSize, uint32_t *buffer)
{
	DataSize = (DataSize + 3) & ~3;
	while (DataSize) {		
		*(unsigned int *)(buffer) = *((unsigned int *) Address);
		Address += 4;                                           // Go to next Word
		buffer += 1;
		DataSize  -= 4;
	}

	return FLASHSTATUS_OK;
}
