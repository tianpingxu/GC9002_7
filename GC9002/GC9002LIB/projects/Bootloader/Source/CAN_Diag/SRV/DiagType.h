
#ifndef DiagType_h
#define DiagType_h
#include "DiagPlatform.h"

#define DIAG_TX_BUF_MAX             (512 + 2)
#define DIAG_RX_BUF_MAX             (512 + 2)

typedef struct 
{
    uint8 DLC;
    uint8 Rx_data[DIAG_RX_BUF_MAX];
    uint8 Rx_Result;
    uint32 Rx_CAN_ID;
    uint16 data_length;
}st_Diag_RX_BufType;

typedef struct 
{
    uint32 Tx_CAN_ID;
    uint16 data_length;
    uint8 Data[DIAG_TX_BUF_MAX];
    uint8 Tx_Req;
    uint8 DLC;
    uint8 Tx_Result;
}st_Diag_Tx_BufType;
#endif

