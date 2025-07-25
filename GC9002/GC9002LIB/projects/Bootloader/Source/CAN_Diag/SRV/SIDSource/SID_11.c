#ifndef SID_11_C
#define SID_11_C
#include "DiagPlatform.h"
#include<stdlib.h>
#include<string.h>
#include "DiagTbl.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagType.h"
///#include "gc90x_can.h"
#include "SID_11.h"
//#include "AHMIInterface.h"
#include "Repro.h"
//#include "stm32f10x_flash.h"
#include "MidTimer.h"
//#include "LinuxFile.h"
//#include "GUI.h"
//#include "Resource.h"
//#include "LcdConf.h"

#include "upgrade_c1.h"
#include "DiagTimer.h"



extern uint32_t Checkflag;             //æ è¯åé

//extern TIMHandle HAHMIBaseTIM;

void gf_func_SID_11_init ( void )
{	
    return;
}

uint8 gf_func_SID_11_exec ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf ,uint8 run_flag)
{
	/* CANåé?ï¼æåè¿å0ï¼å¤±è´?1 */
    uint8 utv1_nrc;
    uint8 utv1_meslength;
	uint8 utv1_Pid;
    uint8 utv1_subfunc_norep;
	CanFrameType t_TxMessage;


    utv1_meslength = (uint8)tpt_Diag_Rx_Buf->data_length;
    utv1_subfunc_norep =  tpt_Diag_Rx_Buf->Rx_data[1] & 0x80;
    
    utv1_nrc = NO_NRC;
    utv1_Pid = tpt_Diag_Rx_Buf->Rx_data[1] & 0x7F;
	if( utv1_meslength != 2 )
	{
		utv1_nrc = NRC_13;
		return utv1_nrc;
	}
	if( utv1_Pid == 0x01)
	{//ç¡¬ä»¶å¤ä½
		
		if (utv1_subfunc_norep == NOT_SUB_FUNCTION)
		{		
       (void)memset(t_TxMessage.data, 0, 8);
       t_TxMessage.data[0] = 0x02;
       t_TxMessage.data[1] = 0x51;
       t_TxMessage.data[2] = 0x01;
       t_TxMessage.Id = BCM_DIAG_CAN_ID;
				/* ååéè¯å®ååºåå¤ä½ */
       CAN_SendDiagMessage( t_TxMessage.Id , t_TxMessage.data, 8, 0 );
		}
		
		/* Quit CAN/UART Run mode, prepare to Jump to APP */
		UPG_SUBSM_PAS_vCanUartRunQuit();
        
        /* For Geely Requirement */
        Diag_vChangeMainState(0x00);
	}
	else if (utv1_Pid == 0x03)
	{//è½¯ä»¶å¤ä½
		if (utv1_subfunc_norep == NOT_SUB_FUNCTION)
		{		
            (void)memset(t_TxMessage.data, 0, 8);
            t_TxMessage.data[0] = 0x02;
            t_TxMessage.data[1] = 0x51;
            t_TxMessage.data[2] = 0x03;
            t_TxMessage.Id = BCM_DIAG_CAN_ID;
			/* ååéè¯å®ååºåå¤ä½ */
			CAN_SendDiagMessage( t_TxMessage.Id , t_TxMessage.data, 8, 0 );
		}
		/* å¤ä½ */
		//AHMISystemReset();  // to do, by Zhu Jiahai
	}
    else
    {
        utv1_nrc = NRC_12;
		return utv1_nrc;
    }
    
    return utv1_nrc;
}

void gf_func_SID_11_term ( void )
{
    return;
}

#endif

