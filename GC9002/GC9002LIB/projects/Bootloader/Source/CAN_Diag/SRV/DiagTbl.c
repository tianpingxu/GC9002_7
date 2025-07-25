
#ifndef DiagTbl_C
#define DiagTbl_C

#include <stdlib.h>
#include "DiagPlatform.h"
#include "DiagMgr.h"
#include "Session.h"
#include "DiagNWL.h"
#include "DiagTbl.h"
#include "SID_10.h"
#include "SID_11.h"
#include "SID_27.h"
#include "SID_28.h"
#include "SID_22.h"
#include "SID_2E.h"
#include "SID_31.h"
#include "SID_85.h"
#include "SID_14.h"
#include "SID_3E.h"
#include "SID_34.h"
#include "SID_36.h"
#include "SID_37.h"
#include "SID_64.h"
#include "SID_66.h"
#include "SID_67.h"


#include "Security.h"

const st_Diag_TableType st_Diag_Table[] =
{
	/* sid,				can_id,							session,		                                   		security_lvl,			func_init,				func_exec,				func_term		 */

    { (uint8)0x10,	(uint16)(TESTER_FUNCTION_CAN_ID),	(DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION),	SECURITY_TYPE_DEFAULT,	gf_func_SID_10_init, gf_func_SID_10_exec, gf_func_SID_10_term },                             //ok
    { (uint8)0x11,  (uint16)(TESTER_FUNCTION_CAN_ID),   (DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION),   SECURITY_TYPE_DEFAULT,  gf_func_SID_11_init, gf_func_SID_11_exec, gf_func_SID_11_term },                             //ok
    { (uint8)0x28,  (uint16)(TESTER_FUNCTION_CAN_ID),   (PROGRAMME_SESSION|EXTENDED_SESSION),  					SECURITY_TYPE_DEFAULT,	gf_func_SID_28_init, gf_func_SID_28_exec, gf_func_SID_28_term },                             //ok
    { (uint8)0x64,  (uint16)(TESTER_DIAG_CAN_ID),       (PROGRAMME_SESSION|EXTENDED_SESSION),                   SECURITY_TYPE_DEFAULT,     gf_func_SID_64_init, gf_func_SID_64_exec, gf_func_SID_64_term },                             //ok
    { (uint8)0x66,  (uint16)(TESTER_DIAG_CAN_ID),       (PROGRAMME_SESSION|EXTENDED_SESSION),                   SECURITY_TYPE_DEFAULT,     gf_func_SID_66_init, gf_func_SID_66_exec, gf_func_SID_66_term },                             //
    { (uint8)0x67,  (uint16)(TESTER_DIAG_CAN_ID),       (PROGRAMME_SESSION|EXTENDED_SESSION),                   SECURITY_TYPE_DEFAULT,     gf_func_SID_67_init, gf_func_SID_67_exec, gf_func_SID_67_term },                             //ok

   #if 0
    { (uint8)0x11,  (uint16)(TESTER_FUNCTION_CAN_ID),	(DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION), 	SECURITY_TYPE_DEFAULT,	gf_func_SID_11_init, gf_func_SID_11_exec, gf_func_SID_11_term },                             //ok
    { (uint8)0x14,  (uint16)(TESTER_FUNCTION_CAN_ID),   (DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION), 	SECURITY_TYPE_DEFAULT,	gf_func_SID_14_init, gf_func_SID_14_exec, gf_func_SID_14_term },                             //ok
    { (uint8)0x19,  (uint16)(TESTER_FUNCTION_CAN_ID),   (DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION), 	SECURITY_TYPE_DEFAULT,	gf_func_SID_19_init, gf_func_SID_19_exec, gf_func_SID_19_term },                             //ok
	{ (uint8)0x22,  (uint16)(TESTER_FUNCTION_CAN_ID),   (DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION), 	SECURITY_TYPE_DEFAULT,	gf_func_SID_22_init, gf_func_SID_22_exec, gf_func_SID_22_term },                             //ok
	{ (uint8)0x27,  (uint16)(TESTER_DIAG_CAN_ID),    	(DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION), 	SECURITY_TYPE_DEFAULT,	gf_func_SID_27_init, gf_func_SID_27_exec, gf_func_SID_27_term },                             
    { (uint8)0x28,  (uint16)(TESTER_FUNCTION_CAN_ID),   (PROGRAMME_SESSION|EXTENDED_SESSION),  					SECURITY_TYPE_DEFAULT,	gf_func_SID_28_init, gf_func_SID_28_exec, gf_func_SID_28_term },                             //ok
	{ (uint8)0x2E,  (uint16)(TESTER_DIAG_CAN_ID),    	(PROGRAMME_SESSION|EXTENDED_SESSION),                	SECURITY_TYPE_APP,		gf_func_SID_2E_init, gf_func_SID_2E_exec, gf_func_SID_2E_term },                             //ok
    { (uint8)0x2F,  (uint16)(TESTER_DIAG_CAN_ID),    	(PROGRAMME_SESSION|EXTENDED_SESSION),  	           		SECURITY_TYPE_APP,		gf_func_SID_2F_init, gf_func_SID_2F_exec, gf_func_SID_2F_term },                             //
    { (uint8)0x31,  (uint16)(TESTER_DIAG_CAN_ID),    	(PROGRAMME_SESSION|EXTENDED_SESSION),  	           		SECURITY_TYPE_BOOT,		gf_func_SID_31_init, gf_func_SID_31_exec, gf_func_SID_31_term },                             //ok
    //{ (uint8)0x31,  (uint16)(TESTER_DIAG_CAN_ID),    	(DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION),  	SECURITY_TYPE_DEFAULT,	gf_func_SID_31_init, gf_func_SID_31_exec, gf_func_SID_31_term },	
    { (uint8)0x3E,  (uint16)(TESTER_FUNCTION_CAN_ID),   (DEFAULT_SESSION|PROGRAMME_SESSION|EXTENDED_SESSION), 	SECURITY_TYPE_DEFAULT,	gf_func_SID_3E_init, gf_func_SID_3E_exec, gf_func_SID_3E_term },                             //ok
    { (uint8)0x85,  (uint16)(TESTER_FUNCTION_CAN_ID),   (PROGRAMME_SESSION|EXTENDED_SESSION),  	            	SECURITY_TYPE_DEFAULT,	gf_func_SID_85_init, gf_func_SID_85_exec, gf_func_SID_85_term },                             //ok
    { (uint8)0x34,  (uint16)(TESTER_DIAG_CAN_ID),    	(PROGRAMME_SESSION|EXTENDED_SESSION),  	            	SECURITY_TYPE_BOOT,		gf_func_SID_34_init, gf_func_SID_34_exec, gf_func_SID_34_term },                             //ok
	{ (uint8)0x36,  (uint16)(TESTER_DIAG_CAN_ID),    	(PROGRAMME_SESSION|EXTENDED_SESSION),                 	SECURITY_TYPE_BOOT,		gf_func_SID_36_init, gf_func_SID_36_exec, gf_func_SID_36_term },                             //
    { (uint8)0x37,  (uint16)(TESTER_DIAG_CAN_ID),    	(PROGRAMME_SESSION|EXTENDED_SESSION),  	            	SECURITY_TYPE_BOOT,		gf_func_SID_37_init, gf_func_SID_37_exec, gf_func_SID_37_term },                             //ok
#endif
    { (uint8)0,     (uint16)0,						 	(uint8)0, 			                                	0,						(func_init_type)NULL, (func_exec_type)NULL, (func_term_type)NULL},
};


#endif

