////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     ActionTriggerClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.04 - C++, 20160321 by YuchunYing
// Additional Comments:
//    definition of tagtrigger input
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef ACTION_TRIGGER_CLASS_CD__H
#define ACTION_TRIGGER_CLASS_CD__H
//tagtrigger
#include "publicDefine.h"


#define ACTION_NULL          		0    //0none事件。
#define ACTION_MOUSE_PRESS   		1    //1 MousePress
#define ACTION_MOUSE_RELEASE 		2    //2 MouseRelease
#define ACTION_MOUSE_HOLDING 		3    //3 MouseHolding
#define ACTION_MOUSE_LONGPRESS		10   //10 MouseLongPress 
#define ACTION_MOUSE_CANCLE  		4    //4 MouseCancle 
#define ACTION_MOUSE_PROPAGATION  	11    //11 Cancle Propagation
#define ACTION_KEYBOARD_PRE  		5    //5 KeyboardPre 指向前一个控件
#define ACTION_KEYBOARD_NEXT 		6    //6 KeyboardNext 指向下一个控件
#define ACTION_KEYBOARD_OK   		7    //7 KeyboardOK   按下OK
#define ACTION_TAG_SET_VALUE 		8    //8 TagSetValue  给tag赋值
#define ACTION_KEYBOARD_CLEAR 		9   //9 clear the highlight
#define ACTION_KEYBOARD_OK_PRESS    12   // keybaord ok press


#define TouchPropagationSet					(0x001)
#define TouchStatePressed					(0x100)
#define TouchStatePropagationWithCancle		(0x200)
#define TouchStatePropagationWithoutCancle	(0x400)

#endif


