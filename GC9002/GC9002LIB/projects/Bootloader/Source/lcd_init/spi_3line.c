/**
  ******************************************************************************
  * @file    spi_3line.c
  * @author  asuwish80
  * @version V1.0.0
  * @date    2023/08/10
  * @brief   3-spi driver functions.
  ******************************************************************************
  */


#include "spi_3line.h"


/**
  * @brief  initialize LCD by 3-spi mode.
  * @param  none
  * @retval none
  */
void lcd_spi3line_init(void)
{
	lcd_io_init();
	lcd_reset();
	lcd_set_reg();
}


/**
  * @brief  configure LCD's driver IC register.
  * @param  none
  * @retval none
  */
static void lcd_set_reg(void)
{  	
		spi_write_comm(0xF0);
	spi_write_data(0x55);
	spi_write_data(0xAA);
	spi_write_data(0x52);
	spi_write_data(0x08);
	spi_write_data(0x00);
	
		spi_write_comm(0xF6);
	spi_write_data(0x5A);
	spi_write_data(0x87);
	
		spi_write_comm(0xC1);
	spi_write_data(0x3F);
	
		spi_write_comm(0xC2);
	spi_write_data(0x0E);
	
		spi_write_comm(0xC6);
	spi_write_data(0xF8);
	
		spi_write_comm(0xC9);
	spi_write_data(0x10);
	
		spi_write_comm(0xCD);
	spi_write_data(0x25);
	
		spi_write_comm(0xF8);
	spi_write_data(0x8A);
	
		spi_write_comm(0xAC);
	spi_write_data(0x65);
	
		spi_write_comm(0xA7);
	spi_write_data(0x47);
	
		spi_write_comm(0xA0);
	spi_write_data(0xDD);
	
		spi_write_comm(0xFA);
	spi_write_data(0x08);
	spi_write_data(0x08);
	spi_write_data(0x00);
	spi_write_data(0x04);
	
		spi_write_comm(0xA3);
	spi_write_data(0x2E);
	
		spi_write_comm(0xFD);
	spi_write_data(0x28);
	spi_write_data(0x3C);
	spi_write_data(0x00);
	
		spi_write_comm(0x71);
	spi_write_data(0x48);
	
		spi_write_comm(0x72);
	spi_write_data(0x48);
	
		spi_write_comm(0x73);
	spi_write_data(0x00);
	spi_write_data(0x44);
	
		spi_write_comm(0x97);
	spi_write_data(0xEE);	
	
		spi_write_comm(0x83);
	spi_write_data(0x93);
	
		spi_write_comm(0x9A);
	spi_write_data(0x78);
	
		spi_write_comm(0x9B);
	spi_write_data(0x48);
	
		spi_write_comm(0x82);
	spi_write_data(0x50);
	spi_write_data(0x50);
	
		spi_write_comm(0xB1);
	spi_write_data(0x30);         //0x30:RGB 0x10:BGR
	
		spi_write_comm(0x7A);
	spi_write_data(0x13);
	spi_write_data(0x1A);
	
		spi_write_comm(0x7B);
	spi_write_data(0x13);
	spi_write_data(0x1A);
	
		spi_write_comm(0x6D);
	spi_write_data(0x00);
	spi_write_data(0x1D);
	spi_write_data(0x0A);
	spi_write_data(0x10);
	spi_write_data(0x08);
	spi_write_data(0x1F);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1E);
	spi_write_data(0x1F);
	spi_write_data(0x01);
	spi_write_data(0x0F);
	spi_write_data(0x09);
	spi_write_data(0x1D);
	spi_write_data(0x00);
	
		spi_write_comm(0x64);
	spi_write_data(0x18);
	spi_write_data(0x05);
	spi_write_data(0x03);
	spi_write_data(0xC4);
	spi_write_data(0x03);
	spi_write_data(0x03);
	spi_write_data(0x18);
	spi_write_data(0x04);
	spi_write_data(0x03);
	spi_write_data(0xC3);
	spi_write_data(0x03);
	spi_write_data(0x03);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	
		spi_write_comm(0x67);
	spi_write_data(0x18);
	spi_write_data(0x03);
	spi_write_data(0x03);
	spi_write_data(0xC2);
	spi_write_data(0x03);
	spi_write_data(0x03);
	spi_write_data(0x18);
	spi_write_data(0x02);
	spi_write_data(0x03);
	spi_write_data(0xC1);
	spi_write_data(0x03);
	spi_write_data(0x03);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	
		spi_write_comm(0x60);
	spi_write_data(0x18);
	spi_write_data(0x07);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	spi_write_data(0x18);
	spi_write_data(0x02);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	
		spi_write_comm(0x63);
	spi_write_data(0x18);
	spi_write_data(0x02);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	spi_write_data(0x18);
	spi_write_data(0x06);
	spi_write_data(0x7A);
	spi_write_data(0x7A);
	
		spi_write_comm(0x69);
	spi_write_data(0x14);
	spi_write_data(0x22);
	spi_write_data(0x14);
	spi_write_data(0x22);
	spi_write_data(0x44);
	spi_write_data(0x22);
	spi_write_data(0x08);
	
		spi_write_comm(0xD1);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x10);
	spi_write_data(0x00);
	spi_write_data(0x26);
	spi_write_data(0x00);
	spi_write_data(0x3B);
	spi_write_data(0x00);
	spi_write_data(0x4B);
	spi_write_data(0x00);
	spi_write_data(0x69);
	spi_write_data(0x00);
	spi_write_data(0x85);
	spi_write_data(0x00);
	spi_write_data(0xB3);
	spi_write_data(0x00);
	spi_write_data(0xDB);
	spi_write_data(0x01);
	spi_write_data(0x1B);
	spi_write_data(0x01);
	spi_write_data(0x4F);
	spi_write_data(0x01);
	spi_write_data(0xA8);
	spi_write_data(0x01);
	spi_write_data(0xEF);
	spi_write_data(0x01);
	spi_write_data(0xF0);
	spi_write_data(0x02);
	spi_write_data(0x34);
	spi_write_data(0x02);
	spi_write_data(0x88);
	spi_write_data(0x02);
	spi_write_data(0xc5);
	spi_write_data(0x03);
	spi_write_data(0x1d);
	spi_write_data(0x03);
	spi_write_data(0x5c);
	spi_write_data(0x03);
	spi_write_data(0xA9);
	spi_write_data(0x03);
	spi_write_data(0xC8);
	spi_write_data(0x03);
	spi_write_data(0xE8);
	spi_write_data(0x03);
	spi_write_data(0xF0);
	spi_write_data(0x03);
	spi_write_data(0xF5);
	spi_write_data(0x03);
	spi_write_data(0xFE);
	spi_write_data(0x03);
	spi_write_data(0xFF);
	
		spi_write_comm(0xD2);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x10);
	spi_write_data(0x00);
	spi_write_data(0x26);
	spi_write_data(0x00);
	spi_write_data(0x3B);
	spi_write_data(0x00);
	spi_write_data(0x4B);
	spi_write_data(0x00);
	spi_write_data(0x69);
	spi_write_data(0x00);
	spi_write_data(0x85);
	spi_write_data(0x00);
	spi_write_data(0xB3);
	spi_write_data(0x00);
	spi_write_data(0xDB);
	spi_write_data(0x01);
	spi_write_data(0x1B);
	spi_write_data(0x01);
	spi_write_data(0x4F);
	spi_write_data(0x01);
	spi_write_data(0xA8);
	spi_write_data(0x01);
	spi_write_data(0xEF);
	spi_write_data(0x01);
	spi_write_data(0xF0);
	spi_write_data(0x02);
	spi_write_data(0x34);
	spi_write_data(0x02);
	spi_write_data(0x88);
	spi_write_data(0x02);
	spi_write_data(0xc5);
	spi_write_data(0x03);
	spi_write_data(0x1d);
	spi_write_data(0x03);
	spi_write_data(0x5c);
	spi_write_data(0x03);
	spi_write_data(0xA9);
	spi_write_data(0x03);
	spi_write_data(0xC8);
	spi_write_data(0x03);
	spi_write_data(0xE8);
	spi_write_data(0x03);
	spi_write_data(0xF0);
	spi_write_data(0x03);
	spi_write_data(0xF5);
	spi_write_data(0x03);
	spi_write_data(0xFE);
	spi_write_data(0x03);
	spi_write_data(0xFF);
	
		spi_write_comm(0xD3);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x10);
	spi_write_data(0x00);
	spi_write_data(0x26);
	spi_write_data(0x00);
	spi_write_data(0x3B);
	spi_write_data(0x00);
	spi_write_data(0x4B);
	spi_write_data(0x00);
	spi_write_data(0x69);
	spi_write_data(0x00);
	spi_write_data(0x85);
	spi_write_data(0x00);
	spi_write_data(0xB3);
	spi_write_data(0x00);
	spi_write_data(0xDB);
	spi_write_data(0x01);
	spi_write_data(0x1B);
	spi_write_data(0x01);
	spi_write_data(0x4F);
	spi_write_data(0x01);
	spi_write_data(0xA8);
	spi_write_data(0x01);
	spi_write_data(0xEF);
	spi_write_data(0x01);
	spi_write_data(0xF0);
	spi_write_data(0x02);
	spi_write_data(0x34);
	spi_write_data(0x02);
	spi_write_data(0x88);
	spi_write_data(0x02);
	spi_write_data(0xc5);
	spi_write_data(0x03);
	spi_write_data(0x1d);
	spi_write_data(0x03);
	spi_write_data(0x5c);
	spi_write_data(0x03);
	spi_write_data(0xA9);
	spi_write_data(0x03);
	spi_write_data(0xC8);
	spi_write_data(0x03);
	spi_write_data(0xE8);
	spi_write_data(0x03);
	spi_write_data(0xF0);
	spi_write_data(0x03);
	spi_write_data(0xF5);
	spi_write_data(0x03);
	spi_write_data(0xFE);
	spi_write_data(0x03);
	spi_write_data(0xFF);
	
		spi_write_comm(0xD4);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x10);
	spi_write_data(0x00);
	spi_write_data(0x26);
	spi_write_data(0x00);
	spi_write_data(0x3B);
	spi_write_data(0x00);
	spi_write_data(0x4B);
	spi_write_data(0x00);
	spi_write_data(0x69);
	spi_write_data(0x00);
	spi_write_data(0x85);
	spi_write_data(0x00);
	spi_write_data(0xB3);
	spi_write_data(0x00);
	spi_write_data(0xDB);
	spi_write_data(0x01);
	spi_write_data(0x1B);
	spi_write_data(0x01);
	spi_write_data(0x4F);
	spi_write_data(0x01);
	spi_write_data(0xA8);
	spi_write_data(0x01);
	spi_write_data(0xEF);
	spi_write_data(0x01);
	spi_write_data(0xF0);
	spi_write_data(0x02);
	spi_write_data(0x34);
	spi_write_data(0x02);
	spi_write_data(0x88);
	spi_write_data(0x02);
	spi_write_data(0xc5);
	spi_write_data(0x03);
	spi_write_data(0x1d);
	spi_write_data(0x03);
	spi_write_data(0x5c);
	spi_write_data(0x03);
	spi_write_data(0xA9);
	spi_write_data(0x03);
	spi_write_data(0xC8);
	spi_write_data(0x03);
	spi_write_data(0xE8);
	spi_write_data(0x03);
	spi_write_data(0xF0);
	spi_write_data(0x03);
	spi_write_data(0xF5);
	spi_write_data(0x03);
	spi_write_data(0xFE);
	spi_write_data(0x03);
	spi_write_data(0xFF);	
	
		spi_write_comm(0xD5);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x10);
	spi_write_data(0x00);
	spi_write_data(0x26);
	spi_write_data(0x00);
	spi_write_data(0x3B);
	spi_write_data(0x00);
	spi_write_data(0x4B);
	spi_write_data(0x00);
	spi_write_data(0x69);
	spi_write_data(0x00);
	spi_write_data(0x85);
	spi_write_data(0x00);
	spi_write_data(0xB3);
	spi_write_data(0x00);
	spi_write_data(0xDB);
	spi_write_data(0x01);
	spi_write_data(0x1B);
	spi_write_data(0x01);
	spi_write_data(0x4F);
	spi_write_data(0x01);
	spi_write_data(0xA8);
	spi_write_data(0x01);
	spi_write_data(0xEF);
	spi_write_data(0x01);
	spi_write_data(0xF0);
	spi_write_data(0x02);
	spi_write_data(0x34);
	spi_write_data(0x02);
	spi_write_data(0x88);
	spi_write_data(0x02);
	spi_write_data(0xc5);
	spi_write_data(0x03);
	spi_write_data(0x1d);
	spi_write_data(0x03);
	spi_write_data(0x5c);
	spi_write_data(0x03);
	spi_write_data(0xA9);
	spi_write_data(0x03);
	spi_write_data(0xC8);
	spi_write_data(0x03);
	spi_write_data(0xE8);
	spi_write_data(0x03);
	spi_write_data(0xF0);
	spi_write_data(0x03);
	spi_write_data(0xF5);
	spi_write_data(0x03);
	spi_write_data(0xFE);
	spi_write_data(0x03);
	spi_write_data(0xFF);	
	
		spi_write_comm(0xD6);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x00);
	spi_write_data(0x10);
	spi_write_data(0x00);
	spi_write_data(0x26);
	spi_write_data(0x00);
	spi_write_data(0x3B);
	spi_write_data(0x00);
	spi_write_data(0x4B);
	spi_write_data(0x00);
	spi_write_data(0x69);
	spi_write_data(0x00);
	spi_write_data(0x85);
	spi_write_data(0x00);
	spi_write_data(0xB3);
	spi_write_data(0x00);
	spi_write_data(0xDB);
	spi_write_data(0x01);
	spi_write_data(0x1B);
	spi_write_data(0x01);
	spi_write_data(0x4F);
	spi_write_data(0x01);
	spi_write_data(0xA8);
	spi_write_data(0x01);
	spi_write_data(0xEF);
	spi_write_data(0x01);
	spi_write_data(0xF0);
	spi_write_data(0x02);
	spi_write_data(0x34);
	spi_write_data(0x02);
	spi_write_data(0x88);
	spi_write_data(0x02);
	spi_write_data(0xc5);
	spi_write_data(0x03);
	spi_write_data(0x1d);
	spi_write_data(0x03);
	spi_write_data(0x5c);
	spi_write_data(0x03);
	spi_write_data(0xA9);
	spi_write_data(0x03);
	spi_write_data(0xC8);
	spi_write_data(0x03);
	spi_write_data(0xE8);
	spi_write_data(0x03);
	spi_write_data(0xF0);
	spi_write_data(0x03);
	spi_write_data(0xF5);
	spi_write_data(0x03);
	spi_write_data(0xFE);
	spi_write_data(0x03);
	spi_write_data(0xFF);	
	
	//spi_write_comm(0x3A);
	//spi_write_data(0x50);

	spi_write_comm(0x11);
	spi_write_data(0x00);
	spi_delay(120);
	
	spi_write_comm(0x29);
	spi_write_data(0x00);
}


/**
  * @brief  initialize 3-spi and reset gpio.
  * @param  none
  * @retval none
  */
static void lcd_io_init(void)
{
	GPIO_InitTypeDef IO_InitStruct;
	
	/* lcd_cs */
	HAL_GPIO_StructInit(&IO_InitStruct);
	IO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	IO_InitStruct.Pin = LCD_CS_PIN;
	HAL_GPIO_Init(LCD_CS_PORT, &IO_InitStruct);
	
	/* lcd_sck */
	HAL_GPIO_StructInit(&IO_InitStruct);
	IO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	IO_InitStruct.Pin = LCD_SCK_PIN;
	HAL_GPIO_Init(LCD_SCK_PORT, &IO_InitStruct);
	
	/* lcd_sda */
	HAL_GPIO_StructInit(&IO_InitStruct);
	IO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	IO_InitStruct.Pin = LCD_SDA_PIN;
	HAL_GPIO_Init(LCD_SDA_PORT, &IO_InitStruct);
	
	/* lcd_rst */
	HAL_GPIO_StructInit(&IO_InitStruct);
	IO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	IO_InitStruct.Pin = LCD_RST_PIN;
	HAL_GPIO_Init(LCD_RST_PORT, &IO_InitStruct);
	
	LCD_CS(1);
	LCD_SCK(1);
	LCD_SDA(1);   
	LCD_RST(1); 
}


/**
  * @brief  configure LCD reset sequence.
  * @param  none
  * @retval none
  */
static void lcd_reset(void)
{
	LCD_RST(1);
	spi_delay(10);
	LCD_RST(0);
	spi_delay(50); 
	LCD_RST(1);
	spi_delay(120);
}


/**
  * @brief  delay function.
  * @param  us time
  * @retval none
  */
static void spi_delay(uint32_t nus)  //10us
{
	volatile uint32_t num = nus * 125;
	while(num--);
}


/**
  * @brief  transmit 3-spi serial data packet.
  * @param  one 9-bit valid para, the MSB is transmitted first.
  *         the 9th is 0, represents command bytes.
  *         the 9th is 1, represents data bytes.
  * @retval none
  */
static void spi_write_byte(uint16_t para)
{
	uint8_t i = 0;
	
	LCD_CS(0);
	
	for (i = 0; i < 9; i++)   
	{  
		LCD_SCK(0);
		
		if (para & 0x0100)
			LCD_SDA(1);
		else
			LCD_SDA(0);
		spi_delay(1);
		
		LCD_SCK(1);
		spi_delay(1);
		
		para <<= 1;
	}
	
	LCD_CS(1);
}


/**
  * @brief  3-spi write command.
  * @param  one byte para.
  * @retval none
  */
static void spi_write_comm(uint8_t comm)
{
	uint16_t temp = 0x000;
	
	temp |= comm;
	spi_write_byte(temp);
}


/**
  * @brief  3-spi write data.
  * @param  one byte para.
  * @retval none
  */
static void spi_write_data(uint8_t data)
{
	uint16_t temp = 0x100;
	
	temp |= data;
	spi_write_byte(temp);
}