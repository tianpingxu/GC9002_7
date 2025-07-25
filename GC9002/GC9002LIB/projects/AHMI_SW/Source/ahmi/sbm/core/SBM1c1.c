/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : SBM1c1.c
*
* Description  : This file contains the implementation of SBM interface
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-07-21
****************************************************************************/


/* Foreign headerfiles */
#include <stdio.h>
//#include "core_cm0.h"
//#include "CPU1_CMSDK_CM0.h"
#include "stdbool.h"


/* Own headerfiles */
#include "SBM1c1.h"



/**
  * @name       SBM_bIfCpu1HardwareOpBusy
  * @brief      Get if CPU1 is on hardware operation
  * @note       
  * @param      none
  * @retval     bool
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
bool SBM_bIfCpu1HardwareOpBusy(void)
{
    uint32_t value;
    bool ret;

    value = *(uint32_t*)SBM_CPU1_START_ADDR;
    if(SBM_HARDWARE_IN_USE == SBM_u8GetHardwareState(value))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}


/**
  * @name       SBM_vSetCpu1HardwareOpState
  * @brief      Set Cpu1 hardware operation state 
  * @note        
  * @param      uint8_t state
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
void SBM_vSetCpu1HardwareOpState(uint8_t state)
{
    uint32_t value;
    
    value = *(uint32_t*)SBM_CPU1_START_ADDR;
    
    *(uint32_t*)SBM_CPU1_START_ADDR = SBM_u32SetHardwareState(value, state);
}


/**
  * @name       SBM_bIfCpu0HardwareOpBusy
  * @brief      Get if CPU0 is on hardware operation
  * @note       
  * @param      none
  * @retval     bool
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
bool SBM_bIfCpu0HardwareOpBusy(void)
{
    uint32_t value;
    bool ret;

    value = *(uint32_t*)SBM_CPU0_START_ADDR;
    if(SBM_HARDWARE_IN_USE == SBM_u8GetHardwareState(value))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}


/**
  * @name       SBM_vSetCpu0HardwareOpState
  * @brief      Set Cpu0 hardware operation state 
  * @note        
  * @param      uint8_t state
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
void SBM_vSetCpu0HardwareOpState(uint8_t state)
{
    uint32_t value;
    
    value = *(uint32_t*)SBM_CPU0_START_ADDR;
    
    *(uint32_t*)SBM_CPU0_START_ADDR = SBM_u32SetHardwareState(value, state);
}




/**
  * @name       SBM_vEnterCpu1HardwareOp
  * @brief      CPU1 apply to start operate hardware  
  * @note        
  * @param      void
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
void SBM_vEnterCpu1HardwareOp(void)
{
    /* wait for CPU0 operating hardware end */
    while(true == SBM_bIfCpu0HardwareOpBusy())
    {
        
    }

    /* Tell CPU0 that CPU1 is operating hardware */
    SBM_vSetCpu1HardwareOpState(SBM_HARDWARE_IN_USE);    
}



/**
  * @name       SBM_vExitCpu1HardwareOp
  * @brief      CPU1 release operation hardware  
  * @note        
  * @param      void
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
void SBM_vExitCpu1HardwareOp(void)
{

    /* Tell CPU0 that CPU1 is operating hardware */
    SBM_vSetCpu1HardwareOpState(SBM_HARDWARE_NO_USE);    
}

/**
  * @name       SBM_vEnterCpu0HardwareOp
  * @brief      CPU1 apply to start operate hardware  
  * @note        
  * @param      void
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
void SBM_vEnterCpu0HardwareOp(void)
{
    /* wait for CPU0 operating hardware end */
    while(true == SBM_bIfCpu1HardwareOpBusy())
    {
        
    }

    /* Tell CPU1 that CPU0 is operating hardware */
    SBM_vSetCpu0HardwareOpState(SBM_HARDWARE_IN_USE);    
}



/**
  * @name       SBM_vExitCpu0HardwareOp
  * @brief      CPU1 release operation hardware  
  * @note        
  * @param      void
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-07-21
  */
void SBM_vExitCpu0HardwareOp(void)
{

    /* Tell CPU0 that CPU1 is operating hardware */
    SBM_vSetCpu0HardwareOpState(SBM_HARDWARE_NO_USE);    
}



