/**
  ******************************************************************************
  * @file    spi_3line.h
  * @author  asuwish80
  * @version V1.0.0
  * @date    2023/08/10
  * @brief   3-spi driver functions.
  ******************************************************************************
  */

#ifndef __SPI_3LINE_H__
#define __SPI_3LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gc90xx_hal_def.h"
#include "lcd_init.h"


#define LCD_CS_PORT        GPIOB         //lcd_cs
#define LCD_CS_PIN         GPIO_PIN_1
#define LCD_SCK_PORT       GPIOB         //lcd_sck
#define LCD_SCK_PIN        GPIO_PIN_2
#define LCD_SDA_PORT       GPIOB         //lcd_sda
#define LCD_SDA_PIN        GPIO_PIN_3
#define LCD_RST_PORT       GPIOB         //lcd_rst
#define LCD_RST_PIN        GPIO_PIN_0

#define LCD_CS(x)         (x)?(HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET))
#define LCD_SCK(x)        (x)?(HAL_GPIO_WritePin(LCD_SCK_PORT, LCD_SCK_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(LCD_SCK_PORT, LCD_SCK_PIN, GPIO_PIN_RESET))
#define LCD_SDA(x)        (x)?(HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_RESET))
#define LCD_RST(x)        (x)?(HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET))


void lcd_spi3line_init(void);
static void lcd_io_init(void);
static void lcd_reset(void);
static void lcd_set_reg(void);

static void spi_delay(uint32_t nus);	
static void spi_send_byte(uint16_t para);
static void spi_write_comm(uint8_t comm);
static void spi_write_data(uint8_t data);


#ifdef __cplusplus
}
#endif
#endif /* __SPI_3LINE_H__ */