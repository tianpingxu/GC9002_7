/**
  ******************************************************************************
  * @file    gc90x_bt656.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the BT565 
  *          firmware library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Grephichina.com</center></h2>
  ******************************************************************************
  */

#include "gc90x_bt656.h"
//#include "drawImmediately_cd.h"
#include "csi_irq.h"
#include "csi_hal_core.h"

extern uint32_t SizeOfCVBSBuffer;
extern uint32_t StartAddrOfCVBS0Even;

static VIDEOCAP_HandleTypeDef hvideo = {.Instance = VIDEOCAP};

/**
  * @brief  Initializes the BT656 module.
  * @param  BT656_InitStruct: pointer to a BT656_InitTypeDef structure
  *         that contains the configuration information for the BT565.
  * @retval None
  */
//void BT656_Init(BT656_InitTypeDef *BT656_InitStruct)
//{
//    /* If the pointer is NULL. */
//    if(BT656_InitStruct == NULL)
//        return;
//    /* Reset status.*/
//	BT656_REG->InterruptStatus = 0;
//    
//    /* Reset interrupt control register. */
//	BT656_REG->InterruptControl = 0;
//    
//    /* Config interrupt control register. */
//	BT656_REG->InterruptControl = BT656_InitStruct->FrameDrawDoneInterrupt + (BT656_InitStruct->WriteFifoFullInterrrupt << 8);
//	
//    /* Config base address of chunnel. */
//    BT656_REG->CH0BaseAddr = BT656_InitStruct->CH0FieldBaseAddr;
//    
//    /* Config max size of vedio buffer. */
//	BT656_REG->MaxSize = SizeOfCVBSBuffer;
//    
//    /* Config and enable the BT565 module. */
//	BT656_REG->Control = BT656_InitStruct->Enable + (BT656_InitStruct->CHnum << 3) + (BT656_InitStruct->ColorBit << 1) + (BT656_InitStruct->FieldSampleMode << 8);
//}

#define OV7670_ADDR    0x42
//#define LOG_E

#define I2C_ACK   0
#define I2C_NACK  1

#define OV7670_GPIO_PORT    GPIOB          //GPIO端口
#define OV7670_SCL_PIN      GPIO_PIN_15    //连接到SCL时钟线的GPIO
#define OV7670_SDA_PIN      GPIO_PIN_14    //连接到SDA数据线的GPIO
#define OV7670_RST_PIN      GPIO_PIN_13    //连接到RST复位线的GPIO

#define OV7670_SCL_1()      HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_SCL_PIN, GPIO_PIN_SET)
#define OV7670_SCL_0()      HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_SCL_PIN, GPIO_PIN_RESET)
#define OV7670_SDA_1()      HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_SDA_PIN, GPIO_PIN_SET)
#define OV7670_SDA_0()      HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_SDA_PIN, GPIO_PIN_RESET)
#define OV7670_SDA_READ()   HAL_GPIO_ReadPin(OV7670_GPIO_PORT, OV7670_SDA_PIN)

//OV7670正常运行模式下PWDN引脚拉低，RST引脚拉高
//#define OV7670_PWDN         HAL_GPIO_WritePin(OV7670_GPIO_PORT, GPIO_PIN_X, GPIO_PIN_SET)
//#define OV7670_PWUP         HAL_GPIO_WritePin(OV7670_GPIO_PORT, GPIO_PIN_X, GPIO_PIN_RESET)
//#define OV7670_RST          HAL_GPIO_WritePin(OV7670_GPIO_PORT, GPIO_PIN_2, GPIO_PIN_RESET)
//#define OV7670_RUN          HAL_GPIO_WritePin(OV7670_GPIO_PORT, GPIO_PIN_2, GPIO_PIN_SET)

static void I2C_Delay(void)
{
    for(uint32_t i=0; i<40; i++) __NOP();  //50us
}

static void I2C_Start(void)
{
    OV7670_SCL_1();
    OV7670_SDA_1();
    I2C_Delay();
    OV7670_SDA_0();
    I2C_Delay();
    OV7670_SCL_0();
    I2C_Delay();
}

static void I2C_Stop(void)
{
    OV7670_SDA_0();
    OV7670_SCL_1();
    I2C_Delay();
    OV7670_SDA_1();
    I2C_Delay();
}

static uint8_t I2C_WaitAck(void)
{
    uint8_t r = I2C_NACK;

    OV7670_SDA_1();
    I2C_Delay();
    OV7670_SCL_1();
    I2C_Delay();
    if(OV7670_SDA_READ())
    {
        r = I2C_NACK;
    }
    else
    {
        r = I2C_ACK;
    }
    OV7670_SCL_0();
    I2C_Delay();

    return r;
}

static void I2C_Ack(void)
{
    OV7670_SDA_0();
    I2C_Delay();
    OV7670_SCL_1();
    I2C_Delay();
    OV7670_SCL_0();
    I2C_Delay();
    OV7670_SDA_1();
    I2C_Delay();

}

static void I2C_NAck(void)
{
    OV7670_SDA_1();
    I2C_Delay();
    OV7670_SCL_1();
    I2C_Delay();
    OV7670_SCL_0();
    I2C_Delay();
}

static void I2C_Send_Byte(uint8_t byte)
{
    /*先发送字节的高bit*/
    for(uint8_t i=0; i<8; i++)
    {
        if(byte & 0x80)
        {
            OV7670_SDA_1();
        }
        else
        {
            OV7670_SDA_0();
        }

        I2C_Delay();
        OV7670_SCL_1();
        I2C_Delay();
        OV7670_SCL_0();
        I2C_Delay();

        byte <<= 1;
    }

    OV7670_SDA_1();
    I2C_Delay();
}

static uint8_t I2C_Read_Byte(void)
{
    uint8_t value = 0;
    /*读取到第一个bit为数据的bit7*/
    for(uint8_t i = 0; i<8; i++)
    {
        value <<= 1;
        OV7670_SCL_1();
        I2C_Delay();
        if(OV7670_SDA_READ())
        {
            value++;
        }
        OV7670_SCL_0();
        I2C_Delay();
    }

    return value;
}

//注意：调用一次此接口后可以获取到寄存器的数据，但是也会导致摄像头配置全部复位
int8_t OV7670_Read_Byte(uint8_t regIdx, uint8_t *pVal)
{
    I2C_Start();

    I2C_Send_Byte(OV7670_ADDR & 0xFE);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("read step1, wait ack failed.");
        return -1;
    }

    I2C_Send_Byte(regIdx);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("read step2, wait ack failed.");
        return -1;
    }

    I2C_Stop();


    I2C_Start();
    I2C_Send_Byte((OV7670_ADDR & 0xFE) | 0x01);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("read step3, wait ack failed.");
        return -1;
    }

    *pVal = I2C_Read_Byte();

    I2C_NAck();

    I2C_Stop();

    return 0;
}

int8_t OV7670_Write_Byte(uint8_t regIdx, uint8_t val)
{
    I2C_Start();

    I2C_Send_Byte(OV7670_ADDR & 0xFE);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("write step1, wait ack failed.");
        return -1;
    }

    I2C_Send_Byte(regIdx);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("write step2, wait ack failed.");
        return -1;
    }

    I2C_Send_Byte(val);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("write step3, wait ack failed.");
        return -1;
    }

    I2C_Stop();

    return 0;
}

__attribute__((optimize(0))) void OV7670_Init(void)
{
	uint32_t i;
    uint8_t reg = 0;

    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_StructInit(&GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  //此处必须是开漏模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
    GPIO_InitStruct.Pin = OV7670_SDA_PIN | OV7670_SCL_PIN;
    HAL_GPIO_Init(OV7670_GPIO_PORT, &GPIO_InitStruct);

	HAL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = OV7670_RST_PIN;
	HAL_GPIO_Init(OV7670_GPIO_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_RST_PIN, GPIO_PIN_SET);

    I2C_Stop();
	for(i = 0; i < 10000; i++);
	
    OV7670_Write_Byte(0x12, 0x80);
	for(i = 0; i < 10000; i++);

    OV7670_Write_Byte(0x3A, 0x05);  //配置YUV输出顺序为YUYV
	for(i = 0; i < 10000; i++);

    uint16_t width = 384, height = 384;
    uint16_t startx = 272, starty = 12;
    uint16_t endx = (startx + width);
    uint16_t endy = (starty + height);
    uint16_t temp = 0;

    temp = (startx & 0x7F8) >> 3;
    OV7670_Write_Byte(0x17, temp);  //配置窗口水平方向(X)起始坐标高8位
    temp = (endx & 0x7F8) >> 3;
    OV7670_Write_Byte(0x18, temp);  //配置窗口水平方向(X)结束坐标高8位
    temp = 0x80 | ((endx & 0x7) << 3) | (startx & 0x7);
    OV7670_Write_Byte(0x32, temp);  //配置窗口水平方向(X)起始和结束坐标低3位

	for(i = 0; i < 10000; i++);

    temp = (starty & 0x3FC) >> 2;
    OV7670_Write_Byte(0x19, temp);  //配置窗口垂直方向(Y)起始坐标高8位
    temp = (endy & 0x3FC) >> 2;
    OV7670_Write_Byte(0x1A, temp);  //配置窗口垂直方向(Y)结束坐标高8位
    temp = ((endy & 0x3) << 2) | (starty & 0x3);
    OV7670_Write_Byte(0x03, temp);  //配置窗口垂直方向(Y)起始和结束坐标低2位

	for(i = 0; i < 10000; i++);
    //显示正常
//    OV7670_Write_Byte(0x17, 0x16);
//    OV7670_Write_Byte(0x18, 0x04);
//    OV7670_Write_Byte(0x32, 0x80);
//    OV7670_Write_Byte(0x19, 0x02);
//    OV7670_Write_Byte(0x1A, 0x7B);
//    OV7670_Write_Byte(0x03, 0x06);

    //显示正常
//    OV7670_Write_Byte(0x17, 0x13);
//    OV7670_Write_Byte(0x18, 0x01);
//    OV7670_Write_Byte(0x32, 0xb6);
//    OV7670_Write_Byte(0x19, 0x02);
//    OV7670_Write_Byte(0x1a, 0x7a);
//    OV7670_Write_Byte(0x03, 0x0a);


    OV7670_Write_Byte(0x0c, 0x00);
    OV7670_Write_Byte(0x3e, 0x00);
    OV7670_Write_Byte(0x70, 0x3a);
    OV7670_Write_Byte(0x71, 0x35);
    OV7670_Write_Byte(0x72, 0x11);
    OV7670_Write_Byte(0x73, 0xf0);
    OV7670_Write_Byte(0xa2, 0x02);
    OV7670_Write_Byte(0x7a, 0x20);
    OV7670_Write_Byte(0x7b, 0x10);
    OV7670_Write_Byte(0x7c, 0x1e);
    OV7670_Write_Byte(0x7d, 0x35);
    OV7670_Write_Byte(0x7e, 0x5a);
    OV7670_Write_Byte(0x7f, 0x69);
    OV7670_Write_Byte(0x80, 0x76);
    OV7670_Write_Byte(0x81, 0x80);
    OV7670_Write_Byte(0x82, 0x88);
    OV7670_Write_Byte(0x83, 0x8f);
    OV7670_Write_Byte(0x84, 0x96);
    OV7670_Write_Byte(0x85, 0xa3);
    OV7670_Write_Byte(0x86, 0xaf);
    OV7670_Write_Byte(0x87, 0xc4);
    OV7670_Write_Byte(0x88, 0xd7);
    OV7670_Write_Byte(0x89, 0xe8);
    OV7670_Write_Byte(0x13, 0xe0);
    OV7670_Write_Byte(0x01, 0x50);
    OV7670_Write_Byte(0x02, 0x68);
    OV7670_Write_Byte(0x00, 0x00);
    OV7670_Write_Byte(0x10, 0x00);
    OV7670_Write_Byte(0x0d, 0x40);
    OV7670_Write_Byte(0x14, 0x18);
    OV7670_Write_Byte(0xa5, 0x07);
    OV7670_Write_Byte(0xab, 0x08);
    OV7670_Write_Byte(0x24, 0x95);
    OV7670_Write_Byte(0x25, 0x33);
    OV7670_Write_Byte(0x26, 0xe3);
    OV7670_Write_Byte(0x9f, 0x78);
    OV7670_Write_Byte(0xa0, 0x68);
    OV7670_Write_Byte(0xa1, 0x03);
    OV7670_Write_Byte(0xa6, 0xd8);
    OV7670_Write_Byte(0xa7, 0xd8);
    OV7670_Write_Byte(0xa8, 0xf0);
    OV7670_Write_Byte(0xa9, 0x90);
    OV7670_Write_Byte(0xaa, 0x94);
    OV7670_Write_Byte(0x13, 0xe5);
    OV7670_Write_Byte(0x0e, 0x61);
    OV7670_Write_Byte(0x0f, 0x4b);
    OV7670_Write_Byte(0x16, 0x02);
    OV7670_Write_Byte(0x1e, 0x07);
    OV7670_Write_Byte(0x21, 0x02);
    OV7670_Write_Byte(0x22, 0x91);
    OV7670_Write_Byte(0x29, 0x07);
    OV7670_Write_Byte(0x33, 0x0b);
    OV7670_Write_Byte(0x35, 0x0b);
    OV7670_Write_Byte(0x37, 0x1d);
    OV7670_Write_Byte(0x38, 0x71);
    OV7670_Write_Byte(0x39, 0x2a);
    OV7670_Write_Byte(0x3c, 0x78);
    OV7670_Write_Byte(0x4d, 0x40);
    OV7670_Write_Byte(0x4e, 0x20);
    OV7670_Write_Byte(0x69, 0x00);
    OV7670_Write_Byte(0x6b, 0x0a);
    OV7670_Write_Byte(0x74, 0x10);
    OV7670_Write_Byte(0x8d, 0x4f);
    OV7670_Write_Byte(0x8e, 0x00);
    OV7670_Write_Byte(0x8f, 0x00);
    OV7670_Write_Byte(0x90, 0x00);
    OV7670_Write_Byte(0x91, 0x00);
    OV7670_Write_Byte(0x92, 0x66);
    OV7670_Write_Byte(0x96, 0x00);
    OV7670_Write_Byte(0x9a, 0x80);
    OV7670_Write_Byte(0xb0, 0x84);
    OV7670_Write_Byte(0xb1, 0x0c);
    OV7670_Write_Byte(0xb2, 0x0e);
    OV7670_Write_Byte(0xb3, 0x82);
    OV7670_Write_Byte(0xb8, 0x0a);
    OV7670_Write_Byte(0x43, 0x14);
    OV7670_Write_Byte(0x44, 0xf0);
    OV7670_Write_Byte(0x45, 0x34);
    OV7670_Write_Byte(0x46, 0x58);
    OV7670_Write_Byte(0x47, 0x28);
    OV7670_Write_Byte(0x48, 0x3a);
    OV7670_Write_Byte(0x59, 0x88);
    OV7670_Write_Byte(0x5a, 0x88);
    OV7670_Write_Byte(0x5b, 0x44);
    OV7670_Write_Byte(0x5c, 0x67);
    OV7670_Write_Byte(0x5d, 0x49);
    OV7670_Write_Byte(0x5e, 0x0e);
    OV7670_Write_Byte(0x64, 0x04);
    OV7670_Write_Byte(0x65, 0x20);
    OV7670_Write_Byte(0x66, 0x05);
    OV7670_Write_Byte(0x94, 0x04);
    OV7670_Write_Byte(0x95, 0x08);
    OV7670_Write_Byte(0x6c, 0x0a);
    OV7670_Write_Byte(0x6d, 0x55);
    OV7670_Write_Byte(0x6e, 0x11);
    OV7670_Write_Byte(0x6f, 0x9f);
    OV7670_Write_Byte(0x6a, 0x40);
    OV7670_Write_Byte(0x01, 0x40);
    OV7670_Write_Byte(0x02, 0x40);
    OV7670_Write_Byte(0x13, 0xe7);
    OV7670_Write_Byte(0x4f, 0x80);
    OV7670_Write_Byte(0x50, 0x80);
    OV7670_Write_Byte(0x51, 0x00);
    OV7670_Write_Byte(0x52, 0x22);
    OV7670_Write_Byte(0x53, 0x5e);
    OV7670_Write_Byte(0x54, 0x80);
    OV7670_Write_Byte(0x58, 0x9e);
    OV7670_Write_Byte(0x41, 0x08);
    OV7670_Write_Byte(0x3f, 0x00);
    OV7670_Write_Byte(0x75, 0x03);
    OV7670_Write_Byte(0x76, 0xe1);
    OV7670_Write_Byte(0x4c, 0x00);
    OV7670_Write_Byte(0x77, 0x00);
    OV7670_Write_Byte(0x3d, 0xc2);
    OV7670_Write_Byte(0x4b, 0x09);
    OV7670_Write_Byte(0xc9, 0x60);
    OV7670_Write_Byte(0x41, 0x38);
    OV7670_Write_Byte(0x56, 0x40);
    OV7670_Write_Byte(0x34, 0x11);
    OV7670_Write_Byte(0x3b, 0x0a);
    OV7670_Write_Byte(0xa4, 0x88);
    OV7670_Write_Byte(0x96, 0x00);
    OV7670_Write_Byte(0x97, 0x30);
    OV7670_Write_Byte(0x98, 0x20);
    OV7670_Write_Byte(0x99, 0x30);
    OV7670_Write_Byte(0x9a, 0x84);
    OV7670_Write_Byte(0x9b, 0x29);
    OV7670_Write_Byte(0x9c, 0x03);
    OV7670_Write_Byte(0x9d, 0x98);
    OV7670_Write_Byte(0x9e, 0x3f);
    OV7670_Write_Byte(0x78, 0x04);
    OV7670_Write_Byte(0x79, 0x01);
    OV7670_Write_Byte(0xc8, 0xf0);
    OV7670_Write_Byte(0x79, 0x0f);
    OV7670_Write_Byte(0xc8, 0x00);
    OV7670_Write_Byte(0x79, 0x10);
    OV7670_Write_Byte(0xc8, 0x7e);
    OV7670_Write_Byte(0x79, 0x0a);
    OV7670_Write_Byte(0xc8, 0x80);
    OV7670_Write_Byte(0x79, 0x0b);
    OV7670_Write_Byte(0xc8, 0x01);
    OV7670_Write_Byte(0x79, 0x0c);
    OV7670_Write_Byte(0xc8, 0x0f);
    OV7670_Write_Byte(0x79, 0x0d);
    OV7670_Write_Byte(0xc8, 0x20);
    OV7670_Write_Byte(0x79, 0x09);
    OV7670_Write_Byte(0xc8, 0x80);
    OV7670_Write_Byte(0x79, 0x02);
    OV7670_Write_Byte(0xc8, 0xc0);
    OV7670_Write_Byte(0x79, 0x03);
    OV7670_Write_Byte(0xc8, 0x40);
    OV7670_Write_Byte(0x79, 0x05);
    OV7670_Write_Byte(0xc8, 0x30);
    OV7670_Write_Byte(0x79, 0x26);
    OV7670_Write_Byte(0x2d, 0x00);
    OV7670_Write_Byte(0x2e, 0x00);
};

#if 0
unsigned char oldRN675x_init_cfg[] = {
// 720H@50, 27MHz, BT656 output
// Slave address is 0x58
// Register, data

// if clock source(Xin) of RN675x is 26MHz, please add these procedures marked first
//0xD2, 0x85, // disable auto clock detect
//0xD6, 0x37, // 27MHz default
//0xD8, 0x18, // switch to 26MHz clock
//delay(100), // delay 100ms
 
0x81, 0x01, // turn on video decoder
0xA3, 0x00, // enable 72MHz sampling
0xDB, 0x8F, // internal use*
0xFF, 0x00, // switch to ch0 (default; optional)
0x2C, 0x30, // select sync slice points
0x50, 0x00, // 720H resolution select for BT.601
0x56, 0x00, // disable SAV & EAV for BT601; 0x00 enable SAV & EAV for BT656
0x63, 0x09, // filter control
0x59, 0x00, // extended register access
0x5A, 0x00, // data for extended register
0x58, 0x01, // enable extended register write
0x07, 0x22, // PAL format
0x2F, 0x14, // internal use
0x5E, 0x03, // disable H-scaling control
0x5B, 0x00, //
0x3A, 0x04, // no channel information insertion; invert VBLK for frame valid
0x3E, 0x32, // AVID & VBLK out for BT.601
0x40, 0x04, // no channel information insertion; invert VBLK for frame valid
0x46, 0x23, // AVID & VBLK out for BT.601
0x28, 0x92, // cropping
0x00, 0x00, // internal use*
0x2D, 0xF2, // cagc adjust
0x0D, 0x20, // cagc initial value 
//0x05, 0x00, // sharpness
//0x04, 0x80, // hue
0x11, 0x03,
0x37, 0x33,
0x61, 0x6C,

0xDF, 0xFF, // enable 720H format
0x8E, 0x00, // single channel output for VP
0x8F, 0x00, // 720H mode for VP
0x8D, 0x31, // enable VP out
0x89, 0x00, // select 27MHz for SCLK
0x88, 0xC1, // enable SCLK out
0x81, 0x01, // turn on video decoder

0x96, 0x00, // select AVID & VBLK as status indicator
0x97, 0x0B, // enable status indicator out on AVID,VBLK & VSYNC 
0x98, 0x00, // video timing pin status
0x9A, 0x40, // select AVID & VBLK as status indicator 
0x9B, 0xE1, // enable status indicator out on HSYNC
0x9C, 0x00, // video timing pin status

//0x01, 0x30,//brightness
};

#define USE_BLUE_SCREEN
#define USE_DVP
unsigned char RN675x_init_cfg_forCVBS[] =
{
0x81, 0x01, // turn on video decoder
0xA3, 0x04,
0xDF, 0x0F, // enable CVBS format
0x88, 0x40, // disable SCLK1 out

0xFF, 0x00, // ch0
0x00, 0x00, // internal use*
0x06, 0x08, // internal use*
0x07, 0x62, // HD format
0x2A, 0x81, // filter control
0x3A, 0x24,
0x3F, 0x10,
0x4C, 0x37, // equalizer
0x4F, 0x00, // sync control
0x50, 0x00, // D1 resolution
0x56, 0x01, // 72M mode and BT656 mode
0x5F, 0x00, // blank level
0x63, 0x75, // filter control
0x59, 0x00, // extended register access
0x5A, 0x00, // data for extended register
0x58, 0x01, // enable extended register write
0x59, 0x33, // extended register access
0x5A, 0x02, // data for extended register
0x58, 0x01, // enable extended register write
0x5B, 0x00, // H-scaling control
0x5E, 0x01, // enable H-scaling control
0x6A, 0x00, // H-scaling control
0x28, 0xB2, // cropping
#ifdef USE_CVBS_ONE_FIELD
0x20, 0x24,
0x23, 0x17,
0x24, 0x37,
0x25, 0x17,
0x26, 0x00,
0x42, 0x00,
#else
0x28, 0x92,
0x20, 0xA4,
0x23, 0x17,
0x24, 0xFF,
0x25, 0x00,
0x26, 0xFF,
0x42, 0x00,
#endif
0x03, 0x80, // saturation
0x04, 0x80, // hue
0x05, 0x03, // sharpness
0x57, 0x20, // black/white stretch
0x68, 0x32, // coring
0x37, 0x33,
0x61, 0x6C,
#ifdef USE_BLUE_SCREEN
0x3A, 0x24,
#else
0x3A, 0x2C,
0x3B, 0x00,
0x3C, 0x80,
0x3D, 0x80,
#endif
0x33, 0x10,	// video in detection
0x4A, 0xA8, // video in detection
0x2E, 0x30,	// force no video
0x2E, 0x00,	// return to normal 

#ifdef USE_DVP
0x8E, 0x00,
0x8F, 0x80,
0x8D, 0x31,
0x89, 0x09,
0x88, 0x41,
#ifdef USE_BT601
0xFF, 0x00,
0x56, 0x05,
0x3A, 0x24,
0x3E, 0x32,
0x40, 0x04,
0x46, 0x23,
0x96, 0x00,
0x97, 0x0B,
0x98, 0x00,
0x9A, 0x40,
0x9B, 0xE1,
0x9C, 0x00,
#endif

#else
0xFF, 0x09,
0x00, 0x03,
0xFF, 0x08,
0x04, 0x03,
0x6C, 0x11,
#ifdef USE_MIPI_4LANES
0x06, 0x7C,
#else
0x06, 0x4C,
#endif
0x21, 0x01,
0x34, 0x06,
0x35, 0x0B,
0x78, 0x68,
0x79, 0x01,
0x6C, 0x01,
0x04, 0x00,
0x20, 0xAA,
#ifdef USE_MIPI_NON_CONTINUOUS_CLOCK
0x07, 0x05,
#else
0x07, 0x04,
#endif
0xFF, 0x0A,
0x6C, 0x10,
#endif
};
#endif

unsigned char TP2860_CVBS_NTSC_init_cfg[] =
{
0x40, 0x00,
0x02, 0xcf,
0x0c, 0x13,
0x0d, 0x50,
0x15, 0x03,
0x16, 0xd6,
0x17, 0xa0,
0x18, 0x12,
0x19, 0xf0,
0x1a, 0x05,
0x1b, 0x01,
0x1c, 0x06,
0x1d, 0xb4,
0x20, 0x40,
0x21, 0x84,
0x22, 0x36,
0x23, 0x3c,
0x2a, 0x34,
0x2b, 0x70,
0x2c, 0x2a,
0x2d, 0x4b,
0x2e, 0x57,
0x30, 0x62,
0x31, 0xbb,
0x32, 0x96,
0x33, 0xcb,
0x35, 0x65,
0x38, 0x00,
0x39, 0x04,
0x42, 0x00,
0x4C, 0x43,
0x4E, 0x1d,
0x54, 0x04,
0xF6, 0x00,
0xF7, 0x44,
0xFa, 0x04,

0x40, 0x08,

0x12, 0x54,
0x13, 0xef,
0x14, 0x41,
0x15, 0x12,

0x40, 0x00,
};

unsigned char TP2860_CVBS_PAL_init_cfg[] =
{
0x40, 0x00,
0x02, 0xcf,
0x06, 0x32,
0x0c, 0x13,
0x0d, 0x51,
0x15, 0x03,
0x16, 0xf0,
0x17, 0xa0,
0x18, 0x17,
0x19, 0x20,
0x1a, 0x15,
0x1b, 0x01,
0x1c, 0x06,
0x1d, 0xc0,
0x20, 0x48,
0x21, 0x84,
0x22, 0x37,
0x23, 0x3f,
0x2a, 0x34,
0x2b, 0x70,
0x2c, 0x2a,
0x2d, 0x4b,
0x2e, 0x56,
0x30, 0x7a,
0x31, 0x4a,
0x32, 0x4d,
0x33, 0xfb,
0x35, 0x65,
0x38, 0x00,
0x39, 0x04,
0x42, 0x00,
0x4C, 0x43,
0x4E, 0x1d,
0x54, 0x04,
0xF6, 0x00,
0xF7, 0x44,
0xFa, 0x04,

0x40, 0x08,

0x12, 0x54,
0x13, 0xef,
0x14, 0x41,
0x15, 0x12,

0x40, 0x00,
};

//#define AHD_RV6752V1
#define AHD_TP2860

#ifdef AHD_RV6752V1
#define VIDEO_DECODER_IIC_ADDR 0x58
unsigned char RN675x_init_cfg[] =
{
// 720P@25 BT656
// Slave address is 0x58
// Register, data

// if clock source(Xin) of RN6752 is 26MHz, please add these procedures marked first
//0xD2, 0x85, // disable auto clock detect
//0xD6, 0x37, // 27MHz default
//0xD8, 0x18, // switch to 26MHz clock
//delay(100), // delay 100ms

0x81, 0x01, // turn on video decoder
0xA3, 0x04,
0xDF, 0xFE, // enable HD format

0x88, 0x40, // disable SCLK0B out
0xF6, 0x40, // disable SCLK3A out

// ch0
0xFF, 0x00, // switch to ch0 (default; optional)
0x00, 0x20, // internal use*
0x06, 0x08, // internal use*
0x07, 0x63, // HD format
0x2A, 0x01, // filter control
0x3A, 0x00, // No Insert Channel ID in SAV/EAV code
0x3F, 0x10, // channel ID
0x4C, 0x37, // equalizer
0x4F, 0x03, // sync control
0x50, 0x02, // 720p resolution
0x56, 0x01, // 72M mode and BT656 mode
0x5F, 0x40, // blank level
0x63, 0xF5, // filter control
0x59, 0x00, // extended register access
0x5A, 0x42, // data for extended register
0x58, 0x01, // enable extended register write
0x59, 0x33, // extended register access
0x5A, 0x23, // data for extended register
0x58, 0x01, // enable extended register write
0x51, 0xE1, // scale factor1
0x52, 0x88, // scale factor2
0x53, 0x12, // scale factor3
0x5B, 0x07, // H-scaling control
0x5E, 0x08, // enable H-scaling control
0x6A, 0x82, // H-scaling control
0x28, 0x92, // cropping
0x03, 0x80, // saturation
0x04, 0x80, // hue
0x05, 0x00, // sharpness
0x57, 0x23, // black/white stretch
0x68, 0x32, // coring
0x37, 0x33,
0x61, 0x6C,
/*
0x8E, 0x00,
0x8F, 0x80,
0x8D, 0x31,
0x89, 0x09,
0x88, 0x41,
0xFF, 0x00,
0x56, 0x05,
0x3A, 0x24,
0x3E, 0x32,
0x40, 0x04,
0x46, 0x23,
0x96, 0x00,
0x97, 0x0B,
0x98, 0x00,
0x9A, 0x40,
0x9B, 0xE1,
0x9C, 0x00,
*/

0x8E, 0x00,
0x8F, 0x80,
0x8D, 0x31,
0x89, 0x09,
0x88, 0x41,
0xFF, 0x00,
0x56, 0x05,
0x3A, 0x20,
0x3E, 0x32,
0x40, 0x04,
0x46, 0x10,
0x96, 0x00,
0x97, 0x0B,
0x98, 0x00,
0x9A, 0x40,
0x9B, 0xE1,
0x9C, 0x00,


0x8E, 0x00, // single channel output for VP
0x8F, 0x80, // 720p mode for VP
0x8D, 0x31, // enable VP out
0x89, 0x09, // select 72MHz for SCLK
0x88, 0x41, // enable SCLK out
};
#endif

#ifdef AHD_TP2860
#define VIDEO_DECODER_IIC_ADDR 0x88
unsigned char RN675x_init_cfg[] =
{
0x40, 0x00,
0x02, 0xce,
0x07, 0xc0,
0x0b, 0xc0,
0x0c, 0x13,
0x0d, 0x71,
0x15, 0x13,
0x16, 0x15,
0x17, 0x00,
0x18, 0x1b,
0x19, 0xd0,
0x1a, 0x25,
0x1b, 0x01,
0x1c, 0x07,
0x1d, 0xbc,
0x20, 0x40,
0x21, 0x46,
0x22, 0x36,
0x23, 0x3c,
0x25, 0xfe,
0x26, 0x01,
0x2a, 0x34,
0x2b, 0x60,
0x2c, 0x3a,
0x2d, 0x5a,
0x2e, 0x40,
0x30, 0x9e,
0x31, 0x20,
0x32, 0x10,
0x33, 0x90,
0x35, 0x25,
0x38, 0x00,
0x39, 0x18,
0x42, 0x00,
0x4C, 0x4F,
0x4E, 0x1d,
0x54, 0x04,
0xF6, 0x00,
0xF7, 0x44,
0xFa, 0x00,

0x40, 0x08,

0x12, 0x54,
0x13, 0xef,
0x14, 0x41,
0x15, 0x12,

0x40, 0x00,
};
#endif

int8_t RN6752_Write_Byte(uint8_t regIdx, uint8_t val)
{
    I2C_Start();

    I2C_Send_Byte(VIDEO_DECODER_IIC_ADDR);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("write step1, wait ack failed.");
        return -1;
    }

    I2C_Send_Byte(regIdx);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("write step2, wait ack failed.");
        return -1;
    }

    I2C_Send_Byte(val);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("write step3, wait ack failed.");
        return -1;
    }

    I2C_Stop();

    return 0;
}

int8_t RN6752_Read_Byte(uint8_t regIdx, uint8_t *pVal)
{
    I2C_Start();

    I2C_Send_Byte(VIDEO_DECODER_IIC_ADDR);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("read step1, wait ack failed.");
        return -1;
    }

    I2C_Send_Byte(regIdx);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("read step2, wait ack failed.");
        return -1;
    }

    I2C_Stop();


    I2C_Start();
    I2C_Send_Byte((VIDEO_DECODER_IIC_ADDR) | 0x01);
    if(I2C_ACK != I2C_WaitAck())
    {
//        LOG_E("read step3, wait ack failed.");
        return -1;
    }

    *pVal = I2C_Read_Byte();

    I2C_NAck();

    I2C_Stop();

    return 0;
}

__attribute__((optimize(0))) void TP2860_CVBS_Init(void)
{
	uint32_t i,j;

    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_StructInit(&GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  //此处必须是开漏模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
    GPIO_InitStruct.Pin = OV7670_SDA_PIN | OV7670_SCL_PIN;
    HAL_GPIO_Init(OV7670_GPIO_PORT, &GPIO_InitStruct);

	HAL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = OV7670_RST_PIN;
	HAL_GPIO_Init(OV7670_GPIO_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_RST_PIN, GPIO_PIN_RESET);	
	for(i=0; i<100000; i++);
	HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_RST_PIN, GPIO_PIN_SET);
	for(i=0; i<1000000; i++);

	i = sizeof(TP2860_CVBS_PAL_init_cfg);
	i /= 2;
	for (j=0; j<i; j++)
	{
		RN6752_Write_Byte(TP2860_CVBS_PAL_init_cfg[j*2], TP2860_CVBS_PAL_init_cfg[j*2+1]);
	}
}

__attribute__((optimize(0))) void TP2860_CVBS_Adapt(void)
{
	uint32_t i,j;
	uint8_t value = 0;
	
	RN6752_Write_Byte(0x2f, 0x9);
	for(int k=0; k<100000; k++);
	
	RN6752_Read_Byte(0x04, &value);
	
	if(value == 0x94)
	{
		return;
	}
	else
	{
		HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_RST_PIN, GPIO_PIN_RESET);	
		for(i=0; i<100000; i++);
		HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_RST_PIN, GPIO_PIN_SET);
		for(i=0; i<1000000; i++);

		i = sizeof(TP2860_CVBS_NTSC_init_cfg);
		i /= 2;
		for (j=0; j<i; j++)
		{
			RN6752_Write_Byte(TP2860_CVBS_NTSC_init_cfg[j*2], TP2860_CVBS_NTSC_init_cfg[j*2+1]);
		}
	}
}

#if (defined AHD_RV6752V1) || (defined AHD_TP2860)
__attribute__((optimize(0))) void RN6752_Init(void)
{
	uint32_t i,j;

    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_StructInit(&GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  //此处必须是开漏模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
    GPIO_InitStruct.Pin = OV7670_SDA_PIN | OV7670_SCL_PIN;
    HAL_GPIO_Init(OV7670_GPIO_PORT, &GPIO_InitStruct);

	HAL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = OV7670_RST_PIN;
	HAL_GPIO_Init(OV7670_GPIO_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_RST_PIN, GPIO_PIN_RESET);	
	for(i=0; i<100000; i++);
	HAL_GPIO_WritePin(OV7670_GPIO_PORT, OV7670_RST_PIN, GPIO_PIN_SET);
	for(i=0; i<1000000; i++);

	i = sizeof(RN675x_init_cfg);
	i /= 2;
	for (j=0; j<i; j++)
	{
		RN6752_Write_Byte(RN675x_init_cfg[j*2], RN675x_init_cfg[j*2+1]);
	}
}
#endif

/**
  * @brief  Configerate and start the BT656 module.
  * @param  None.
  * @retval None
  */
void BT656_Configurate(uint8_t videotype)
{
	if(videotype == 1)//CVBS
	{
		hvideo.Init.ColorBit        = VIDEOCAP_COLOR_16BIT;
		hvideo.Init.CaptureMode     = BT656_MODE_NORMAL;
		hvideo.Init.DvpMode         = VIDEOCAP_DVP_YUV422;
		hvideo.Init.YUVQuantization = VIDEOCAP_YUV_FullRange;
		hvideo.Init.FieldSampleMode = VIDEOCAP_Sample_BOTH_Filed;
		hvideo.Init.DE_Pol          = VIDEOCAP_DE_Pol_High;
		hvideo.Init.HS_Pol          = VIDEOCAP_HS_Pol_High;
		hvideo.Init.VS_Pol          = VIDEOCAP_VS_Pol_High;
		hvideo.Init.YCBCR_POS       = VIDEOCAP_CB_Y0_CR_Y1;
		hvideo.Init.DownscaleSample = VIDEOCAP_DownSample_Enable;
		hvideo.Init.MaxAddrSize     = SizeOfCVBSBuffer;
		hvideo.Init.CH0FieldBaseAddr= StartAddrOfCVBS0Even;
		hvideo.Init.FrameNumCnt     = 0;
		hvideo.Init.MaxADCFrameNum  = 0;
		
		TP2860_CVBS_Init();
		HAL_VIDEOCAP_Init(&hvideo);
		
		__HAL_VIDEOCAP_IT_ENABLE(&hvideo, VIDEOCAP_IT_DRAWDONE|VIDEOCAP_IT_FRAMEOVERSIZE);
		
		/* Enable CSI video interrupt. */
		csi_hal_irq_register(VIDEO_CAP_IRQn, (void *)Video_Cap_HandlerISR);
		csi_irq_enable(VIDEO_CAP_IRQn);
		
		TP2860_CVBS_Adapt();
	}
	else if(videotype == 2)//DVP
	{
		OV7670_Init();
		HAL_VIDEOCAP_StructInit(&hvideo);
		hvideo.Init.MaxAddrSize     = SizeOfCVBSBuffer;
		hvideo.Init.CH0FieldBaseAddr= StartAddrOfCVBS0Even;
		
		HAL_VIDEOCAP_Init(&hvideo);
		
		__HAL_VIDEOCAP_IT_ENABLE(&hvideo, VIDEOCAP_IT_DRAWDONE|VIDEOCAP_IT_FRAMEOVERSIZE);
		
		/* Enable CSI video interrupt. */
		csi_hal_irq_register(VIDEO_CAP_IRQn, (void *)Video_Cap_HandlerISR);
		csi_irq_enable(VIDEO_CAP_IRQn);
	}
	else if(videotype == 3)//AHD
	{
		hvideo.Init.ColorBit        = VIDEOCAP_COLOR_16BIT;
		hvideo.Init.CaptureMode     = BT656_MODE_DVP;
		hvideo.Init.DvpMode         = VIDEOCAP_DVP_YUV422;
		hvideo.Init.YUVQuantization = VIDEOCAP_YUV_FullRange;
		hvideo.Init.FieldSampleMode = VIDEOCAP_Sample_ODD_Filed;
		hvideo.Init.DE_Pol          = VIDEOCAP_DE_Pol_High;
		hvideo.Init.HS_Pol          = VIDEOCAP_HS_Pol_High;
		hvideo.Init.VS_Pol          = VIDEOCAP_VS_Pol_High;
		hvideo.Init.YCBCR_POS       = VIDEOCAP_CB_Y0_CR_Y1;
		hvideo.Init.DownscaleSample = VIDEOCAP_DownSample_Enable;
		hvideo.Init.MaxAddrSize     = SizeOfCVBSBuffer;
		hvideo.Init.CH0FieldBaseAddr= StartAddrOfCVBS0Even;
		hvideo.Init.FrameNumCnt     = 0;
		hvideo.Init.MaxADCFrameNum  = 0;
		
		#if (defined AHD_RV6752V1) || (defined AHD_TP2860)
		RN6752_Init();
		#endif
		
		HAL_VIDEOCAP_Init(&hvideo);
		
		__HAL_VIDEOCAP_IT_ENABLE(&hvideo, VIDEOCAP_IT_DRAWDONE|VIDEOCAP_IT_FRAMEOVERSIZE);
		
		/* Enable CSI video interrupt. */
		csi_hal_irq_register(VIDEO_CAP_IRQn, (void *)Video_Cap_HandlerISR);
		csi_irq_enable(VIDEO_CAP_IRQn);
	}
}


/**
  * @brief  Disable the BT656 module.
  * @param  None.
  * @retval None
  */
void BT656_SetBaseAddress(uint32_t address)
{
	/* Config base address of chunnel. */
    //BT656_REG->CH0BaseAddr = address;
	hvideo.Instance->CH0BaseAddr = address;
}


/**
  * @brief  Disable the BT656 module.
  * @param  None.
  * @retval None
  */
void BT656_Disable()
{
	//BT656_REG->Control &= 0xfffffffe;
	hvideo.Instance->CR &= 0xfffffffe;
}


/**
  * @brief  Start the BT565 module.
  * @param  None.
  * @retval None
  */
void BT656_Start()
{
	//BT656_REG->Control |= 1;
	hvideo.Instance->CR |= 1;
}


/**
  * @brief  Get the BT565 StartStatus.
  * @param  None.
  * @retval Enable or Disable
  */
uint8_t BT656_GetStartStatus(void)
{
	if((hvideo.Instance->CR & 1) && (hvideo.Instance->CH0FieldPara != 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/**
  * @brief  Reset the BT656's interrput flag.
  * @param  None.
  * @retval None
  */
void BT656_ResetTIStatus(uint32_t BT656_IT)
{
    //BT656_REG->InterruptStatus &= (~BT656_IT);
	hvideo.Instance->ISR &= (~BT656_IT);
}

/**
	*	@brief get BT656 specified interrupt status.
	*	@param BT656_IT: specified interrupt.
	*				 This parameter can be: BT656_IT_C0DD, BT656_IT_WFFIFO, BT656_IT_FOSE.
	*	@retval return specified interrupt status.
	*/
ITStatus BT656_GetITStatus(uint32_t BT656_IT)
{
	ITStatus bitstatus = RESET;
	/* Check the parameters */
//	assert_param(IS_BT656_IT(BT656_IT));
	
	if ((hvideo.Instance->ISR & BT656_IT) == 0)
	{
		bitstatus = RESET;
	}
	else 
	{
		bitstatus = SET;
	}
	
	return bitstatus;
}


/**
	*	@brief set BT656 sample mode.
	*	@param flag: specified sample mode.
	*				 This parameter can be: SAMPLE_ODD_FIELD, SAMPLE_EVEN_FIELD, SAMPLE_BOTH_FIELD.
	*	@retval return specified interrupt status.
	*/
void BT656_SetSampleMode(uint8_t flag)
{
	if( (flag == SAMPLE_ODD_FIELD) || (flag == SAMPLE_EVEN_FIELD) || (flag == SAMPLE_BOTH_FIELD) )
	{
		hvideo.Instance->CR &= (~SAMPLE_FIELD_MASK);
		hvideo.Instance->CR |= (flag << 8);
		
		if(((flag == SAMPLE_ODD_FIELD) || (flag == SAMPLE_EVEN_FIELD)) && ((hvideo.Instance->CR & 0xC) == 0))
		{
			//BT656_REG->MaxSize = SizeOfCVBSBuffer/2 - 80;//hardware burstline is 80 bytes
			hvideo.Instance->MaxSize = SizeOfCVBSBuffer/2 - 80;//hardware burstline is 80 bytes
		}
		else
		{
			//BT656_REG->MaxSize = SizeOfCVBSBuffer - 80;//hardware burstline is 80 bytes
			hvideo.Instance->MaxSize = SizeOfCVBSBuffer - 80;//hardware burstline is 80 bytes
		}
	}
	
}

/**
	*	@brief Get specified channel field parameter
	*	@param Height: field height
	*	@param Width: field width
	*	@retval None
	*/
void BT656_GetParam(uint16_t* Height, uint16_t* Width)
{
	/* Check the parameters */
//	assert_param(IS_VCAP_CH(CHx));
	
//	*Height = BT656_REG->CH0FieldParameter >> 16;
//	*Width = BT656_REG->CH0FieldParameter;
	*Height = hvideo.Instance->CH0FieldPara >> 16;
	*Width = hvideo.Instance->CH0FieldPara;
}
