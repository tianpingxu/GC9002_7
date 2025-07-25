////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/10/17
// File Name:     AHMIDriver.h
// Project Name:  AHMIGEN_V2.0
// 
// Revision:
// Revision 2.00 - File Created 2018/10/17 by Zhou Yuzhi
// Additional Comments:
//    the driver api for display module
////////////////////////////////////////////////////////////////////////////////

#ifndef DISPLAY_DRIVER__H
#define DISPLAY_DRIVER__H

#include "publicType.h"
#include "ahmiv3.h"
#include "simulatorParameters.h"

//control param
//#define ENABLE                        0x1
//#define DISABLE                       0x0
#define PRIMARY_LVCMOS                0x0
#define PRIMARY_LVDS                  0x1
#define PRIMARY_DUAL_LVDS             0x2
#define PRIMARY_SECONDARY_LVDS        0x3
#define PRIMARY_COLOR_16BIT           0x1
#define PRIMARY_COLOR_24BIT           0X0
#define JEIDA                         0x0
#define SPWG                          0x1

/**
  * @}
  */ 

/** @defgroup DISPLAY_Exported_Constants
  * @{
  */ 

#define SCREEN1							1
#define SCREEN2							2

/** @defgroup DISPLAY_Output_Mode
  * @{
  */ 

#define DISP_OUTPUT_PVGA										((uint32_t)0x00000000)		/* display output primart vga mode */
#define	DISP_OUTPUT_PLVDS										((uint32_t)0x00000002)		/* display output primary lvds mode */
#define	DISP_OUTPUT_PDLVDS									((uint32_t)0x00000004)		/* display output primary dual lvds mode */
#define	DISP_OUTPUT_PSLVDS									((uint32_t)0x00000006)		/* display output primary + secondary lvds mode */
#define IS_DUAL_MODE(MODE)									((MODE) == DISP_OUTPUT_PSLVDS)
#define IS_DISPLAY_MODE(MODE)								(((MODE) == DISP_OUTPUT_PVGA) || \
																						((MODE) == DISP_OUTPUT_PLVDS) || \
																						((MODE) == DISP_OUTPUT_PDLVDS)|| \
																						((MODE) == DISP_OUTPUT_PSLVDS))
/**
  * @}
  */ 

/** @defgroup display primary color mode select 
	*	@{
	*/
	
#define DISP_PRICOLOR24_MODE								((uint32_t)0xfffffeff)		/* display primary color mode 24 bit */
#define	DISP_PRICOLOR16_MODE								((uint32_t)0x00000100)		/* display primary color mode 16 bit */
#define IS_PRICOLOR_MODE(MODE)							(((MODE) == DISP_PRICOLOR24_MODE) || \
																						((MODE) == DISP_PRICOLOR16_MODE))

/**
  * @}
  */ 
	
/** @defgroup display primary lvds data mapping mode select 
	*	@{
	*/
	
#define	DISP_PRIJEIDA_MODE									((uint32_t)0xfffffdff)		/* JEIDA mode */
#define	DISP_PRISPWG_MODE										((uint32_t)0x00000200)		/* SPWG mode  */
#define IS_PRIMAP_MODE(MODE)								(((MODE) == DISP_PRIJEIDA_MODE) || \
																						((MODE) == DISP_PRISPWG_MODE)
/**
	*	@}
	*/
	
/** @defgroup display primary clock invert 
	*	@{
	*/

#define PRI_CLK_INV													((uint32_t)0x00000400)		/* primary clock invert */							
#define PRI_CLK_NINV												((uint32_t)0xfffffbff)		/* primary clock not invert */
#define IS_PRI_CLKINV(CLKINV)								(((CLKINV) == PRI_CLK_INV) || \
																						((CLKINV) == PRI_CLK_NINV)
	
/**
	*	@}
	*/
	
/** @defgroup display primary data invert 
	*	@{
	*/

#define LVDS_PRIDATA_INV										((uint32_t)0x00000800)		/* lvds data invert */
#define	LVDS_PRIDATA_NINV										((uint32_t)0xfffff7ff)		/* lvds data not invert */
#define IS_PRI_DATAINV(DATAINV)							(((DATAINV) == LVDS_PRIDATA_INV) || \
																						((DATAINV) == LVDS_PRIDATA_NINV))

/**
	*	@}
	*/
	
/** @defgroup display primary port hsync polarity 
	*	@{
	*/	

#define PRI_HSYNC_POLAL											((uint32_t)0xffffefff)		/* active lower mask */
#define	PRI_HSYNC_POLAH											((uint32_t)0x00001000)		/* active high mask */
#define IS_PRIHSYNC_POL(POL)								(((POL) == PRI_HSYNC_POLAL) || \
																						((POL) == PRI_HSYNC_POLAH))
	
/**
	*	@}
	*/
	
/** @defgroup display primary port hsync polarity 
	*	@{
	*/
	
#define PRI_VSYNC_POLAL											((uint32_t)0xffffdfff)		/* active lower mask */
#define	PRI_VSYNC_POLAH											((uint32_t)0x00002000)		/* active high mask */
#define IS_PRIVSYNC_POL(POL)								(((POL) == PRI_VSYNC_POLAL) || \
																						((POL) == PRI_VSYNC_POLAH))

/**
	*	@}
	*/
	
/** @defgroup display primary burst length 
	*	@{
	*/
#define PRI_BL130														((uint32_t)0x00004000)		/* primary bl = 130 */
#define PRI_BL96														((uint32_t)0xffffbfff)		/* primary bl = 96 */
#define IS_PRI_BL(BL)												(((BL) == PRI_BL130) || \
																						((BL) == PRI_BL96))

/**
	*	@}
	*/

/** @defgroup display secondary color mode select 
	*	@{
	*/
	
#define DISP_SECCOLOR24_MODE								((uint32_t)0xfffeffff)		/* display secondary color mode 24 bit */
#define	DISP_SECCOLOR16_MODE								((uint32_t)0x00010000)		/* display secondary color mode 16 bit */
#define IS_SECCOLOR_MODE(MODE)							(((MODE) == DISP_SECCOLOR24_MODE) || \
																						((MODE) == DISP_SECCOLOR16_MODE))

/**
  * @}
  */ 
	
/** @defgroup display secondary lvds data mapping mode select 
	*	@{
	*/
	
#define	DISP_SECJEIDA_MODE									((uint32_t)0xfffdffff)		/* JEIDA mode */
#define	DISP_SECSPWG_MODE										((uint32_t)0x00020000)		/* SPWG mode  */
#define IS_SECMAP_MODE(MODE)								(((MODE) == DISP_SECJEIDA_MODE) || \
																						((MODE) == DISP_SECSPWG_MODE)
/**
	*	@}
	*/
	
/** @defgroup display secondary clock invert 
	*	@{
	*/

#define SEC_CLK_INV													((uint32_t)0x00040000)		/* secondary clock invert */							
#define SEC_CLK_NINV												((uint32_t)0xfffbffff)		/* secondary clock not invert */
#define IS_SEC_CLKINV(CLKINV)								(((CLKINV) == SEC_CLK_INV) || \
																						((CLKINV) == SEC_CLK_NINV)
	
/**
	*	@}
	*/
	
/** @defgroup display secondary data invert 
	*	@{
	*/

#define LVDS_SECDATA_INV										((uint32_t)0x00080000)		/* lvds data invert */
#define	LVDS_SECDATA_NINV										((uint32_t)0xfff7ffff)		/* lvds data not invert */
#define IS_SEC_DATAINV(DATAINV)							(((DATAINV) == LVDS_SECDATA_INV) || \
																						((DATAINV) == LVDS_SECDATA_NINV))

/**
	*	@}
	*/
	
/** @defgroup display secondary port hsync polarity 
	*	@{
	*/	

#define SEC_HSYNC_POLAL											((uint32_t)0xffefffff)		/* active lower mask */
#define	SEC_HSYNC_POLAH											((uint32_t)0x00100000)		/* active high mask */
#define IS_SECHSYNC_POL(POL)								(((POL) == SEC_HSYNC_POLAL) || \
																						((POL) == SEC_HSYNC_POLAH))
	
/**
	*	@}
	*/
	
/** @defgroup display secondary port hsync polarity 
	*	@{
	*/
	
#define SEC_VSYNC_POLAL											((uint32_t)0xffdfffff)		/* active lower mask */
#define	SEC_VSYNC_POLAH											((uint32_t)0x00200000)		/* active high mask */
#define IS_SECVSYNC_POL(POL)								(((POL) == SEC_VSYNC_POLAL) || \
																						((POL) == SEC_VSYNC_POLAH))

/**
	*	@}
	*/
	
/** @defgroup display secondary burst length 
	*	@{
	*/
	
#define SEC_BL64														((uint32_t)0x00400000)		/* secondary bl = 64 */
#define SEC_BL32														((uint32_t)0xffbfffff)		/* secondary bl = 32 */
#define IS_SEC_BL(BL)												(((BL) == SEC_BL64) || \
																						((BL) == SEC_BL32))

/**
	*	@}
	*/

/** @defgroup USART_Interrupt_definition 
  * @{
  */
	
#define DISPLAY_IT_PFDD											((uint32_t)0x00000001)
#define DISPLAY_IT_PREF											((uint32_t)0x00000002)
#define DISPLAY_IT_SFDD											((uint32_t)0x00000100)
#define DISPLAY_IT_SREF											((uint32_t)0x00000200)	
#define IS_DISPLAY_IT(IT)										(((IT) == DISPLAY_IT_PFDD) || \
																				    ((IT) == DISPLAY_IT_PREF) || \
																						((IT) == DISPLAY_IT_SFDD) || \
																						((IT) == DISPLAY_IT_SREF))

#define HIGH_PERFERMANCE              0

typedef struct _DISPLAY_CONTROL_Type
{
	uint32_t Enable; //enable the module
	uint32_t OutputMode; //primary screen output mode. LVDS/RGB
	uint32_t PrimaryColorMode;      //16bit / 24bit color
	uint32_t PrimaryLVDSMode;       //DISP_PRIJEIDA_MODE, DISP_PRISPWG_MODE.
	uint32_t PrimaryClockInvert;    //whether invert the clock signal
	uint32_t PrimaryLVDSDataInvert; //whether invert data
	uint32_t PrimaryHsyncPolarity;  //hs is high/low valid
	uint32_t PrimaryVsyncPolarity;  //vs is high/low valid
	uint32_t SecondaryColorMode;       //16bit / 24bit color
	uint32_t SecondaryLVDSMode;		   //DISP_PRIJEIDA_MODE, DISP_PRISPWG_MODE.
	uint32_t SecondaryClockInvert;	   //whether invert the clock signal
	uint32_t SecondaryLVDSDataInvert;  //whether invert data;
	uint32_t SecondaryHsyncPolarity;   //hs is high/low valid
	uint32_t SecondaryVsyncPolarity;   //vs is high/low valid
	uint32_t PimaryDrawDoneInterrupt;  //primal done
	uint32_t PimaryFIFOFullInterrupt;  //primal fifo full error
	uint32_t SecondaryDrawDoneInterrupt; //second done
	uint32_t SecondaryFIFOFullInterrupt; //second fifo full error
	uint32_t PrimaryClockCLKINDevideParameter;  //div for pll
	uint32_t PrimaryClockVCOMutiplyParameter;   //mul for pll
	uint32_t PrimaryAddress;                    //primal address
	uint32_t PrimaryHPIXELS;                    //primal display parameters
	uint32_t PrimaryHSYNCPW;                   	//primal display parameters
	uint32_t PrimaryHFNPRCH;					//primal display parameters
	uint32_t PrimaryHBKPRCH;					//primal display parameters
	uint32_t PrimaryVLINES;						//primal display parameters
	uint32_t PrimaryVSYNCPW;					//primal display parameters
	uint32_t PrimaryVFNPRCH;					//primal display parameters
	uint32_t PrimaryVBKPRCH;					//primal display parameters
	uint32_t SecondaryClockCLKINDevideParameter; //div for pll
	uint32_t SecondaryClockVCOMutiplyParameter;	 //mul for pll
	uint32_t SecondaryAddress;					 //secondary address
	uint32_t SecondaryHPIXELS;					 //secondary display parameters
	uint32_t SecondaryHSYNCPW;					 //secondary display parameters
	uint32_t SecondaryHFNPRCH;					 //secondary display parameters
	uint32_t SecondaryHBKPRCH;					 //secondary display parameters
	uint32_t SecondaryVLINES;					 //secondary display parameters
	uint32_t SecondaryVSYNCPW;					 //secondary display parameters
	uint32_t SecondaryVFNPRCH;					 //secondary display parameters
	uint32_t SecondaryVBKPRCH;					 //secondary display parameters
}DISPLAY_CONTROL_Type;

class DisplayDriver
{
private:
	DISPLAY_CONTROL_Type mDISPLAY_CONTROL_Type;

public:
	/**
	*	@brief setting the screen's width and height
	*	@param screenWidth : the width of the screen
	*   @param screenHeight : the height of the screen
	*	@retval None.
	*/
	void Display_SetScreenWidthAndHeight(uint32_t screenWidth, uint32_t screenHeight);

	/**
	*	@brief setting the framebuffer address
	*	@param framebufferaddress : the address of the framebuffer
	*	@retval None.
	*/
	void Display_SetFBAddress(uint32_t framebufferaddress);

	/**
  * @brief  DISPLAY select output mode.
  * @param  Mode: DISPLAY output mode select.
  *         This parameter can be: DISP_OUTPUT_PVGA, DISP_OUTPUT_PLVDS,
	*					DISP_OUTPUT_PDLVDS or DISP_OUTPUT_PSLVDS.
  * @retval None
  */
	void DISPLAY_MODESel(uint32_t Mode);

/**
	*	@brief	DISPLAY primary config 
	*	@param 	Mode: DISPLAY output mode select.
  *         This parameter can be: DISP_OUTPUT_PVGA, DISP_OUTPUT_PLVDS,
	*					DISP_OUTPUT_PDLVDS or DISP_OUTPUT_PSLVDS.
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
	void DISPLAY_PriConfig(uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl);

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
	void DISPLAY_SecConfig(uint32_t Mode, uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl);

/**
	*	@brief set primary frame base address
	*	@param FrameBaseAddress: primary frame base address to set
	*	@retval	none
	*/
	void DISPLAY_SetPriAddress(uint32_t FrameBaseAddress);

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
	void DISPLAY_SetPriParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch);

/**
	*	@brief set secondary frame base address
	*	@param FrameBaseAddress: secondary frame base address to set
	*	@retval	none
	*/
	void DISPLAY_SetSecAddress(uint32_t FrameBaseAddress);

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
	void DISPLAY_SetSecParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch);

/**
	*	@brief set secondary frame base address
	*	@param FrameBaseAddress: secondary frame base address to set
	*	@retval	none
	*/
	void DISPLAY_SetPrimaryClockParam(uint32_t ClkMul, uint32_t ClkDiv);

/**
	*	@brief set secondary frame base address
	*	@param FrameBaseAddress: secondary frame base address to set
	*	@retval	none
	*/
	void DISPLAY_SetSecondaryClockParam(uint32_t ClkMul, uint32_t ClkDiv);

/**
	*	@brief adjust RGB parameter.
	*	@param Contrast: contrast to adjust.
	*	@param Bright: bright to adjust.
	*	@param Saturability: saturability to adjust.
	*/
	void DISPLAY_RGBAjustParam(uint16_t Contrast, uint16_t Bright, uint16_t Saturability);

	/**
	*	@brief start ahmi drawing
	*	@param none
	*	@retval None.
	*/
	void Display_SetRegs(void);

	/**
	*	@brief start display rendering current frame, only used in software
	*	@param pFrameBuffer : the address of the frame buffer
	*	@retval None.
	*/
	void Display_StartRendering(FramBufferPtr pFrameBuffer);

};

#endif
