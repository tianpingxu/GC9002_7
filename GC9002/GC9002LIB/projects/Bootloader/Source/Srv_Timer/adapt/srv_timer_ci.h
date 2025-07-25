/**
  ******************************************************************************
  * @file    srv_timer_ci.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    08-07-2021
  * @brief   The internal configuration head file of Timer serve module.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRV_TIMER_CI_H
#define __SRV_TIMER_CI_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "csi_irq.h"



/* Enter critical status */
#define SRV_TIMER_ENTER_CRITICAL            __disable_irq()

/* exit critical status */
#define SRV_TIMER_EXIT_CRITICAL             __enable_irq()

#ifdef __cplusplus
}
#endif

#endif /* __SRV_TIMER_CI_H */




