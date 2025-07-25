////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     Mouse.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef MOUSE__H
#define MOUSE__H
#include "publicInclude.h"

/* Define Touch Task default delay time */
#define TOUCH_DELAY_TIME_DEFAULT                            (10)

/* Define Touch Task delay time under touch event existing */
#define TOUCH_DELAY_TIME_UNDER_EVENT                        (20)

void TouchInfo_vMain(void* pvParameters);
void Touch_vMain(void* pvParameters);

#endif
