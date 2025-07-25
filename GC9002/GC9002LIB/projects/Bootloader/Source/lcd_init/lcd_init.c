/**
  ******************************************************************************
  * @file    lcd_init.c
  * @author  asuwish80
  * @version V1.0.0
  * @date    2023/08/10
  * @brief   LCD power on sequence.
  ******************************************************************************
  */
#include "lcd_init.h"


/**
  * @brief  initialize LCD.
  * @param  none
  * @retval none
  */
void lcd_init(void)
{
#ifdef SPI_3LINE_ENABLED
	lcd_spi3line_init();
#endif
}