#ifndef SID_10_h
#define SID_10_h


#include "DiagType.h"
#include "Session.h"



void gf_func_SID_10_init( void );
uint8 gf_func_SID_10_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf, uint8 run_flag);
void gf_func_SID_10_term(void);



#endif

