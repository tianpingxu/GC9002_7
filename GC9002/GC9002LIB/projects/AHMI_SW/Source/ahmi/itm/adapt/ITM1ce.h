/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : ITM1ce.h
*
* Description  : This file contains the external / export configuration of ITM
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-05-19
****************************************************************************/
#ifndef ITM_CE_H
#define ITM_CE_H

#ifdef __cplusplus 
extern "C" {
#endif


#define ON                                      (1)
#define OFF                                     (0)

/* Note: set the switch to ON only when do Integration Testing */
#define ITM_COMPILE_SWITCH                      OFF

/* switch of debug */
#define ITM_DEBUG_SWITCH                        ON

#define configITM_STACK_SIZE                    (256)  
#define configTMP_ARRAY_SIZE                    (400)

/* Print period, unit: minute */
#define configITM_PRINT_PERIOD                  (1)         
/* 10ms one SYS Tick */
#define configSYS_TICK                          (1)
#define configITM_DELAY_TIMES_PER_MINUTE        (60000 / configSYS_TICK)
#define errTASK_CREATE_FAIL                     (-8)


#if (ITM_DEBUG_SWITCH == ON)
#define ITM_STATIC
#else
#define ITM_STATIC                              static
#endif

/* As Timer7Init() has no declaration, so use extern xxx */
extern void Timer7Init(void);

#define SYN_ITM_TIMER_INIT                      Timer7Init

#ifdef __cplusplus 
}
#endif

#endif

