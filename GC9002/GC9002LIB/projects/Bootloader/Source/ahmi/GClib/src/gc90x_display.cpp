//
///* Includes ------------------------------------------------------------------*/
//#include "display_reg.h"
#include "display.h"
#include "gc90x_display.h"
//#include "gc90x_AHMIClock.h"

//DISPLAY_HandleTypeDef hdisplay;

///** @defgroup DISPLAY_Private_Defines											
//  * @{
//  */
//
///* display output mode mask */
//#define	DISP_OUTPUT_MASK								((uint32_t)0xfffffff9)		/* display output mode mask */

///* display enable or disable */
//#define DISP_EN													((uint32_t)0x00000001)		/* display enable mask */
//#define	DISP_DIS												((uint32_t)0xfffffffe)		/* display disable mask */													

///* display frame drawing done interrupt enable or not */
//#define INT_DRAW_EN											((uint8_t)0x01)				/* enable display frame drawing done interrupt */
//#define INT_DRAW_DIS										((uint8_t)0xfe)				/* disable display frame drawing done interrupt */
//
///* display mode read empty fifo enable or not */
//#define INT_EMPTY_EN										((uint8_t)0x02)				/* enable display mode read empty fifo interrupt */
//#define INT_EMPTY_DIS										((uint8_t)0xfd)				/* disable display mode read empty fifo interrupt */
//
///* define primary display parameter */
//#define PRI_PARM1												((uint32_t)0x02d00001)
//#define	PRI_PARM2												((uint32_t)0x00640064)
//#define PRI_PARM3												((uint32_t)0x02400001)	
//#define	PRI_PARM4												((uint32_t)0x00010001)
//
///**
//  * @}
//  */
//	
///**
//  * @brief  Deinitializes the DISPLAY registers to their default reset values.
//  * @param        	
//  * @retval None
//  */
//void DISPLAY_DeInit(void)
//{
//	//APBCLK1_OFF;
//}
//
///**
//  * @brief  Initializes the DISPLAY.
//  * @param  DISPLAY_InitStruct: pointer to a DISPLAY_InitTypeDef structure
//  *         that contains the configuration information for the DISPLAY.
//  * @retval None
//  */
//
//void DISPLAY_Init(DISPLAY_InitTypeDef* DISPLAY_InitStruct)
//{
////	uint32_t tmpreg = 0;
////	
////	/* check the parameters */
//////	assert_param(IS_DISPLAY_MODE(DISPLAY_InitStruct->OUTPUT_Mode));
//////	assert_param(IS_PRICOLOR_MODE(DISPLAY_InitStruct->PRI_Col_Mode));
//////	assert_param(IS_PRIMAP_MODE(DISPLAY_InitStruct->PRI_Map_Mode));
//////	assert_param(IS_PRI_CLKINV(DISPLAY_InitStruct->PRI_CLK_Invert));
//////	assert_param(IS_PRI_DATAINV(DISPLAY_InitStruct->PRI_DATA_Invert));
//////	assert_param(IS_PRIHSYNC_POL(DISPLAY_InitStruct->PRI_HSYNC_Pol));
//////	assert_param(IS_PRIVSYNC_POL(DISPLAY_InitStruct->PRI_VSYNC_Pol));
//////	assert_param(IS_PRI_BL(DISPLAY_InitStruct->PRI_BL));
////	
////	/* enable primary and secondary display mode read empty fifo interrupt and display frame drawing done interrupt */
////	DISPLAY->IER = (uint32_t)(INT_DRAW_EN | (INT_DRAW_EN << 8)) | (uint32_t)(INT_EMPTY_EN | (INT_EMPTY_EN << 8));
////	
//////primary clock parameter
////	
////	/* set frame base address according to PRI_ADDR value */
////	DISPLAY->PAR = (uint32_t)DISPLAY_InitStruct->PRI_ADDR;
////	
////	/* set primary display parameter according PRI_DIS_PARMx */
////	DISPLAY->PDPR1 = DISPLAY_InitStruct->PRI_DIS_PARM1;
////	DISPLAY->PDPR2 = DISPLAY_InitStruct->PRI_DIS_PARM2;
////	DISPLAY->PDPR3 = DISPLAY_InitStruct->PRI_DIS_PARM3;
////	DISPLAY->PDPR4 = DISPLAY_InitStruct->PRI_DIS_PARM4;
////	
/////*---------------------- display CR configuration ----------------------*/
////	/* set output mode according to OUTPUT_Mode */
////	/* set primary color mode according to PRI_Col_Mode */
////	/* set primary data mapping mode according to PRI_Map_Mode */
////	/* set primary clock invert or not according to PRI_CLK_Invert */
////	/* set primary data invert or not according to PRI_DATA_Invert */
////	/* set primary port hsync polarity according to PRI_HSYNC_Pol */
////	/* set primary port vsync polarity according to PRI_VSYNC_Pol */
////	/* set primary burst length according to PRI_BL */
////	/* enable display */
////	tmpreg = DISPLAY->CR;
////	tmpreg &= DISP_OUTPUT_MASK;
////	tmpreg |= (uint32_t)DISPLAY_InitStruct->OUTPUT_Mode & DISPLAY_InitStruct->PRI_Col_Mode &
////								DISPLAY_InitStruct->PRI_Map_Mode & DISPLAY_InitStruct->PRI_CLK_Invert & 
////								DISPLAY_InitStruct->PRI_DATA_Invert | DISPLAY_InitStruct->PRI_HSYNC_Pol |
////								DISPLAY_InitStruct->PRI_VSYNC_Pol | DISPLAY_InitStruct->PRI_BL;
////	DISPLAY->CR = tmpreg;
//}
//
///**
//  * @brief  Fills each DISPLAY_InitStruct member with its default value.
//  * @param  DISPLAY_InitStruct: pointer to a DISPLAY_InitTypeDef structure
//  *         which will be initialized.
//  * @retval None
//  */
//void DISPLAY_StructInit(DISPLAY_InitTypeDef* DISPLAY_InitStruct)
//{
//  /* DISPLAY_InitStruct members default value */
//	DISPLAY_InitStruct->OUTPUT_Mode = DISP_OUTPUT_PLVDS;
//	DISPLAY_InitStruct->PRI_Col_Mode = DISP_PRICOLOR24_MODE;
//	DISPLAY_InitStruct->PRI_Map_Mode = DISP_PRIJEIDA_MODE;
//	DISPLAY_InitStruct->PRI_CLK_Invert = PRI_CLK_NINV;
//	DISPLAY_InitStruct->PRI_DATA_Invert = LVDS_PRIDATA_NINV;
//	DISPLAY_InitStruct->PRI_HSYNC_Pol = PRI_HSYNC_POLAH;
//	DISPLAY_InitStruct->PRI_VSYNC_Pol = PRI_VSYNC_POLAH;
//	DISPLAY_InitStruct->PRI_BL = PRI_BL130;
//	DISPLAY_InitStruct->PRI_ADDR = FRAME_BASE_ADDR;
//	DISPLAY_InitStruct->PRI_DIS_PARM1 = PRI_PARM1;
//	DISPLAY_InitStruct->PRI_DIS_PARM2 = PRI_PARM2;
//	DISPLAY_InitStruct->PRI_DIS_PARM3 = PRI_PARM3;
//	DISPLAY_InitStruct->PRI_DIS_PARM4 = PRI_PARM4;
//}
//
///**
//  * @brief  Enables or disables the DISPLAY PLL .
//  * @param  NewState: new state of the DISPLAY PLL.
//  *         This parameter can be: ENABLE or DISABLE.
//  * @retval None
//  */
//void DISPLAY_PLLCmd(FunctionalState NewState)
//{
////	/* Check the parameters */
//////	assert_param(IS_FUNCTIONAL_STATE(NewState));
////	
////	if (NewState != DISABLE)
////    {
////		/* Enable DISPLAY */
////		DISPLAY->PLLEN |= DISP_EN;
////    }
////    else
////    {
////		/* Disable DISPLAY */
////		DISPLAY->PLLEN &= DISP_DIS;
////    }
//}
//

/* Used to store Display input color mode */
static uint32_t g_u32DispInputColorMode;

/**
  * @brief  Store Display input color mode.
  * @param  inputColorMode: Display input color mode.
  *         
  * @retval None
  */
void DISPLAY_vStoreInputColorMode(uint32_t inputColorMode)
{
    g_u32DispInputColorMode = inputColorMode;
}


/**
  * @brief  Get Display input color mode.
  * @param  none
  *         
  * @retval: Display input color mode
  */

uint32_t DISPLAY_u32GetInputColorMode(void)
{
    return g_u32DispInputColorMode;
}





/**
  * @brief  Enables or disables the DISPLAY.
  * @param  NewState: new state of the DISPLAY.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DISPLAY_Cmd(FunctionalState NewState)
{
	/* Check the parameters */
//	assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if (NewState != DISABLE)
    {
		/* Enable DISPLAY */
		DISPLAY->display_cntl.reg_u32val |= 0x1;
    }
    else
    {
		/* Disable DISPLAY */
		DISPLAY->display_cntl.reg_u32val &= ~0x1;
    }
}

/**
  * @brief  Get DISPLAY mode had been config(master or slave).
  * @param  None
  * @retval Mode: DISPLAY mode config.
  *         This parameter can be: DISPLAY_Mode_MasterActive, DISPLAY_Mode_MasterPassive,
  *					DISPLAY_Mode_SlavePassive or DISPLAY_Mode_Configuration.
  */
uint32_t DISPLAY_GetMode(void)
{
	return ((DISPLAY->display_cntl.reg_u32val >> 16) & 0x3);
}

/**
  * @brief  Get DISPLAY interface mode had been config(DISPLAY_InterfaceType_24BIT or Other).
  * @param  None
  * @retval Mode: DISPLAY interface mode config.
  *         This parameter can be: DISPLAY_InterfaceType_SPI_LINE3, DISPLAY_InterfaceType_SPI_LINE4,
  *					DISPLAY_InterfaceType_QSPI, DISPLAY_InterfaceType_8BIT or DISPLAY_InterfaceType_24BIT.
  */
uint32_t DISPLAY_GetInterfaceMode(void)
{
	return ((DISPLAY->display_cntl.reg_u32val >> 13) & 0x7);
}

///**
//  * @brief  DISPLAY select output mode.
//  * @param  Mode: DISPLAY output mode select.
//  *         This parameter can be: DISP_OUTPUT_PVGA, DISP_OUTPUT_PLVDS,
//	*					DISP_OUTPUT_PDLVDS or DISP_OUTPUT_PSLVDS.
//  * @retval None
//  */
//void DISPLAY_MODESel(uint32_t Mode)
//{
////	uint32_t tmpreg = 0;
////	/* Check the parameters */
//////	assert_param(IS_DISPLAY_MODE(Mode));
////  
////	tmpreg = DISPLAY->CR;
////	tmpreg &= DISP_OUTPUT_MASK;
////	tmpreg |= Mode;
////	/* set mode */
////	DISPLAY->CR = tmpreg;
//}
//
///**
//	*	@brief	DISPLAY primary config 
//	*	@param 	Mode: DISPLAY output mode select.
//  *         This parameter can be: DISP_OUTPUT_PVGA, DISP_OUTPUT_PLVDS,
//	*					DISP_OUTPUT_PDLVDS or DISP_OUTPUT_PSLVDS.
//	*	@param  ColMode: primary color mode.
//	*					This parameter can be: DISP_PRICOLOR24_MODE, DISP_PRICOLOR16_MODE.
//	*	@param	MapMode: primart lvds data mapping mode.
//	*					This parameter can be: DISP_PRIJEIDA_MODE, DISP_PRISPWG_MODE.
//	*	@param	ClkInv: primary clock invert or not.
//	*					This parameter can be: PRI_CLK_INV, PRI_CLK_NINV.
//	*	@param	DataInv: primary data invert or not.
//	*					This parameter can be: LVDS_PRIDATA_INV, LVDS_PRIDATA_NINV.
//	*	@param  HsyncPol: primary port hsync polarity.
//	*					This parameter can be: PRI_HSYNC_POLAL, PRI_HSYNC_POLAH.
//	*	@param  VsyncPol: primary port vsync polarity.
//	*					This parameter can be: PRI_VSYNC_POLAL, PRI_VSYNC_POLAH.
//	*	@param  Bl: primary burst length.
//	* @retval None
//	*/
//void DISPLAY_PriConfig(uint32_t Mode, uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl)
//{
////	uint32_t tmpreg = 0;
////	/* Check the parameters */
//////	assert_param(IS_DISPLAY_MODE(Mode));
//////	assert_param(IS_PRICOLOR_MODE(ColMode));
//////	assert_param(IS_PRIMAP_MODE(MapMode));
//////	assert_param(IS_PRI_CLKINV(ClkInv));
//////	assert_param(IS_PRI_DATAINV(DataInv));
//////	assert_param(IS_PRIHSYNC_POL(HsyncPol));
//////	assert_param(IS_PRIVSYNC_POL(VsyncPol));
//////	assert_param(IS_PRI_BL(Bl));
////	
////	tmpreg = DISPLAY->CR;
////	
////	/* get color mode */
////	if(ColMode == DISP_PRICOLOR24_MODE)
////	{
////		tmpreg &= ColMode;
////	}
////	else
////	{
////		tmpreg |= ColMode;
////	}
////	
////	/* get data mapping mode */
////	if (MapMode == DISP_PRIJEIDA_MODE)
////	{
////		tmpreg &= MapMode;
////	}
////	else
////	{
////		tmpreg |= MapMode;
////	}
////	
////	/* get clock invert or not */
////	if (ClkInv == PRI_CLK_NINV)
////	{
////		tmpreg &= ClkInv;
////	}
////	else
////	{
////		tmpreg |= ClkInv;
////	}
////	
////	/* get data invert or not, only for output mode PLVDS, PDLVDS, PSLVDS */
////	if(Mode != DISP_OUTPUT_PVGA)
////	{
////		if (DataInv == LVDS_PRIDATA_NINV)
////		{
////			tmpreg &= DataInv;
////		}
////		else
////		{
////			tmpreg |= DataInv;
////		}
////	}
////	
////	/* get hsync polarity */
////	if (HsyncPol == PRI_HSYNC_POLAL)
////	{
////		tmpreg &= HsyncPol;
////	}
////	else
////	{
////		tmpreg |= HsyncPol;
////	}
////	
////	/* get vsync polarity */
////	if (VsyncPol == PRI_VSYNC_POLAL)
////	{
////		tmpreg &= VsyncPol;
////	}
////	else
////	{
////		tmpreg |= VsyncPol;
////	}
////	
////	/* get busrt length */
////	if (Bl == PRI_BL96)
////	{
////		tmpreg &= Bl;
////	}
////	else
////	{
////		tmpreg |= Bl;
////	}
////	
////	/* config control register */
////	DISPLAY->CR = tmpreg;
//}
//
///**
//	*	@brief	DISPLAY secondary config 
//	*	@param 	Mode: DISPLAY output mode select.
//  *         This parameter can be: DISP_OUTPUT_PSLVDS.
//	*	@param  ColMode: secondary color mode.
//	*					This parameter can be: DISP_SECCOLOR24_MODE, DISP_SECCOLOR16_MODE.
//	*	@param	MapMode: secondary lvds data mapping mode.
//	*					This parameter can be: DISP_SECJEIDA_MODE, DISP_SECSPWG_MODE.
//	*	@param	ClkInv: secondary clock invert or not.
//	*					This parameter can be: SEC_CLK_INV, SEC_CLK_NINV.
//	*	@param	DataInv: secondary data invert or not.
//	*					This parameter can be: LVDS_SECDATA_INV, LVDS_SECDATA_NINV.
//	*	@param  HsyncPol: secondary port hsync polarity.
//	*					This parameter can be: SEC_HSYNC_POLAL, SEC_HSYNC_POLAH.
//	*	@param  VsyncPol: secondary port vsync polarity.
//	*					This parameter can be: SEC_VSYNC_POLAL, SEC_VSYNC_POLAH.
//	*	@param  Bl: secondary burst length.
//	* @retval None
//	*/
////void DISPLAY_SecConfig(uint32_t Mode, uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl)
////{
////	uint32_t tmpreg = 0;
////	/* Check the parameters */
//////	assert_param(IS_DUAL_MODE(Mode));
//////	assert_param(IS_SECCOLOR_MODE(ColMode));
//////	assert_param(IS_SECMAP_MODE(MapMode));
//////	assert_param(IS_SEC_CLKINV(ClkInv));
//////	assert_param(IS_SEC_DATAINV(DataInv));
//////	assert_param(IS_SECHSYNC_POL(HsyncPol));
//////	assert_param(IS_SECVSYNC_POL(VsyncPol));
//////	assert_param(IS_SEC_BL(Bl));
////	tmpreg = DISPLAY->CR;
////	
////	/* get color mode */
////	if(ColMode == DISP_SECCOLOR24_MODE)
////	{
////		tmpreg &= ColMode;
////	}
////	else
////	{
////		tmpreg |= ColMode;
////	}
////	
////	/* get data mapping mode */
////	if (MapMode == DISP_SECJEIDA_MODE)
////	{
////		tmpreg &= MapMode;
////	}
////	else
////	{
////		tmpreg |= MapMode;
////	}
////	
////	/* get clock invert or not */
////	if (ClkInv == SEC_CLK_NINV)
////	{
////		tmpreg &= ClkInv;
////	}
////	else
////	{
////		tmpreg |= ClkInv;
////	}
////	
////	/* get data invert or not, only for output mode PLVDS, PDLVDS, PSLVDS */
////	if(Mode != DISP_OUTPUT_PVGA)
////	{
////		if (DataInv == LVDS_SECDATA_NINV)
////		{
////			tmpreg &= DataInv;
////		}
////		else
////		{
////			tmpreg |= DataInv;
////		}
////	}
////	
////	/* get hsync polarity */
////	if (HsyncPol == SEC_HSYNC_POLAL)
////	{
////		tmpreg &= HsyncPol;
////	}
////	else
////	{
////		tmpreg |= HsyncPol;
////	}
////	
////	/* get vsync polarity */
////	if (VsyncPol == SEC_VSYNC_POLAL)
////	{
////		tmpreg &= VsyncPol;
////	}
////	else
////	{
////		tmpreg |= VsyncPol;
////	}
////	
////	/* get busrt length */
////	if (Bl == SEC_BL32)
////	{
////		tmpreg &= Bl;
////	}
////	else
////	{
////		tmpreg |= Bl;
////	}
////	
////	/* config control register */
////	DISPLAY->CR = tmpreg;
////}
//
/**
  * @brief  Enables or disables the specified display interrupts.
  * @param  DISPLAY_IT: specifies the DISPLAY interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *		@arg DISPLAY_IT_PFDD primary display frame drawing done interrupt.
  *     @arg DISPLAY_IT_PREF primary display mode read empty fifo interrupt.
  *		@arg DISPLAY_IT_SFDD secondary display frame drawing done interrupt.
  *		@arg DISPLAY_IT_SREF secondary display mode read empty fifo interrupt.
  * @param  NewState: new state of the specified DISPLAY interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
	*/
void DISPLAY_ITConfig(uint32_t DISPLAY_IT, FunctionalState NewState)
{
	/* Check the parameters */
////	assert_param(IS_DISPLAY_IT(DISPLAY_IT));
////	assert_param(IS_FUNCTIONAL_STATE(NewState));
//	
	if (NewState != DISABLE)
	{
		/* enable the specified interrupt */
		DISPLAY->interrupt_mask |= DISPLAY_IT;
	}
	else
	{
		/* disable the specified interrupt */
		DISPLAY->interrupt_mask &= ~DISPLAY_IT;
	}
}

/**
	*	@brief get display specified interrupt status.
	*	@param DISPLAY_IT: specified interrupt.
	*				 This parameter can be: DISPLAY_IT_PFDD, DISPLAY_IT_PREF, DISPLAY_IT_SFDD, DISPLAY_IT_SREF.
	*	@retval return specified interrupt status.
	*/
ITStatus DISPLAY_GetITStatus(uint32_t DISPLAY_IT)
{
	ITStatus bitstatus = RESET;
	/* Check the parameters */
//	assert_param(IS_DISPLAY_IT(DISPLAY_IT));
	
	if ((DISPLAY->interrupt & DISPLAY_IT) == 0)
	{
		bitstatus = RESET;
	}
	else 
	{
		bitstatus = SET;
	}
	
	return bitstatus;
}

/**
	*	@brief get display specified interrupt status.
	*	@param DISPLAY_IT: specified interrupt.
	*				 This parameter can be: DISPLAY_IT_PFDD, DISPLAY_IT_PREF, DISPLAY_IT_SFDD, DISPLAY_IT_SREF.
	*	@retval return specified interrupt status.
	*/
void DISPLAY_ClearITStatus(uint32_t DISPLAY_IT)
{
//	ITStatus bitstatus = RESET;
	/* Check the parameters */
//	assert_param(IS_DISPLAY_IT(DISPLAY_IT));
	
	DISPLAY->interrupt &= ~DISPLAY_IT;
}

/**
	*	@brief set primary frame base address
	*	@param FrameBaseAddress: primary frame base address to set
	*	@retval	none
	*/
void DISPLAY_SetPriAddress(uint32_t FrameBaseAddress)
{
	if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	{
		DISPLAY->base_addr = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
	}
	else
	{
		DISPLAY->base_addr = FrameBaseAddress;
	}
}

///**
//	*	@brief set primary display parameter
//	*	@param Hpixels: horizental pixels.
//	*	@param HSyncPW: horizental sync pulse width.
//	*	@param HFnPrch: horizental front porch.
//	*	@param HBkPrch: horizental back porch.
//	*	@param VLines: vertical pixels.
//	*	@param VSyncPW: vertical sync pulse width.
//	*	@param VFnPrch: vertical front porch.
//	*	@param VBkPrch: vertical front porch.
//	*	@retval none
//	*/
//void DISPLAY_SetPriParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
//												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch)
//{
////	DISPLAY->PDPR1 = (uint32_t)(Hpixels << 16 | HSyncPW);
////	DISPLAY->PDPR2 = (uint32_t)((HFnPrch - 1) << 16 | (HBkPrch + 1));
////	DISPLAY->PDPR3 = (uint32_t)(VLines  << 16 | VSyncPW);
////	DISPLAY->PDPR4 = (uint32_t)((VFnPrch - 1) << 16 | (VBkPrch + 1));
//}
void DISPLAY_SetPriParamForRotate(void)
{
	uint32_t temp = 0;
	
	temp = DISPLAY->timing_param0 & 0xFFFF0000;
	DISPLAY->timing_param0 &= 0x0000FFFF;
	DISPLAY->timing_param0 |= (DISPLAY->timing_param2 & 0xFFFF0000);
	DISPLAY->timing_param2 &= 0x0000FFFF;
	DISPLAY->timing_param2 |= temp;
}
//
///**
//	*	@brief set secondary frame base address
//	*	@param FrameBaseAddress: secondary frame base address to set
//	*	@retval	none
//	*/
////void DISPLAY_SetSecAddress(uint32_t FrameBaseAddress)
////{
////	DISPLAY->SAR = FrameBaseAddress;
////}
//
///**
//	*	@brief set secondary display parameter
//	*	@param Hpixels: horizental pixels.
//	*	@param HSyncPW: horizental sync pulse width.
//	*	@param HFnPrch: horizental front porch.
//	*	@param HBkPrch: horizental back porch.
//	*	@param VLines: vertical pixels.
//	*	@param VSyncPW: vertical sync pulse width.
//	*	@param VFnPrch: vertical front porch.
//	*	@param VBkPrch: vertical front porch.
//	*	@retval none
//	*/
//void DISPLAY_SetSecParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
//												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch)
//{
////	DISPLAY->SDPR1 = (uint32_t)(Hpixels << 16 | HSyncPW);
////	DISPLAY->SDPR2 = (uint32_t)(HFnPrch << 16 | HBkPrch);
////	DISPLAY->SDPR3 = (uint32_t)(VLines  << 16 | VSyncPW);
////	DISPLAY->SDPR4 = (uint32_t)(VFnPrch << 16 | VBkPrch);
//}
//
///**
//	*	@brief adjust RGB parameter.
//	*	@param Contrast: contrast to adjust.
//	*	@param Bright: bright to adjust.
//	*	@param Saturability: saturability to adjust.
//	*/
//void DISPLAY_RGBAjustParam(uint16_t Contrast, uint16_t Bright, uint16_t Saturability)
//{
////	DISPLAY->RGBAPR1 = (uint32_t)(Contrast << 16 | Bright);
////	DISPLAY->RGBAPR2 = (uint32_t)Saturability;
//}
//
///**
//	*	@brief adjust RGB parameter.
//	*	@param Contrast: contrast to adjust.
//	*/
//void DISPLAY_SetContrast(uint32_t contrast)
//{
////    uint32_t temp = 0;
////    
////    temp = DISPLAY->RGBAPR1;
////    temp &= 0x0000FFFF;
////    temp |= (contrast << 16);
////    
////	DISPLAY->RGBAPR1 = temp;
//}
//
///**
//	*	@brief adjust RGB parameter.
//	*	@param Contrast: contrast to adjust.
//	*/
//void DISPLAY_SetBright(uint32_t bright)
//{
////    uint32_t temp = 0;
////    
////    temp = DISPLAY->RGBAPR1;
////    temp &= 0xFFFF0000;
////    temp |= bright;
////    
////	DISPLAY->RGBAPR1 = (uint32_t)temp;
//}
//
///**
//	*	@brief adjust RGB parameter.
//	*	@param Contrast: contrast to adjust.
//	*/
//void DISPLAY_SetSaturability(uint32_t saturability)
//{
////	DISPLAY->RGBAPR2 = (uint32_t)saturability;
//}

uint32_t SetDisplayContrast(uint32_t contrast)
{
//    uint32_t tempContrast = 0;
    uint32_t ret = 1;
//    
//    if(contrast > 200)
//    {
//        tempContrast = 0;
//        ret = 0;
//    }
//    else if((contrast >= 100) && (contrast <= 200))
//    {
//        tempContrast = (contrast - 100) * 255 / 100;
//    }
//    else
//    {
//        tempContrast = 0x100 + contrast * 255 / 100;
//    }
//    
//    DISPLAY_SetContrast(tempContrast);
//    Display::SetContrast(contrast);
    return ret;
}

uint32_t SetDisplayBrightness(uint32_t bright)
{
//    uint32_t tempBright = 0;
    uint32_t ret = 1;
//    
//    if(bright > 200)
//    {
//        tempBright = 0;
//        ret = 0;
//    }
//    else if((bright >= 100) && (bright <= 200))
//    {
//        tempBright = (bright - 100) * 255 / 100;
//    }
//    else
//    {
//        tempBright = 0x100 + bright * 255 / 100;
//    }
//    
//    DISPLAY_SetBright(tempBright);
//	Display::SetBright(bright);
    return ret;
}

uint32_t SetDisplaySaturation(uint32_t saturation)
{
//    uint32_t tempSaturation = 0;
    uint32_t ret = 1;
//    
//    if(saturation > 200)
//    {
//        tempSaturation = 0;
//        ret = 0;
//    }
//    else if((saturation >= 100) && (saturation <= 200))
//    {
//        tempSaturation = (saturation - 100) * 255 / 100;
//    }
//    else
//    {
//        tempSaturation = 0x100 + saturation * 255 / 100;
//    }
//    
//    DISPLAY_SetSaturability(tempSaturation);
//    Display::SetHueSaturation(saturation, 0);
    return ret;
}

void InitDisplayByBin(  uint32_t PolarityPara, 
						uint16_t Hpixels, 
                        uint16_t VLines, 
                        uint16_t HSyncPW,
                        uint16_t VSyncPW,
                        uint16_t HFnPrch, 
                        uint16_t VFnPrch,
                        uint16_t HBkPrch,
                        uint16_t VBkPrch,
                        uint32_t CLKINDevide,
                        uint32_t ScreenType,
                        uint32_t ScreenRam,                          
                        uint32_t BaseAddr,
                        uint32_t ParaExtra
                    )
{
	struct display_conf config;
    config.lcd_type = ScreenType;
	if(ScreenRam)
	{
		config.run_mode = DISPLAY_Mode_SlavePassive;
	}
	else 
	{
		config.run_mode = DISPLAY_Mode_MasterActive;
	}
	if(PolarityPara & 0x1)
	{
		config.invert_oclk_en    = 1;
	}else{
		config.invert_oclk_en    = 0;
	}
	if(PolarityPara & 0x2)
	{
		config.invert_hs         = 1;
	}else{
		config.invert_hs         = 0;
	}
	if(PolarityPara & 0x4)
	{
		config.invert_vs         = 1;
	}else{
		config.invert_vs         = 0;
	}
    config.clk_spi_div = DISPLAY_DIV_2;
	if(config.lcd_type == LCD_ST77903)
	{
		config.disp_qspi_duty_l = 2;
		config.disp_qspi_duty_h = 2;
	}
	else 
	{
		config.disp_qspi_duty_l = 3;
		config.disp_qspi_duty_h = 3;
	}
	
	//disp_duty_l:2~511; disp_duty_h:1~511; syspll/(disp_duty_l+disp_duty_h) < 100MHz
	if(CLKINDevide > 1022)
	{
		config.disp_duty_l = 511;
		config.disp_duty_h = 511;
	}
	else if(CLKINDevide < 5)
	{
		config.disp_duty_l = 2;
		config.disp_duty_h = 3;
	}
	else
	{
		config.disp_duty_l = CLKINDevide/2;
		config.disp_duty_h = CLKINDevide/2 + CLKINDevide%2;
	}
	
    config.input_color_mode = DISPLAY_InputColorMode_RGB565;
	if(config.lcd_type == LCD_ST77903)
	{
		config.output_color_mode = DISPLAY_OutputColorMode_RGB888;
	}
    /* 8080 LCD can support RGB666 format output */
    else if(config.lcd_type == LCD_MRB3205)
    {
        config.input_color_mode = DISPLAY_InputColorMode_RGBA8888;
        config.output_color_mode = DISPLAY_OutputColorMode_RGB666;
    }
	else 
	{
		config.output_color_mode = DISPLAY_OutputColorMode_RGB565;
	}

    /* Store Display color mode */
    DISPLAY_vStoreInputColorMode(config.input_color_mode);
    
    config.frame_buffer_addr = BaseAddr;
    config.HPIXEL_VALUE = Hpixels;
    config.VLINES_VALUE = VLines;
    config.HSYNCPW_VALUE = HSyncPW;
    config.VSYNCPW_VALUE = VSyncPW;
    config.HFNPRCH_VALUE = HFnPrch;
    config.VFNPRCH_VALUE = VFnPrch;
    config.HBKPRCH_VALUE = HBkPrch;
    config.VBKPRCH_VALUE = VBkPrch;
    config.PARA_EXTRA = ParaExtra;

    Display::Init(&config);
	DISPLAY_ITConfig(DISPLAY_IT_PFDD, ENABLE);
	
//    Display::SetBright(50);
//    Display::SetContrast(50);
//    Display::SetHueSaturation(50, 0);
//    Display::GammaCorrection(0x80008000);//γ = -0.5
	
//	display_cntl_struct s_disp_cntl;
//    display_timing_param_struct s_disp_timing_param;
//    display_update_area_struct s_disp_update_area;
//	set_master_struct s_set_master;
//	int i = 0;
//	
//	s_disp_cntl.enable            = 1;
//	if(PolarityPara & 0x1)
//	{
//		s_disp_cntl.invert_oclk       = 1;
//	}else{
//		s_disp_cntl.invert_oclk       = 0;
//	}
//	if(PolarityPara & 0x2)
//	{
//		s_disp_cntl.invert_hs         = 1;
//	}else{
//		s_disp_cntl.invert_hs         = 0;
//	}
//	if(PolarityPara & 0x4)
//	{
//		s_disp_cntl.invert_vs         = 1;
//	}else{
//		s_disp_cntl.invert_vs         = 0;
//	}
//    s_disp_cntl.update_ram        = 1;
//    s_disp_cntl.fg_enable         = 1;
//    s_disp_cntl.dithering         = 1;
//    s_disp_cntl.input_color_mode  = INPUT_COLOR_MODE_RGB565; //rgba8888
//    s_disp_cntl.output_color_mode = OUTPUT_COLOR_MODE_RGB565; //rgb565 
//    s_disp_cntl.interface_type    = INTERFACE_TYPE_24B;
//    s_disp_cntl.running_type      = RUNNING_TYPE_MA;
//    s_disp_cntl.tearing_en        = 0;
//    s_disp_cntl.update_en         = 1;
//    s_disp_cntl.clk_spi_div       = 0; //lowest clock
//    s_disp_cntl.invert_lcd_reset  = 0; 
//    s_disp_cntl.lcd_mv            = 0;
//
//    s_disp_timing_param.hpixels   = Hpixels;
//    s_disp_timing_param.hsyncpw   = HSyncPW;
//    s_disp_timing_param.hfnprch   = HFnPrch;
//    s_disp_timing_param.hbkprch   = HBkPrch;
//    s_disp_timing_param.vlines    = VLines;
//    s_disp_timing_param.vsyncpw   = VSyncPW;
//    s_disp_timing_param.vfnprch   = VFnPrch;
//    s_disp_timing_param.vbkprch   = VBkPrch;
//	
//	s_disp_update_area.start_row = 0;
//    s_disp_update_area.end_row   = Hpixels-1;
//    s_disp_update_area.start_col = 0;
//    s_disp_update_area.end_col   = VLines-1;
//	
//	s_set_master.base_addr      = BaseAddr; //hyper ram
//    s_set_master.total_byte_cnt = Hpixels*VLines*2;
//    s_set_master.burst_length   = 4; //one byte 8 pixels
// 
//	disp_set_master_regs(DISPLAY, &s_set_master);
//	
//	initial_regs(DISPLAY, 
//                &s_disp_cntl, 
//                &s_disp_timing_param,
//                &s_disp_update_area);
//	
//	//config display CLK
//	*(uint32_t*)(0x4020000c) = (0x80000000 | ((CLKINDevide/2) << 16) | (CLKINDevide/2));
//	DISPLAY_ITConfig(DISPLAY_IT_PFDD, ENABLE);
//	
//	//config display GPIOA(0~19)
//	for(i = 0; i < 20; i++)
//	{
//		*(uint32_t*)(0x4000a000 + 4*i) = 0x01010000;
//	}
}
//
//
//#ifdef GC9003
///**
//	*	@brief check display PLL clock parameters.
//	*	@param PLLOutputDivide
//	*	@param CLKINDevide
//	*	@param VCOMutiply
//	*	@brief Fdclock * VCOMutiply / CLKINDevide / (2^PLLOutputDivide)
//	*/
//uint32_t DISPLAY_AdjustPare(uint32_t* pVCOMutiply,  uint32_t* pCLKINDivide, uint32_t* pPLLOutputDivide)
//{
////    uint32_t CLK_OUT = 0;
////    
////    CLK_OUT = AHMI_ClockIn * (*pVCOMutiply) / (*pCLKINDivide);
////    
////    /* Parameters meet the conditions */
////    if((CLK_OUT >= 200UL) && (CLK_OUT <= 600UL) && ((*pVCOMutiply) >= 2) && ((*pCLKINDivide) >= 2))
////    {
////        return SET;
////    }
////    
////    /* adjust VCOMutiply */
////    if((*pVCOMutiply) < 2)
////    {
////        (*pVCOMutiply) = 2;
////        (*pCLKINDivide) = 2 * (*pCLKINDivide);
////        DISPLAY_AdjustPare(pVCOMutiply, pCLKINDivide, pPLLOutputDivide);
////        return SET;
////    }
////    
////    /* adjust CLKINDivide */
////    if((*pCLKINDivide) < 2)
////    {
////        (*pCLKINDivide) = 2;
////        (*pVCOMutiply) = 2 * (*pVCOMutiply);
////        DISPLAY_AdjustPare(pVCOMutiply, pCLKINDivide, pPLLOutputDivide);
////        return SET;
////    }
////    
////    /* adjust VCOMutiply,CLKINDivide,PLLOutputDivide */
////	if(CLK_OUT < 200UL)
////	{
////        if(((*pVCOMutiply) < 128) && ((*pPLLOutputDivide) < 6))
////        {
////            (*pVCOMutiply) = 2 * (*pVCOMutiply);
////            (*pPLLOutputDivide) = (*pPLLOutputDivide) + 1;
////        }
////        else if(((*pCLKINDivide) > 4) && ((*pPLLOutputDivide) < 6))
////        {
////            (*pCLKINDivide) = (*pCLKINDivide) / 2;
////            (*pPLLOutputDivide) = (*pPLLOutputDivide) + 1;
////        }
////        else
////        {
////            (*pVCOMutiply) = 0x15;
////            (*pCLKINDivide) = 0x3;
////            (*pPLLOutputDivide) = 0x3;
////            DISPLAY_AdjustPare(pVCOMutiply, pCLKINDivide, pPLLOutputDivide);
////            return SET;
////        }
////        DISPLAY_AdjustPare(pVCOMutiply, pCLKINDivide, pPLLOutputDivide);
////		return SET;
////	}
////    
////    /* adjust VCOMutiply,CLKINDivide,PLLOutputDivide */
////    if(CLK_OUT > 600UL)
////    {
////        if(((*pVCOMutiply) > 4) && ((*pPLLOutputDivide) > 0))
////        {
////            (*pVCOMutiply) = (*pVCOMutiply) / 2;
////            (*pPLLOutputDivide) = (*pPLLOutputDivide) - 1;
////        }
////        else if(((*pCLKINDivide) < 8) && ((*pPLLOutputDivide) > 0))
////        {
////            (*pCLKINDivide) = (*pCLKINDivide) * 2;
////            (*pPLLOutputDivide) = (*pPLLOutputDivide) - 1;
////        }
////        else
////        {
////            (*pVCOMutiply) = 0x15;
////            (*pCLKINDivide) = 0x3;
////            (*pPLLOutputDivide) = 0x3;
////            DISPLAY_AdjustPare(pVCOMutiply, pCLKINDivide, pPLLOutputDivide);
////            return SET;
////        }
////        DISPLAY_AdjustPare(pVCOMutiply, pCLKINDivide, pPLLOutputDivide);
////		return SET;
////    }
//		return RESET;
//}
//
///**
//	*	@brief set display clock.
//	*	@param PLLOutputDivide
//	*	@param CLKINDevide
//	*	@param VCOMutiply
//	*	@brief Fdclock * VCOMutiply / CLKINDivide / (2^PLLOutputDivide)
//	*/
//ITStatus DISPLAY_SetClock(uint32_t VCOMutiply,  uint32_t CLKINDivide, uint32_t PLLOutputDivide)
//{
////	uint32_t temp = 0;
////	
////    DISPLAY_AdjustPare(&VCOMutiply, &CLKINDivide, &PLLOutputDivide);
////    
////	temp = (DISPLAY->PLLPARAM & 0xFF00F08F);
////	temp |= ((VCOMutiply << 16) | (CLKINDivide << 8) | (PLLOutputDivide << 4));
////	DISPLAY->PLLPARAM = temp;
//	return RESET;
//}
//#endif
//
///**
//  * @}
//  */
//
///**
//  * @}
//  */
//
///**
//  * @}
//  */
//
