#ifndef SID_64_h
#define SID_64_h

#include "DiagType.h"
#include "Session.h"

/* server对应的36服务一次最大传送数据长度4096 */
#define MAX_TRANS_BUF_SIZE_64 DIAG_TX_BUF_MAX

void gf_func_SID_64_init( void );
uint8 gf_func_SID_64_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf,uint8 run_flag);
void gf_func_SID_64_term(void);

#endif

