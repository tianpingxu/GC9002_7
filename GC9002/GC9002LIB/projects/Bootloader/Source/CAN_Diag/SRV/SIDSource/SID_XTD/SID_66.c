#ifndef SID_66_C
#define SID_66_C
#include <string.h>
#include "DiagPlatform.h"
#include "DiagTbl.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "DiagType.h"
#include "Session.h"
#include "Repro.h"
#include "SID_64.h"
#include "SID_66.h"
#include "upgrade_c1.h"
#include "crc_c1.h"





//#pragma CODE_SEG PAGED_ROM

#define BLOCK_COUNTER_MAX		0xFF				/* counterçæå¤§å?¼ï¼æ?å¤§å?¼ä¹åå 1å°±åæ?0 */


//extern uint32_t Checkflag;  // by Zhu Jiahai 





/* æ£?æ¥ç¼ç¨å®æ´æ?§çCRC */
uint32 g_ProgramCRC_value = 0xFFFFFFFFul;

/* ç¨äºå¤æ¬¡æ°æ®ä¼ è¾çcounter */
uint8 block_counter = 1;

/* åå§åï¼ä¸?è¬ç¨äº?34hexçè°ç? */
void counter_init( void )
{
	block_counter = 1;
}


/* å®æä¸?æ¬¡æåçæ°æ®ä¼ è¾å°±å 1ï¼å¦æè¶è¿ä¸éï¼åä¸º0 */
void counter_update( void )
{
	block_counter += 1;
	if( block_counter > BLOCK_COUNTER_MAX )
	{
		block_counter = 0;
	}
}


void Init_CRC_Value(void)
{
    g_ProgramCRC_value = 0xFFFFFFFFul;
}


uint32 Get_CRC_Value(void)
{
    return g_ProgramCRC_value;
}


void gf_func_SID_66_init ( void )
{
    return;
}


uint8 gf_func_SID_66_exec ( st_Diag_RX_BufType *tpt_Diag_Rx_Buf, st_Diag_Tx_BufType *tpt_Diag_Tx_Buf ,uint8 run_flag)
{
	uint8 utv1_nrc;
	uint8 counter;
	uint16 utv1_meslength;
	
    int32_t res;

	utv1_meslength = tpt_Diag_Rx_Buf->data_length;
	counter = tpt_Diag_Rx_Buf->Rx_data[1];
	utv1_nrc = NO_NRC;
	
	/* å¦æä¹åæ²¡æéè¿è¯·æ±ä¸è½½æ¥è¿å¥æ°æ®ä¼ è¾ç¶æï¼åæ¥é? */
	if(GetReproStage() != REPRO_TRANSFERDATA_DRIVER)
	{
		utv1_nrc = NRC_24;
		return utv1_nrc;
	}
	/* å¦æä¸?æ¬¡æ?§ä¼ è¾çæ°æ®å¤§äº34ååºä¸­å®ä¹çæ?å¤§é¿åº? */
	if( utv1_meslength > MAX_TRANS_BUF_SIZE_64 )
	{
		utv1_nrc = NRC_13;
		return utv1_nrc;
	}
	/* è¯·æ±ä¸è½½çæ°æ®å¤§å°è¶è¿?64æå¡è¯·æ±çå?? */
//	if( utv1_meslength-2 > GetDownloadSize() )
//	{
//		utv1_nrc = NRC_71;
//		return utv1_nrc;
//	}
	/* å¦æä¼ è¾çcounteræéçæ¶å? */
	if( (block_counter!=counter) && (block_counter!=(counter+1)) )
	{
		utv1_nrc = NRC_73;
		return utv1_nrc;
	}
	
	/* å¦æä¼ è¿æ¥çcounteræ¯å½åserverä¿å­çå?¼å1çè¯ï¼è¯´æè¯¥æ°æ®åæä¼ è¿æ¯éå¤æ°æ®ï¼	 	*/
	/* è¿æ¶åªéè¦è¿åè¯å®ååºå³å¯ï¼å ä¸ºè¯¥æ°æ®å·²ç»æ­£ç¡®åå¥åå­äºï¼åªæ¯clientæ²¡ææ¶å°ååºèå·² */
	if( block_counter == counter + 1 )
	{
		;
	}
	/* counteråserveræ¬å°å¼ç¸ç­ï¼è¯´æä¸æ¬¡ä¼ è¾å¤±è´¥ï¼æèä¸æ¬¡ä¼ è¾æåå¹¶ä¸clientæ­£ç¡®æ¥æ¶ååºæ¯æ°çä¸ä¸ªæ°æ? */
	else
	{
		/* ä¸è½½æ°æ® */
		//ptr = (uint8*)GetDownloadStartAddress();
		
		//for(i = 0; i < utv1_meslength - 2; i++)
		//{
		//	*(ptr + i) = tpt_Diag_Rx_Buf->Rx_data[2 + i];
		//}
		
		res = UPG_PAS_i32GetData(&tpt_Diag_Rx_Buf->Rx_data[2], utv1_meslength - 2);
        if(0 != res)
        {
            utv1_nrc = NRC_70;
		    return utv1_nrc;
        }

		
		/* æ­£ç¡®ï¼counterå¼å 1 */
		ChangeSize(utv1_meslength - 2);
		ChangeStartAddress(utv1_meslength - 2);
		//counter_update();  //by Zhu Jiahai, it's not correct to adjust here
	}
	
	/* æ°æ®ä¼ è¾æ­£ç¡®å°±æ´æ°CRCï¼ç¨äº?31 hexçæ£æ¥å®æ´æ?§ä¾ç¨? */
	//g_ProgramCRC_value = CRC32(&(tpt_Diag_Rx_Buf->Rx_data[2]), utv1_meslength - 2, g_ProgramCRC_value);
    g_ProgramCRC_value = CRC_u32Crc32(g_ProgramCRC_value, &(tpt_Diag_Rx_Buf->Rx_data[2]), utv1_meslength - 2);
    
	memset(&tpt_Diag_Tx_Buf->Data[0], 0xAA, 8);  /* Customer requirement, MainState as 0xAA after 10 02 */
	tpt_Diag_Tx_Buf->Tx_CAN_ID = (uint32)BCM_DIAG_CAN_ID;
	tpt_Diag_Tx_Buf->data_length = 2;
	tpt_Diag_Tx_Buf->DLC = (uint8)0x08;
	tpt_Diag_Tx_Buf->Data[0] = (uint8)0xA6;
	tpt_Diag_Tx_Buf->Data[1] = block_counter;  /* to do, by Zhu Jiahai */
	tpt_Diag_Tx_Buf->Tx_Req = NOW_REQUEST;

    counter_update(); // add by Zhu Jiahai
	//Checkflag = 0x5A5A5A5A;   //ç¼ç¨å®æ´æ§æ£æ¥å®æ¯ï¼æ è¯åéç½®ä¸º0x5A5A5A5A        // by Zhu Jiahai 

    return utv1_nrc;
}


void gf_func_SID_66_term ( void )
{
    
    return;
}

#endif

//#pragma CODE_SEG DEFAULT
