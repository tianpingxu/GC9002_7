#ifndef Session_C
#define Session_C
#include <stdlib.h>
#include "DiagPlatform.h"
#include "DiagTbl.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagType.h"
#include "DiagTimer.h"
#include "Session.h"
#include "Security.h"

uint8 ugv1_Session_status;                                  //当前的session
static uint8 program_con = BOOLE_FALSE;						//进入编程会话的先决条件

/* 当前是否可以进入编程会话的条件，用在31 hex的检查预编译条件之后，默认是FALSE，编译条件检查通过就设置为TRUE，用完再变成FALSE */
uint8 gf_program_get()
{
	return program_con;
}

void gf_program_set( uint8 set )
{
	program_con = set;
}

/************************************************************/
/*函数名：gf_Session_init                                   */
/*函数作用：会话层初始化                                    */
/************************************************************/
void gf_Session_init( void )
{
	ugv1_Session_status = DEFAULT_SESSION;
	Diag_Timer_disable( Diag_TIMER_S3_SERVER );
	/* 设置安全等级最低 */
	SetNowSecurityLevel(SECURITY_TYPE_DEFAULT);
	return;
}


/************************************************************/
/*函数名：gf_Session_term                                   */
/*函数作用：会话结束回到default session，所有诊断服务结束   */
/************************************************************/
void gf_Session_term ( void )
{
    uint8 utv1_index;
	
	for( utv1_index = 0; st_Diag_Table[utv1_index].func_term != NULL; utv1_index++ )
	{
		st_Diag_Table[utv1_index].func_term();
	}
	return;
}


/************************************************************/
/*函数名：gf_Session_Timer                                  */
/*函数作用：检查是否超时                                    */
/************************************************************/
void gf_Session_Timer ( void )
{
	if ( get_Diag_Timer_flag(Diag_TIMER_S3_SERVER) == 1 )
	{
		/* 不要忘记定时器清零 */
		clear_Diag_Timer_flag(Diag_TIMER_S3_SERVER);
		/* 如果当前是编程会话下超时，则重启ECU */
		if( ugv1_Session_status == PROGRAMME_SESSION )
		{
			//AHMISystemReset();  //to do  by Zhu Jiahai
		}
//		/* 测试阶段手速没有那么快，先注释掉，之后去掉注释 */
//		ugv1_Session_status = DEFAULT_SESSION ;                         //回到default session
//		Diag_Timer_disable( Diag_TIMER_S3_SERVER );

//		/* 27hex的term会把安全等级设置为最低 */
//		gf_Session_term();                                            //各诊断服务结束执行
	}

	return;
}


/************************************************************/
/*函数名：gf_Session_Time_update                            */
/*函数作用：收到报文，刷新会话计时                          */
/************************************************************/
void gf_Session_Time_update ( void )
{
	if ( ugv1_Session_status != DEFAULT_SESSION )
	{
		Diag_Timer_update( Diag_TIMER_S3_SERVER );	
	}
	return;
}


void ugv1_session_change ( uint8 utv1_p_index )
{
	if( utv1_p_index == DEFAULT_SESSION )
	{
		ugv1_Session_status = DEFAULT_SESSION;
		Diag_Timer_disable( Diag_TIMER_S3_SERVER );
	}
	else if( utv1_p_index == EXTENDED_SESSION )
	{
		ugv1_Session_status = EXTENDED_SESSION;
		gf_Session_Time_update();
	}
    else if (utv1_p_index == PROGRAMME_SESSION)
    {
        ugv1_Session_status = PROGRAMME_SESSION;
		gf_Session_Time_update();
    }

	return ;
}

uint8 get_ugv1_session(void)
{
    return ugv1_Session_status;
}

uint8 gf_NRC_7F_check ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf )
{
  uint8 utv1_p_index;
	uint8 utv1_nrc;
	
	utv1_nrc = NO_NRC;
	for ( utv1_p_index = (uint8)0; st_Diag_Table[utv1_p_index].func_exec != NULL; utv1_p_index++ )
	{
		if ( st_Diag_Table[utv1_p_index].sid == tpt_Diag_Rx_Buf->Rx_data[0] )
		{
			/* 判断当前会话是不是服务所支持的会话模式中的一种 */
			if( ((ugv1_Session_status) & (st_Diag_Table[utv1_p_index].session)) == 0)   
			{
				utv1_nrc = NRC_7F;
			}
		}
	}		
	return utv1_nrc;
}
#endif

