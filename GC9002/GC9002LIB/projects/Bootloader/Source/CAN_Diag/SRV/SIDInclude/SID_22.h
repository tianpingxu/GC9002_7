#ifndef SID_22_h
#define SID_22_h

#include "DiagType.h"
#include "Session.h"
#include "DiagMgr.h"

/********************************º¯ÊýÉùÃ÷********************************/
void gf_func_SID_22_init( void );
uint8 gf_func_SID_22_exec(st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf,uint8 run_flag);
void gf_func_SID_22_term( void );

#endif
