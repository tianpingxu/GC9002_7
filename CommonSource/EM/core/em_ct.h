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

typedef enum {TRUE_1 = 0, FALSE_0 = !TRUE_1} EM_tenErrBool;

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
	//uint8_t                              u8ModuleIndex;                  /* Module index */
    //uint8_t                              u8ErrLevel;                     /* Error Level */
    //uint16_t                             u16OccurCount;                  /* Error occur count */
    //uint32_t                             u32ExtraInfo;
	uint16_t                             u16ErrType;				/* Error Type */
    uint16_t                             u16ErrLevel;				/* Error Level */
    uint16_t                             u16OccurCount;				/* Error occur count */
	uint16_t                             u16PageID;					/* Error Page ID */
    uint16_t                             u16CanvasID;				/* Error Canvas ID */
	uint16_t                             u16SubcanvasID;			/* Error Subcanvas ID */
    uint16_t                             u16WidgetID;				/* Error Widget ID */
	uint16_t                             u16TagID;					/* Error Tag ID */
	uint32_t                             u32TagValue;				/* Error Tag Value */
}EM_tstErrItem;


typedef enum
{
    EM_enErrRetModularInit = 0x10000,
    EM_enErrRetModularActionProcessor = 0x20000,
    EM_enErrRetModularAnimation,
    EM_enErrRetModularPage,
    EM_enErrRetModularCanvas,
    EM_enErrRetModularSubcanvas,
	EM_enErrRetModularWidget,
	EM_enErrRetModularTexture,
	EM_enErrRetModularEasing,
	EM_enErrRetModularEncoding,
	EM_enErrRetModularFontlayoutEngine,
	EM_enErrRetModularMatrix,
	EM_enErrRetModularMyMath,
	EM_enErrRetModularRefreshQueue,
	EM_enErrRetModularTag,
	EM_enErrRetModularInterval,
	EM_enErrRetModularWaveFilter
   
}EM_tenErrRetModular;

	 

#ifdef __cplusplus
}
#endif

#endif /* __EM_CT_H */




