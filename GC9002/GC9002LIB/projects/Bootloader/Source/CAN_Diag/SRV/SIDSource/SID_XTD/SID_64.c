#include <string.h>
#include "DiagPlatform.h"
#include "DiagTbl.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagType.h"
#include "Session.h"
#include "SID_36.h"
#include "Repro.h"
#include "SID_64.h"
#include "upgrade_c1.h"


/* 64æå¡çæ°æ®é¿åº? */
#define DATA_LENGTH_64 11
/* è¯·æ±æ°æ®çæ ¼å¼ID */
#define DATA_FORMAT_ID_64 0x00
/* å°ååé¿åº¦çæ ¼å¼ID */
#define ADDR_LEN_FORMAT_ID_64 0x44

static uint8 CheckReproCondition( void );

//#pragma CODE_SEG PAGED_ROM

void gf_func_SID_64_init ( void )
{
    return;
}

uint8 gf_func_SID_64_exec ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf ,uint8 run_flag)
{
    uint8 utv1_nrc;
	
    uint8 utv1_meslength;
	uint8 u8DataFormatId;
    uint8 u8AddrLenId;
	uint32 t_ReproStartAddress;
	uint32 t_ReproSize;
	
//	uint32 i;
//	uint32* ptr;
	int32_t res;

    utv1_meslength = tpt_Diag_Rx_Buf->data_length;
    u8DataFormatId = tpt_Diag_Rx_Buf->Rx_data[1];
    u8AddrLenId = tpt_Diag_Rx_Buf->Rx_data[2];
    utv1_nrc = NO_NRC;
    
	/* è¿å¥ç­å¾è¯·æ±ä¸è½½çç¶æ? */
	SetReroStage( REPRO_REQUEST_DOWNLOAD_DRIVER );
	/* é¿åº¦éè¯¯ */
    if( utv1_meslength != DATA_LENGTH_64 )
    {
		utv1_nrc = NRC_13;
		return utv1_nrc;
	}
	/* æ ¼å¼ä¸å¯¹ */
	if((u8DataFormatId != DATA_FORMAT_ID_64) || (u8AddrLenId != ADDR_LEN_FORMAT_ID_64))
	{
		utv1_nrc = NRC_31;
		return utv1_nrc;
	}
	/* æ¡ä»¶ä¸æ»¡è¶? */
	utv1_nrc = CheckReproCondition();
	if( utv1_nrc != NO_NRC )
	{
		return utv1_nrc;
	}
	/* å¾å°æ°æ® */
	t_ReproStartAddress = tpt_Diag_Rx_Buf->Rx_data[3];
	t_ReproStartAddress = tpt_Diag_Rx_Buf->Rx_data[4]|(t_ReproStartAddress<<8);
	t_ReproStartAddress = tpt_Diag_Rx_Buf->Rx_data[5]|(t_ReproStartAddress<<8);
	t_ReproStartAddress = tpt_Diag_Rx_Buf->Rx_data[6]|(t_ReproStartAddress<<8);
	t_ReproSize = tpt_Diag_Rx_Buf->Rx_data[7];
	t_ReproSize = tpt_Diag_Rx_Buf->Rx_data[8]|(t_ReproSize<<8);
	t_ReproSize = tpt_Diag_Rx_Buf->Rx_data[9]|(t_ReproSize<<8);
	t_ReproSize = tpt_Diag_Rx_Buf->Rx_data[10]|(t_ReproSize<<8);
	
	/* å°è¦ä¸è½½çèµ·å§å°å?åsizeä¿å­å¨Reproçç¸å³åéä¸­ */
    res = UPG_PAS_i32SetUpgFileAndSize((uint8_t *) t_ReproStartAddress, t_ReproSize);
    if(res != 0)
    {
        utv1_nrc = NRC_13;
		return utv1_nrc;
    }
	
	tpt_Diag_Tx_Buf->Tx_CAN_ID = (uint32)BCM_DIAG_CAN_ID;
	tpt_Diag_Tx_Buf->data_length = 4;
	tpt_Diag_Tx_Buf->DLC = (uint8)0x08;
	tpt_Diag_Tx_Buf->Data[0] = (uint8)0xA4;  /* 0x64 + 0x40 = 0xA4 */
	tpt_Diag_Tx_Buf->Data[1] = 0x20;
	tpt_Diag_Tx_Buf->Data[2] = ((MAX_TRANS_BUF_SIZE_64)>>8);
	tpt_Diag_Tx_Buf->Data[3] = (uint8)(MAX_TRANS_BUF_SIZE_64);
	tpt_Diag_Tx_Buf->Tx_Req = NOW_REQUEST;
	
	/* å°ç¶æåä¸ºæ°æ®ä¼ é? */
	SetReroStage(REPRO_TRANSFERDATA_DRIVER);

	
	/* è¯·æ±æåä¹åå°±æ36çcounteråå§åä¸º1 */
	counter_init();
	
    Init_CRC_Value();
    return utv1_nrc;
}

static uint8 CheckReproCondition( void )
{
	uint8 t_RetData;
	
	t_RetData = NO_NRC;
	
	return t_RetData;
}

void gf_func_SID_64_term ( void )
{
    return;
}





