////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMIС���Ա
//
// Create Date:   2018/08/24
// File Name:     AHMIDriver.h
// Project Name:  AHMIGEN_V2.0
//
// Revision:
// Revision 2.00 - File Created 2018/08/24 by Yu Ning
// Additional Comments:
//
////////////////////////////////////////////////////////////////////////////////
#ifndef AHMI_DRIVER__H
#define AHMI_DRIVER__H

#include "stdint.h"
#include "AHMIDriverParameters.h"
#include "publicDefine.h"

typedef enum {AHMI_MALLOC_FAILURE = 0, AHMI_MALLOC_SUCCESS = !AHMI_MALLOC_FAILURE} TypeStatus;
typedef enum {AHMI_BUSY  = 0, AHMI_DONE = !AHMI_BUSY } AHMIFlagStatus;

/** @defgroup ahmi core number
	* @{
	*/

#define CORE0						((uint8_t)0x00)
#define CORE1						((uint8_t)0x01)
#define	IS_AHMI_CORE(CORE)			(((CORE) == CORE0) || \
									((CORE) == CORE1))

/**
	*	@}
	*/

/** @defgroup ahmi interrupt status
	* @{
	*/

#define ST_ALTOR32_DONE				((uint32_t)0x80000000)		/* altor32 done status mask */
#define ST_DC_RDOVER				((uint32_t)0x00000008)		/* dram cache read time over status mask */
#define ST_SB_ENF					((uint32_t)0x00000004)		/* source buffer ending not found status mask */
#define ST_SB_RDOVER				((uint32_t)0x00000002)		/* source buffer read time over status mask */
#define ST_DRAW_DONE				((uint32_t)0x00000001)		/* drawing done status mask */
#define IS_AHMI_STATUS(STATUS)		(((STATUS) == ST_ALTOR32_DONE) || \
									((STATUS) == ST_DC_RDOVER) || \
									((STATUS) == ST_SB_ENF) || \
									((STATUS) == ST_SB_RDOVER) || \
									((STATUS) == ST_DRAW_DONE))

/**
	*	@}
	*/

/** @defgroup ahmi interrupt
	* @{
	*/

#define IT_INTERRUPT_EN				((uint32_t)0x80000000)		/* dram cache read time over interrupt */
#define IT_DC_RDOVER				((uint32_t)0x00000008)		/* dram cache read time over interrupt */
#define IT_SB_ENF					((uint32_t)0x00000004)		/* source buffer ending not found interrupt */
#define IT_SB_RDOVER				((uint32_t)0x00000002)		/* source buffer read time over interrupt */
#define IT_DRAW_DONE				((uint32_t)0x00000001)		/* drawing done interrupt */
#define IS_AHMI_IT(IT)				(((IT) == IT_INTERRUPT_EN) || \
									((IT) == ST_DC_RDOVER) || \
									((IT) == ST_SB_ENF) || \
									((IT) == ST_SB_RDOVER) || \
									((IT) == ST_DRAW_DONE))

/**
	*	@}
	*/

/** @defgroup ahmi output rotation angle
	* @{
	*/

#define DEGREE0									((uint32_t)0x00000000)		/* 0 degree */
#define DEGREE90								((uint32_t)0x00000001)		/* 90 degree */
#define DEGREE180								((uint32_t)0x00000002)		/* 180 degree */
#define DEGREE270								((uint32_t)0x00000003)		/* 270 degree */
#define IS_AHMI_OUTPUT_ROTATION(ROTATION)		(((ROTATION) == DEGREE0) || \
												((ROTATION) == DEGREE90) || \
												((ROTATION) == DEGREE180) || \
												((ROTATION) == DEGREE270))

/**
	*	@}
	*/

/**	@defgroup	video capture input channel definition
	*	@{
	*/

//#define	CH0										((uint8_t)0x00)	/* channel0 */
//#define CH1										((uint8_t)0x01)	/* channel1 */
#define CH2										((uint8_t)0x02)	/* channel2 */
#define CH3										((uint8_t)0x03)	/* channel3 */
#define	IS_VCAP_CH(CHx)							(((CHx) == CH0) || \
												((CHx) == CH1) || \
												((CHx) == CH2) || \
												((CHx) == CH3))

/**
	*	@}
	*/

/**	@defgroup	core drawing information
	*	@{
	*/

#define INF_CORE_EN								((uint32_t)0x80000000)		/* core enable status */
#define	INF_CORE_RASTER_BUSY					((uint32_t)0x00000004)		/* core raster busy status */
#define	INF_CORE_RASTER_DONE					((uint32_t)0x00000002)		/* core raster done status */
#define	INF_CORE_TB_DONE						((uint32_t)0x00000001)		/* core TB done status */
#define IS_CORE_INF(INF)						(((INF) == INF_CORE_EN)	|| \
												((INF) == INF_CORE_RASTER_BUSY) || \
												((INF) == INF_CORE_RASTER_DONE) || \
												((INF) == INF_CORE_TB_DONE))

/**
	*	@}
	*/

/**	@defgroup	OutputFormat
	*	@{
	*/
#ifdef VERSION_2
#define RGB565_Output							((uint32_t)0x00000000)		/* rgb565 */
#define	RGB888_Output							((uint32_t)0x00000001)		/* rgb888 */
#endif




class AHMIDriver
{
public:
	AHMI_InitTypeDef mAHMI_InitTypeDef;


	/**
	*	@brief set output rotation.
	*	@param Rotation: output rotation.
	*		   This parameter can be: DEGREE0,DEGREE90,DEGREE180,DEGREE270.
	*	@retval None.
	*/
	void AHMI_SetRot(uint32_t Rotation);

	/**
	*	@brief set tile box
	*	@param XStart: X direction of start tile.
	*	@param YStart: Y direction of start tile.
	*	@param X direction of end tile.
	*	@param Y direction of end tile.
	*	@retval None.
	*/
	void AHMI_TileBox(uint8_t XStart, uint8_t YStart, uint8_t XEnd, uint8_t YEnd);

	/**
	*	@brief set maximum size of source buffer.
	*	@param maximum size of source buffer.
	*	@retval None.
	*/
	void AHMI_SetSBSize(uint32_t Size);

	/**
	*	@brief set source buffer base address.
	*	@param SBAdress: source buffer base address.
	*	@retval None.
	*/
	void AHMI_SetSBAdress(uint32_t SBAdress);

	/**
	*	@brief set frame buffer base address.
	*	@param FBAdress: frame buffer base address.
	*	@retval None.
	*/
	void AHMI_SetFBAdress(uint32_t FBAdress);

	/**
	*	@brief set frame size.
	*	@param Width: frame width.
	*	@param Height: frame height.
	*	@retval None.
	*/
	void AHMI_SetFSize(uint16_t Width, uint16_t Height);

	/**
	*	@brief set output format. RGB565_Output is rgb565, RGB888_Output is rgb888
	*	@param Width: frame width.
	*	@param Height: frame height.
	*	@retval None.
	*/
	void AHMI_SetOutputFormat(uint32_t outputFormat);

	/**
	*	@brief set output format. 0 is rgb565, 1 is rgb888
	*	@param Width: frame width.
	*	@param Height: frame height.
	*	@retval None.
	*/
	void AHMI_SetRegs(void);

#ifdef VERSION_2
	/**
	*	@brief set output format. 0 is rgb565, 1 is rgb888
	*	@param maxSBSize: the max size of SB
	*	@param pSourceBuffer: if success, return the sourcebuffer address. else return NULL.
	*	@retval typeStatys, if connot malloc, return AHMI_MALLOC_FAILURE. else return AHMI_MALLOC_SUCCESS
	*/
	TypeStatus AHMI_MallocSourceBuffer(uint32_t maxSBSize, NewSourceBuffer pSourceBuffer);

	/**
	*	@brief set output format. 0 is rgb565, 1 is rgb888
	*	@param maxFBSize: the max size of FB
	*	@param pFrameBuffer: if success, return the address. else return NULL.
	*	@retval typeStatys, if connot malloc, return AHMI_MALLOC_FAILURE. else return AHMI_MALLOC_SUCCESS
	*/
	TypeStatus AHMI_MallocFrameBuffer(uint32_t maxFBSize, u8 *pFrameBuffer);
#endif

	/**
	*	@brief set output format. 0 is rgb565, 1 is rgb888
	*	@param Width: frame width.
	*	@param Height: frame height.
	*	@return AHMIFlagStatus, this could be AHMI_BUSY or AHMI_DONE.
	*/
	AHMIFlagStatus AHMI_GetStatus(void);

	/**
	*	@brief start ahmi drawing
	*	@param DrawCallInfoClassPtr // the drawcall information
	*	@retval None.
	*/
	void AHMI_Start(void);

};

typedef AHMIDriver* AHMIDriverPtr;


#endif

