////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     Keyboard.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/09/01 by Zhou Yuzhi 
// Additional Comments:
//    definition of keyboard trigger
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef KEYBOARD__H
#define KEYBOARD__H

#include "keyboard_cd.h"
#include "publicType.h"

//the tag tiggers the left/right/enter cmd for keyboard
void keyboardLeft( void );
void keyboardRight( void );
void keyboardEntered( void );
void keyboardEnteredPress( void );
//clear the highlight 
//reset the highlight pointer into the first highlight widget
void keyboardClear( void );    
#ifdef __cplusplus
extern "C"{
#endif

//sent the value of key to keyboard queue
void sentToKeyboardQueue(u8 key);
#ifdef __cplusplus
}
#endif

#endif
