#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "upgrade_c1.h"
#include "upgrade_btld.h"
#include "crc_c1.h"
#include "csi_hal_driver_flash.h"



#if defined(CHIP_GC9002)
#define __FLASH_DRIVER 		__attribute__((used, section(".flashdriver")))
#else
#define  __FLASH_DRIVER  
#endif

uint32_t g_u32FlagSelfUpgOpFinished __attribute__((section(".bss_data_noint")));
static bool UPG_BTLD_bFlagNeedUpg = UPG_FALSE;

extern uint8_t myUpgradebuffer[TextureBufferMaxSize];
extern UPG_tstWorkBuf *UPG_pstWorkBuf;


/**
  * @name       UPG_BTLD_vSetUpgFlag
  * @brief      set the flag to indicate if need to upgrade bootloader
  * @note       
  * @param      flag:  
  * @retval     none
  * @author     
  * @date       2023-05-31
  */
void UPG_BTLD_vSetUpgFlag(bool flag)
{
    UPG_BTLD_bFlagNeedUpg = flag;
}


/**
  * @name       UPG_BTLD_vGetIfNeedUpgBootloader
  * @brief      Get the flag to indicate if need to upgrade bootloader
  * @note       
  * @param      none  
  * @retval     bool
  * @author     
  * @date       2023-05-31
  */
bool UPG_BTLD_vGetIfNeedUpgBootloader(void)
{
    return UPG_BTLD_bFlagNeedUpg;
}


/**
  * @name       UPG_BTLD_bIfBootloaderValid
  * @brief      Check if bootloader saved on Flash is valid 
  * @note       
  * @param      addr: the saved start address of bootloader 
  * @retval     True: valid, False: Invalid;
  * @author     
  * @date       2023-05-31
  */
bool UPG_BTLD_bIfBootloaderValid(uint8_t * addr)
{
    bool res;
    UPG_tstBtldHeadInfoPtr pHdInfo = (UPG_tstBtldHeadInfoPtr)addr;
    uint32_t size;
    uint32_t readSize;
    uint8_t *pReadAddr;
    uint32_t crc = 0xFFFFFFFFul;
        
    /* Input parameter valid check */
    if((NULL == addr) || (UPG_BTLD_MAGIC_NUM != pHdInfo->u32MagicNum))
    {
        /* Log err */
        res = UPG_FALSE;
        return res;
    }

    size = pHdInfo->u32BtldSize;
    pReadAddr = addr + sizeof(UPG_tstBtldHeadInfo);

    while(size)
    {
        if(size >= TextureBufferMaxSize)
        {
            readSize = TextureBufferMaxSize;
        }
        else
        {
            readSize = size;
        }

        csi_hal_extern_flash_read(((uint32_t)pReadAddr), readSize, myUpgradebuffer);
        crc = CRC_u32Crc32(crc, myUpgradebuffer, readSize);
        
        /* Adjust variable */
        size -= readSize;
        pReadAddr += readSize;  
    }
    
    crc = crc ^ 0xFFFFFFFFF;

    if(crc == pHdInfo->u32Crc)
    {
        res = UPG_TRUE;
    }
    else
    {
        res = UPG_FALSE;
    }

    return res;
}



/**
  * @name       UPG_BTLD_vUpgBootloader
  * @brief      Upgrade bootloader from Flash to Flash 
  * @note       
  * @param      addr: the saved start address of bootloader 
  * @retval     none
  * @author     
  * @date       2023-05-31
  */
__FLASH_DRIVER void UPG_BTLD_vUpgBootloader(uint8_t * addr)
{
    UPG_tstBtldHeadInfoPtr pHdInfo = (UPG_tstBtldHeadInfoPtr)addr;
    uint32_t size;
    uint32_t readSize;
    uint8_t *pReadAddr;
    uint8_t *pWhiteAddr;
    int32_t i;
    uint32_t n;
    
    uint32_t JumpAddress;           //start address of application function.
    pFunction Jump_To_Application;  //function pointer for application function.
    uint32_t startValue;
    uint32_t cpu1Size;
    uint32_t copiedSize;
    uint32_t* addrHyper;
    uint32_t* addrFlash;


    /* Input parameter valid check */
    if((NULL == addr) || (UPG_BTLD_MAGIC_NUM != pHdInfo->u32MagicNum) || (BOOTLOADER_CODE_SIZE_MAX < pHdInfo->u32BtldSize))
    {
        /* Log err */
        return;
    }

    n = (pHdInfo->u32BtldSize - 1) / TextureNorFlashBlockSize + 1;
    for(i = 0; i < n; i++)
    {
        csi_hal_extern_flash_block_erase(BOOTLOADER_START_ADDR + i * TextureNorFlashBlockSize);
    }
    

    size = pHdInfo->u32BtldSize;
    pReadAddr = addr + sizeof(UPG_tstBtldHeadInfo);
    pWhiteAddr = (uint8_t *)BOOTLOADER_START_ADDR;

    while(size)
    {
        if(size >= TextureBufferMaxSize)
        {
            readSize = TextureBufferMaxSize;
        }
        else
        {
            readSize = size;
        }
        csi_hal_extern_flash_read(((uint32_t)pReadAddr), readSize, myUpgradebuffer);
        csi_hal_extern_flash_write(((uint32_t)pWhiteAddr), readSize, myUpgradebuffer);
        
        /* Adjust variable */
        size -= readSize;
        pReadAddr += readSize; 
        pWhiteAddr += readSize; 
    }
    
    UPG_BTLD_vSetSelfUpgOpFinished();
    /* Passive mode need to jump to APP, Active mode need reset */
    if(UPG_enComCan == UPG_pstWorkBuf->enComMode || UPG_enComUart == UPG_pstWorkBuf->enComMode)
    {
        /* Jump to App, as Flash is erased */

    	/* Judge whether GC9002 is Nohyper */
    	char* cpu1_compile_addr = (char*)CPU1_APP_COMPILE_ADDR_STORE_ADDR;
    	if((cpu1_compile_addr[0] == 'N') && (cpu1_compile_addr[1] == 'o') && (cpu1_compile_addr[2] == 'H') && (cpu1_compile_addr[3] == 'y')
    	 && (cpu1_compile_addr[4] == 'p') && (cpu1_compile_addr[5] == 'e') && (cpu1_compile_addr[6] == 'r'))
    	{
    		JumpAddress = APP_FUNCTION_START_ADDR;
    	}
    	/* Judge whether GC9002 code in flash */
    	else if((cpu1_compile_addr[0] == 'G') && (cpu1_compile_addr[1] == 'C') && (cpu1_compile_addr[2] == '9') && (cpu1_compile_addr[3] == '0')
    	 && (cpu1_compile_addr[4] == '0') && (cpu1_compile_addr[5] == '2'))
    	{
    		JumpAddress = APP_FUNCTION_START_ADDR;
    	}
    	/* Judge whether GC9002 code in hyper */
    	else if((cpu1_compile_addr[0] == 'C') && (cpu1_compile_addr[1] == 'o') && (cpu1_compile_addr[2] == 'd') && (cpu1_compile_addr[3] == 'e')
    	 && (cpu1_compile_addr[4] == 'H') && (cpu1_compile_addr[5] == 'y') && (cpu1_compile_addr[6] == 'p'))
    	{
            JumpAddress = APP_FUNCTION_START_ADDR_HYPER;
            
            /* Copy cpu1.bin from Flash to HyperRam */
            cpu1Size = CPU1_BIN_DEFAULT_SIZE;
            addrHyper = (uint32_t*)START_ADDR_HYPER;
            addrFlash = (uint32_t*)CPU1_START_ADDRESS;
            copiedSize = 0;
            while(cpu1Size > copiedSize)
            {
                *addrHyper++ = *addrFlash++;
                copiedSize += 4;
            }
    	}
    	else
    	{
    		/* Block here */
    		while(1);
    	}
    	
    //    startValue = *(uint32_t*)APP_FUNCTION_START_ADDR;
    //    if((APP_FLASH_START_VALUE & APP_JUMP_START_MASK) == (startValue & APP_JUMP_START_MASK))
    //    {
    //        JumpAddress = APP_FUNCTION_START_ADDR;
    //    }
    //    else if((APP_HYPER_START_VALUE & APP_JUMP_START_MASK) == (startValue & APP_JUMP_START_MASK))
    //    {
    //        JumpAddress = APP_FUNCTION_START_ADDR_HYPER;
    //        
    //        /* Copy cpu1.bin from Flash to HyperRam */
    //        cpu1Size = CPU1_BIN_DEFAULT_SIZE;
    //        addrHyper = (uint32_t*)START_ADDR_HYPER;
    //        addrFlash = (uint32_t*)CPU1_START_ADDRESS;
    //        copiedSize = 0;
    //        while(cpu1Size > copiedSize)
    //        {
    //            *addrHyper++ = *addrFlash++;
    //            copiedSize += 4;
    //        }
    //    }
    //    else
    //    {
    //		/* Judge whether GC9002 is Nohyper */
    //		char* cpu1_compile_addr = (char*)CPU1_APP_COMPILE_ADDR_STORE_ADDR;
    //		if((cpu1_compile_addr[0] == 'N') && (cpu1_compile_addr[1] == 'o') && (cpu1_compile_addr[2] == 'H') && (cpu1_compile_addr[3] == 'y')
    //		 && (cpu1_compile_addr[4] == 'p') && (cpu1_compile_addr[5] == 'e') && (cpu1_compile_addr[6] == 'r'))
    //		{
    //			JumpAddress = APP_FUNCTION_START_ADDR;
    //		}
    //		else
    //		{
    //		 	/* Block here */
    //			while(1);
    //		}
    //    }

    	
    	/* Set application function pointer. */
    	Jump_To_Application = (pFunction)JumpAddress;

    	/* Jump to appliocation function. */
    	Jump_To_Application();
    }
    else if(UPG_enComSdcard == UPG_pstWorkBuf->enComMode || UPG_enComMSD == UPG_pstWorkBuf->enComMode)
    {
        HAL_RCC_SystemReset();
    }
    else
    {
        
    }
}


/**
  * @name       UPG_BTLD_bIfSelfUpgOpFinished
  * @brief      Check if bootloader self upgrade operation finished 
  * @note       
  * @param      none 
  * @retval     True or False
  * @author     
  * @date       2024-09-20
  */
bool UPG_BTLD_bIfSelfUpgOpFinished(void)
{
    return (g_u32FlagSelfUpgOpFinished == UPG_FLAG_SELF_UPG_OP_FINISHED) ? UPG_TRUE : UPG_FALSE;
}


/**
  * @name       UPG_BTLD_vSelfUpgOpFinished
  * @brief      Check if bootloader self upgrade operation finished 
  * @note       
  * @param      none 
  * @retval     none
  * @author     
  * @date       2024-09-20
  */
__FLASH_DRIVER void UPG_BTLD_vSetSelfUpgOpFinished(void)
{
    g_u32FlagSelfUpgOpFinished = UPG_FLAG_SELF_UPG_OP_FINISHED;
}


