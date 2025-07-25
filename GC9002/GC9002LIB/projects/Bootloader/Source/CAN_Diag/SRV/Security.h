#ifndef SECURITY_H
#define SECURITY_H

#include "DiagType.h"

/* ����İ�ȫ�ȼ������ɵ͵��� */
typedef enum
{
    SECURITY_TYPE_DEFAULT=0,
    SECURITY_TYPE_APP,
    SECURITY_TYPE_BOOT,
    SECURITY_TYPE_END,
}SecurytyLevelEnumType;

extern uint8 SecurityCheck( uint8 t_NeedSecurityLevel );
extern void SetNowSecurityLevel( uint8 t_NowLevel );
extern uint8 GetNowDiagSecurityLevel( void );
#endif
