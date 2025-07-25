#ifndef SID_34_h
#define SID_34_h

#include "DiagType.h"
#include "Session.h"

/* server对应的36服务一次最大传送数据长度4096 */
#define MAX_TRANS_BUF_SIZE 0x0800

void gf_func_SID_34_init( void );
uint8 gf_func_SID_34_exec( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf,uint8 run_flag);
void gf_func_SID_34_term(void);

#endif

