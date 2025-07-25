////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2015/11/17
// File Name:     Trace.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef __AHMITOUCHDEFINE_H
#define __AHMITOUCHDEFINE_H
#include "stdint.h"
#include "platform.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef EMBEDDED
    
//#define TOUCHSCREENOFF              (0)
//#define TOUCHSCREENRESISTIVE        (1)
//#define TOUCHSCREENCAPACITIVE       (2)
//#define TOUCHSCREENRESISTIVE_ADC    (3)
//#define TOUCHSCREENSHMEM            (4)
//#define TOUCHSCREENTYPEEND          (5)

typedef enum TouchScreenTypeTag
{
    TOUCHSCREENOFF = (0),
    TOUCHSCREENRESISTIVE, 
    TOUCHSCREENRESISTIVE_ADC ,  
    TOUCHSCREENSHMEM_RESISTIVE,    
    TOUCHSCREENCAPACITIVE ,      
    TOUCHSCREENSHMEM_CAPACITIVE,           
    TOUCHSCREENTYPEEND ,         
}TouchScreenTypeDef;

typedef enum BinTouchScreenTypeTag
{
    BinTOUCHSCREENOFF = (0),
    BinTOUCHSCREENRESISTIVE, 
	BinTOUCHSCREENCAPACITIVE ,  
    BinTOUCHSCREENRESISTIVE_ADC ,  
    BinTOUCHSCREENSHMEM_RESISTIVE,    
    BinTOUCHSCREENSHMEM_CAPACITIVE,           
    BinTOUCHSCREENTYPEEND ,         
}BinTouchScreenTypeDef;

    
#define NOTOUCH	    ((uint32_t)0x00000000)
#define TOUCHED		((uint32_t)0x00000001)

//enum TouchEvent {NOTHING = 0,CLICK,RELEASE,HOLD};
#define NOTHING     ((uint32_t)0x00000000)
#define CLICK       ((uint32_t)0x00000001)
#define RELEASE     ((uint32_t)0x00000002)
#define HOLD        ((uint32_t)0x00000003)

//calibrating point size
#define POINTERSIZE                 4
#define	HALFPOINTERSIZE             (POINTERSIZE / 2)	


#define SAMP_CNT                            8//sample count
#define SAMP_CNT_DIV2                       (SAMP_CNT/2)


typedef struct PositionTag
{
	int pos_x;
	int pos_y;
	int event;
}Position;

typedef struct TouchRotationTag
{
	int mA;
	int mB;
	int mC;
    int mD;
    int mE;
    int mF;
    int mRotate;
}TouchRotation;

#endif

#ifdef __cplusplus 
}
#endif
#endif
