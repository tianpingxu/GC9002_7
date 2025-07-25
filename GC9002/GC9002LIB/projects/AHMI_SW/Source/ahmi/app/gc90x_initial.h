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
#ifndef GC90X_INIT_H
#define GC90X_INIT_H

#include "gc90x_config.h"

#ifdef __cplusplus 
extern "C" {
#endif

#include "cfg_c1.h"


#define PWMDisable     (0)    
#define PWMEnable      (1)

#define APBFREQUENCE                    (21000000)

#define BACKLIGHTONFRAMENUMBER      	(4)
	
#define BACKLIGHTPWMFRE                 (5000)  
#define BACKLIGHTPWMCOUNTERMAXVALUE     (65535)    
#define DefaultBacklightBrightness      (80)

//use GPIO/PWM drive buzzer, use only one of following define
#define BUZZER_DRIVER_BY_PWM
//#define BUZZER_DRIVER_BY_GPIO
    
//use PWM driver buzzer
#ifdef BUZZER_DRIVER_BY_PWM
    #define BUZZERPWMFRE            (3000) 
#endif

//use GPIO driver buzzer
//#define GPIOA                   (0)
//#define GPIOB                   (1)
//BUZZER GPIO
#define BUZZER_GPIOX                g_buzzerPort
#define BUZZER_GPIOPin              g_buzzerPin

#define BACKLIGHT_GPIOX             g_backLightPort
#define BACKLIGHT_GPIOPin           g_backLightPin      //GPIO_PIN_4 For gc9002_deb20230210_v1_1_0


//#define DRAWDONETEST_ENBALE

#ifdef DRAWDONETEST_ENBALE
#define DRAWDONETEST_GPIOX          g_sysTestPort
#define DRAWDONETEST_GPIOPin        g_sysTestPin
#endif

#define CPU1_WDT_TIMEOUT_MS         (uint32_t)(1000)              


#define TOUCH_TIMER_THRESHOLD       (10)       /* Add up to threshold will cause a execution of Touch processing, TIM1 500us, 10ms needs 20 * 500us */    

#define ADCSHARE_TIMER_THRESHOLD    (10)       /* Add up to threshold will cause a execution of Touch processing, TIM1 500us, 10ms needs 20 * 500us */  

typedef enum ResolutionRatioTypeTag
{
    RESRATIOTYPE0 = (0),
    RESRATIOTYPE1,   
	RESRATIOTYPE2, 	
    RESRATIOTYPEEND ,         
}ResolutionRatioTypeDef;

bool GetIfTouchNeedWork(void);

bool GetIfADCShareNeedWork(void);

void SetBacklightPWMFrequence(uint32_t frequence);

uint32_t GetAHMIOutputRotation(void);
    
void HardwareInit(void);
   
void SetValueTask(void* pvParameters);
void SetRTCTask(void* pvParameters);

void StartBacklightForFivePoint(void);
void StartBacklight(void);
uint32_t CheckBacklight(void);
void SetBacklightBrightness(uint32_t brightness);

void InitBacklightAndTimer(void);
void StartBuzzer(void);
void StopBuzzer(void);
void StartBuzzerAndTimer(void);
void StartBuzzerWithDuration(uint32_t duration);
void SetTouchSoundFlag(uint32_t flag);
void SetProductInfo(void);
void GetTouchScreenCalibrationFlag(uint32_t* PFlag);
void SetTRADCDelayTime(uint32_t time);
    
uint8_t GetDisplayCounter(void);
void StopBacklight(void);
void InitBuzzer(uint32_t frequence, uint32_t duty, uint32_t dutyDiv, uint32_t duration);
uint32_t CheckBuzzer(void);	
void WDG2_ReloadCounter(void);

#ifdef __cplusplus 
}
#endif

#endif


