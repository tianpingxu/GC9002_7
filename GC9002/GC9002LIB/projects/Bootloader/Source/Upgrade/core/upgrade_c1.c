////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2021/07/05
// File Name:     upgrade_c1.c
// Project Name:  CPU0_Bootloader_Redesign
// 
// Revision:
// Revision 1.0.0 - File Created 2021/06/22 by Zhu Jiahai
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#include "DrawProgramBar.h"
#include "crc_c1.h"
#include "upgrade_c1.h"
#include "em_ct.h"
#include "ParsePara.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "csi_hal_driver_flash.h"



/* variables to be used for save version information */

extern char CPU1_bootloader_version[32];
extern char CPU1_bootloader_compile_date[32];
extern char CPU1_bootloader_compile_time[32];

extern const char BootVersion[32];
extern const char Boot_compile_date[32];
extern const char Boot_compile_time[32];




/* buffer for Texture upgrading */
uint8_t myUpgradebuffer[TextureBufferMaxSize] = {0} /* __attribute__((aligned(0x200))) */ ;
uint8_t myReadBuffer[TextureBufferMaxSize]  /* __attribute__((aligned(0x200))) */;

/* To save the size of cpu1.bin */
static uint32_t u32Cpu1BinSize = 0;

extern UPG_tstWorkBuf *UPG_pstWorkBuf;

extern uint32_t SCREEN_WIDTH;
extern uint32_t SCREEN_HEIGHT;
/* Saved the Flash size initiallized in Flash driver */
extern uint32_t TextureFlashSize;


/**
  * @name       UPG_u32GetMaxTextureSize
  * @brief      Get max size of Texture that can be saved in Flash
  * @note        
  * @param      none
  * @retval     uint32_t: max size of Texture
  * @author     
  * @date       2023-09-25
  */
uint32_t UPG_u32GetMaxTextureSize(void)
{
    uint32_t res;
    if(TextureFlashSize > TEXTURE_FLASH_OFFSET)
    {
        res = TextureFlashSize - TEXTURE_FLASH_OFFSET;
    }
    else
    {
        res = TEXTURE_SIZE_MAX;
    }
    return res;
}

/**
  * @name       UPG_i32GetUpgFileName
  * @brief      Get indicated Upgrade file name
  * @note        
  * @param      fileIndex: indicate the file index
  * @param      ppFileName: the pointer to save the filename start address
  * @retval     The result: OK or ERR.
  * @author     
  * @date       2023-07-24
  */
int32_t UPG_i32GetUpgFileName(uint8_t fileIndex, uint8_t** ppFileName)
{
    if((NULL == ppFileName) || (fileIndex >= UPG_enUpgFileCount))
    {
        SYN_ERR_LOG(EM_enErrInParaInvalid, EM_enErrLevelFatal);
		return UPG_ERR;
    }
    else
    {
        *ppFileName = UPG_pstWorkBuf->pstPbConfig[UPG_pstWorkBuf->u8FileIndex].pu8FileName;
    }
    
    return UPG_OK;
}


/**
  * @name       UPG_u8GetCurUpgFileIndex
  * @brief      Get current Upgrade file index
  * @note        
  * @param      none
  * @retval     current Upgrade file index
  * @author     
  * @date       2023-07-24
  */
uint8_t UPG_u8GetCurUpgFileIndex(void)
{
   
    return UPG_pstWorkBuf->u8FileIndex;
   
}


/**
  * @name       UPG_u32GetCpu1BinSize
  * @brief      Get cpu1.bin size
  * @note       If just upgrade cpu1.bin, it will keep the exact value, else it return a defined value
  * @param      none
  * @retval     cpu1.bin size
  * @author     
  * @date       2023-07-28
  */
uint32_t UPG_u32GetCpu1BinSize(void)
{
    if(u32Cpu1BinSize != 0)
    {
        return u32Cpu1BinSize;
    }
    else
    {
        return CPU1_BIN_DEFAULT_SIZE;
    }
}


/**
  * @name       UPG_vKeepCpu1BinSize
  * @brief      Keep size of cpu1.bin 
  * @note       
  * @param      size: the size of cpu1.bin
  * @retval     none
  * @author     
  * @date       2023-07-28
  */
void UPG_vKeepCpu1BinSize(uint32_t size)
{
    u32Cpu1BinSize = size;
}


/**
  * @name       UPG_i32VerifyCPU0CodeByShareMemory
  * @brief      verify the validity of CPU0 code by share memory through CRC
  * @note       the original CRC value saved in the last four bytes 
  * @param      buffer: pointer to buffer
  * @param      size: size of the code in the buffer
  * @param      filetype: indicate the file type
  * @param      optype: option type
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_i32VerifyCPU0CodeByShareMemory(uint8_t* buffer, uint32_t size, uint8_t filetype, uint8_t optype)
{
    //vPortEnterCritical();
    //VerifyFlashByShareMemory((uint32_t)buffer, size);
    //vPortExitCritical();
    
    return UPG_OK;
}

/**
  * @name       UPG_i32VerifyCPUCodeFromBuffer
  * @brief      verify the validity of CPU code saved in the buffer through CRC
  * @note       the original CRC value saved in the last four bytes 
  * @param      buffer: pointer to buffer
  * @param      size: size of the code in the buffer
  * @param      filetype: indicate the file type
  * @param      optype: option type
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_i32VerifyCPUCodeFromBuffer(FIL* myfsrc, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype)
{
	uint32_t checkCode = 0xFFFFFFFF;
	uint32_t i = 0, offsetflag = 0, count;
	uint32_t numOfPage = 0;
	uint32_t remainder = 0;
	uint32_t CheckResult = 0;
	uint32_t CheckOriginalResult = 0;
	FRESULT fileRes;
	
	numOfPage = (filesize - 4) / bufsize;
		
	if((filesize - 4) % bufsize)
		numOfPage++;
	
	remainder = (filesize - 4) % bufsize;
	if(remainder == 0)
	{
		remainder = bufsize;
	}
	
	if(((uint32_t)buffer & CodeFlashBaseAddrMask) == CodeFlashBaseAddr)
	{
		offsetflag = 1;
	}
	else
	{
		offsetflag = 0;
		if(filesize > CPU1_CODE_SIZE_MAX)
		{
			SYN_ERR_LOG(EM_enErrCPUSizeOverrun, EM_enErrLevelFatal);
			return UPG_ERR;
		}
	}
	
	if(offsetflag == 1)
	{
		for(i = 0; i < 4; i++)
		{
			CheckOriginalResult |= (*(buffer + filesize - 4 + i)) << (8 * i);
		}
	}
	else
	{
		f_lseek(myfsrc, (filesize - 4));
		
		fileRes = f_read(myfsrc, &CheckOriginalResult, 4, &i);
		
		if(fileRes != FR_OK)
		{
            /* verify input CPU code fail */
			SYN_ERR_LOG(EM_enErrVerifyCodeCrcFail, EM_enErrLevelFatal);
			return UPG_ERR;
		}
		
		f_lseek(myfsrc, 0);
	}
	
	CRC_vInit();

	/* Write data to cpu1 by data frame. */
	for(i = 0; i < numOfPage; i++)
	{
		if(offsetflag == 0)
		{
			if(i == (numOfPage - 1))
			{
				fileRes = f_read(myfsrc, buffer, remainder, &count);
			}
			else
			{
				fileRes = f_read(myfsrc, buffer, bufsize, &count);
			}
			
			if(fileRes != FR_OK)
			{
				/* verify input CPU code fail */
				SYN_ERR_LOG(EM_enErrVerifyCodeCrcFail, EM_enErrLevelFatal);
				return UPG_ERR;
			}
		}
		
		if(i == (numOfPage - 1))
		{
			checkCode = CRC_u32Crc32(checkCode, (buffer + offsetflag * bufsize * i), remainder);
		}
		else
		{
			checkCode = CRC_u32Crc32(checkCode, (buffer + offsetflag * bufsize * i), bufsize);
		}
		
		if((filetype < UPG_enUpgFileCount) && (optype < UPGRADE_OPTYPE_END))
		{
			DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, filetype, optype, (i + 1) * SCREEN_WIDTH / numOfPage);
		}
	}
	
	CheckResult |= (~checkCode);
	
	if(CheckResult == CheckOriginalResult)
	{
		return UPG_OK;
	}
	else
	{
        /* verify input CPU code fail */
        SYN_ERR_LOG(EM_enErrVerifyCodeCrcFail, EM_enErrLevelFatal);
		return UPG_ERR;
	}
}


/**
  * @name       UPG_i32VerifyTextureACF
  * @brief      verify the validity of Texture
  * @note        
  * @param      myfsrc: pointer to file 
  * @retval     The result: OK or ERR.
  * @author     Yn
  * @date       2022-12-14
  */
int32_t UPG_i32VerifyTextureACF(FIL* myfsrc, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype)
{
	uint32_t sizeOfFile = 0, numOfPage, checkCode = 0xA55A5AA5, i, count, offsetflag;
	uint32_t NewProgressValue = 0, OldProgressValue = 0;
	FRESULT fileRes;
    int32_t res = UPG_OK;
	
	if(((uint32_t)buffer & CodeFlashBaseAddrMask) == CodeFlashBaseAddr)
	{
		offsetflag = 1;
	}
	else
	{
		offsetflag = 0;
		if(filesize > UPG_u32GetMaxTextureSize())
		{
			SYN_ERR_LOG(EM_enErrTextureSizeOverrun, EM_enErrLevelFatal);
			return UPG_ERR;
		}
	}
	
	numOfPage = (filesize + bufsize - 1) / bufsize;
	
	for(i = 0; i < numOfPage; i++)
    {
		if(offsetflag == 0)
		{
			fileRes = f_read(myfsrc, buffer, bufsize, &count);
			if(fileRes != FR_OK)
			{
                res = UPG_ERR;
				break;
			}
		}
        else
		{
			if(i == (numOfPage - 1))
			{
				count = filesize % bufsize;
			}
			else
			{
				count = bufsize;
			}
		}
		
        checkCode = CRC_u32XorCheck(&checkCode, (uint32_t *)(buffer + offsetflag * bufsize * i), count / 4);
		
		OldProgressValue = NewProgressValue;
		NewProgressValue = i * SCREEN_WIDTH / (numOfPage - 1);
			
		if(NewProgressValue != OldProgressValue)
		{
			DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, filetype, optype, NewProgressValue);
		}
		
		//vTaskDelay(20 / portTICK_RATE_MS);
	}
	
	if(checkCode == 0)
	{
		return res;
	}
	else
	{
		/* verify Texture fail */
        SYN_ERR_LOG(EM_enErrVerifyTextureFail, EM_enErrLevelFatal);
		return UPG_ERR;
	}
}


/**
  * @name       UPG_i32VerifyTextureACF_Every2K
  * @brief      verify the validity of Texture
  * @note        
  * @param      myfsrc: pointer to file 
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_i32VerifyTextureACF_Every2K(FIL* myfsrc)
{
	uint32_t checkCode = 0xA55A5AA5;
	uint32_t sizeOfFile = 0;
	uint32_t size = 0, i = 0;
	uint32_t numOfPage = 0;
	uint32_t remainder = 0;
	uint32_t CheckResult = 0;
	uint32_t CheckOriginalResult = 0;
	FRESULT fileRes;
	
	/* Get size of the file*/
	sizeOfFile = f_size(myfsrc);
	
	/* Get number of data frame.*/
    numOfPage = (sizeOfFile - 4) / TextureBufferMaxSize;
    
    if((sizeOfFile - 4) % TextureBufferMaxSize)
        numOfPage++;
	
	/* Change read pointer to start position for the file. */
    f_lseek(myfsrc, (sizeOfFile - 4));
	
	fileRes = f_read(myfsrc, &CheckOriginalResult, 4, &size);
	
	if(fileRes != FR_OK)
		return UPG_FAILURE;
	
	/* Change read pointer to start position for the file. */
    f_lseek(myfsrc, 0);
	
	remainder = (sizeOfFile - 4) % TextureBufferMaxSize;
	if(remainder == 0)
	{
		remainder = TextureBufferMaxSize;
	}
	
	/* Write data to cpu1 by data frame. */
    for(i = 0; i < numOfPage; i++)
    {
		if(i == (numOfPage - 1))
		{
			fileRes = f_read(myfsrc, myUpgradebuffer, remainder, &size);
			
			/*If read data successful. */
			if(fileRes == FR_OK)
			{
				CheckResult = CRC_u32XorCheck(&checkCode, (uint32_t *)myUpgradebuffer, size/4);
			}
		}
		else
		{
			fileRes = f_read(myfsrc, myUpgradebuffer, TextureBufferMaxSize, &size);
			
			/*If read data successful. */
			if(fileRes == FR_OK)
			{
				CheckResult = CRC_u32XorCheck(&checkCode, (uint32_t *)myUpgradebuffer, size/4);
			}
		}
		
	}
	
	if(CheckResult == CheckOriginalResult)
	{
		return UPG_OK;
	}
	else
	{   
        /* verify Texture fail */
        SYN_ERR_LOG(EM_enErrVerifyTextureFail, EM_enErrLevelFatal);
		return UPG_ERR;
	}
}

/**
  * @name       UPG_i32VerifyTextureData
  * @brief      verify the validity of Texture
  * @note        
  * @param      filesize: file size
  * @retval     The result: OK or ERR.
  * @author     Yn
  * @date       2022-12-15
  */
int32_t UPG_i32VerifyTextureData(uint32_t startaddr, uint32_t filesize)
{
	uint32_t checkCode = 0xA55A5AA5;
	uint32_t sizeOfFile = 0;
	uint32_t size = 0, i = 0;
	uint32_t numOfPage = 0;
	uint32_t CheckResult = 0xFFFFFFFF;
	uint32_t fileRes = UPG_ERR;
	uint32_t readAddr = startaddr;
    uint8_t fileIndex;

    fileIndex = UPG_u8GetCurUpgFileIndex();

	{
		uint32_t OldProgressValue = 0;
		uint32_t NewProgressValue = 0;
		
		/* Get size of the file*/
		sizeOfFile = filesize;
		
		/* Get number of data frame.*/
		numOfPage = (sizeOfFile) / TextureBufferMaxSize;
		
		if((sizeOfFile) % TextureBufferMaxSize)
			numOfPage++;
		
		/* Write data to cpu1 by data frame. */
		for(i = 0; i < numOfPage; i++)
		{
			if(sizeOfFile > TextureBufferMaxSize)
				size = TextureBufferMaxSize;
			else
				size = sizeOfFile;
				
			fileRes = csi_hal_extern_flash_read((readAddr & 0xFFFFF800), size, myUpgradebuffer);
				
			/*If read data successful. */
			if(fileRes == OK)
			{
				CheckResult = CRC_u32XorCheck(&checkCode, (uint32_t *)myUpgradebuffer, size/4);
			}
			
			sizeOfFile -= TextureBufferMaxSize;
			readAddr += TextureBufferMaxSize;
			
			OldProgressValue = NewProgressValue;
			
			/* get new progress value */
			NewProgressValue = i * SCREEN_WIDTH / (numOfPage - 1);
			
			if(NewProgressValue != OldProgressValue)
			{
				DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, fileIndex, VERIFY_DATA, NewProgressValue);
			}
		}
		
		if(CheckResult == 0)
		{
			return UPG_OK;
		}
		else
		{
			return UPG_ERR;
		}
	}
}


/**
  * @name       UPG_i32EraseCpu0Flash
  * @brief      to erase CPU0 Flash
  * @note        
  * @param      address: beginning address of Flash planned to erase
  * @param      size: erasing size 
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_i32EraseCpu0Flash(uint32_t address, uint32_t size)
{
	//EraseFlashByShareMemory(address, size);
	
	return UPG_OK;
}

/**
  * @name       UPG_i32EraseCpu1Flash
  * @brief      to erase CPU1 Flash
  * @note        
  * @param      address: beginning address of Flash planned to erase
  * @param      size: erasing size 
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_i32EraseCpu1Flash(uint32_t address, uint32_t size)
{
	uint32_t EraseOfSector;
    int32_t i, value = 0;
    uint8_t fileIndex;

    fileIndex = UPG_u8GetCurUpgFileIndex();

    DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, fileIndex, ERASE, 0);

    /* Calculate number of sector. */
	EraseOfSector = size / CodeFlashSectorSize;
    if(size % CodeFlashSectorSize)
        EraseOfSector++;

    
    /*Erase Upgrade Flash*/
	for( i = 0; i < EraseOfSector; i++ )
	{
		csi_hal_interal_flash_sector_erase(address + i * CodeFlashSectorSize);

        value = (i + 1) * SCREEN_WIDTH / EraseOfSector;
        DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, fileIndex, ERASE, value);
	}
	
	return UPG_OK;
}

/**
  * @brief  Erase texture flash
  * @note
  * @param  address:start address of flash area that wanted to be erased .
  * @param  size:size of the erased ares.
  * @retval The result of send the command: SUCCESS or FAILURE.
  */
uint32_t UPG_u32EraseTextureFlash(uint32_t address, uint32_t size)
{
    uint32_t EraseOfBlock = 0;
    uint32_t i = 0, value = 0;  
    uint32_t ret = UPG_OK;
    uint8_t fileIndex = UPG_u8GetCurUpgFileIndex();

	DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, fileIndex, ERASE, 0);

    EraseOfBlock = size / TextureNorFlashBlockSize; 
    
    if(size % TextureNorFlashBlockSize)
    {
        EraseOfBlock++;
    }
    
    /* Erase blocks. */
    for(i = 0; i < EraseOfBlock; i++)
    {
        /* Erase block with start page ID. */
		csi_hal_extern_flash_block_erase(address + i * TextureNorFlashBlockSize);
		//INTERNAL_FLASH_BlockErasePage(address + i*TextureNorFlashBlockSize);
        //ret = ext_flash_erase(address + i*TextureNorFlashBlockSize);
		
		value = (i + 1) * SCREEN_WIDTH / EraseOfBlock;
		DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, fileIndex, ERASE, value);
    }
	
    return ret;
}

/**
  * @brief  write texture flash
  * @note
  * @param  address:start address of flash area that wanted to be erased .
  * @param  size:size of the erased ares.
  * @retval The result of send the command: OK or OTHER.
  */
uint32_t UPG_u32WriteTextureFlash(uint32_t address, uint8_t* pbuffer, uint32_t size)
{
    uint32_t ret = UPG_OK;
   
    csi_hal_extern_flash_write(address, size, (uint8_t *)pbuffer);

    return ret; 
}



/**
  * @brief  read texture flash
  * @note
  * @param  address:start address of flash area that wanted to be erased .
  * @param  size:size of the erased ares.
  * @retval The result of send the command: OK or OTHER.
  */
uint32_t UPG_u32ReadTextureFlash(uint32_t address, uint8_t* pbuffer, uint32_t size)
{
    uint32_t ret = UPG_OK;

	csi_hal_extern_flash_read(address, size, pbuffer);
	
    return ret;
}


uint32_t UPG_u32VerifyTextureData(uint32_t TextureSize)
{
	uint32_t checkCode = 0xA55A5AA5;
	uint32_t sizeOfFile = 0;
	uint32_t size = 0, i = 0;
	uint32_t numOfPage = 0;
	uint32_t CheckResult = 0xFFFFFFFF;
	uint32_t fileRes = UPG_ERR;
	uint32_t readAddr = 0;
	
	/* Get size of the file*/
	sizeOfFile = TextureSize;
	
	/* Get number of data frame.*/
    numOfPage = (sizeOfFile) / TextureBufferMaxSize;
    
    if((sizeOfFile) % TextureBufferMaxSize)
        numOfPage++;
	
	/* Write data to cpu1 by data frame. */
    for(i = 0; i < numOfPage; i++)
    {
        if(sizeOfFile > TextureBufferMaxSize)
            size = TextureBufferMaxSize;
        else
            size = sizeOfFile;

		fileRes = csi_hal_extern_flash_read((readAddr & 0xFFFFF800), size, myUpgradebuffer);;
			
		/*If read data successful. */
		if(fileRes == UPG_OK)
		{
			CheckResult = CRC_u32XorCheck(&checkCode, (uint32_t *)myUpgradebuffer, size/4);
		}
        
        sizeOfFile -= TextureBufferMaxSize;
        readAddr += TextureBufferMaxSize;       						       
	}
	
	if(CheckResult == 0)
	{
		return UPG_SUCCESS;
	}
	else
	{
		return UPG_FAILURE;
	}
}


/**
  * @name       UPG_i32WriteCpu1FlashFromBuffer
  * @brief      write CPU1 Flash from buffer
  * @note       
  * @param      buffer: pointer to buffer
  * @param      FlashStartAddress: start of address
  * @param      size: size of the code in the buffer
  * @param      size: size of the code in the buffer
  * @param      filetype: indicate the file type
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_i32WriteCpu1FlashFromBuffer(FIL* myfsrc, uint32_t FlashStartAddress, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype)
{
	uint32_t sizeOfFile = filesize;
	uint32_t i,j,numOfPage = 0;
	uint32_t count1;
	uint32_t NewProgressValue = 0;
//    uint32_t OldProgressValue = 0;
//	uint8_t *tembuffer = NULL;
	FRESULT fileRes;
	
	f_lseek(myfsrc, 0);
	
	numOfPage = sizeOfFile / bufsize;
	if(sizeOfFile % bufsize)
    	numOfPage++;
	
//	tembuffer = buffer;
    
	if((FlashStartAddress & StartAddrOfCodeFlash2Mask) == OffsetAddrOfDisplayPara)
	{
		uint32_t temp[2] = {DisplayParaFlag};
		temp[1] = sizeOfFile;
				
		/* Write product.bin flag and size to code flash */
		//INTERNAL_FLASH_WriteData(BANK_WRITE_START_ADDR_PRODUCT_PARA, 8, (uint8_t *)(temp));
		csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_PRODUCT_PARA, 8, (uint8_t *)(temp));
	}
	else if((FlashStartAddress & StartAddrOfCodeFlash2Mask) == OffsetAddrOfHWConfigMask)
	{
		uint32_t temp[2] = {HWConfigFlag};
		temp[1] = sizeOfFile;
		
		/* Write hwconfig.bin flag and size to code flash */
		//INTERNAL_FLASH_WriteData(BANK_WRITE_START_ADDR_HWCONFIG, 8, (uint8_t *)(temp));
		csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_HWCONFIG, 8, (uint8_t *)(temp));
	}
	
    /*Program Upgrade Flash*/
	for( i = 0; i < numOfPage; i++)
	{
        if(sizeOfFile >= bufsize)
        {
            count1 = bufsize;
        }
        else
        {
            count1 = sizeOfFile;
        }
        
		if(bufsize < filesize)
		{
			fileRes = f_read(myfsrc, buffer, bufsize, &count1);
			
			if(fileRes != FR_OK)
			{
				/* verify input CPU code fail */
				SYN_ERR_LOG(EM_enErrVerifyCodeCrcFail, EM_enErrLevelFatal);
				return UPG_ERR;
			}
		}
		
        /* Write a page data to flash. */
//		qspi_nor_write(FlashStartAddress + i*QSPI_NOR_BUFFER_LEN, count1, (unsigned char *)tembuffer);
//		INTERNAL_FLASH_WriteData(FlashStartAddress + i*QSPI_NOR_BUFFER_LEN, count1, (unsigned char *)tembuffer);
		csi_hal_internal_flash_write(FlashStartAddress + i * bufsize, count1, (unsigned char *)buffer);

		
        /* Flush ICache.*/
		//FlushIcache;
#if (UPG_VERIFY_READ_AFTER_WRITE)	        
        /* Read a page data from flash. */
//		qspi_nor_read(FlashStartAddress + i*QSPI_NOR_BUFFER_LEN, count1, (unsigned char *)myReadBuffer);
//		INTERNAL_FLASH_ReadData(FlashStartAddress + i*QSPI_NOR_BUFFER_LEN, count1, (unsigned char *)myReadBuffer);
		csi_hal_internal_flash_read(FlashStartAddress + i * bufsize, count1, (unsigned char *)myReadBuffer);
        
        /* Verify a page data.*/
		for (j=0; j< count1; j++)
		{
            /* If wrote data not equal read data.*/
			if (buffer[j] != myReadBuffer[j])
			{
                SYN_ERR_LOG(EM_enErrWrCPU0FlashFail, EM_enErrLevelFatal);
				return UPG_ERR;
			}
		}
#endif		
//        tembuffer += bufsize;
        sizeOfFile -= bufsize;
		
		/* get old progress value */
//        OldProgressValue = NewProgressValue;                              
    
        NewProgressValue = i * SCREEN_WIDTH / (numOfPage - 1);
        //if(NewProgressValue != OldProgressValue)  
        {                
            /* draw Progress Bar*/
            DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, filetype, optype, NewProgressValue);
        }
	}
	
	return UPG_OK;
}


int32_t UPG_i32WriteTextureFlashFromBuffer(FIL* myfsrc, uint32_t FlashStartAddress, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype)
{
	uint32_t writesize = 0;
    uint32_t numOfPage = 0;
	uint32_t i = 0, j = 0, count = 0;
	uint32_t OldProgressValue, NewProgressValue;
	FRESULT myres;
    int32_t res = UPG_OK;
            
	f_lseek(myfsrc, 0);
			
	numOfPage = (filesize + bufsize - 1) / bufsize;

	for(i = 0; i < numOfPage; i++)
    {                
        myres = f_read(myfsrc, buffer, bufsize, &count);
        if(myres != FR_OK)
        {
            res = UPG_ERR;
            break;
        }
		
        if(count == 0)
        {
            SYN_ERR_LOG(EM_enErrWriteTextureFlashFail, EM_enErrLevelSerious);
			return UPG_ERR;
        }
		
		if(count >= bufsize)
			writesize = bufsize;
		else
			writesize = count;
		
		UPG_u32WriteTextureFlash((FlashStartAddress & 0xFFFFF800), buffer, writesize);
#if (UPG_VERIFY_READ_AFTER_WRITE)		
		UPG_u32ReadTextureFlash((FlashStartAddress & 0xFFFFF800), myReadBuffer, writesize);
		
		for(j = 0; j < writesize; j++)
		{
			if(myReadBuffer[j] != buffer[j])
			{
				SYN_ERR_LOG(EM_enErrWriteTextureFlashFail, EM_enErrLevelSerious);
				return UPG_ERR;
			}
		}
#endif		
		FlashStartAddress += writesize;
		
		OldProgressValue = NewProgressValue;
		NewProgressValue = i * SCREEN_WIDTH / (numOfPage - 1);
		
		if(OldProgressValue != NewProgressValue)
		{	
			DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, filetype, optype, NewProgressValue);
		}
        
        //vTaskDelay(100 / portTICK_RATE_MS);
    }
	
	return res;
}


/**
  * @name       UPG_i32GetVersionOnFlash
  * @brief      Get version of CPU0/1 stored on Flash
  * @note       
  * @param      u8Index: file index to indicate which CPU version to get
  * @param      ppcVersion: [inout]pointer to get the version
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_i32GetVersionOnFlash(uint8_t u8Index, char** ppcVersion)
{   
    uint32_t infoOffset;
    
    if(NULL == ppcVersion)
    {
        /* Input parameter error */
        SYN_ERR_LOG(EM_enErrInParaInvalid, EM_enErrLevelSerious);
    }
    
    if(UPG_enUpgFileCpu0Bin == u8Index)
    {
        //memcpy(pcVersion, (const char*)CPU0_VERSION_ADDRESS, VERSION_LEN);
        //*ppcVersion = (char*)mCPU0APPVERSION;
    }
    else if(UPG_enUpgFileCpu1Bin == u8Index)
    {
        *ppcVersion = (char*)CPU1_VERSION_ADDRESS;
    }
    else if(UPG_enUpgFileTexture == u8Index)
    {
        infoOffset = *(uint32_t*)(TEXTURE_START_ADDRESS);
        /* SIM_GEN_ALL_VERSION_STORE_ADDR need add infoOffset */
        *ppcVersion = (char*)(SIM_GEN_ALL_VERSION_STORE_ADDR +infoOffset);
    }

    return UPG_OK;
}


/**
  * @name       UPG_i32GetVersionOnFlash
  * @brief      Get version of CPU0/1 stored on Flash
  * @note       
  * @param      u8Index: file index to indicate which CPU version to get
  * @param      ppcVersion: [inout]pointer to get the version
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
uint32_t GetVersionFromTextureFlash( uint8_t *pYTH, uint8_t *pIDE, uint8_t *pDEVICEID)
{
	uint32_t offset = 0;
	uint32_t align_size = 16*1024;
	char mCPU1APPVERSION[32];
	
	memcpy((uint8_t*)(mCPU1APPVERSION), (uint8_t*)CPU1_APP_VERSION_STORE_ADDR, 32);

	if((mCPU1APPVERSION[5] == '1') && ((mCPU1APPVERSION[6] == '7') || (mCPU1APPVERSION[6] == '8') || (mCPU1APPVERSION[6] == '9')))
	{
		offset = *(uint32_t *)TEXTURE_START_ADDRESS;
		if(offset % align_size != 0)
		{
			return UPG_FAILURE;
		}
		memcpy(pYTH, (uint8_t*)(SIM_GEN_ALL_VERSION_STORE_ADDR + offset), 32);
		memcpy(pIDE, (uint8_t*)(IDE_VERSION_STORE_ADDR + offset), 32);
		memcpy(pDEVICEID, (uint8_t*)(DEVICE_ID_STORE_ADDR + offset), 32);
	}
	else if((mCPU1APPVERSION[5] == '1') && (mCPU1APPVERSION[6] == '6'))
	{
		offset = 0;
		memcpy(pYTH, (uint8_t*)(SIM_GEN_ALL_VERSION_STROE_ADDR_FOR16 + offset), 32);
		memcpy(pIDE, (uint8_t*)(IDE_VERSION_STORE_ADDR_FOR16 + offset), 32);
		memcpy(pDEVICEID, (uint8_t*)(DEVICE_ID_STORE_ADDR_FOR16 + offset), 32);
	}
	else
	{
		return UPG_FAILURE;
	}

    return UPG_SUCCESS;
}


int32_t UPG_i32PrintVersionToFile(UPG_tstWorkBuf *pstWorkBuf)
{
    char PathName[32] = "1:version.txt";
    char tmp_buf[256];
    FRESULT res;
    UINT bw;
    FIL* fp;
    
    fp = pstWorkBuf->pFil;
    
    if((NULL == pstWorkBuf) || (NULL == fp))
    {
        /* Input parameter error */
        SYN_ERR_LOG(EM_enErrInParaInvalid, EM_enErrLevelSerious);
        return UPG_ERR;
    }
    
//    UPG_vCbReadVersion(0);
    
    if(pstWorkBuf->enComMode == UPG_enComMSD)
    {
        PathName[0] = '2';
    }

    /* Find the special file by name for version code. */
    res = f_open(fp, PathName, FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK)
    {
		char* cpu1_version = (char*)CPU1_APP_VERSION_STORE_ADDR; // cpu1 version string
		char* cpu1_compile_date = (char*)CPU1_APP_COMPILE_DATA_STORE_ADDR;
		char* cpu1_compile_time = (char*)CPU1_APP_COMPILE_TIME_STORE_ADDR;
		
		uint8_t SimGenAllVersion[32];
		uint8_t IdeVersion[32];
		uint8_t DeviceId[32];
	
		// Bootloader
		memset(tmp_buf, 0, sizeof(tmp_buf));
		snprintf(tmp_buf, sizeof(tmp_buf), "Bootloader:\r\n\t--Version:%s\r\n\t--Compile:%s,%s\r\n",
			BootVersion, Boot_compile_date, Boot_compile_time);
		f_write(fp, tmp_buf, strlen(tmp_buf), &bw);
	
		// cpu1 app
		memset(tmp_buf, 0, sizeof(tmp_buf));
		if ((cpu1_version[0] == 0x00) || (cpu1_version[1] == 0xFF))
		{
			f_write(fp, "CPU1: unkonwn\r\n", 14, &bw);
		}
		else
		{
			snprintf(tmp_buf, sizeof(tmp_buf), "CPU1:\r\n\t--Version:%s\r\n\t--Compile:%s,%s\r\n",
				cpu1_version, cpu1_compile_date, cpu1_compile_time);
			f_write(fp, tmp_buf, strlen(tmp_buf), &bw);
		}
	
		if(GetVersionFromTextureFlash(SimGenAllVersion, IdeVersion, DeviceId) != UPG_SUCCESS)
		{
			SYN_ERR_LOG(EM_enErrGetVerFromTextureFail, EM_enErrLevelSerious);
		}
		
		// SimGenAll
		memset(tmp_buf, 0, sizeof(tmp_buf));
		snprintf(tmp_buf, sizeof(tmp_buf), "SimGenAll:\r\n\t--Version:%s\r\n",SimGenAllVersion);
		f_write(fp, tmp_buf, strlen(tmp_buf), &bw);
  	     
		// IDE
		memset(tmp_buf, 0, sizeof(tmp_buf));
		snprintf(tmp_buf, sizeof(tmp_buf), "IDE:\r\n\t--Version:%s\r\n",IdeVersion);
		f_write(fp, tmp_buf, strlen(tmp_buf), &bw);
  	     
		// DeviceID
		memset(tmp_buf, 0, sizeof(tmp_buf));
		snprintf(tmp_buf, sizeof(tmp_buf), "DeviceID:\r\n\t--Version:%s\r\n",DeviceId);
		f_write(fp, tmp_buf, strlen(tmp_buf), &bw);
		
        /* Close file. */
        f_close(fp);
    }
    else
    {
        /* Open file "version.txt" fail */
        SYN_ERR_LOG(EM_enErrOpenVersionTxt, EM_enErrLevelSerious);
    }
    return UPG_OK;
}



/**
  * @name       UPG_bIfCpu1HasCode
  * @brief      check if CPU1 has already burnt code 
  * @note       
  * @param      none
  * @retval     The result: True or False
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
bool UPG_bIfCpu1HasCode(void)
{
    char* pAddress;

    pAddress = (char*)CPU1_VERSION_ADDRESS;


    if ('1'==pAddress[0] && '.'==pAddress[1])
    {
        return UPG_TRUE;
    }
    else
    {
        return UPG_FALSE;
    }
}


/**
  * @name       UPG_vCbReadVersion
  * @brief      callback function to copy the version to specified variable 
  * @note       
  * @param      pointer to the start address of buffer
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_vCbReadVersion(uint32_t* address)
{
//    memcpy(Boot1Version, (uint8_t*)((uint32_t)0x8001000), 64);
//    memcpy(Boot1CompileDate, (uint8_t*)((uint32_t)(0x8001000+64)), 32);
//    memcpy(Boot1CompileTime, (uint8_t*)((uint32_t)(0x8001000+96)), 32);
	
//    memcpy(AppVersion, (uint8_t*)((uint32_t)CPU1_VERSION_ADDRESS), 64);
//    memcpy(AppCompileDate, (uint8_t*)((uint32_t)(CPU1_VERSION_ADDRESS+64)), 32);
//    memcpy(AppCompileTime, (uint8_t*)((uint32_t)(CPU1_VERSION_ADDRESS+96)), 32);   

//    memcpy(DeviceId, (uint8_t*)((uint32_t)address+192), 32);
//    
//    UPG__bGetVersionFromCpu1Vaild = UPG_TRUE;
}


/**
  * @name       UPG_vMain
  * @brief      main function of Upgrade module 
  * @note       
  * @param      void
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_vMain(void)
{
    UPG_SM__vMainSmProcess();
}



/**
  * @name       UPG_vInit
  * @brief      Initialization function of Upgrade module 
  * @note       
  * @param      void
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_vInit(void)
{
    UPG_SM__vInit();
}

void UpgradeMain_Task(void* pvParameters)
{
	SRV_TIMER_vInit();
    EM_vInit();

    UPG_vInit();
	
	for(;;)
	{
		UPG_vMain();
		vTaskDelay(1);
	}
}

xTaskHandle     UpgradeMainTaskHandle;
void UPG_Main_vInit(void)
{
    portBASE_TYPE xReturn;
	xReturn = xTaskCreate(UpgradeMain_Task, "UpgradeMainTask", configMINIMAL_STACK_SIZE * 3, NULL, 2, &UpgradeMainTaskHandle);
	if (xReturn != pdTRUE)
	{
		return;
	}
}



