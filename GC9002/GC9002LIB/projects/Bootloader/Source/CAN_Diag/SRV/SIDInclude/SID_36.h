#ifndef SID_36_H
#define SID_36_H
#include "DiagPlatform.h"
#include "DiagType.h"
//void SetNowReproStartAdress( uint32 t_StartAddress );
void SetNowReproStartAdress( uint32 t_StartAddress, uint32 t_size);
void SetNowReproStartAdressRep( uint32 t_StartAddress );
uint8 gf_func_SID_36_exec ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf ,uint8 run_flag);
void gf_func_SID_36_init ( void );
void gf_func_SID_36_term ( void );
void counter_init( void );
void counter_update( void );
void Init_CRC_Value(void);
uint32 Get_CRC_Value(void);
#endif
