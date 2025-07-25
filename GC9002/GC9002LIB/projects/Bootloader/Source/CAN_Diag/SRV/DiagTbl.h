
#ifndef DiagTbl_h
#define DiagTbl_h
#include "DiagType.h"

#define DIAG_TBL_INEDX          ((uint8)8)
#define DIAG_NRC_TBL_INDEX      ((uint8)2)

typedef void (*func_init_type)(void);        
typedef void (*func_term_type)(void);       
typedef void (*func_NRC_Check_type)( st_Diag_RX_BufType *tpt_Diag_Rx_Buf,st_Diag_Tx_BufType *tpt_Diag_Tx_Buf, uint8 utv1_sid_index,uint8 *utpt_ret_NRC, uint8 utv1_NRC );    
typedef uint8 (*func_exec_type)( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf, uint8 run_flag);

typedef struct
{
    uint8 sid;
    uint32 can_id;
	uint8 session;
	uint8 security_lvl;
    func_init_type func_init;
    func_exec_type func_exec;
    func_term_type func_term;
}st_Diag_TableType;

typedef struct
{
    func_NRC_Check_type func_NRC_Check;
    uint8 NRC;
}st_Diag_NRC_CheckType;

extern const st_Diag_TableType st_Diag_Table[];

#endif

