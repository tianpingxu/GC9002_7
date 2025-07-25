/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : SBM1ce.h
*
* Description  : This file contains the external / export configuration of SBM
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
#ifndef SBM_CE_H
#define SBM_CE_H

#ifdef __cplusplus 
extern "C" {
#endif


#define ON                                      (1)
#define OFF                                     (0)

/* Note: set the switch to ON only when do Integration Testing */
#define SBM_CPU0_START_ADDR                     0x410003C0

#define SBM_CPU1_START_ADDR                     0x410003C4


#define SBM_HARDWARE_IN_USE                     (0xF7)
#define SBM_HARDWARE_NO_USE                     (0x00)


#define SBM_u32SetHardwareState(a, b)           (((a) & 0xFFFFFF00) + ((b) & 0xFF))
#define SBM_u8GetHardwareState(a)               (uint8_t)((a) & 0xFF)


#ifdef __cplusplus 
}
#endif

#endif

