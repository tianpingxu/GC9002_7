////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2021/07/05
// File Name:     upgrade_sm.c
// Project Name:  CPU0_Bootloader_Redesign
// 
// Revision:
// Revision 1.0.0 - File Created 2021/06/22 by Zhu Jiahai
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "upgrade_c1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "csi_irq.h"
#include "DrawProgramBar.h"
#include "DiagPlatform.h"




UPG_tstWorkBuf UPG_stWorkBuf;
UPG_tstWorkBuf *UPG_pstWorkBuf = &UPG_stWorkBuf;
FATFS fatFs;
FIL myFil;
DIR myDir;

extern uint32_t TextureFlashSize;


const UPG_tstPrebuildConfig UPG_astPrebuildConfig[] =
{
    /* Can configure it in ci.h */
    UPG_PRECONFIG
};


/**
  * @name       UPG_SM__Jump_User_Application
  * @brief      Jump to application function, if jump to application function,will not return
  * @note       
  * @param      void
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_SM__Jump_User_Application(void)
{
    uint32_t JumpAddress;           //start address of application function.
    pFunction Jump_To_Application;  //function pointer for application function.
    uint32_t startValue;
    uint32_t cpu1Size;
    uint32_t copiedSize;
    uint32_t* addrHyper;
    uint32_t* addrFlash;

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
        cpu1Size = UPG_u32GetCpu1BinSize();
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
	

	/* Set application function pointer. */
	Jump_To_Application = (pFunction)JumpAddress;

	/* Jump to appliocation function. */
	Jump_To_Application();
}



/**
  * @name       UPG_SM__vInit
  * @brief      Initialization function, initialize variable
  * @note       
  * @param      void
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_SM__vInit(void)
{
    memset(UPG_pstWorkBuf, 0, sizeof(UPG_tstWorkBuf));
    /* load FATFS, FIL */
    UPG_pstWorkBuf->pFatFs = &fatFs;
    UPG_pstWorkBuf->pFil = &myFil;
    UPG_pstWorkBuf->pDir = &myDir;

    UPG_pstWorkBuf->pstPbConfig = UPG_astPrebuildConfig;
    UPG_pstWorkBuf->u8FileCount = sizeof(UPG_astPrebuildConfig) / sizeof(UPG_tstPrebuildConfig);

    /* Calculate bootloader self-upg Flash address dynamically */
    if(TextureFlashSize > BOOTLOADER_SAVE_OFFSET)
    {
        UPG_pstWorkBuf->u32BootloaderAdaptedFlashAddr = BOOTLOADER_START_ADDR + TextureFlashSize - BOOTLOADER_SAVE_OFFSET;
    }
    else
    {
        UPG_pstWorkBuf->u32BootloaderAdaptedFlashAddr = TEXTURE_FLASH_MAX_ADDR - BOOTLOADER_SAVE_OFFSET;
    }
	
	/* get from configfile */
    if(CFG_Get_cfg_can())
    {
        UPG_pstWorkBuf->bExistUpgComCAN = TRUE_1;
    }
    else
    {
        UPG_pstWorkBuf->bExistUpgComCAN = FALSE_0;
    }

    if(CFG_Get_cfg_uart())
    {
        UPG_pstWorkBuf->bExistUpgComUART = TRUE_1;
    }
    else
    {
        UPG_pstWorkBuf->bExistUpgComUART = FALSE_0;
    }

    if(CFG_Get_cfg_sd())
    {
        UPG_pstWorkBuf->bExistUpgComSD = TRUE_1;
    }
    else
    {
        UPG_pstWorkBuf->bExistUpgComSD = FALSE_0;
    }

    if(CFG_Get_cfg_usb())
    {
        UPG_pstWorkBuf->bExistUpgComUSB = TRUE_1;
    }
    else
    {
        UPG_pstWorkBuf->bExistUpgComUSB = FALSE_0;
    }
    
}


/**
  * @name       UPG_SM__vUpgrade
  * @brief      state machine upgrade function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_SM__vUpgrade(UPG_tstWorkBuf *pstWorkBuf)
{
    switch (pstWorkBuf->enSubSM)
    {
		case UPG_enSubSM_Process_Passive:
		    UPG_SUBSM_PAS__vProcess(pstWorkBuf);
		    break;
			
        case UPG_enSubSM_Process_Active:
            UPG_SUBSM__vProcess(pstWorkBuf);
            break;
        
        case UPG_enSubSM_GetComMode:
            UPG_SUBSM__vGetComMode(pstWorkBuf);
            break;
        default:
            // Log error: to do
            break;
    }
}

/**
  * @name       UPG_SM__vSystemDeinit
  * @brief      System Deinit before jumping to Application function
  * @note       
  * @param      none
  * @retval     void
  * @author     Yn
  * @date       2022-12-19
  */
void UPG_SM__vSystemDeinit(void)
{
//    ShareMemory_InterruptDISABLE();
//    //disable timer7
//    NVIC_DisableIRQ(T7_IRQn);
//    XAD_TMR7->T7CON = 0x0;
//    T7CLK_OFF;
//    
//    NVIC_DisableIRQ(OTG_IRQn);
//    USBCLK_OFF;
	
	csi_irq_disable(OTG_IRQn);
	
    vTaskEndScheduler();
}

/**
  * @name       UPG_SM__vUpgrade
  * @brief      state machine jumping to Application function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_SM__vJumpToApp(UPG_tstWorkBuf *pstWorkBuf)
{
    /* close Timer Service (include Timer0 interrupt) */
    SRV_TIMER_vDeInit();
    
    DRAW_vDeinit();
    /* set code flash write protect */
    //INTERNAL_FLASH_SetWriteProtect();
    
    /* Before jumping, deinit device */
    if(pstWorkBuf->bExistUpgComCAN)
    {
        CANDIAG_vCanFdHwDeInit();
    }
    if(pstWorkBuf->bExistUpgComUART)
    {
        UPG_UART_vUartHwDeInit();
    }

	/* System Deinit.*/
	UPG_SM__vSystemDeinit();

    /*Before Jump to APP, check if need to upgrade bootloader from Flash to Flash */
    if(UPG_BTLD_vGetIfNeedUpgBootloader())
    {
        if(UPG_BTLD_bIfBootloaderValid((uint8_t *)pstWorkBuf->u32BootloaderAdaptedFlashAddr))
        {
            UPG_BTLD_vUpgBootloader((uint8_t *)pstWorkBuf->u32BootloaderAdaptedFlashAddr);
        }
        else
        {
            /* Log err */
        }
    }
    
    /* If there is application function, jump to appliocation function.*/
    UPG_SM__Jump_User_Application();
}



/**
  * @name       UPG_SM__vErrorDeal
  * @brief      state machine errer deal function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_SM__vErrorDeal(UPG_tstWorkBuf *pstWorkBuf)
{
    /* Print error information to File */
//    if((NULL != pstWorkBuf->pFil) && (pstWorkBuf->bFlagFfOperation))
    {
        EM_vPrintErrorInfoToFile(pstWorkBuf->pFil);
    }
    
    UPG_pstWorkBuf->enMainSM = UPG_enMainSM_Invalid;

//    /* Reset system */
//    NVIC_SystemReset();
}


/**
  * @name       UPG_SM__vOutputHeartBeat
  * @brief      Output heartbeat for SLT
  * @note       
  * @param      pin: pointer to the PIN address
  * @retval     void
  * @author     
  * @date       2023-12-01
  */
void UPG_SM__vOutputHeartBeat(volatile unsigned int *pin)
{
    
    if(*pin == 0)
    {
        *pin = 1;
    }
    else
    {
        *pin = 0;
    }
}


/**
  * @name       UPG_SM__vMainSmProcess
  * @brief      main state machine deal function
  * @note       
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
void UPG_SM__vMainSmProcess(void)
{
    switch (UPG_pstWorkBuf->enMainSM)
    {
        case UPG_enMainSM_Upgrade:
            UPG_SM__vUpgrade(UPG_pstWorkBuf);            
            break;
        
        case UPG_enMainSM_Init:
            /* Can do some initialization */       
            UPG_pstWorkBuf->enMainSM = UPG_enMainSM_Upgrade;
            break;

        case UPG_enMainSM_JumpToApp:
            UPG_SM__vJumpToApp(UPG_pstWorkBuf);

            break;
        
        case UPG_enMainSM_Error:
            UPG_SM__vErrorDeal(UPG_pstWorkBuf);
            break;

        default:
            /* PB1 output 0 & 1 alternately */
            //UPG_SM__vOutputHeartBeat((volatile unsigned int*)0x4000B004);
            break;
    }
}



