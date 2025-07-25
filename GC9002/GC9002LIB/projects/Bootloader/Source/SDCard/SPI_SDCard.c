////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI小组成员
//
// Create Date:   2017/11/27
// File Name:     main.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2017/11/27 by Yu Chunying
////////////////////////////////////////////////////////////////////////////////

//#include "CPU1_CMSDK_CM0.h"
#include "SPI_SDCard.h"
#include "upgrade_ci.h"
#include "gc90xx.h"

#define CMD0CRC		0x95
#define CMD1CRC		0xF9
#define CMD8CRC		0x87
#define CMD41CRC	0x77
#define CMD55CRC	0x65
#define CMD58CRC	0xFD
#define CMD59CRC	0x91

#define SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

#if (UPG_USE_SD_CARD == 1)

static uint32_t CardType =  SDIO_STD_CAPACITY_SD_CARD_V1_1;	//存储卡的类型，先把它初始化为1.1协议的卡
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 0;//存储CSD，DID，寄存器和卡相对地址
//static uint8_t SDSTATUS_Tab[16]; //存储卡状态，是CSR的一部分
SD_CardInfo SDCardInfo;	  //用于存储卡的信息，DSR的一部分？
SPI_TypeDef * UPG_SdSpi;



//uint8_t buffer[2048];
//uint8_t buffer1[512] = {0};

//SpiBasic SDCard_Device={
//						GPIOA,
//						SPI1,
//						RCC_APB2Periph_GPIOA,
//						RCC_APB2Periph_SPI1	,
//						GPIO_Pin_5,
//						GPIO_Pin_6,
//						GPIO_Pin_7,
//						GPIO_Pin_4,
//						SPI_DataSize_8b,
//						SPI_BaudRatePrescaler_2
//                      };
//SpiBasic SDCard_Device={
//						0,
//						0	,
//						0,
//						0,
//						0,
//						0,
//						0,
//						0
//                      };
 __attribute__((optimize(0))) static void WaitSPIDone(int times)
{
	for(;times>=0;times--);
}
__attribute__((optimize(0))) static void SPI_SendData(SPI_TypeDef* SPIx, uint16_t Data)
{
	/* Write in the DR register the data to be sent */
	SPIx->DR = Data;
}
__attribute__((optimize(0))) static uint16_t SPI_ReceiveData(SPI_TypeDef* SPIx)
{
	/* Return the data in the DR register */
	return SPIx->DR;
}
__attribute__((optimize(0))) static FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
	FlagStatus bitstatus = RESET;
	
	/* Check the status of the specified SPI flag */
	if ((SPIx->SR & SPI_FLAG) != (uint16_t)RESET)
	{
		/* SPI_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* SPI_FLAG is reset */
		bitstatus = RESET;
	}
	/* Return the SPI_FLAG status */
	return bitstatus;
}
/*******************************************************************************
* Function Name  : ChangeSPIBaudRateForSDCard
* Description    : 修改SPI的时钟频率
* Input          : SPI:spi pointer
* Output         : None
*******************************************************************************/
__attribute__((optimize(0))) void ChangeSPIBaudRateForSDCard(SPI_TypeDef *SPIx)
{
//	SPI_Interface_Init(SPI1);
    
	SPIx->CPSR = 0x3;
}

static SPI_HandleTypeDef hspi;
__attribute__((optimize(0))) void SPI_Interface_Init(SPI_TypeDef *SPIx)
{
	hspi.Instance = SPIx;
	hspi.Init.Mode = SPI_MODE_MASTER | SPI_MODE_NORMAL;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.FrameFormat = SPI_FRF_MOTOROLA;
	hspi.Init.SPO = SPI_SPO_HIGH;
	hspi.Init.SPH = SPI_SPH_2EDGE;
	hspi.Init.CPSDVR = 0x80; // 时钟预分频系数 取值范围2-254之前的偶数
	hspi.Init.SCR = 0x0; // Clock pre-frequency dividing parameters     bitrate = Fspiclk/(CPSDVR*(1 + SCR))
	HAL_SPI_Init(&hspi);
}

__attribute__((optimize(0))) uint8_t SPI_FLASH_SendByte(SPI_TypeDef* SPIx,uint8_t byte)
{
	/* Loop while DR register in not empty */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_TFE) == RESET);
	
	SPI_ReceiveData(SPIx);

	/* Send byte through the SPI1 peripheral */
	SPI_SendData(SPIx, byte);

	/* Wait to receive a byte */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_RNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_ReceiveData(SPIx);
}

__attribute__((optimize(0))) void Spi_Flash_cs_low(SPI_TypeDef *SPIx)
{
	if(SPIx == SPI0)
	{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);}
	if(SPIx == SPI1)
	{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_18, GPIO_PIN_RESET);}
	if(SPIx == SPI2)
	{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_17, GPIO_PIN_RESET);}
}

__attribute__((optimize(0))) void Spi_Flash_cs_high(SPI_TypeDef *SPIx)
{
	if(SPIx == SPI0)
	{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);}
	if(SPIx == SPI1)
	{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_18, GPIO_PIN_SET);}
	if(SPIx == SPI2)
	{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_17, GPIO_PIN_SET);}
}

/*******************************************************************************
* Function Name  : SDCardSPI_CheckInsert
* Description    : 检查SD卡插入（根据SD卡初始化时是否有响应进行判断）
* Input          : SPIx   指向SPI首地址的指针
* Output         : None
* Return         : 0: 未插入，1：插入
*******************************************************************************/
__attribute__((optimize(0))) uint8_t SDCardSPI_CheckInsert(SPI_TypeDef* SPIx)
{
    int i;
    uint8_t ret = 0;

    SPI_Interface_Init(SPIx);
    
    Spi_Flash_cs_high(SPIx);
    /*******send  74 or more clock pulses to SCLK***/
    for(i = 0; i < 12; i++)
        SPI_FLASH_SendByte(SPIx, 0xFF);
    
    /*******send CMD0,software reset, enter idle mode***/   
//  if(SDCardSPI_SendCommand(SPIx, SD_CMD_GO_IDLE_STATE, 0, CMD0CRC, 0) != 0x01)
//      return SD_ERROR

    for (i = 0; i < 0x20; i++)
    {
        if (SDCardSPI_SendCommand(SPIx, SD_CMD_GO_IDLE_STATE, 0, CMD0CRC, 0) == 0x01)
        {
            ret = 1;
            break;
        }
    }

    return ret;
}


/*******************************************************************************
* Function Name  : SDCardSPI_Init
* Description    : 初始化SD卡
* Input          : SPIx   指向SPI首地址的指针
* Output         : None
* Return         : u8 r1 SD卡返回的响应
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_Init(SPI_TypeDef *SPIx)
{
	int i;
	uint32_t recvalue = 0;
	SD_Error errorstatus = SD_OK; 
	uint16_t retry=0;
	uint8_t rcv = 0; 
	
	SPI_Interface_Init(SPIx);
	
	Spi_Flash_cs_high(SPIx);
	/*******send  74 or more clock pulses to SCLK***/
	for(i = 0; i < 12; i++)
		SPI_FLASH_SendByte(SPIx, 0xFF);
	
	/*******send CMD0,software reset, enter idle mode***/	
//	if(SDCardSPI_SendCommand(SPIx, SD_CMD_GO_IDLE_STATE, 0, CMD0CRC, 0) != 0x01)
//		return SD_ERROR
    retry = 0x20;
    do
    {
        rcv = SDCardSPI_SendCommand(SPIx, SD_CMD_GO_IDLE_STATE, 0, CMD0CRC, 0);
    
    }
    while((rcv!=0X01) && (retry--));
    
	
//	for(i = 0; i < 10000; i++)
//		SPI_FLASH_SendByte(SPIx, 0xFF);
	/*******send CMD8***/	
	rcv = SDCardSPI_SendCommand(SPIx, 8, SD_CHECK_PATTERN, CMD8CRC, &recvalue);
	if(rcv == 0x01)   //version 2.0
	{
//		for(i = 0; i < 10000; i++)
//			SPI_FLASH_SendByte(SPIx, 0xFF);
//		/*******send CMD59,does not support crc***/	
//		SDCardSPI_SendCommand(&SDCard_Device, 59, 0, CMD59CRC, 0);
		retry = 0xFFFE;
		do
		{
			/*******send CMD55,exit idle mode***/	
//			rcv = SDCardSPI_SendCommand(SPIx, 55, 0, CMD55CRC, 0);
//			rcv = SDCardSPI_SendCommand(SPIx, 41, 0x40000000, CMD41CRC, 0);
			rcv = SDCardSPI_SendCommand(SPIx, 55, 0, 0x01, 0);
			rcv = SDCardSPI_SendCommand(SPIx, 41, 0x40000000, 0x01, 0);
			retry--;
		}while(rcv && retry);
        
//        rcv = SDCardSPI_SendCommand(SPIx, 55, 0, CMD55CRC, 0);
		
		/*******send CMD58,check SD2.0 or SD2.0HS***/
		SDCardSPI_SendCommand(SPIx, 58, 0, CMD58CRC, &recvalue);
		if(recvalue & 0x40000000)
			CardType = SDIO_HIGH_CAPACITY_SD_CARD;
		else
			CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;
//		SDCardSPI_SendCommand(&SDCard_Device, 59, 0, CMD59CRC, 0);
		
	}
	else
	{
//		/*******send CMD59,does not support crc***/	
//		SDCardSPI_SendCommand(&SDCard_Device, 59, 0, CMD59CRC, 0);
//		for(i = 0; i < 10000; i++)
//			SPI_FLASH_SendByte(SPIx, 0xFF);
		retry = 0xFF;
		do
		{
			/*******send CMD55,exit idle mode***/	
			rcv = SDCardSPI_SendCommand(SPIx, 55, 0, CMD55CRC, 0);
//			rcv = SDCardSPI_SendCommand(SPIx, 41, 0x40000000, CMD41CRC, 0);
            rcv = SDCardSPI_SendCommand(SPIx, 41, 0, 0xF8, 0);
			retry--;
		}while(rcv && retry);
		
		if((rcv==0x00)&&(retry!=0x00))
		{
			CardType =  SDIO_STD_CAPACITY_SD_CARD_V1_1;
		}
		else if((rcv!=0x00)&&(retry==0x00))
		{
			retry = 0xFF;
			do
			{
				rcv = SDCardSPI_SendCommand(SPIx, 1, 0, CMD1CRC, 0);
				retry--;
			}while(rcv && retry);
			if((rcv==0x00)&&(retry!=0x00))
			{
				CardType =  SDIO_HIGH_CAPACITY_MMC_CARD;
			}
		}
		
        /*******send CMD16,set block length***/	
        SDCardInfo.CardBlockSize = 512;
        if(SDCardSPI_SendCommand(SPIx, 16, SDCardInfo.CardBlockSize, 0xff, 0) > 0x01)
//      if(SDCardSPI_SendCommand(SPIx, 16, SDCardInfo.CardBlockSize, 0x01, 0) > 0x01)
            return SD_ERROR;
        
	}
	
//	SDCardSPI_SendCommand(&SDCard_Device, 3, 0, 0x01, &recvalue);  //SPI mode does not suport CMD3
	
	/*******Change SPI BaudRate 18MHz***/	
	ChangeSPIBaudRateForSDCard(SPIx);
	
	/*******send CMD10,read CID***/	
	if(SDCardSPI_GetCIDorCSD(SPIx, 10, CID_Tab) != 0xFE)
		return SD_ERROR;

	/*******send CMD9,read CSD***/
	if(SDCardSPI_GetCIDorCSD(SPIx, 9, CSD_Tab) != 0xFE)
		return SD_ERROR;
	
	SD_GetCardInfo(&SDCardInfo);
	
    /*******send CMD16,set block length***/	
    SDCardInfo.CardBlockSize = 512;
	if(SDCardSPI_SendCommand(SPIx, 16, SDCardInfo.CardBlockSize, 0xff, 0) > 0x01)
//    if(SDCardSPI_SendCommand(SPIx, 16, SDCardInfo.CardBlockSize, 0x01, 0) > 0x01)
		return SD_ERROR;
    
//    /*******Change SPI BaudRate 18MHz***/	
//	ChangeSPIBaudRateForSDCard(SPIx);
				
	return errorstatus;
}

/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : 等待SD卡Ready
* Input          : None
* Output         : None
* Return         : u8 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) uint8_t SDCardSPI_WaitReady(SPI_TypeDef *SPIx)
{
    uint8_t rcv;
    uint16_t retry;
    retry = 0;
    do
    {
        rcv = SPI_FLASH_SendByte(SPIx, 0xFF);
        if(retry == 0xfffe)
        {
            return 1;
        }
    }while(rcv != 0xFF);

    return 0;
}


/*******************************************************************************
* Function Name  : SD_SendCommand
* Description    : 向SD卡发送一个命令
* Input          : u8 cmd   命令 
*                  u32 arg  命令参数
*                  u8 crc   crc校验值
* Output         : None
* Return         : u8 r1 SD卡返回的响应
*******************************************************************************/
__attribute__((optimize(0))) uint8_t SDCardSPI_SendCommand(SPI_TypeDef *SPIx, uint8_t cmd, uint32_t arg, uint8_t crc, uint32_t *presdata)
{
	uint32_t temp = 0;
    uint16_t retry=0;
	uint8_t rcv = 0; 

	
	SPI_FLASH_SendByte(SPIx, 0xFF);
    SPI_FLASH_SendByte(SPIx, 0xFF);
    SPI_FLASH_SendByte(SPIx, 0xFF);
	Spi_Flash_cs_low(SPIx);
    SPI_FLASH_SendByte(SPIx, 0xFF);
	
//	Spi_Flash_cs_low(SPIx);
	
	/*******send command code***/
	SPI_FLASH_SendByte(SPIx,cmd | 0x40);
	
	/*******send argument data***/
	SPI_FLASH_SendByte(SPIx,arg >> 24);
	SPI_FLASH_SendByte(SPIx,arg >> 16);
	SPI_FLASH_SendByte(SPIx,arg >> 8);
	SPI_FLASH_SendByte(SPIx,arg);
	
	/*******send crc data***/
	SPI_FLASH_SendByte(SPIx,crc | 0x01);
	
    
//    Spi_Flash_cs_high(SPIx);
//    
//    Spi_Flash_cs_low(SPIx);
    
	if(cmd == 12)
		SPI_FLASH_SendByte(SPIx, 0xFF);
	
	retry = 0xFE;
	do
	{
		rcv = SPI_FLASH_SendByte(SPIx, 0xFF);
		retry--;
	}while((rcv & 0x80) && (retry > 0));
	
	if((cmd == 8) || (cmd == 58)|| (cmd == 13))
	{
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		
		if(presdata != 0)
			*presdata = temp;
	}

	Spi_Flash_cs_high(SPIx);
	
	return rcv;
}


/*******************************************************************************
* Function Name  : SDCardSPI_GetCIDorCSD
* Description    : Get CID or CSD
* Input          : u8 cmd   command 
*                  u32 arg  arguments
*                  u8 crc   crc value
* Output         : None
* Return         : u8 r1 SD card CID or CSD information
*******************************************************************************/
__attribute__((optimize(0))) uint8_t SDCardSPI_GetCIDorCSD(SPI_TypeDef *SPIx, uint8_t cmd, uint32_t *presdata) 
{
	uint8_t rcv = 0; 
	uint8_t retry = 0;
	uint32_t temp = 0;	
	
	if((cmd != 9) && (cmd != 10)&& (cmd != 13))
		return 0;
    
    SPI_FLASH_SendByte(SPIx, 0xFF);
	SPI_FLASH_SendByte(SPIx, 0xFF);
	
	//CMD10 or CMD9,CMD10 for CID,CMD9 for CSD 
	Spi_Flash_cs_low(SPIx);
	
	SPI_FLASH_SendByte(SPIx, 0xFF);
	SPI_FLASH_SendByte(SPIx, 0xFF);
	SPI_FLASH_SendByte(SPIx, 0xFF);
	SPI_FLASH_SendByte(SPIx, 0xFF);
	SPI_FLASH_SendByte(SPIx, 0xFF);
	
	/*******send command code***/
	SPI_FLASH_SendByte(SPIx,cmd | 0x40);
	
	/*******send argument data***/
	SPI_FLASH_SendByte(SPIx,0);
	SPI_FLASH_SendByte(SPIx,0);
	SPI_FLASH_SendByte(SPIx,0);
	SPI_FLASH_SendByte(SPIx,0);
	
	/*******send crc data***/
	SPI_FLASH_SendByte(SPIx, 0x01);
	
	retry = 0xFF;
	do
	{
		rcv = SPI_FLASH_SendByte(SPIx, 0xFF);
		retry--;
	}while((rcv != 0xFE) && (retry));
	
	if(rcv == 0xFE)
	{
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		*(presdata++) = temp;
		
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		*(presdata++) = temp;
		
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		*(presdata++) = temp;
		
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		temp <<= 8;
		temp |= SPI_FLASH_SendByte(SPIx, 0xFF);
		*(presdata++) = temp;
	}

	Spi_Flash_cs_high(SPIx);
	
	return rcv;
}

/*
 * 函数名：SD_GetCardInfo
 * 描述  ：获取SD卡的具体信息
 * 输入  ：-cardinfo 指向SD_CardInfo结构体的指针
 *         这个结构里面包含了SD卡的具体信息
 * 输出  ：-SD_Error SD卡错误代码
 *         成功时则为 SD_OK
 * 调用  ：外部调用
 */
__attribute__((optimize(0))) SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
  uint8_t tmp = 0;

  cardinfo->CardType = (uint8_t)CardType;
  cardinfo->RCA = (uint16_t)RCA;

  /*!< Byte 0 */
  tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;
	
  if(cardinfo->SD_csd.CSDStruct == 0)
  {
	  CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1;
	  cardinfo->CardType = (uint8_t)CardType;
  }

  /*!< Byte 1 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

  if (CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1)
  {
    cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
    
    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if ((CardType == SDIO_HIGH_CAPACITY_SD_CARD) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SD_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    
    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    cardinfo->CardBlockSize = 512;    
  }


  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;


  /*!< Byte 0 */
  tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;
  
  return(errorstatus);
}

/*******************************************************************************
* Function Name  : SDCardSPI_ReceiveData
* Description    : 从SD卡中读回指定长度的数据，放置在给定位置
* Input          : u8 *data(存放读回数据的内存>len)
*                  u16 len(数据长度）
*                  u8 release(传输完成后是否释放总线CS置高 0：不释放 1：释放）
* Output         : None
* Return         : u8 
*                  0：NO_ERR
*                  other：错误信息
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_ReceiveData(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len)
{
    uint16_t retry;
    uint8_t rcv;
//	uint8_t *dest;
//	uint8_t dat = 0xff;
	
    //wait for SD 0xFE
	
	/* Loop while DR register in not empty */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_TFE) == RESET);
	
	SPI_ReceiveData(SPIx);
	
    retry = 0;
    do
    {
		/* Send byte through the SPI1 peripheral */
		SPI_SendData(SPIx, 0xff);
		
		/* Wait to receive a byte */
		while (SPI_GetFlagStatus(SPIx, SPI_FLAG_RNE) == RESET);
		
        rcv = SPI_ReceiveData(SPIx);
		
        //rcv = SPI_FLASH_SendByte(SPIx, 0xFF);
        retry++;
        if(retry > 0xFFFE)  //2000次等待后没有应答，退出报错
        {
            return SD_ERROR;
        }
    }while(rcv != 0xFE);
	
    //start recive data
	
	/* Loop while DR register in not empty */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_TFE) == RESET);
	
	SPI_ReceiveData(SPIx);
	
	SPI_Hword_read(SPIx, data, len);
	
//	data--;
//	dest = data + len;
//	
//    do
//    {
//		/* Send byte through the SPI1 peripheral */
//		SPI_SendData(SPIx, dat);
//        data++;
//		/* Wait to receive a byte */
//		//while (SPI_GetFlagStatus(SPIx, SPI_FLAG_RNE) == RESET);
//		WaitSPIDone(5);
//		
//        *data = SPI_ReceiveData(SPIx);
//		
//        //data++;
//    }while(data != dest);

    // two dummy CRC data
    SPI_FLASH_SendByte(SPIx, 0xFF);
    SPI_FLASH_SendByte(SPIx, 0xFF);

    return SD_OK;
}

__attribute__((optimize(0))) void SPI_Hword_read(SPI_TypeDef* SPIx, uint8_t* pBuffer, uint16_t Length)
{
    int Remain; // Remaining Hword
    int LastByte;
    int count, i;
    uint16_t temp;
    int sspcr0;

    LastByte = Length & 0x1;
    Remain = Length >> 1;

    // switch to 16bit mode
    sspcr0 = SPIx->CR0;
    SPIx->CR0 |=0x8;

    do
    {
         if(Remain>=8)
         {
             count = 8;
             Remain -=8;
         }
         else
         {
             count = Remain;
             Remain = 0;
         }

         for(i=0;i<count;i++)
             SPI_SendData(SPIx,0xffff);

         /* Wait to receive buffer not empty */
         while (SPI_GetFlagStatus(SPIx, SPI_FLAG_RNE) == RESET);

         /* Return the byte read from the SPI bus */
         for(i=0;i<count;i++)
         {
           temp = SPI_ReceiveData(SPIx);
           *pBuffer++ = (uint8_t)(temp>>8);
           *pBuffer++ = (uint8_t) temp & 0xff;
           WaitSPIDone(1);
         }                                  

    }while(Remain!=0);

    // return to 8bit mode
    SPIx->CR0 = sspcr0;

    if(LastByte)
    {
         SPI_SendData(SPIx,0xff);

         /* Wait to receive a byte */
         while (SPI_GetFlagStatus(SPIx, SPI_FLAG_RNE) == RESET); 

         *pBuffer++ = SPI_ReceiveData(SPIx);

    }
}
/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : 读SD卡的一个block
* Input          : u32 sectorAddress 取地址（物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_ReadSingleBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize)
{
	uint8_t rcv;
	
	if ((CardType == SDIO_HIGH_CAPACITY_SD_CARD)|| (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
	{
		blocksize = 512;
		sectorAddress /= 512;
	}

	rcv = SDCardSPI_SendCommand(SPIx, 17, sectorAddress, 0x01, 0);//读命令
	
	if(rcv != 0x00)
    {
        return SD_ERROR;
    }
	
    Spi_Flash_cs_low(SPIx);
	
    rcv = SDCardSPI_ReceiveData(SPIx, buffer, blocksize);
	
	Spi_Flash_cs_high(SPIx);
	
    if(rcv != SD_OK)
    {
        return SD_ERROR;   //读数据出错！
    }

    return SD_OK;
}

__attribute__((optimize(0))) SD_Error SPI_ReadSingleBlock_test(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len)
{
//	uint8_t rcv;
	uint8_t *dest;
	uint8_t dat=0xff;
	
	/* Loop while DR register in not empty */
	while (SPI_GetFlagStatus(SPIx, SPI_FLAG_TFE) == RESET);
	
	SPI_ReceiveData(SPIx);
	
	data--;
	dest = data + len;
	
    do
    {
		/* Send byte through the SPI1 peripheral */
		SPI_SendData(SPIx, dat);
        data++;
		/* Wait to receive a byte */
		//while (SPI_GetFlagStatus(SPIx, SPI_FLAG_RNE) == RESET);
		WaitSPIDone(5);
		
        *data = SPI_ReceiveData(SPIx);
		
        //data++;
    }while(data != dest);
    return SD_OK;
}
/*******************************************************************************
* Function Name  : SDCardSPI_ReadSingleBlockAPI
* Description    : 使用SPI读SD卡的一个block
* Input          : u32 sectorAddress 取地址（物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_ReadSingleBlockAPI(uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize)
{
	return SDCardSPI_ReadSingleBlock(UPG_SdSpi, sectorAddress, buffer, blocksize);
}


/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : 读SD卡的多个block
* Input          : u32 u32 sectorAddress 取地址（物理地址）  
*                  u8 *buffer 数据存储地址（大小至少512byte）
*                  u8 count 连续读count个block
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_ReadMultiBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks)
{
	uint32_t count;
	uint8_t rcv;
	
	if ((CardType == SDIO_HIGH_CAPACITY_SD_CARD)|| (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
	{
		blocksize = 512;
		sectorAddress /= 512;
	}
	
	count = NumberOfBlocks;
    
    //SD_WaitReady();
    //发读多块命令
	rcv = SDCardSPI_SendCommand(SPIx, 18, sectorAddress, 0x01, 0);//读命令
	if(rcv != 0x00)
    {
        return SD_ERROR;
    }
	
	//开始接收数据
	Spi_Flash_cs_low(SPIx);
    
    do
    {
        if(SDCardSPI_ReceiveData(SPIx, buffer, blocksize) != SD_OK)
        {
            break;
        }
        buffer += 512;
    } while(--count);

    //全部传输完毕，发送停止命令
    SDCardSPI_SendCommand(SPIx, 12, 0, 0x01, 0);
    //释放总线
    Spi_Flash_cs_high(SPIx);
    SPI_FLASH_SendByte(SPIx, 0xFF);
    
    if(count != 0)
    {
        return SD_ERROR;   //如果没有传完，返回剩余个数
    }
    else
    {
        return SD_OK;
    }
}

/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : 使用SPI读SD卡的多个block
* Input          : u32 u32 sectorAddress 取地址（物理地址）  
*                  u8 *buffer 数据存储地址（大小至少512byte）
*                  u8 count 连续读count个block
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_ReadMultiBlockAPI(uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks)
{
	return SDCardSPI_ReadMultiBlock(UPG_SdSpi, sectorAddress, buffer, blocksize, NumberOfBlocks);
}

/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : 写入SD卡的一个block
* Input          :u32 sectorAddress 取地址（物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/

//有问题不能用，未使用
__attribute__((optimize(0))) SD_Error SDCardSPI_WriteSingleBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize)
{
	uint16_t i;
    uint16_t retry;
    uint8_t rcv;
	
	if ((CardType == SDIO_HIGH_CAPACITY_SD_CARD)|| (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
	{
		blocksize = 512;
		sectorAddress /= 512;
	}

    rcv = SDCardSPI_SendCommand(SPIx, 24, sectorAddress, 0x01, 0);
    if(rcv != 0x00)
    {
        return SD_ERROR;  //应答不正确，直接返回
    }
    
    //start a transaction
	Spi_Flash_cs_low(SPIx);
	
    //send 3 dummy data
    SPI_FLASH_SendByte(SPIx, 0xFF);
    SPI_FLASH_SendByte(SPIx, 0xFF);
    SPI_FLASH_SendByte(SPIx, 0xFF);
	
    //send token data(0xFE)
    SPI_FLASH_SendByte(SPIx, 0xFE);

    //send data
    for(i = 0;i < blocksize;i++)
    {
        SPI_FLASH_SendByte(SPIx, *buffer++);
    }
    //send dummy CRC
    SPI_FLASH_SendByte(SPIx, 0xFF);
    SPI_FLASH_SendByte(SPIx, 0xFF);
    
    //wait response
    rcv = SPI_FLASH_SendByte(SPIx, 0xFF);
    if((rcv & 0x1F) != 0x05)
    {
        Spi_Flash_cs_high(SPIx);
        return SD_ERROR;
    }
    
    //wait for finish the operation
    retry = 0;
    while(!SPI_FLASH_SendByte(SPIx, 0xFF))
    {
        retry++;
        if(retry > 0xfffe)        //如果长时间写入没有完成，报错退出
        {
            Spi_Flash_cs_high(SPIx);
            return SD_ERROR;           //写入超时返回1
        }
    }

    //disable the cs
    Spi_Flash_cs_high(SPIx);
	
    SPI_FLASH_SendByte(SPIx, 0xFF);

    return SD_OK;
}

/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : 使用SPI写入SD卡的一个block
* Input          :u32 sectorAddress 取地址（物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_WriteSingleBlockAPI(uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize)
{
	return SDCardSPI_WriteSingleBlock(UPG_SdSpi, sectorAddress, buffer, blocksize);
}

/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : 写入SD卡的N个block
* Input          : u32 sector 扇区地址（sector值，非物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte）
*                  u8 count 写入的block数目
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_WriteMultiBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks)
{
	uint32_t count = NumberOfBlocks;
	uint8_t rcv;
    uint16_t i;


    if ((CardType == SDIO_HIGH_CAPACITY_SD_CARD)|| (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
	{
		blocksize = 512;
		sectorAddress /= 512;
	}
	
    //如果目标卡不是MMC卡，启用ACMD23指令使能预擦除
//    if(SD_Type != SD_TYPE_MMC)
//    {
//        rcv = SD_SendCommand(ACMD23, count, 0x00);
//    }
	
    //发多块写入指令
	rcv = SDCardSPI_SendCommand(SPIx, 25, sectorAddress, 0x01, 0);
    if(rcv != 0x00)
    {
        return SD_ERROR;  //应答不正确，直接返回
    }
    
    //start a transaction
	Spi_Flash_cs_low(SPIx);
	
    ///send 3 dummy data
    SPI_FLASH_SendByte(SPIx, 0xFF);
    SPI_FLASH_SendByte(SPIx, 0xFF);
	SPI_FLASH_SendByte(SPIx, 0xFF);

    //--------下面是N个sector写入的循环部分
    do
    {
        //放起始令牌0xFC 表明是多块写入
        SPI_FLASH_SendByte(SPIx, 0xFC);
    
        //放一个sector的数据
        for(i = 0; i < blocksize; i++)
        {
            SPI_FLASH_SendByte(SPIx,*(buffer++));
        }
        //发2个Byte的dummy CRC
        SPI_FLASH_SendByte(SPIx, 0xFF);
        SPI_FLASH_SendByte(SPIx, 0xFF);
        
        //等待SD卡应答
        rcv = SPI_FLASH_SendByte(SPIx, 0xFF);
        if((rcv & 0x1F)!=0x05)
        {
            Spi_Flash_cs_high(SPIx);    //如果应答为报错，则带错误代码直接退出
            return SD_ERROR;
        }

        //等待SD卡写入完成
        if(SDCardSPI_WaitReady(SPIx)==1)
        {
            Spi_Flash_cs_high(SPIx);    //等待SD卡写入完成超时，直接退出报错
            return SD_ERROR;
        }

        //本sector数据传输完成
    }while(--count);
    
    //发结束传输令牌0xFD
    rcv = SPI_FLASH_SendByte(SPIx, 0xFD);
    if(rcv == 0x00)
    {
        count =  0xfe;
    }

    if(SDCardSPI_WaitReady(SPIx))
    {
        Spi_Flash_cs_high(SPIx);    //等待SD卡写入完成超时，直接退出报错
        return SD_ERROR;
    }
    
    //写入完成，片选置1
    Spi_Flash_cs_high(SPIx);
    SPI_FLASH_SendByte(SPIx, 0xFF);

    return SD_OK;   //返回count值，如果写完则count=0，否则count=1
}


/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : 使用SPI写入SD卡的N个block
* Input          : u32 sector 扇区地址（sector值，非物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte）
*                  u8 count 写入的block数目
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
__attribute__((optimize(0))) SD_Error SDCardSPI_WriteMultiBlockAPI(uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks)
{
	return SDCardSPI_WriteMultiBlock(UPG_SdSpi, sectorAddress, buffer, blocksize, NumberOfBlocks);
}


#endif
