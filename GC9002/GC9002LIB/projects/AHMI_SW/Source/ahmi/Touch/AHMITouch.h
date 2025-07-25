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
#ifndef __AHMITOUCH_H
#define __AHMITOUCH_H
#include "stdint.h"
#include "platform.h"
#include "AHMITouchDefine.h"
//#include "shmem_touch.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef EMBEDDED
   
#define TOUCH_CALIBRATION_ByUI5
//#define TOUCH_CALIBRATION_ByUI13

#define TOUCHCALIFLAG           (0x696C6163)

#define AHMITOUCHMAXREADVALUE   (4096)
#define TOUCKFILTERNUMBER       (15)

typedef struct TouchStateMachineTag
{
    Position mPosi;
    uint32_t mTimer;
}TouchFSM;

#if 0
void ClearCalibration_ing(void);
#endif
void SetTouchScreenType(uint32_t state);
void SetTouchScreenRotation(uint32_t rotation);
void SetTouchScreenInterchangeXY(uint32_t flag);
uint32_t GetTouchScreenType(void);
    
void ResetCalibrationflag(void);
void SetCalibrationflag(void);
uint32_t GetCalibrationflag(void);

void AHMITouch_SetRotateMatrix(void);
void AHMITouch_TouchToScreenByUI5(Position* touchposition, Position* screenposition);
void AHMITouch_TouchToScreenByUI13(Position* touchposition, Position* screenposition);
void AHMITouch_vGetTouchInfo(void);
void AHMITouch_GetTouchEvent(Position* pScreenCurPosition, Position* pScreenContactPosition, Position* pScreenLeavePosition);


uint16_t AHMITouch_CaliGetXY(Position* positon);//get position when calibration
uint16_t AHMITouch_Calibration(void);//calibrate

uint16_t AHMITouch_CalibrationByUI5(void);
uint16_t AHMITouch_CalibrationByUI13(void);
uint8_t AHMITouch_InitDevice(void);

int EraseFlashForTouch(void);//erase gc9003 flash storing calibrating parameter  
int ProgramFlashForTouchByUI5(void);//store calibrating parameter to gc9003 flash
int ProgramFlashForTouchByUI13(void);//store calibrating parameter to gc9003 flash
void ReadDataFromFlashForTouchByUI5(void);//read calibrating parameter from gc9003 flash
void ReadDataFromFlashForTouchByUI13(void);//read calibrating parameter from gc9003 flash

void CheckWhetherEnterDebuggingPageAfterStartup(Position* pScreenCurPosition);

#endif

#ifdef __cplusplus 
}
#endif
#endif
