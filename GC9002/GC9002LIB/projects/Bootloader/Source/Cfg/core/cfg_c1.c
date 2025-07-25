#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cfg_c1.h"




/**
  * @name       CFG_vInit
  * @brief      CFG module initialization function
  * @note       If intialization failed, will blocked
  * @param      none  
  * @retval     none
  * @author     
  * @date       2023-07-05
  */
void CFG_vInit(void)
{
    if(CFG_bCheckVaild(CFG_START_ADDR))
    {
        CFG_SetCfgPtr(CFG_START_ADDR);
    }
    else if((CFG_bCheckVaild(CFG_GetCfgPtrOnRom())))
    {
        CFG_SetCfgPtr(CFG_GetCfgPtrOnRom());
    }
    else
    {
        /* Assert Fatal error */
        while(1);
    }
}


/**
  * @name       CFG_bCheckVaild
  * @brief      CFG module initialization function
  * @note       
  * @param      flag:  
  * @retval     bool:
  * @author     
  * @date       2023-07-05
  */
bool CFG_bCheckVaild(CFG_tstUpgCfg * cfg)
{
    uint32_t size, i;
    uint8_t * ptr;
    uint16_t checkSum = 0;
    
    if((cfg->Head.valid_flag == CFG_MAGIC_NUM) && (cfg->Head.version == CFG_VERSION))
    {
        size = cfg->Head.size;
        ptr = (uint8_t *)((uint32_t)cfg + sizeof(cfg->Head));
        for(i = 0; i < size; i++)
        {
            checkSum += *ptr++;
        }
        
        if((checkSum & 0xFFFF) == cfg->Head.checksum)
        {
            return TRUE;
        }
    }
    else
    {
        /* do nothing */
    }
    
    return FALSE;
}

