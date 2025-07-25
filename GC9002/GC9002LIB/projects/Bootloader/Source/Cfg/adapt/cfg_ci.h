/**
  ******************************************************************************
  * @file    cfg_ci.h
  * @author  
  * @version V1.0.0
  * @date    05-07-2023
  * @brief   The internal configuration head file of CFG module
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_CI_H
#define __CFG_CI_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#ifdef CONFIG_RUN_BOOTLOADER
#include "upgrade_c1.h"
#endif
#include "cfg_autogen_ct.h"




#define CFG_MAGIC_NUM                                       (0x41484D49)
#define CFG_VERSION                                         (0x0102)

#ifdef CONFIG_RUN_BOOTLOADER
#define CFG_START_ADDR                                      (CFG_tstUpgCfg *)(UPG_CFG_START_ADDRESS)
#else
/* Make sure the value keep equal to Bootloader project */
#define CFG_START_ADDR                                      (CFG_tstUpgCfg *)(0x080F0000 + 0x7000)

#endif





#ifdef __cplusplus
}
#endif

#endif /* __CFG_CI_H */




