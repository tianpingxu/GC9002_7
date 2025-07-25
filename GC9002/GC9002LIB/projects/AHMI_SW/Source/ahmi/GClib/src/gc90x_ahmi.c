/**
  ******************************************************************************
  * @file    gc90x_AHMI.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file provides all the AHMI firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "gc90x_ahmi.h"
#include "display_conf.h"
//#include "CPU1_CMSDK_CM0.h"
//#include "globalPointer.h"
#include "publicDefine.h"
#include "drawImmediately_cd.h"
#include "SBBatchDraw.h"

//extern u8 sourceBuffer[SoureBufferSize];
extern uint32_t sourceshift;

/** @addtogroup GC90x_StdPeriph_Driver
  * @{
  */

/** @defgroup AHMI 
  * @brief AHMI driver modules
  * @{
  */

/** @defgroup AHMI_Private_Defines											
  * @{
  */



/* ahmi interrupt enable mask */
#define AHMI_IT_EN															((uint32_t)0x80000000)
#define AHMI_IT_DIS															((uint32_t)0x7fffffff)

/* dram cache time over interrupt mask */
#define IT_DC_RDOVER_EN													((uint32_t)0x00000008)
#define IT_DC_RDOVER_DIS												((uint32_t)0xfffffff7)

/* source buffer ending not found interrupt mask */
#define IT_SB_ENF_EN														((uint32_t)0x00000004)
#define IT_SB_ENF_DIS														((uint32_t)0xfffffffb)

/* source buffer read time over interrupt mask */
#define IT_SB_RDOVER_EN													((uint32_t)0x00000002)
#define IT_SB_RDOVER_DIS												((uint32_t)0x0000000d)

/* drawing done mask */
#define IT_DRAW_DONE_EN													((uint32_t)0x00000001)
#define IT_DRAW_DONE_DIS												((uint32_t)0xfffffffe)

/* core0 enable mask */
#define CORE0_EN																((uint32_t)0x00000001)
#define CORE0_DIS																((uint32_t)0xfffffffe)

/* core1 enable mask */
#define	CORE1_EN																((uint32_t)0x00000002)
#define	CORE1_DIS																((uint32_t)0x0000000d)

/* GPE start mask */
#define GPE_START																((uint32_t)0x00000001)
#define GPE_END																	((uint32_t)0xfffffffe)

/* DWIN Alpha secquence mask */
#define DWIN_ALPHA																((uint32_t)0x00000010)
#define AHMI_ALPHA																((uint32_t)0xffffffef)

/* Enable gate clock. */
#define GATECLOCK_EN															((uint32_t)0x10000000)
#define GATECLOCK_DIS															((uint32_t)0xefffffff)

/* Reset Dram Cache. */
#define DRAMCACHEReset_EN														((uint32_t)0x00000002)
#define DRAMCACHEReset_DIS														((uint32_t)0xfffffffd)

/* output rotation mask */
#define OUTPUT_ROTATION_MASK										((uint32_t)0xfffffffc)
/**
  * @}
  */

/** @defgroup AHMI_Private_Functions
  * @{
  */
	
/**
  * @brief  Deinitializes the AHMI registers to their default reset values.
  * @param        	
  * @retval None
  */
void AHMI_DeInit(void)
{
	//APBCLK1_OFF;
}

/**
  * @brief  Initializes the AHMI.
  * @param  AHMI_InitStruct: pointer to a AHMI_InitTypeDef structure
  *         that contains the configuration information for the AHMI.
  * @retval None
  */
extern uint32_t DISPLAY_GetMode(void);
extern uint32_t DISPLAY_u32GetInputColorMode(void);

void AHMI_Init(AHMI_InitTypeDef* AHMI_InitStruct)
{
	uint32_t tmpreg = 0;
	/* check the parameters */
//	assert_param(IS_AHMI_OUTPUT_ROTATION(AHMI_InitStruct->OUTPUT_Rotation));
	
	/* enable interrupt of ahmi */
	tmpreg = AHMI->IMR;
	tmpreg |= (uint32_t)(/*AHMI_IT_EN |*/ IT_DC_RDOVER_EN | IT_SB_ENF_EN |IT_SB_RDOVER_EN | IT_DRAW_DONE_EN);
	AHMI->IMR = tmpreg;
	
	/* set output rotation according to OUTPUT_Rotation */
	tmpreg = AHMI->ORR;
	tmpreg &= OUTPUT_ROTATION_MASK;
	tmpreg |= (AHMI_InitStruct->OUTPUT_Rotation);//bit4 means output_trans; bit8 means dither enable(hardware dither exist bug, can't enable!!!);
	AHMI->ORR = tmpreg;
	
	/* set X/Y direction start and end tile according to XDIR_StartTile,YDIR_StartTile,XDIR_EndTile,YDIR_EndTile */
	AHMI->DTBR = (uint32_t)(AHMI_InitStruct->XDIR_StartTile | (AHMI_InitStruct->YDIR_StartTile << 8)
										| (AHMI_InitStruct->XDIR_EndTile << 16) | AHMI_InitStruct->YDIR_EndTile << 24);
	
	/* set maximum size of source buffer according to MAX_SB */
	AHMI->MER = AHMI_InitStruct->MAX_SB;
	
	/* set source buffer base address according to SB_BASE */
	//AHMI->SBR = AHMI_InitStruct->SB_BASE;
	
	/* set frame buffer base address according to FB_BASE */
	if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	{
		AHMI->FBR = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
	}
	else
	{
		AHMI->FBR = AHMI_InitStruct->FB_BASE;
	}
	
	/* set frame width and hight according to FRAME_Width and FRAME_Hight */
	AHMI->FSR = (uint32_t)(AHMI_InitStruct->FRAME_Width | (AHMI_InitStruct->FRAME_Hight << 16));
	
	/* set output format */
	AHMI->OUTPUTFORMAT = (uint32_t)(AHMI_InitStruct->OUTPUT_FORMAT);
	
	/* GPE start */
//	AHMI->SR |= GPE_START;

	/* wait until drawing done */
//	while(!(AHMI->STR & ST_DRAW_DONE));
}

/**
  * @brief  Fills each AHMI_InitStruct member with its default value.
  * @param  AHMI_InitStruct: pointer to a AHMI_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void AHMI_StructInit(AHMI_InitTypeDef* AHMI_InitStruct)
{
  /* AHMI_InitStruct members default value */
	AHMI_InitStruct->OUTPUT_Rotation = DEGREE0;
	AHMI_InitStruct->XDIR_StartTile = 0;//((DISPLAY->col_update_area & 0xFFFF) + 1) / 16;
	AHMI_InitStruct->YDIR_StartTile = 0;//((DISPLAY->row_update_area & 0xFFFF) + 1) / 16;
	AHMI_InitStruct->XDIR_EndTile = (SCREEN_WIDTH+15)/16 - 1;//AHMI_InitStruct->XDIR_StartTile + ((((DISPLAY->timing_param0 & 0xFFFF0000) >> 16)+15)/16 - 1);
	AHMI_InitStruct->YDIR_EndTile = (SCREEN_HEIGHT+15)/16 - 1;//AHMI_InitStruct->YDIR_StartTile + ((((DISPLAY->timing_param2 & 0xFFFF0000) >> 16)+15)/16 - 1);
	AHMI_InitStruct->MAX_SB = 0x00000400;
//	AHMI_InitStruct->SB_BASE = SOURCE_BUFFER_START_ADDR;
	AHMI_InitStruct->FB_BASE = StartAddrOfDisplayBuffer; 
//	AHMI_InitStruct->FRAME_Width = DISPLAY_WIDTH;       
//	AHMI_InitStruct->FRAME_Hight = DISPLAY_HEIGHT;
    AHMI_InitStruct->FRAME_Width = ((DISPLAY->timing_param0 & 0xFFFF0000) >> 16);//SCREEN_WIDTH
	AHMI_InitStruct->FRAME_Hight = ((DISPLAY->timing_param2 & 0xFFFF0000) >> 16);//SCREEN_HEIGHT
	AHMI_InitStruct->OUTPUT_FORMAT = OUTPUTFORMAT_RGB565;
	/* Adjust AHMI Output Format depends on Display request */
    if(DISPLAY_InputColorMode_RGBA8888 == DISPLAY_u32GetInputColorMode())
    {
        AHMI_InitStruct->OUTPUT_FORMAT = DISPLAY_InputColorMode_RGBA8888;
    }
}

/**
  * @brief  Fills each AHMI_InitStruct member with its default value.
  * @param  AHMI_InitStruct: pointer to a AHMI_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void AHMI_StructInit_UpdateRange(AHMI_InitTypeDef* AHMI_InitStruct)
{
	/* AHMI_InitStruct members default value */
	AHMI_InitStruct->OUTPUT_Rotation = AHMI->ORR;
	if((AHMI->OUTPUTFORMAT == DEGREE90 || AHMI->OUTPUTFORMAT == DEGREE270) && 
	  ((DISPLAY_GetMode() == DISPLAY_Mode_MasterActive) || (DISPLAY_GetMode() == DISPLAY_Mode_MasterPassive)))
	{
		AHMI_InitStruct->XDIR_StartTile = ((DISPLAY->row_update_area & 0xFFFF) + 1) / 16;
		AHMI_InitStruct->YDIR_StartTile = ((DISPLAY->col_update_area & 0xFFFF) + 1) / 16;
		AHMI_InitStruct->XDIR_EndTile = AHMI_InitStruct->XDIR_StartTile + ((((DISPLAY->timing_param2 & 0xFFFF0000) >> 16)+15)/16 - 1);
		AHMI_InitStruct->YDIR_EndTile = AHMI_InitStruct->YDIR_StartTile + ((((DISPLAY->timing_param0 & 0xFFFF0000) >> 16)+15)/16 - 1);
	}
	else
	{
		AHMI_InitStruct->XDIR_StartTile = ((DISPLAY->col_update_area & 0xFFFF) + 1) / 16;
		AHMI_InitStruct->YDIR_StartTile = ((DISPLAY->row_update_area & 0xFFFF) + 1) / 16;
		AHMI_InitStruct->XDIR_EndTile = AHMI_InitStruct->XDIR_StartTile + ((((DISPLAY->timing_param0 & 0xFFFF0000) >> 16)+15)/16 - 1);
		AHMI_InitStruct->YDIR_EndTile = AHMI_InitStruct->YDIR_StartTile + ((((DISPLAY->timing_param2 & 0xFFFF0000) >> 16)+15)/16 - 1);
	}
	AHMI_InitStruct->MAX_SB = AHMI->MER;
	AHMI_InitStruct->FB_BASE = AHMI->FBR; 
    AHMI_InitStruct->FRAME_Width = (AHMI->FSR & 0xFFFF);
	AHMI_InitStruct->FRAME_Hight = ((AHMI->FSR & 0xFFFF0000) >> 16);
	AHMI_InitStruct->OUTPUT_FORMAT = AHMI->OUTPUTFORMAT;
}

/**
  * @brief  Set source buffer base address.
  * @param  address: base address for source buffer.
  * @retval None
  */

void AHMI_SetSBAddress(uint32_t address)
{
    if(address == 0)
        return;
    
	/* set source buffer base address according to SB_BASE */
	//AHMI->SBR = address;	
}

/**
  * @brief  Set frame buffer base address.
  * @param  address: base address for frame buffer.
  * @retval None
  */

void AHMI_SetFBAddress(uint32_t address)
{
    if(address == 0)
        return;
    	
	/* set frame buffer base address according to FB_BASE */
	AHMI->FBR = address;
}

/**
  * @brief  Enables or disables the specified AHMI core.
	*	@param	COREx: core number.
	*					This parameter can be: CORE0 or CORE1.
	* @param  NewState: new state of the core.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CORE_Cmd(uint8_t COREx, FunctionalState NewState)
{
	/* Check the parameters */
//	assert_param(IS_AHMI_CORE(COREx));
//	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if (NewState != DISABLE)
  {
		/* Enable specified core */
		if (COREx == CORE0)
		{
			AHMI->SIR |= CORE0_EN;
		}
		else
		{
			AHMI->SIR |= CORE1_EN;
		}
  }
  else
  {
		/* Disable specified core */
		if (COREx == CORE0)
		{
			AHMI->SIR &= CORE0_DIS;
		}
		else
		{
			AHMI->SIR &= CORE1_DIS;
		}
	}
}


void gc90x_ahmi_delay(int i)
{
//LOOP
//	SUBS r0,r0,#1
//	CMP r0,#0
//	BNE LOOP
//	BX lr	
}


/**
  * @brief  Reset AHMI.
	* @param  NewState: new state of the GPE.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void Reset_AHMI()
{
	/* AHMI reset */
	AHMI->CT |= 0x1;
    gc90x_ahmi_delay(100);
    AHMI->CT &= (~0x1);
    gc90x_ahmi_delay(100);
}


/**
  * @brief  Enable gate clock.
  * @param  
  * @retval None
  */
void GateClock_Enable(void)
{
	/* GPE start */
	AHMI->CT |= GATECLOCK_EN;
}

/**
  * @brief  Enable gate clock.
  * @param  
  * @retval None
  */
void GateClock_Disable(void)
{
	/* GPE start */
	AHMI->CT &= GATECLOCK_DIS;
}

/**
  * @brief  Reset DramCache.
  * @param  
  * @retval None
  */
void DramCache_Reset(void)
{
	/* DramCache reset */
	AHMI->CT |= DRAMCACHEReset_EN;
	AHMI->CT &= DRAMCACHEReset_DIS;
}

/**
  * @brief  Enables or disables GPE.
	* @param  NewState: new state of the GPE.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPE_Cmd(FunctionalState NewState)
{
    int x;
	/* Check the parameters */
//	assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    
    //if last draw call is not complete, dont start a new draw call
	if(AHMI->STR == 0)
    {
        x++;
        return;
    }
        
	if (NewState != DISABLE)
	{
		/* GPE start */
		AHMI->SR |= GPE_START;
		while(((AHMI->STR & 0x1) == 1));
	}
	else
	{
		/* GPE end */
		AHMI->SR &= GPE_END;
	}
}

/**
	*	@brief get AHMI specified status.
	*	@param AHMI_Status: specified ahmi status.
	*				 This parameter can be: ST_ALTOR32_DONE,ST_DC_RDOVER,ST_SB_ENF,ST_SB_RDOVER,ST_DRAW_DONE.
	*	@retval return specified status.
	*/
uint8_t AHMI_GetStatus(uint32_t AHMI_Status)
{
	uint8_t bitstatus = RESET;
	/* Check the parameters */
	//assert_param(IS_AHMI_STATUS(AHMI_Status));
	
	if (AHMI->STR & AHMI_Status)
	{
		bitstatus = SET;
	}
	else 
	{
		bitstatus = RESET;
	}
	
	return bitstatus;
}

/**
  * @brief  Enables or disables the specified AHMI interrupts.
  * @param  AHMI_IT: specifies the AHMI interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
	*			@arg IT_INTERRUPT_EN interrupt enable.
	*			@arg IT_DC_RDOVER dram cache read time over interrupt.
  *     @arg IT_SB_ENF source buffer ending not found interrupt.
	*			@arg IT_SB_RDOVER source buffer read time over interrupt.
	*			@arg IT_DRAW_DONE drawing done interrupt.
  * @param  NewState: new state of the specified AHMI interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
	*/
void AHMI_ITConfig(uint32_t AHMI_IT, FunctionalState NewState)
{
	/* Check the parameters */
//	assert_param(IS_AHMI_IT(AHMI_IT));
//	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if (NewState != DISABLE)
	{
		/* enable the specified interrupt */
		AHMI->IMR |= AHMI_IT;
	}
	else
	{
		/* disable the specified interrupt */
		AHMI->IMR &= ~AHMI_IT;
	}
}

/**
  * @brief  Enables or disables AHMI interrupts.
  * @param  NewState: new state of the specified AHMI interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
	*/
void AHMI_InterruptConfig(FunctionalState NewState)
{
	uint32_t tmpreg = 0;
	/* Check the parameters */
//	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if(NewState == ENABLE)
	{
		tmpreg = AHMI->IMR;
//		tmpreg |= 0x80000001;
        tmpreg |= 0x00000001;
		AHMI->IMR = tmpreg;
	}
	else
	{
		tmpreg = AHMI->IMR;
//		tmpreg &= 0x7FFFFFFF;
//		tmpreg |= 0x00000001;
        tmpreg &= 0x00000000;
		AHMI->IMR = tmpreg;
	}
	
	//AHMI->INTERRUPT = NewState;

}

/**
	*	@brief get AHMI specified interrupt status.
	*	@param AHMI_Status: specified ahmi status.
	*				 This parameter can be: ST_DC_RDOVER,ST_SB_ENF,ST_SB_RDOVER,ST_DRAW_DONE.
	*	@retval return specified interrupt status.
	*/
uint8_t AHMI_GetITStatus(uint32_t AHMI_Status)
{
	uint8_t bitstatus = RESET;
	/* Check the parameters */
//	assert_param(IS_AHMI_IT(AHMI_Status));
	
	if (AHMI->INTERRUPT & AHMI_Status)
	{
		bitstatus = SET;
	}
	else 
	{
		bitstatus = RESET;
	}
	
	return bitstatus;
}

/**
	*	@brief clear AHMI specified interrupt status.
	*	@param AHMI_IT: specified interrupt.
	*	@retval None.
	*/
void AHMI_ClearITStatus(uint32_t AHMI_IT)
{
//	ITStatus bitstatus = RESET;
	/* Check the parameters */
//	assert_param(IS_AHMI_IT(AHMI_IT));
	
	AHMI->INTERRUPT &= ~AHMI_IT;
}

/**
	*	@brief set output rotation.
	*	@param Rotation: output rotation.
	*				 This parameter can be: DEGREE0,DEGREE90,DEGREE180,DEGREE270.
	*	@retval None.
	*/
extern uint32_t DISPLAY_GetInterfaceMode(void);
void AHMI_SetRot(uint32_t Rotation)
{
	uint32_t tmpreg = 0;
	/* check the parameters */
//	assert_param(IS_AHMI_OUTPUT_ROTATION(Rotation));

	if(DISPLAY_GetInterfaceMode() != DISPLAY_InterfaceType_24BIT)
	{
		AHMI->ORR &= OUTPUT_ROTATION_MASK;
	}
	else 
	{
		/* config output rotation register according to Rotation */
		tmpreg = AHMI->ORR;
		tmpreg &= OUTPUT_ROTATION_MASK;
		tmpreg |= Rotation;
		AHMI->ORR = tmpreg;
	}
}

/**
	*	@brief set tile box
	*	@param XStart: X direction of start tile.
	* @param YStart: Y direction of start tile.
	*	@param X direction of end tile.
	* @param Y direction of end tile.
	*	@retval None.
	*/
void AHMI_TileBox(uint8_t XStart, uint8_t YStart, uint8_t XEnd, uint8_t YEnd)
{
	AHMI->DTBR = (uint32_t)(XStart | YStart << 8 | XEnd << 16 | YEnd << 24);
}

/**
	*	@brief set maximum size of source buffer.
	*	@param maximum size of source buffer.
	*	@retval None.
	*/
void AHMI_SetSBSize(uint32_t Size)
{
	AHMI->MER = Size;
}

/**
	*	@brief wait for ahmi core draw done.
	*	@param None.
	*	@retval None.
	*/
void AHMI_WaitDrawDone(void)
{
	while( ((AHMI->STR & 0x1) == 0))
    {
    ;};
}

/**
	*	@brief set source buffer base address.
	*	@param SBAdress: source buffer base address.
	*	@retval None.
	*/
void AHMI_SetSBAdress(uint32_t SBAdress)
{
	//AHMI->SBR = SBAdress;
	uint32_t i = 0;
	
	#ifndef CHIP_GC9002_NoHyper
	if((SBAdress == 0x0200c000) || (sourceshift == 0))
		return;
	
	AHMI_WaitDrawDone();
	
	for(i = 0; i < 0x200; i++)
	{
		*(uint32_t *)(0x0200c000 + 4 * i) = *(uint32_t *)(SBAdress + 4 * i);
	}
	#endif
}

/**
	*	@brief Get ahmi core draw done flag.
	*	@param None.
	*	@retval None.
	*/
uint8_t AHMI_GetDrawDoneFlag(void)
{
	return (AHMI->STR & 0x1);
}

void AHMI_SetSBAdressForSBBatchDraw(uint32_t DesStartOffsetAddr, uint32_t SrcStartAddr, uint32_t size)
{
	uint32_t i = 0;
	
	if((DesStartOffsetAddr + size) > SBBatchDrawSBAddrMax)
	{
		return;
	}
	
	AHMI_WaitDrawDone();
	
	for(i = 0; i < size/4; i++)
	{
		*(uint32_t *)(0x0200c000 + DesStartOffsetAddr + 4 * i) = *(uint32_t *)(SrcStartAddr + 4 * i);
	}
	
	if((DesStartOffsetAddr != 0) || (size != SBBatchDrawBackgroundSBLength))
	{
		if(((*(uint32_t *)(0x0200c000 + DesStartOffsetAddr + size - 24)) & 0xFFFF7E00) == 0x80000000)
		{
			*(uint32_t *)(0x0200c000 + DesStartOffsetAddr + size - 24) |= 0x8000;
		}
		else
		{
			*(uint32_t *)(0x0200c000 + DesStartOffsetAddr + size - 64) |= 0x8000;
		}
	}
}

/**
	*	@brief set frame buffer base address.
	*	@param FBAdress: frame buffer base address.
	*	@retval None.
	*/
void AHMI_SetFBAdress(uint32_t FBAdress)
{
	if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	{
		AHMI->FBR = 0x30000000;
	}
	else
	{
		AHMI->FBR = FBAdress;
	}
}

uint32_t AHMI_GetFBAdress(void)
{
	return AHMI->FBR;
}

uint32_t AHMI_GetOutputFormat(void)
{
	return AHMI->OUTPUTFORMAT;
}

/**
	*	@brief set frame size.
	*	@param Width: frame width.
	*	@param Height: frame height.
	*	@retval None.
	*/
void AHMI_SetFSize(uint16_t Width, uint16_t Height)
{
	AHMI->FSR = (uint32_t)(Width | Height << 16);
}

/**
	*	@brief get testure draw call count of Corex.
	*	@param Corex: core number.
	*				This parameter can be: CORE0,CORE1.
	*	@retval testure draw call count of Corex.
	*/
uint32_t AHMI_GETCoreCallCount(uint8_t Corex)
{
	uint32_t count = 0;
	/* check the parameters */
//	assert_param(IS_AHMI_CORE(Corex));
	
	if (Corex == CORE0)
	{
		/* get testure draw call count of core0 */
		count = AHMI->RCR0;
	}
	else
	{
		/* get testure draw call count of core1 */
		count = AHMI->RCR1;
	}
	
	return count;
}

/**
	*	@brief set testure draw call count of Corex.
	*	@param Corex: core number.
	*				This parameter can be: CORE0,CORE1.
	*	@retval testure draw call count of Corex.
	*/
void AHMI_SETCoreCallCount(uint8_t Corex, uint32_t count)
{
	/* check the parameters */
//	assert_param(IS_AHMI_CORE(Corex));

	if (Corex == CORE0)
	{
		/* set testure draw call count of core0 */
		AHMI->RCR0 = count;
	}
	else
	{
		/* set testure draw call count of core1 */
		AHMI->RCR1 = count;
	}
}

/**
	*	@brief get core drawing information.
	*	@param Corex: core number.
	*				This parameter can be: CORE0,CORE1.
	*	@retval draw status of Corex.
	*/
FlagStatus AHMI_GetCoreDrawStatus(uint8_t Corex, uint32_t Information)
{
	FlagStatus bitstatus = RESET;
	/* check the parameters */
//	assert_param(IS_AHMI_CORE(Corex));
//	assert_param(IS_CORE_INF(Information));
	
	if (Corex == CORE0)
	{
		if (AHMI->DIR0 & Information)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}
	else
	{
		if (AHMI->DIR1 & Information)
		{
			bitstatus = SET;
		}
		else
		{
			bitstatus = RESET;
		}
	}
	
	return bitstatus;
}

/**
	*	@brief get core lower word cache busy cycle count.
	*	@param Corex: core number.
	*				This parameter can be: CORE0,CORE1.
	*	@retval lower word cache busy cycle count of Corex.
	*/
void AHMI_GetLowerBusyCount(uint8_t Corex, uint32_t* count)
{
//	uint32_t count = 0;
	/* check the parameters */
//	assert_param(IS_AHMI_CORE(Corex));
	
	if (Corex == CORE0)
	{
		*count = AHMI->CCLR0;
	}
	else
	{
		*count = AHMI->CCLR1;
	}
	
}

/**
	*	@brief get core upper word cache busy cycle count.
	*	@param Corex: core number.
	*				This parameter can be: CORE0,CORE1.
	*	@retval upper word cache busy cycle count of Corex.
	*/
void AHMI_GetUpperBusyCount(uint8_t Corex, uint32_t* count)
{
//	uint32_t count = 0;
	/* check the parameters */
//	assert_param(IS_AHMI_CORE(Corex));
	
	if (Corex == CORE0)
	{
		*count = AHMI->CCUR0;
	}
	else
	{
		*count = AHMI->CCUR1;
	}
	
}

/**
	*	@brief get core lower word total draw pixel cycle count.
	*	@param Corex: core number.
	*				This parameter can be: CORE0,CORE1.
	*	@retval lower word total draw pixel cycle of Corex.
	*/
void AHMI_GetLowerDrawCount(uint8_t Corex, uint32_t* count)
{
//	uint32_t count = 0;
	/* check the parameters */
//	assert_param(IS_AHMI_CORE(Corex));
	
	if (Corex == CORE0)
	{
		*count = AHMI->DPCLR0;
	}
	else
	{
		*count = AHMI->DPCLR1;
	}
	
}

/**
	*	@brief get core upper word total draw pixel cycle count.
	*	@param Corex: core number.
	*				This parameter can be: CORE0,CORE1.
	*	@retval upper word total draw pixel cycle of Corex.
	*/
void AHMI_GetUpperDrawCount(uint8_t Corex, uint32_t* count)
{
//	uint32_t count = 0;
	/* check the parameters */
//	assert_param(IS_AHMI_CORE(Corex));
	
	if (Corex == CORE0)
	{
		*count = AHMI->DPCUR0;
	}
	else
	{
		*count = AHMI->DPCUR1;
	}

}

/**
	*	@brief set cut window coordinate.
	*	@param x0: x0 coordinate 1.11.4.
	*		   y0: y0 coordinate 1.11.4.
	*		   x1: x1 coordinate 1.11.4.
	*		   y1: y1 coordinate 1.11.4.
	*		   x2: x2 coordinate 1.11.4.
	*		   y2: y2 coordinate 1.11.4.
	*		   x3: x3 coordinate 1.11.4.
	*		   y3: y4 coordinate 1.11.4.
	*	@retval none.
	*/
void AHMI_SetWindowCoordinate(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3)
{
	int32_t temp;

	temp = ((x0 << 16) | y0);
	AHMI->WCP0 = temp;

	temp = ((x1 << 16) | y1);
	AHMI->WCP1 = temp;

	temp = ((x2 << 16) | y2);
	AHMI->WCP2 = temp;

	temp = ((x3 << 16) | y3);
	AHMI->WCP3 = temp;

	AHMI->EWC = 1;
}

/**
	*	@brief disable window cut.
	*	@param none.
	*	@retval none.
	*/
void AHMI_DisableWindowCut(void)
{
	AHMI->EWC = 0;
}

/**
	*	@brief set DWIN alpha byte sequence.
	*	@param none.
	*	@retval none.
	*/
void AHMI_SetDWINAlphaSeuqence(void)
{
	AHMI->SR |= DWIN_ALPHA;
}

/**
	*	@brief set AHMI alpha byte sequence.
	*	@param none.
	*	@retval none.
	*/
void AHMI_SetAHMIAlphaSeuqence(void)
{
	AHMI->SR &= AHMI_ALPHA;
}

/**
	*	@brief Config AHMI module and start gate clock.
	*	@param 
	*	@retval 
	*/
void AHMI_Config(void)
{
	AHMI_InitTypeDef AHMI_InitStruct;
	
	/* Wait AHMI Draw Done */
	while(!AHMI_GetStatus(ST_DRAW_DONE));
	
    Reset_AHMI();
	
	AHMI_ITConfig(IT_DC_RDOVER, ENABLE);
	AHMI_ITConfig(IT_SB_ENF, ENABLE);
	AHMI_ITConfig(IT_SB_RDOVER, ENABLE);
	AHMI_ITConfig(IT_DRAW_DONE, ENABLE);
    
    AHMI_StructInit(&AHMI_InitStruct);
    
	AHMI_Init(&AHMI_InitStruct);

	AHMI_SetAHMIAlphaSeuqence();
	//AHMI_SetWindowCoordinate(0<<4,0<<4,800<<4,50<<4,600<<4,480<<4,20<<4,350<<4);
	AHMI_SETCoreCallCount(CORE0,0);
	AHMI_SETCoreCallCount(CORE1,0);

    GateClock_Enable();
}

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


