#ifndef SID_28_h
#define SID_28_h

#include "DiagType.h"
#include "Session.h"
//#include "AutoEle_Comm.h"
#include "DiagMgr.h"

#define ENABLE_RXTX 0
#define DISABLE_RXTX 3

/********************************º¯ÊýÉùÃ÷********************************/
void gf_func_SID_28_init( void );
uint8 gf_func_SID_28_exec(st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf,uint8 run_flag);
void gf_func_SID_28_term( void );
void set_Communication_flag(uint8 value);
uint8  get_Communication_flag(void);

#endif
