////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/10/16
// File Name:     AHMIDriverParameters.h
// Project Name:  AHMIGEN_V2.0
// 
// Revision:
// Revision 2.00 - File Created 2018/10/16 by Zhou Yuzhi
// Additional Comments:
//    the definition of parameters for simualtor layer
//
////////////////////////////////////////////////////////////////////////////////
#ifndef SIMULATOR_PARAMETERS__H
#define SIMULATOR_PARAMETERS__H

#include "ahmiv3_vd.h"
#include "publicType.h"

#ifdef VERSION_2
//regs structure for AHMI
typedef struct _VIDEOCAPTURE_REGS
{
	u32 control;			//the control signal, only used in hardware
	u32 interruptControl;	//interrupt signal, only used in hardware
	u32 interruptStatus;	//interrupt status, only used in hardware
	u32 maxfieldsize;		//max field size
	u32 CH0Parameter;		//only used in hardware for read only 
	u32 CH0BaseAddr;		//CH0 Field write base addr
	u32 CH1Parameter;		//only used in hardware for read only 
	u32 CH1BaseAddr;		//CH1 Field write base addr
	u32 CH2Parameter;		//only used in hardware for read only 
	u32 CH2BaseAddr;		//CH2 Field write base addr
	u32 CH3Parameter;		//only used in hardware for read only 
	u32 CH3BaseAddr;		//CH3 Field write base addr
	u32 CH0FieldCount;		//CH0 Field count
	u32 CH1FieldCount;		//CH1 Field count
	u32 CH2FieldCount;		//CH2 Field count
	u32 CH3FieldCount;		//CH3 Field count
}VIDEOCAPTURE_REGS;//the reg list of video capture module

typedef struct _DISPLAY_REGS
{
	u32 control; //the control signal, only used in hardware
	u32 interruptEnable; //interrupt signal, only used in hardware
	u32 interruptStatus; //interrupt status, only used in hardware
	u32 reserved0;       //reserved
	u32 reserved1;       //reserved
	u32 primaryAddress;  //the adress of the frame
	u32 primaryParam1 ; //[31:16] is the width of the screen, the other reserved
	u32 primaryParam2 ; //only used in hardware
	u32 primaryParam3 ; //[31:16] is the height of the screen, the other reserved
	u32 primaryParam4 ; //only used in hardware
	u32 reserved2;     	//reserved
	u32 reserved3;		//reserved
	u32 reserved4;		//reserved
	u32 secondaryAddress;  //only used in hardware
	u32 secondaryParam1 ; //only used in hardware
	u32 secondaryParam2 ; //only used in hardware
	u32 secondaryParam3 ; //only used in hardware
	u32 secondaryParam4 ; //only used in hardware
	u32 rgbAdjustParam1 ; //[31:16] CONTRAST [15:0] BRIGHT
	u32 rgbAdjustParam2 ; //[31:16] reserved [15:0] SATURABILITY
}DISPLAY_REGS; //the reg list of display module

typedef struct _AHMIREGS
{
	int SYSTEM_INFO;  //system information, [1:0] is core enable, only used in hardware
	int DEVICE_VER;   //device versoin, normally "SOC1", only used in hardware
	int REG02;        //CONTROL, 0 is restart AHMI, only used in hardware
	int START;        //start AHMI, only used in hardware
	int STATUS;       //the status of AHMI, only used in hardware
	int INTERUPT_MASK;//the mask of INTERRUPT, only used in hardware
	int OUPUT_ROT;    //the output rotation
	int DrawBox;   // 31:24	Y direction of end tile 23:16	X direction of end tile 15:8	Y direction of start tile 7:0	X direction of start tile
	int MaxSBNum;  // max sourcebuffer element number(256 default)
	int SB_ADDR;   // the address of source buffer
	int FB_ADDR;   // the address of frame buffer
	int INTERRUPT; // interrupt, only used in hardware
	int FrameSize;//31:16 frame height 15:0 frame width
	int OUTPUFORMAT; //output foramt. 0 is rgb565, 1 is rgb888
}AHMIREGS;
#endif

#endif

