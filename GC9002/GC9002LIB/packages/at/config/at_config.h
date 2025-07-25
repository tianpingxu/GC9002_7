#ifndef __AT_CONFIG_H__
#define __AT_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rtconfig.h"
#include "csi_config.h"
#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)

#define AT_DEVICE_USING_ECR6600 0U
#define AT_DEVICE_USING_E103W11 1U


#define AT_SW_VERSION_NUM       0x10301

#define AT_USING_CLI            0U
#define AT_USING_SERVER         0U
#define AT_USING_CLIENT         1U
#define AT_USING_SOCKET         1U

#define AT_PRINT_RAW_CMD        1U

#define AT_DEVICE_RX_BUFFER_SIZE        2048
#define AT_DEVICE_RX_DMA_BUFFER_SIZE    127//Maximum DMA block transfers is 127 bytes

#define FINSH_CMD_SIZE          80
#define AT_DEVICE_RX_USING_DMA  (0U)
#define AT_DEVICE_TX_USING_DMA  (0U)
#if(defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1U)
    #define AT_DEVICE_TX_USING_INT  (1U)//If AT_DEVICE_TX_USING_DMA = 1,AT_DEVICE_TX_USING_INT must set 1
#else
    #define AT_DEVICE_TX_USING_INT  (0U)
#endif

#define AT_DEVICE_DMA_RXHS      DMA_HS_UART1_RX
#define AT_DEVICE_DMA_TXHS      DMA_HS_UART1_TX

#define AT_DEVICE_UART_IDX      1//UART1 
#define AT_DEVICE_BAUDRATE      115200
#define AT_DEVICE_DATA_BITS     UART_DATA_BITS_8
#define AT_DEVICE_STOPBITS      UART_STOP_BITS_1
#define AT_DEVICE_PARITY        CSI_UART_PARITY_NONE
#define AT_DEVICE_MODE          UART_MODE_TX_RX
#define AT_DEVICE_HWCONTROL     UART_HWCONTROL_NONE

#define AT_SAMPLE_DEIVCE_NAME   "ecr6600"

#endif /* CONFIG_AT_CLIENT_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* __AT_CONFIG_H__ */