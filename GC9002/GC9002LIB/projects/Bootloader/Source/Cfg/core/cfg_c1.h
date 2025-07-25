/**
  ******************************************************************************
  * @file    cfg_ci.h
  * @author  
  * @version V1.0.0
  * @date    05-07-2023
  * @brief   The interface head file of CFG module
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_C1_H
#define __CFG_C1_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "em_c1.h"
#include "cfg_autogen_c1.h"
#include "cfg_ci.h"
#include "cfg_ca.h"
#include "cfg_ct.h"







void CFG_vInit(void);
bool CFG_bCheckVaild(CFG_tstUpgCfg * cfg);




#ifdef __cplusplus
}
#endif

#endif /* __CFG_C1_H */




