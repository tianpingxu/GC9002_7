#include "DiagMgr.h"
#include "Security.h"

uint8 NowDiagSecurityLevel;   //��ǰ���ϵͳ�İ�ȫ�ȼ�

/************************************************************/
/*��������GetNowDiagSecurityLevel                           */
/*�������ã�ȡ�õ�ǰ�����ϵͳ��ȫ�ȼ�                      */
/************************************************************/
uint8 GetNowDiagSecurityLevel( void )
{
	return NowDiagSecurityLevel;
	
}

/************************************************************/
/*��������SetNowSecurityLevel                               */
/*�������ã����õ�ǰ�����ϵͳ��ȫ�ȼ�                      */
/************************************************************/
void SetNowSecurityLevel( uint8 t_NowLevel )
{
	NowDiagSecurityLevel = t_NowLevel;
	return;
}

/************************************************************/
/*��������SetNowSecurityLevel                               */
/*�������ã����õ�ǰ�����ϵͳ��ȫ�ȼ�                      */
/************************************************************/
uint8 SecurityCheck( uint8 t_NeedSecurityLevel )
{
	uint8 t_RetData;
	
	t_RetData = NO_NRC;
	
	/* ԭ�����ж��Ƿ���ȣ����ﰴ�����ȼ���С�ĸ������ķ������ȼ������ڵ�ǰ���ܽ��� */
	/* ����0x10��0x27�������ܱ���ֹ�ķ���Ӧ������Ϊ������ȼ� */
	if( t_NeedSecurityLevel > NowDiagSecurityLevel )
	{
		/* ��ȫ�ȼ���ƥ����33���� */
		t_RetData = NRC_33;
	}
	
	return t_RetData;
	
}
