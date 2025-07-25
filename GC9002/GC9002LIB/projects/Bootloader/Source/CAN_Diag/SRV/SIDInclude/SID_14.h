#ifndef SID_14
#define SID_14

#include "DiagMgr.h"
#include "DiagType.h"
#include "Session.h"
#include "DiagTbl.h"

#define DIAG_14_SID		(uint8)0x14
#define DIAG_14_NRC_INDEX (uint8)0x03
#define  DIAG_14_VAILD_DATA_LENGTH (uint8)0x04

void gf_func_SID_14_init(void );
uint8 gf_func_SID_14_exec(st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType* tpt_Diag_Tx_Buf,uint8 run_flag);
void gf_func_SID_14_term(void);


#endif 
