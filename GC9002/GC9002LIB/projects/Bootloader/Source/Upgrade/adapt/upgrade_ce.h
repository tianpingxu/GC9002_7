/**
  ******************************************************************************
  * @file    upgrade_ce.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    05-01-2021
  * @brief   The external configuration head file of Upgrade module
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPGRADE_CE_H
#define __UPGRADE_CE_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "core_rv32.h"
#include <csi_config.h>
#include "csi_irq.h"
#include "csi_irq_vectors.h"
#include "gc90xx_hal.h"





/* Config if need verify data correction by reading data after write */
#define UPG_VERIFY_READ_AFTER_WRITE                         (0)


#define UPG_ASSERT(x)                                       if((x) == 0) { taskDISABLE_INTERRUPTS(); while(1); }

#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_CE_H */




