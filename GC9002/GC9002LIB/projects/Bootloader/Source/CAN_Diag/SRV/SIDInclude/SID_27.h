#ifndef SID_27_h
#define SID_27_h


#include "DiagType.h"
#include "Session.h"



void gf_func_SID_27_init( void );
uint8 gf_func_SID_27_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf,uint8 run_flag);
void gf_func_SID_27_term(void);
uint32 GetRandSeed(void);




#endif

