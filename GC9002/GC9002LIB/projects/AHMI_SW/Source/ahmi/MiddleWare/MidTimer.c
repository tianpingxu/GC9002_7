////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI Groop
//
// Create Date:   2017/07/06
// File Name:     Trace.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2017/07/12 by Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "MidTimer.h"
#include "tagClass_cd.h"
#include "drawImmediately_cd.h"
#include "gc90x_initial.h"
//#include "gc90x_wdg2.h" 
#include "ITM1c1.h"
//#include "gc90x_shmemmanage.h"
#include "AHMIInterface.h"
#if defined(CHIP_GC9002)
#include "SBBatchDraw.h"
#include "csi_ahmi_hw.h"
#include "gc90x_display.h"
#endif
AHMITimer  AHMITIM[NUMOFAHMITIM] = {0};
//uint32_t AHMITick = 0;

AHMITimerHandle hTimer0 = 
{
	AHMITIM0,
	1000,
	TIMSINGLE
};

AHMITimerHandle hTimer1 = 
{
	AHMITIM1,
	10,
	TIMSINGLE
};

//AHMITimerHandle hTimer2 = 
//{
//	AHMITIM2,
//	10,
//	TIMCYCLE
//};

//AHMITimerHandle hTimer3 = 
//{
//	AHMITIM3,
//	50000,
//	TIMCYCLE
//};

//AHMITimerHandle hTimer4 = 
//{
//	AHMITIM4,
//	40000,
//	TIMCYCLE
//};

//AHMITimerHandle hTimer5 = 
//{
//	AHMITIM5,
//	30000,
//	TIMCYCLE
//};

//AHMITimerHandle hTimer6 = 
//{
//	AHMITIM6,
//	20000,
//	TIMCYCLE
//};

//AHMITimerHandle hTimer7 = 
//{
//	AHMITIM7,
//	10000,
//	TIMCYCLE
//};

const AHMITimerCB AHMITIMCB[NUMOFAHMITIM] = 
{
	AHMITIMCB0,
	AHMITIMCB1,
	AHMITIMCB2,
	AHMITIMCB3,
	AHMITIMCB4,
	AHMITIMCB5,
	AHMITIMCB6,
	AHMITIMCB7
};

//-----------------------------
// function: StartAHMITimer
// @brief: Start AHMITimer.
// parameters:
// @param1:hTimer   pointer to a AHMITimer
//-----------------------------
void StartAHMITimer(AHMITimerHandle* hTimer)
{
	uint32_t irq;
	
	if((hTimer == 0) || (hTimer->mTimerX == 0))
		return;
		
	irq = csi_irq_save();
	hTimer->mTimerX->mCurValue = 0;
	hTimer->mTimerX->mStatus = TIMENABLE;
	csi_irq_restore(irq);
}

//-----------------------------
// function: StopAHMITimer
// @brief: Stop AHMITimer.
// parameters:
// @param1:hTimer   pointer to a AHMITimer
//-----------------------------
void StopAHMITimer(AHMITimerHandle* hTimer)
{
	uint32_t irq;
	if((hTimer == 0) || (hTimer->mTimerX == 0))
		return;
	irq = csi_irq_save();
	hTimer->mTimerX->mStatus = TIMDISNABLE;
	csi_irq_restore(irq);
}

//-----------------------------
// function: InitAHMITimer
// @brief: Init AHMITimer.
// parameters:
// @param1:hTimer   pointer to a AHMITimer config structure
//-----------------------------
void InitAHMITimer(AHMITimerHandle* hTimer)
{
	uint32_t irq;
	if((hTimer == 0) || (hTimer->mTimerX == 0))
		return;
	
	irq = csi_irq_save();
	StopAHMITimer(hTimer);
	hTimer->mTimerX->mStopValue = hTimer->mStopValue;
	hTimer->mTimerX->mMode = hTimer->mMode;
	hTimer->mTimerX->mCurValue = 0;
	csi_irq_restore(irq);
}

//void AHMITIMCB0(void)
//{
////	AHMIPrintf("0.\r\n");
//}

//void AHMITIMCB1(void)
//{
////	AHMIPrintf("1.\r\n");
//}

void AHMITIMCB2(void)
{
//	AHMIPrintf("2.\r\n");
}

void AHMITIMCB3(void)
{
//	AHMIPrintf("3.\r\n");
}

//void AHMITIMCB4(void)
//{
////	AHMIPrintf("4.\r\n");
//}

void AHMITIMCB5(void)
{
//	AHMIPrintf("5.\r\n");
}

void AHMITIMCB6(void)
{
//	AHMIPrintf("6.\r\n");
}

void AHMITIMCB7(void)
{
//	AHMIPrintf("7\r\n");
}

//-----------------------------
// function: AHMITimerHandlerISR
// @brief: Entry for all AHMITimer.
//-----------------------------
void AHMITimerHandlerISR(void)
{
	uint8_t i;
	
	
	for(i = 0; i < NUMOFAHMITIM; i++)
	{
		if(AHMITIM[i].mStatus == TIMENABLE)
		{
			if(AHMITIM[i].mCurValue <= AHMITIM[i].mStopValue)
			{
				AHMITIM[i].mCurValue++;
			}
			else
			{
				if(AHMITIM[i].mMode == TIMCYCLE)
				{
					AHMITIM[i].mCurValue = 0;
					AHMITIMCB[i]();
				}
				else if(AHMITIM[i].mMode == TIMSINGLE)
				{
					AHMITIM[i].mCurValue = 0;
					AHMITIM[i].mStatus = TIMDISNABLE;
					AHMITIMCB[i]();
				}
			}
		}
	}
}



////-----------------------------
//// function: xTimerInit
//// @brief: Init hardware timer, only support TIM2, TIM3, TIM4, TIM5, TIM6, TIM7.
//// parameters:
//// @param1:hTimer   pointer to a hardware Timer config structure
////-----------------------------
//void xTimerInit(TIMHandle* hTimer)
//{
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	
//	
//	if((0 == hTimer) || (0 == hTimer->mTIMx))
//		return;
//	
//    /* Enable the clock for TIMx . */
//	if(hTimer->mTIMx == TIM2)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	}
//	else if(hTimer->mTIMx == TIM3)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	}
//	else if(hTimer->mTIMx == TIM4)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	}
//	else if(hTimer->mTIMx == TIM5)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//	}
//	else if(hTimer->mTIMx == TIM6)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//	}
//	else if(hTimer->mTIMx == TIM7)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
//	}	
//	else
//		return;
//	
//	/* Configure the NVIC Preemption Priority Bits */  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	/* Set interruot source. */	
//	if(hTimer->mTIMx == TIM2)
//	{
//		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	}
//	else if(hTimer->mTIMx == TIM3)
//	{
//		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	}
//	else if(hTimer->mTIMx == TIM4)
//	{
//		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	}
//	else if(hTimer->mTIMx == TIM5)
//	{
//		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	}
//	else if(hTimer->mTIMx == TIM6)
//	{
//		NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
//	}
//	else if(hTimer->mTIMx == TIM7)
//	{
//		NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
//	}	
//	
//	/* Set interrupt parameters.*/
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = hTimer->mInit.mTimerPrePriority;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = hTimer->mInit.mTimerSubPriority;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//	
//	
//    /* Set timer parameters.*/
//    TIM_ITConfig(hTimer->mTIMx, TIM_IT_Update,DISABLE);
//    TIM_Cmd(hTimer->mTIMx, DISABLE);

//    TIM_TimeBaseStructure.TIM_Period = hTimer->mInit.mPeriod;    	
//    TIM_TimeBaseStructure.TIM_Prescaler = hTimer->mInit.mPrescaler;
//    TIM_TimeBaseStructure.TIM_ClockDivision = hTimer->mInit.mClockDivision;	
//    TIM_TimeBaseStructure.TIM_CounterMode = hTimer->mInit.mCounterMode;
//    TIM_TimeBaseInit(hTimer->mTIMx, &TIM_TimeBaseStructure);
//	
//    TIM_ClearFlag(hTimer->mTIMx, TIM_FLAG_Update);
//	
//    /* Clear it's counter. */
//    TIM_SetCounter(hTimer->mTIMx,0x00);
//    TIM_ITConfig(hTimer->mTIMx,TIM_IT_Update,ENABLE);
//    TIM_Cmd(hTimer->mTIMx,ENABLE);
//}

//-----------------------------
// function: xTimersEnable
// @brief: Enable a hardware timer.
// parameters:
// @param1:hTimer   pointer to a hardware Timer config structure
//-----------------------------
//void xTimersEnable(TIMHandle* hTimer)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	
//    TIM_ITConfig(hTimer->mTIMx, TIM_IT_Update,DISABLE);
//    TIM_Cmd(hTimer->mTIMx, DISABLE);

//    TIM_TimeBaseStructure.TIM_Period = hTimer->mInit.mPeriod;    
//    TIM_TimeBaseStructure.TIM_Prescaler = hTimer->mInit.mPrescaler;	
//    TIM_TimeBaseStructure.TIM_ClockDivision = hTimer->mInit.mClockDivision;
//    TIM_TimeBaseStructure.TIM_CounterMode = hTimer->mInit.mCounterMode;	
//    TIM_TimeBaseInit(hTimer->mTIMx, &TIM_TimeBaseStructure);
//	
//    TIM_ClearFlag(hTimer->mTIMx, TIM_FLAG_Update);
//	
//    TIM_SetCounter(hTimer->mTIMx,0x00);			
//    TIM_ITConfig(hTimer->mTIMx,TIM_IT_Update,ENABLE);
//    TIM_Cmd(hTimer->mTIMx,ENABLE);
//}

//-----------------------------
// function: xTimersDisable
// @brief: Disable a hardware timer.
// parameters:
// @param1:hTimer   pointer to a hardware Timer config structure
//-----------------------------
//void xTimersDisable(TIMHandle* hTimer)
//{
//    TIM_ITConfig(hTimer->mTIMx, TIM_IT_Update,DISABLE);
//    TIM_Cmd(hTimer->mTIMx, DISABLE);
//}

/**
  * @brief  This function handles TIM2 Handler.
  * @param  None
  * @retval None
  */
//void TIM2_IRQHandler(void)
//{
//    TIM_ClearFlag(TIM2 , TIM_FLAG_Update);
//    AHMITimerHandlerISR();
//}
extern uint32_t TouchScreenType;
extern uint32_t sourceshift;
uint32_t AHMITick;
uint32_t TouchCounter_Tick;
extern uint32_t TouchCount;
extern u16 TouchCalibration_PageID;
extern void SetTagValueC(unsigned int ID, unsigned int value);

uint16_t BuzzerCounter = 0;
uint16_t lastTouchCount = 0;
uint8_t prepareGotoTouchCalibrationFlag = 0;


extern uint32_t ADCChannelTest(void);
/* Timer1, 500us called once */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
void MidTimer_IRQHandler(void)
{
    static uint32_t u32Counter = 0;
	
	uint32_t i,k;
    
	/* WDG2 feed dog */
	//WDG2_ReloadCounter();

    u32Counter++;

    
#if(ITM_COMPILE_SWITCH == ON)
    /* 500us called once*/
    ITM_vIsrProc();
#endif
    
    if(0 == (u32Counter % 2))
    {
        AHMITick += 1;
        //judge whether texture data exceed sourcebuffer size
        #ifndef CHIP_GC9002_NoHyper
		SBBatchDraw_vMain();
		#endif
    }
    /* 10ms called once*/
    if(0 == (u32Counter % 20))
	{    	
    	//if TOUCH SCREEN SUPPORT, judge whether goto touch calibration by multi touch
    	if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENCAPACITIVE))
    	{
    		if(prepareGotoTouchCalibrationFlag != 1)
    		{
    			TouchCounter_Tick += 10;
    			
    			if(TouchCounter_Tick >= 2000)
    			{
    				TouchCount = 0;
    				TouchCounter_Tick = 0;
    			}
    			else
    			{
    				if(TouchCount > 10)
    				{
    					prepareGotoTouchCalibrationFlag = 1;
    					lastTouchCount = TouchCount;
    					TouchCounter_Tick = 0;
    					StartBuzzer();
    				}
    			}
    		}
    		else
    		{
    			BuzzerCounter += 10;
    			if((TouchCount - lastTouchCount) > 2)
    			{
    				StopBuzzer();
    				TouchCount = 0;
    				lastTouchCount = 0;
    				BuzzerCounter = 0;
    				prepareGotoTouchCalibrationFlag = 0;
    			}
    			
    			if(BuzzerCounter >= 2000)
    			{
    				StopBuzzer();
    				TouchCount = 0;
    				lastTouchCount = 0;
    				BuzzerCounter = 0;
    				prepareGotoTouchCalibrationFlag = 0;
    				SetTagValueC(SYSTEM_PAGE_TAG, TouchCalibration_PageID + 1);
    			}
    		}
    	}
    	

    	if((sourceshift >= SOURCE_BUFFER_SIZE)
			#ifdef CHIP_GC9002
			|| BatchDraw_u8JudgeSBPos()
			#endif
			)
    	{
			while(1)
			{
				csi_ahmi_SetBacklightBrightness(100);
				csi_ahmi_StartBacklightForFivePoint();
				for (k=0; k<3; k++)
				{
					DrawFivePointPage(
						SCREEN_WIDTH,
						SCREEN_HEIGHT,
						0xFFFFFFFF, //BackgroundColor:white
						16,
						//LeftTop position of LeftTop Point
						0, 
						0,
						0xFF0000FF, //Pointer0Color:blue
						//LeftTop position of RightTop Point
						SCREEN_WIDTH - 16, 
						0,
						0xFF0000FF, //Pointer1Color:blue
						//LeftTop position of LeftBottum Point
						0, 
						SCREEN_HEIGHT - 16,
						0xFF0000FF, //Pointer1Color:blue
						//LeftTop position of RightBottum Point
						SCREEN_WIDTH - 16, 
						SCREEN_HEIGHT - 16,
						0xFF0000FF, //Pointer1Color:blue
						//LeftTop position of Center Point
						SCREEN_WIDTH / 2, 
						SCREEN_HEIGHT / 2,
						0xFF0000FF //Pointer1Color:blue
						);
						for(i = 0; i < 720000; i++)
						{
							i++;
						}
				 }
			}
    	}
    	
        AHMITimerHandlerISR();

    	/* Timer function for share memory. */
        //SHMEM_Timer();	
	}
}

/*********************************************END OF FILE**********************/
