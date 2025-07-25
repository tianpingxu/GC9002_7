
#ifndef DiagMgr_c
#define DiagMgr_c
#include <stdlib.h>
#include "DiagPlatform.h"
#include "DiagType.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagTbl.h"
#include "DiagTimer.h"
#include "Session.h"
#include "Security.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


extern uint8 usv1_Diag_Receive_status;
extern uint8 usv1_Diag_Transmit_status;

static void sf_service_init( void );
static void sf_service_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf );
static void sf_service_term( void );

st_Diag_RX_BufType *spt_Diag_Rx_Buf;        //接收报文的缓�?
st_Diag_Tx_BufType *spt_Diag_Tx_Buf;        //发�?�报文的缓存


/************************************************************/
/*函数名：sf_service_init                                   */
/*函数作用：诊断服务初始化                                  */
/************************************************************/
static void sf_service_init( void )
{
	uint8 utv1_index;
	
	utv1_index = (uint8)0;
	
	while( st_Diag_Table[utv1_index].func_init != NULL )
	{
		st_Diag_Table[utv1_index].func_init();
		utv1_index++;
	}
	
	return;
}
/************************************************************/
/*函数名：sf_service_exec                                   */
/*函数作用：诊断服务入口程�?                                */
/************************************************************/
static void sf_service_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf )
{
	uint8 utv1_nrc = 0;
	uint8 utv1_sid_index = 0;
	uint8 utv1_index = 0;
	uint8 utv1_hit_flag = 0;
	
	
	utv1_hit_flag =(uint8)0;
	
	if( (tpt_Diag_Rx_Buf->Rx_Result == RX_FINISH) || (tpt_Diag_Rx_Buf->Rx_Result == RX_ERROR) )                                                                        //判断是否接收到完整请�?
	{ 
		/* 接受完再变成初始化状�? */
		tpt_Diag_Rx_Buf->Rx_Result = RX_INIT;

//		utv1_nrc = gf_NRC_7F_check( tpt_Diag_Rx_Buf, tpt_Diag_Tx_Buf );                                                               //判断session是否支持
		
		if( utv1_nrc == NO_NRC )
		{
			for( utv1_index=(uint8)0; (st_Diag_Table[utv1_index].func_exec!= NULL)&&(utv1_hit_flag==(uint8)0); utv1_index++ )   //查找service function
			{
				if( st_Diag_Table[utv1_index].sid == tpt_Diag_Rx_Buf->Rx_data[0] )
				{
					utv1_hit_flag = (uint8)1;
					utv1_sid_index = utv1_index;
				}
			}
			
			if( utv1_hit_flag == (uint8)0 )
			{
				/* 该服务不支持 */
				utv1_nrc = NRC_11;
			}
			/* 如果该服务只支持物理寻址但是请求是功能寻�? */
			if( (st_Diag_Table[utv1_sid_index].can_id == TESTER_DIAG_CAN_ID) && (tpt_Diag_Rx_Buf->Rx_CAN_ID == TESTER_FUNCTION_CAN_ID) )
			{
				utv1_nrc = NRC_11; 
			}
		
			if( utv1_nrc == NO_NRC )
			{
				/* 安全等级�?查，按照由低到高，如果请求安全等级比当前低就可以直接使用 */
				utv1_nrc = SecurityCheck( st_Diag_Table[utv1_sid_index].security_lvl );                                             //确认安全等级是否支持
				
				/* 31 hex的对应请求不�?要安全访问控�? */
				if( (tpt_Diag_Rx_Buf->Rx_data[0] == 0x31) && (tpt_Diag_Rx_Buf->Rx_data[1] == 0x01) && (tpt_Diag_Rx_Buf->Rx_data[2] == 0x02) && (tpt_Diag_Rx_Buf->Rx_data[3] == 0x02) )
				{
					utv1_nrc = NO_NRC;
				}
				
				if( utv1_nrc == NO_NRC )
				{
					utv1_nrc = st_Diag_Table[utv1_sid_index].func_exec( tpt_Diag_Rx_Buf, tpt_Diag_Tx_Buf,Normal_Run );              //执行相应的诊断服�?
				}
			}
		}
	
		/* 注意这个�?定要放最后，因为之前的服务函数是按照子功能的首位决定是否抑制响应的，不管是肯定还是否定，而否定响应即使抑制位�?1也要返回，所以不能放到前�? */
		if( utv1_nrc != NO_NRC )
		{
			/* 这个函数里面已经实现了是否是功能寻址�?要抑制某些否定响�? */
			gf_set_NRC( utv1_nrc, tpt_Diag_Rx_Buf, tpt_Diag_Tx_Buf );                                                               //NRC否定应答
		}
	}
	
	return;
}

static void sf_service_term( void )  //terminal
{
	uint8 utv1_index;
	
	utv1_index = (uint8)0;
	
	while( st_Diag_Table[utv1_index].func_term!= NULL )
	{
		st_Diag_Table[utv1_index].func_term();
		utv1_index++;
	}
	return;
}

/************************************************************/
/*函数名：gf_set_NRC                                        */
/*函数作用：负响应的NRC统一处理                             */
/************************************************************/
void gf_set_NRC( uint8 utv1_NRC, st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf )
{
	tpt_Diag_Tx_Buf->Tx_CAN_ID = (uint32)BCM_DIAG_CAN_ID;
	tpt_Diag_Tx_Buf->data_length = (uint16)0x03;
	tpt_Diag_Tx_Buf->DLC = (uint8)0x08;
	tpt_Diag_Tx_Buf->Data[0] = (uint8)0x7F;
	tpt_Diag_Tx_Buf->Data[1] = tpt_Diag_Rx_Buf->Rx_data[0];
	tpt_Diag_Tx_Buf->Data[2] = utv1_NRC;
	tpt_Diag_Tx_Buf->Tx_Req = NOW_REQUEST;
	
    if( tpt_Diag_Rx_Buf->Rx_CAN_ID == (uint32)TESTER_FUNCTION_CAN_ID )
    {//功能寻址不发送部分负反馈
		if( (utv1_NRC==NRC_11) || (utv1_NRC==NRC_12) || (utv1_NRC==NRC_31) )
		tpt_Diag_Tx_Buf->Tx_Req = NEED_NOT_REQUEST;
    }
	
    return;
}


/************************************************************/
/*函数名：gf_diag_com_INIT                                  */
/*函数作用：诊断系统初始化                                  */
/************************************************************/
void gf_diag_com_INIT( void )
{
    ENTER_CRITICAL;
	spt_Diag_Rx_Buf = gf_Rx_init( );
	spt_Diag_Tx_Buf = gf_Tx_init( );
	gf_Diag_NWL_init( );
	sf_service_init( );
	gf_Session_init( );
    Diag_Timer_vInit();
    CANDIAG_vInit();
    EXIT_CRITICAL;
}


/************************************************************/
/*函数名：gf_diag_com_EXEC                                  */
/*函数作用：诊断系统运�?                                    */
/************************************************************/
void gf_diag_com_EXEC( void )
{
	//Diag_Timer_disable(Diag_AHMITIM6);
	
	gf_Session_Timer ( );                                                            //session 超时处理
	
	/* 初始化一�? */
//	spt_Diag_Rx_Buf = gf_Rx_init( );

	if(spt_Diag_Tx_Buf->Tx_Result != TX_SENDING)
	{
			spt_Diag_Tx_Buf = gf_Tx_init( );
	}
	
	gf_NWL_Rrceive_DiagMess( spt_Diag_Rx_Buf );                                     //ISO15765  传输层的接收
	
	/* 如果不是在接收后续帧的过程中，则�?始超时响�?78错误的定时器计时 */
	if( (usv1_Diag_Receive_status != RX_CF) && (usv1_Diag_Transmit_status != TX_CF) )
	{
		Diag_Timer_update( Diag_TIMER_P2CAN_SERVER );
	}
	
	sf_service_exec( spt_Diag_Rx_Buf, spt_Diag_Tx_Buf );                            //执行相应的诊断服�?
	
	/* 正在接收后续帧时不发送数�? */
	if( usv1_Diag_Receive_status != RX_CF )
	{
		/* 如果是多帧接收状态就不执�? */
		//SID_19_one_cycle_call();                                                        //保存故障现场
		
		gf_NWL_Transmit_DiagMess( spt_Diag_Tx_Buf );                                    //ISO15765  传输层的发�??
		
		/* 已经成功发�?�了，禁止超时响应定时器 */
		Diag_Timer_disable( Diag_TIMER_P2CAN_SERVER );
		set_Diag_Timer_mStopValue( Diag_TIMER_P2CAN_SERVER, CAN_TIMER_P2CAN_SERVER_LEN);
		
	}

	
	if( spt_Diag_Tx_Buf->Tx_Result == TX_ERROR )                                    //传�?�数据超过长度时的处�?
	{
		gf_Diag_NWL_init( );
		spt_Diag_Rx_Buf = gf_Rx_init( );
		spt_Diag_Tx_Buf = gf_Tx_init( );
	}
    
	/* 应该放到�?后，防止之前有�?�时的操作所造成的影响，因为这个5s只与诊断仪有�? */
	gf_Session_Time_update ( );                                                                                             //更新session �?5秒时�?
	
	return;
}

void gf_diag_com_TERM( void )
{
	sf_service_term();

}

#endif

/*********************End Line***********************************/


