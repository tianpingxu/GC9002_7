
#ifndef DiagMgr_h
#define DiagMgr_h
#include "DiagType.h"

#define NO_NRC (uint8)0
#define NRC_11 (uint8)0x11
#define NRC_12 (uint8)0x12
#define NRC_13 (uint8)0x13
#define NRC_22 (uint8)0x22
#define NRC_24 (uint8)0x24
#define NRC_31 (uint8)0x31
#define NRC_33 (uint8)0x33
#define NRC_35 (uint8)0x35
#define NRC_36 (uint8)0x36
#define NRC_37 (uint8)0x37
#define NRC_70 (uint8)0x70
#define NRC_71 (uint8)0x71
#define NRC_72 (uint8)0x72
#define NRC_73 (uint8)0x73
#define NRC_78 (uint8)0x78
#define NRC_7F (uint8)0x7F

#define NOT_SUB_FUNCTION    0
#define SUB_FUNCTION        0x80

#define P2CAN_SERVER 50			//server接收到请求到发出响应的间隔，超过则返回0x78NRC
#define P2$CAN_SERVER 5000		//server返回0x78后到返回响应的间隔


#ifdef __cplusplus 
extern "C" {
#endif


enum
{
	Normal_Run,
	Always_Run
};


/* 诊断任务主函数 */
void DiagComMainTask( void* para );

void gf_diag_com_INIT( void );
void gf_diag_com_EXEC( void );
void gf_diag_com_TERM( void );
void gf_diag_com( uint8 type );

void gf_set_NRC( uint8 utv1_NRC, st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf );

#ifdef __cplusplus 
}
#endif

#endif
/****************************End Line****************************/

