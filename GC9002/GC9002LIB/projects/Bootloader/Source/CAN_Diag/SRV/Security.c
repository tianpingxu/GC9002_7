#include "DiagMgr.h"
#include "Security.h"

uint8 NowDiagSecurityLevel;   //当前诊断系统的安全等级

/************************************************************/
/*函数名：GetNowDiagSecurityLevel                           */
/*函数作用：取得当前的诊断系统安全等级                      */
/************************************************************/
uint8 GetNowDiagSecurityLevel( void )
{
	return NowDiagSecurityLevel;
	
}

/************************************************************/
/*函数名：SetNowSecurityLevel                               */
/*函数作用：设置当前的诊断系统安全等级                      */
/************************************************************/
void SetNowSecurityLevel( uint8 t_NowLevel )
{
	NowDiagSecurityLevel = t_NowLevel;
	return;
}

/************************************************************/
/*函数名：SetNowSecurityLevel                               */
/*函数作用：设置当前的诊断系统安全等级                      */
/************************************************************/
uint8 SecurityCheck( uint8 t_NeedSecurityLevel )
{
	uint8 t_RetData;
	
	t_RetData = NO_NRC;
	
	/* 原先是判断是否相等，这里按照优先级大小的概念，请求的服务优先级不大于当前才能进行 */
	/* 所以0x10和0x27这样不能被阻止的服务应该设置为最低优先级 */
	if( t_NeedSecurityLevel > NowDiagSecurityLevel )
	{
		/* 安全等级不匹配是33错误 */
		t_RetData = NRC_33;
	}
	
	return t_RetData;
	
}
