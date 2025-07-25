////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/10/17
// File Name:     DisplayDriver.h
// Project Name:  AHMIGEN_V2.0
// 
// Revision:
// Revision 2.00 - File Created 2018/10/17 by Zhou Yuzhi
// Additional Comments:
//    the display driver api for display module
////////////////////////////////////////////////////////////////////////////////

#include "DisplayDriver.h"
#include "DisplayModuleClass.h"
#include "drawImmediately_cd.h"

#ifdef VERSION_1
extern u8		stm32info[];
#endif

#ifdef VERSION_2
extern DisplayModuleClass mDisplayModule;
#endif

/**
	*	@brief setting the screen's width and height
	*	@param screenWidth : the width of the screen
	*   @param screenHeight : the height of the screen
	*	@retval None.
	*/
void DisplayDriver::Display_SetScreenWidthAndHeight(uint32_t screenWidth, uint32_t screenHeight)
{
	this->mDISPLAY_CONTROL_Type.PrimaryHPIXELS = screenWidth;
	this->mDISPLAY_CONTROL_Type.PrimaryHSYNCPW = 0;
	this->mDISPLAY_CONTROL_Type.PrimaryVLINES = screenHeight;
	this->mDISPLAY_CONTROL_Type.PrimaryVSYNCPW = 0;
}

/**
	*	@brief setting the framebuffer address
	*	@param framebufferaddress : the address of the framebuffer
	*	@retval None.
	*/
void DisplayDriver::Display_SetFBAddress(uint32_t framebufferaddress)
{
	this->mDISPLAY_CONTROL_Type.PrimaryAddress = framebufferaddress;
}

/**
  * @brief  DISPLAY select output mode. only for primary screen
  * @param  Mode: DISPLAY output mode select.
  *         This parameter can be: DISP_OUTPUT_PVGA, DISP_OUTPUT_PLVDS,
	*					DISP_OUTPUT_PDLVDS or DISP_OUTPUT_PSLVDS.
  * @retval None
  */
void DisplayDriver::DISPLAY_MODESel(uint32_t Mode)
{
	this->mDISPLAY_CONTROL_Type.OutputMode = Mode;
}

/**
	*	@brief	DISPLAY primary config 
	*	@param  ColMode: primary color mode.
	*					This parameter can be: DISP_PRICOLOR24_MODE, DISP_PRICOLOR16_MODE.
	*	@param	MapMode: primart lvds data mapping mode.
	*					This parameter can be: DISP_PRIJEIDA_MODE, DISP_PRISPWG_MODE.
	*	@param	ClkInv: primary clock invert or not.
	*					This parameter can be: PRI_CLK_INV, PRI_CLK_NINV.
	*	@param	DataInv: primary data invert or not.
	*					This parameter can be: LVDS_PRIDATA_INV, LVDS_PRIDATA_NINV.
	*	@param  HsyncPol: primary port hsync polarity.
	*					This parameter can be: PRI_HSYNC_POLAL, PRI_HSYNC_POLAH.
	*	@param  VsyncPol: primary port vsync polarity.
	*					This parameter can be: PRI_VSYNC_POLAL, PRI_VSYNC_POLAH.
	*	@param  Bl: primary burst length.
	* @retval None
	*/
void DisplayDriver::DISPLAY_PriConfig( uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl)
{
	this->mDISPLAY_CONTROL_Type.PrimaryColorMode = ColMode;
	this->mDISPLAY_CONTROL_Type.PrimaryLVDSMode  = MapMode;
	this->mDISPLAY_CONTROL_Type.PrimaryClockInvert = ClkInv;
	this->mDISPLAY_CONTROL_Type.PrimaryLVDSDataInvert = DataInv;
	this->mDISPLAY_CONTROL_Type.PrimaryHsyncPolarity = HsyncPol;
	this->mDISPLAY_CONTROL_Type.PrimaryVsyncPolarity = VsyncPol;
}

/**
	*	@brief	DISPLAY secondary config 
	*	@param 	Mode: DISPLAY output mode select.
  *         This parameter can be: DISP_OUTPUT_PSLVDS.
	*	@param  ColMode: secondary color mode.
	*					This parameter can be: DISP_SECCOLOR24_MODE, DISP_SECCOLOR16_MODE.
	*	@param	MapMode: secondary lvds data mapping mode.
	*					This parameter can be: DISP_SECJEIDA_MODE, DISP_SECSPWG_MODE.
	*	@param	ClkInv: secondary clock invert or not.
	*					This parameter can be: SEC_CLK_INV, SEC_CLK_NINV.
	*	@param	DataInv: secondary data invert or not.
	*					This parameter can be: LVDS_SECDATA_INV, LVDS_SECDATA_NINV.
	*	@param  HsyncPol: secondary port hsync polarity.
	*					This parameter can be: SEC_HSYNC_POLAL, SEC_HSYNC_POLAH.
	*	@param  VsyncPol: secondary port vsync polarity.
	*					This parameter can be: SEC_VSYNC_POLAL, SEC_VSYNC_POLAH.
	*	@param  Bl: secondary burst length.
	* @retval None
	*/
void DisplayDriver::DISPLAY_SecConfig(uint32_t Mode, uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl)
{
	this->mDISPLAY_CONTROL_Type.SecondaryColorMode = ColMode;
	this->mDISPLAY_CONTROL_Type.SecondaryLVDSMode  = MapMode;
	this->mDISPLAY_CONTROL_Type.SecondaryClockInvert = ClkInv;
	this->mDISPLAY_CONTROL_Type.SecondaryLVDSDataInvert = DataInv;
	this->mDISPLAY_CONTROL_Type.SecondaryHsyncPolarity = HsyncPol;
	this->mDISPLAY_CONTROL_Type.SecondaryVsyncPolarity = VsyncPol;
}

/**
	*	@brief set primary frame base address
	*	@param FrameBaseAddress: primary frame base address to set
	*	@retval	none
	*/
void DisplayDriver::DISPLAY_SetPriAddress(uint32_t FrameBaseAddress)
{
	this->mDISPLAY_CONTROL_Type.PrimaryAddress = FrameBaseAddress;
#ifdef VERSION_1
	u8 *stm32ptraddr;
	stm32ptraddr = (u8 *)Stm32_interface_Addr;
	//set the stminfo for drawing
	//tile box
	*( (u32 *)(stm32ptraddr + addr_disp_0) ) = this->mDISPLAY_CONTROL_Type.PrimaryAddress ;
#endif
}

/**
	*	@brief set primary display parameter
	*	@param Hpixels: horizental pixels.
	*	@param HSyncPW: horizental sync pulse width.
	*	@param HFnPrch: horizental front porch.
	*	@param HBkPrch: horizental back porch.
	*	@param VLines: vertical pixels.
	*	@param VSyncPW: vertical sync pulse width.
	*	@param VFnPrch: vertical front porch.
	*	@param VBkPrch: vertical front porch.
	*	@retval none
	*/
void DisplayDriver::DISPLAY_SetPriParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch)
{
	this->mDISPLAY_CONTROL_Type.PrimaryHPIXELS = Hpixels;
	this->mDISPLAY_CONTROL_Type.PrimaryHSYNCPW = HSyncPW;
	this->mDISPLAY_CONTROL_Type.PrimaryHFNPRCH = HFnPrch;
	this->mDISPLAY_CONTROL_Type.PrimaryHBKPRCH = HBkPrch;
	this->mDISPLAY_CONTROL_Type.PrimaryVLINES = VLines;
	this->mDISPLAY_CONTROL_Type.PrimaryVSYNCPW = VSyncPW;
	this->mDISPLAY_CONTROL_Type.PrimaryVFNPRCH = VFnPrch;
	this->mDISPLAY_CONTROL_Type.PrimaryVBKPRCH = VBkPrch;

#ifdef VERSION_1
	u8 *stm32ptraddr;
	stm32ptraddr = (u8 *)Stm32_interface_Addr;
	*((u16*)(stm32ptraddr+hpixels_0)) = Hpixels;
	*((u16*)(stm32ptraddr+vlines_0)) = VLines;
	*((u16*)(stm32ptraddr+hsyncpw_0)) = HSyncPW;
	*((u16*)(stm32ptraddr+vsyncpw_0)) = VSyncPW;
	*((u16*)(stm32ptraddr+hfnprch_0)) = HFnPrch;
	*((u16*)(stm32ptraddr+vfnprch_0)) = VFnPrch;
	*((u16*)(stm32ptraddr+hbkprch_0)) = HBkPrch;
	*((u16*)(stm32ptraddr+vbkprch_0)) = VBkPrch;
#endif
}

/**
	*	@brief set secondary frame base address
	*	@param FrameBaseAddress: secondary frame base address to set
	*	@retval	none
	*/
void DisplayDriver::DISPLAY_SetSecAddress(uint32_t FrameBaseAddress)
{
	this->mDISPLAY_CONTROL_Type.SecondaryAddress = FrameBaseAddress;
}

/**
	*	@brief set secondary display parameter
	*	@param Hpixels: horizental pixels.
	*	@param HSyncPW: horizental sync pulse width.
	*	@param HFnPrch: horizental front porch.
	*	@param HBkPrch: horizental back porch.
	*	@param VLines: vertical pixels.
	*	@param VSyncPW: vertical sync pulse width.
	*	@param VFnPrch: vertical front porch.
	*	@param VBkPrch: vertical front porch.
	*	@retval none
	*/
void DisplayDriver::DISPLAY_SetSecParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch)
{
	this->mDISPLAY_CONTROL_Type.SecondaryHPIXELS = Hpixels;
	this->mDISPLAY_CONTROL_Type.SecondaryHSYNCPW = HSyncPW;
	this->mDISPLAY_CONTROL_Type.SecondaryHFNPRCH = HFnPrch;
	this->mDISPLAY_CONTROL_Type.SecondaryHBKPRCH = HBkPrch;
	this->mDISPLAY_CONTROL_Type.SecondaryVLINES  = VLines;
	this->mDISPLAY_CONTROL_Type.SecondaryVSYNCPW = VSyncPW;
	this->mDISPLAY_CONTROL_Type.SecondaryVFNPRCH = VFnPrch;
	this->mDISPLAY_CONTROL_Type.SecondaryVBKPRCH = VBkPrch;
}

/**
	*	@brief set secondary frame base address
	*	@param FrameBaseAddress: secondary frame base address to set
	*	@retval	none
	*/
void DisplayDriver::DISPLAY_SetPrimaryClockParam(uint32_t ClkMul, uint32_t ClkDiv)
{
	this->mDISPLAY_CONTROL_Type.PrimaryClockVCOMutiplyParameter = ClkMul;
	this->mDISPLAY_CONTROL_Type.PrimaryClockCLKINDevideParameter = ClkDiv;
#ifdef VERSION_1
	u8 *stm32ptraddr;
	stm32ptraddr = (u8 *)Stm32_interface_Addr;
	*((u16*)(stm32ptraddr+clk_disp_mul_0)) = ClkMul;
	*((u16*)(stm32ptraddr+clk_disp_div_0)) = ClkDiv;
#endif
}

/**
	*	@brief set secondary frame base address
	*	@param FrameBaseAddress: secondary frame base address to set
	*	@retval	none
	*/
void DisplayDriver::DISPLAY_SetSecondaryClockParam(uint32_t ClkMul, uint32_t ClkDiv)
{
	this->mDISPLAY_CONTROL_Type.SecondaryClockVCOMutiplyParameter = ClkMul;
	this->mDISPLAY_CONTROL_Type.SecondaryClockCLKINDevideParameter = ClkDiv;
}

/**
	*	@brief set secondary frame base address
	*	@param FrameBaseAddress: secondary frame base address to set
	*	@retval	none
	*/
void DisplayDriver::DISPLAY_RGBAjustParam(uint16_t Contrast, uint16_t Bright, uint16_t Saturability)
{
	//TODO
	;
}


/**
	*	@brief start ahmi drawing
	*	@param none
	*	@retval None.
	*/
void DisplayDriver::Display_SetRegs(void)
{
#ifdef VERSION_1
	//TODO
#endif
#ifdef VERSION_2
	//TODO
	//mDisplayModule.mDisplayRegs.interruptEnable = this->mDISPLAY_CONTROL_Type.PimaryDrawDoneInterrupt | this->mDISPLAY_CONTROL_Type.PimaryFIFOFullInterrupt << 1;
	mDisplayModule.mDisplayRegs.primaryAddress = this->mDISPLAY_CONTROL_Type.PrimaryAddress;
	mDisplayModule.mDisplayRegs.primaryParam1 = (this->mDISPLAY_CONTROL_Type.PrimaryHPIXELS << 16) | this->mDISPLAY_CONTROL_Type.PrimaryHSYNCPW;
	mDisplayModule.mDisplayRegs.primaryParam2 = (this->mDISPLAY_CONTROL_Type.PrimaryHFNPRCH <<16) | this->mDISPLAY_CONTROL_Type.PrimaryHBKPRCH;
	mDisplayModule.mDisplayRegs.primaryParam3 = (this->mDISPLAY_CONTROL_Type.PrimaryVLINES << 16) | this->mDISPLAY_CONTROL_Type.PrimaryVSYNCPW;
	mDisplayModule.mDisplayRegs.primaryParam4 = (this->mDISPLAY_CONTROL_Type.PrimaryVFNPRCH << 16) | this->mDISPLAY_CONTROL_Type.PrimaryVBKPRCH;
#endif

}


/**
	*	@brief start display rendering current frame, only used in software
	*	@param pFrameBuffer : the address of the frame buffer
	*	@retval None.
	*/
void DisplayDriver::Display_StartRendering(FramBufferPtr pFrameBuffer)
{
#ifdef VERSION_1
	//TODO
#endif
#ifdef VERSION_2
	mDisplayModule.startDrawing(pFrameBuffer);
#endif
}



