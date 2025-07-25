#include "DiagPlatform.h"
#include "Repro.h"
#include "DiagType.h"
#include "DiagNWL.h"
#include "SID_36.h"
#include "Session.h"
//#include "stm32f10x_flash.h"
///#include "gc90x_can.h"
//#include "CANLoad_mal.h"

static uint32 s_ReproStartAddressForDownload;
static uint32 s_ReproSizeForReceive;

static uint8 s_Repro_Stage = REPRO_HAS_NOT_STARTED;


uint8 GetReproStage(void)
{
    return s_Repro_Stage;
}

void SetReroStage(uint8 stage)
{
    if((stage <= (uint8)REPRO_GET_EXIT) )
    {
        s_Repro_Stage = stage;
    }
}

uint8 checksum_add(uint8 * data, uint16 len)
{
    uint8 sum=0;
    uint16 i = 0;
    
    for( i = 0; i < len; i++ )
    {
        sum = sum + data[i];
    }
    sum = 0xFF - sum;
    return sum;
}

uint8 SID_data[3][19] = {0};
uint16 g_blockNum = 0;
uint32 g_t_ReproSize = 0;
uint32 g_totalLength = 0;
uint8 sum = 0;
uint8 checkSum = 0;
uint32 g_eraseAddr = 0;
uint32 g_writeAddr = 0;
uint32 g_writeLength = 0;
uint8 recvData[109] = {0};

#if 0
/* 36服务，把数据下载到Flash，t_Data是待写的数据起始，length是写入的长度，注意如果成功写入则s_ReproStartAddressForDownload要加，s_ReproSizeForReceive要减length */
/* 之后37�?出下载的时�?�就可以�?查Size是否变成0，如果没有变�?0则说明还没有下载完，不能�?出�?�另外，如果Size变成0还继续调用该函数，则也报错，这里直接返回NRC */
uint8 ReproDataToFlash( uint8 *t_Data, uint32 length, uint32 t_SrcAddress )
{
    
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
//	uint32_t *p_Data = (uint32_t *)t_Data;
//	uint32_t i;
	
	if( t_SrcAddress < EXE_APP_FLAG )
	{
		return FLASH_ERROR_PG;
	}
	
	FLASH_Unlock();
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
//	CANLoad_MAL_Erase(s_ReproStartAddressForDownload);
	
//	CANLoad_MAL_Write(s_ReproStartAddressForDownload,s_ReproSizeForReceive);
	
	/* 因为涉及到分组写，所以不能每次都擦除 */
//	/* Erase Data */
//	for( i=0 ; i < 56 ; i++ )
//	{
//		FLASHStatus = FLASH_ErasePage( t_SrcAddress+i*STMCAN_PAGE_SIZE );
//	}
	
	/* Programming Data */
//	for( i=0; i < length; i = i + 4 )
//	{
//		FLASH_ProgramWord( t_SrcAddress, *p_Data );
//		
//		t_SrcAddress += 4;
//		p_Data++;
//	}
	/* 写入总长度减去length，方�?37服务判断 */
//	s_ReproSizeForReceive -= length;
	s_ReproStartAddressForDownload = t_SrcAddress;
	FLASH_Lock();
	return FLASHStatus;
    
}
#endif


uint8 s_app_state_data[4];
uint8 CheckAppExist( void )
{

//	uint8 t_RetData;

//	EepParam_Read(EEP_PARAM_APP_STATE,s_app_state_data);                //cjt 由于暂时没有EEPROM接口，暂时注释掉
	if(1 == s_app_state_data[0])
	{
		return BOOLE_TRUE;
	}
	else
	{
		return BOOLE_FALSE;
	}

//return BOOLE_FALSE;	
}

void SetAppNotExist()
{
//	uint8 t_Data = 0;
	//EepParam_Write( EEP_PARAM_APP_STATE, &t_Data );                   //cjt 由于暂时没有EEPROM接口，暂时注释掉
}

void SetAppExist()
{
//	uint16 t_Data = 0x0101;
	//EepParam_Write( EEP_FLAG_APPWRITED, &t_Data );                    //cjt 由于暂时没有EEPROM接口，暂时注释掉
}

uint8 s_Temp_Data[4];
//static uint8 s_JumpFromAppFlg;
void CheckFromApp( void )
{
    #if 0
    CAN_StandardType t_TxMessage;
    

	EepParam_Read(EEP_PARAM_JUMP_FROM_APP,s_Temp_Data);
	if( s_Temp_Data[0]==1 )
	{
        s_JumpFromAppFlg=1;
        SetNowSecurityLevel(0x12);          //安全等级修改
        ugv1_session_change(PROGRAMME_SESSION);
        /*由于应用层已经将34的pending返回完毕，因此直接返回possitive应答*/
		SetReroStage(REPRO_TRANSFERDATA_DRIVER);
		(void)memset(t_TxMessage.data, 0xAA, 8);
        t_TxMessage.data[0] = 0x04;
        t_TxMessage.data[1] = 0x74;
        t_TxMessage.data[2] = 0x20;
        t_TxMessage.data[3] = 0x04;
        t_TxMessage.data[3] = 0x02;
        t_TxMessage.can_id = BCM_DIAG_CAN_ID;
        t_TxMessage.len = 8;
        AutoEle_CAN_EXTEND_SendMsg( &t_TxMessage, CAN0 );
	}
	#endif
	return ;
}

uint32 GetDownloadStartAddress( void )
{
    return s_ReproStartAddressForDownload;
}

uint32 GetDownloadSize( void )
{
	return s_ReproSizeForReceive;
}

void SetReproStartAddressAndSize( uint32 t_StartAddress, uint32 t_Size )
{
	s_ReproSizeForReceive = t_Size;
	s_ReproStartAddressForDownload = t_StartAddress;
}

void ChangeSize(uint32 length)
{
	s_ReproSizeForReceive -= length;
}

void ChangeStartAddress(uint32 length)
{
	s_ReproStartAddressForDownload += length;;
}



//#pragma CODE_SEG DEFAULT_ROM
