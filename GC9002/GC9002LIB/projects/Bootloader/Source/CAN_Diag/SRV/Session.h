#ifndef Session_h
#define Session_h
#include "DiagType.h"

typedef enum
{
	DEFAULT_SESSION = 1,
	PROGRAMME_SESSION = 2,
	EXTENDED_SESSION = 4,
}SeesionEnumType;
#ifdef __cplusplus
extern "C" {
#endif
extern uint8 ugv1_session_time_flag;

uint8 gf_program_get(void);
void gf_program_set( uint8 set );

void gf_session_timeout_set ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf );
void gf_Session_init ( void );
void gf_Session_term ( void );
void gf_Session_Timer ( void );
void gf_Session_Time_update ( void );
void gf_session_Transmit ( void );
uint8 gf_NRC_7F_check ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf );
void ugv1_session_change ( uint8 utv1_p_index );
uint8 get_ugv1_session(void);
#ifdef __cplusplus
}
#endif
#endif

