////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     Keyboard.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/09/01 by Zhou Yuzhi 
// Additional Comments:
//    definition of keyboard trigger
// 
////////////////////////////////////////////////////////////////////////////////

#include "Keyboard.h"
#include "publicInclude.h"
#include "ActionTriggerClass.h"

extern QueueHandle_t        keyboardQueue;
extern TagClassPtr          TagPtr;


void keyboardLeft( void )
{
	ActionTriggerClass tagtrigger;
	tagtrigger.mInputType = ACTION_KEYBOARD_PRE;
	tagtrigger.keyboardTouch();
}
void keyboardRight( void )
{
	ActionTriggerClass tagtrigger;
	tagtrigger.mInputType = ACTION_KEYBOARD_NEXT;
	tagtrigger.keyboardTouch();
}
void keyboardEntered( void )
{
	ActionTriggerClass tagtrigger;
	tagtrigger.mInputType = ACTION_KEYBOARD_OK;
	tagtrigger.keyboardTouch();
}

void keyboardEnteredPress( void )
{
	
	ActionTriggerClass tagtrigger;
	tagtrigger.mInputType = ACTION_KEYBOARD_OK_PRESS;
	tagtrigger.keyboardTouch();
}

//clear the highlight 
//reset the highlight pointer into the first highlight widget
void keyboardClear( void )     
{
	ActionTriggerClass tagtrigger;
	tagtrigger.mInputType = ACTION_KEYBOARD_CLEAR;
	tagtrigger.keyboardTouch();
}

#ifdef PC_SIM
//sent the value of key to keyboard queue
void sentToKeyboardQueue(u8 key)
{
	csi_kernel_msgq_put(keyboardQueue, &key, 0, portMAX_DELAY);
}
#endif
