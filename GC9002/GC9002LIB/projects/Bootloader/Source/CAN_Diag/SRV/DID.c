#include "DiagPlatform.h"
#include "DiagMgr.h"
#include <string.h>


#if 0
extern HMIDETAIL    HMIDetail;//界面内容

/****************************************************/
/*          asc码转成bcd�?                          */
/* source_data和下�?个字节的ascll码放到一个字节中�? */
/* 					每四位表示一个数	 			*/
/****************************************************/
static uint8 BCD_to_hex(uint8 *source_data)
{
    uint8 i;
    uint8 j;
    uint8 target_data;
    
    i = *source_data;
    j = *(source_data + 1);
    if ((i >= '0') && (i <= '9') && (j >= '0') && (j <= '9'))
    {
        target_data = ((i - '0') << 4) + (j - '0');
    }
    else
    {
        target_data = 0;
    }
    
    return target_data;
}

/* 获得固定数据 */
void Get_Const_Data( Typ_DID_Information * p,  uint8 * data )
{
	uint8 * t_source_tab;
	uint8 i;
	uint8 j = 0;
	while(ICM_Diag_Info_Config[j].source_id != 0xff)
	{
		if (p->TargetID == ICM_Diag_Info_Config[j].source_id)
		{       
			t_source_tab = ICM_Diag_Info_Config[j].source_data;
			i = 0;
			/* 读取字符类型 */
			if (ICM_Diag_Info_Config[j].diag_out_type == TYPE_CHAR)     
			{
				/* �?大copy20个字符，因为buffer限制 */
				while ((*t_source_tab != '\0') && (i < DIAG_DID_MAX_BYTE))
				{
					*data = *t_source_tab;
					i ++;
					data ++;
					t_source_tab ++;
				}
			}
			/* 读取BCD类型,�?要转�? */
			else if (ICM_Diag_Info_Config[j].diag_out_type == TYPE_BCD)     
			{
				while ((*t_source_tab != '\0') && (*(t_source_tab + 1) != '\0') && (i < DIAG_DID_MAX_BYTE))
				{//�?大copy20个字符，因为buffer限制
					*data = BCD_to_hex(t_source_tab);
					i ++;
					data ++;
					t_source_tab += 2;     //BCD码一个字节里有两个数�?
				}
			}
			break;
		}
		j ++;
	}
}

/******************************************************************/
/*      获取DID数据，并赋�?�T buffer                               */
/*      22 hex服务才用得到										  */
/******************************************************************/
void GetDiagData( Typ_DID_Information * p, uint8 * data )
{
	uint8 * t_mid_tab;
	uint8 i,j;
	uint8 temp;
	uint16 check_code;
	uint16 addr_vin[2] = {EEPROM_ADDR_VIN_0, EEPROM_ADDR_VIN_1};
	 /* 根据类型决定从哪个数组取数据，不同数组用几位表示�?个数据不�?�? */
    switch(p->TargetType)
    {
        case DIAG_SWITCH:
            t_mid_tab = DiagnoseSwitchStatusTable;
//			*data = GetTwoBitByIndex( p->TargetID, t_mid_tab );
            break;
        case DIAG_LED:
            /* 现在还没有定义LED的数组，只有�?�?32位数用于2F服务 */
            break;
        case DIAG_LCD:
//            t_mid_tab = DiagnoseLCDStatusTable;
//			*data = GetTwoBitByIndex( p->TargetID, t_mid_tab );
            break;
        case DIAG_MISC:
			if( p->DID_ID == DID_BAT_VOL )
			{
				*data = HMIDetail.u32_Voltage;
			}
			else if( p->DID_ID == DID_ODOMETER )
			{
				for( i = 0; i<(p->byteNum); i++ )
				{
					*(data+i) = HMIDetail.u32_ODOMilleage >> (8*(p->byteNum - 1 - i));
				}
			}
			else if( p->DID_ID == DID_BACKLIGHT )
			{
				*data = (HMIDetail.u8_BackLight);
			}
			else if( p->DID_ID == DID_MAINTAIN_METER )
			{
				for( i = 0; i<(p->byteNum); i++ )
				{
					*(data+i) = (HMIDetail.u32_MaitMilleage) >> (8*(p->byteNum - 1 - i));
				}
			}
			else if( p->DID_ID == DID_MAINTAIN_TIMES )
			{
				/* 待定 */
//				*data = 
			}

            break;
        case DIAG_BUZZER:
            /* 现在还没有定义BUZZER的数组，只有�?�?32位数用于2F服务 */
            break;
        case DIAG_CONST_INFO:
			Get_Const_Data( p, data );
            break;
        case DIAG_EEPROM_BOOT:
			if( p->DID_ID == DID_STORE_VIHICLE_VIN )
			{
				/* 从EEPROM里面读取VIN */
				check_code = 0;
				for( j=0; j<2; j++ )
				{
					/* 计算校验码并且比�? */
					for( i=0; i<p->byteNum; i++ )
					{
						EEPROM_Read( addr_vin[j] + i, &temp, 1);
						check_code += ( uint16 )temp;
					}
					EEPROM_Read( addr_vin[j] + p->byteNum, &temp, 1 );
					if( temp != ( uint8 )( 0x00FF & check_code ) )
					{
						/* 该备份无效，读下�?个区�? */
						continue;
					}
					EEPROM_Read( addr_vin[j] + p->byteNum + 1, &temp, 1 );
					if( temp != ( uint8 )( check_code>>8 ) )
					{
						continue;
					}
					/* 读取VIN数据 */
					EEPROM_Read( EEPROM_ADDR_VIN_0, data, p->byteNum);
				}
			}
            break;   
        default:
            break;
    }
	
}
#endif 
