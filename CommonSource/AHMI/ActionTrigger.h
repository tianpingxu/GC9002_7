////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     ActionTrigger.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef  ACTIONTRIGGER__H
#define  ACTIONTRIGGER__H


#include "AHMICfgDefinition.h"

#ifdef PC_SIM
#define  MOUSE_NOTHING      0  // nothing
#define  MOUSE_PRESS		1 // press the mouse
#define  MOUSE_RELEASE		2 // release the mouse
#define  MOUSE_HOLD			3 // drag the mouse
#define  MOUSE_LONGPRESS    4 // long press the mouse
#endif

#ifdef EMBEDDED
#define  MOUSE_NOTHING      0  // nothing
#define  MOUSE_PRESS    	1  // press the mouse
#define  MOUSE_RELEASE  	2  // release the mouse
#define  MOUSE_HOLD     	3  // drag the mouse
#define  MOUSE_LONGPRESS    4  // long press the mouse
#endif

#ifdef PC_SIM_Gen
#define  MOUSE_NOTHING      0  // nothing
#define  MOUSE_PRESS		1  // press the mouse
#define  MOUSE_RELEASE		2  // release the mouse
#define  MOUSE_HOLD			3  // drag the mouse
#define  MOUSE_LONGPRESS    4  // long press the mouse
#endif

#endif
