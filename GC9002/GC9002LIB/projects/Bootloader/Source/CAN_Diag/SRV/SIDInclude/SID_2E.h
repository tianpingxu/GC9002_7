#ifndef SID_2E_h
#define SID_2E_h

#include "DiagType.h"
#include "Session.h"


void gf_func_SID_2E_init( void );
uint8 gf_func_SID_2E_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf, uint8 run_flag);
void gf_func_SID_2E_term(void);
uint8 check_data_valid( st_Diag_RX_BufType *tpt_Diag_Rx_Buf );
void logicblock_update( void );
/*
typedef struct
{
    uint8 pid;
    uint8 length;
	uint8 session_num;
}st_Session_PID_TableType;
*/
#endif

