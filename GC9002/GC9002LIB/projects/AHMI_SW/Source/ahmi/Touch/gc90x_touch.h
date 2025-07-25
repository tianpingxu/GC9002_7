#ifndef _GC90x_TOUCH_H
#define _GC90x_TOUCH_H

//#include  "CPU1_CMSDK_CM0.h"
//#include "gc90x_adc.h"
#include "AHMITouchDefine.h"
#include "AHMITouch.h"

#define GCADCTOUCH_DELAY_VALUE  2
#define GCADCTOUCH_VALUE_BUFFER_LENGTH  12

#define GCADC_X_TOUTH_THREHOLD          100
#define GCADC_Y_TOUTH_THREHOLD          100
#define GCADC_Z_TOUTH_THREHOLD          0x80

typedef enum
{
    GCADCTouch_enZ,
    GCADCTouch_enX,
    GCADCTouch_enY,
}GCADCTouch_teReadDirect;

typedef enum
{
    GCADCTouch_VarInit = 0,
//    GCADCTouch_GPIOInit,
    GCADCTouch_GPIOInitDelay, 
//    GCADCTouch_GetZValue,       
//    GCADCTouch_GetXValue,    
//    GCADCTouch_GetYValue,       
}GCADCTouch_teRtpMainSM;

typedef struct
{
     GCADCTouch_teReadDirect    teReadDirect;
     int32_t                    int32XValue;
     int32_t                    int32YValue;
     int32_t                    int32ZValue;
     int32_t                    int32tempXValue;
     int32_t                    int32tempYValue;
     int32_t                    int32tempZValue;
     uint32_t                   u32Debounce;
     int                        int16X[GCADCTOUCH_VALUE_BUFFER_LENGTH];
     int                        int16Y[GCADCTOUCH_VALUE_BUFFER_LENGTH];
     int                        int16Z[GCADCTOUCH_VALUE_BUFFER_LENGTH];
     GCADCTouch_teRtpMainSM     teRtpManSM;
     uint32_t                   u32PreTick;
}GCADCTouch_tsWorkbuf;

void Touch_Test(int *x_o,int *y_o,int *touch);
uint32_t GC90xTouch_GetTouchState(void);
void GC90xTouch_ReadPosition(Position* pPosition);
void GC90xTouch_GetScreenPositionXY(Position* ScreenCurPosition, Position* ScreenContactPosition, Position* ScreenLeavePosition);
uint16_t GC90xTouch_CalibrationByUI(void);
void GCADCTouch_vTest(void);

void GCADCtouch_vMain(void);
#endif
