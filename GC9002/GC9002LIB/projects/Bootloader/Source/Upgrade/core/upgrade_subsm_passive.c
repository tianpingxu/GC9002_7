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
#include "em_ct.h"
#include "DiagPlatform.h"




//static uint32_t UPG_u32CanUartUpgFlag __attribute__((at(CPU0_CAN_UART_EXIST_ADDR)));
static uint32_t UPG_u32CanUartUpgFlag;


extern UPG_tstWorkBuf *UPG_pstWorkBuf;

extern void gf_func_Respond_10_02(void);

/**
  * @name       UPG_bIfCpu0HasCode
  * @brief      check if CPU0 has already burnt code 
  * @note       
  * @param      none
  * @retval     The result: True or False
  * @author     Zhu Jiahai
  * @date       2021-06-22
  */
bool UPG_bIfCpu0HasCode(void)
{
    #if 0
    char* pAddress;

    pAddress = (char*)CPU0_VERSION_ADDRESS;

    if (('1' == pAddress[0]) && ('.' == pAddress[1]))
    {
        return UPG_TRUE;
    }
    else
    {
        return UPG_FALSE;
    }
    #endif
    return UPG_TRUE;
}


/**
  * @name       UPG_bIfCanUartUpgFlagExist
  * @brief      check if CAN/UART upgrade flag exist
  * @note       
  * @param      none
  * @retval     The result: True or False
  * @author     Zhu Jiahai
  * @date       2022-11-23
  */
bool UPG_bIfCanUartUpgFlagExist(void)
{

    if ((CPU0_CAN_EXIST_FLAG == UPG_u32CanUartUpgFlag) || (CPU0_UART_EXIST_FLAG == UPG_u32CanUartUpgFlag))
    {
        return UPG_TRUE;
    }
    else
    {
        return UPG_FALSE;
    }
}


/**
  * @name       UPG_vClearCanUartUpgFlag
  * @brief      clear CAN/UART upgrade flag
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-11-23
  */
void UPG_vClearCanUartUpgFlag(void)
{
    UPG_u32CanUartUpgFlag = 0;
}


/**
  * @name       UPG_vSetCanUartUpgFlag
  * @brief      set CAN/UART upgrade flag
  * @note       
  * @param      uint32_t flag
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-11-23
  */
void UPG_vSetCanUartUpgFlag(uint32_t flag)
{
    //uint32_t* pAddress = (uint32_t*)CPU0_CAN_UART_EXIST_ADDR;

    UPG_u32CanUartUpgFlag = flag;
}


/**
  * @name       UPG_SUBSM_PAS__vProcess
  * @brief      Passive state machine process function
  * @note       
  *             
  * @param      pstWorkBuf: pointer to the main work buffer
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2022-11-23
  */
void UPG_SUBSM_PAS__vProcess(UPG_tstWorkBuf *pstWorkBuf)
{
    switch (pstWorkBuf->enSubSMPassive)
    {
        case UPG_enSubSMPassive_CanUart_FirstIn:
            /* check Flag of waiting synchronization */
            if(pstWorkBuf->bFlagCanWork)
            {
#if(GEELY_JIHE)
                /* call CAN interface to respond to 10 02, set send-out MainState value to 0xAA */
                gf_func_Respond_10_02();
#endif
				
                /* SubSM switch to CanUart_Run */
                pstWorkBuf->enSubSMPassive = UPG_enSubSMPassive_CanUart_Run;
            }
            else if(pstWorkBuf->bFlagUartWork)
            {
                pstWorkBuf->enSubSMPassive = UPG_enSubSMPassive_CanUart_Run;
            }
           
            break;

        case UPG_enSubSMPassive_CanUart_Run:
            /* do nothing, upgrading process be handle by CAN command passively */
            
            break;

        case UPG_enSubSMPassive_CanUart_Quit:
            /* Received 11 01 from CAN, MainSM quit this mode and try to enter JumpToApp  */
            if(UPG_TRUE == UPG_bIfCpu1HasCode()) /**/
            {
                #if 0
                if(UPG_bIfCanUartUpgFlagExist())
                {
                    /* clear Flag to prevent upgrading again, only upgrade successfully to clear Flag */
                    UPG_vClearCanUartUpgFlag();

                    /* RTC reset cpu0 and cpu1 */
                    //*(uint32_t *)(0x40102030) = 0x01;
                    /// to do
                }
                else
                #endif
                {
                    vTaskDelay(1);
                    /* Before jumping, deinit device */
                    /* Deinit move to UPG_SM__vJumpToApp as active Upgrade mode need to deinit too */
                    pstWorkBuf->enMainSM = UPG_enMainSM_JumpToApp;
                }
            }
            else
            {
                pstWorkBuf->enMainSM = UPG_enMainSM_Error;
                // Log err 
                SYN_ERR_LOG(EM_enErrNoComForUpgrade, EM_enErrLevelFatal);
            }
            break;

        default: 
            break;
    }


}


/**
  * @name       UPG_SUBSM_PAS_vCanUartRunQuit
  * @brief      Passive Sub state machine quit CAN/UART Run mode
  * @note       
  * @param      none
  * @retval     void
  * @author     Zhu Jiahai
  * @date       2022-11-28
  */
void UPG_SUBSM_PAS_vCanUartRunQuit(void)
{
    if((UPG_enSubSM_Process_Passive == UPG_pstWorkBuf->enSubSM) && (UPG_enSubSMPassive_CanUart_Run == UPG_pstWorkBuf->enSubSMPassive))
    {
        UPG_pstWorkBuf->enSubSMPassive = UPG_enSubSMPassive_CanUart_Quit;
    }
}


/**
  * @name       UPG_SUBSM_PAS_vUartWork
  * @brief      Indicate Uart work(shakehand finished)
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-12-29
  */
void UPG_SUBSM_PAS_vUartWork(void)
{
    UPG_pstWorkBuf->bFlagUartWork = TRUE_1;
}


void UPG_SUBSM_PAS_vCanWork(void)
{
    UPG_pstWorkBuf->bFlagCanWork = TRUE_1;
}

