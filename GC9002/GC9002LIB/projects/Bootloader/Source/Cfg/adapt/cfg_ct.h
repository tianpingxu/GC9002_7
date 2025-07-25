/**
  ******************************************************************************
  * @file    cfg_ct.h
  * @author  
  * @version V1.0.0
  * @date    8-15-2023
  * @brief   The type definition head file of CFG module
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_CT_H
#define __CFG_CT_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"

#define UART_BAUDRATE_9600                                  (uint32_t)(9600)
#define UART_BAUDRATE_19200                                 (uint32_t)(19200)
#define UART_BAUDRATE_57600                                 (uint32_t)(57600)
#define UART_BAUDRATE_115200                                (uint32_t)(115200)



typedef enum
{
    CFG_enPin_0 = 0,
    CFG_enPin_1,
    CFG_enPin_2,
    CFG_enPin_3,
    CFG_enPin_4,
    CFG_enPin_5,
    CFG_enPin_6,
    CFG_enPin_7,
    CFG_enPin_8,
	CFG_enPin_9,
    CFG_enPin_10,
    CFG_enPin_11,
    CFG_enPin_12,
    CFG_enPin_13,
    CFG_enPin_14,
    CFG_enPin_15,
    CFG_enPin_16,
    CFG_enPin_17,
    CFG_enPin_18,
    CFG_enPin_19,
    CFG_enPin_20,
    CFG_enPin_21,
    CFG_enPin_22,
	CFG_enPin_23,
    CFG_enPin_24,
    CFG_enPin_25,
    CFG_enPin_26,
    CFG_enPin_27,
    CFG_enPin_28,
    CFG_enPin_29,
    CFG_enPin_30,
    CFG_enPin_31,
    CFG_enPin_Total

}CFG_tenPin;


typedef enum
{
    CFG_enPort_A = 0,
    CFG_enPort_B,
    CFG_enPort_Total,
 
}CFG_tenPort;


typedef enum
{
    CFG_enUartBaud_9600 = 0,
    CFG_enUartBaud_19200,
    CFG_enUartBaud_57600,
    CFG_enUartBaud_115200,
    CFG_enUartBaud_Total
 
}CFG_tenUartBaud;


typedef enum
{
    CFG_enUartChanel_0 = 0,
    CFG_enUartChanel_1,
    CFG_enUartChanel_2,
    CFG_enUartChanel_Total
 
}CFG_tenUartChannel;


typedef enum
{
    CFG_enCanBaud_250K = 0,
    CFG_enCanBaud_500K,
    CFG_enCanBaud_833K,
    CFG_enCanBaud_1000K,
    CFG_enCanBaud_1538K,
    CFG_enCanBaud_2000K,
    CFG_enCanBaud_3077K,
    CFG_enCanBaud_5000K,
    CFG_enCanBaud_6667K,
    CFG_enCanBaud_8000K,
    CFG_enCanBaud_10000K,
    CFG_enCanBaud_Total,
 
}CFG_tenCanBaud;


typedef enum
{
    CFG_enCanChanel_0 = 0,
    CFG_enCanChanel_Total
 
}CFG_tenCanChannel;


typedef enum
{
    CFG_enSpiChanel_0 = 0,
    CFG_enSpiChanel_1,
    CFG_enSpiChanel_2,
    CFG_enSpiChanel_Total
 
}CFG_tenSpiChannel;


#ifdef __cplusplus
}
#endif

#endif /* __CFG_CT_H */




