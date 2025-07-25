////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2021/07/05
// File Name:     upgrade_subsm.c
// Project Name:  CPU0_Bootloader_Redesign
// 
// Revision:
// Revision 1.0.0 - File Created 2021/06/24 by Zhu Jiahai
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "upgrade_c1.h"
#include "upgrade_subsm.h"

#include "DrawProgramBar.h"

#include "ParsePara.h"

#include "srv_timer_c1.h"
#include "em_ct.h"


#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"
#include "DiagMgr.h"

#include "FreeRTOS.h"
#include "task.h"

#include "SPI_SDCard.h"




extern uint8_t myUpgradebuffer[TextureBufferMaxSize];

extern uint32_t SCREEN_WIDTH;
extern uint32_t SCREEN_HEIGHT;


extern SPI_TypeDef * UPG_SdSpi;

bool Version_Same = FALSE_0;





/**
  * @name       UPG_SUBSM__i32UpgStepInit
  * @brief      sub state machine initialization step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepInit(UPG_tstWorkBuf *pstWorkBuf)
{
    uint8_t* pFileName;
	uint8_t PathName[8]={"1:"};
    uint8_t aFileName[255];
    int32_t  i32Ret = UPG_OK;
    bool bFlagNeedUpg;
    
    if(pstWorkBuf->enComMode == UPG_enComMSD)
    {
        PathName[0] = '2';
    }

    /* have File System (SD-Card) */
    if((UPG_TRUE == pstWorkBuf->bFlagFfOperation) && (pstWorkBuf->u8FileIndex < pstWorkBuf->u8FileCount))
    {
        pFileName = pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8FileName;
		memset(aFileName, 0, sizeof(aFileName));
		strncat((char *)aFileName, (const char*)PathName, 240);
		strncat((char *)aFileName, (const char*)pFileName, 240);
        if(aFileName != NULL)
        {
			if(FR_OK == f_open(pstWorkBuf->pFil, (const char*)aFileName, FA_OPEN_EXISTING | FA_READ))
			{
                /* Judge if bootloader.bin file exist */
                if(UPG_enUpgFileBootloader == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
                {
                    pstWorkBuf->bFlagBootloaderExist = UPG_TRUE;
                }

                /* Self-Upgrade operation finished */
                if(UPG_BTLD_bIfSelfUpgOpFinished())
                {
                    /* Flag of upgrade with bootloader.bin is True, quit upgrading , is False, need to upgrade */
                    if(UPG_TRUE == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagUpgWithSelfUpg)
                    {
                        bFlagNeedUpg = UPG_FALSE;
                    }
                    else
                    {
                        bFlagNeedUpg = UPG_TRUE;
                    }
                }
                /* Self-Upgrade operation not finished */
                else
                {
                    if(pstWorkBuf->bFlagBootloaderExist)
                    {
                        if(UPG_TRUE == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagUpgWithSelfUpg)
                        {
                            bFlagNeedUpg = UPG_TRUE;
                        }
                        else
                        {
                            bFlagNeedUpg = UPG_FALSE;
                        }
                    }
                    else
                    {
                        bFlagNeedUpg = UPG_TRUE;
                    }
                }

                if(UPG_TRUE == bFlagNeedUpg)
                {
                    if((pFileName[0] == 'E') && (pFileName[1] == 'r') && (pFileName[2] == 'a') && (pFileName[3] == 's'))
    				{
    					/* File exist, maintain variable in pstWorkBuf */
    					pstWorkBuf->stRtConfig.u32FileSize = CodeFlashSectorSize * 2;
    					
    					/* enter next step: UPG_enUpgStepEraseFlash */
    					pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEraseFlash;
    				}
    				else
    				{
    					/* File exist, maintain variable in pstWorkBuf */
    					pstWorkBuf->stRtConfig.u32FileSize = f_size(pstWorkBuf->pFil);
                        if(pstWorkBuf->stRtConfig.u32FileSize < 4)
                        {
                            i32Ret = UPG_ERR;
                            SYN_ERR_LOG(EM_enErrUpgFileSizeInvalid, EM_enErrLevelFatal);
                            pstWorkBuf->u8ErrState = UPG_ERR;
                            return i32Ret;
                        }
                        
    					pstWorkBuf->stRtConfig.u32FileReadSize = 0;
    					pstWorkBuf->stRtConfig.u32FlashWroteSize = 0;
    					pstWorkBuf->stRtConfig.u32TmpWrLeftSize = 0;
    			
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

                        /* Special deal: if upgrade file is bootloader, write Flash Address will be variable depend on Flash size */
                        if(UPG_enUpgFileBootloader == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
                        {
                             pstWorkBuf->stRtConfig.pu8FlashWrCurAddress = (uint8_t*)pstWorkBuf->u32BootloaderAdaptedFlashAddr;
                        }
                        else
                        {
                            pstWorkBuf->stRtConfig.pu8FlashWrCurAddress = pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress;
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
    					else
    					{
    						pstWorkBuf->stRtConfig.bFlagBufEnough = UPG_FALSE;
    					}
    					
    					/* enter next step: UPG_enUpgStepCheckVersion */
    					pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepCheckVersion;

                        /* maintain size of cpu1.bin */
                        if(UPG_enUpgFileCpu1Bin == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
                        {
                            UPG_vKeepCpu1BinSize(pstWorkBuf->stRtConfig.u32FileSize);
                        }
                        else if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
                        {
                            /* Check size of Texture, if err will log */
                            if(pstWorkBuf->stRtConfig.u32FileSize > UPG_u32GetMaxTextureSize())
                            {
                                i32Ret = UPG_ERR;
                                SYN_ERR_LOG(EM_enErrTextureSizeOverrun, EM_enErrLevelFatal);
                                pstWorkBuf->u8ErrState = UPG_ERR;
                            }
                            UPG_ASSERT(pstWorkBuf->stRtConfig.u32FileSize <= UPG_u32GetMaxTextureSize());
                        }
                        else
                        {
                        }
    				}
                }
                else
                {
                    pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEnd;
                }
			}
			else
			{
				/* File doesn't exist on SD-Card, try other files, enter next step: UPG_enUpgStepEnd */
				pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEnd;
			}
        }
        else
        {
            /* Log error, configration err */
            SYN_ERR_LOG(EM_enErrConfigParaInvalid, EM_enErrLevelWarning);
        }

    }

    return i32Ret;
}



/**
  * @name       UPG_SUBSM__i32UpgStepCheckVersion
  * @brief      sub state machine check version step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepCheckVersion(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;
    char* pVersion;
    uint32_t u32Offset = 0;
    uint32_t u32ByteRead;
    char acVersion[VERSION_LEN];
    
    /* have File System (SD-Card) */
    if(UPG_TRUE == pstWorkBuf->bFlagFfOperation)
    {
		if(pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagCheckVersion)
		{
			/* check version */
			if(UPG_OK != UPG_i32GetVersionOnFlash(pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType, &pVersion)) 
			{
				if(pstWorkBuf->u16GetVersionTryTimes < UPG_GET_VERSION_TRY_TIMES)
				{
					pstWorkBuf->u16GetVersionTryTimes++;
					
					/* Log has done at UPG_i32GetVersionOnFlash() */
					return UPG_ERR;
				}
				else
				{
					/* Get version err */
					
				}
			}
	
	
			if(UPG_enUpgFileCpu0Bin == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
			{
				//u32Offset = CPU0_VERSION_OFFSET;
			}
			else if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
			{
                /* pVersion saved the value is SIM_GEN_ALL_VERSION_STORE_ADDR +infoOffset, sub TEXTURE_START_ADDRESS equal (92 + infoOffset) */
				u32Offset = (uint32_t)pVersion - TEXTURE_START_ADDRESS;
			}
			else if(UPG_enUpgFileCpu1Bin == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
			{
				u32Offset = CPU1_VERSION_OFFSET;
			}
	
			f_lseek(pstWorkBuf->pFil, u32Offset);
			
			memset(acVersion, 0, VERSION_LEN);
			
			if(FR_OK == f_read(pstWorkBuf->pFil, 
				acVersion, 
				VERSION_LEN, 
				&u32ByteRead))
			{
				/* Read version success, compare it with version on Flash */
				if(UPG_OK == memcmp(acVersion, pVersion, VERSION_LEN))
				{
					/* The same version, skip this file's upgrade */
                    SYN_ERR_LOG(EM_enErrUpgradeSameVersion, EM_enErrLevelInfo);
					pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEnd;
					
				}
				else
				{
					/* The different version, enter next step: UPG_enUpgStepGetData */
					pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepGetData;
				}
			}
			else
			{
				/* Read version fail */
				i32Ret = UPG_ERR;
				SYN_ERR_LOG(EM_enErrReadVerFromFileFail, EM_enErrLevelFatal);
				pstWorkBuf->u8ErrState = UPG_ERR;
			}
		}
		else
		{
			/* enter next step: UPG_enUpgStepGetData */
			pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepGetData;
		}
	
		/* It's important to reset the file position to original */
		f_lseek(pstWorkBuf->pFil, 0);
        
    }

    return i32Ret;
}


/**
  * @name       UPG_SUBSM__i32UpgStepGetData
  * @brief      sub state machine getting data step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepGetData(UPG_tstWorkBuf *pstWorkBuf)
{
    uint32_t u32ByteRead;
    int32_t  i32Ret = UPG_OK;
    
    /* have File System (SD-Card) */
    if(UPG_TRUE == pstWorkBuf->bFlagFfOperation)
    {
        if(pstWorkBuf->stRtConfig.bFlagBufEnough)
        {
            /* Read all data to buffer */
            if(FR_OK == f_read(pstWorkBuf->pFil, 
                pstWorkBuf->stRtConfig.pu8RecvBufCurAddress, 
                pstWorkBuf->stRtConfig.u32FileSize , 
                &u32ByteRead))
            {
                pstWorkBuf->stRtConfig.pu8RecvBufCurAddress += u32ByteRead;
                pstWorkBuf->stRtConfig.u32FileReadSize +=u32ByteRead;
                
                if(pstWorkBuf->stRtConfig.u32FileSize == pstWorkBuf->stRtConfig.u32FileReadSize)
                {
                    pstWorkBuf->stRtConfig.bFlagReadFinished = UPG_TRUE;
                    
                    /* enter next step(depend on configuration): UPG_enUpgStepVerifyData or  UPG_enUpgStepEraseFlash */
                    if(pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagVerifyInputFile)
                    {
                        pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepVerifyData;
                    }
                    else
                    {
                        pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEraseFlash;
                    }
                }
            }
            else
            {
                /* Read file failed (C20) */
                i32Ret = UPG_ERR;
                SYN_ERR_LOG(EM_enErrReadDataFromFileFail, EM_enErrLevelFatal);
                
                pstWorkBuf->u8ErrState = UPG_ERR;
            }
        }
        else
        {
            /* If buffer is small, getting the data to buffer is unnecessary */
            /* enter next step(depend on configuration): UPG_enUpgStepVerifyData or  UPG_enUpgStepEraseFlash */
            if(pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagVerifyInputFile)
            {
                pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepVerifyData;
            }
            else
            {
                pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEraseFlash;
            }
        }
    }
    
    return i32Ret;
}



/**
  * @name       UPG_SUBSM__i32UpgStepVerifyData
  * @brief      sub state machine verifying data step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepVerifyData(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;
	
	/* have File System (SD-Card) and upgrade file is cpu0.bin/cpu1.bin, call the special verificaiton function */
	if((pstWorkBuf->bFlagFfOperation) && (UPG_enUpgFileCpu1Bin == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType))
    {
        i32Ret = UPG_i32VerifyCPUCodeFromBuffer(pstWorkBuf->pFil, pstWorkBuf->stRtConfig.pu8RecvBufStartAddress, pstWorkBuf->stRtConfig.u32RecvBufTotalSize,
            pstWorkBuf->stRtConfig.u32FileSize, (uint8_t)pstWorkBuf->u8FileIndex, VERIFY_FILE);
    }
    /* have File System (SD-Card) and upgrade file is Texture, call the special verificaiton function */
    else if((pstWorkBuf->bFlagFfOperation) && (UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType))
    {
        i32Ret = UPG_i32VerifyTextureACF(pstWorkBuf->pFil, pstWorkBuf->stRtConfig.pu8RecvBufStartAddress, pstWorkBuf->stRtConfig.u32RecvBufTotalSize,
            pstWorkBuf->stRtConfig.u32FileSize, (uint8_t)pstWorkBuf->u8FileIndex, VERIFY_FILE);
    }
    else
    {
        /* currently no such requirement */
        
    }

    if(UPG_OK == i32Ret)
    {
        /* Verify data success, enter next step: UPG_enUpgStepEraseFlash */
        pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEraseFlash;
    }
    else
    {
        /* Verify data fail, exit Upgrade: change Main State Machine to Error Deal (C13) */
        i32Ret = UPG_ERR;
		
        /* Log has done at UPG_i32VerifyCPUCodeFromBuffer() or UPG_i32VerifyTextureACF() */
        pstWorkBuf->u8ErrState = UPG_ERR;
    }

    return i32Ret;
}



/**
  * @name       UPG_SUBSM__i32UpgStepEraseFlash
  * @brief      sub state machine erasing Flash step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepEraseFlash(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;
    uint32_t addr;
	
    
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
            addr = pstWorkBuf->u32BootloaderAdaptedFlashAddr;
        }
        else
        {
            addr = (uint32_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress;
        }
        
        i32Ret = UPG_i32EraseCpu1Flash(addr, pstWorkBuf->stRtConfig.u32FileSize);
    }
	
    if(i32Ret == UPG_OK)
    {
		if(UPG_enUpgFileEraseStoreSpace != pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
		{
			/* Erase flash success, enter next step: UPG_enUpgStepWriteFlash */
			pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepWriteFlash;
		}
		else
		{
			/* EraseStoreSpace only Erase flash , enter next step: UPG_enUpgStepEnd */
			pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEnd;
		}
        
    }
    else
    {
		/* Erase Flash fail, give a chance to try again */
        // Log error
		if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
		{
			SYN_ERR_LOG(EM_enErrEraseTextureFlashFail, EM_enErrLevelFatal);
		}
		else
		{
			SYN_ERR_LOG(EM_enErrEraseCPU1FlashFail, EM_enErrLevelFatal);
		}
    }
    
    
    return i32Ret;
}




/**
  * @name       UPG_SUBSM__i32UpgStepWaitEraseDone
  * @brief      sub state machine wait erasing done step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepWaitEraseDone(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;
     
    return i32Ret;
}



/**
  * @name       UPG_SUBSM__i32UpgStepWriteFlash
  * @brief      sub state machine writtng Flash step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepWriteFlash(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;
    uint32_t u32SendSize;
	uint32_t u32ReadSize;
    uint32_t u32ByteRead;
    uint32_t addr;
	
    
	if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
    {
		i32Ret = UPG_i32WriteTextureFlashFromBuffer(pstWorkBuf->pFil,
			(uint32_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress,
			pstWorkBuf->stRtConfig.pu8RecvBufStartAddress,
			pstWorkBuf->stRtConfig.u32RecvBufTotalSize, 
			pstWorkBuf->stRtConfig.u32FileSize,
			(uint8_t)pstWorkBuf->u8FileIndex, WRITEFILE);
    }
    else
    {
        /* Special deal: if upgrade file is bootloader, write Flash Address will be variable depend on Flash size */
        if(UPG_enUpgFileBootloader == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
        {
             addr = pstWorkBuf->u32BootloaderAdaptedFlashAddr;
        }
        else
        {
            addr = (uint32_t)pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress;
        }
        
		i32Ret = UPG_i32WriteCpu1FlashFromBuffer(pstWorkBuf->pFil,
			addr,
			pstWorkBuf->stRtConfig.pu8RecvBufStartAddress,
			pstWorkBuf->stRtConfig.u32RecvBufTotalSize, 
			pstWorkBuf->stRtConfig.u32FileSize,
			(uint8_t)pstWorkBuf->u8FileIndex, WRITEFILE);
    }
	
    if(i32Ret == UPG_OK)
    {
        /* Write flash success, enter next step: UPG_enUpgStepVerifyFlash */
        pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepVerifyFlash;
        
    }
    else
    {
		/* Write Flash fail, give a chance to try again */
        // Log error
		if(UPG_enUpgFileTexture == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
		{
			SYN_ERR_LOG(EM_enErrWriteTextureFlashFail, EM_enErrLevelFatal);
		}
		else
		{
			SYN_ERR_LOG(EM_enErrWriteCPU1FlashFail, EM_enErrLevelFatal);
		}
        i32Ret = UPG_ERR;
        pstWorkBuf->u8ErrState = UPG_ERR;
    }
	
    return i32Ret;
}



/**
  * @name       UPG_SUBSM__i32UpgStepWaitWriteDone
  * @brief      sub state machine wait writting done step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepWaitWriteDone(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;    
     
    return i32Ret;
}



/**
  * @name       UPG_SUBSM__i32UpgStepVerifyFlash
  * @brief      sub state machine verify Flash step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepVerifyFlash(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;

    /* Judge whether need verify stored data */
    if(UPG_TRUE == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].bFlagVerifyStoredFile)
    {
		/* cpu0.bin/cpu1.bin need verify stored data by Crc */
		if((UPG_enVerifyInputCrc == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enVerifyInputType)         
        && ((pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType == UPG_enUpgFileCpu0Bin) ||
        (pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType == UPG_enUpgFileCpu1Bin))
        )
        {
            i32Ret = UPG_i32VerifyCPUCodeFromBuffer(pstWorkBuf->pFil,
				pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress,
				pstWorkBuf->stRtConfig.u32RecvBufTotalSize,
                pstWorkBuf->stRtConfig.u32FileSize,
                pstWorkBuf->u8FileIndex, VERIFY_DATA);
            if(i32Ret != UPG_OK)
            {
                /* Verify Flash fail, Jump out of Upgrading,  */
                // Log err
                i32Ret = UPG_ERR;
                SYN_ERR_LOG(EM_enErrVerifyFlashFail, EM_enErrLevelFatal);
                pstWorkBuf->u8ErrState = UPG_ERR;
            }
            else
            {
                DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, pstWorkBuf->u8FileIndex, UPGRADE_OK, 0);
            }
        }
		/* Texture.acf need verify stored data by Xor */
		else if((UPG_enVerifyInputXor == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enVerifyInputType)         
        && pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType==UPG_enUpgFileTexture
        )
		{
			i32Ret = UPG_i32VerifyTextureACF(pstWorkBuf->pFil,
				pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].pu8UpgFlashStartAdress,
				pstWorkBuf->stRtConfig.u32RecvBufTotalSize,
				pstWorkBuf->stRtConfig.u32FileSize,
				pstWorkBuf->u8FileIndex, VERIFY_DATA);
            if(i32Ret != UPG_OK)
            {
                /* Verify Flash fail, Jump out of Upgrading,  */
                // Log err
                i32Ret = UPG_ERR;
                SYN_ERR_LOG(EM_enErrVerifyFlashFail, EM_enErrLevelFatal);
                pstWorkBuf->u8ErrState = UPG_ERR;
            }
            else
            {
                DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, pstWorkBuf->u8FileIndex, UPGRADE_OK, 0);
            }
		}
        else
        {
            
        }
  
    }
    else
    {
        /* no need verify, show upgrade OK */
        DrawProgressBar(SCREEN_WIDTH, SCREEN_HEIGHT, pstWorkBuf->u8FileIndex, UPGRADE_OK, 0);
       
    }

     /* If write isbootloader.bin successfully, set flag to indicate */
    if(UPG_enUpgFileBootloader == pstWorkBuf->pstPbConfig[pstWorkBuf->u8FileIndex].enUpgFileType)
    {
        UPG_BTLD_vSetUpgFlag(UPG_TRUE);
    }
    
    pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepEnd;
     
    return i32Ret;
}

/**
  * @name       UPG_SUBSM__i32UpgStepWaitVerifyDone
  * @brief      sub state machine wait verifying done step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2022-12-19
  */
int32_t UPG_SUBSM__i32UpgStepWaitVerifyDone(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;

    return i32Ret;
}


/**
  * @name       UPG_SUBSM__i32UpgStepEnd
  * @brief      sub state machine last step function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
int32_t UPG_SUBSM__i32UpgStepEnd(UPG_tstWorkBuf *pstWorkBuf)
{
    int32_t  i32Ret = UPG_OK;

    /* have File System (SD-Card) */
    if(UPG_TRUE == pstWorkBuf->bFlagFfOperation)
    {
        pstWorkBuf->u8FileIndex++;
        if(pstWorkBuf->u8FileIndex >= pstWorkBuf->u8FileCount)
        {
            /* When bootloader.bin exist, and Self upgrade not finished , no need to print version to file */
            if(pstWorkBuf->bFlagBootloaderExist && (UPG_FALSE == UPG_BTLD_bIfSelfUpgOpFinished()))
            {
                /* do nothing */
            }
            else
            {
                /* Print Version to File*/
                UPG_i32PrintVersionToFile(pstWorkBuf);
            }

            /* test if has Logged error */
            if(EM_bIfErrorExisted())
            {
                EM_vPrintErrorInfoToFile(pstWorkBuf->pFil);
            }
            
            //f_mount(NULL, "", 0);
            
            /* adjust Main and Sub State Machine */
            pstWorkBuf->enMainSM = UPG_enMainSM_JumpToApp;
            pstWorkBuf->enSubSM = UPG_enSubSM_GetComMode;
            pstWorkBuf->u8FileIndex = UPG_enUpgFileTexture;

        }
        else
        {
             //pstWorkBuf->stRtConfig.enUpgStep = UPG_enUpgStepInit;
             memset(&pstWorkBuf->stRtConfig, 0, sizeof(pstWorkBuf->stRtConfig));
        }
    }


    
    return i32Ret;
}


/**
  * @name       UPG_SUBSM__vGetComMode
  * @brief      sub state machine getting communication mode function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
//extern uint8_t SDCardSPI_CheckInsert(SPI_TypeDef* SPIx);
void UPG_SUBSM__vGetComMode(UPG_tstWorkBuf *pstWorkBuf)
{
    bool bFlagComExist = UPG_FALSE;
    uint32_t i;
    uint16_t u16CanUartTryTimes;

    if(pstWorkBuf->u16GetModeTryTimes < UPG_GET_MODE_TRY_TIMES)
    {
		/* Check if SD-Card com exist */
        if(pstWorkBuf->bExistUpgComSD)
		{
            UPG_SdSpi = SYN_GET_SD_SPI_CHANEL;
			/* Check if SD-Card exist */
			if(SDCardSPI_CheckInsert(UPG_SdSpi)) 
			{				
            
				/* load File System */
				for (i = 0; i < 10; i++)
				{
					/* if mount the SD card successful. */
					if (f_mount(pstWorkBuf->pFatFs, "1:", 1) == FR_OK)
					{
						bFlagComExist = UPG_TRUE;
						pstWorkBuf->enComMode = UPG_enComSdcard;
						/* SD-Card need FF operation */
						pstWorkBuf->bFlagFfOperation = UPG_TRUE;
						break;
					}
				}
				/* load HAL driver */
			}
		}
		
        /* If bFlagComExist, will not initial passive Upg mode */
        if(UPG_FALSE == bFlagComExist)
        {
    		/* Check if CAN com exist */
            if(pstWorkBuf->bExistUpgComCAN)
    		{
    			//init CAN and break
                if(UPG_TRUE != pstWorkBuf->bFlagCanInit)
    			{
                    gf_diag_com_INIT();
                    pstWorkBuf->bFlagCanInit = UPG_TRUE;
    			}
            }
            if(pstWorkBuf->bExistUpgComUART)
    		{
    			//init UART and break
    		    if(UPG_TRUE != pstWorkBuf->bFlagUartInit)
    			{
                    UPG_UART_vInit();
                    pstWorkBuf->bFlagUartInit = UPG_TRUE;
    			}
            }

            if((pstWorkBuf->bExistUpgComCAN) || (pstWorkBuf->bExistUpgComUART))
            {
                u16CanUartTryTimes = UPG_CAN_UART_WORK_TRY_TIMES;
    		    for(i = 0; i < u16CanUartTryTimes; i++)
    		    {
                    if((pstWorkBuf->bExistUpgComCAN) && (pstWorkBuf->bFlagCanWork))
                    {
                        bFlagComExist = UPG_TRUE;
                        pstWorkBuf->enComMode = UPG_enComCan;
                        /* CAN need not FF operation */
                        pstWorkBuf->bFlagFfOperation = UPG_FALSE;
                        break;
                    }
                    else if((pstWorkBuf->bExistUpgComUART) && (pstWorkBuf->bFlagUartWork))
                    {
                        bFlagComExist = UPG_TRUE;
                        pstWorkBuf->enComMode = UPG_enComUart;
                        /* UART need not FF operation */
                        pstWorkBuf->bFlagFfOperation = UPG_FALSE;
                        break;
                    }
                    else
                    {
                        vTaskDelay(1);
                    }
    		    }
        
    		}
        }

        /* Check if USB com exist */
        if(pstWorkBuf->bExistUpgComUSB && (UPG_FALSE == bFlagComExist))
        {
            struct usbh_msc *pt;
            
            usbh_initialize();
            
            if(checkMSDDevice() == 1)
            {     
                pt = msc_dev_probe(100);
                for (i=0; i<5; i++)
                {
                    if(NULL == pt)
                    {
                        break;
                    }
            
                    /* if mount the SD card successful. */
                    if (f_mount(pstWorkBuf->pFatFs, "2:", 1) == FR_OK)
                    {
                    
                        bFlagComExist = UPG_TRUE;
                        pstWorkBuf->enComMode = UPG_enComMSD;
                        /* MSD need FF operation */
                        pstWorkBuf->bFlagFfOperation = UPG_TRUE;
                        break;
                    }
                }
            
            }
        }
        pstWorkBuf->u16GetModeTryTimes++;
        
    }
    else
    {
        /* Log info: no Com exist */
    
        /* If CPU1.bin already exist in ROM, MainSM enter UPG_enMainSM_JumpToApp, else enter UPG_enMainSM_Error */
        if(UPG_TRUE == UPG_bIfCpu1HasCode()) /**/
        {
            pstWorkBuf->enMainSM = UPG_enMainSM_JumpToApp;
            pstWorkBuf->u16GetModeTryTimes = 0;
        }
        else
        {
            pstWorkBuf->enMainSM = UPG_enMainSM_Error;
            // Log err 
            SYN_ERR_LOG(EM_enErrNoAppCode, EM_enErrLevelFatal);
            pstWorkBuf->u8ErrState = UPG_ERR;
        }

    }

    /* change Sub State Machine to CheckVersion */
    if(UPG_TRUE == bFlagComExist)
    {
        if (pstWorkBuf->enComMode == UPG_enComSdcard || pstWorkBuf->enComMode == UPG_enComMSD)
        {
			/* Sub State Machine enter UPG_enSubSM_Process_Active */
			pstWorkBuf->enSubSM = UPG_enSubSM_Process_Active;
        }
        else if (pstWorkBuf->enComMode == UPG_enComCan || pstWorkBuf->enComMode == UPG_enComUart)
		{
			/* Sub State Machine enter UPG_enSubSM_Process_Passive */
			pstWorkBuf->enSubSM = UPG_enSubSM_Process_Passive;
		}
        /* clear the counter */
        pstWorkBuf->u16GetModeTryTimes = 0;
    
    }
    

}





/**
  * @name       UPG_SUBSM__vGetComMode
  * @brief      sub state machine main process function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_SUBSM__vProcess(UPG_tstWorkBuf *pstWorkBuf)
{
    // to do
    switch (pstWorkBuf->stRtConfig.enUpgStep)
    {

        case UPG_enUpgStepInit:
            UPG_SUBSM__i32UpgStepInit(pstWorkBuf); 
            break;
        
        case UPG_enUpgStepCheckVersion:
            UPG_SUBSM__i32UpgStepCheckVersion(pstWorkBuf);
            break;

        case UPG_enUpgStepGetData:
            UPG_SUBSM__i32UpgStepGetData(pstWorkBuf);
            break;
        
        case UPG_enUpgStepVerifyData:
            UPG_SUBSM__i32UpgStepVerifyData(pstWorkBuf);
            break;
        
        case UPG_enUpgStepEraseFlash:
            UPG_SUBSM__i32UpgStepEraseFlash(pstWorkBuf);
            break;
        
        case UPG_enUpgStepWaitEraseDone:
            UPG_SUBSM__i32UpgStepWaitEraseDone(pstWorkBuf);
            break;

        case UPG_enUpgStepWriteFlash:
            UPG_SUBSM__i32UpgStepWriteFlash(pstWorkBuf);
            break;
        
        case UPG_enUpgStepWaitWriteDone:
            UPG_SUBSM__i32UpgStepWaitWriteDone(pstWorkBuf);
            break;
        
        case UPG_enUpgStepVerifyFlash:
            UPG_SUBSM__i32UpgStepVerifyFlash(pstWorkBuf);
            break;
		
		case UPG_enUpgStepWaitVerifyDone:
            UPG_SUBSM__i32UpgStepWaitVerifyDone(pstWorkBuf);
            break;
        
        case UPG_enUpgStepEnd:
            UPG_SUBSM__i32UpgStepEnd(pstWorkBuf);
            break;

        default:
            // Log error: to do
            break;
    }

    /* Fatal error exists */
    if(UPG_ERR == pstWorkBuf->u8ErrState)
    {
        pstWorkBuf->enMainSM = UPG_enMainSM_Error;
    }
}

