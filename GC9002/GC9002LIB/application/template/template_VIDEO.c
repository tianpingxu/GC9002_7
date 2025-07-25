/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-01     huyt        the first version
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_VIDEO_

static VIDEOCAP_HandleTypeDef hvideo = {.Instance = VIDEOCAP};
static uint32_t *buff;
unsigned char OV7670_init(void);




static HYPER_HandleTypeDef handle_hyper;
int main(void)
{    
//    GPIO_InitTypeDef GPIO_InitStruct;
//    HAL_GPIO_StructInit(&GPIO_InitStruct);
//  
//    GPIO_InitStruct.Pin =  GPIO_PIN_12;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
//    
//	handle_hyper.Instance = HYPER;
//    if(HAL_HYPER_Init(&handle_hyper) != HAL_OK)
//    {
//        while(1);
//    }
//    buff = (uint32_t *)0x20000000;
    for(int i = 0; i < 512; i++)
    {
        buff[i] = 0xFFFFFFFF;
    }
    
#if(0)
	HAL_VIDEOCAP_MspInit(NULL);
    unsigned int *ptr;
    ptr = (unsigned int *)0x4010B00C;
    *ptr = 0x200000;
    ptr = (unsigned int *)0x4010B014;
    *ptr = 0x20000000;
    ptr = (unsigned int *)0x4010B000;
    *ptr = 0xc005;
    ptr = (unsigned int *)0x4010B008;
    while((*ptr & 0x1) == 0)
    {;}
    while(1);
#endif

#include "display_888.h"
    DISPLAY_HandleTypeDef hdisplay;
    display_888_init(&hdisplay, NULL);
    display_888_master_passive(&hdisplay, NULL);
    
    uint16_t Height;
    uint16_t Width;
    buff = (uint32_t *)0x20001600U;
    for(int i = 0; i < 512; i++)
    {
        buff[i] = 0;
    }
    HAL_VIDEOCAP_StructInit(&hvideo);
    
    hvideo.Init.MaxAddrSize     = 0xFFFFFFFF;
    hvideo.Init.CH0FieldBaseAddr= (uint32_t)buff;//不可以是sram地址，video无法访问sram
    hvideo.Init.FrameNumCnt     = 20;
    hvideo.Init.MaxADCFrameNum  = 20;
    
    HAL_VIDEOCAP_Init(&hvideo);
//    OV7670_init();//使用IIC通讯，OV7670不回复ack 
    HAL_VIDEOCAP_SetSampleMode(&hvideo, BT656_SAMPLEMODE_ODD);
    HAL_VIDEOCAP_Start(&hvideo);
    while(!__HAL_VIDEOCAP_GET_IT_FLAG(&hvideo, VIDEOCAP_IT_DRAWDONE));
    while (1)
    {
        HAL_VIDEOCAP_GetParam(&hvideo, &Height, &Width);
		HAL_Delay(500);
    }
    return 0;
}





#include "SCCB.h"
//#define TEST_USE_IIC    1
static I2C_HandleTypeDef hi2c0 = {.Instance = I2C0};
static I2C_HandleTypeDef *phi2c;
/* 写OV7670寄存器
 * 从地址0x42 是写
 * 从地址0x43 是读
 * 返回：1-成功 0-失败
 * */
unsigned char Write_OV7670_Reg(unsigned char regID, unsigned char regDat)
{
#if(defined TEST_USE_IIC)
    uint8_t buff[2];
    buff[0] = regID;
    buff[1] = regDat;
    if(HAL_I2C_Master_Transmit(phi2c, 0x42, buff, 2, 5000) != HAL_OK)
    {
//        printf("I2C M-TX Err, regID:%xH, regDat:%xH\n", (uint32_t)regID, (uint32_t)regDat);
        return 0;
    }
  	return 1;
#else
	startSCCB();
	if(0==SCCBwriteByte(0x42 << 1))
//	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regDat))
	{
		stopSCCB();
		return(0);
	}
  	stopSCCB();
	
  	return(1);
#endif
}
/* 读OV7670寄存器
 * 从地址0x42 是写
 * 从地址0x43 是读
 * 返回：1-成功 0-失败
 * */
unsigned char Read_OV7670_Reg(unsigned char regID, unsigned char *regDat)
{
#if(defined TEST_USE_IIC)
    //通过写操作设置寄存器地址
    if(HAL_I2C_Master_Transmit(phi2c, 0x42, &regID, 1, 5000) != HAL_OK)
    {
//        printf("I2C M-TX Err, regID:%xH\n", (uint32_t)regID);
        return 0;
    }
    //设置寄存器地址后，才是读
    if(HAL_I2C_Master_Receive(phi2c, 0x43, regDat, 1, 5000) == HAL_OK)
    {
//        printf("I2C M-TX Err, regID:%xH\n", (uint32_t)regID);
        return 0;
    }
  	return 1;
#else
	//Í¨¹ýÐ´²Ù×÷ÉèÖÃ¼Ä´æÆ÷µØÖ·
	startSCCB();
	if(0==SCCBwriteByte(0x42 << 1))
//	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	stopSCCB();
	
	delay_us(100);
	
	//ÉèÖÃ¼Ä´æÆ÷µØÖ·ºó£¬²ÅÊÇ¶Á
	startSCCB();
	if(0==SCCBwriteByte((0x43 << 1) | 0x1))
//	if(0==SCCBwriteByte(0x43))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	*regDat=SCCBreadByte();
  	noAck();
  	stopSCCB();
  	return(1);
#endif
}
void set_OV7670reg(void);
void OV7670_config_window(unsigned int startx, unsigned int starty, unsigned int width, unsigned int height);
/* OV7670 初始化
 * 返回：1-成功 0-失败
 * */
unsigned char OV7670_init(void)
{
	unsigned char temp;
#if(defined TEST_USE_IIC)
    phi2c = &hi2c0;
    
    HAL_I2C_StructInit(&hi2c0);
    hi2c0.Init.OwnAddress1 = 0x5B;
    hi2c0.Init.OwnAddress2 = 0x5A;
    hi2c0.Init.Speed = I2C_Speed_Fast;
//    hi2c0.Init.ss_hcnt = 0;//6
//    hi2c0.Init.ss_lcnt = 0;//12
//    hi2c0.Init.fs_hcnt = 0;//6
//    hi2c0.Init.fs_lcnt = 0;//15
//    hi2c0.Init.fp_hcnt = 0;//6
//    hi2c0.Init.fp_lcnt = 0;//15
//    hi2c0.Init.hs_hcnt = 6;//6
//    hi2c0.Init.hs_lcnt = 16;//16
//    hi2c0.Init.tx_sda_hold_time =  0;
//    hi2c0.Init.rx_sda_hold_time = 0;
//    hi2c0.Init.sda_fall_ns =  0;
//    hi2c0.Init.scl_fall_ns = 0;
    HAL_I2C_Init(&hi2c0);
#else
	SCCB_GPIO_Config();
#endif
    
    temp = 0x80;
	if(0 == Write_OV7670_Reg(0x12, temp)) //Reset SCCB
	{
        return 0 ;
	}
    mdelay(200);
    set_OV7670reg();
    
	OV7670_config_window(272, 12, 320, 240);// set 240*320
    return 1;
}
/* config_OV7670_window
 * (140,16,640,480) is good for VGA
 * (272,16,320,240) is good for QVGA
 * */
void OV7670_config_window(unsigned int startx, unsigned int starty, unsigned int width, unsigned int height)
{
	unsigned int endx;
	unsigned int endy;
	unsigned char temp_reg1, temp_reg2;
	unsigned char temp=0;
	
	endx = startx + width;
	endy = starty + height + height;
    Read_OV7670_Reg(0x03, &temp_reg1);
	temp_reg1 &= 0xf0;
	Read_OV7670_Reg(0x32, &temp_reg2);
	temp_reg2 &= 0xc0;
	
	// Horizontal
	temp = temp_reg2|((endx&0x7)<<3)|(startx&0x7);
	Write_OV7670_Reg(0x32, temp);
	temp = (startx&0x7F8)>>3;
	Write_OV7670_Reg(0x17, temp);
	temp = (endx&0x7F8)>>3;
	Write_OV7670_Reg(0x18, temp);
	
	// Vertical
	temp = temp_reg1|((endy&0x3)<<2)|(starty&0x3);
	Write_OV7670_Reg(0x03, temp);
	temp = starty>>2;
	Write_OV7670_Reg(0x19, temp);
	temp = endy>>2;
	Write_OV7670_Reg(0x1A, temp);
}
void set_OV7670reg(void)
{
	Write_OV7670_Reg(0x8c, 0x00);
	Write_OV7670_Reg(0x3a, 0x04);
	Write_OV7670_Reg(0x40, 0xd0);
	Write_OV7670_Reg(0x8c, 0x00);
	Write_OV7670_Reg(0x12, 0x14);
	Write_OV7670_Reg(0x32, 0x80);
	Write_OV7670_Reg(0x17, 0x16);
	Write_OV7670_Reg(0x18, 0x04);
	Write_OV7670_Reg(0x19, 0x02);
	Write_OV7670_Reg(0x1a, 0x7b);
	Write_OV7670_Reg(0x03, 0x06);
	Write_OV7670_Reg(0x0c, 0x04);
	Write_OV7670_Reg(0x3e, 0x00);
	Write_OV7670_Reg(0x70, 0x3a);
	Write_OV7670_Reg(0x71, 0x35); 
	Write_OV7670_Reg(0x72, 0x11); 
	Write_OV7670_Reg(0x73, 0x00);
	Write_OV7670_Reg(0xa2, 0x00);
	Write_OV7670_Reg(0x11, 0xff); 
	//Write_OV7670_Reg(0x15 , 0x31);
	Write_OV7670_Reg(0x7a, 0x20); 
	Write_OV7670_Reg(0x7b, 0x1c); 
	Write_OV7670_Reg(0x7c, 0x28); 
	Write_OV7670_Reg(0x7d, 0x3c);
	Write_OV7670_Reg(0x7e, 0x55); 
	Write_OV7670_Reg(0x7f, 0x68); 
	Write_OV7670_Reg(0x80, 0x76);
	Write_OV7670_Reg(0x81, 0x80); 
	Write_OV7670_Reg(0x82, 0x88);
	Write_OV7670_Reg(0x83, 0x8f);
	Write_OV7670_Reg(0x84, 0x96); 
	Write_OV7670_Reg(0x85, 0xa3);
	Write_OV7670_Reg(0x86, 0xaf);
	Write_OV7670_Reg(0x87, 0xc4); 
	Write_OV7670_Reg(0x88, 0xd7);
	Write_OV7670_Reg(0x89, 0xe8);
	 
	Write_OV7670_Reg(0x32,0xb6);
	
	Write_OV7670_Reg(0x13, 0xff); 
	Write_OV7670_Reg(0x00, 0x00);
	Write_OV7670_Reg(0x10, 0x00);
	Write_OV7670_Reg(0x0d, 0x00);
	Write_OV7670_Reg(0x14, 0x4e);
	Write_OV7670_Reg(0xa5, 0x05);
	Write_OV7670_Reg(0xab, 0x07); 
	Write_OV7670_Reg(0x24, 0x75);
	Write_OV7670_Reg(0x25, 0x63);
	Write_OV7670_Reg(0x26, 0xA5);
	Write_OV7670_Reg(0x9f, 0x78);
	Write_OV7670_Reg(0xa0, 0x68);
//	Write_OV7670_Reg(0xa1, 0x03);//0x0b,
	Write_OV7670_Reg(0xa6, 0xdf);
	Write_OV7670_Reg(0xa7, 0xdf);
	Write_OV7670_Reg(0xa8, 0xf0); 
	Write_OV7670_Reg(0xa9, 0x90); 
	Write_OV7670_Reg(0xaa, 0x94); 
	//Write_OV7670_Reg(0x13, 0xe5); 
	Write_OV7670_Reg(0x0e, 0x61);
	Write_OV7670_Reg(0x0f, 0x43);
	Write_OV7670_Reg(0x16, 0x02); 
	Write_OV7670_Reg(0x1e, 0x37);
	Write_OV7670_Reg(0x21, 0x02);
	Write_OV7670_Reg(0x22, 0x91);
	Write_OV7670_Reg(0x29, 0x07);
	Write_OV7670_Reg(0x33, 0x0b);
	Write_OV7670_Reg(0x35, 0x0b);
	Write_OV7670_Reg(0x37, 0x3f);
	Write_OV7670_Reg(0x38, 0x01);
	Write_OV7670_Reg(0x39, 0x00);
	Write_OV7670_Reg(0x3c, 0x78);
	Write_OV7670_Reg(0x4d, 0x40);
	Write_OV7670_Reg(0x4e, 0x20);
	Write_OV7670_Reg(0x69, 0x00);
	Write_OV7670_Reg(0x6b, 0x4a);
	Write_OV7670_Reg(0x74, 0x19);
	Write_OV7670_Reg(0x8d, 0x4f);
	Write_OV7670_Reg(0x8e, 0x00);
	Write_OV7670_Reg(0x8f, 0x00);
	Write_OV7670_Reg(0x90, 0x00);
	Write_OV7670_Reg(0x91, 0x00);
	Write_OV7670_Reg(0x92, 0x00);
	Write_OV7670_Reg(0x96, 0x00);
	Write_OV7670_Reg(0x9a, 0x80);
	Write_OV7670_Reg(0xb0, 0x84);
	Write_OV7670_Reg(0xb1, 0x0c);
	Write_OV7670_Reg(0xb2, 0x0e);
	Write_OV7670_Reg(0xb3, 0x82);
	Write_OV7670_Reg(0xb8, 0x0a);
	Write_OV7670_Reg(0x43, 0x14);
	Write_OV7670_Reg(0x44, 0xf0);
	Write_OV7670_Reg(0x45, 0x34);
	Write_OV7670_Reg(0x46, 0x58);
	Write_OV7670_Reg(0x47, 0x28);
	Write_OV7670_Reg(0x48, 0x3a);
	
	Write_OV7670_Reg(0x59, 0x88);
	Write_OV7670_Reg(0x5a, 0x88);
	Write_OV7670_Reg(0x5b, 0x44);
	Write_OV7670_Reg(0x5c, 0x67);
	Write_OV7670_Reg(0x5d, 0x49);
	Write_OV7670_Reg(0x5e, 0x0e);
	
	Write_OV7670_Reg(0x64, 0x04);
	Write_OV7670_Reg(0x65, 0x20);
	Write_OV7670_Reg(0x66, 0x05);

	Write_OV7670_Reg(0x94, 0x04);
	Write_OV7670_Reg(0x95, 0x08);	 

	Write_OV7670_Reg(0x6c, 0x0a);
	Write_OV7670_Reg(0x6d, 0x55);
	Write_OV7670_Reg(0x6e, 0x11);
	Write_OV7670_Reg(0x6f, 0x9f); 

	Write_OV7670_Reg(0x6a, 0x40);
	Write_OV7670_Reg(0x01, 0x40);
	Write_OV7670_Reg(0x02, 0x40);
	
	//Write_OV7670_Reg(0x13, 0xe7);
	Write_OV7670_Reg(0x15, 0x00);
	Write_OV7670_Reg(0x4f, 0x80);
	Write_OV7670_Reg(0x50, 0x80);
	Write_OV7670_Reg(0x51, 0x00);
	Write_OV7670_Reg(0x52, 0x22);
	Write_OV7670_Reg(0x53, 0x5e);
	Write_OV7670_Reg(0x54, 0x80);
	Write_OV7670_Reg(0x58, 0x9e);

	Write_OV7670_Reg(0x41, 0x08);
	Write_OV7670_Reg(0x3f, 0x00);
	Write_OV7670_Reg(0x75, 0x05);
	Write_OV7670_Reg(0x76, 0xe1);

	Write_OV7670_Reg(0x4c, 0x00);
	Write_OV7670_Reg(0x77, 0x01);
	
	Write_OV7670_Reg(0x3d, 0xc1);
	Write_OV7670_Reg(0x4b, 0x09);
	Write_OV7670_Reg(0xc9, 0x60);
	//Write_OV7670_Reg(0x41, 0x38);	
	Write_OV7670_Reg(0x56, 0x40);
	Write_OV7670_Reg(0x34, 0x11);
	Write_OV7670_Reg(0x3b, 0x02);
	Write_OV7670_Reg(0xa4, 0x89);
	
	Write_OV7670_Reg(0x96, 0x00);
	Write_OV7670_Reg(0x97, 0x30);
	Write_OV7670_Reg(0x98, 0x20);
	Write_OV7670_Reg(0x99, 0x30);
	Write_OV7670_Reg(0x9a, 0x84);
	Write_OV7670_Reg(0x9b, 0x29);
	Write_OV7670_Reg(0x9c, 0x03);
	Write_OV7670_Reg(0x9d, 0x4c);
	Write_OV7670_Reg(0x9e, 0x3f);	

	Write_OV7670_Reg(0x09, 0x00);
	Write_OV7670_Reg(0x3b, 0xc2);
}

#endif/* _TEMPLATE_VIDEO_ */
#endif/* BSP_DEBUG */
