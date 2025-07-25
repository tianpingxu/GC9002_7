#ifndef SID_28_C
#define SID_28_C

#include "DiagTbl.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagType.h"
#include "Session.h"
#include "SID_28.h"

static uint8 Communication_flag = 0; /*00:允许收发,01:只收不发,02:只发不收,03:禁止收发*/

void gf_func_SID_28_init( void )
{
	set_Communication_flag(0);	/*允许收发*/
}

uint8 gf_func_SID_28_exec(st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf,uint8 run_flag)
{
	uint8 utv1_nrc;				/*负响应码*/
	uint8 com_stat;
    uint8 utv1_subfunc_norep;
	uint8 utv1_meslength;
	uint8 com_type;

	utv1_nrc = NO_NRC;

	utv1_meslength = (uint8)tpt_Diag_Rx_Buf->data_length;
	com_stat = tpt_Diag_Rx_Buf -> Rx_data[1] & 0x7F; /*取得请求的BCM通讯状态*/
	utv1_subfunc_norep = tpt_Diag_Rx_Buf -> Rx_data[1] & 0x80;
	/* 需要控制的通讯类型，只需要后两位 */
	com_type = tpt_Diag_Rx_Buf -> Rx_data[2] & 0x03;
	
	/* 长度为3 */
	if( utv1_meslength != (uint8)3 )
	{
    	utv1_nrc = NRC_13;
		return utv1_nrc;
	}
	/* 子功能只支持00和03 */
	if( com_stat != 0 && com_stat != 3 )
	{
		utv1_nrc = NRC_12;
		return utv1_nrc;
	}
	/* 后期如果有需要可以加上一个条件的判断 */
	if(0)
	{
		utv1_nrc = NRC_22;
		return utv1_nrc;
	}
	/* 通讯类型支持03，网络管理报文和应用报文 */
	if( com_type != 0x03 )
	{
		utv1_nrc = NRC_31;
		return utv1_nrc;
	}
	set_Communication_flag(com_stat);	/*设置通讯标志位*/
	
	if (utv1_subfunc_norep == NOT_SUB_FUNCTION)
	{
		tpt_Diag_Tx_Buf->Tx_CAN_ID = (uint32)BCM_DIAG_CAN_ID;
		tpt_Diag_Tx_Buf->data_length = (uint8)0x02; /*有效数据长度*/
		tpt_Diag_Tx_Buf->DLC = (uint8)0x08;	        /*报文长度*/
		tpt_Diag_Tx_Buf->Data[0] = (uint8)0x68;	    /*正响应服务标识符*/
		tpt_Diag_Tx_Buf->Data[1] = com_stat;	
		tpt_Diag_Tx_Buf->Tx_Req = NOW_REQUEST;
	}
	else
	{
		tpt_Diag_Tx_Buf->Tx_Req = NEED_NOT_REQUEST;
	}
    
	return utv1_nrc;
}

void gf_func_SID_28_term( void )
{
	set_Communication_flag (0);
}

void set_Communication_flag(uint8 value) /*0,1,2,3*/
{	/*这里不再范围检测*/
	Communication_flag = value;
}

uint8  get_Communication_flag(void)
{
	return Communication_flag;
}

#endif
