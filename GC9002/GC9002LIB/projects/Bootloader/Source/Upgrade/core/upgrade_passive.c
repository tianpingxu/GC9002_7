////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2022/11/17
// File Name:     upgrade_it.c
// Project Name:  CPU0_Bootloader_Redesign
// 
// Revision:
// Revision 1.0.0 - File Created 2022/11/17 by Zhu Jiahai
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "gc90x_upgrade.h"
//#include "qspi_nor.h"
//#include "LoadFiles_SDCard.h"
#include "upgrade_c1.h"
#include "em_c1.h"
#include "DrawProgramBar.h"
#include "csi_hal_driver_flash.h"


extern UPG_tstWorkBuf *UPG_pstWorkBuf;
extern uint8_t myUpgradebuffer[TextureBufferMaxSize];
extern uint8_t myReadBuffer[TextureBufferMaxSize];
extern uint32_t SCREEN_WIDTH;
extern uint32_t SCREEN_HEIGHT;


int32_t UPG_PAS__i32VerifyFlash(UPG_tstWorkBuf *pstWorkBuf);


/**
  * @name       UPG_PAS__vDrawProgressBar
  * @brief      Drawing progress bar 
  * @note        
  * @param      pstWorkBuf: pointer to work buffer
  * @param      optype: operation type
  * @param      value: progress bar value to show
  * @retval     n.a.
  * @author     Zhu Jiahai
  * @date       2023-04-20
  */
void  UPG_PAS__vDrawProgressBar(UPG_tstWorkBuf *pstWorkBuf, uint32_t optype, uint32_t value)
{
    uint8_t fileIndex;

    fileIndex = pstWorkBuf->u8FileIndex;
    DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, fileIndex, optype, value);
}


/**
  * @name       UPG_PAS__vWriteFlagAndSize
  * @brief      specially treatment for Product.bin(add Flag and size to the first 8 Bytes)
  * @note        
  * @param      pstWorkBuf: pointer to work buffer
  * @retval     n.a.
  * @author     Zhu Jiahai
  * @date       2023-04-17
  */
void  UPG_PAS__vWriteFlagAndSize(UPG_tstWorkBuf *pstWorkBuf)
{
    uint32_t temp[2];
    uint32_t addr;
    
    addr = (uint32_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress;
    
    if((addr & StartAddrOfCodeFlash2Mask) == OffsetAddrOfDisplayPara)
	{
		temp[0] = DisplayParaFlag;
		temp[1] = pstWorkBuf->stRtConfig.u32FileSize;
				
		/* Write product.bin flag and size to code flash */
		//INTERNAL_FLASH_WriteData(BANK_WRITE_START_ADDR_PRODUCT_PARA, 8, (uint8_t *)(temp));
		csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_PRODUCT_PARA, 8, (uint8_t *)(temp));
	}
	else if((addr & StartAddrOfCodeFlash2Mask) == OffsetAddrOfHWConfigMask)
	{
		temp[0] = HWConfigFlag;
		temp[1] =  pstWorkBuf->stRtConfig.u32FileSize;
		
		/* Write hwconfig.bin flag and size to code flash */
		//INTERNAL_FLASH_WriteData(BANK_WRITE_START_ADDR_HWCONFIG, 8, (uint8_t *)(temp));
		csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_HWCONFIG, 8, (uint8_t *)(temp));
	}
}

/**
  * @name       UPG_PAS__vRtConfigInit
  * @brief      Initialize the runtime configure
  * @note        
  * @param      fileSize: size of the file
  * @param      pstWorkBuf: pointer to work buffer
  * @retval     n.a.
  * @author     Zhu Jiahai
  * @date       2022-11-17
  */
void UPG_PAS__vRtConfigInit(uint32_t fileSize, UPG_tstWorkBuf *pstWorkBuf)
{

    memset(&pstWorkBuf->stRtConfig, 0, sizeof(UPG_tstRuntimeConfig));
    pstWorkBuf->stRtConfig.u32FileSize = fileSize;

    /* pu8RecvBufStartAddress and  u32RecvBufTotalSize may adjust depend on different Upgrade file */
    if(pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagFixedRecvBufAddress)
    {
        pstWorkBuf->stRtConfig.pu8RecvBufStartAddress = pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8RecvBufStartAddress;
        pstWorkBuf->stRtConfig.u32RecvBufTotalSize = pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].u32RecvBufSize;
    }
    else
    {
        pstWorkBuf->stRtConfig.pu8RecvBufStartAddress = myUpgradebuffer;
        pstWorkBuf->stRtConfig.u32RecvBufTotalSize = TextureBufferMaxSize;
    }
    pstWorkBuf->stRtConfig.pu8RecvBufCurAddress = pstWorkBuf->stRtConfig.pu8RecvBufStartAddress;

#if 0  // no dual core
    /* The passed address of CPU1 has handled specially, so need to recover it */
    if(OK == memcmp(pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8FileName, "cpu1.bin", strlen("cpu1.bin")))
    {
        pstWorkBuf->stRtConfig.pu8FlashWrCurAddress = (u8*)((uint32_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress & (~(uint32_t)CPU1_ADDR_FLAG));
    }
    else
#endif
    {
        /* Special deal: if upgrade file is bootloader, write Flash Address will be variable depend on Flash size */
        if(UPG_enUpgFileBootloader == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
        {
            pstWorkBuf->stRtConfig.pu8FlashWrCurAddress = (uint8_t*)pstWorkBuf->u32BootloaderAdaptedFlashAddr;
        }
        else
        {
            pstWorkBuf->stRtConfig.pu8FlashWrCurAddress = pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress;
        }
    }
    

    /* clear buffer */
    if(pstWorkBuf->stRtConfig.pu8RecvBufStartAddress)
    {
        memset(pstWorkBuf->stRtConfig.pu8RecvBufStartAddress, 0, pstWorkBuf->stRtConfig.u32RecvBufTotalSize);
    }

    if( pstWorkBuf->stRtConfig.u32RecvBufTotalSize >= pstWorkBuf->stRtConfig.u32FileSize)
    {
        pstWorkBuf->stRtConfig.bFlagBufEnough = UPG_TRUE;
    }
}



/**
  * @name       UPG_PAS__i32EraseFlash
  * @brief      erasing Flash function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2022-11-20
  */
int32_t UPG_PAS__i32EraseFlash(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  res = OK;
	int32_t  i32Ret = UPG_OK;
    uint8_t u8Index;
    uint32_t u32Address;

    /* Erase CPU0 Flash, no need wait UPG_enUpgStepWaitEraseDone */
    if(UPG_enUpgFlashCpu0Nor == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFlashType)
    {
        if(UPG_OK == UPG_i32EraseCpu0Flash((u32)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress, 
            pstWorkBuf->stRtConfig.u32FileSize))
        {
            /* Do nothing */
        
        }
        else
        {
            // Log error
            SYN_ERR_LOG(EM_enErrEraseCPU0FlashFail, EM_enErrLevelSerious);
            res = EM_enErrEraseCPU0FlashFail;
        }
    }
    else
    {
		
		if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
        {
			i32Ret = UPG_u32EraseTextureFlash((uint32_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress, 
            pstWorkBuf->stRtConfig.u32FileSize);
        }
        else
        {
            /* Special deal: if upgrade file is bootloader, write Flash Address will be variable depend on Flash size */
            if(UPG_enUpgFileBootloader == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
            {
                u32Address = pstWorkBuf->u32BootloaderAdaptedFlashAddr;
            }
            else
            {
                u32Address = (uint32_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress;
            }
			i32Ret = UPG_i32EraseCpu1Flash(u32Address, pstWorkBuf->stRtConfig.u32FileSize);
        }
		
		if(i32Ret == UPG_OK)
        {
            
        }
        else
        {
			/* Erase Flash fail, give a chance to try again */
            // Log error
			if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
			{
				SYN_ERR_LOG(EM_enErrEraseTextureFlashFail, EM_enErrLevelFatal);
				res = EM_enErrEraseTextureFlashFail;
			}
			else
			{
				SYN_ERR_LOG(EM_enErrEraseCPU1FlashFail, EM_enErrLevelFatal);
				res = EM_enErrEraseCPU1FlashFail;
			}
        }
	}


    return res;
}


/**
  * @name       UPG_PAS_i32SetUpgFileAndSize
  * @brief      Internal interface, handle with file's saving address and size 
  * @note        
  * @param      pAddr: pointer to buffer
  * @param      size: size of the file
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2022-11-17
  */
int32_t UPG_PAS__i32SetUpgFileAndSize(uint8_t * pAddr, uint32_t size, UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t res = OK;
    int32_t i;

    for(i = 0; i < pstWorkBuf->u8FileCount; i++)
    {
        if(pAddr == pstWorkBuf->pstPbConfig[i].pu8UpgFlashStartAdress)
        {
            pstWorkBuf->u8FileIndex = i;
            UPG_PAS__vRtConfigInit(size, pstWorkBuf);
            
            /* maintain size of cpu1.bin */
            if(UPG_enUpgFileCpu1Bin == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
            {
                UPG_vKeepCpu1BinSize(pstWorkBuf->stRtConfig.u32FileSize);
            }
            else if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
            {
                /* Check size of Texture, if err will log */
                if(size > UPG_u32GetMaxTextureSize())
                {
                    SYN_ERR_LOG(EM_enErrTextureSizeOverrun, EM_enErrLevelFatal);
                }
                UPG_ASSERT(size <= UPG_u32GetMaxTextureSize());
            }
            else
            {
            }
            
            break;
        }
    }

    /* Err: no addr be found, it's invalid */
    if(i >= pstWorkBuf->u8FileCount)
    {
        /* Log error, configration err */
        SYN_ERR_LOG(EM_enErrInAddrInvalid, EM_enErrLevelWarning);
        res = EM_enErrInAddrInvalid;
        return res;
    }

    /* Init receive buffer */
    
    /* Call Erase Interface */
    res = UPG_PAS__i32EraseFlash(pstWorkBuf);

    if(OK == res)
    {
        UPG_PAS__vWriteFlagAndSize(pstWorkBuf);
    }
    
    return res;
}


/**
  * @name       UPG_PAS__i32WriteCpu1Flash
  * @brief      Internal interface for writing flash on CPU0
  * @note       
  * @param      pAddr: pointer to buffer
  * @param      size: size of the file
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
#if 0 
int32_t UPG_PAS__i32WriteCpu0Flash(uint8_t * pAddr, uint32_t size, UPG_tstWorkBuf *pstWorkBuf)
{
    int res = OK;
   
    res = UPG_i32WriteCpu0FlashFromBuffer(pAddr, 
            (uint32_t)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress,
            size,
            (uint8_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType, WRITEFILE);
    if(OK == res)
    {
        /* Adjust parameter */
        pstWorkBuf->stRtConfig.pu8FlashWrCurAddress += size;
        pstWorkBuf->stRtConfig.u32FlashWroteSize += size;
    }
    else
    {
        /* Log error, configration err */
        SYN_ERR_LOG(EM_enErrWrCPU0FlashFail, EM_enErrLevelSerious);
    }


    return res;
}
#endif


/**
  * @name       UPG_PAS__i32WriteCpu1Flash
  * @brief      Internal interface for writing flash on CPU1
  * @note       
  * @param      pAddr: pointer to buffer
  * @param      size: size of the file
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
int32_t UPG_PAS__i32WriteCpu1Flash(uint8_t * pAddr, uint32_t size, UPG_tstWorkBuf *pstWorkBuf)
{
    int res = OK;
    uint32_t u32SendSize;
    uint32_t u32WriteState;
    uint32_t u32Value;
    
//    bool bWrTimeout;
//    uint8_t u8TryAgain = 0;
    int i;
        
    pstWorkBuf->stRtConfig.u32TmpWrLeftSize = size;
    pstWorkBuf->stRtConfig.u32TmpWrSize = 0;
    

    while(pstWorkBuf->stRtConfig.u32TmpWrLeftSize > 0)
    {
//        bWrTimeout = FALSE_0;
//        u32SendSize = MIN(BUFFERMAXSIZE, pstWorkBuf->stRtConfig.u32TmpWrLeftSize);
		u32SendSize = pstWorkBuf->stRtConfig.u32TmpWrLeftSize;

        u32Value = (uint32_t)(((uint64_t)SCREEN_WIDTH * (pstWorkBuf->stRtConfig.u32FlashWroteSize + pstWorkBuf->stRtConfig.u32TmpWrLeftSize))/ pstWorkBuf->stRtConfig.u32FileSize);
        UPG_PAS__vDrawProgressBar(pstWorkBuf, WRITEFILE, u32Value);
        
        /* Write a frame data to cpu1. */
//        res = UPG_i32SendDataByShareMemory((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress, u32SendSize, 
//        pstWorkBuf->stRtConfig.pu8RecvBufStartAddress + pstWorkBuf->stRtConfig.u32TmpWrSize);
		if(((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress >= CPU1_START_ADDRESS) && ((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress < TEXTURE_START_ADDRESS))
        {
			csi_hal_internal_flash_write((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress, u32SendSize, pstWorkBuf->stRtConfig.pu8RecvBufStartAddress + pstWorkBuf->stRtConfig.u32TmpWrSize);
#if (UPG_VERIFY_READ_AFTER_WRITE)	
            csi_hal_internal_flash_read((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress, u32SendSize, myReadBuffer);
#endif
        }
		else if(((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress >= TEXTURE_START_ADDRESS) && ((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress < TEXTURE_START_ADDRESS + UPG_u32GetMaxTextureSize()))
		{
			csi_hal_extern_flash_write((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress, u32SendSize, pstWorkBuf->stRtConfig.pu8RecvBufStartAddress + pstWorkBuf->stRtConfig.u32TmpWrSize);
#if (UPG_VERIFY_READ_AFTER_WRITE)
            csi_hal_extern_flash_read((u32)pstWorkBuf->stRtConfig.pu8FlashWrCurAddress, u32SendSize, myReadBuffer);
#endif
		}
		else
		{
			/* Log err: share Memory fail */
            SYN_ERR_LOG(EM_enErrInAddrInvalid, EM_enErrLevelSerious);
			res = UPG_ERR;
		}
		
        if(OK == res) 
        {
#if (UPG_VERIFY_READ_AFTER_WRITE)
			for (i = 0; i < u32SendSize; i++)
			{
				/* If wrote data not equal read data.*/
				if (pstWorkBuf->stRtConfig.pu8FlashWrCurAddress[i] != myReadBuffer[i])
				{
					SYN_ERR_LOG(EM_enErrWrCPU0FlashFail, EM_enErrLevelFatal);
					return UPG_ERR;
				}
			}
#endif
			/* parameter adjust */
			pstWorkBuf->stRtConfig.u32FlashWroteSize += u32SendSize;
			pstWorkBuf->stRtConfig.pu8FlashWrCurAddress += u32SendSize;
			pstWorkBuf->stRtConfig.u32TmpWrSize += u32SendSize;

			pstWorkBuf->stRtConfig.u32TmpWrLeftSize -= u32SendSize;

        }
        
    }

    return res;
}


/**
  * @name       UPG_PAS__i32WriteFlash
  * @brief      Internal interface for writing flash 
  * @note       
  * @param      pAddr: pointer to buffer
  * @param      size: size of the file
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
int32_t UPG_PAS__i32WriteFlash(uint8_t * pAddr, uint32_t size, UPG_tstWorkBuf *pstWorkBuf)
{
    int res = OK;
  
    if(UPG_enUpgFlashCpu0Nor == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFlashType)
    {
        /* Flash on CPU0, use cpu1 interface for 9002. */
        res = UPG_PAS__i32WriteCpu1Flash(pAddr, size, pstWorkBuf);
        
    }
    else
    {
        /* Flash on CPU1, need start timer */
        if(pstWorkBuf->u8FileIndex != UPG_enUpgFileEraseStoreSpace)
        {
            res = UPG_PAS__i32WriteCpu1Flash(pAddr, size, pstWorkBuf);
        }
        else
        {
            res = OK;
        }
    }

    return res;
}

/**
  * @name       UPG_PAS__i32GetData
  * @brief      Internal interface for saving input data
  * @note       when all data of the special file has received, it will burn flash(if the received buffer is full, it also burn flash)
  * @param      pAddr: pointer to buffer
  * @param      size: size of the file
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
int32_t UPG_PAS__i32GetData(uint8_t * pAddr, uint32_t size, UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t res = OK;
    uint32_t bufferLeftSize;
    uint32_t restSize;
    int i;
      
    bufferLeftSize = pstWorkBuf->stRtConfig.u32RecvBufTotalSize + pstWorkBuf->stRtConfig.u32FlashWroteSize - pstWorkBuf->stRtConfig.u32FileReadSize;
    
    /* RevBuf big enough or Left RevBuf is greater than receive size */
    if((UPG_TRUE == pstWorkBuf->stRtConfig.bFlagBufEnough) || (bufferLeftSize > size))  
    {
        memcpy((uint8_t *)pstWorkBuf->stRtConfig.pu8RecvBufCurAddress, pAddr, size);
        /* Adjust runtime parameter */
        pstWorkBuf->stRtConfig.pu8RecvBufCurAddress += size;
        pstWorkBuf->stRtConfig.u32FileReadSize += size;
    }
    else
    {
        memcpy((uint8_t *)pstWorkBuf->stRtConfig.pu8RecvBufCurAddress, pAddr, bufferLeftSize);
        /* Adjust runtime parameter */
        //pstWorkBuf->stRtConfig.pu8RecvBufCurAddress += bufferLeftSize;
        pstWorkBuf->stRtConfig.u32FileReadSize += bufferLeftSize;
        /* pu8RecvBufCurAddress need point to orignal position */
        pstWorkBuf->stRtConfig.pu8RecvBufCurAddress = pstWorkBuf->stRtConfig.pu8RecvBufStartAddress;

        /* Write Flash: size is the whole RecBuffer */
        res = UPG_PAS__i32WriteFlash(pstWorkBuf->stRtConfig.pu8FlashWrCurAddress, pstWorkBuf->stRtConfig.u32RecvBufTotalSize, pstWorkBuf);
        if(OK == res)
        {
            restSize = size - bufferLeftSize;
        }
        else
        {
            /* Log error, configration err */
            SYN_ERR_LOG(EM_enErrWriteFlashFail, EM_enErrLevelWarning);
            return res;
        }
        if(restSize > 0)
        {
            memcpy((uint8_t *)pstWorkBuf->stRtConfig.pu8RecvBufCurAddress, pAddr, restSize);
            pstWorkBuf->stRtConfig.pu8RecvBufCurAddress += restSize;
            pstWorkBuf->stRtConfig.u32FileReadSize += restSize;
        }
        
    }

    if(pstWorkBuf->stRtConfig.u32FileReadSize == pstWorkBuf->stRtConfig.u32FileSize)
    {
        /* Before Writing data to Flash, Verify data in advance.  Beware: only appropriate for File size smaller than receive Buffer, otherwise will cause error */
        res = UPG_PAS__i32VerifyFlash(pstWorkBuf);
        
        if((OK == res) && (pstWorkBuf->stRtConfig.u32FileReadSize > pstWorkBuf->stRtConfig.u32FlashWroteSize))
        {
            /* Write Flash: size is the left un-Wrote */
            res = UPG_PAS__i32WriteFlash(pstWorkBuf->stRtConfig.pu8FlashWrCurAddress, pstWorkBuf->stRtConfig.u32FileReadSize - pstWorkBuf->stRtConfig.u32FlashWroteSize, pstWorkBuf);

            if(OK == res)
            {
                UPG_PAS__vDrawProgressBar(pstWorkBuf, UPGRADE_OK, 0);
            }
        }

        /* If write isbootloader.bin successfully, set flag to indicate */
        if((OK == res) && (UPG_enUpgFileBootloader == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType))
        {
            UPG_BTLD_vSetUpgFlag(UPG_TRUE);
        }
    }

    return res;
}


/**
  * @name       UPG_PAS__i32VerifyFlash
  * @brief      verify Flash internal interface
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
int32_t UPG_PAS__i32VerifyFlash(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  res = OK;

    /* Write CPU0 Flash, no need wait UPG_enUpgStepWaitWriteDone */
    if(UPG_TRUE == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagVerifyStoredFile)
    {
        if((UPG_enVerifyInputCrc == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enVerifyInputType)
            && (pstWorkBuf->stRtConfig.u32RecvBufTotalSize >= pstWorkBuf->stRtConfig.u32FileSize))
        /* Note: currently only CPU0.bin/CPU1.bin need to verify flash by CPU0 bootloader */
        {
            res = UPG_i32VerifyCPUCodeFromBuffer(NULL, pstWorkBuf->stRtConfig.pu8RecvBufStartAddress, 
                pstWorkBuf->stRtConfig.u32RecvBufTotalSize, pstWorkBuf->stRtConfig.u32FileSize,
                pstWorkBuf->u8FileIndex, VERIFY_FILE);
            if(res != OK)
            {

                /* Verify Flash fail, Jump out of Upgrading,  */
                // Log err
                
                SYN_ERR_LOG(EM_enErrVerifyFlashFail, EM_enErrLevelFatal);
                res = EM_enErrVerifyFlashFail;
//                pstWorkBuf->u8ErrState = ERR;   // to do, by Zhu Jiahai
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* currently no requirement */
            // do nothing
        }
  
    }
	

    return res;
}

/**
  * @name       UPG_PAS_i32GetData
  * @brief      Interface for saving input data
  * @note       when all data of the special file has received, it will burn flash(if the received buffer is full, it also burn flash)
  * @param      pAddr: pointer to buffer
  * @param      size: size of the file
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
int32_t UPG_PAS_i32GetData(uint8_t * pAddr, uint32_t size)
{
    int32_t res;

    if(NULL == pAddr)
    {
        /* Log error, input parameter err */
        SYN_ERR_LOG(EM_enErrInParaInvalid, EM_enErrLevelWarning);
        return EM_enErrInParaInvalid;
    }

    res = UPG_PAS__i32GetData(pAddr, size, UPG_pstWorkBuf);
    return res;
}


/**
  * @name       UPG_PAS_i32SetUpgFileAndSize
  * @brief      Indicate the upcoming file's saving address and size 
  * @note        
  * @param      pAddr: pointer to buffer
  * @param      size: size of the file
  * @retval     The result: OK or ERR.
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
int32_t UPG_PAS_i32SetUpgFileAndSize(uint8_t * pAddr, uint32_t size)
{
    return UPG_PAS__i32SetUpgFileAndSize(pAddr, size, UPG_pstWorkBuf);

}


/**
  * @name       UPG_PAS_i32VerifyFlash
  * @brief      verify Flash interface
  * @note       
  * @param      N.A.
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2022-11-21
  */
int32_t UPG_PAS_i32VerifyFlash(void)
{
    //return UPG_PAS__i32VerifyFlash(UPG_pstWorkBuf);
    return OK;
}


