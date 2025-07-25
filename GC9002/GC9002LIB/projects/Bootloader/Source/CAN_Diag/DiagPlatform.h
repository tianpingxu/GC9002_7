#ifndef DIAGPLATFORM_H
#define DIAGPLATFORM_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"



#ifdef __cplusplus 
extern "C" {
#endif


#include "gc90xx_hal.h"
#include "cfg_c1.h"


typedef unsigned          char uint8;
typedef unsigned short     int uint16;
typedef unsigned           int uint32;

typedef      					    char sint8;
typedef short					     int sint16;
typedef      					     int sint32;

    
//typedef long BaseType_t;
#define pdFALSE			( ( BaseType_t ) 0 )
#define pdTRUE			( ( BaseType_t ) 1 )
#define pdPASS			( pdTRUE )
#define pdFAIL			( pdFALSE )

#define BOOLE_TRUE	1
#define BOOLE_FALSE	0





#define CAN_BUS1  1          //CAN1
#define CAN_BUS2  2          //CAN2
#define CAN_Id_Standard             ((uint32_t)0x00000000)  /*!< Standard Id */
#define CAN_Id_Extended             ((uint32_t)0x00000004)  /*!< Extended Id */


#define CAN_RX_QUEUE_SIZE                       (100)
#define CAN_TX_QUEUE_SIZE                       (20)

/* Switch for sending MainState cyclical */
#define SWITCH_SEND_MAIN_STATE                  (0)
#define APP_CAN_ID                              (0x531)


#if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
#define CAN_TIMER_P2CAN_SERVER_LEN              (60)  /* 1ms Timer becomes 33ms as erase Flash shutdown Interrupt */
#else
#define CAN_TIMER_P2CAN_SERVER_LEN              (1000)
#endif

/* Synchronization with CFG, if interface changed, only need to redefine macro in .h file */
#define SYN_GET_CAN_RECV_ADDR                   (CFG_Get_can_recv_addr())
#define SYN_GET_CAN_SEND_ADDR                   (CFG_Get_can_send_addr())
#define SYN_GET_CAN_FILTER_ID0                  (CFG_Get_can_filter_id0())
#define SYN_GET_CAN_FILTER_MASK0                (CFG_Get_can_filter_mask0())
#define SYN_GET_CAN_FILTER_ID1                  (CFG_Get_can_filter_id1())
#define SYN_GET_CAN_FILTER_MASK1                (CFG_Get_can_filter_mask1())


#define SYN_GET_CAN_CHANEL                      (CFG_pCfg2CanChannel(CFG_Get_can_channel()))
#define SYN_GET_CAN_BAUD_SLOW                   (CFG_u32Cfg2CanBaudRate(CFG_Get_canfd_baud_slow()))
#define SYN_GET_CAN_BAUD_FAST                   (CFG_u32Cfg2CanBaudRate(CFG_Get_canfd_baud_fast()))
#define SYN_GET_CAN_TX_PORT                     (CFG_pCfg2Port(CFG_Get_can_tx_port()))
#define SYN_GET_CAN_TX_PIN                      (CFG_u32Cfg2Pin(CFG_Get_can_tx_pin()))
#define SYN_GET_CAN_RX_PORT                     (CFG_pCfg2Port(CFG_Get_can_rx_port()))
#define SYN_GET_CAN_RX_PIN                      (CFG_u32Cfg2Pin(CFG_Get_can_rx_pin()))

#define SYN_GET_CAN_IF_TRANSCEIVER_CTRL         (CFG_Get_can_if_transceiver_ctrl())
#define SYN_GET_CAN_TRANSCEIVER_PORT            (CFG_pCfg2Port(CFG_Get_can_transceiver_port()))
#define SYN_GET_CAN_TRANSCEIVER_PIN             (CFG_u32Cfg2Pin(CFG_Get_can_transceiver_pin()))





typedef struct{
	uint32_t	Id;
	uint8_t	data[8];
}CanFrameType;


uint32_t CAN_SendDiagMessage(uint32_t	id, uint8_t* pdata, uint8_t dlc, uint32_t channel);
uint32_t CAN_SendDiagMessageIsr(uint32_t	id, uint8_t* pdata, uint8_t dlc, uint32_t channel);

void CANDIAG_vInit(void);
void CANDIAG_vCanFdHwInit(void);
void CANDIAG_vCanFdHwDeInit(void);
void CANDIAG_vCanRxMainProcess(void * pvParameters);
void CANDIAG_vCanFdHwInit(void);
void HAL_CANFD_RxMsgCallback(CANFD_HandleTypeDef *hcanfd);
void CANDIAG_vCanTransceiverInit(void);



#ifdef __cplusplus 
}
#endif

#endif  //end of DIAGPLATFORM_H

