////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/11/20
// File Name:     VideoCaptureDriver.h
// Project Name:  ahmiSimulator_v0304
// 
// Revision:
// Revision 2.00 - File Created 2018/11/20 by Yu Ning
// Additional Comments:
//    the VideoCapture driver api for VideoCapture module
////////////////////////////////////////////////////////////////////////////////

#ifndef VIDEOCAPTURE_DRIVER__H
#define VIDEOCAPTURE_DRIVER__H

#include "publicType.h"

/**
  * @}
  */ 

/** @defgroup VCAP_Exported_Constants
  * @{
  */ 

/**	@defgroup	video capture filed sample mode
	*	@{
	*/

#define VCAP_SAMPLE_ODD							   	((uint32_t)0x00000000)	/* sample odd field */
#define VCAP_SAMPLE_EVEN							((uint32_t)0x00000100)	/* sample even field */
#define VCAP_SAMPLE_BOTH							((uint32_t)0x00000200)	/* sample both field */
#define IS_VCAP_SAMPLE_MODE(MODE)			 	(((MODE) == VCAP_SAMPLE_ODD) || \
																				((MODE) == VCAP_SAMPLE_EVEN)	|| \
																				((MODE) == VCAP_SAMPLE_BOTH))

/**
	*	@}
	*/

/**	@defgroup	video capture input channel number
	*	@{
	*/

#define	VCAP_RXCH_1											((uint32_t)0x00000000)	/* 1 channel */
#define	VCAP_RXCH_2											((uint32_t)0x00000008)	/* 2 channel */
#define	VCAP_RXCH_4											((uint32_t)0x00000010)	/* 4 channel */
#define	IS_VCAP_RXCH_NUM(NUM)						(((NUM) == VCAP_RXCH_1) || \
																				((NUM) == VCAP_RXCH_2)	|| \
																				((NUM) == VCAP_RXCH_4))

/**
	*	@}
	*/

/**	@defgroup	video capture color bit
	*	@{
	*/

#define VCAP_COLOR_16										((uint32_t)0x00000000)	/* 16 bit */
#define VCAP_COLOR_32										((uint32_t)0x00000002)	/* 32 bit */
#define	IS_VCAP_COLOR_BIT(BIT)					(((BIT) == VCAP_COLOR_16) || \
																				((BIT) == VCAP_COLOR_32))

/**
	*	@}
	*/

/**	@defgroup video capture interrupt definition
	* @{
	*/
	
#define VCAP_IT_CH0											((uint32_t)0x00000000)	/* channel 0 drawing done interrupt */
#define VCAP_IT_CH1											((uint32_t)0x00000002)	/* channel 1 drawing done interrupt */
#define VCAP_IT_CH2											((uint32_t)0x00000004)	/* channel 2 drawing done interrupt */
#define VCAP_IT_CH3											((uint32_t)0x00000008)	/* channel 3 drawing done interrupt */
#define VCAP_IT_WRFULL									((uint32_t)0x00000100)	/* write full data fifo interrupt */
#define VCAP_IT_OVERWR									((uint32_t)0x00000200)	/* data buffer overwrite enable */
#define IS_VCAP_IT(IT)									(((IT) == VCAP_IT_CH0) || \
																				((IT) == VCAP_IT_CH1) || \
																				((IT) == VCAP_IT_CH2) || \
																				((IT) == VCAP_IT_CH3) || \
																				((IT) == VCAP_IT_WRFULL) || \
																				((IT) == VCAP_IT_OVERWR)) 																							 

/**
	*	@}
	*/
	
/**	@defgroup	video capture input channel definition
	*	@{
	*/
	
#define	CH0															((uint8_t)0x00)	/* channel0 */
#define CH1															((uint8_t)0x01)	/* channel1 */
#define CH2															((uint8_t)0x02)	/* channel2 */
#define CH3															((uint8_t)0x03)	/* channel3 */
#define	IS_VCAP_CH(CHx)									(((CHx) == CH0) || \
																				((CHx) == CH1) || \
																				((CHx) == CH2) || \
																				((CHx) == CH3)) 
																				 
/**
	*	@}
	*/


/** @addtogroup GC90x_StdPeriph_Driver
  * @{
  */

/** @defgroup VCAP 
  * @brief VCAP driver modules
  * @{
  */

/** @defgroup VCAP_Private_Defines											
  * @{
  */

/* video capture enable mask */
#define VCAP_Enable													((uint32_t)0x00000001)
#define VCAP_Disable												((uint32_t)0xfffffffe)

/* video capture field sample mode mask */
#define VCAP_SMP_MODE												((uint32_t)0xfffffcff)

/* video capture field input channel number mask */
#define VCAP_RXCH_NUM												((uint32_t)0xffffffe7)

/* video capture color bit mask */
#define VCAP_COLOR_Bit												((uint32_t)0xfffffffd)

/* video capture channel0 drawing done interrupt mask*/
#define VCAP_CH0_ITEn												((uint32_t)0x00000001)
#define VCAP_CH0_ITDis												((uint32_t)0xfffffffe)

/* video capture channel1 drawing done interrupt mask*/
#define VCAP_CH1_ITEn												((uint32_t)0x00000002)
#define VCAP_CH1_ITDis												((uint32_t)0xfffffffd)

/* video capture channel2 drawing done interrupt mask*/
#define VCAP_CH2_ITEn												((uint32_t)0x00000004)
#define VCAP_CH2_ITDis												((uint32_t)0xfffffffb)

/* video capture channel3 drawing done interrupt mask*/
#define VCAP_CH3_ITEn												((uint32_t)0x00000008)
#define VCAP_CH3_ITDis												((uint32_t)0xfffffff7)

/* video capture write full data fifo interrupt mask*/
#define VCAP_WRFULL_ITEn											((uint32_t)0x00000100)
#define VCAP_WRFULL_ITDis											((uint32_t)0xfffffeff)

/* video capture data buffer overwrite interrupt mask*/
#define VCAP_OVERWR_ITEn											((uint32_t)0x00000200)
#define VCAP_OVERWR_ITDis											((uint32_t)0xfffffeff)

/**
  * @}
  */

/** @defgroup _Private_Functions
  * @{
  */

/**
  * @brief  Deinitializes the VCAP registers to their default reset values.
  * @param        	
  * @retval None
  */

typedef struct
{
	uint32_t CH0_BaseAddress;		/* channel0 field write base address */
	uint32_t CH0_FieldCount;		/* channel0 field count */
	uint32_t CH1_BaseAddress;		/* channel1 filed write base address */
	uint32_t CH1_FieldCount;		/* channel1 field count */
	uint32_t CH2_BaseAddress;		/* channel2 field write base address */
	uint32_t CH2_FieldCount;		/* channel2 field count */
	uint32_t CH3_BaseAddress;		/* channel3 field write base address */
	uint32_t CH3_FieldCount;		/* channel3 field count */
	uint32_t SMP_Mode;				/* field sample mode */
	uint32_t RXCH_Num;				/* number of input channels */
	uint32_t MAXFIELD_Size;			/* max field size */
	uint32_t COL_Bit;				/* color bit */
} VIDEOCAPTURE_CONTROL_Type;

class VideoCaptureDriver
{
private:
	VIDEOCAPTURE_CONTROL_Type mVIDEOCAPTURE_CONTROL_Type;

public:
/**
  * @brief  VCAP field sample mode select.
  * @param  Mode: field sample mode select.
  *         This parameter can be: VCAP_SAMPLE_ODD, VCAP_SAMPLE_EVEN, VCAP_SAMPLE_BOTH.
  * @retval None
  */
	void VideoCapture_SetSmpMode(uint32_t Mode);

/**
  * @brief  VCAP color bit select.
  * @param  Bit: color bit.
  *         This parameter can be: VCAP_COLOR_16, VCAP_COLOR_32.
  * @retval None
  */
	void VideoCapture_SetColBit(uint32_t Bit);

/**
  * @brief  set max field size.
  * @param  Size: max field size.
  * @retval None
  */
	void VideoCapture_SetMFSize(uint32_t Size);

/**
  * @brief  VCAP input channels number select.
  * @param  CHANUM: input channels number select.
  *         This parameter can be: VCAP_RXCH_1, VCAP_RXCH_2, VCAP_RXCH_4.
  * @retval None
  */
	void VideoCapture_SetChannel(uint32_t ChannelNumber);

/**
  * @brief  Set specified channel field write base addr.
  * @param  CHx: video capture input channel number.
  *         This parameter can be CH0, CH1, CH2, CH3.
  *	@param  BassAdress: field write base address.
  * @retval None
  */
	void VideoCapture_SetCHxBaseAddress(uint8_t CHx, uint32_t BaseAddress);

/**
  * @brief  Set specified channel field count.
  * @param  CHx: video capture input channel number.
  *         This parameter can be CH0, CH1, CH2, CH3.
  *	@param  FieldCount: field count.
  * @retval None
  */
	void VideoCapture_SetCHxFieldCount(uint8_t CHx, uint32_t FieldCount);

/**
  * @brief  Set Video Capture registers.
  *	@param  None
  * @retval None
  */
	void VideoCapture_SetRegs(void);

};

#endif