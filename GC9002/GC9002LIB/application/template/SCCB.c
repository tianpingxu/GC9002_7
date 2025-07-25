#include "SCCB.h"

/*
-----------------------------------------------
   ¹¦ÄÜ: ³õÊ¼»¯Ä£ÄâSCCB½Ó¿Ú
   ²ÎÊý: ÎÞ
 ·µ»ØÖµ: ÎÞ
-----------------------------------------------
*/
void SCCB_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Pin =  SCCB_SIC_BIT|SCCB_SID_BIT;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_InitStruct.Pin, GPIO_PIN_SET);
}
void SCCB_SID_GPIO_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_StructInit(&GPIO_InitStruct);
  
    GPIO_InitStruct.Pin =  SCCB_SID_BIT;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void SCCB_SID_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin =  SCCB_SID_BIT;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void delay_us(uint32_t us)
{
    while(us--)
    {
        for(int i = 0; i < 2; i++)
        {
            __NOP();
        }
    }
}
/*
-----------------------------------------------
   ¹¦ÄÜ: startÃüÁî,SCCBµÄÆðÊ¼ÐÅºÅ
   ²ÎÊý: ÎÞ
 ·µ»ØÖµ: ÎÞ
-----------------------------------------------
*/
void startSCCB(void)
{
    SCCB_SID_H();     //Êý¾ÝÏß¸ßµçÆ½
    delay_us(500);

    SCCB_SIC_H();	   //ÔÚÊ±ÖÓÏß¸ßµÄÊ±ºòÊý¾ÝÏßÓÉ¸ßÖÁµÍ
    delay_us(500);
 
    SCCB_SID_L();
    delay_us(500);

    SCCB_SIC_L();	 //Êý¾ÝÏß»Ö¸´µÍµçÆ½£¬µ¥²Ù×÷º¯Êý±ØÒª
    delay_us(500);


}
/*
-----------------------------------------------
   ¹¦ÄÜ: stopÃüÁî,SCCBµÄÍ£Ö¹ÐÅºÅ
   ²ÎÊý: ÎÞ
 ·µ»ØÖµ: ÎÞ
-----------------------------------------------
*/
void stopSCCB(void)
{
    SCCB_SID_L();
    delay_us(500);
 
    SCCB_SIC_H();	
    delay_us(500);
  

    SCCB_SID_H();	
    delay_us(500);
   
}

/*
-----------------------------------------------
   ¹¦ÄÜ: noAck,ÓÃÓÚÁ¬Ðø¶ÁÈ¡ÖÐµÄ×îºóÒ»¸ö½áÊøÖÜÆÚ
   ²ÎÊý: ÎÞ
 ·µ»ØÖµ: ÎÞ
-----------------------------------------------
*/
void noAck(void)
{
	
	SCCB_SID_H();	
	delay_us(500);
	
	SCCB_SIC_H();	
	delay_us(500);
	
	SCCB_SIC_L();	
	delay_us(500);
	
	SCCB_SID_L();	
	delay_us(500);

}

/*
-----------------------------------------------
   ¹¦ÄÜ: Ð´ÈëÒ»¸ö×Ö½ÚµÄÊý¾Ýµ½SCCB
   ²ÎÊý: Ð´ÈëÊý¾Ý
 ·µ»ØÖµ: ·¢ËÍ³É¹¦·µ»Ø1£¬·¢ËÍÊ§°Ü·µ»Ø0
-----------------------------------------------
*/
unsigned char SCCBwriteByte(unsigned char m_data)
{
	unsigned char j,tem;

	for(j=0;j<8;j++) //Ñ­»·8´Î·¢ËÍÊý¾Ý
	{
		if((m_data<<j)&0x80)
		{
			SCCB_SID_H();	
		}
		else
		{
			SCCB_SID_L();	
		}
		delay_us(500);
		SCCB_SIC_H();	
		delay_us(500);
		SCCB_SIC_L();	
		delay_us(500);

	}
	delay_us(100);
	SCCB_SID_IN;/*ÉèÖÃSDAÎªÊäÈë*/
	delay_us(500);
	SCCB_SIC_H();	
	delay_us(100);
	if(SCCB_SID_STATE){tem=0;}   //SDA=1·¢ËÍÊ§°Ü£¬·µ»Ø0}
	else {tem=1;}   //SDA=0·¢ËÍ³É¹¦£¬·µ»Ø1
	SCCB_SIC_L();	
	delay_us(500);	
        SCCB_SID_OUT;/*ÉèÖÃSDAÎªÊä³ö*/

	return (tem);  
}

/*
-----------------------------------------------
   ¹¦ÄÜ: Ò»¸ö×Ö½ÚÊý¾Ý¶ÁÈ¡²¢ÇÒ·µ»Ø
   ²ÎÊý: ÎÞ
 ·µ»ØÖµ: ¶ÁÈ¡µ½µÄÊý¾Ý
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
	unsigned char read,j;
	read=0x00;
	
	SCCB_SID_IN;/*ÉèÖÃSDAÎªÊäÈë*/
	delay_us(500);
	for(j=8;j>0;j--) //Ñ­»·8´Î½ÓÊÕÊý¾Ý
	{		     
		delay_us(500);
		SCCB_SIC_H();
		delay_us(500);
		read=read<<1;
		if(SCCB_SID_STATE) 
		{
			read=read+1;
		}
		SCCB_SIC_L();
		delay_us(500);
	}	
	return(read);
}
