#include <stdio.h>
#include <stdint.h>
#include "csi_ahmi_hw.h"
#include "AHMITouch.h"
#include "gc90x_initial.h"


/**
  * @name       csi_ahmi_hw_int
  * @brief      AHMI Hardware Init interface
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_hw_int(void)
{
	HardwareInit();
}


/**
  * @name       csi_ahmi_SetBacklightBrightness
  * @brief      Set backlight brightness interface
  * @note       
  * @param      brightness: specify the brightness to set
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_SetBacklightBrightness(uint32_t brightness)
{
    SetBacklightBrightness(brightness);
}


/**
  * @name       csi_ahmi_CheckBacklight
  * @brief      Check backlight brightness interface
  * @note       
  * @param      none
  * @retval     the current back brightness
  * @author     
  * @date       2024-01-02
  */
uint32_t csi_ahmi_CheckBacklight(void)
{
    return CheckBacklight();
}


/**
  * @name       csi_ahmi_StartBacklightForFivePoint
  * @brief      Start backlight for Five Point
  * @note       Customer no need to care this inferface
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_StartBacklightForFivePoint(void)
{
    StartBacklightForFivePoint();
}


/**
  * @name       csi_ahmi_StartBacklight
  * @brief      Start backlight brightness interface
  * @note       After called this interface, backlight start work
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_StartBacklight(void)
{
    StartBacklight();
}


/**
  * @name       csi_ahmi_StopBacklight
  * @brief      Stop backlight brightness interface
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_StopBacklight(void)
{
    StopBacklight();
}


/**
  * @name       csi_ahmi_InitBuzzer
  * @brief      Init Buzzer interface
  * @note       
  * @param      frequence: indicate buzzer frequence
  * @param      duty: indicate buzzer duty
  * @param      dutyDiv: indicate buzzer dutyDiv
  * @param      duration: indicate duration of buzzer
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_InitBuzzer(uint32_t frequence, uint32_t duty, uint32_t dutyDiv, uint32_t duration)
{
    InitBuzzer(frequence, duty, dutyDiv, duration);
}


/**
  * @name       csi_ahmi_CheckBuzzer
  * @brief      Check if need to stop Buzzer
  * @note       
  * @param      none
  * @retval     1 -> need stop Buzzer; return 0 -> need not Stop Buzzer;
  * @author     
  * @date       2024-01-02
  */
uint32_t csi_ahmi_CheckBuzzer(void)
{
    return CheckBuzzer();
}


/**
  * @name       csi_ahmi_StartBuzzer
  * @brief      Start Buzzer interface
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_StartBuzzer(void)
{
    StartBuzzer();
}


/**
  * @name       csi_ahmi_StopBuzzer
  * @brief      Stop Buzzer interface
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_StopBuzzer(void)
{
    StopBuzzer();
}


/**
  * @name       csi_ahmi_InitBacklightAndTimer
  * @brief      Init backlight and Timer interface
  * @note       When Timer timeout, will stop backlight
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_InitBacklightAndTimer(void)
{
    InitBacklightAndTimer();
}


/**
  * @name       csi_ahmi_StartBuzzerAndTimer
  * @brief      Start buzzer and Timer interface
  * @note       When Timer timeout, will stop buzzer
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_StartBuzzerAndTimer(void)
{
    StartBuzzerAndTimer();
}


/**
  * @name       csi_ahmi_AHMITouch_InitDevice
  * @brief      Init AHMI Touch device interface
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
uint8_t csi_ahmi_AHMITouch_InitDevice(void)
{
    return AHMITouch_InitDevice();
}






