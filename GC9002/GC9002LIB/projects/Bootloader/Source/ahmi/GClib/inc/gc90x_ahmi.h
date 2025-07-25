/**
  ******************************************************************************
  * @file    gc90x_ahmi.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the AHMI 
  *          firmware library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90x_AHMI_H
#define __GC90x_AHMI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
//#include "CPU1_CMSDK_CM0.h"
//#include "AHMIDriverParameters.h"
#include "gc90xx.h"

#define DRV_DISPLAY_AHB_SLAVE_BASE_ADDR     (0x30000000U)

/** @addtogroup GC90x_StdPeriph_Driver
  * @{
  */

/** @addtogroup AHMI
  * @{
  */ 

/** @defgroup AHMI_Exported_Types
  * @{
  */ 

/** 
  * @brief  AHMI Init Structure definition  
  */ 
typedef struct
{
	uint32_t OUTPUT_Rotation;										/* output rotation angle */
	uint8_t XDIR_StartTile;											/* X direction of start tile */
	uint8_t YDIR_StartTile;											/* Y direction of start tile */
	uint8_t XDIR_EndTile;												/* X direction of end tile */													
	uint8_t YDIR_EndTile;												/* Y direction of end tile */
	uint32_t MAX_SB;														/* maximum elements of source buffer */
	uint32_t SB_BASE;														/* source buffer base address */
	uint32_t FB_BASE;														/* frame buffer base address */
	uint16_t FRAME_Width;												/* frame width */
	uint16_t FRAME_Hight;												/* frame hight */
	uint32_t OUTPUT_FORMAT;		/*the format of output, 0 is rgb 565 and 1 is rgb 8888*/
} AHMI_InitTypeDef;

typedef struct
{
	__IO uint32_t SIR;
	__IO uint32_t DV;
	__IO uint32_t CT;
	__IO uint32_t SR;
	__IO uint32_t STR;
	__IO uint32_t IMR;
	__IO uint32_t ORR;
	__IO uint32_t DTBR;
	__IO uint32_t MER;
	__IO uint32_t RESERVED0[1];
	__IO uint32_t FBR;
	__IO uint32_t INTERRUPT;
	__IO uint32_t FSR;
	__IO uint32_t OUTPUTFORMAT;
	__IO uint32_t FILTERRANGE;
	uint32_t  RESERVED1[5];
	__IO uint32_t FRAMECOUNT;
	uint32_t  RESERVED2[3];
	__IO uint32_t RCR0;
	__IO uint32_t RCR1;
	uint32_t  RESERVED3[2];
	__IO uint32_t DIR0;
	__IO uint32_t DIR1;
	uint32_t  RESERVED4[2];
	__IO uint32_t CCLR0;
	__IO uint32_t CCUR0;
	__IO uint32_t CCLR1;
	__IO uint32_t CCUR1;
	uint32_t  RESERVED5[4];
	__IO uint32_t DPCLR0;
	__IO uint32_t DPCUR0;
	__IO uint32_t DPCLR1;
	__IO uint32_t DPCUR1;
	uint32_t  RESERVED6[4];
	__IO uint32_t EWC;   		//enable window cut
	__IO uint32_t WCP0;			//cut window coordinate p0 {x0,y0}, 1.11.4
	__IO uint32_t WCP1;			//cut window coordinate p1 {x1,y1}, 1.11.4
	__IO uint32_t WCP2;			//cut window coordinate p2 {x2,y2}, 1.11.4
	__IO uint32_t WCP3;			//cut window coordinate p3 {x3,y3}, 1.11.4
	__IO uint32_t YW;			//rgb to y weight
	__IO uint32_t SBA;			//sram base addr
}	AHMI_TypeDef;

#define AHMI_BASE								0x40104000
#define AHMI								 ((AHMI_TypeDef *) AHMI_BASE)
/**
  * @}
  */ 

/** @defgroup AHMI_Exported_Constants
  * @{
  */ 

/** @defgroup ahmi core number 
	* @{
	*/

#define CORE0																	((uint8_t)0x00)
#define CORE1																	((uint8_t)0x01)
#define	IS_AHMI_CORE(CORE)										(((CORE) == CORE0) || \
																							((CORE) == CORE1))

/**
	*	@}
	*/

/** @defgroup ahmi interrupt status
	* @{
	*/

#define ST_ALTOR32_DONE												((uint32_t)0x80000000)		/* altor32 done status mask */
#define ST_DC_RDOVER													((uint32_t)0x00000008)		/* dram cache read time over status mask */
#define ST_SB_ENF															((uint32_t)0x00000004)		/* source buffer ending not found status mask */
#define ST_SB_RDOVER													((uint32_t)0x00000002)		/* source buffer read time over status mask */
#define ST_DRAW_DONE													((uint32_t)0x00000001)		/* drawing done status mask */
#define IS_AHMI_STATUS(STATUS)								(((STATUS) == ST_ALTOR32_DONE) || \
																							((STATUS) == ST_DC_RDOVER) || \
																							((STATUS) ==	ST_SB_ENF) || \
																							((STATUS) == ST_SB_RDOVER) || \
																							((STATUS) == ST_DRAW_DONE)
	
/**
	*	@}
	*/

/** @defgroup ahmi interrupt
	* @{
	*/

#define IT_INTERRUPT_EN												((uint32_t)0x80000000)		/* dram cache read time over interrupt */
#define IT_DC_RDOVER													((uint32_t)0x00000008)		/* dram cache read time over interrupt */
#define IT_SB_ENF															((uint32_t)0x00000004)		/* source buffer ending not found interrupt */
#define IT_SB_RDOVER													((uint32_t)0x00000002)		/* source buffer read time over interrupt */
#define IT_DRAW_DONE													((uint32_t)0x00000001)		/* drawing done interrupt */
#define IS_AHMI_IT(IT)												(((IT) == IT_INTERRUPT_EN) || \
																							((IT) == ST_DC_RDOVER) || \
																							((IT) ==	ST_SB_ENF) || \
																							((IT) == ST_SB_RDOVER) || \
																							((IT) == ST_DRAW_DONE))
	
/**
	*	@}
	*/

/** @defgroup ahmi output rotation angle 
	* @{
	*/
	
#define DEGREE0																((uint32_t)0x00000000)		/* 0 degree */				
#define DEGREE90															((uint32_t)0x00000001)		/* 90 degree */
#define DEGREE180															((uint32_t)0x00000002)		/* 180 degree */
#define DEGREE270															((uint32_t)0x00000003)		/* 270 degree */
#define IS_AHMI_OUTPUT_ROTATION(ROTATION)			(((ROTATION) == DEGREE0) || \
																							((ROTATION) == DEGREE90) || \
																							((ROTATION) == DEGREE180) || \
																							((ROTATION) == DEGREE270))
																							
/**
	*	@}
	*/


/** @defgroup ahmi output format 
	* @{
	*/
	
#define OUTPUTFORMAT_RGBA8888														((uint32_t)0x00000000)			
#define OUTPUTFORMAT_RGB565															((uint32_t)0x00000001)
#define OUTPUTFORMAT_RGB888															((uint32_t)0x00000002)
#define OUTPUTFORMAT_SFBC															((uint32_t)0x00000003)
#define OUTPUTFORMAT_Y8																((uint32_t)0x00000004)

																							
/**
	*	@}
	*/



/**	@defgroup	core drawing information
	*	@{
	*/
				
#define INF_CORE_EN														((uint32_t)0x80000000)		/* core enable status */
#define	INF_CORE_RASTER_BUSY									((uint32_t)0x00000004)		/* core raster busy status */
#define	INF_CORE_RASTER_DONE									((uint32_t)0x00000002)		/* core raster done status */
#define	INF_CORE_TB_DONE											((uint32_t)0x00000001)		/* core TB done status */
#define IS_CORE_INF(INF)											(((INF) == INF_CORE_EN)	|| \
																							((INF) == INF_CORE_RASTER_BUSY) || \
																							((INF) == INF_CORE_RASTER_DONE) || \
																							((INF) == INF_CORE_TB_DONE))

/**
	*	@}
	*/
	
void AHMI_DeInit(void);
void AHMI_Init(AHMI_InitTypeDef* AHMI_InitStruct);
void Reset_AHMI(void);
void GateClock_Enable(void);
void GateClock_Disable(void);
void DramCache_Reset(void);
void AHMI_StructInit(AHMI_InitTypeDef* AHMI_InitStruct);
void CORE_Cmd(uint8_t COREx, FunctionalState NewState);
uint8_t GPE_Cmd(FunctionalState NewState);

uint8_t AHMI_GetStatus(uint32_t AHMI_Status);
void AHMI_ITConfig(uint32_t AHMI_IT, FunctionalState NewState);
void AHMI_InterruptConfig(FunctionalState NewState);
uint8_t AHMI_GetITStatus(uint32_t AHMI_Status);
void AHMI_ClearITStatus(uint32_t AHMI_IT);
void AHMI_SetRot(uint32_t Rotation);
void AHMI_TileBox(uint8_t XStart, uint8_t YStart, uint8_t XEnd, uint8_t YEnd);
void AHMI_SetSBSize(uint32_t Size);
void AHMI_WaitDrawDone(void);
void AHMI_SetSBAdress(uint32_t SBAdress, uint32_t StartAddr, uint32_t SizeOfWord);
void AHMI_SetFBAdress(uint32_t FBAdress);
uint32_t AHMI_GetFBAdress(void);
uint32_t AHMI_GetOutputFormat(void);
void AHMI_SetFSize(uint16_t Width, uint16_t Height);
uint32_t AHMI_GETCoreCallCount(uint8_t Corex);
void AHMI_SETCoreCallCount(uint8_t Corex, uint32_t count);
FlagStatus AHMI_GetCoreDrawStatus(uint8_t Corex, uint32_t Information);
void AHMI_GetLowerBusyCount(uint8_t Corex, uint32_t* count);
void AHMI_GetUpperBusyCount(uint8_t Corex, uint32_t* count);
void AHMI_GetLowerDrawCount(uint8_t Corex, uint32_t* count);
void AHMI_GetUpperDrawCount(uint8_t Corex, uint32_t* count);
void AHMI_SetWindowCoordinate(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
void AHMI_DisableWindowCut(void);
void AHMI_SetDWINAlphaSeuqence(void);
void AHMI_SetAHMIAlphaSeuqence(void);
void AHMI_Config(void);


#ifdef __cplusplus
}
#endif

#endif /* __GC90x_AHMI_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
