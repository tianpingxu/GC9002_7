////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI Team
//
// Create Date:   2015/11/17
// File Name:     Trace.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////
#include "stdio.h"
#include "publicInclude.h"
#include "bsp_i2c_touch.h"
#include "i2c_aw2083.h"
#include "DynamicPageClass.h"
#include "AHMIInterface.h"
//#include "gc90x_gpio.h"
//#include "gc90x_i2c.h"
#include "gc90x_flash.h"
#include "gt9xx.h"
#include "gc90x_touch.h"
#include "gc90x_ahmi.h"
#include "gc90x_initial.h"
#include "AHMITouch.h"
#include "semphr.h"
#include "capacitiveTouch_Interface.h"

#include "csi_hal_driver_flash.h"

//extern SemaphoreHandle_t interuptSemaphore_touch;
int screen_a = 0, screen_b = 0, screen_c = 0;        //X = screen_a*pos_x + screen_b*pos_y + screen_c
int screen_d = 0, screen_e = 0, screen_f = 0;        //Y = screen_d*pos_x + screen_e*pos_y + screen_f

typedef struct TouchParaDef
{
    int screen_a; 
    int screen_b; 
    int screen_c;        //X = screen_a*pos_x + screen_b*pos_y + screen_c
    int screen_d; 
    int screen_e; 
    int screen_f;       //Y = screen_d*pos_x + screen_e*pos_y + screen_f
}TouchPara;

TouchPara transpara[5] = {0};

uint32_t flag_Calibration = 0;//flag to indicate calibrated or not
uint32_t InterchangeXY = 1;//flag to indicate calibrated or not
uint32_t TouchScreenRotation = 1;//flag to indicate calibrated or not

TouchScreenTypeDef TouchScreenType = TOUCHSCREENOFF;

TouchRotation TouchRatitonmatirx = {1, 0, 0, 0, 1, 0, 0};
TouchRotation TouchRatitonInversematirx = {1, 0, 0, 0, 1, 0, 0};

TouchFSM g_TM_stTouchFsm = {0};

uint8_t calibration_ing = 0;

#if 0
extern "C"{
void TouchTim_IRQHandler(void)
{
	
	if(calibration_ing)
		return;
	
	AHMITouch_vGetTouchInfo();
}
}

void ClearCalibration_ing(void)
{
	calibration_ing = 0;
}
#endif

/*******************************************************************************
* Function Name  : SetTouchScreenType
* Description    : Set touch screen type
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetTouchScreenType(uint32_t state)
{
    if(state == BinTOUCHSCREENRESISTIVE)
        TouchScreenType = TOUCHSCREENRESISTIVE;
    else if(state == BinTOUCHSCREENCAPACITIVE)
        TouchScreenType = TOUCHSCREENCAPACITIVE;
    else if(state == BinTOUCHSCREENRESISTIVE_ADC)
        TouchScreenType = TOUCHSCREENRESISTIVE_ADC;
    else if(state == BinTOUCHSCREENSHMEM_RESISTIVE)
        TouchScreenType = TOUCHSCREENSHMEM_RESISTIVE;
    else if(state == BinTOUCHSCREENSHMEM_CAPACITIVE)
        TouchScreenType = TOUCHSCREENSHMEM_CAPACITIVE;
    else if(state == BinTOUCHSCREENTYPEEND)
        TouchScreenType = TOUCHSCREENTYPEEND;
    else
        TouchScreenType = TOUCHSCREENOFF;

}

void SetTouchScreenRotation(uint32_t rotation)
{
    
    if(rotation == 0)
    {
         TouchScreenRotation = DEGREE0;
    }
    else if(rotation == 90)
    {
         TouchScreenRotation = DEGREE90;
    }
    else if(rotation == 180)
    {
         TouchScreenRotation = DEGREE180;
    }
    else if(rotation == 270)
    {
         TouchScreenRotation = DEGREE270;
    }
    else
    {
         TouchScreenRotation = DEGREE0;
    }
    
}

void SetTouchScreenInterchangeXY(uint32_t flag)
{
    if(flag == 1)
        InterchangeXY = 1;
    else
        InterchangeXY = 0;
}

/*******************************************************************************
* Function Name  : GetTouchScreenType
* Description    : get touch screen type
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t GetTouchScreenType(void)
{
    return TouchScreenType;
}

/*******************************************************************************
* Function Name  : ResetCalibrationflag
* Description    : Reset calibration flag
* Input          : None
* Output         : None
* Return         : state
*******************************************************************************/
void ResetCalibrationflag(void)
{
    if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENCAPACITIVE))
	{
        flag_Calibration = 0;
		calibration_ing = 1;
	}
}

/*******************************************************************************
* Function Name  : SetCalibrationflag
* Description    : Set calibration flag
* Input          : None
* Output         : None
* Return         : state
*******************************************************************************/
void SetCalibrationflag(void)
{
    flag_Calibration = 1;
}

/*******************************************************************************
* Function Name  : SetCalibrationflag
* Description    : Set calibration flag
* Input          : None
* Output         : None
* Return         : state
*******************************************************************************/
uint32_t GetCalibrationflag(void)
{
    return flag_Calibration;
}

/*******************************************************************************
* Function Name  : SetCalibrationflag
* Description    : Set calibration flag
* Input          : None
* Output         : None
* Return         : state
*******************************************************************************/
void AHMITouch_InitRotateMatrix(TouchRotation* pMatrix, TouchRotation* pInverseMatrix)
{
    uint32_t displayRotation = 0;
    uint32_t TPRotation = 0;
    uint32_t temp = 0;
    displayRotation = GetAHMIOutputRotation();
    
    
    if(TouchScreenType < TOUCHSCREENCAPACITIVE)
    {
        if(displayRotation == DEGREE0)
        {
            pMatrix->mRotate = DEGREE0;
            pMatrix->mA = 1;
            pMatrix->mB = 0;
            pMatrix->mC = 0;
            pMatrix->mD = 0;
            pMatrix->mE = 1;
            pMatrix->mF = 0;
            
            pInverseMatrix->mRotate = DEGREE0;
            pInverseMatrix->mA = 1;
            pInverseMatrix->mB = 0;
            pInverseMatrix->mC = 0;
            pInverseMatrix->mD = 0;
            pInverseMatrix->mE = 1;
            pInverseMatrix->mF = 0;
        }
        else if(displayRotation == DEGREE90)
        {
            pMatrix->mRotate = DEGREE90;
            pMatrix->mA = 0;
            pMatrix->mB = -1;
            pMatrix->mC = SCREEN_WIDTH;
            pMatrix->mD = 1;
            pMatrix->mE = 0;
            pMatrix->mF = 0;
            
            pInverseMatrix->mRotate = DEGREE90;
            pInverseMatrix->mA = 0;
            pInverseMatrix->mB = 1;
            pInverseMatrix->mC = 0;
            pInverseMatrix->mD = -1;
            pInverseMatrix->mE = 0;
            pInverseMatrix->mF = SCREEN_WIDTH;
        }
        else if(displayRotation == DEGREE180)
        {
            pMatrix->mRotate = DEGREE180;
            pMatrix->mA = -1;
            pMatrix->mB = 0;
            pMatrix->mC = SCREEN_WIDTH;
            pMatrix->mD = 0;
            pMatrix->mE = -1;
            pMatrix->mF = SCREEN_HEIGHT;
            
            pInverseMatrix->mRotate = DEGREE180;
            pInverseMatrix->mA = -1;
            pInverseMatrix->mB = 0;
            pInverseMatrix->mC = SCREEN_WIDTH;//SCREEN_HEIGHT;//SCREEN_WIDTH;
            pInverseMatrix->mD = 0;
            pInverseMatrix->mE = -1;
            pInverseMatrix->mF = SCREEN_HEIGHT;//SCREEN_WIDTH;//SCREEN_HEIGHT;
        }
        else if(displayRotation == DEGREE270)
        {
            pMatrix->mRotate = DEGREE270;
            pMatrix->mA = 0;
            pMatrix->mB = 1;
            pMatrix->mC = 0;
            pMatrix->mD = -1;
            pMatrix->mE = 0;
            pMatrix->mF = SCREEN_HEIGHT;
            
            pInverseMatrix->mRotate = DEGREE270;
            pInverseMatrix->mA = 0;
            pInverseMatrix->mB = -1;
            pInverseMatrix->mC = SCREEN_HEIGHT;
            pInverseMatrix->mD = 1;
            pInverseMatrix->mE = 0;
            pInverseMatrix->mF = 0;
        }
        else
        {
            pMatrix->mRotate = DEGREE0;
            pMatrix->mA = 1;
            pMatrix->mB = 0;
            pMatrix->mC = 0;
            pMatrix->mD = 0;
            pMatrix->mE = 1;
            pMatrix->mF = 0;
            
            pInverseMatrix->mRotate = DEGREE0;
            pInverseMatrix->mA = 1;
            pInverseMatrix->mB = 0;
            pInverseMatrix->mC = 0;
            pInverseMatrix->mD = 0;
            pInverseMatrix->mE = 1;
            pInverseMatrix->mF = 0;
        }
    }
    else 
    {
    
        TPRotation = TouchScreenRotation;

        if(TPRotation == DEGREE0)
        {
            pMatrix->mRotate = DEGREE0;
            pMatrix->mA = 1;
            pMatrix->mB = 0;
            pMatrix->mC = 0;
            pMatrix->mD = 0;
            pMatrix->mE = 1;
            pMatrix->mF = 0;
        }
        else if(TPRotation == DEGREE90)
        {
            pMatrix->mRotate = DEGREE90;
            pMatrix->mA = 0;
            pMatrix->mB = 1;
            pMatrix->mC = 0;
            pMatrix->mD = -1;
            pMatrix->mE = 0;
            pMatrix->mF = SCREEN_HEIGHT;
        }
        else if(TPRotation == DEGREE180)
        {
            pMatrix->mRotate = DEGREE180;
            pMatrix->mA = -1;
            pMatrix->mB = 0;
            pMatrix->mC = SCREEN_WIDTH;
            pMatrix->mD = 0;
            pMatrix->mE = -1;
            pMatrix->mF = SCREEN_HEIGHT;
        }
        else if(TPRotation == DEGREE270)
        {
            pMatrix->mRotate = DEGREE270;
            pMatrix->mA = 0;
            pMatrix->mB = -1;
            pMatrix->mC = SCREEN_WIDTH;
            pMatrix->mD = 1;
            pMatrix->mE = 0;
            pMatrix->mF = 0;
        }
        else
        {
            pMatrix->mRotate = DEGREE0;
            pMatrix->mA = 1;
            pMatrix->mB = 0;
            pMatrix->mC = 0;
            pMatrix->mD = 0;
            pMatrix->mE = 1;
            pMatrix->mF = 0;
        }
        
        if(InterchangeXY == 1)
        {
            temp = pMatrix->mA;
            pMatrix->mA = pMatrix->mB;
            pMatrix->mB = temp;
            
            temp = pMatrix->mD;
            pMatrix->mD = pMatrix->mE;
            pMatrix->mE = temp;
        }
    }
}

void AHMITouch_SetRotateMatrix(void)
{
    AHMITouch_InitRotateMatrix(&TouchRatitonmatirx, &TouchRatitonInversematirx);
}

/*******************************************************************************
* Function Name  : AHMITouch_PositionRotate
* Description    : Rotate touch position:
*                : newX = A * X + B * Y + C
*                : newY = D * X + E * Y + F
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AHMITouch_PositionRotate(Position* CurPosition, TouchRotation* pRotationMatrix)
{
    int tempX = 0;
    int tempY = 0;
    
    if(TouchScreenType < TOUCHSCREENCAPACITIVE)
    {
        if(pRotationMatrix->mRotate == DEGREE0)
        {
            tempX = CurPosition->pos_x;
            tempY = CurPosition->pos_y;
        }
        else
        {
            tempX = pRotationMatrix->mA * CurPosition->pos_x + pRotationMatrix->mB * CurPosition->pos_y + pRotationMatrix->mC;
            tempY = pRotationMatrix->mD * CurPosition->pos_x + pRotationMatrix->mE * CurPosition->pos_y + pRotationMatrix->mF;
        }
        
        CurPosition->pos_x = tempX;
        CurPosition->pos_y = tempY;
    }
    else
    {
        tempX = pRotationMatrix->mA * CurPosition->pos_x + pRotationMatrix->mB * CurPosition->pos_y + pRotationMatrix->mC;
        tempY = pRotationMatrix->mD * CurPosition->pos_x + pRotationMatrix->mE * CurPosition->pos_y + pRotationMatrix->mF;
        
        CurPosition->pos_x = tempX;
        CurPosition->pos_y = tempY;  
    }
}

/*******************************************************************************
* Function Name  : AHMITouch_InitDevice
* Description    : initial touch screen device
* Input          : None
* Output         : None
* Return         : ucAck
*******************************************************************************/
uint8_t AHMITouch_InitDevice(void)
{
	#ifndef TOUCH_CALIBRATION_ByUI5	
	ReadDataFromFlashForTouchByUI13();
	#else
	ReadDataFromFlashForTouchByUI5();
	#endif
	
    /* NS2009/AW2083. */
    if(TouchScreenType == TOUCHSCREENRESISTIVE)
    {
        AW2083_InitDevice();
    }
    /* GC9003 ADC. */
    else if(TouchScreenType == TOUCHSCREENRESISTIVE_ADC)
    {
    }
    /* CAPACITIVETouch. */
    else if(TouchScreenType == TOUCHSCREENCAPACITIVE)
    {
		Capacitivetouch_vInit();
    }
    else if((TouchScreenType == TOUCHSCREENSHMEM_RESISTIVE) || (TouchScreenType == TOUCHSCREENSHMEM_CAPACITIVE))
    {
        /*  Configure touch screen and the chip */
        //SHMEMTouch_Init();
    }
    
    AHMITouch_InitRotateMatrix(&TouchRatitonmatirx, &TouchRatitonInversematirx);
    
    return 0;
}

/*******************************************************************************
* Function Name  : AW2083_ReadPosition
* Description    : store read location to CurPositon
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AHMITouch_ReadPosition(Position* CurPosition)
{
	CurPosition->event = g_TM_stTouchFsm.mPosi.event;
	CurPosition->pos_x = g_TM_stTouchFsm.mPosi.pos_x;
	CurPosition->pos_y = g_TM_stTouchFsm.mPosi.pos_y;
}

void AHMITouch_ReadPositionForCalibration(Position* CurPosition)
{
    Position TempPosition = {0, 0, NOTOUCH};
	int dif = 0, dif_threshold = 0;
	
    if(TouchScreenType == TOUCHSCREENRESISTIVE)
    {
		dif_threshold = 400; // NS2009ADC accuracy 12bits
        AW2083_ReadPosition(&TempPosition);//read position
    }
    else if(TouchScreenType == TOUCHSCREENRESISTIVE_ADC)
    {
		dif_threshold = 100; // GC9003ADC accuracy 10bits
        GC90xTouch_ReadPosition(&TempPosition);//get positon
    }
    else if(TouchScreenType == TOUCHSCREENCAPACITIVE)
    {
		dif_threshold = 100; // Capacitive screen for pixel num
        //GTP_ReadPosition(&TempPosition);//capacitive need not calibration
    }
    else if((TouchScreenType == TOUCHSCREENSHMEM_RESISTIVE) || (TouchScreenType == TOUCHSCREENSHMEM_CAPACITIVE))
    {
		dif_threshold = 100; // GC9003ADC accuracy 10bits
        //SHMEMTouch_ReadPosition(&TempPosition);
    }
	
	if(TempPosition.event == TOUCHED)
	{
		if(CurPosition->event == TOUCHED)
		{
			dif = TempPosition.pos_x - CurPosition->pos_x;
			if(dif < 0)
				dif = -dif;
			
			if(dif >= dif_threshold)
			{
				return;
			}
			
			dif = TempPosition.pos_y - CurPosition->pos_y;
			if(dif < 0)
				dif = -dif;
			
			if(dif >= dif_threshold)
			{
				return;
			}
		}
		
		CurPosition->pos_x = TempPosition.pos_x;
		CurPosition->pos_y = TempPosition.pos_y;
		CurPosition->event = TempPosition.event;
	}
	else
	{
		CurPosition->event = TempPosition.event;
	}
}

/*******************************************************************************
* Function Name  : AHMITouch_CaliGetXY
* Description    : read touch loation when calibrate and store to positon struct
* Output         : position
* Return         : None
*******************************************************************************/
uint16_t AHMITouch_CaliGetXY(Position* position)
{
	u8 i, j, k, min;
	u32 temp;
	u32 tempXY[2][SAMP_CNT], XY[2];
//    uint32_t counter = 0;
    Position CurPosition;
 	
	//judge pointer is valid or not
	if(NULL == position)
	{
		return 0;
	}
	
	//get SAMP_CNT times sample value for every touch
	for(i=0; i<SAMP_CNT;)
	{
        CurPosition.event = NOTOUCH;
        
        AHMITouch_ReadPositionForCalibration(&CurPosition);
        
        
        if(CurPosition.event == TOUCHED)
        {            
            if(CurPosition.pos_x != 0)
            {
                tempXY[0][i] = CurPosition.pos_x;
                tempXY[1][i] = CurPosition.pos_y;
                i++;
                
//                for(counter = 0; counter < 560000; counter++);
            }
        }
		else
		{
			return 0;
		}
	}
	
	//sort sample data,from min to max
	for(k=0; k<2; k++)
	{ 
		for(i=0; i<SAMP_CNT-1; i++)
		{
			min=i;
			for (j=i+1; j<SAMP_CNT; j++)
			{
				if (tempXY[k][min] > tempXY[k][j]) min=j;
			}
			temp = tempXY[k][i];
			tempXY[k][i] = tempXY[k][min];
			tempXY[k][min] = temp;
		}

		//if vary value beyond 20,then return
		if((tempXY[k][SAMP_CNT_DIV2]-tempXY[k][SAMP_CNT_DIV2-1]) > 20)
		return 0;

		//get average from middle 
		XY[k] = (tempXY[k][SAMP_CNT_DIV2]+tempXY[k][SAMP_CNT_DIV2-1]) / 2;
	}
	position->pos_x = XY[0];
	position->pos_y = XY[1];
	return 1;
}



/*******************************************************************************
* Function Name  : AHMITouch_Calibration
* Description    : calibration function ,if calibrate success ,then store calibrating parameter to code flash
* Output         : None
* Return         : status infor
*******************************************************************************/
uint16_t AHMITouch_Calibration(void)
{
	int delay1, delay2;//delay counter
	Position pt[5] = {{0}};//array to store touch location
	//five calibrating locations
	Position ps[5] = {{100,60, NOTHING},{SCREEN_WIDTH - 100,60, NOTHING},{100,SCREEN_HEIGHT - 60, NOTHING},{SCREEN_WIDTH - 100,SCREEN_HEIGHT - 60,NOTHING},{SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,NOTHING}};
	float k;
	//temple calibrating parameters
	float temp_screen_a = 0, temp_screen_b = 0, temp_screen_c = 0, temp_screen_d = 0, temp_screen_e = 0, temp_screen_f = 0;
	int mx1 = 0, mx2 = 0, mx3 = 0;
	int my1 = 0, my2 = 0, my3 = 0;
	//0xFF000000 is black,0xFFFF0000 is red
    uint32_t clacolor[5] = {0xFF000000,0xFF000000,0xFF000000,0xFF000000,0xFF000000};
	uint8_t i,j,m;
	uint16_t temp_x = 0, temp_y = 0;
	
	//if SCREEN_WIDTH less than 481,modify calibrating locations
	if(SCREEN_WIDTH < 481)
	{
		ps[0].pos_x = 50;
		ps[0].pos_y = 30;
		
		ps[1].pos_x = SCREEN_WIDTH - 50;
		ps[1].pos_y = 30;
		
		ps[2].pos_x = 50;
		ps[2].pos_y = SCREEN_HEIGHT - 30;
		
		ps[3].pos_x = SCREEN_WIDTH - 50;
		ps[3].pos_y = SCREEN_HEIGHT - 30;
		
		ps[4].pos_x = SCREEN_WIDTH / 2 - POINTERSIZE / 2;
		ps[4].pos_y = SCREEN_HEIGHT / 2 - POINTERSIZE / 2;
	}
    
    for(i = 0; i < 5; i++)
    {
        AHMITouch_PositionRotate(&ps[i], &TouchRatitonInversematirx);
    }
    
	for(i = 0; i < 5; i++)
	{
		clacolor[i] = 0xFFFF0000;//red colour
		//draw five point page
		for (j=0; j<3; j++)
		{
			DrawFivePointPage(
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				0xFFFFFFFF, //BackgroundColor:white
				POINTERSIZE,
				//LeftTop position of LeftTop Point
				ps[0].pos_x - HALFPOINTERSIZE, 
				ps[0].pos_y - HALFPOINTERSIZE,
				clacolor[0], //Pointer0Color:red
				//LeftTop position of RightTop Point
				ps[1].pos_x - HALFPOINTERSIZE, 
				ps[1].pos_y - HALFPOINTERSIZE,
				clacolor[1], //Pointer1Color:black
				//LeftTop position of LeftBottum Point
				ps[2].pos_x - HALFPOINTERSIZE, 
				ps[2].pos_y - HALFPOINTERSIZE,
				clacolor[2], //Pointer1Color:black
				//LeftTop position of RightBottum Point
				ps[3].pos_x - HALFPOINTERSIZE, 
				ps[3].pos_y - HALFPOINTERSIZE,
				clacolor[3], //Pointer1Color:black
				//LeftTop position of Center Point
				ps[4].pos_x - HALFPOINTERSIZE, 
				ps[4].pos_y - HALFPOINTERSIZE,
				clacolor[4] //Pointer1Color:black
			);
			for(delay1 = 0; delay1 < 30; delay1++)
					for(delay2 = 0; delay2 < 36000;)
						delay2++;

	  }
//		printf("Please Click The Point\n");
		while((AHMITouch_CaliGetXY(&pt[i]) == 0));

		clacolor[i] = 0xFF000000;
        for(j = 0; j < 3; j++)
        {
            DrawFivePointPage(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
                0xFFFFFFFF, //BackgroundColor:white
                POINTERSIZE,
                //LeftTop position of LeftTop Point
                ps[0].pos_x - HALFPOINTERSIZE, 
                ps[0].pos_y - HALFPOINTERSIZE,
                clacolor[0], //Pointer0Color:red
                //LeftTop position of RightTop Point
                ps[1].pos_x - HALFPOINTERSIZE, 
                ps[1].pos_y - HALFPOINTERSIZE,
                clacolor[1], //Pointer1Color:black
                //LeftTop position of LeftBottum Point
                ps[2].pos_x - HALFPOINTERSIZE, 
                ps[2].pos_y - HALFPOINTERSIZE,
                clacolor[2], //Pointer1Color:black
                //LeftTop position of RightBottum Point
                ps[3].pos_x - HALFPOINTERSIZE, 
                ps[3].pos_y - HALFPOINTERSIZE,
                clacolor[3], //Pointer1Color:black
                //LeftTop position of Center Point
                ps[4].pos_x - HALFPOINTERSIZE, 
                ps[4].pos_y - HALFPOINTERSIZE,
                clacolor[4] //Pointer1Color:black
            );
            for(delay1 = 0; delay1 < 30; delay1++)
                for(delay2 = 0; delay2 < 36000;)
                    delay2++;
        }
		for(delay1 = 0; delay1 < 20; delay1++)
			for(delay2 = 0; delay2 < 36000;)
				delay2++;
		
	}
	
	//calculate x difference 
	mx1 = (int)pt[1].pos_x - (int)pt[0].pos_x;
	mx2 = (int)pt[3].pos_x - (int)pt[2].pos_x;
	
	//calculate y difference 
	my1 = (int)pt[0].pos_y - (int)pt[2].pos_y;
	my2 = (int)pt[1].pos_y - (int)pt[3].pos_y;
	
	//absolute mx1 and mx2
	mx1 = mx1 > 0 ? mx1 : (-mx1);
	mx2 = mx2 > 0 ? mx2 : (-mx2);
	
	//absolute my1 and my2
	my1 = my1 > 0 ? my1 : (-my1);
	my2 = my2 > 0 ? my2 : (-my2);
	
	//calculate difference 
	mx3 = mx1 - mx2;
	my3 = my1 - my2;
	
	//absolute mx3 and my3
	mx3 = mx3 > 0 ? mx3 : (-mx3);
	my3 = my3 > 0 ? my3 : (-my3);
	
//	printf("mx3:%d\n",mx3);
//	printf("my3:%d\n",my3);
	
	if((mx3 <= 256) && (my3 <= 128))
	{
		//calculate calibrating parameters below
		k = (pt[0].pos_x - pt[3].pos_x) * (pt[1].pos_y - pt[3].pos_y) - (pt[1].pos_x - pt[3].pos_x) * (pt[0].pos_y - pt[3].pos_y);
		temp_screen_a = ((ps[0].pos_x - ps[3].pos_x) * (pt[1].pos_y - pt[3].pos_y) / k) - ((ps[1].pos_x - ps[3].pos_x) * (pt[0].pos_y - pt[3].pos_y) / k);
		temp_screen_b = ((pt[0].pos_x - pt[3].pos_x) * (ps[1].pos_x - ps[3].pos_x) / k) - ((ps[0].pos_x - ps[3].pos_x) * (pt[1].pos_x - pt[3].pos_x) / k);
		temp_screen_c = pt[0].pos_y * ((pt[3].pos_x * ps[1].pos_x - pt[1].pos_x * ps[3].pos_x) / k) + pt[1].pos_y * ((pt[0].pos_x * ps[3].pos_x - pt[3].pos_x * ps[0].pos_x) / k) + pt[3].pos_y * ((pt[1].pos_x * ps[0].pos_x - pt[0].pos_x * ps[1].pos_x) / k);
		temp_screen_d = ((ps[0].pos_y - ps[3].pos_y) * (pt[1].pos_y - pt[3].pos_y) / k) - ((ps[1].pos_y - ps[3].pos_y) * (pt[0].pos_y - pt[3].pos_y) / k); 
		temp_screen_e = ((pt[0].pos_x - pt[3].pos_x) * (ps[1].pos_y - ps[3].pos_y) / k) - ((ps[0].pos_y - ps[3].pos_y) * (pt[1].pos_x - pt[3].pos_x) / k);
		temp_screen_f = pt[0].pos_y * ((pt[3].pos_x * ps[1].pos_y - pt[1].pos_x * ps[3].pos_y) / k) + pt[1].pos_y * ((pt[0].pos_x * ps[3].pos_y - pt[3].pos_x * ps[0].pos_y) / k) + pt[3].pos_y * ((pt[1].pos_x * ps[0].pos_y - pt[0].pos_x * ps[1].pos_y) / k);
		
		temp_x = (uint16_t)(temp_screen_a * pt[4].pos_x + temp_screen_b * pt[4].pos_y + temp_screen_c);
		temp_y = (uint16_t)(temp_screen_d * pt[4].pos_x + temp_screen_e * pt[4].pos_y + temp_screen_f);
		
		mx1 = temp_x - ps[4].pos_x;
		my1 = temp_y - ps[4].pos_y;
		
//		printf("X:%d\n",temp_x);
//		printf("Y:%d\n",temp_y);
		
		mx1 = mx1 > 0 ? mx1 : (-mx1);
		my1 = my1 > 0 ? my1 : (-my1);
		//calculate calibrating parameters above
		
		if((mx1 <= 10) && (my1 <= 10))
		{
			screen_a = (int)(temp_screen_a * 0x10000);
			screen_b = (int)(temp_screen_b * 0x10000);
			screen_c = (int)(temp_screen_c + 0.5);
			screen_d = (int)(temp_screen_d * 0x10000);
			screen_e = (int)(temp_screen_e * 0x10000);
			screen_f = (int)(temp_screen_f + 0.5);
			flag_Calibration = 1;
//			printf("Calibrate Success");
			while(EraseFlashForTouch() != 1);
			while(ProgramFlashForTouchByUI5() != 1);//store parameters to stm flash

      //draw five points page		
       for (m=0; m<3; m++)			
	   {
			DrawFivePointPage(
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				0xFFFFFFFF, //BackgroundColor:white
				0,
				//LeftTop position of LeftTop Point
				ps[0].pos_x - HALFPOINTERSIZE, 
				ps[0].pos_y - HALFPOINTERSIZE,
				clacolor[0], //Pointer0Color:red
				//LeftTop position of RightTop Point
				ps[1].pos_x - HALFPOINTERSIZE, 
				ps[1].pos_y - HALFPOINTERSIZE,
				clacolor[1], //Pointer1Color:black
				//LeftTop position of LeftBottum Point
				ps[2].pos_x - HALFPOINTERSIZE, 
				ps[2].pos_y - HALFPOINTERSIZE,
				clacolor[2], //Pointer1Color:black
				//LeftTop position of RightBottum Point
				ps[3].pos_x - HALFPOINTERSIZE, 
				ps[3].pos_y - HALFPOINTERSIZE,
				clacolor[3], //Pointer1Color:black
				//LeftTop position of Center Point
				ps[4].pos_x - HALFPOINTERSIZE, 
				ps[4].pos_y - HALFPOINTERSIZE,
				clacolor[4] //Pointer1Color:black
                );
			}				
			return 1;
		}
	}
	return 0;
}
/*******************************************************************************
* Function Name  : AHMITouch_CalibrationByUI
* Description    : calibration function by UI,if calibrate success ,then store calibrating parameter to code flash
* Output         : None
* Return         : status infor
*******************************************************************************/
__attribute__((optimize(0))) uint16_t AHMITouch_CalibrationByUI5(void)
{
	Position pt[5] = {{0}};//array to store touch location
	//five calibrating locations
	Position ps[5] = {{100,60, NOTHING},{SCREEN_WIDTH - 100,60, NOTHING},{100,SCREEN_HEIGHT - 60, NOTHING},{SCREEN_WIDTH - 100,SCREEN_HEIGHT - 60,NOTHING},{SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,NOTHING}};
//    Position ps[5] = {{50,50, NOTHING},{SCREEN_WIDTH - 50,50, NOTHING},{50,SCREEN_HEIGHT - 50, NOTHING},{SCREEN_WIDTH - 50,SCREEN_HEIGHT - 50,NOTHING},{SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,NOTHING}};
	float k;
	//temple calibrating parameters
	float temp_screen_a = 0, temp_screen_b = 0, temp_screen_c = 0, temp_screen_d = 0, temp_screen_e = 0, temp_screen_f = 0;
	int mx1 = 0, mx2 = 0, mx3 = 0;
	int my1 = 0, my2 = 0, my3 = 0;
	int delay1, delay2;//delay counter
	uint8_t i;
//    uint8_t j,m;
	uint16_t temp_x = 0, temp_y = 0;
	
	uint16_t CalibratingPoint=0;
	
	calibration_ing = 1;
		
	//if SCREEN_WIDTH less than 481,modify calibrating locations
	if(SCREEN_WIDTH < 481)
	{
		ps[0].pos_x = 50;
		ps[0].pos_y = 30;
		
		ps[1].pos_x = SCREEN_WIDTH - 50;
		ps[1].pos_y = 30;
		
		ps[2].pos_x = 50;
		ps[2].pos_y = SCREEN_HEIGHT - 30;
		
		ps[3].pos_x = SCREEN_WIDTH - 50;
		ps[3].pos_y = SCREEN_HEIGHT - 30;
		
		ps[4].pos_x = SCREEN_WIDTH / 2 - POINTERSIZE / 2;
		ps[4].pos_y = SCREEN_HEIGHT / 2 - POINTERSIZE / 2;
	}
    
    for(i = 0; i < 5; i++)
    {
        AHMITouch_PositionRotate(&ps[i], &TouchRatitonInversematirx);
    }
    
	/*left top red point is bit0,black point is bit1; right top red point is bit2,black piont is bit3;left bottom red point is bit6,black piont is bit7;right bottom red point is bit4,black piont is bit5;center red point is bit8,black piont is bit9;*/
	for(i = 1; i < 6; i++)
	{
		if (i == 1)
		{
			CalibratingPoint = 3;//left top point is red
		}
		else if (i == 2)
		{
		    CalibratingPoint = 48;//right top point is red
		}
		else if (i == 3)
		{
		    CalibratingPoint = 12;//left bottom point is red
		}
		else if (i == 4)
		{
		    CalibratingPoint = 192;//right bottom point is red
		}
		else if (i == 5)
		{
		    CalibratingPoint = 768;//center point is red
		}
        
        //change pointer
        setTagValue(SYSTEM_TOUCHSCREEN_TAG, CalibratingPoint);
        
		while((AHMITouch_CaliGetXY(&pt[i-1]) == 0));
		for(delay1 = 0; delay1 < 100; delay1++)
			for(delay2 = 0; delay2 < 36000;)
				delay2++;
		
	}
	
	
	//calculate x difference 
	mx1 = (int)pt[1].pos_x - (int)pt[0].pos_x;
	mx2 = (int)pt[3].pos_x - (int)pt[2].pos_x;
	
	//calculate y difference 
	my1 = (int)pt[0].pos_y - (int)pt[2].pos_y;
	my2 = (int)pt[1].pos_y - (int)pt[3].pos_y;
	
	//absolute mx1 and mx2
	mx1 = mx1 > 0 ? mx1 : (-mx1);
	mx2 = mx2 > 0 ? mx2 : (-mx2);
	
	//absolute my1 and my2
	my1 = my1 > 0 ? my1 : (-my1);
	my2 = my2 > 0 ? my2 : (-my2);
	
	//calculate difference 
	mx3 = mx1 - mx2;
	my3 = my1 - my2;
	
	//absolute mx3 and my3
	mx3 = mx3 > 0 ? mx3 : (-mx3);
	my3 = my3 > 0 ? my3 : (-my3);
	
	if((mx3 <= 256) && (my3 <= 128))
	{
		//calculate calibrating parameters below
		k = (pt[0].pos_x - pt[3].pos_x) * (pt[1].pos_y - pt[3].pos_y) - (pt[1].pos_x - pt[3].pos_x) * (pt[0].pos_y - pt[3].pos_y);
		temp_screen_a = ((ps[0].pos_x - ps[3].pos_x) * (pt[1].pos_y - pt[3].pos_y) / k) - ((ps[1].pos_x - ps[3].pos_x) * (pt[0].pos_y - pt[3].pos_y) / k);
		temp_screen_b = ((pt[0].pos_x - pt[3].pos_x) * (ps[1].pos_x - ps[3].pos_x) / k) - ((ps[0].pos_x - ps[3].pos_x) * (pt[1].pos_x - pt[3].pos_x) / k);
		temp_screen_c = pt[0].pos_y * ((pt[3].pos_x * ps[1].pos_x - pt[1].pos_x * ps[3].pos_x) / k) + pt[1].pos_y * ((pt[0].pos_x * ps[3].pos_x - pt[3].pos_x * ps[0].pos_x) / k) + pt[3].pos_y * ((pt[1].pos_x * ps[0].pos_x - pt[0].pos_x * ps[1].pos_x) / k);
		temp_screen_d = ((ps[0].pos_y - ps[3].pos_y) * (pt[1].pos_y - pt[3].pos_y) / k) - ((ps[1].pos_y - ps[3].pos_y) * (pt[0].pos_y - pt[3].pos_y) / k); 
		temp_screen_e = ((pt[0].pos_x - pt[3].pos_x) * (ps[1].pos_y - ps[3].pos_y) / k) - ((ps[0].pos_y - ps[3].pos_y) * (pt[1].pos_x - pt[3].pos_x) / k);
		temp_screen_f = pt[0].pos_y * ((pt[3].pos_x * ps[1].pos_y - pt[1].pos_x * ps[3].pos_y) / k) + pt[1].pos_y * ((pt[0].pos_x * ps[3].pos_y - pt[3].pos_x * ps[0].pos_y) / k) + pt[3].pos_y * ((pt[1].pos_x * ps[0].pos_y - pt[0].pos_x * ps[1].pos_y) / k);
		
		temp_x = (uint16_t)(temp_screen_a * pt[4].pos_x + temp_screen_b * pt[4].pos_y + temp_screen_c);
		temp_y = (uint16_t)(temp_screen_d * pt[4].pos_x + temp_screen_e * pt[4].pos_y + temp_screen_f);
		
		mx1 = temp_x - ps[4].pos_x;
		my1 = temp_y - ps[4].pos_y;
		
		mx1 = mx1 > 0 ? mx1 : (-mx1);
		my1 = my1 > 0 ? my1 : (-my1);
		//calculate calibrating parameters above
		
		if((mx1 <= 10) && (my1 <= 10))
		{
			screen_a = (int)(temp_screen_a * 0x10000);
			screen_b = (int)(temp_screen_b * 0x10000);
			screen_c = (int)(temp_screen_c + 0.5);
			screen_d = (int)(temp_screen_d * 0x10000);
			screen_e = (int)(temp_screen_e * 0x10000);
			screen_f = (int)(temp_screen_f + 0.5);
			flag_Calibration = 1;
			while(EraseFlashForTouch() != 1);
			while(ProgramFlashForTouchByUI5() != 1);//store parameters to stm flash	
			
			CalibratingPoint = 3072;//all points are black and display calibration is successful(tag bit11)

            setTagValue(SYSTEM_TOUCHSCREEN_TAG, CalibratingPoint);            
			for(delay1 = 0; delay1 < 200; delay1++)
				for(delay2 = 0; delay2 < 36000;)
					delay2++;
			
			calibration_ing = 0;
			return 1;
		}
	}
	CalibratingPoint = 682+1024;//all points are black and display calibration is fail,try again(tag bit10)

    setTagValue(SYSTEM_TOUCHSCREEN_TAG, CalibratingPoint);	
	for(delay1 = 0; delay1 < 200; delay1++)
		for(delay2 = 0; delay2 < 36000;)
			delay2++;

	return 0;
}

/*******************************************************************************
* Function Name  : AHMITouch_CalibrationByUI
* Description    : calibration function by UI,if calibrate success ,then store calibrating parameter to code flash
* Output         : None
* Return         : status infor
*******************************************************************************/
uint16_t AHMITouch_CalibrationByUI13(void)
{
	Position pt[13] = {{0}};//array to store touch location
	//five calibrating locations
	Position ps[13] = { {50, 30, NOTHING},                 {SCREEN_WIDTH / 2, 30, NOTHING},                 {SCREEN_WIDTH - 50, 30, NOTHING},
                        {50, SCREEN_HEIGHT / 2, NOTHING},  {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, NOTHING},  {SCREEN_WIDTH - 50, SCREEN_HEIGHT / 2, NOTHING},
                        {50, SCREEN_HEIGHT - 30, NOTHING}, {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, NOTHING}, {SCREEN_WIDTH - 50, SCREEN_HEIGHT - 30, NOTHING},
                        {SCREEN_WIDTH / 4 + 25, SCREEN_HEIGHT / 4 + 15, NOTHING},
                        {SCREEN_WIDTH * 3 / 4 - 25, SCREEN_HEIGHT / 4 + 15, NOTHING},
                        {SCREEN_WIDTH / 4 + 25, SCREEN_HEIGHT * 3 / 4 - 15, NOTHING},
                        {SCREEN_WIDTH * 3 / 4 - 25, SCREEN_HEIGHT * 3 / 4 - 15, NOTHING}};

	float k;
	//temple calibrating parameters
	float temp_screen_a = 0, temp_screen_b = 0, temp_screen_c = 0, temp_screen_d = 0, temp_screen_e = 0, temp_screen_f = 0;
	int mx1 = 0, mx2 = 0, mx3 = 0;
	int my1 = 0, my2 = 0, my3 = 0;
	int delay1, delay2;//delay counter
	
	uint32_t CalibratingPoint=0;
                        
    uint16_t temp_x = 0, temp_y = 0;
    
    uint8_t index_tl = 0;
    uint8_t index_tr = 0;
    uint8_t index_bl = 0;
    uint8_t index_br = 0;
    uint8_t index_center = 0;
    
    uint8_t i = 0;
	
	calibration_ing = 1;
						
//	//if SCREEN_WIDTH less than 481,modify calibrating locations
//	if(SCREEN_WIDTH < 481)
//	{
//		ps[0].pos_x = 50;
//		ps[0].pos_y = 30;
//		
//		ps[1].pos_x = SCREEN_WIDTH - 50;
//		ps[1].pos_y = 30;
//		
//		ps[2].pos_x = 50;
//		ps[2].pos_y = SCREEN_HEIGHT - 30;
//		
//		ps[3].pos_x = SCREEN_WIDTH - 50;
//		ps[3].pos_y = SCREEN_HEIGHT - 30;
//		
//		ps[4].pos_x = SCREEN_WIDTH / 2 - POINTERSIZE / 2;
//		ps[4].pos_y = SCREEN_HEIGHT / 2 - POINTERSIZE / 2;
//	}
    
    for(i = 0; i < 13; i++)
    {
        AHMITouch_PositionRotate(&ps[i], &TouchRatitonInversematirx);
    }
    
	/*left top red point is bit0,black point is bit1; right top red point is bit2,black piont is bit3;left bottom red point is bit6,black piont is bit7;right bottom red point is bit4,black piont is bit5;center red point is bit8,black piont is bit9;*/
	for(i = 1; i < 14; i++)
	{
		if (i == 1)
		{
			CalibratingPoint = 0x3;//left top point is red
		}
		else if (i == 2)
		{
		    CalibratingPoint = 0xc;//right top point is red
		}
		else if (i == 3)
		{
		    CalibratingPoint = 0x30;//left bottom point is red
		}
		else if (i == 4)
		{
		    CalibratingPoint = 0xc0;//right bottom point is red
		}
		else if (i == 5)
		{
		    CalibratingPoint = 0x300;//center point is red
		}
        else if (i == 6)
		{
		    CalibratingPoint = 0xc00;//center point is red
		}
        else if (i == 7)
		{
		    CalibratingPoint = 0x3000;//center point is red
		}
        else if (i == 8)
		{
		    CalibratingPoint = 0xc000;//center point is red
		}
        else if (i == 9)
		{
		    CalibratingPoint = 0x30000;//center point is red
		}
        else if (i == 10)
		{
		    CalibratingPoint = 0xc0000;//center point is red
		}
        else if (i == 11)
		{
		    CalibratingPoint = 0xC00000;//center point is red
		}
        else if (i == 12)
		{
		    CalibratingPoint = 0x300000;//center point is red
		}
        else if (i == 13)
		{
		    CalibratingPoint = 0x3000000;//center point is red
		}
        
        //change pointer
        setTagValue(SYSTEM_TOUCHSCREEN_TAG, CalibratingPoint);
        
		while((AHMITouch_CaliGetXY(&pt[i-1]) == 0)){};
		for(delay1 = 0; delay1 < 100; delay1++)
			for(delay2 = 0; delay2 < 36000;)
				delay2++;
        
        while((AHMITouch_CaliGetXY(&pt[i-1]) == 0)){};
		for(delay1 = 0; delay1 < 100; delay1++)
			for(delay2 = 0; delay2 < 36000;)
				delay2++;
		
	}
    
    for(i = 0; i < 5; i++)
    {
        if(i == 0)
        {
            index_tl = 9;
            index_tr = 10;
            index_bl = 11;
            index_br = 12;
            index_center = 4;
        }
        else if(i == 1)
        {
            index_tl = 0;
            index_tr = 1;
            index_bl = 3;
            index_br = 4;
            index_center = 9;
        }
        else if(i == 2)
        {
            index_tl = 1;
            index_tr = 2;
            index_bl = 4;
            index_br = 5;
            index_center = 10;
        }
        else if(i == 3)
        {
            index_tl = 3;
            index_tr = 4;
            index_bl = 6;
            index_br = 7;
            index_center = 11;
        }
        else if(i == 4)
        {
            index_tl = 4;
            index_tr = 5;
            index_bl = 7;
            index_br = 8;
            index_center = 12;
        }
        
        //calculate x difference 
        mx1 = (int)pt[index_tr].pos_x - (int)pt[index_tl].pos_x;
        mx2 = (int)pt[index_br].pos_x - (int)pt[index_bl].pos_x;
        
        //calculate y difference 
        my1 = (int)pt[index_tl].pos_y - (int)pt[index_bl].pos_y;
        my2 = (int)pt[index_tr].pos_y - (int)pt[index_br].pos_y;
        
        //absolute mx1 and mx2
        mx1 = mx1 > 0 ? mx1 : (-mx1);
        mx2 = mx2 > 0 ? mx2 : (-mx2);
        
        //absolute my1 and my2
        my1 = my1 > 0 ? my1 : (-my1);
        my2 = my2 > 0 ? my2 : (-my2);
        
        //calculate difference 
        mx3 = mx1 - mx2;
        my3 = my1 - my2;
        
        //absolute mx3 and my3
        mx3 = mx3 > 0 ? mx3 : (-mx3);
        my3 = my3 > 0 ? my3 : (-my3);
        
//        if(i == 1)
//        {
//            index_tl = index_bl;
//        }
//        else if(i == 2)
//        {
//            index_br = index_bl;
//        }
//        else if(i == 4)
//        {
//            index_br = index_bl;
//        }

        if(i == 1)
        {
            index_br = index_bl;
        }
        else if(i == 3)
        {
            index_tr = index_bl;
        }
        else if(i == 4)
        {
            index_tl = index_bl;
        }
        
//      index_tl : 0;
//      index_tr : 1;
//      index_bl : 2;
//      index_br : 3;
//      index_center : 4;
        
        if((mx3 <= 128) && (my3 <= 128))
        {
            //calculate calibrating parameters below
            k = (pt[index_tl].pos_x - pt[index_br].pos_x) * (pt[index_tr].pos_y - pt[index_br].pos_y) - (pt[index_tr].pos_x - pt[index_br].pos_x) * (pt[index_tl].pos_y - pt[index_br].pos_y);
            temp_screen_a = ((ps[index_tl].pos_x - ps[index_br].pos_x) * (pt[index_tr].pos_y - pt[index_br].pos_y) / k) - ((ps[index_tr].pos_x - ps[index_br].pos_x) * (pt[index_tl].pos_y - pt[index_br].pos_y) / k);
            temp_screen_b = ((pt[index_tl].pos_x - pt[index_br].pos_x) * (ps[index_tr].pos_x - ps[index_br].pos_x) / k) - ((ps[index_tl].pos_x - ps[index_br].pos_x) * (pt[index_tr].pos_x - pt[index_br].pos_x) / k);
            temp_screen_c = pt[index_tl].pos_y * ((pt[index_br].pos_x * ps[index_tr].pos_x - pt[index_tr].pos_x * ps[index_br].pos_x) / k) + pt[index_tr].pos_y * ((pt[index_tl].pos_x * ps[index_br].pos_x - pt[index_br].pos_x * ps[index_tl].pos_x) / k) + 
                            pt[index_br].pos_y * ((pt[index_tr].pos_x * ps[index_tl].pos_x - pt[index_tl].pos_x * ps[index_tr].pos_x) / k);
            temp_screen_d = ((ps[index_tl].pos_y - ps[index_br].pos_y) * (pt[index_tr].pos_y - pt[index_br].pos_y) / k) - ((ps[index_tr].pos_y - ps[index_br].pos_y) * (pt[index_tl].pos_y - pt[index_br].pos_y) / k); 
            temp_screen_e = ((pt[index_tl].pos_x - pt[index_br].pos_x) * (ps[index_tr].pos_y - ps[index_br].pos_y) / k) - ((ps[index_tl].pos_y - ps[index_br].pos_y) * (pt[index_tr].pos_x - pt[index_br].pos_x) / k);
            temp_screen_f = pt[index_tl].pos_y * ((pt[index_br].pos_x * ps[index_tr].pos_y - pt[index_tr].pos_x * ps[index_br].pos_y) / k) + pt[index_tr].pos_y * ((pt[index_tl].pos_x * ps[index_br].pos_y - pt[index_br].pos_x * ps[index_tl].pos_y) / k) + 
                            pt[index_br].pos_y * ((pt[index_tr].pos_x * ps[index_tl].pos_y - pt[index_tl].pos_x * ps[index_tr].pos_y) / k);
            
            temp_x = (uint16_t)(temp_screen_a * pt[index_center].pos_x + temp_screen_b * pt[index_center].pos_y + temp_screen_c);
            temp_y = (uint16_t)(temp_screen_d * pt[index_center].pos_x + temp_screen_e * pt[index_center].pos_y + temp_screen_f);
            
            mx1 = temp_x - ps[index_center].pos_x;
            my1 = temp_y - ps[index_center].pos_y;
            
            mx1 = mx1 > 0 ? mx1 : (-mx1);
            my1 = my1 > 0 ? my1 : (-my1);
            //calculate calibrating parameters above
            
            if((mx1 <= 5) && (my1 <= 5))
            {
                transpara[i].screen_a = (int)(temp_screen_a * 0x10000);
                transpara[i].screen_b = (int)(temp_screen_b * 0x10000);
                transpara[i].screen_c = (int)(temp_screen_c + 0.5);
                transpara[i].screen_d = (int)(temp_screen_d * 0x10000);
                transpara[i].screen_e = (int)(temp_screen_e * 0x10000);
                transpara[i].screen_f = (int)(temp_screen_f + 0.5);
            }
            else
            {
                
                CalibratingPoint = 0x2AAAAAA;//all points are black and display calibration is fail,try again(tag bit10)

                setTagValue(SYSTEM_TOUCHSCREEN_TAG, CalibratingPoint);	
                for(delay1 = 0; delay1 < 200; delay1++)
                    for(delay2 = 0; delay2 < 36000;)
                        delay2++;
                
                return 0;
            }
        }
        else
        {
            CalibratingPoint = 0x2AAAAAA;//all points are black and display calibration is fail,try again(tag bit10)

            setTagValue(SYSTEM_TOUCHSCREEN_TAG, CalibratingPoint);	
            for(delay1 = 0; delay1 < 200; delay1++)
                for(delay2 = 0; delay2 < 36000;)
                    delay2++;
            
            return 0;
        }
    
    }
    
    flag_Calibration = 1;
    while(EraseFlashForTouch() != 1);
    while(ProgramFlashForTouchByUI13() != 1);//store parameters to stm flash	
    
    CalibratingPoint = 0x8000000;//all points are black and display calibration is successful(tag bit27)
    
    setTagValue(SYSTEM_TOUCHSCREEN_TAG, CalibratingPoint);            
    for(delay1 = 0; delay1 < 200; delay1++)
	{
        for(delay2 = 0; delay2 < 36000;)
		{
            delay2++;
		}
	}
    
	calibration_ing = 0;
    return 1;
}

/*******************************************************************************
* Function Name  : AHMITouch_TouchToScreen
* Description    : conver reading data to screen loction,pixel unit
* Input          : -touchposition
*				 : -screenposition
* Output         : None
* Return         : None
*******************************************************************************/
void AHMITouch_TouchToScreenByUI5(Position* touchposition, Position* screenposition)
{
    int temp_screenx = 0, temp_screeny = 0;
    
	if((NULL == touchposition) || (NULL == screenposition))
		return;
    
    /* TOUCH SCREEN SUPPORT */
    if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENTYPEEND))
    {
        temp_screenx = ((screen_a * touchposition->pos_x + screen_b * touchposition->pos_y) >> 16) + screen_c;
        temp_screeny = ((screen_d * touchposition->pos_x + screen_e * touchposition->pos_y) >> 16) + screen_f;
        screenposition->pos_x = temp_screenx;
        screenposition->pos_y = temp_screeny;
    }
    
    AHMITouch_PositionRotate(screenposition, &TouchRatitonmatirx);
}

void AHMITouch_TouchToScreenByUI13(Position* touchposition, Position* screenposition)
{
    int temp_screenx = 0, temp_screeny = 0;
    int temp_screenx_0 = 0, temp_screeny_0 = 0;
    int index = 0;
    int diff_x = 0;
    int diff_y = 0;
    int half_width = 0;
    int half_height = 0;

    
	if((NULL == touchposition) || (NULL == screenposition))
		return;
    
    half_width = SCREEN_WIDTH / 2;
    half_height = SCREEN_HEIGHT / 2;
    
    /* TOUCH SCREEN SUPPORT */
    if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENTYPEEND))
    {
        temp_screenx_0 = ((transpara[0].screen_a * touchposition->pos_x + transpara[0].screen_b * touchposition->pos_y) >> 16) + transpara[0].screen_c;
        temp_screeny_0 = ((transpara[0].screen_d * touchposition->pos_x + transpara[0].screen_e * touchposition->pos_y) >> 16) + transpara[0].screen_f;
        
        if(temp_screenx_0 <= half_width)
        {
            diff_x = half_width - temp_screenx_0;
            
            if(temp_screeny_0 <= half_height)
            {
                diff_y = half_height - temp_screeny_0;
                index = 1;
            }
            else
            {
                diff_y = temp_screeny_0 - half_height;
                index = 2; 
            }
        }
        else
        {
            diff_x = temp_screenx_0 - half_width;
            
            if(temp_screeny_0 <= half_height)
            {
                diff_y = half_height - temp_screeny_0;
                index = 3;
            }
            else
            {
                diff_y = temp_screeny_0 - half_height;
                index = 4; 
            }
        }
        
        temp_screenx = ((transpara[index].screen_a * touchposition->pos_x + transpara[index].screen_b * touchposition->pos_y) >> 16) + transpara[index].screen_c;
        temp_screeny = ((transpara[index].screen_d * touchposition->pos_x + transpara[index].screen_e * touchposition->pos_y) >> 16) + transpara[index].screen_f;
        
        temp_screenx += 1;
        temp_screeny += 1;
        
//        temp_screenx_0 *= (half_width - diff_x);
//        temp_screenx *= diff_x;
//        screenposition->pos_x = (temp_screenx_0 + temp_screenx) / half_width;
//        
//        temp_screeny_0 *= (half_height - diff_y);
//        temp_screeny *= diff_y;
//        screenposition->pos_y = (temp_screeny_0 + temp_screeny) / half_height;
        
        diff_x *= diff_y;
        diff_x /= 128;
        
        diff_y = half_width * half_height / 128;
        
        temp_screenx_0 *= (diff_y - diff_x);
        temp_screenx *= diff_x;
        screenposition->pos_x = (temp_screenx_0 + temp_screenx) / diff_y;
        
        temp_screeny_0 *= (diff_y - diff_x);
        temp_screeny *= diff_x;
        screenposition->pos_y = (temp_screeny_0 + temp_screeny) / diff_y;
    }
    
    AHMITouch_PositionRotate(screenposition, &TouchRatitonmatirx);
}

/*******************************************************************************
* Function Name  : AHMITouch_vGetTouchInfo
* Description    : get touch location and conver to screen location , then store to g_TM_stTouchFsm
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AHMITouch_vGetTouchInfo(void)
{
	switch(TouchScreenType)
    {
        case TOUCHSCREENRESISTIVE:
			AW2083touch_vMain();
            break;
        case TOUCHSCREENRESISTIVE_ADC:
			GCADCtouch_vMain();
            break;
		case TOUCHSCREENCAPACITIVE:
			Capacitivetouch_vMain();
            break;
		case TOUCHSCREENSHMEM_RESISTIVE:
		case TOUCHSCREENSHMEM_CAPACITIVE:
			//SHMEMTouch_vMain();
            break;
        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : AHMITouch_GetTouchEvent
* Description    : get touch location and conver to screen location , then store to ScreenCurPosition
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if defined(EMBEDDED) && defined(VERSION_2)
uint32_t TouchCount = 0;
#endif
void AHMITouch_GetTouchEvent(Position* pScreenCurPosition, Position* pScreenContactPosition, Position* pScreenLeavePosition)
{
    static Position CurPosition = {0, 0, NOTOUCH}; 
    
    //get positon 
    AHMITouch_ReadPosition(&CurPosition);
    
    if(CurPosition.event == TOUCHED)
    {
        if((pScreenCurPosition->event == CLICK) || (pScreenCurPosition->event == HOLD))
        {
            #ifndef TOUCH_CALIBRATION_ByUI5	
			AHMITouch_TouchToScreenByUI13(&CurPosition, pScreenCurPosition);
			#else
			AHMITouch_TouchToScreenByUI5(&CurPosition, pScreenCurPosition);
			#endif
            pScreenCurPosition->event = HOLD;//hold event
        }
        else
        {
            #ifndef TOUCH_CALIBRATION_ByUI5	
			AHMITouch_TouchToScreenByUI13(&CurPosition, pScreenCurPosition);
			#else
			AHMITouch_TouchToScreenByUI5(&CurPosition, pScreenCurPosition);
			#endif
            pScreenCurPosition->event = CLICK;//click event
            pScreenContactPosition->pos_x = pScreenCurPosition->pos_x;
            pScreenContactPosition->pos_y = pScreenCurPosition->pos_y;
        }     
    }
    else
    {            
        if((pScreenCurPosition->event == NOTHING) || (pScreenCurPosition->event == RELEASE))
        {
            pScreenCurPosition->pos_x = 0;
            pScreenCurPosition->pos_y = 0;
            pScreenCurPosition->event = NOTHING;
        }
        else
        {
            pScreenLeavePosition->pos_x = pScreenCurPosition->pos_x;
            pScreenLeavePosition->pos_y = pScreenCurPosition->pos_y;
            pScreenCurPosition->pos_x = 0;
            pScreenCurPosition->pos_y = 0;
            pScreenCurPosition->event = RELEASE;
			if(TouchScreenType < TOUCHSCREENCAPACITIVE)
			{
				TouchCount++;
			}
        }     
    }
}

/*******************************************************************************
* Function Name  : EraseFlashForTouch
* Description    : erase gc9003 flash
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int EraseFlashForTouch(void)        
{ 
//	/*********for erase stm32 flash*********/
//	uint32_t EraseCounter = 0x00;
//	uint32_t NbrOfPage = 0x00;
//	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
//	
//	FLASH_Unlock();
//	
//	NbrOfPage = (BANK_WRITE_END_ADDR_FOR_TOUCH - BANK_WRITE_START_ADDR_FOR_TOUCH) / FLASH_PAGE_SIZE_FOR_TOUCH;
//	
//	for(EraseCounter = 0; (EraseCounter < NbrOfPage)&&(FLASHStatus == FLASH_COMPLETE);EraseCounter++)
//	{
//		FLASHStatus = FLASH_ErasePage(BANK_WRITE_START_ADDR_FOR_TOUCH + (FLASH_PAGE_SIZE_FOR_TOUCH * EraseCounter));
//	}
//	
//	FLASH_ClearFlag(FLASH_FLAG_EOP);
//	
//	FLASH_Lock();
    
    csi_hal_internal_flash_ClearWriteProtect();
    csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_FOR_TOUCH);
    csi_hal_internal_flash_SetWriteProtect();
	
	return 1;
}

/*******************************************************************************
* Function Name  : ProgramFlashForTouch
* Description    : store calibrating parameters to gc9003 flash
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int ProgramFlashForTouchByUI5(void)
{
////	int temp[7] = {0};
//	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint32_t Address = BANK_WRITE_START_ADDR_FOR_TOUCH;
    
//	
//	FLASH_Unlock();//enter keyword
//	FLASHStatus = FLASH_ProgramWord(Address, screen_a);
//	Address += 4;
//	if(FLASHStatus == FLASH_COMPLETE)
//	{
//		FLASHStatus = FLASH_ProgramWord(Address, screen_b);
//		Address += 4;
//	}
//	if(FLASHStatus == FLASH_COMPLETE)
//	{
//		FLASHStatus = FLASH_ProgramWord(Address, screen_c);
//		Address += 4;
//	}
//	if(FLASHStatus == FLASH_COMPLETE)
//	{
//		FLASHStatus = FLASH_ProgramWord(Address, screen_d);
//		Address += 4;
//	}
//	if(FLASHStatus == FLASH_COMPLETE)
//	{
//		FLASHStatus = FLASH_ProgramWord(Address, screen_e);
//		Address += 4;
//	}
//	if(FLASHStatus == FLASH_COMPLETE)
//	{
//		FLASHStatus = FLASH_ProgramWord(Address, screen_f);
//		Address += 4;
//	}
//	if(FLASHStatus == FLASH_COMPLETE)
//	{
//		FLASHStatus = FLASH_ProgramWord(Address, flag_Calibration);
//	}
//	
//	FLASH_ClearFlag(FLASH_FLAG_EOP);
//	FLASH_Lock();

    csi_hal_internal_flash_ClearWriteProtect();
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&screen_a);
    Address += 4;
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&screen_b);
    Address += 4;
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&screen_c);
    Address += 4;
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&screen_d);
    Address += 4;
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&screen_e);
    Address += 4;
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&screen_f);
    Address += 4;
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&flag_Calibration);
	csi_hal_internal_flash_SetWriteProtect();
    
	return 1;
}


int ProgramFlashForTouchByUI13(void)
{
	uint32_t Address = BANK_WRITE_START_ADDR_FOR_TOUCH;
    uint32_t i = 0;
    
    csi_hal_internal_flash_ClearWriteProtect();
    for(i = 0; i < 5; i++)
    {
        csi_hal_internal_flash_write(Address, 4, (uint8_t *)&(transpara[i].screen_a));
        Address += 4;
        csi_hal_internal_flash_write(Address, 4, (uint8_t *)&(transpara[i].screen_b));
        Address += 4;
        csi_hal_internal_flash_write(Address, 4, (uint8_t *)&(transpara[i].screen_c));
        Address += 4;
        csi_hal_internal_flash_write(Address, 4, (uint8_t *)&(transpara[i].screen_d));
        Address += 4;
        csi_hal_internal_flash_write(Address, 4, (uint8_t *)&(transpara[i].screen_e));
        Address += 4;
        csi_hal_internal_flash_write(Address, 4, (uint8_t *)&(transpara[i].screen_f));
        Address += 4;
    }
    
    csi_hal_internal_flash_write(Address, 4, (uint8_t *)&flag_Calibration);
	csi_hal_internal_flash_SetWriteProtect();
    
	return 1;
}

/*******************************************************************************
* Function Name  : ReadDataFromFlashForTouch
* Description    : read calibrating parameters from gc9003 flash
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ReadDataFromFlashForTouchByUI5(void) 
{
	uint32_t Address  = BANK_WRITE_START_ADDR_FOR_TOUCH;
	
    if(TouchScreenType < TOUCHSCREENCAPACITIVE)
    {
        screen_a = *(__IO int *) Address;
        Address += 4;
        screen_b = *(__IO int *) Address;
        Address += 4;
        screen_c = *(__IO int *) Address;
        Address += 4;
        screen_d = *(__IO int *) Address;
        Address += 4;
        screen_e = *(__IO int *) Address;
        Address += 4;
        screen_f = *(__IO int *) Address;
        Address += 4;
        flag_Calibration = *(__IO int *) Address;
		if(flag_Calibration != 1)
		{
			flag_Calibration = 0;
			calibration_ing = 1;
		}
    }
    else
    {
        screen_a = 1 << 16;
        screen_b = 0;
        screen_c = 0;
        screen_d = 0;
        screen_e = 1 << 16;
        screen_f = 0;
        flag_Calibration = 1;
    }
}

void ReadDataFromFlashForTouchByUI13(void) 
{
	uint32_t Address  = BANK_WRITE_START_ADDR_FOR_TOUCH;
    uint32_t i = 0;
	
    if(TouchScreenType < TOUCHSCREENCAPACITIVE)
    {
        for(i = 0; i < 5; i++)
        {
            transpara[i].screen_a = *(__IO int *) Address;
            Address += 4;
            transpara[i].screen_b = *(__IO int *) Address;
            Address += 4;
            transpara[i].screen_c = *(__IO int *) Address;
            Address += 4;
            transpara[i].screen_d = *(__IO int *) Address;
            Address += 4;
            transpara[i].screen_e = *(__IO int *) Address;
            Address += 4;
            transpara[i].screen_f = *(__IO int *) Address;
            Address += 4;
        }
        
        flag_Calibration = *(__IO int *) Address;
		if(flag_Calibration != 1)
		{
			flag_Calibration = 0;
			calibration_ing = 1;
		}
    }
    else
    {
        for(i = 0; i < 5; i++)
        {
            transpara[i].screen_a = 1 << 16;
            transpara[i].screen_b = 0;
            transpara[i].screen_c = 0;
            transpara[i].screen_d = 0;
            transpara[i].screen_e = 1 << 16;
            transpara[i].screen_f = 0;
        }
        
        flag_Calibration = 1;
    }
}

extern DynamicPageClassPtr gPagePtr;
extern u16 TouchCalibration_PageID;
extern u16 Debugging_PageID;
extern u16 WorkingPageID;
extern u32 AHMITick;
extern u8 windowPressed;

void CheckWhetherEnterDebuggingPageAfterStartup(Position* pScreenCurPosition)
{
	static uint32_t boot_touch_keep_press_time_count = 0;
    static uint32_t boot_touch_keep_press_nothing_count = 0;
    static uint32_t keep_press_time_cnt = 0xFFFFFFFF;
    static uint32_t keep_press_tick;
	
	if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENCAPACITIVE))
	{
		if (boot_touch_keep_press_time_count != 0xFFFFFFFF)
		{
			if (pScreenCurPosition->event == NOTHING)
			{
				if (boot_touch_keep_press_nothing_count != AHMITick)
				{
					boot_touch_keep_press_nothing_count = AHMITick;
					if (boot_touch_keep_press_nothing_count >= 100)
					{
						boot_touch_keep_press_time_count = 0xFFFFFFFF;
					}
				}
			}
			else if (pScreenCurPosition->event == RELEASE)
			{
				boot_touch_keep_press_time_count = 0xFFFFFFFF;
			}
			else if (boot_touch_keep_press_time_count != AHMITick)
			{
				boot_touch_keep_press_time_count = AHMITick;
				if (boot_touch_keep_press_time_count >= 5000)
				{
					boot_touch_keep_press_time_count = 0xFFFFFFFF;
					keep_press_time_cnt = 0;
					keep_press_tick = 0xFFFFFFFF;
					windowPressed = 0;
					//gPagePtr[WorkingPageID].loadPage(Debugging_PageID, 1);
					TagPtr[SYSTEM_PAGE_TAG].setValue(Debugging_PageID + 1, SYSTEM_PAGE_TAG);
				}
			}
		}
	
		if (keep_press_time_cnt != 0xFFFFFFFF)
		{
			if (keep_press_tick != 0xFFFFFFFF)
			{
				if (pScreenCurPosition->event == HOLD)
				{
					if (AHMITick - keep_press_tick >= 5000)
					{
						keep_press_time_cnt = 0xFFFFFFFF;
						windowPressed = 0;
						//gPagePtr[WorkingPageID].loadPage(TouchCalibration_PageID, 1);
						TagPtr[SYSTEM_PAGE_TAG].setValue(TouchCalibration_PageID + 1, SYSTEM_PAGE_TAG);
					}
				}
				else
				{
					keep_press_tick = AHMITick;
				}
			}
			else
			{
				if ((pScreenCurPosition->event == NOTHING) || (pScreenCurPosition->event == RELEASE))
				{
					if (AHMITick == 0xFFFFFFFF)
					{
						keep_press_tick = 0;
					}
					else
					{
						keep_press_tick = AHMITick;
					}
				}
			}
		}
	}
}
