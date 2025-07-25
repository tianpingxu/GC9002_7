/****************************************************************/
/*名 称：DiagNWL                                               */
/*功 能：Diag帧关联的驱动                         */
/*说 明：Diag帧的取得和解析                             */
/****************************************************************/

#include "DiagPlatform.h"
#include "DiagMgr.h"
#include "DiagNWL.h"
#include "string.h"
#include "DiagTimer.h"
///#include "gc90x_can.h"
#include "Session.h"
//#include "FreeRTOS.h"
//#include "semphr.h"

/***********************内部变更***************************/

static uint8 usv1_SN;
static uint8 usv1_STmin;
static uint8 usv1_BSize;
static uint8 usv1_FS;
static uint8 usv1_BS_count;
uint8 usv1_Diag_Receive_status;
uint8 usv1_Diag_Transmit_status;
static uint8 usv1_check_no;
static uint16 usv2_data_count;
static uint16 usv2_data_TX_count;
/* 临时的对象 */
st_Diag_RX_BufType sst_Diag_Rx_Buf;
st_Diag_Tx_BufType sst_Diag_Tx_Buf;

CanFrameType Can_Frame;

/* 信号量用于同步多帧发送 */
//SemaphoreHandle_t xSemaphore_multy_trans;

/************************全局函数****************************/
/****************************************************************/
/*函数名:gf_Diag_NWL_init                                       */
/*功能：网络层初始化                                    		*/
/****************************************************************/
void gf_Diag_NWL_init( void )
{
	/* 创建信号量 */
//	xSemaphore_multy_trans = xSemaphoreCreateBinary();
	usv1_Diag_Receive_status = RX_SF;
	usv1_Diag_Transmit_status = TX_SF;
	usv2_data_count = (uint16)0;
	usv1_check_no = (uint8)0;
	usv1_SN = (uint8)1;
	usv1_STmin = (uint8)0;
	usv1_BSize = (uint8)0;
	usv1_FS = 0;
	usv1_BS_count = (uint8)1;
	usv2_data_TX_count = (uint16)0;

    return;
}

/****************************************************************/
/*函数名:gf_Rx_init                                             */
/*功能：R Buffer初始化                                          */
/****************************************************************/
st_Diag_RX_BufType* gf_Rx_init( void )
{
    memset( &sst_Diag_Rx_Buf, (uint8)0, sizeof( st_Diag_RX_BufType ) );

    return (&sst_Diag_Rx_Buf);
}

st_Diag_Tx_BufType* gf_Tx_init( void )
{
    memset( &sst_Diag_Tx_Buf, (uint8)0, sizeof( sst_Diag_Tx_Buf ) );

    return ( &sst_Diag_Tx_Buf );
}


void gf_NWL_Rrceive_DiagMess( st_Diag_RX_BufType *tpt_Diag_Rx_Buf )
{
	CanFrameType TxMessage;
	uint8 utv1_rx_index;
	uint32 utv4_data_length_ff;
	uint8 utv1_data_no;
		

	memset(TxMessage.data,0,8);
	
	/* 功能寻址只支持单帧 */
	if( Can_Frame.Id == (uint32)TESTER_FUNCTION_CAN_ID )
	{
		if( usv1_Diag_Receive_status == RX_SF )
		{
			if( ( Can_Frame.data[0]&(uint8)0xf8 ) == (uint8)0x00 )
			{
				tpt_Diag_Rx_Buf->Rx_CAN_ID = Can_Frame.Id;
				tpt_Diag_Rx_Buf->data_length = Can_Frame.data[0];
				memcpy(tpt_Diag_Rx_Buf->Rx_data, (Can_Frame.data+1), 7);
				tpt_Diag_Rx_Buf->Rx_Result = RX_FINISH;
			}
			else
			{
				tpt_Diag_Rx_Buf->Rx_Result = RX_ERROR;
			}
		}
	}
	/* 物理寻址 */
	else
	{
		switch ( usv1_Diag_Receive_status )
		{
			/* 等待接收单帧 */
			case RX_SF:
				/* 这里用f8而不是f0保证了后半个字节小于等于7，即数据长度符合条件 */
				if( (Can_Frame.data[0]&(uint8)0xf8) == 0x00 )
				{
					tpt_Diag_Rx_Buf->Rx_CAN_ID = Can_Frame.Id;
					tpt_Diag_Rx_Buf->data_length = Can_Frame.data[0];
					memcpy(tpt_Diag_Rx_Buf->Rx_data, (Can_Frame.data+1), 7);
					tpt_Diag_Rx_Buf->Rx_Result = RX_FINISH;

				}
				/* 首帧 */
				else if( (Can_Frame.data[0]&(uint8)0xf0) == 0x10)
				{
					utv4_data_length_ff = (uint16)(Can_Frame.data[0]&0x0f);
					utv4_data_length_ff = utv4_data_length_ff<<8;
					utv4_data_length_ff = utv4_data_length_ff|(uint16)Can_Frame.data[1];

					if(utv4_data_length_ff>((uint32)DIAG_RX_BUF_MAX))
					{
						/* 流控为溢出 */
						usv1_FS = 2;
						/* 进入等待发送流控 */
						usv1_Diag_Transmit_status = TX_FC;
						/* 出错回到默认状态 */
						usv1_Diag_Receive_status = RX_SF;
						tpt_Diag_Rx_Buf->Rx_Result = RX_ERROR;
					}
					else if(utv4_data_length_ff<8)
					{
						tpt_Diag_Rx_Buf->Rx_Result = RX_ERROR;
						usv1_Diag_Receive_status = RX_SF;
					}
					else
					{
						tpt_Diag_Rx_Buf->Rx_CAN_ID = Can_Frame.Id;
						tpt_Diag_Rx_Buf->data_length = utv4_data_length_ff;
						for(utv1_rx_index=0; utv1_rx_index<6; utv1_rx_index++)
						{
							tpt_Diag_Rx_Buf->Rx_data[utv1_rx_index]=Can_Frame.data[utv1_rx_index+2];
						}
						/* 首帧SN是0，之后一个后续帧是1，然后逐次加1直到15，然后又为0，循环 */
						usv1_check_no = 1;
						usv2_data_count = (uint16)6;
						
						/* 流控为继续发送 */
						usv1_FS = 0;
						/* 进入等待发送流控 */
						usv1_Diag_Transmit_status = TX_FC;
						
						tpt_Diag_Rx_Buf->Rx_Result = RX_RECEIVING;
					}
				}
			break;
				
			/* 等待接收后续帧 */
			case RX_CF:
				if( (Can_Frame.data[0]&(uint8)0xf0) == 0x20)
				{
					/* 后续帧SN号 */
					utv1_data_no = (Can_Frame.data[0]&0x0f);
					if(usv1_check_no == utv1_data_no)
					{
						tpt_Diag_Rx_Buf->Rx_Result = RX_RECEIVING;
						for(utv1_rx_index=1; utv1_rx_index<8; utv1_rx_index++)
						{
							tpt_Diag_Rx_Buf->Rx_data[usv2_data_count]=Can_Frame.data[utv1_rx_index];
							usv2_data_count++;
							/* 长度达到了首帧规定长度 */
							if((usv2_data_count) == (tpt_Diag_Rx_Buf->data_length) )
							{
								usv1_Diag_Transmit_status = TX_SF;
								usv1_Diag_Receive_status = RX_SF;
								tpt_Diag_Rx_Buf->Rx_Result = RX_FINISH;
								usv1_check_no = 1;
								/* 跳出循环 */
								utv1_rx_index=utv1_rx_index+7;
							}
						}
						usv1_check_no++;
						if(usv1_check_no>0x0f)
						{
							usv1_check_no = 0;
						}
					}
					/* 后续帧号对不上号，也跳出循环，并且使得报长度错误 */
					else
					{
						tpt_Diag_Rx_Buf->data_length += 1;
						usv1_Diag_Transmit_status = TX_SF;
						usv1_Diag_Receive_status = RX_SF;
						tpt_Diag_Rx_Buf->Rx_Result = RX_FINISH;
						usv1_check_no = 1;
					}
				}
			break;

			/* 等待接收流控，这是在发送首帧后进入的状态 */
			case RX_FC:
				if( (Can_Frame.data[0]&(uint8)0xf0) == 0x30 )
				{
					usv1_BSize = Can_Frame.data[1];
					usv1_STmin = Can_Frame.data[2];
					/* 设置定时器值 */
					set_Diag_Timer_mStopValue( Diag_TIMER_STmin, usv1_STmin );
					/* 继续发送 */
					if((Can_Frame.data[0]&0x0f) == CTS)
					{
						usv1_Diag_Transmit_status = TX_CF;
						usv1_BS_count = 1;
					}
					/* 等待下一个流控 */
					else if((Can_Frame.data[0]&0x0f) == WT)
					{
						;
					}
					/* 溢出 */
					else if((Can_Frame.data[0]&0x0f) == OVFLW)
					{
						usv1_Diag_Receive_status = RX_SF;
						usv1_Diag_Transmit_status = TX_SF;
						tpt_Diag_Rx_Buf->Rx_Result = RX_ERROR;
					}
				}
			break;
		}
	}
	return;
}



void gf_NWL_Transmit_DiagMess( st_Diag_Tx_BufType *tpt_Diag_Tx_Buf )
{

    uint8 utv1_count_index;
    uint8 utv1_index;
    uint8 utv1_FF_send_index;
    CanFrameType TxMessage;
    
	/* 不需要发送直接返回 */
	if( tpt_Diag_Tx_Buf->Tx_Req == NEED_NOT_REQUEST )
    {
		tpt_Diag_Tx_Buf->Tx_Result = TX_FINISH;
		return;
	}

	for( utv1_index = (uint8)0; utv1_index< (uint8)8; utv1_index++ )
    {
        TxMessage.data[utv1_index] = (uint8)0;
    }
	
	tpt_Diag_Tx_Buf->Tx_Result = TX_INIT;
	switch ( usv1_Diag_Transmit_status )
	{
        case TX_SF:
            if( (tpt_Diag_Tx_Buf->data_length) < (uint16)8 )
            {
                TxMessage.Id = BCM_DIAG_CAN_ID;
                TxMessage.data[0] = (uint8)tpt_Diag_Tx_Buf->data_length;
                for( utv1_index=0; utv1_index<7; utv1_index++ )
                {
                    TxMessage.data[utv1_index+1] = tpt_Diag_Tx_Buf->Data[utv1_index];
                }
                tpt_Diag_Tx_Buf->Tx_Result = TX_FINISH;
                CAN_SendDiagMessage( TxMessage.Id , TxMessage.data, 8, 0 );
            }
            else if(( ( tpt_Diag_Tx_Buf->data_length ) > 7 ) && ( (tpt_Diag_Tx_Buf->data_length) <= (uint16)DIAG_TX_BUF_MAX ))
            {
                usv1_Diag_Receive_status = RX_FC;
                usv1_SN = 1;                    //初始化循环发送计数
                TxMessage.Id = BCM_DIAG_CAN_ID;
                TxMessage.data[0] = ((uint8)(( (tpt_Diag_Tx_Buf->data_length) & 0x0F00)>>8));
				/* 首帧 */
                TxMessage.data[0] = TxMessage.data[0] |0x10;
                TxMessage.data[1] = (uint8)(tpt_Diag_Tx_Buf->data_length);
                for( utv1_FF_send_index=(uint8)0; utv1_FF_send_index < (uint8)6; utv1_FF_send_index++)
                {
                    TxMessage.data[utv1_FF_send_index+2] = tpt_Diag_Tx_Buf->Data[utv1_FF_send_index];
                }
                usv2_data_TX_count = (uint16)6;
                tpt_Diag_Tx_Buf->Tx_Result = TX_SENDING;
                CAN_SendDiagMessage( TxMessage.Id , TxMessage.data, 8, 0 );
            }
            else if( (tpt_Diag_Tx_Buf->data_length) > DIAG_TX_BUF_MAX )
            {
                tpt_Diag_Tx_Buf->Tx_Result = TX_ERROR;
            }
            break;

		/* 发送多帧 */
        case TX_CF:
			/* 使能定时器 */
			Diag_Timer_update(Diag_TIMER_STmin);
			/* 如果没有传完就一直发送 */
			while( usv1_Diag_Transmit_status != TX_SF )
			{
				
				/* 如果流控不是一次性传完且已经到达一次的数据上限 */
				if( usv1_BSize != 0 )
				{
					if(usv1_BS_count == usv1_BSize)
					{
						/* 执行完这个循环就跳出 */
						usv1_Diag_Transmit_status = TX_SF;
						usv1_Diag_Receive_status = RX_FC;
						tpt_Diag_Tx_Buf->Tx_Result = TX_FINISH;
						usv1_BS_count = 1;
					}
					else
					{
						usv1_BS_count++;
					}
				}
				
				/* 每发送一帧都要先清零 */
				for( utv1_index = (uint8)0; utv1_index < (uint8)8; utv1_index++ )
				{
					TxMessage.data[utv1_index] = (uint8)0;
				}
				TxMessage.Id = BCM_DIAG_CAN_ID;
				/* 后续帧从1开始，首帧是0 */
				TxMessage.data[0] = usv1_SN|((uint8)0x20);
				
				for( utv1_count_index = (uint8)1; utv1_count_index < (uint8)8; utv1_count_index++ )
				{
					TxMessage.data[utv1_count_index] = tpt_Diag_Tx_Buf->Data[usv2_data_TX_count];
					usv2_data_TX_count++;
					
					if(usv2_data_TX_count >= (tpt_Diag_Tx_Buf->data_length))
					{
						utv1_count_index = utv1_count_index+7;
						usv1_Diag_Transmit_status = TX_SF;
						usv1_Diag_Receive_status = RX_SF;
						tpt_Diag_Tx_Buf->Tx_Result = TX_FINISH;
					}
					else
					{
						;
					}
						
				}

				if( usv1_SN == (uint8)0x0f )
				{
				   usv1_SN = 0;
				}
				else
				{
					usv1_SN++;
				}

				CAN_SendDiagMessage( TxMessage.Id , TxMessage.data, 8, 0 );
				
				/* 等待延时，发送下一个帧 */
				//xSemaphoreTake( xSemaphore_multy_trans, portMAX_DELAY );  // to do , by Zhu Jiahai
					
			}
			/* 关闭定时器 */
			Diag_Timer_disable(Diag_TIMER_STmin);
        break;
		
		/* 等待发送流控 */
		case TX_FC:
			TxMessage.Id = BCM_DIAG_CAN_ID;
			/* 3表示是流控，后面加上的数值就是流控状态 */
			TxMessage.data[0] = 0x30 + usv1_FS;
			TxMessage.data[1] = BSize;
			TxMessage.data[2] = St_min;
			CAN_SendDiagMessage( TxMessage.Id , TxMessage.data, 8, 0 );
			/* 准备接收后续帧 */
			usv1_Diag_Receive_status = RX_CF;
    }
    
    return;
}



/************************End Line********************************/


