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

uint8 ugv1_Session_status;                                  //��ǰ��session
static uint8 program_con = BOOLE_FALSE;						//�����̻Ự���Ⱦ�����

/* ��ǰ�Ƿ���Խ����̻Ự������������31 hex�ļ��Ԥ��������֮��Ĭ����FALSE�������������ͨ��������ΪTRUE�������ٱ��FALSE */
uint8 gf_program_get()
{
	return program_con;
}

void gf_program_set( uint8 set )
{
	program_con = set;
}

/************************************************************/
/*��������gf_Session_init                                   */
/*�������ã��Ự���ʼ��                                    */
/************************************************************/
void gf_Session_init( void )
{
	ugv1_Session_status = DEFAULT_SESSION;
	Diag_Timer_disable( Diag_TIMER_S3_SERVER );
	/* ���ð�ȫ�ȼ���� */
	SetNowSecurityLevel(SECURITY_TYPE_DEFAULT);
	return;
}


/************************************************************/
/*��������gf_Session_term                                   */
/*�������ã��Ự�����ص�default session��������Ϸ������   */
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
/*��������gf_Session_Timer                                  */
/*�������ã�����Ƿ�ʱ                                    */
/************************************************************/
void gf_Session_Timer ( void )
{
	if ( get_Diag_Timer_flag(Diag_TIMER_S3_SERVER) == 1 )
	{
		/* ��Ҫ���Ƕ�ʱ������ */
		clear_Diag_Timer_flag(Diag_TIMER_S3_SERVER);
		/* �����ǰ�Ǳ�̻Ự�³�ʱ��������ECU */
		if( ugv1_Session_status == PROGRAMME_SESSION )
		{
			//AHMISystemReset();  //to do  by Zhu Jiahai
		}
//		/* ���Խ׶�����û����ô�죬��ע�͵���֮��ȥ��ע�� */
//		ugv1_Session_status = DEFAULT_SESSION ;                         //�ص�default session
//		Diag_Timer_disable( Diag_TIMER_S3_SERVER );

//		/* 27hex��term��Ѱ�ȫ�ȼ�����Ϊ��� */
//		gf_Session_term();                                            //����Ϸ������ִ��
	}

	return;
}


/************************************************************/
/*��������gf_Session_Time_update                            */
/*�������ã��յ����ģ�ˢ�»Ự��ʱ                          */
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
			/* �жϵ�ǰ�Ự�ǲ��Ƿ�����֧�ֵĻỰģʽ�е�һ�� */
			if( ((ugv1_Session_status) & (st_Diag_Table[utv1_p_index].session)) == 0)   
			{
				utv1_nrc = NRC_7F;
			}
		}
	}		
	return utv1_nrc;
}
#endif

