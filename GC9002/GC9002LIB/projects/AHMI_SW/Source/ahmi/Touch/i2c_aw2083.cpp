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
#include "stdio.h"
#include "publicInclude.h"
#include "bsp_i2c_touch.h"
#include "i2c_aw2083.h"
#include "DynamicPageClass.h"
#include "AHMIInterface.h"
//#include "gc90x_gpio.h"
//#include "gc90x_i2c.h"
#include "gc90x_flash.h"
#include "AHMITouch.h"
#include "gc90xx_hal.h"


////#define USE_I2C_Hardware
//#define USE_I2C_GPIO

#define AW2083_WRITE_ADDR  0x90//I2C Writing address
#define AW2083_READ_ADDR   0x91//I2C Reading address


///* accelerate mode */
//#define AW2083_CW_READ_X	0x80//0x80 data set to control register to select x axis  
//#define AW2083_CW_READ_Y	0x90//0x90 data set to select y axis  
//#define AW2083_CW_READ_Z1   0xE0//data set to select z1 axis
//#define AW2083_CW_READ_Z2   0xF0//data set to select z2 axis

/* low power mode */
#define AW2083_CW_READ_X	0xC4 //data set to control register to select x axis  
#define AW2083_CW_READ_Y	0xD4 //data set to select y axis  
#define AW2083_CW_READ_Z1   0xE4 //data set to select z1 axis
#define AW2083_CW_READ_Z2   0xF4 //data set to select z2 axis


//#ifdef USE_I2C_GPIO
//#define AW2083_SCL_1()	GPIOB_SetBits(GPIO_Pin_8)   /* SCL = 1 */
//#define AW2083_SCL_0()	GPIOB_ResetBits(GPIO_Pin_8)  /* SCL = 0 */

//#define AW2083_SDA_1()	GPIOB_SetBits(GPIO_Pin_9)   /* SDA = 1 */
//#define AW2083_SDA_0()	GPIOB_ResetBits(GPIO_Pin_9)  /* SDA = 0 */

//#define AW2083_SDA_READ()	GPIOB_ReadInputDataBit(GPIO_Pin_9)	/*read SDA*/
//#endif


#define AW2083TOUCHX_MIN  (50) 
#define AW2083TOUCHX_MAX  (4046)// max and min touch x axis adc value
#define AW2083TOUCHY_MIN  (50) 
#define AW2083TOUCHY_MAX  (4046)// max and min touch y axis adc value
extern uint32_t flag_Calibration;//flag to indicate calibrated or not


/*******************************************************************************
* Function Name  : GetTouchState
* Description    : Get touchs state
* Input          : None
* Output         : None
* Return         : state
*******************************************************************************/
uint32_t AW2083_GetTouchState()
{
//	return GPIO_ReadInputDataBit(GPIO_PORT_AW2083_PENIRQN, AW2083_PENIRQN_PIN);
    
    return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);//GPIOA_ReadInputDataBit(GPIO_Pin_10);
}

/*******************************************************************************
* Function Name  : AW2083_Start
* Description    : I2C start function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AW2083_Start(void)
{
    Touch_I2C_Start();
}

/*******************************************************************************
* Function Name  : AW2083_Stop
* Description    : I2C stop function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AW2083_Stop(void)
{  
    Touch_I2C_Stop();
}

/*******************************************************************************
* Function Name  : AW2083_SendByte
* Description    : send one byte by I2C
* Input          : ucByte	,the byte you will send
* Output         : None
* Return         : None
*******************************************************************************/
void AW2083_SendByte(uint8_t ucByte)
{
    Touch_I2C_SendByte(ucByte);
    
}

/*******************************************************************************
* Function Name  : AW2083_ReadByte
* Description    : read one byte by I2C
* Input          : None
* Output         : None
* Return         : value, the read byte
*******************************************************************************/
uint8_t AW2083_ReadByte(void)
{
    return Touch_I2C_ReadByte();
}

/*******************************************************************************
* Function Name  : AW2083_WaitAck
* Description    : wait acknowledge
* Input          : None
* Output         : None
* Return         : re	, 1 or 0
*******************************************************************************/
uint8_t AW2083_WaitAck(void)
{   
    return Touch_I2C_WaitAck();
}

/*******************************************************************************
* Function Name  : AW2083_Ack
* Description    : send acknowledge
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AW2083_Ack(void)
{
    Touch_I2C_Ack();
}

/*******************************************************************************
* Function Name  : AW2083_NoAck
* Description    : dont send acknowledge
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AW2083_NoAck(void)
{
    Touch_I2C_NoAck();
}

/*******************************************************************************
* Function Name  : AW2083_ConfGPIO
* Description    : config I2C control GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AW2083_ConfI2C(void)
{
    Touch_I2C_ConfI2C();
}


/*******************************************************************************
* Function Name  : AW2083_InitDevice
* Description    : initial AW2083
* Input          : None
* Output         : None
* Return         : ucAck
*******************************************************************************/
uint8_t AW2083_InitDevice(void)
{
	uint8_t ucAck;
	
	//Config mcu control gpio to AW2083 and do i2c stop once
	AW2083_ConfI2C();
		
	//read calibrating data from stm32 flash and judge have calibarated or not  
//	ReadDataFromSTMFlashForTouch();
	
	//I2C start
 	AW2083_Start();
	
	//Send writing address
	AW2083_SendByte(AW2083_WRITE_ADDR);
	
	//wait acknowledge
	ucAck = AW2083_WaitAck();
	
	if(ucAck == 0)//acknowledge ok
	{
		AW2083_SendByte(AW2083_CW_READ_X);
		ucAck = AW2083_WaitAck();
	}
	
	//i2c stop
	AW2083_Stop();
	
	return ucAck;
}

/*******************************************************************************
* Function Name  : AW2083_ReadSinglePosition
* Description    : read one channel data
* Input          : AW2083_cw,channel
* Output         : None
* Return         : data,channel data
*******************************************************************************/
uint16_t AW2083_ReadSinglePosition(uint16_t AW2083_cw)
{
//	uint16_t data;
	uint16_t data_temp[8] = {0};
	uint16_t temp1 = 0, temp2 = 0;
	uint16_t i;
    uint16_t m, n, t = 0;
	
	//judeg AW2083_cw is valid or not
	if((AW2083_cw != AW2083_CW_READ_X) &&	(AW2083_cw != AW2083_CW_READ_Y) &&(AW2083_cw != AW2083_CW_READ_Z1) && (AW2083_cw != AW2083_CW_READ_Z2))
			return 0;
	
	//i2c start
	AW2083_Start();
	
	//send writing address 
	AW2083_SendByte(AW2083_WRITE_ADDR);
	
	//get acknowledge
	if(AW2083_WaitAck() != 0)
	{
		AW2083_Stop();
		return 0;
	}
	
	//send AW2083_cw to set contor register
	AW2083_SendByte(AW2083_cw);
	
	if(AW2083_WaitAck() != 0)
	{
		AW2083_Stop();
		return 0;
	}
		
	//get adc value 3 times
	for(i = 0; i < 8; i++)
	{
		AW2083_Start();
		AW2083_SendByte(AW2083_READ_ADDR);
		if(AW2083_WaitAck() != 0)
		{
			AW2083_Stop();
			return 0;
		}
		//12 bit precision so reading 2 times
		temp1 =  AW2083_ReadByte();//get high 8 bit
		AW2083_Ack();
		temp2 =  AW2083_ReadByte();//get low 4 bit ,MSB
		data_temp[i] = (temp1 << 4) |(temp2 >> 4);
		AW2083_NoAck();
	}
	
	AW2083_Stop();
	
	//average value
//	data = (data_temp[0] + data_temp[1] + data_temp[2]) /3;
    
    //Sort 8 data.
    for(m=0; m < 8-1; m++)
    {
        for(n=0; n < 8-m-1; n++)
        {
            if(data_temp[n] > data_temp[n+1])
            {
               t = data_temp[n+1];
               data_temp[n+1] = data_temp[n];
               data_temp[n] = t;
            }
        }
    }
	
    //get mean of 6 data.
	return (data_temp[1] + data_temp[2] + data_temp[3] + data_temp[4] + data_temp[5] + data_temp[6]) /6;  
    
//    return data;
}

/*******************************************************************************
* Function Name  : AW2083_ReadX
* Description    : read x channel data
* Input          : None
* Output         : None
* Return         : data: X channel data
*******************************************************************************/
uint16_t AW2083_ReadX(void)
{
	return AW2083_ReadSinglePosition(AW2083_CW_READ_X);
}

/*******************************************************************************
* Function Name  : AW2083_ReadY
* Description    : read y channel data
* Input          : None
* Output         : None
* Return         : data: y channel data
*******************************************************************************/
uint16_t AW2083_ReadY(void)
{
	return AW2083_ReadSinglePosition(AW2083_CW_READ_Y);
}


/*******************************************************************************
* Function Name  : AW2083_ReadZ1
* Description    : read z1 channel data
* Input          : None
* Output         : None
* Return         : data: z1 channel data
*******************************************************************************/
uint16_t AW2083_ReadZ1(void)
{
	return AW2083_ReadSinglePosition(AW2083_CW_READ_Z1);
}

/*******************************************************************************
* Function Name  : AW2083_ReadZ2
* Description    : read z2 channel data
* Input          : None
* Output         : None
* Return         : data: z2 channel data
*******************************************************************************/
uint16_t AW2083_ReadZ2(void)
{
	return AW2083_ReadSinglePosition(AW2083_CW_READ_Z2);
}

/*******************************************************************************
* Function Name  : AW2083_ReadPosition
* Description    : store read location to CurPositon
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
AW2083Touch_tsWorkbuf mAW2083Touch_tsWorkbuf;
extern TouchFSM g_TM_stTouchFsm;
void AW2083touch_vMain(void)
{
	int r_touch = 0;
	uint16_t temp_r;
	
	switch(mAW2083Touch_tsWorkbuf.teRtpManSM)
    {
        case AW2083Touch_GetTouchState:
			mAW2083Touch_tsWorkbuf.uint16XValue = 0;
			mAW2083Touch_tsWorkbuf.uint16YValue = 0;
			mAW2083Touch_tsWorkbuf.uint16Z1Value = 0;
			mAW2083Touch_tsWorkbuf.uint16Z2Value = 0;
			if(AW2083_GetTouchState() == 0)
			{
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetXValue;
			}
			else
			{
				g_TM_stTouchFsm.mPosi.event = NOTOUCH;
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
			}
            break;
        case AW2083Touch_GetXValue:
			if(AW2083_GetTouchState() == 0)
			{
				mAW2083Touch_tsWorkbuf.uint16XValue = AW2083_ReadX();//get x adc value
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetYValue;
			}
			else
			{
				g_TM_stTouchFsm.mPosi.event = NOTOUCH;
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
			}
            break;
        case AW2083Touch_GetYValue:
			if(AW2083_GetTouchState() == 0)
			{
				mAW2083Touch_tsWorkbuf.uint16YValue = AW2083_ReadY();//get y adc value
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetZ1Value;
			}
			else
			{
				g_TM_stTouchFsm.mPosi.event = NOTOUCH;
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
			}
            break;
        case AW2083Touch_GetZ1Value: 
			if(AW2083_GetTouchState() == 0)
			{
				mAW2083Touch_tsWorkbuf.uint16Z1Value = AW2083_ReadZ1();//get z1 adc value
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetZ2Value;
			}
			else
			{
				g_TM_stTouchFsm.mPosi.event = NOTOUCH;
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
			}			
            break;
        case AW2083Touch_GetZ2Value:
			if(AW2083_GetTouchState() == 0)
			{
				mAW2083Touch_tsWorkbuf.uint16Z2Value = AW2083_ReadZ2();//get z2 adc value
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetPosition;
			}
			else
			{
				g_TM_stTouchFsm.mPosi.event = NOTOUCH;
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
			}
            break;
        case AW2083Touch_GetPosition:
			if(AW2083_GetTouchState() == 0)
			{
				//judge reading fail or not
				if((0 == mAW2083Touch_tsWorkbuf.uint16Z1Value) || (0 == mAW2083Touch_tsWorkbuf.uint16Z2Value))
				{
					mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
					return;
				}
				
				//judge x value and y value beyond range or not
				if((mAW2083Touch_tsWorkbuf.uint16XValue < AW2083TOUCHX_MIN) || (mAW2083Touch_tsWorkbuf.uint16XValue > AW2083TOUCHX_MAX) || (mAW2083Touch_tsWorkbuf.uint16YValue < AW2083TOUCHY_MIN) || (mAW2083Touch_tsWorkbuf.uint16YValue > AW2083TOUCHY_MAX))
				{
					mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
					return;
				}
				
				r_touch = mAW2083Touch_tsWorkbuf.uint16XValue * (mAW2083Touch_tsWorkbuf.uint16Z2Value - mAW2083Touch_tsWorkbuf.uint16Z1Value) * 100;
				r_touch = r_touch / (mAW2083Touch_tsWorkbuf.uint16Z1Value * 4096);
				temp_r = (uint16_t)r_touch;
				
				if(flag_Calibration == 1)//touch srceen have calibrated
				{
					if((temp_r > 20) && (temp_r < 350))//judeg pressure vaue range
					{
						g_TM_stTouchFsm.mPosi.pos_x = mAW2083Touch_tsWorkbuf.uint16XValue;
						g_TM_stTouchFsm.mPosi.pos_y = mAW2083Touch_tsWorkbuf.uint16YValue;
						g_TM_stTouchFsm.mPosi.event = TOUCHED;
					}
					else
					{
						g_TM_stTouchFsm.mPosi.pos_x = 0;
						g_TM_stTouchFsm.mPosi.pos_y = 0;
						g_TM_stTouchFsm.mPosi.event = NOTOUCH;
					}
				}
				else
				{
					if((temp_r > 75) && (temp_r < 300))
					{
						g_TM_stTouchFsm.mPosi.pos_x = mAW2083Touch_tsWorkbuf.uint16XValue;
						g_TM_stTouchFsm.mPosi.pos_y = mAW2083Touch_tsWorkbuf.uint16YValue;
						g_TM_stTouchFsm.mPosi.event = TOUCHED;
					}
					else
					{
						g_TM_stTouchFsm.mPosi.pos_x = 0;
						g_TM_stTouchFsm.mPosi.pos_y = 0;
						g_TM_stTouchFsm.mPosi.event = NOTOUCH;
					}
				}
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
			}
			else
			{
				g_TM_stTouchFsm.mPosi.event = NOTOUCH;
				mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
			}
            break;
        default:
			g_TM_stTouchFsm.mPosi.event = NOTOUCH;
			mAW2083Touch_tsWorkbuf.teRtpManSM = AW2083Touch_GetTouchState;
            break;
    }
}

void AW2083_ReadPosition(Position* CurPosition)
{
	int r_touch = 0;
	uint16_t temp_x; 
	uint16_t temp_y;
	uint16_t temp_z1;
	uint16_t temp_z2;
	uint16_t temp_r;
    
    if(AW2083_GetTouchState() == 0)
    {         
        temp_x = AW2083_ReadX();//get x adc value
        temp_y = AW2083_ReadY();//get y adc value
        temp_z1 = AW2083_ReadZ1();//get z1 adc value
        temp_z2 = AW2083_ReadZ2();//get z2 adc value
        
		//judge reading fail or not
		if((0 == temp_z1) || (0 == temp_z2))
		{
			CurPosition->pos_x = 0;
			CurPosition->pos_y = 0;
			CurPosition->event = NOTOUCH;
			return;
		}
		
		//judge x value and y value beyond range or not
		if((temp_x < AW2083TOUCHX_MIN) || (temp_x > AW2083TOUCHX_MAX) || (temp_y < AW2083TOUCHY_MIN) || (temp_y > AW2083TOUCHY_MAX))
		{
			CurPosition->pos_x = 0;
			CurPosition->pos_y = 0;
			CurPosition->event = NOTOUCH;
			return;
		}
		
		//get pressure value		
		//r_touch = (float)temp_y / 4096.0;
		//r_touch = temp_y * ((temp_z2 * 10) / temp_z1 -10) / 4096;
        r_touch = temp_x * (temp_z2 - temp_z1) * 100;
        r_touch = r_touch / (temp_z1 * 4096);
        temp_r = (uint16_t)r_touch;
        if(flag_Calibration == 1)//touch srceen have calibrated
        {
            if((temp_r > 20) && (temp_r < 350))//judeg pressure vaue range
            {
				CurPosition->pos_x = temp_x;
				CurPosition->pos_y = temp_y;
				CurPosition->event = TOUCHED;
            }
            else
            {
                CurPosition->pos_x = 0;
                CurPosition->pos_y = 0;
                CurPosition->event = NOTOUCH;
            }
        }
        else
        {
            if((temp_r > 75) && (temp_r < 300))
            {
				CurPosition->pos_x = temp_x;
				CurPosition->pos_y = temp_y;
				CurPosition->event = TOUCHED;
            }
            else
            {
                CurPosition->pos_x = 0;
                CurPosition->pos_y = 0;
                CurPosition->event = NOTOUCH;
            }
        }
    }
	else
	{
		CurPosition->pos_x = 0;
		CurPosition->pos_y = 0;
		CurPosition->event = NOTOUCH;
	}
}

