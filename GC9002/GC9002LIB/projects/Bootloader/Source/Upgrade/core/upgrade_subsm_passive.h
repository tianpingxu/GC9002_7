/**
  ******************************************************************************
  * @file    upgrade_sm.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    23-06-2021
  * @brief   The head file of Upgrade State Machine.
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPGRADE_SUBSM_PASSIVE_H
#define __UPGRADE_SUBSM_PASSIVE_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#include "upgrade_ct.h"
#include "upgrade_ci.h"
#include "upgrade_subsm.h"





void UPG_SUBSM_PAS_vCanUartRunQuit(void);
void UPG_SUBSM_PAS_vUartWork(void);
void UPG_SUBSM_PAS_vCanWork(void);
void UPG_SUBSM_PAS__vProcess(UPG_tstWorkBuf *pstWorkBuf);	 

#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_SUBSM_PASSIVE_H */




