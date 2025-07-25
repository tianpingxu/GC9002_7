/**
  ******************************************************************************
  * @file    upgrade_passive.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    11-19-2022
  * @brief   The head file of Passive Upgrade module interface.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPGRADE_PASSIVE_H
#define __UPGRADE_PASSIVE_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/





int32_t UPG_PAS_i32SetUpgFileAndSize(uint8_t * pAddr, uint32_t size);
int32_t UPG_PAS_i32GetData(uint8_t * pAddr, uint32_t size);
int32_t UPG_PAS_i32VerifyFlash(void);

	 

#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_PASSIVE_H */




