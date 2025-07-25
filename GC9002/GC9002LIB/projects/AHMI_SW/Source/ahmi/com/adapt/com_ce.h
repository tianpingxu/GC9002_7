/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : com_ce.h
*
* Description  : This file contains the external / export configuration of COM
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
#ifndef COM_CE_H
#define COM_CE_H

#ifdef __cplusplus 
extern "C" {
#endif

#include "gc90xx.h"
#include "csi_irq_vectors.h"
#include "csi_hal_core.h"
#include "csi_kernel.h"



#define CUSTOM_UART                                             UART0
#define CUSTOM_UART_IRQn                                        UART0_IRQn
#define CUSTOM_UART_DMA_RX_HS                                   DMA_HS_UART0_RX
#define CUSTOM_UART_DMA_TX_HS                                   DMA_HS_UART0_TX

#define CUSTOM_UART_RX_PORT                                     GPIOB
#define CUSTOM_UART_RX_PIN                                      GPIO_PIN_2
#define CUSTOM_UART_TX_PORT                                     GPIOB
#define CUSTOM_UART_TX_PIN                                      GPIO_PIN_3

#define CUSTOM_UART_BAUD_RATE                                   (9600)


/* Customer define parse once size */
#define CUSTOM_UART_PARSE_ONCE_SIZE                             (256)

/* Note: custmoer need to define own parse function format like ParseFunc(pbuf, count) */
#define CUSTOM_UART_PARSE_FUNC(pbuf, count)    RxDataParser(pbuf, count)


/* Define UART Receive buffer size */
#define UART_RX_BUF_SIZE                                        (512)
/* Define UART Transmit buffer size */
#define UART_TX_BUF_SIZE                                        (512)

/* Note: do not change the size of UART_DMA_SIZE, can not be greater than 127 */
#define UART_DMA_SIZE                                           (128)


#ifdef __cplusplus 
}
#endif

#endif

