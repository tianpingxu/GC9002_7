/**
  ******************************************************************************
  * @file    lcd_init.h
  * @author  asuwish80
  * @version V1.0.0
  * @date    2023/08/10
  * @brief   LCD power on sequence.
  ******************************************************************************
  */

#ifndef __LCD_INIT_H__
#define __LCD_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "spi_3line.h"


//#define SPI_3LINE_ENABLED


void lcd_init(void);


#ifdef __cplusplus
}
#endif
#endif /* __LCD_INIT_H__ */