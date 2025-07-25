/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : adcshare_ci.h
*
* Description  : This file contains the internal / import configuration of ADCSHARE
*
* Environment  : MDK / PC Simulation
*
* Responsible  : 
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2023-12-18
****************************************************************************/
#ifndef ADCSHARE_CI_H
#define ADCSHARE_CI_H


/* Max data size per channel */
#define ADCSHR_CHANNEL_DATA_SIZE                                (16)

/* Max channel number */
#define ADCSHR_CHANNEL_MAX_NUMBER		                        (9)

/* the max channel busy count, if reach max busy count, will change the busy state */
#define ADCSHR_CHANNEL_MAX_BUSY_COUNT		                    (5)

/* the max wait time of Immediately getting ADC value */
#define ADCSHR_IMMEDIATELY_MAX_WAIT_TIME		                (10)






#endif

