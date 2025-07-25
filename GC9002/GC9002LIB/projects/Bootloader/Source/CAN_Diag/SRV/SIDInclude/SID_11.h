#ifndef SID_11_h
#define SID_11_h

#include "DiagType.h"
#include "Session.h"

void gf_func_SID_11_init( void );
uint8 gf_func_SID_11_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf,uint8 run_flag);
void gf_func_SID_11_term(void);

#endif

