/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : com_ci.h
*
* Description  : This file contains the internal / import configuration of COM
*
* Environment  : MDK / PC Simulation
*
* Responsible  : 
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2024-02-27
****************************************************************************/
#ifndef COM_CI_H
#define COM_CI_H

/* Note: only set the switch to 1 when COM module will be compiled in system */
#define COM_COMPILE_SWITCH                                      (1)     /* if #ifndef CHIP_GC9002_NoHyper , can't open this option , as no much space to compile */

/* Set this config to 1 will run UART rollback test: send out what received , set to 0 will run customer defined parse function */
#define COM_UART_TEST_ROLLBACK                                  (0)


/* Uart RX Event */
#define UART_RX_EVENT_UART_IDLE                                 0x80




#endif

