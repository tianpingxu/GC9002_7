#ifndef SID_31_H
#define SID_31_H
#include "DiagPlatform.h"
#include "DiagType.h"

#define CHECK_PROGRAMMING_INTEGRITY     0x0201
#define CHECK_PROGRAMMING_PRECONDITIONS 0x0202
#define ERASE_MEMORY                    0xFF00
#define CHECK_PROGRAMMING_DEPENDENCIES  0xFF01

uint8 gf_func_SID_31_exec ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf ,uint8 run_flag);
void gf_func_SID_31_init ( void );
void gf_func_SID_31_term ( void );
uint8 Start_Routine(st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf);
uint8 Request_Routine_Results(st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf);

#endif
