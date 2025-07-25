#ifndef __UPGRADE_UART_PLATFORM_H
#define __UPGRADE_UART_PLATFORM_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#include "upgrade_ct.h"
#include "upgrade_ci.h"
#include "upgrade_ce.h"
#include "csi_irq.h"
#include "cfg_c1.h"


#define UART_RX_QUEUE_SIZE                                  (3)

#define UPG_UART_HEAD_A_SIZE                                (1)
#define UPG_UART_HEAD_H_SIZE                                (1)

#define UPG_UART_HEAD_SIZE                                  (2)
#define UPG_UART_CTRL_SIZE                                  (2)
#define UPG_UART_CMD_SIZE                                   (2)

#define UPG_UART_TAIL_SIZE                                  (2)

#define UPG_UART_OTHER_SIZE                                 (UPG_UART_HEAD_SIZE + UPG_UART_CTRL_SIZE + UPG_UART_TAIL_SIZE)

#define UPG_UART_HEAD_OFFSET                                (0)
#define UPG_UART_HEAD_A_OFFSET                              (UPG_UART_HEAD_OFFSET)
#define UPG_UART_HEAD_H_OFFSET                              (UPG_UART_HEAD_A_OFFSET + UPG_UART_HEAD_A_SIZE)

#define UPG_UART_CTRL_OFFSET                                (UPG_UART_HEAD_OFFSET + UPG_UART_HEAD_SIZE)
#define UPG_UART_CMD_OFFSET                                 (UPG_UART_CTRL_OFFSET + UPG_UART_CTRL_SIZE)
#define UPG_UART_CONTENT_OFFSET                             (UPG_UART_CMD_OFFSET + UPG_UART_CMD_SIZE)

/* Note: This Tail Offset calculated from CMD Offset, as the size contain CMD */
#define UPG_UART_TAIL_OFFSET(x)                             (UPG_UART_CMD_OFFSET + (x))

#define UPG_UART_DOWNLOAD_PARA_SIZE                         (4)

#define UPG_UART_DOWNLOAD_DATA_OFFSET                       (UPG_UART_DOWNLOAD_PARA_SIZE + UPG_UART_CMD_SIZE)

/***** Important: config the baudrate of UART *****/
//#define UPG_UART_BAUD_RATE                                  (115200) /* currently not support high baudrate like 115200 */

/***** Important: config the max pure data length of one frame *****/
#define UPG_UART_DOWNLOAD_MAX_DATA_LEN                      (64)

/***** Important: config the receive buffer size used in ISR, for receiving one frame data from Upper computer *****/
#define UPG_UART_RECV_ISR_BUFFER_SIZE                       (100)

/***** Important: config the send buffer size used in both ISR and Task, for sending one frame data to Upper compute *****/
#define UPG_UART_SEND_BUFFER_SIZE                           (100)

#define UPG_UART_DOWNLOAD_MAX_DATA_LEN_WITH_PARA            (UPG_UART_DOWNLOAD_MAX_DATA_LEN + UPG_UART_DOWNLOAD_PARA_SIZE)

/* Max content length is download CMD: CMD size + Offset + n(data) */
#define UPG_UART_DOWNLOAD_MAX_CONTENT_LEN                   (UPG_UART_CMD_SIZE + UPG_UART_DOWNLOAD_MAX_DATA_LEN_WITH_PARA)


#define UPG_CMD_SHAKE_HAND                                  (0x0080)
#define UPG_CMD_REQ_DOWNLOAD                                (0x0081)
#define UPG_CMD_DOWNLOAD                                    (0x0082)
#define UPG_CMD_UPLOAD                                      (0x0083)
#define UPG_CMD_DOWNLOAD_END                                (0x008F)

#define UPG_REPLY_OFFSET                                    (0x40)    
#define UPG_REPLY_DELAY_FLAG                                (0xFFFFFFFF)

/* UART use MSB to transport data */
#define UPG_UART_MSB                                        (1)
#if(UPG_UART_MSB)
#define SWAP_UINT16(x)                                      ((((uint16_t)x) >> 8) | ((((uint16_t)x) << 8)))
#define SWAP_UINT32(x)                                      ((((uint32_t)x) >> 24) | ((((uint32_t)x) & 0x00FF0000) >> 8) | ((((uint32_t)x) & 0x0000FF00) << 8) | (((uint32_t)x) << 24))
#else
#define SWAP_UINT16(x)                                      (x)
#define SWAP_UINT32(x)                                      (x)
#endif

#define TIMER_WAIT_DEAL_FLASH_DONE_LEN                      (30 * SECOND)

#define UPG_UART_SET_TIMER()                                SRV_TIMER_i32SetTimer(SRV_TIMER_enTimerWaitUartDone, TIMER_WAIT_DEAL_FLASH_DONE_LEN, UPG_UART_vTimerCallback)
#define UPG_UART_STOP_TIMER()                               SRV_TIMER_i32StopTimer(SRV_TIMER_enTimerWaitUartDone)

/* Enter critical status */
#define UPG_UART_ENTER_CRITICAL                             csi_irq_save

/* exit critical status */
#define UPG_UART_EXIT_CRITICAL                              csi_irq_restore


/* Synchronization with CFG, if interface changed, only need to redefine macro in .h file */
#define SYN_GET_UART_CHANEL                                 (CFG_pCfg2UartChannel(CFG_Get_uart_channel()))
#define SYN_GET_UART_BAUD                                   (CFG_u32Cfg2UartBaudRate(CFG_Get_uart_baud()))
#define SYN_GET_UART_TX_PORT                                (CFG_pCfg2Port(CFG_Get_uart_tx_port()))
#define SYN_GET_UART_TX_PIN                                 (CFG_u32Cfg2Pin(CFG_Get_uart_tx_pin()))
#define SYN_GET_UART_RX_PORT                                (CFG_pCfg2Port(CFG_Get_uart_rx_port()))
#define SYN_GET_UART_RX_PIN                                 (CFG_u32Cfg2Pin(CFG_Get_uart_rx_pin()))


typedef enum
{
    UPG_enUartRecvSM_Idle = 0,
    UPG_enUartRecvSM_Head_A,
    UPG_enUartRecvSM_Head_H,
    UPG_enUartRecvSM_Ctrl,
    UPG_enUartRecvSM_CmdVsContent,
    UPG_enUartRecvSM_Tail,
    UPG_enUartRecvSM_Invalid
	
}UPG_tenUartRecvSM;

typedef union
{
    volatile uint16_t u16Val;
    struct{
        uint16_t len    : 11;
        uint16_t res    : 4;
        uint16_t crcEn  : 1;

    }bits;
}UPG_tunUartCrtl, *UPG_tunUartCrtlPtr;


typedef struct 
{	

    bool                            bUartInited;                                    /* Uart initialization status */
    UPG_tenUartRecvSM               enUartRecvSm;                                   /* current Receive State Machine */
    uint16_t                        u16RecvCmdVsContentLen;                         /* Received CMD and Content length */
    uint16_t                        u16CurCmd;                                      /* Current CMD , used for timeout reply */
    uint16_t                        u16LastOffset;                                  /* Saving last Offset */
	uint8_t*                        pu8RecvBufStartAddress;                         /* start address of Receive buffer */
    uint8_t*                        pu8SendBufStartAddress;                         /* start address of Send buffer */
}UPG_tstUartWorkBuf, *UPG_tstUartWorkBufPtr;


typedef struct 
{	
    uint32_t                        u32Addr;                                        /* Flash Address to download */
    uint32_t                        u32FileSize;                                    /* File size */
}UPG_tstUartContentReqDownload;


typedef struct 
{	
    uint32_t                        u32Offset;                                      /* Offset of the start of burn Flash Address */
    uint8_t                         au8Data[UPG_UART_DOWNLOAD_MAX_DATA_LEN];        /* Upgrade data */
}UPG_tstUartContentDownload;

typedef union
{
    UPG_tstUartContentReqDownload   stContentReqDownload;
    UPG_tstUartContentDownload      stContentDownload;
    
}UPG_tunUartContent;


typedef struct 
{	
    UPG_tunUartCrtl                 unCtrl;                                         /* Ctrl word */
    uint16_t                        u16Cmd;                                         /* Cmd */
	UPG_tunUartContent              unUartContent;                                  /* Cmd Content */

}UPG_tstUartCmd, *UPG_tstUartCmdPtr;


typedef union
{
    uint32_t                        u32MaxPackageSize;                              /* CMD 0x0080 reply the Max package(pure data) size */
    uint32_t                        u32Res;                                         /* reply the process result */
}UPG_tunUartRplContent;


typedef struct
{
    UPG_tunUartCrtl                 unCtrl;                                         /* Ctrl word */
    uint16_t                        u16RplCmd;                                      /* Reply Cmd */
    UPG_tunUartRplContent           unRplContent;                                   /* reply Content */

}UPG_tstUartRpl, *UPG_tstUartRplPtr;



void UPG_UART_vInit(void);
void UPG_UART_vUartHwInit(void);
void UPG_UART_vUartHwDeInit(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void UPG_UART_vUartRxMainProcess(void * pvParameters);
void UPG_UART_vParseCmd(UPG_tstUartCmdPtr pCmd);
void UPG_UART_vTimerCallback(void);
void UPG_UART_vStartReceive(void);



#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_UART_PLATFORM_H */




