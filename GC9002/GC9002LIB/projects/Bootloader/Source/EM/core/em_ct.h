/**
  ******************************************************************************
  * @file    em_ct.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    12-07-2021
  * @brief   The data structure proto definition of Error Management module.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EM_CT_H
#define __EM_CT_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "em_ci.h"



typedef enum
{
#define CFG(a, b)                   a,

    EM_ALL_ERROR_INFO_TABLE                                         /* No need a comma at the end */
    EM_enErrCount
    
#undef CFG  
}EM_tenErrType;


typedef enum
{
    EM_enErrLevelNoErr = 0,
    EM_enErrLevelInfo,
    EM_enErrLevelWarning,
    EM_enErrLevelSerious,
    EM_enErrLevelFatal,
    EM_enErrLevelRecover
   
}EM_tenErrLevel;


typedef struct 
{	
    uint8_t                              u8ModuleIndex;                  /* Module index */
    uint8_t                              u8ErrLevel;                     /* Error Level */
    uint16_t                             u16OccurCount;                  /* Error occur count */
    uint32_t                             u32ExtraInfo;
}EM_tstErrItem;




	 

#ifdef __cplusplus
}
#endif

#endif /* __EM_CT_H */




