////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:     AHMI
//
// Create Date:   2015/11/17
// File Name:     main.c
// Project Name:  AHMISimulator
//
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.02 - add the modbus definition
// Additional Comments:
//    in 2.00, add the canvas initiation
//
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "publicInclude.h"
#include "AHMIInterface.h"
#include "writeTexture.h"

#include "gc90x_initial.h"
#include "gc90x_display.h"
#include "display_conf.h"
#include "texture_flash.h"
#include "gc90x_flash.h" 

#include "MidTimer.h"
#include "AHMITouch.h"
#include "TouchTask.h"
#include "tagclass.h"
#include "ParsePara.h"
#include "ITM1c1.h"
#include "csi_irq.h"
#include "csi_hal_driver_flash.h"
#include "csi_gpio.h"
#include "csi_pin.h"
#include "csi_pwm.h"
#include "csi_wdt.h"
#include "adcshare_c1.h"

extern ConfigInfoClass ConfigData;

extern AHMITimerHandle hTimer0;
extern AHMITimerHandle hTimer1;

void Timer0Init(void);
void Timer1Init(void);

extern uint32_t AHMIDone_fps;
extern uint32_t TouchScreenType;
extern uint32_t flag_Calibration;

uint32_t BacklightWaitOn = 1;
uint32_t BacklightNormalBrightness = 80;
uint32_t BacklightWaitBrightness = 10;
uint32_t BacklightWaitDuration = 1000;  //1000 * 10 ms

uint32_t BacklightPWMFrequence = BACKLIGHTPWMFRE;
uint32_t BacklightCurrentBrightness = 0;

uint32_t BuzzerStatus = 0;
uint32_t BuzzerFrequence = 3000;
uint32_t BuzzerDuty = 3499;
uint32_t BuzzerDutyDiv = 6999;
uint32_t BuzzerDuration = 10;       //10 * 10 ms
uint32_t TouchSound = 1;
uint32_t AHMIOutputRotation = DEGREE0;
uint32_t BacklightPWMEnable = PWMDisable;
uint32_t BuzzerPWMEnable = PWMDisable;
uint32_t ResolutionRatioType = RESRATIOTYPE0;  //for 16M SDRAM
uint32_t SystemOption = 0;  //for 16M SDRAM

uint8_t DisplayContrast = 0;
uint8_t DisplaySaturation = 0;
uint8_t DisplayBrightness = 0;
static bool g_bTouchNeedWork = FALSE;
static uint16_t g_bTouchTimerCounter = 0;
static uint16_t g_bADCShareTimerCounter = 0;


extern const char CPU1Version[32];
void SetProductInfo()
{
    TagPtr[SYSTEM_DEVICEID_TAG].setString((char *)(ConfigData.DeviceID), 0);
	
    /* Send share memory frame after rtos vTaskStartScheduler. */
    //ShareMemory_ReadCPU0StringInfoFrame(CPU0StringInfoSoftVersion, 64, 1, 0, ReadCPU0StringInfoResponseCB);
    
	setStrTagValue(SYSTEM_CPU1APPVERSION_TAG, (char*)CPU1Version);
	setStrTagValue(SYSTEM_CPU1BOOTVERSION_TAG, (char *)0x08000100);
}

void SetResolutionRatioType(uint32_t type)
{
    if(type < RESRATIOTYPEEND)
        ResolutionRatioType = type;
}

TIM_HandleTypeDef htim0 = {.Instance = TIM0};
TIM_HandleTypeDef htim1 = {.Instance = TIM1};


static csi_pwm_t g_pwm0,g_pwm1;
static uint8_t g_u8PWMChannel = 0;
#define BLOFF    csi_pwm_out_stop(&g_pwm0, g_u8PWMChannel);
#define BLON     csi_pwm_out_start(&g_pwm0, g_u8PWMChannel);

#define BUZZEROFF    csi_pwm_out_stop(&g_pwm1, g_u8PWMChannel);
#define BUZZERON     csi_pwm_out_start(&g_pwm1, g_u8PWMChannel);


void SetTouchNeedWork(void)
{
    g_bTouchNeedWork = TRUE;
    g_bTouchTimerCounter = 0;
}

bool GetIfTouchNeedWork(void)
{
    bool res = FALSE;
    if(g_bTouchNeedWork)
    {
        if(g_bTouchTimerCounter < TOUCH_TIMER_THRESHOLD)
        {
            g_bTouchTimerCounter++;
        }
        else
        {
            g_bTouchTimerCounter = 0;
            res = TRUE;
        }
    }
    
    return res;
}

bool GetIfADCShareNeedWork(void)
{
    bool res = FALSE;
    if(1)
    {
        if(g_bADCShareTimerCounter < ADCSHARE_TIMER_THRESHOLD)
        {
            g_bADCShareTimerCounter++;
        }
        else
        {
            g_bADCShareTimerCounter = 0;
            res = TRUE;
        }
    }
    
    return res;
}



void ConfigPWM(csi_pwm_t *pwm, uint32_t GPIOx, uint32_t GPIO_Pin, uint32_t Freq, uint32_t DutyCycle)
{
	switch(GPIOx)
	{
		case GPIOA_BASE:
			switch(GPIO_Pin)
			{
				case GPIO_PIN_0:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA0, PA0_MOTOR_PWM0);
					csi_pwm_init(pwm, 2);
				break;
				
				case GPIO_PIN_1:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA1, PA1_MOTOR_PWM1);
					csi_pwm_init(pwm, 3);
				break;
				
				case GPIO_PIN_2:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA2, PA2_MOTOR_PWM2);
					csi_pwm_init(pwm, 4);
				break;
				
				case GPIO_PIN_3:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA3, PA3_MOTOR_PWM3);
					csi_pwm_init(pwm, 5);
				break;
				
				case GPIO_PIN_4:
					g_u8PWMChannel = 1;
					csi_pin_set_mux(PA4, PA4_TIM1_1);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_5:
					g_u8PWMChannel = 2;
					csi_pin_set_mux(PA5, PA5_TIM1_2);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_6:
					g_u8PWMChannel = 3;
					csi_pin_set_mux(PA6, PA6_TIM1_3);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_7:
					g_u8PWMChannel = 4;
					csi_pin_set_mux(PA7, PA7_TIM1_4);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_8:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA8, PA8_MOTOR_PWM0);
					csi_pwm_init(pwm, 2);
				break;
				
				case GPIO_PIN_9:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA9, PA9_MOTOR_PWM1);
					csi_pwm_init(pwm, 3);
				break;
				
				case GPIO_PIN_10:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA10, PA10_MOTOR_PWM2);
					csi_pwm_init(pwm, 4);
				break;
				
				case GPIO_PIN_11:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA11, PA11_MOTOR_PWM3);
					csi_pwm_init(pwm, 5);
				break;
				
				case GPIO_PIN_12:
					g_u8PWMChannel = 1;
					csi_pin_set_mux(PA12, PA12_TIM1_1);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_13:
					g_u8PWMChannel = 2;
					csi_pin_set_mux(PA13, PA13_TIM1_2);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_14:
					g_u8PWMChannel = 3;
					csi_pin_set_mux(PA14, PA14_TIM1_3);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_15:
					g_u8PWMChannel = 4;
					csi_pin_set_mux(PA15, PA15_TIM1_4);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_16:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA16, PA16_MOTOR_PWM0);
					csi_pwm_init(pwm, 2);
				break;
				
				case GPIO_PIN_17:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA17, PA17_MOTOR_PWM1);
					csi_pwm_init(pwm, 3);
				break;
				
				case GPIO_PIN_18:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA18, PA18_MOTOR_PWM2);
					csi_pwm_init(pwm, 4);
				break;
				
				case GPIO_PIN_19:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PA19, PA19_MOTOR_PWM3);
					csi_pwm_init(pwm, 5);
				break;
				
				default:
				break;
			}
		break;
		
		case GPIOB_BASE:
			switch(GPIO_Pin)
			{
				case GPIO_PIN_0:
					g_u8PWMChannel = 1;
					csi_pin_set_mux(PB0, PB0_TIM1_1);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_1:
					g_u8PWMChannel = 2;
					csi_pin_set_mux(PB1, PB1_TIM1_2);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_2:
					g_u8PWMChannel = 3;
					csi_pin_set_mux(PB2, PB2_TIM1_3);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_3:
					g_u8PWMChannel = 4;
					csi_pin_set_mux(PB3, PB3_TIM1_4);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_4:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB4, PB4_MOTOR_PWM0);
					csi_pwm_init(pwm, 2);
				break;
				
				case GPIO_PIN_5:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB5, PB5_MOTOR_PWM1);
					csi_pwm_init(pwm, 3);
				break;
				
				case GPIO_PIN_6:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB6, PB6_MOTOR_PWM2);
					csi_pwm_init(pwm, 4);
				break;
				
				case GPIO_PIN_7:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB7, PB7_MOTOR_PWM3);
					csi_pwm_init(pwm, 5);
				break;
				
				case GPIO_PIN_8:
					g_u8PWMChannel = 1;
					csi_pin_set_mux(PB8, PB8_TIM1_1);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_9:
					g_u8PWMChannel = 2;
					csi_pin_set_mux(PB9, PB9_TIM1_2);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_10:
					g_u8PWMChannel = 3;
					csi_pin_set_mux(PB10, PB10_TIM1_3);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_11:
					g_u8PWMChannel = 4;
					csi_pin_set_mux(PB11, PB11_TIM1_4);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_12:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB12, PB12_MOTOR_PWM0);
					csi_pwm_init(pwm, 2);
				break;
				
				case GPIO_PIN_13:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB13, PB13_MOTOR_PWM1);
					csi_pwm_init(pwm, 3);
				break;
				
				case GPIO_PIN_14:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB14, PB14_MOTOR_PWM2);
					csi_pwm_init(pwm, 4);
				break;
				
				case GPIO_PIN_15:
					g_u8PWMChannel = 0;
					csi_pin_set_mux(PB15, PB15_MOTOR_PWM3);
					csi_pwm_init(pwm, 5);
				break;
				
				case GPIO_PIN_16:
					g_u8PWMChannel = 1;
					csi_pin_set_mux(PB16, PB16_TIM1_1);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_17:
					g_u8PWMChannel = 2;
					csi_pin_set_mux(PB17, PB17_TIM1_2);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_18:
					g_u8PWMChannel = 3;
					csi_pin_set_mux(PB18, PB18_TIM1_3);
					csi_pwm_init(pwm, 1);
				break;
				
				case GPIO_PIN_19:
					g_u8PWMChannel = 4;
					csi_pin_set_mux(PB19, PB19_TIM1_4);
					csi_pwm_init(pwm, 1);
				break;
				
				default:
				break;
			}
		break;
		
		default:
		break;
	}
	csi_pwm_out_config(pwm, g_u8PWMChannel, (1000000 / Freq), (DutyCycle * (1000000 / Freq)) / 100, PWM_POLARITY_HIGH);
	csi_pwm_out_stop(pwm, g_u8PWMChannel);
}

void SetBacklightPWMEnable(uint32_t state)
{
    if(state == PWMEnable)
	{
        BacklightPWMEnable = PWMEnable;
		#ifdef CHIP_GC9002_NoHyper
		BacklightPWMEnable = PWMDisable;
		#endif
	}
    else
        BacklightPWMEnable = PWMDisable;
		
}

void SetBacklightBrightness(uint32_t brightness)
{
    if(brightness > 100)
        return;
    
    if(BacklightCurrentBrightness == brightness)
        return;
    
    BacklightCurrentBrightness = brightness;
	if(BacklightWaitBrightness != brightness)
	{
		BacklightNormalBrightness = BacklightCurrentBrightness;
	}
	
    if(BacklightPWMEnable == PWMEnable)
    {
		#ifndef CHIP_GC9002_NoHyper
		ConfigPWM(&g_pwm0, (uint32_t)BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin, BacklightPWMFrequence, brightness);
		#endif
    }
    else if(BacklightPWMEnable == PWMDisable)
    {
        GPIO_InitTypeDef Backlight_InitStruct;
        
        /* Config BACKLIGHT GPIO */
		HAL_GPIO_StructInit(&Backlight_InitStruct);
        Backlight_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		Backlight_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		Backlight_InitStruct.Pin = BACKLIGHT_GPIOPin;
		HAL_GPIO_Init(BACKLIGHT_GPIOX, &Backlight_InitStruct);
		
        HAL_GPIO_WritePin(BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin, GPIO_PIN_RESET);
    }
}

uint32_t CheckBacklight(void)//return 1 -> need StartBacklight; return 0 -> need not StartBacklight;
{
    if(BacklightPWMEnable == PWMEnable)
    {
        if(BacklightCurrentBrightness != 0)
        {
            if(csi_pwm_get_status(&g_pwm0, 2))
			{
				return 0;
			}
			else
			{
				return 1;
			}
        }
        else
        {
            return 0;
        }
        
    }
    else if(BacklightPWMEnable == PWMDisable)
    {
        if(BacklightCurrentBrightness != 0)
        {
			if(HAL_GPIO_ReadPin(BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
    }
	return 0;
}

extern uint8_t displayconuter;
uint8_t GetDisplayCounter(void)
{
	return displayconuter;
}

void StartBacklightForFivePoint(void)
{
    if(BacklightPWMEnable == PWMEnable)
    {
        if(BacklightCurrentBrightness != 0)
        {
            BLON;
        }
        else
        {
            BLOFF;
        }
        
    }
    else if(BacklightPWMEnable == PWMDisable)
    {
        if(BacklightCurrentBrightness != 0)
        {
            HAL_GPIO_WritePin(BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin, GPIO_PIN_RESET);
        }
    }
}

void StartBacklight(void)
{
	if(GetDisplayCounter() <= BACKLIGHTONFRAMENUMBER)
		return;
	
    if(BacklightPWMEnable == PWMEnable)
    {
        if(BacklightCurrentBrightness != 0)
        {
            BLON;
        }
        else
        {
            BLOFF;
        }
        
    }
    else if(BacklightPWMEnable == PWMDisable)
    {
        if(BacklightCurrentBrightness != 0)
        {
            HAL_GPIO_WritePin(BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin, GPIO_PIN_RESET);
        }
    }
}

void StopBacklight(void)
{
    if(BacklightPWMEnable == PWMEnable)
    {
        BLOFF;
    }
    else if(BacklightPWMEnable == PWMDisable)
    {
        HAL_GPIO_WritePin(BACKLIGHT_GPIOX, BACKLIGHT_GPIOPin, GPIO_PIN_RESET);
    }
}

void SetBacklightPWMFrequence(uint32_t frequence)
{
    if((APBFREQUENCE / frequence) > BACKLIGHTPWMCOUNTERMAXVALUE)
        BacklightPWMFrequence = BACKLIGHTPWMFRE;
    else
        BacklightPWMFrequence = frequence;
}

void SetBacklightWait(uint32_t wait)
{
	if(BacklightPWMEnable == PWMEnable)
	{
		BacklightWaitOn = wait;
	}
	else
	{
		BacklightWaitOn = 0;
	}
}

void SetBacklightNormalBrightness(uint32_t brightness)
{
    BacklightNormalBrightness = brightness;
}

void SetBacklightWaitBrightness(uint32_t brightness)
{
    BacklightWaitBrightness = brightness;
}

void SetBacklightWaitDuration(uint32_t duration)
{
    BacklightWaitDuration = duration;
}

void SetBuzzerFrequence(uint32_t frequence)
{
    BuzzerFrequence = frequence;
}

void SetBuzzerDuty(uint32_t duty)
{
    BuzzerDuty = duty;
}

void SetBuzzerDutyDiv(uint32_t dutyDiv)
{
    BuzzerDutyDiv = dutyDiv;
}

void SetBuzzerDuration(uint32_t duration)
{
    BuzzerDuration = duration;
}

void SetBuzzerPWMEnable(uint32_t state)
{
    if(state == PWMEnable)
	{
        BuzzerPWMEnable = PWMEnable;
		#ifdef CHIP_GC9002_NoHyper
		BuzzerPWMEnable = PWMDisable;
		#endif
	}
    else
        BuzzerPWMEnable = PWMDisable;
		
}

void InitBuzzer(uint32_t frequence, uint32_t duty, uint32_t dutyDiv, uint32_t duration)
{
    if(BuzzerPWMEnable == PWMEnable)
    {
		#ifndef CHIP_GC9002_NoHyper
		ConfigPWM(&g_pwm1, (uint32_t)BUZZER_GPIOX, BUZZER_GPIOPin, BuzzerFrequence, 50);
		#endif
    }
    else if(BuzzerPWMEnable == PWMDisable)
    {
		GPIO_InitTypeDef BUZZER_InitStruct;
        
        /* Config BUZZER GPIO */
        BUZZER_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		BUZZER_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		BUZZER_InitStruct.Pin = BUZZER_GPIOPin;
		HAL_GPIO_Init(BUZZER_GPIOX, &BUZZER_InitStruct);
		
        HAL_GPIO_WritePin(BUZZER_GPIOX, BUZZER_GPIOPin, GPIO_PIN_RESET);
    }
}

uint32_t CheckBuzzer(void)//return 1 -> need StopBuzzer; return 0 -> need not StopBuzzer;
{
	if(BuzzerPWMEnable == PWMEnable)
    {
		if(csi_pwm_get_status(&g_pwm1, 3))
		{
			return 1;
		}
		else
		{
			return 0;
		}
    }
    else if(BuzzerPWMEnable == PWMDisable)
    {
		if(HAL_GPIO_ReadPin(BUZZER_GPIOX, BUZZER_GPIOPin))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void StartBuzzer(void)
{
    if(BuzzerPWMEnable == PWMEnable)
    {
        BUZZERON;
    }
    else if(BuzzerPWMEnable == PWMDisable)
    {
		if(BacklightCurrentBrightness != 0)
        {
            HAL_GPIO_WritePin(BUZZER_GPIOX, BUZZER_GPIOPin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(BUZZER_GPIOX, BUZZER_GPIOPin, GPIO_PIN_RESET);
        }
    }
	BuzzerStatus = 1;
}

void StopBuzzer(void)
{
    if(BuzzerPWMEnable == PWMEnable)
    {
        BUZZEROFF;
    }
    else if(BuzzerPWMEnable == PWMDisable)
    {
        HAL_GPIO_WritePin(BUZZER_GPIOX, BUZZER_GPIOPin, GPIO_PIN_RESET);
    }
	BuzzerStatus = 0;
}

void SetTouchSoundFlag(uint32_t flag)
{
    TouchSound = flag;
}

extern uint32_t ADC_DelayTimer;
void SetTRADCDelayTime(uint32_t time)
{
    ADC_DelayTimer = time;
}

void InitBacklightAndTimer(void)
{ 
    if((BacklightWaitOn > 0) && (BacklightPWMEnable == PWMEnable))
    {
        if(BacklightCurrentBrightness != BacklightNormalBrightness)
        {
            SetBacklightBrightness(BacklightNormalBrightness);
            StartBacklight();
        }
		
        hTimer0.mStopValue  = BacklightWaitDuration;
        InitAHMITimer(&hTimer0);
        StartAHMITimer(&hTimer0);
    }
}

void StartBuzzerAndTimer(void)
{
    if(TouchSound > 0)
    {
        StartBuzzer();
    
        hTimer1.mStopValue = BuzzerDuration;
        InitAHMITimer(&hTimer1);
        StartAHMITimer(&hTimer1);
    }
}

void StartBuzzerWithDuration(uint32_t duration)
{
    if(TouchSound > 0)
    {
        StartBuzzer();
    
        hTimer1.mStopValue = duration;
        InitAHMITimer(&hTimer1);
        StartAHMITimer(&hTimer1);
    }
}
       
void SetAHMIOutputRotation(uint32_t rotation)
{
    if(rotation == 0)
    {
         AHMIOutputRotation = DEGREE0;
    }
    else if(rotation == 90)
    {
         AHMIOutputRotation = DEGREE90;
    }
    else if(rotation == 180)
    {
         AHMIOutputRotation = DEGREE180;
    }
    else if(rotation == 270)
    {
         AHMIOutputRotation = DEGREE270;
    }
    else
    {
         AHMIOutputRotation = DEGREE0;
    }
}

void SetSystemOption(uint32_t option)
{
    if(option == 1)
    {
        SystemOption = 1;
    }
    else
    {
        SystemOption = 0;
    }
}

uint32_t GetAHMIOutputRotation(void)
{ 
    return AHMIOutputRotation;
}

void GetTouchScreenCalibrationFlag(uint32_t* PFlag)
{
    if((*(uint32_t *)(BANK_WRITE_START_ADDR_FOR_TOUCH) == TOUCHCALIFLAG) && ((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENCAPACITIVE)))
    {
        *PFlag = 0;
        
//        csi_hal_interal_flash_erase(BANK_WRITE_START_ADDR_FOR_TOUCH);
    }
}

uint32_t initAHMIHardware( void )
{
#ifdef EMBEDDED
    /* Default value */
    uint32_t para[DisplayParaNumber] = {0,   //polarity
                                        800, 480, 20, 10, 210, 22, 46, 23, 8, LCD_RGB565, DISPLAY_Mode_MasterActive, 0,  //diaplay
										//132, 132, 15, 15, 15, 1, 15, 1, 16, LCD_ST7735S, DISPLAY_Mode_SlavePassive, 0,  //diaplay
										//240, 320, 15, 15, 15, 1, 15, 1, 16, LCD_MRB3205, DISPLAY_Mode_SlavePassive, 0,  //diaplay
										//400, 400, 150, 8, 100, 6, 1, 6, 48, LCD_ST77903, DISPLAY_Mode_MasterActive, 0,  //diaplay
										//360, 360, 15, 15, 15, 1, 15, 1, 16, LCD_SJXD, DISPLAY_Mode_SlavePassive, 0,  //diaplay
										//128, 160, 15, 15, 15, 1, 15, 1, 48, LCD_ST7735S, DISPLAY_Mode_SlavePassive, 0,
                                        0,  //ResolutionType
                                        1,  //crc
                                        0, 5000, 0, 100, 50, 6000,  // BackLight
                                        0, 4000, 300, 3000, 10,     //Buzzer
                                        0, 0, 0, 0,         //TouchScreen
                                        0,                  //System option
                                        100, 100, 100,      //display contrast, brightness, saturation
                                        1000                //ADC delay time
                                        };
	uint32_t initAHMIHardwareState = AHMI_FUNC_SUCCESS;
	uint32_t UpgradeProductParaState;
	
    /* init extern texture flash */
	//csi_hal_internal_flash_init();
	
	/* Get data from procduct.bin *//* when defined CHIP_GC9002_NoHyper, cpu will goto trap_c from scanf. */
    UpgradeProductParaState = UpgradeProductPara(BANK_WRITE_START_ADDR_PRODUCT_PARA, para);
	if(UpgradeProductParaState == (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr))
	{
		initAHMIHardwareState = (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr);
	}
    UpgradeProductParaState = UpgradeProductPara(BANK_WRITE_START_ADDR_HWCONFIG, para);
	if(UpgradeProductParaState == (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr))
	{
		initAHMIHardwareState = (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr);
	}
    
	/* Initial Display & parameters by product.bin */
    InitDisplayByBin(
		para[BINPOLARITY], 
        para[BINHPIXEL_VALUE], 
        para[BINVLINES_VALUE],   
        para[BINHSYNCPW_VALUE],  
        para[BINVSYNCPW_VALUE],  
        para[BINHFNPRCH_VALUE],  
        para[BINVFNPRCH_VALUE],  
        para[BINHBKPRCH_VALUE],  
        para[BINVBKPRCH_VALUE],  
		para[BINCLKINDEVIDE],  
        para[BINDISPLAYTNTERFACE],     
        para[BINSCREENRAM], 
        FRAME_BASE_ADDR,
        para[BINPARA_EXTRA]);
    
    SetAHMIOutputRotation(para[BINDISPLAYROTATE]);
    
    SetBacklightPWMEnable(para[BINBACKLIGHTPWMENABLE]);
    SetBacklightPWMFrequence(para[BINBACKLIGHTPWMFREQUENCE]);
    SetBacklightWait(para[BINBACKLIGHTON]);
    SetBacklightNormalBrightness(para[BINBACKLIGHTNORMAL]);
    SetBacklightWaitBrightness(para[BINBACKLIGHTWAIT]);
    SetBacklightWaitDuration(para[BINBACKLIGHTDURATION]);
    
    SetTouchSoundFlag(para[BINTOUCHSOUND]);
    SetBuzzerPWMEnable(para[BINBUZZERPWMENABLE]);
    SetBuzzerFrequence(para[BINBUZZERFREQUENCE]);
    SetBuzzerDuty(para[BINBUZZERDUTY]);
    SetBuzzerDutyDiv(para[BINBUZZERDUTYDIV]);
    SetBuzzerDuration(para[BINBINBUZZERDURATION]);
    SetTouchScreenType(para[BINTOUCHSCREENTYPE]);
    SetTouchScreenInterchangeXY(para[BINTOUCHSCREENCHANGEXY]);
    SetTouchScreenRotation(para[BINTOUCHSCREENROTATE]);
    SetResolutionRatioType(para[BINRESOLUTIONRATIOTYPE]);
    SetSystemOption(para[BINSYSTEMOPTION]);
    SetDisplayContrast(para[BINDISPLAYCONTRAST]);
	DisplayContrast = para[BINDISPLAYCONTRAST];
    SetDisplayBrightness(para[BINDISPLAYBRIGHTNESS]);
	DisplayBrightness = para[BINDISPLAYBRIGHTNESS];
    SetDisplaySaturation(para[BINDISPLAYSATURATION]);
	DisplaySaturation = para[BINDISPLAYSATURATION];
    SetTRADCDelayTime(para[BINTRADCDELAY]);
    
    if(BacklightWaitOn > 0)
    {
        SetBacklightBrightness(BacklightNormalBrightness);
        hTimer0.mStopValue  = BacklightWaitDuration;
        InitAHMITimer(&hTimer0);
        StartAHMITimer(&hTimer0);
    }
    else
    {
        SetBacklightBrightness(BacklightNormalBrightness);
    }
    
    if(TouchSound > 0)
    {
        InitBuzzer(BuzzerFrequence, BuzzerDuty, BuzzerDutyDiv, BuzzerDuration);
    }
	
    /* TOUCH SCREEN SUPPORT */
    if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENTYPEEND))
    {
        /*  Configure touch screen and the chip */
        AHMITouch_InitDevice();
        
        GetTouchScreenCalibrationFlag(&flag_Calibration);
		
		/*  Configure TIM0 for AHMITouch_vGetTouchInfo */
		//Timer0Init(); 
		SetTouchNeedWork();
    }
	
	return initAHMIHardwareState;
#endif
}

#ifdef EMBEDDED
static csi_wdt_t g_wdt;

void SetWDG2(void)
{
    csi_error_t ret;
    
    ret = csi_wdt_init(&g_wdt, 0);
	if(ret)
    {
        /* record err */
    }

    csi_wdt_set_timeout(&g_wdt, CPU1_WDT_TIMEOUT_MS);

    csi_wdt_start(&g_wdt);
    /* Feed dog after start */
    csi_wdt_feed(&g_wdt);
}

void WDG2_ReloadCounter(void)
{
    csi_wdt_feed(&g_wdt);
}

#endif

//-----------------------------
// function: HardwareInit
// @brief:
//-----------------------------
//#include "cm_backtrace.h"
void HardwareInit(void)
{
	GPIO_InitTypeDef DrawDoneTest_InitStruct;

    CFG_vInit();
    CFG_vGetPinAndPortInfo();
    
	#ifdef DRAWDONETEST_ENBALE
	/* Config DrawDone Test GPIO */
	HAL_GPIO_StructInit(&DrawDoneTest_InitStruct);
	DrawDoneTest_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	DrawDoneTest_InitStruct.Pull = GPIO_PULLUP;
	DrawDoneTest_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	DrawDoneTest_InitStruct.Pin = DRAWDONETEST_GPIOPin;
	HAL_GPIO_Init(DRAWDONETEST_GPIOX, &DrawDoneTest_InitStruct);
	#endif
	
	/* Initial AHMIHardware */
    initAHMIHardware();
    
    ADCSHR_vInit();
	/* Initial Timer */
    //Timer5Init(); //by Zhu Jiahai, 2022-06-14  
    //Timer8Init();
	Timer0Init();

	/* set WDG2 enable if need */
	//SetWDG2();
}

extern uint32_t AHMITick;
uint32_t old_AHMITicks = 0;
uint8_t led_flag = 0;
void SetValueTask(void* pvParameters)
{
    //uint32_t i = 0;
	
	/* Set Version Information System String Tag. */
    SetProductInfo();
	
 	for(;;)
 	{   
//		//for breathing lamp
//		if((AHMITick - old_AHMITicks) > 1000)
//		{
//			led_flag = !led_flag;
//			Value.mID = 0;
//			Value.mValue = led_flag;
//			ShareMemory_SendDrawDoneFrame(0, 1, &Value, 50, 0, 0);
//			old_AHMITicks = AHMITick;
//		}

		//if(i < 20)
        //{
            TagPtr[SYSTEM_FRAME_RATE_TAG].setValue(AHMIDone_fps, SYSTEM_FRAME_RATE_TAG);
        //    i++;
            vTaskDelay(1000 / portTICK_RATE_MS);
        //}
        //else
        //{
        //    vTaskDelay(portMAX_DELAY);
        //}
		
        
 	}
}

//void SetRTCTask(void* pvParameters)
//{
//    uint32_t date = 0;
//    uint32_t time = 0;
// 	for(;;)
// 	{   
//        
//        date = RTC_GetCurrentDate();
//        time = RTC_GetCurrentTime();
//        
//        setTagValue(SYSTEM_RTC_YEAR_TAG, date);
//        
//        setTagValue(SYSTEM_RTC_HOUR_MINUITE_TAG, time);
//        
//        vTaskDelay(5 / portTICK_RATE_MS);
// 	}
//}

extern "C"{
	extern void TIM0_IRQHandler(void);
}
void Timer0Init(void)
{
	htim0.Init.Prescaler = HAL_RCC_GetAPBClock()/10000 - 1;
	htim0.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim0.Init.Period = 5 - 1;
	htim0.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim0.Init.RepetitionCounter = 0xFFFE;
	htim0.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	
	HAL_TIM_Base_Init(&htim0);
	
	csi_hal_irq_register(TIM0_IRQn, (void *)TIM0_IRQHandler);
	csi_hal_irq_enable(TIM0_IRQn);
	HAL_TIM_Base_Start_IT(&htim0);
}

extern "C"{
	extern void TIM1_IRQHandler(void);
}
void Timer1Init(void)
{
	htim1.Init.Prescaler = HAL_RCC_GetAPBClock()/10000 - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 100 - 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0xFFFE;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	
	HAL_TIM_Base_Init(&htim1);
	
	csi_hal_irq_register(TIM1_IRQn, (void *)TIM1_IRQHandler);
	csi_hal_irq_enable(TIM1_IRQn);
	HAL_TIM_Base_Start_IT(&htim1);
}

void AHMITIMCB0(void)
{
	if(BacklightCurrentBrightness <= BacklightWaitBrightness)
	{
		SetBacklightBrightness(BacklightCurrentBrightness);
		StartBacklight();
	}
	else
	{
		SetBacklightBrightness(BacklightWaitBrightness);
		StartBacklight();
	}
}

void AHMITIMCB1(void)
{
    StopBuzzer();
}
