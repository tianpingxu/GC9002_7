#ifndef SID_67_C
#define SID_67_C
#include <string.h>
#include "DiagPlatform.h"
#include "DiagTbl.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagType.h"
#include "Session.h"
#include "Repro.h"
#include "SID_64.h"
#include "SID_67.h"
#include "upgrade_c1.h"


extern uint32 g_ProgramCRC_value;

void gf_func_SID_67_init ( void )
{
    return;
}

uint8 gf_func_SID_67_exec ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf ,uint8 run_flag )
{
    uint8 utv1_nrc;
    uint8 utv1_meslength;
	uint8 stage;
	uint32 crc;
        
    utv1_nrc = NO_NRC;
    utv1_meslength = tpt_Diag_Rx_Buf->data_length;
	stage = GetReproStage();
	
	/* 长度错误 */
	if( utv1_meslength != 1 )
	{
		utv1_nrc = NRC_13;
		return utv1_nrc;
	}
	/* 数据传输都没有打�? */
	/* 24错误还可以加入其他的判断条件 */
	if( (stage!=REPRO_REQUEST_DOWNLOAD_DRIVER) && (stage!=REPRO_TRANSFERDATA_DRIVER) )
	{
		utv1_nrc = NRC_24;
		return utv1_nrc;
	}
	/* 接收到这个请求的时�?�数据还没有完全传完，具体实现在Repro�? */
//	if( GetDownloadSize() != 0 )
//	{
//		utv1_nrc = NRC_24;
//		return utv1_nrc;
//	}
    /* Currently only CPU0.bin need verification after burning flash */
    if(0 != UPG_PAS_i32VerifyFlash())
    {
		utv1_nrc = NRC_24;
		return utv1_nrc;
	}
	memset(&tpt_Diag_Tx_Buf->Data[0], 0xAA, 8);
    
    crc = g_ProgramCRC_value ^ 0xFFFFFFFFF;
    
	tpt_Diag_Tx_Buf->Tx_CAN_ID = (uint32)BCM_DIAG_CAN_ID;
	tpt_Diag_Tx_Buf->data_length = (uint8)0x05;             
	tpt_Diag_Tx_Buf->DLC = (uint8)0x08;	                    
	tpt_Diag_Tx_Buf->Data[0] = (uint8)0xA7;
    tpt_Diag_Tx_Buf->Data[1] = (uint8)((crc >> 24) & 0xFF);
    tpt_Diag_Tx_Buf->Data[2] = (uint8)((crc >> 16) & 0xFF);
    tpt_Diag_Tx_Buf->Data[3] = (uint8)((crc >> 8) & 0xFF);
    tpt_Diag_Tx_Buf->Data[4] = (uint8)((crc) & 0xFF);
 
	tpt_Diag_Tx_Buf->Tx_Req = NOW_REQUEST;

	/* 改变状�?? */
	SetReroStage( REPRO_HAS_NOT_STARTED );
	
    return utv1_nrc;
}

void gf_func_SID_67_term ( void )
{
    return;
}

#endif

