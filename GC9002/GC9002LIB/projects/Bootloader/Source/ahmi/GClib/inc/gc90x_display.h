
#ifndef __GC90x_DISPLAY_H
#define __GC90x_DISPLAY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "gc90xx.h"
     
/* define frame base address */
#define FRAME_BASE_ADDR									((uint32_t)0x200A0000)
//	 
////typedef enum {DISPLAY_DISABLE = 0, DISPLAY_ENABLE = !DISPLAY_DISABLE} FunctionalState;
////typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
//
///** 
//  * @brief  DISPLAY Init Structure definition  
//  */ 
//  
//typedef struct
//{
//	uint32_t OUTPUT_Mode;						/* display output mode */
//	uint32_t PRI_Col_Mode;					/* primary color mode */
//	uint32_t PRI_Map_Mode;					/* primary lvds data mapping mode */
//	uint32_t PRI_CLK_Invert;				/* primary clock invert */
//	uint32_t PRI_DATA_Invert;				/* lvds data invert(only for output mode 01,10,11) */
//	uint32_t PRI_HSYNC_Pol;					/* primary port hsync polarity */
//	uint32_t PRI_VSYNC_Pol;					/* primary port vsync polarity */
//	uint32_t PRI_BL;								/* primary burst length */
//	uint32_t PRI_DIS_PARM1;					/* primary display parameter1 */
//	uint32_t PRI_DIS_PARM2;					/* primary display parameter2 */
//	uint32_t PRI_DIS_PARM3;					/* primary display parameter3 */
//	uint32_t PRI_DIS_PARM4;					/* primary display parameter4 */
//	uint32_t PRI_ADDR;							/* frame base address */
//} DISPLAY_InitTypeDef;
//
///**
//  * @}
//  */ 
//
///** @defgroup DISPLAY_Exported_Constants
//  * @{
//  */ 
//
//#define SCREEN1							1
//#define SCREEN2							2
//
///** @defgroup DISPLAY_Output_Mode
//  * @{
//  */ 
//
//#define DISP_OUTPUT_PVGA										((uint32_t)0x00000000)		/* display output primart vga mode */
//#define	DISP_OUTPUT_PLVDS										((uint32_t)0x00000002)		/* display output primary lvds mode */
//#define	DISP_OUTPUT_PDLVDS									((uint32_t)0x00000004)		/* display output primary dual lvds mode */
//#define	DISP_OUTPUT_PSLVDS									((uint32_t)0x00000006)		/* display output primary + secondary lvds mode */
//#define IS_DUAL_MODE(MODE)									((MODE) == DISP_OUTPUT_PSLVDS)
//#define IS_DISPLAY_MODE(MODE)								(((MODE) == DISP_OUTPUT_PVGA) || \
//																						((MODE) == DISP_OUTPUT_PLVDS) || \
//																						((MODE) == DISP_OUTPUT_PDLVDS)|| \
//																						((MODE) == DISP_OUTPUT_PSLVDS))
///**
//  * @}
//  */ 
//
///** @defgroup display primary color mode select 
//	*	@{
//	*/
//	
//#define DISP_PRICOLOR24_MODE								((uint32_t)0xfffffeff)		/* display primary color mode 24 bit */
//#define	DISP_PRICOLOR16_MODE								((uint32_t)0x00000100)		/* display primary color mode 16 bit */
//#define IS_PRICOLOR_MODE(MODE)							(((MODE) == DISP_PRICOLOR24_MODE) || \
//																						((MODE) == DISP_PRICOLOR16_MODE))
//
///**
//  * @}
//  */ 
//	
///** @defgroup display primary lvds data mapping mode select 
//	*	@{
//	*/
//	
//#define	DISP_PRIJEIDA_MODE									((uint32_t)0xfffffdff)		/* JEIDA mode */
//#define	DISP_PRISPWG_MODE										((uint32_t)0x00000200)		/* SPWG mode  */
//#define IS_PRIMAP_MODE(MODE)								(((MODE) == DISP_PRIJEIDA_MODE) || \
//																						((MODE) == DISP_PRISPWG_MODE)
///**
//	*	@}
//	*/
//	
///** @defgroup display primary clock invert 
//	*	@{
//	*/
//
//#define PRI_CLK_INV													((uint32_t)0x00000400)		/* primary clock invert */							
//#define PRI_CLK_NINV												((uint32_t)0xfffffbff)		/* primary clock not invert */
//#define IS_PRI_CLKINV(CLKINV)								(((CLKINV) == PRI_CLK_INV) || \
//																						((CLKINV) == PRI_CLK_NINV)
//	
///**
//	*	@}
//	*/
//	
///** @defgroup display primary data invert 
//	*	@{
//	*/
//
//#define LVDS_PRIDATA_INV										((uint32_t)0x00000800)		/* lvds data invert */
//#define	LVDS_PRIDATA_NINV										((uint32_t)0xfffff7ff)		/* lvds data not invert */
//#define IS_PRI_DATAINV(DATAINV)							(((DATAINV) == LVDS_PRIDATA_INV) || \
//																						((DATAINV) == LVDS_PRIDATA_NINV))
//
///**
//	*	@}
//	*/
//	
///** @defgroup display primary port hsync polarity 
//	*	@{
//	*/	
//
//#define PRI_HSYNC_POLAL											((uint32_t)0xffffefff)		/* active lower mask */
//#define	PRI_HSYNC_POLAH											((uint32_t)0x00001000)		/* active high mask */
//#define IS_PRIHSYNC_POL(POL)								(((POL) == PRI_HSYNC_POLAL) || \
//																						((POL) == PRI_HSYNC_POLAH))
//	
///**
//	*	@}
//	*/
//	
///** @defgroup display primary port hsync polarity 
//	*	@{
//	*/
//	
//#define PRI_VSYNC_POLAL											((uint32_t)0xffffdfff)		/* active lower mask */
//#define	PRI_VSYNC_POLAH											((uint32_t)0x00002000)		/* active high mask */
//#define IS_PRIVSYNC_POL(POL)								(((POL) == PRI_VSYNC_POLAL) || \
//																						((POL) == PRI_VSYNC_POLAH))
//
///**
//	*	@}
//	*/
//	
///** @defgroup display primary burst length 
//	*	@{
//	*/
//#define PRI_BL130														((uint32_t)0x00004000)		/* primary bl = 130 */
//#define PRI_BL96														((uint32_t)0xffffbfff)		/* primary bl = 96 */
//#define IS_PRI_BL(BL)												(((BL) == PRI_BL130) || \
//																						((BL) == PRI_BL96))
//
///**
//	*	@}
//	*/
//
///** @defgroup display secondary color mode select 
//	*	@{
//	*/
//	
//#define DISP_SECCOLOR24_MODE								((uint32_t)0xfffeffff)		/* display secondary color mode 24 bit */
//#define	DISP_SECCOLOR16_MODE								((uint32_t)0x00010000)		/* display secondary color mode 16 bit */
//#define IS_SECCOLOR_MODE(MODE)							(((MODE) == DISP_SECCOLOR24_MODE) || \
//																						((MODE) == DISP_SECCOLOR16_MODE))
//
///**
//  * @}
//  */ 
//	
///** @defgroup display secondary lvds data mapping mode select 
//	*	@{
//	*/
//	
//#define	DISP_SECJEIDA_MODE									((uint32_t)0xfffdffff)		/* JEIDA mode */
//#define	DISP_SECSPWG_MODE										((uint32_t)0x00020000)		/* SPWG mode  */
//#define IS_SECMAP_MODE(MODE)								(((MODE) == DISP_SECJEIDA_MODE) || \
//																						((MODE) == DISP_SECSPWG_MODE)
///**
//	*	@}
//	*/
//	
///** @defgroup display secondary clock invert 
//	*	@{
//	*/
//
//#define SEC_CLK_INV													((uint32_t)0x00040000)		/* secondary clock invert */							
//#define SEC_CLK_NINV												((uint32_t)0xfffbffff)		/* secondary clock not invert */
//#define IS_SEC_CLKINV(CLKINV)								(((CLKINV) == SEC_CLK_INV) || \
//																						((CLKINV) == SEC_CLK_NINV)
//	
///**
//	*	@}
//	*/
//	
///** @defgroup display secondary data invert 
//	*	@{
//	*/
//
//#define LVDS_SECDATA_INV										((uint32_t)0x00080000)		/* lvds data invert */
//#define	LVDS_SECDATA_NINV										((uint32_t)0xfff7ffff)		/* lvds data not invert */
//#define IS_SEC_DATAINV(DATAINV)							(((DATAINV) == LVDS_SECDATA_INV) || \
//																						((DATAINV) == LVDS_SECDATA_NINV))
//
///**
//	*	@}
//	*/
//	
///** @defgroup display secondary port hsync polarity 
//	*	@{
//	*/	
//
//#define SEC_HSYNC_POLAL											((uint32_t)0xffefffff)		/* active lower mask */
//#define	SEC_HSYNC_POLAH											((uint32_t)0x00100000)		/* active high mask */
//#define IS_SECHSYNC_POL(POL)								(((POL) == SEC_HSYNC_POLAL) || \
//																						((POL) == SEC_HSYNC_POLAH))
//	
///**
//	*	@}
//	*/
//	
///** @defgroup display secondary port hsync polarity 
//	*	@{
//	*/
//	
//#define SEC_VSYNC_POLAL											((uint32_t)0xffdfffff)		/* active lower mask */
//#define	SEC_VSYNC_POLAH											((uint32_t)0x00200000)		/* active high mask */
//#define IS_SECVSYNC_POL(POL)								(((POL) == SEC_VSYNC_POLAL) || \
//																						((POL) == SEC_VSYNC_POLAH))
//
///**
//	*	@}
//	*/
//	
///** @defgroup display secondary burst length 
//	*	@{
//	*/
//	
//#define SEC_BL64														((uint32_t)0x00400000)		/* secondary bl = 64 */
//#define SEC_BL32														((uint32_t)0xffbfffff)		/* secondary bl = 32 */
//#define IS_SEC_BL(BL)												(((BL) == SEC_BL64) || \
//																						((BL) == SEC_BL32))
//
///**
//	*	@}
//	*/
//
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
	
///**
//	*	@}
//	*/
//
///**
//	*	@}
//	*/
//
//void DISPLAY_Init(DISPLAY_InitTypeDef* DISPLAY_InitStruct);
//void DISPLAY_StructInit(DISPLAY_InitTypeDef* DISPLAY_InitStruct);
//void DISPLAY_PLLCmd(FunctionalState NewState);
void DISPLAY_Cmd(FunctionalState NewState);
uint32_t DISPLAY_GetMode(void);
uint32_t DISPLAY_GetInterfaceMode(void);
//void DISPLAY_MODESel(uint32_t Mode);
//void DISPLAY_PriConfig(uint32_t Mode, uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl);
//void DISPLAY_SecConfig(uint32_t Mode, uint32_t ColMode, uint32_t MapMode, uint32_t ClkInv, uint32_t DataInv, uint32_t HsyncPol, uint32_t VsyncPol, uint32_t Bl);
void DISPLAY_ITConfig(uint32_t DISPLAY_IT, FunctionalState NewState);
ITStatus DISPLAY_GetITStatus(uint32_t DISPLAY_IT);
void DISPLAY_ClearITStatus(uint32_t DISPLAY_IT);
void DISPLAY_SetPriAddress(uint32_t FrameBaseAddress);
//void DISPLAY_SetPriParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
//												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch);
void DISPLAY_SetPriParamForRotate(void);
//void DISPLAY_SetSecAddress(uint32_t FrameBaseAddress);
//void DISPLAY_SetSecParam(uint16_t Hpixels, uint16_t HSyncPW, uint16_t HFnPrch, uint16_t HBkPrch,
//												 uint16_t VLines, uint16_t VSyncPW, uint16_t VFnPrch, uint16_t VBkPrch);
//void DISPLAY_RGBAjustParam(uint16_t Contrast, uint16_t Bright, uint16_t Saturability);
//void DISPLAY_SetContrast(uint32_t contrast);
//void DISPLAY_SetBright(uint32_t bright);
//void DISPLAY_SetSaturability(uint32_t saturability);
uint32_t SetDisplayContrast(uint32_t contrast);
uint32_t SetDisplayBrightness(uint32_t bright);
uint32_t SetDisplaySaturation(uint32_t saturation);
void DISPLAY_vStoreInputColorMode(uint32_t inputColorMode);
uint32_t DISPLAY_u32GetInputColorMode(void);


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
                    );
//
//#define GC9003
//
//#ifdef GC9003
//uint32_t DISPLAY_AdjustPare(uint32_t* pVCOMutiply,  uint32_t* pCLKINDivide, uint32_t* pPLLOutputDivide);
//ITStatus DISPLAY_SetClock(uint32_t VCOMutiply,  uint32_t CLKINDivide, uint32_t PLLOutputDivide);
//#endif
//
//
#ifdef __cplusplus
}
#endif

#endif /* __GC90x_DISPLAY_H */
