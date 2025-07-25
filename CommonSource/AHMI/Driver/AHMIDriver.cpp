////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/08/24
// File Name:     AHMIDriver.cpp
// Project Name:  AHMIGEN_V2.0
// 
// Revision:
// Revision 2.00 - File Created 2018/08/24 by Yu Ning
// Additional Comments:
//    
////////////////////////////////////////////////////////////////////////////////
#include "AHMIDriver.h"
#include "DisplayDriver.h"
#include "AHMIDriverParameters.h"
#include "drawImmediately_cd.h"

#if defined(EMBEDDED) && defined(VERSION_1)
#include "ahmi3_function.h"
#include "spi_if.h"
#endif

#ifdef PC_SIM
#include "Simulator\simulatorParameters.h"
#include "Simulator\ahmiv3.h"
#include   <stdlib.h>           
#include   <string.h>
#include "trace.h" 
#endif

#ifdef PC_SIM
DisplayDriver mDisplayDriver;
extern ahmi mAHMI;
extern AHMIDriver mAHMIDriver;
extern AHMIREGS AHMI_REGFILE;
extern u8  sourceBuffer[20480]; //for AHMI to test
extern u8  GlobalFrameBuffer[ MAX_SCREEN_SIZE *2];
extern uint32_t NumberOFLongTexture;
extern uint32_t NumberOFShortTexture;
FILE* fp_sb; 
#endif
extern u32 sourceshift;

#ifdef VERSION_1
extern u8		stm32info[];
#endif

/* ahmi interrupt enable mask */
#define AHMI_IT_EN										((uint32_t)0x80000000)
#define AHMI_IT_DIS										((uint32_t)0x7fffffff)

/* dram cache time over interrupt mask */
#define IT_DC_RDOVER_EN									((uint32_t)0x00000008)
#define IT_DC_RDOVER_DIS								((uint32_t)0xfffffff7)

/* source buffer ending not found interrupt mask */
#define IT_SB_ENF_EN									((uint32_t)0x00000004)
#define IT_SB_ENF_DIS									((uint32_t)0xfffffffb)

/* source buffer read time over interrupt mask */
#define IT_SB_RDOVER_EN									((uint32_t)0x00000002)
#define IT_SB_RDOVER_DIS								((uint32_t)0x0000000d)

/* drawing done mask */
#define IT_DRAW_DONE_EN									((uint32_t)0x00000001)
#define IT_DRAW_DONE_DIS								((uint32_t)0xfffffffe)

/* core0 enable mask */
#define CORE0_EN										((uint32_t)0x00000001)
#define CORE0_DIS										((uint32_t)0xfffffffe)

/* core1 enable mask */
#define	CORE1_EN										((uint32_t)0x00000002)
#define	CORE1_DIS										((uint32_t)0x0000000d)

/* GPE start mask */
#define GPE_START										((uint32_t)0x00000001)
#define GPE_END											((uint32_t)0xfffffffe)

/* output rotation mask */
#define OUTPUT_ROTATION_MASK							((uint32_t)0xfffffffc)

/**
	*	@brief set output rotation.
	*	@param Rotation: output rotation.
	*		   This parameter can be: DEGREE0,DEGREE90,DEGREE180,DEGREE270.
	*	@retval None.
	*/
void AHMIDriver::AHMI_SetRot(uint32_t Rotation)
{
	this->mAHMI_InitTypeDef.OUTPUT_Rotation = Rotation;
}

/**
	*	@brief set tile box
	*	@param XStart: X direction of start tile.
	*	@param YStart: Y direction of start tile.
	*	@param X direction of end tile.
	*	@param Y direction of end tile.
	*	@retval None.
	*/
void AHMIDriver::AHMI_TileBox(uint8_t XStart, uint8_t YStart, uint8_t XEnd, uint8_t YEnd)
{
	this->mAHMI_InitTypeDef.XDIR_StartTile = XStart;
	this->mAHMI_InitTypeDef.YDIR_StartTile = YStart;
	this->mAHMI_InitTypeDef.XDIR_EndTile = XEnd;
	this->mAHMI_InitTypeDef.YDIR_EndTile = YEnd;
#ifdef VERSION_1
	//set business object
    u8 *stm32ptraddr;
	stm32ptraddr = (u8 *)Stm32_interface_Addr;
	//set the stminfo for drawing
	//tile box
	*(stm32ptraddr + Tile_start_x) = this->mAHMI_InitTypeDef.XDIR_StartTile ;
	*(stm32ptraddr + Tile_start_y) = this->mAHMI_InitTypeDef.YDIR_StartTile;
	*(stm32ptraddr + Tile_end_x)   = this->mAHMI_InitTypeDef.XDIR_EndTile;
	*(stm32ptraddr + Tile_end_y)   = this->mAHMI_InitTypeDef.YDIR_EndTile ;
#endif
}

/**
	*	@brief set maximum size of source buffer.
	*	@param maximum size of source buffer.
	*	@retval None.
	*/
void AHMIDriver::AHMI_SetSBSize(uint32_t Size)
{
	this->mAHMI_InitTypeDef.MAX_SB = Size;
}

/**
	*	@brief set source buffer base address.
	*	@param SBAdress: source buffer base address.
	*	@retval None.
	*/
void AHMIDriver::AHMI_SetSBAdress(uint32_t SBAdress)
{
	this->mAHMI_InitTypeDef.SB_BASE = SBAdress;

}

/**
	*	@brief set frame buffer base address.
	*	@param FBAdress: frame buffer base address.
	*	@retval None.
	*/
void AHMIDriver::AHMI_SetFBAdress(uint32_t FBAdress)
{
	this->mAHMI_InitTypeDef.FB_BASE = FBAdress;
#ifdef VERSION_1
	//set business object
    u8 *stm32ptraddr;
	stm32ptraddr = (u8 *)Stm32_interface_Addr;
	//set the stminfo for drawing
	//address of frame buffer
	*( (u32*)(stm32ptraddr + addr_ahmi_0) ) = this->mAHMI_InitTypeDef.FB_BASE;
#endif
}

/**
	*	@brief set frame size.
	*	@param Width: frame width.
	*	@param Height: frame height.
	*	@retval None.
	*/
void AHMIDriver::AHMI_SetFSize(uint16_t Width, uint16_t Height)
{
	this->mAHMI_InitTypeDef.FRAME_Hight = Height;
	this->mAHMI_InitTypeDef.FRAME_Width = Width;
}

/**
	*	@brief set output format. 
	*	@param outputFormat: RGB565_Output is RGB565, RGB888_Output is RGB888
			   This parameter can be: RGB565_Output,RGB888_Output.
	*	@retval None.
	*/
void AHMIDriver::AHMI_SetOutputFormat(uint32_t outputFormat)
{
	this->mAHMI_InitTypeDef.OUTPUT_FORMAT = outputFormat;
}

/**
	*	@brief AHMI_SetRegs. write the regs to persistence objects(FPGA or REGS)
	*	@retval None.
	*/
void AHMIDriver::AHMI_SetRegs(void)
{
#ifdef VERSION_1

#ifdef PC_SIM
	mAHMIDriver.AHMI_SetFSize(gScreenWidth, gScreenHeight);
	mAHMI.SetAHMIRegs(this);
#endif
	
#ifdef EMBEDDED 
	uint16_t rcvData;
	
	//write to spi
	taskENTER_CRITICAL();
	rcvData = SPI_FPGA_Burst_Send_REG_data(STARTADDR_OF_STMINFO,(u16*)(stm32info),STM32_DRAWING_REGS_SIZE/2);
	taskEXIT_CRITICAL();

	//stm32_regs
	if(rcvData == 0)
		return;
#endif

#endif

#ifdef VERSION_2
	
#ifdef PC_SIM
	//mAHMIDriver.AHMI_TileBox(0, 0, gScreenWidth/32, gScreenHeight/32);
	//mAHMIDriver.AHMI_SetFBAdress((int)&GlobalFrameBuffer);
	mAHMIDriver.AHMI_SetSBAdress((int)&sourceBuffer);
	mAHMIDriver.AHMI_SetFSize(gScreenWidth, gScreenHeight);
	//mAHMIDriver.AHMI_SetSBSize(255);
	mAHMIDriver.AHMI_SetRot(DEGREE0);
	mAHMIDriver.AHMI_SetOutputFormat(RGB565_Output);
	mAHMI.SetAHMIRegs(&mAHMIDriver);
#endif
    
#if defined(EMBEDDED) && defined(VERSION_1)	
	uint16_t rcvData;
	
	//write to spi
	taskENTER_CRITICAL();
	rcvData = SPI_FPGA_Burst_Send_REG_data(STARTADDR_OF_STMINFO,(u16*)(stm32info),STM32_DRAWING_REGS_SIZE/2);
	taskEXIT_CRITICAL();

	//stm32_regs
	if(rcvData == 0)
		return;
#endif
	
#endif
}
#ifdef VERSION_2
/**
	*	@brief MallocSourceBuffer.
	*	@param maxSBSize: the max size of SB
	*	@param pSourceBuffer: if success, return the sourcebuffer address. else return NULL.
	*	@retval typeStatys, if connot malloc, return AHMI_MALLOC_FAILURE. else return AHMI_MALLOC_SUCCESS
	*/
TypeStatus AHMIDriver::AHMI_MallocSourceBuffer(uint32_t maxSBSize, NewSourceBuffer pSourceBuffer)
{
	return AHMI_MALLOC_SUCCESS;
}

/**
	*	@brief AHMI_MallocFrameBuffer.
	*	@param maxFBSize: the max size of FB
	*	@param pFrameBuffer: if success, return the address. else return NULL.
	*	@retval typeStatys, if connot malloc, return AHMI_MALLOC_FAILURE. else return AHMI_MALLOC_SUCCESS
	*/
TypeStatus AHMIDriver::AHMI_MallocFrameBuffer(uint32_t maxFBSize, u8 *pFrameBuffer)
{
	return AHMI_MALLOC_SUCCESS;
}
#endif

/**
	*	@brief GetStatus.
	*	@return AHMIFlagStatus, this could be AHMI_BUSY or AHMI_DONE.
	*/
AHMIFlagStatus AHMIDriver::AHMI_GetStatus(void)
{
	if(AHMI_REGFILE.STATUS==1) 
	{
		return AHMI_DONE;
	}
	else
	{
		return AHMI_BUSY; 
	}
}

void AHMIDriver::AHMI_Start(void)
{
#ifdef PC_SIM
#ifdef TEST_VECTOR_GEN

	u32 i;

	fp_sb =fopen("sourcebuffer.hex","w"); 
	if(fp_sb == NULL) 
	{ 
		ERROR_PRINT("connot open sourcebuffer.hex"); 
	} 
	else  
	{ 
#ifdef  VERSION_1 
	for(i=0;i<this->mAHMI_InitTypeDef.MAX_SB;i+=2) 
	{
		fprintf(fp_sb,"%04X\n",*(u16*)(&sourceBuffer[i])); 
	}
	fclose(fp_sb); 
#endif
#ifdef VERSION_2
	for(i=0;i < (NumberOFLongTexture*64+NumberOFShortTexture*24);i+=4) 
	{ 
		fprintf(fp_sb,"%08X\n",*(u32*)(&sourceBuffer[i])); 
	} 
	fclose(fp_sb); 
#endif
	}
#endif//end of test vector gen 
    
	//write the regs to spi
    AHMI_SetRegs();
	mAHMI.DrawFrame();

#ifdef VERSION_2
	//mDisplayDriver.Display_SetFBAddress((int)&GlobalFrameBuffer);
	//if(mAHMIDriver.mAHMI_InitTypeDef.OUTPUT_Rotation == DEGREE90 || mAHMIDriver.mAHMI_InitTypeDef.OUTPUT_Rotation == DEGREE270)
	//	mDisplayDriver.Display_SetScreenWidthAndHeight(gScreenHeight, gScreenWidth);
	//else 
	//	mDisplayDriver.Display_SetScreenWidthAndHeight(gScreenWidth, gScreenHeight);
	//mDisplayDriver.Display_SetRegs();
	//mDisplayDriver.Display_StartRendering(GlobalFrameBuffer);
#endif

#endif
#if defined(EMBEDDED) && defined(VERSION_1)
	AHMIDraw(&sourceshift);
#endif
}

