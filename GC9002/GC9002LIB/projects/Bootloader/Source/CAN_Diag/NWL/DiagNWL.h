#include "DiagPlatform.h"
#include "DiagType.h"
#ifndef DiagNWL_h
#define DiagNWL_h

#define BSize (uint8)0          //根据规范
//#define St_min (uint8)0x0A		//10ms的STmin
#define St_min (uint8)0       //XTD Requirement

typedef enum
{
	CTS = 0,
	WT = 1,
	OVFLW = 2
}FLOW_STATE_ENUM_TYPE;

typedef enum
{
	RX_SF,
	RX_CF,
	RX_FC
}RX_STATE_ENUM_TYPE;

typedef enum
{
	TX_SF,
	TX_CF,
	TX_FC
}TX_STATE_ENUM_TYPE;

typedef enum
{
	RX_INIT,
	RX_RECEIVING,
	RX_FINISH,
	RX_ERROR
}RX_PROCESS_ENUM_TYPE;

typedef enum
{
	TX_INIT,
	TX_SENDING,
	TX_FINISH,
	TX_ERROR
}TX_PROCESS_ENUM_TYPE;

typedef enum
{
	REQUEST_NULL,
	NOW_REQUEST,
	NEED_NOT_REQUEST,
	HAVE_NOT_REQUEST
}TxRequestEnumType;

//#define BCM_DIAG_CAN_ID         0x7CF
/* CAN send addr get from CFG */
#define BCM_DIAG_CAN_ID         0x7CF
#define TESTER_DIAG_CAN_ID      0x7C7
#define TESTER_FUNCTION_CAN_ID  0x7DF

void gf_Diag_NWL_init( void );
st_Diag_RX_BufType* gf_Rx_init( void );
st_Diag_Tx_BufType* gf_Tx_init( void );
void gf_NWL_Rrceive_DiagMess( st_Diag_RX_BufType *tpt_Diag_Rx_Buf );
void gf_NWL_Transmit_DiagMess( st_Diag_Tx_BufType *tpt_Diag_Tx_Buf );

#endif
