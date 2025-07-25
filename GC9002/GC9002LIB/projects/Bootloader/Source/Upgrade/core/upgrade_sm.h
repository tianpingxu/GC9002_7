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
#ifndef __UPGRADE_SM_H
#define __UPGRADE_SM_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#include "upgrade_ct.h"
#include "upgrade_ci.h"
#include "upgrade_subsm.h"




void UPG_SM__vMainSmProcess(void);

void UPG_SM__vInit(void);


	 

#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_SM_H */




