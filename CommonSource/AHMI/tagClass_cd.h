////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     Tagclass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.04 - C++, 20160321 by Zhu Yuzhi
// Additional Comments:
//    definition of tag
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef TAGCLASS_CD__H
#define TAGCLASS_CD__H
#include "publicDefine.h"

#define  TAGNAMESIZE 20

#define  TAGTYPE_INT  0
#define  TAGTYPE_STR  1
#define  TAGTYPE_ARR  2
#define  TAG_READ_REG 0x80
#define  TAG_NO_READ_REG 0

#define TAGREAD 0
#define TAGWRITE 1

//spical tag
#define NUM_OF_SYSTEM_TAG					(30)    //number of spical tag.
#define SYSTEM_TAG_0							(0)     //default.
#define SYSTEM_KEYBOARD_TAG					(1)     //keyboard tag.
#define SYSTEM_DISABLEPAGEANIMATION_TAG		(2)     //disable page animation tag.
#define SYSTEM_PAGE_TAG						(3)     //page tag,change page.
#define SYSTEM_RTC_YEAR_TAG					(4)     //RTC year tag.
#define SYSTEM_RTC_HOUR_MINUITE_TAG			(5)     //RTC: hour,minute.
#define SYSTEM_BACKLIGHT_TAG					(6)     //basklight tag.
#define SYSTEM_ALARM_TAG						(7)     //bee tag.
#define SYSTEM_FRAME_RATE_TAG				(8)     //fps tag.
#define SYSTEM_TOUCHSCREEN_TAG				(9)     //touch screen calibration control.
#define SYSTEM_YTHVERSION_TAG				(10)    //yth version
#define SYSTEM_IDEVERSION_TAG				(11)    //ide version
#define SYSTEM_CPU0APPVERSION_TAG			(12)    //cpu0 app version
#define SYSTEM_CPU1APPVERSION_TAG			(13)    //cpu1 app version
#define SYSTEM_CPU0BOOTVERSION_TAG			(14)    //cpu0 boot version
#define SYSTEM_CPU1BOOTVERSION_TAG			(15)    //cpu1 boot version
#define SYSTEM_DEVICEID_TAG					(16)    //device id
#define SYSTEM_ERROR_MESSAGE_TAG				(17)    //error message tag.
#define SYSTEM_ERROR_MESSAGE_PAGE_NUM_TAG	(18)    //error message page number tag.
#define SYSTEM_RESET_TYPE_TAG				(19)    //reset type tag.
#define SYSTEM_CONTRAST_TAG					(20)    //display contrast tag.
#define SYSTEM_SATURATION_TAG				(21)    //display saturation tag.
#define SYSTEM_BRIGHTNESS_TAG				(22)    //display brightness tag.
#define SYSTEM_POINTER_TAG					(23)    //tag pointer system tag.
#define SYSTEM_DisplayRotate_TAG				(24)    //display rotate system tag.
#define SYSTEM_BacklightDuration_TAG			(25)    //backlight duration system tag.
#define SYSTEM_CHROMINANCE_TAG				(26)    //chrominance system tag.
#define SYSTEM_COLOR_TEMPERATURE_TAG			(27)    //Color temperature system tag.
#define SYSTEM_GAMMA_CORRECTION_TAG			(28)    //gamma correction system tag.
#define SYSTEM_PAGE_HIDDEN_TAG				(29)    //Slide to switch page hiding array system tag.(when enable CHIP_GC9002_NoHyper, change to an INT Tag)
#define NORMAL_TAG							(30)    //start ID for user tag.

//linked element's type
#define ELEMENT_TYPE_WIDGET 0
#define ELEMENT_TYPE_CANVAS 1
#define ELEMENT_TYPE_PAGE   2
#define ELEMENT_TYPE_KEYBOARD   3
#define ELEMENT_TYPE_PAGE_GOTO  4


#define TAG_IS_INT(tagType) ( (tagType & 3) == TAGTYPE_INT)
#define TAG_IS_STR(tagType) ( (tagType & 3) == TAGTYPE_STR)
#define TAG_IS_ARR(tagType) ( (tagType & 3) == TAGTYPE_ARR)


#endif
