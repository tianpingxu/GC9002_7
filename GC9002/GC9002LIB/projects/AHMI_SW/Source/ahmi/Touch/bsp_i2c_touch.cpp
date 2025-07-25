////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI Team
//
// Create Date:   2015/11/17
// File Name:     Trace.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////
#include "bsp_i2c_touch.h"

#ifdef USE_I2C_GPIO


#define Touch_I2C_SCL_1()	HAL_GPIO_WritePin(TOUCH_SCL_GPIO_PORT, TOUCH_SCL_GPIO_PIN, GPIO_PIN_SET)//GPIOB_SetBits(GPIO_Pin_8)   /* SCL = 1 */
#define Touch_I2C_SCL_0()	HAL_GPIO_WritePin(TOUCH_SCL_GPIO_PORT, TOUCH_SCL_GPIO_PIN, GPIO_PIN_RESET)//GPIOB_ResetBits(GPIO_Pin_8)  /* SCL = 0 */

#define Touch_I2C_SDA_1()	HAL_GPIO_WritePin(TOUCH_SDA_GPIO_PORT, TOUCH_SDA_GPIO_PIN, GPIO_PIN_SET)//GPIOB_SetBits(GPIO_Pin_9)   /* SDA = 1 */
#define Touch_I2C_SDA_0()	HAL_GPIO_WritePin(TOUCH_SDA_GPIO_PORT, TOUCH_SDA_GPIO_PIN, GPIO_PIN_RESET)//GPIOB_ResetBits(GPIO_Pin_9)  /* SDA = 0 */

#define Touch_I2C_SDA_READ()	HAL_GPIO_ReadPin(TOUCH_SDA_GPIO_PORT, TOUCH_SDA_GPIO_PIN)//GPIOB_ReadInputDataBit(GPIO_Pin_9)	/*read SDA*/
#endif


/*******************************************************************************
* Function Name  : AW2083_Delay
* Description    : I2C delay function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__attribute__((optimize(0)))static void Touch_I2C_Delay()
{
	uint32_t i;

	/*when CPU frequency is 72MHz and cycle index is 10£¬SCL frequency is 208KHz*/
	for (i = 0; i < 20; )   //i<10
	{
		i++;
	}
}

/*******************************************************************************
* Function Name  : AW2083_Delay
* Description    : I2C delay function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__attribute__((optimize(0)))static void Touch_I2C_Delay1_2()
{
	uint32_t i;

	/*when CPU frequency is 72MHz and cycle index is 10£¬SCL frequency is 208KHz*/
	for (i = 0; i < 100; )   //i<5
	{
		i++;
	}
}

/*******************************************************************************
* Function Name  : AW2083_Start
* Description    : I2C start function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Touch_I2C_Start(void)
{
#ifdef USE_I2C_GPIO
    Touch_I2C_SDA_1();
	Touch_I2C_SCL_1();
    Touch_I2C_Delay1_2();
	Touch_I2C_SDA_0();
    Touch_I2C_Delay1_2();
	Touch_I2C_SCL_0();
    Touch_I2C_Delay1_2();
#endif
    
#ifdef USE_I2C_Hardware
    I2C_Start(GC_I2C1);
#endif
}

/*******************************************************************************
* Function Name  : AW2083_Stop
* Description    : I2C stop function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Touch_I2C_Stop(void)
{  
#ifdef USE_I2C_GPIO    
    Touch_I2C_SDA_0();
	Touch_I2C_SCL_1();
    Touch_I2C_Delay1_2();
	Touch_I2C_SDA_1();
    Touch_I2C_Delay1_2();
#endif
    
#ifdef USE_I2C_Hardware    
    I2C_Stop(GC_I2C1);
#endif
}

/*******************************************************************************
* Function Name  : AW2083_SendByte
* Description    : send one byte by I2C
* Input          : ucByte	,the byte you will send
* Output         : None
* Return         : None
*******************************************************************************/
void Touch_I2C_SendByte(uint8_t ucByte)
{
#ifdef USE_I2C_GPIO   
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		if(ucByte & 0x80)
		{
			Touch_I2C_SDA_1();
		}
		else
		{
			Touch_I2C_SDA_0();
		}
		Touch_I2C_Delay1_2();
		Touch_I2C_SCL_1();
        Touch_I2C_Delay1_2();
		Touch_I2C_SCL_0();
        Touch_I2C_Delay1_2();
		if(i == 7)
		{
			Touch_I2C_SDA_1();
		}
		ucByte <<= 1;
	}
#endif

#ifdef USE_I2C_Hardware 
    I2C_WriteByte(GC_I2C1, ucByte);
#endif
    
}

/*******************************************************************************
* Function Name  : AW2083_ReadByte
* Description    : read one byte by I2C
* Input          : None
* Output         : None
* Return         : value, the read byte
*******************************************************************************/
uint8_t Touch_I2C_ReadByte(void)
{
#ifdef USE_I2C_GPIO 
    uint8_t i;
	uint8_t value = 0x00;
	for( i = 0; i < 8; i++)
	{
		value <<= 1;
		Touch_I2C_SCL_1();
        Touch_I2C_Delay1_2();
		if(Touch_I2C_SDA_READ())
		{
			value++;
		}
        Touch_I2C_Delay1_2();
		Touch_I2C_SCL_0();
        Touch_I2C_Delay1_2();
	}
    return value;
#endif
    
#ifdef USE_I2C_Hardware 
    return I2C_ReadByte(GC_I2C1);
#endif
}

/*******************************************************************************
* Function Name  : AW2083_WaitAck
* Description    : wait acknowledge
* Input          : None
* Output         : None
* Return         : re	, 1 or 0
*******************************************************************************/
uint8_t Touch_I2C_WaitAck(void)
{   
#ifdef USE_I2C_GPIO 
    uint8_t re;
	Touch_I2C_SDA_1();
    Touch_I2C_Delay1_2();
	Touch_I2C_SCL_1();
    Touch_I2C_Delay1_2();
	if(Touch_I2C_SDA_READ())
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
    Touch_I2C_Delay1_2();
	Touch_I2C_SCL_0();
    Touch_I2C_Delay1_2();
	return re;
#endif
    
#ifdef USE_I2C_Hardware 
    return I2C_ReadACK(GC_I2C1);
#endif
}

/*******************************************************************************
* Function Name  : AW2083_Ack
* Description    : send acknowledge
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Touch_I2C_Ack(void)
{
#ifdef USE_I2C_GPIO 
    Touch_I2C_SDA_0();
    Touch_I2C_Delay1_2();
	Touch_I2C_SCL_1();
	Touch_I2C_Delay1_2();;
	Touch_I2C_SCL_0();
    Touch_I2C_Delay1_2();
	Touch_I2C_SDA_1();
#endif
    
#ifdef USE_I2C_Hardware 
    I2C_WriteACK(GC_I2C1, 0);
#endif
}

/*******************************************************************************
* Function Name  : AW2083_NoAck
* Description    : dont send acknowledge
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Touch_I2C_NoAck(void)
{
#ifdef USE_I2C_GPIO 
    Touch_I2C_SDA_1();
    Touch_I2C_Delay1_2();
	Touch_I2C_SCL_1();
	Touch_I2C_Delay1_2();;
	Touch_I2C_SCL_0();
    Touch_I2C_Delay1_2();
#endif
    
#ifdef USE_I2C_Hardware 
    I2C_WriteACK(GC_I2C1, 1);
#endif
}

/*******************************************************************************
* Function Name  : AW2083_ConfGPIO
* Description    : config I2C control GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Touch_I2C_ConfI2C(void)
{
#ifdef USE_I2C_GPIO 

	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = TOUCH_SCL_GPIO_PIN;
	HAL_GPIO_Init(TOUCH_SCL_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = TOUCH_SDA_GPIO_PIN;
	HAL_GPIO_Init(TOUCH_SDA_GPIO_PORT, &GPIO_InitStruct);
    
    //i2c stop
	Touch_I2C_Stop();

#endif

#ifdef USE_I2C_Hardware 
    uint32_t i2c_boud =160000;
	uint32_t fosc = 21000000, Baud_Rate = 0;
	I2C_InitTypeDef I2C_InitStruct;
    
    Baud_Rate = ((int32_t)(fosc/i2c_boud - 1)/2);
	I2C_InitStruct.I2C_Mode = I2C_Mode_Master;
	I2C_InitStruct.APBCLKPRE = APBtoI2C_PRE_1;
	I2C_InitStruct.I2C_Baud_Rate = Baud_Rate;
	I2C_Init(GC_I2C1, &I2C_InitStruct);
    
    PACLK_ON;
    PA10_ALTF_CLR;
    PA10_INPUT_MODE;
    
    //i2c stop
	Touch_I2C_Stop();
#endif
}


/**
  * @brief   Read bytes by I2C
  * @param   
  *     @arg deviceAddr:Address of device.
  *		@arg pBuffer:pointer to data buffer.
  *		@arg NumByteToRead: number of data.
  * @retval
  */
uint32_t Touch_I2C_ReadBytes(uint8_t deviceAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{
	
	/* step 1: start I2C */
	Touch_I2C_Start();
	
	/* step 2: send device address and w/r control. */
	Touch_I2C_SendByte(deviceAddr | Touch_I2C_RD);
	
	/* step 3: wait ack. */
	if (Touch_I2C_WaitAck() != 0)
	{
		/* stop I2C */
        Touch_I2C_Stop();
        return 1;
	}

	Touch_I2C_Delay();

    while(NumByteToRead) 
    {

        
        *pBuffer = Touch_I2C_ReadByte();
        
        /* increment the pointer. */
        pBuffer++; 
		
        if(NumByteToRead == 1)
        {
            Touch_I2C_NoAck();	/* Read the last byte data,then cpu send NoACK(SDA = 1). */
        }  
        else 
		{
        	Touch_I2C_Ack();	/* Send ack(SDA = 0). */     
        }      
             
        /* decrement the counter. */
        NumByteToRead--;
 
    }

	/* Stop I2C. */
	Touch_I2C_Stop();
	return 0;	/* Seccessful. */
}

/**
  * @brief   Write bytes by I2C
  * @param   
  *     @arg deviceAddr:Address of device.
  *		@arg pBuffer:pointer to data buffer.
  *		@arg NumByteToWrite:number od data.
  * @retval
  */
uint32_t Touch_I2C_WriteBytes(uint8_t deviceAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite)
{
    uint16_t m;	


    Touch_I2C_Stop();
  
    for (m = 0; m < 10; m++)
    {				
        /* step 1: start I2C */
        Touch_I2C_Start();
        
        /* step 2: send device address and w/r control. */
        Touch_I2C_SendByte(deviceAddr | Touch_I2C_WR);	/* ?????? */
        
        /* step 3: wait ack. */
        if (Touch_I2C_WaitAck() == 0)
        {
            break;
        }
    }
    if (m  == 10)
    {
        Touch_I2C_Stop();
        return 1;
    }	

    Touch_I2C_Delay();
	
    while(NumByteToWrite--)
    {
        /* step 4:send data */
        Touch_I2C_SendByte(*pBuffer);
    
        /* step 5:wait ack. */
        if (Touch_I2C_WaitAck() != 0)
        {
            Touch_I2C_Stop();
            return 1;	
        }
        
        Touch_I2C_Delay();

        pBuffer++;	/* increment the pointer. */		
    }
	
	/* stop I2C */
	Touch_I2C_Stop();
	return 0;
}



/*********************************************END OF FILE**********************/

