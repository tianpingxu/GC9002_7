#ifndef SID_10_C
#define SID_10_C

#include "DiagPlatform.h"
#include "DiagTbl.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagType.h"
#include "Session.h"
///#include "gc90x_can.h"
#include "SID_10.h"
#include "string.h"
#include "DiagTimer.h"
#include "upgrade_c1.h"


extern void AHMISystemReset(void);
void sf_NRC_22_check ( void );

void gf_func_SID_10_init ( void )
{
	ugv1_session_change(DEFAULT_SESSION);
    return;
}

uint8 gf_func_SID_10_exec ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf, uint8 run_flag)
{
    uint8 utv1_nrc;
    uint8 utv1_meslength;
    uint8 sub_func;
    uint8 utv1_subfunc_norep;
    uint8 utv1_session_change;
		CanFrameType t_TxMessage;
	
//	/* 测卡死的时�?�看门狗是否起作用会重启 */
//		uint8 i = *(uint8 *)0x30008000;

    utv1_meslength = tpt_Diag_Rx_Buf->data_length;
    utv1_nrc = NO_NRC;
	
	/* 字节数不符合 */
    if ( utv1_meslength != (uint8)2 )
    {
        utv1_nrc = NRC_13;
        return NRC_13;
    }

	/* 子功能ID */
    sub_func = tpt_Diag_Rx_Buf->Rx_data[1] & 0x7f;
	/* 子功能不支持 */
	if( (sub_func != 0x01) && ( sub_func != 0x02 ) && ( sub_func != 0x03 ) )
	{
		utv1_nrc = NRC_12;
		return utv1_nrc;
	}
	
    utv1_subfunc_norep =  tpt_Diag_Rx_Buf->Rx_data[1] & 0x80;
	utv1_session_change = 0;
	if (sub_func == 0x01)
	{
		utv1_session_change = DEFAULT_SESSION;
		/* 如果当前是编程会话，则说明需要ECU重启 */
		if( get_ugv1_session() == PROGRAMME_SESSION )
		{
			if (utv1_subfunc_norep == NOT_SUB_FUNCTION)
			{
				(void)memset(t_TxMessage.data, 0, 8);
				t_TxMessage.data[0] = 0x06;
				t_TxMessage.data[1] = 0x50;
				t_TxMessage.data[2] = 0x01;
				/* 数据场P2CAN_Server = 50 */
				t_TxMessage.data[3] = 0x00;
				t_TxMessage.data[4] = 0x32;
				/* 数据P2CAN*_Server = 5000 */
				t_TxMessage.data[5] = 0x13;
				t_TxMessage.data[6] = 0x88;
				t_TxMessage.Id = BCM_DIAG_CAN_ID;
				/* 先发送肯定响应后复位 */
				CAN_SendDiagMessage( t_TxMessage.Id , t_TxMessage.data, 8, 0 );
			}
			/* 复位 */
			//AHMISystemReset(); // to do by ZhuJiahai
			return utv1_nrc;
		}
	}
	else if (sub_func == 0x02)
	{
//		/* 默认会话不能进入编程会话，扩展会话可�? */
//		if (get_ugv1_session() == DEFAULT_SESSION) 
//		{
//			utv1_nrc = NRC_22;
//			return utv1_nrc;
//		}
//		/* 车�?�大�?20km/h不能进入编程会话 */
//		if(HMIDetail.u8_Speed >= 20)
//		{
//			utv1_nrc = NRC_22;
//			return utv1_nrc;
//		}
//		/* 如果没有经过31 hex的预编译条件的检�? */
//		if( gf_program_get() == BOOLE_FALSE )
//		{
//			utv1_nrc = NRC_22;
//			return utv1_nrc;
//		}
		/* 重新�?查才能再次进入编程会�? */
		gf_program_set( BOOLE_FALSE );
		utv1_session_change = PROGRAMME_SESSION;
        
        /* For Geely Requirement */
        Diag_vChangeMainState(0xAA);

        /* When receive 10 02, we think CAN communication exist */
        UPG_SUBSM_PAS_vCanWork();
	}
	else
	{
		/* 编程会话不能进入扩展会话 */
		if (get_ugv1_session() != PROGRAMME_SESSION)
		{
			utv1_session_change = EXTENDED_SESSION;
		}
		else
		{
			utv1_nrc = NRC_22;
			return utv1_nrc;
		}
	}
	
	utv1_nrc = NO_NRC;
	/* 请求成功，根据第�?位是否为1决定是不是要抑制肯定响应 */
	if (utv1_subfunc_norep == NOT_SUB_FUNCTION)
	{
		(void)memset(t_TxMessage.data, 0, 8);
		t_TxMessage.data[0] = 0x06;
		t_TxMessage.data[1] = 0x50;
		t_TxMessage.data[2] = sub_func;
		t_TxMessage.data[3] = 0x00;
		t_TxMessage.data[4] = 0x32;
		t_TxMessage.data[5] = 0x13;
		t_TxMessage.data[6] = 0x88;
		t_TxMessage.Id = BCM_DIAG_CAN_ID;
		/* 先发送肯定响应后切换会话模式 */
		CAN_SendDiagMessage( t_TxMessage.Id , t_TxMessage.data, 8, 0 );
		/* 防止用网络层再发送一�? */
		tpt_Diag_Tx_Buf->Tx_Req = NEED_NOT_REQUEST;
	}
	else
	{
		tpt_Diag_Tx_Buf->Tx_Req = NEED_NOT_REQUEST;
	}
	
	if( utv1_session_change )
	{
		ugv1_session_change(utv1_session_change);
	}
    
    return utv1_nrc;
}

void gf_func_SID_10_term ( void )
{
	ugv1_session_change(DEFAULT_SESSION);
    return;
}


void gf_func_Respond_10_02(void)
{
    CanFrameType t_TxMessage;
    (void)memset(t_TxMessage.data, 0, 8);
	t_TxMessage.data[0] = 0x06;
	t_TxMessage.data[1] = 0x50;
	t_TxMessage.data[2] = 0x01;
	/* 数据场P2CAN_Server = 50 */
	t_TxMessage.data[3] = 0x00;
	t_TxMessage.data[4] = 0x32;
	/* 数据P2CAN*_Server = 5000 */
	t_TxMessage.data[5] = 0x13;
	t_TxMessage.data[6] = 0x88;
	t_TxMessage.Id = BCM_DIAG_CAN_ID;
	/* 先发送肯定响应后复位 */
	CAN_SendDiagMessage( t_TxMessage.Id , t_TxMessage.data, 8, 0 );
    /* For Geely Requirement */
    Diag_vChangeMainState(0xAA);
}

#endif

