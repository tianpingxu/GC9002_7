#ifndef __CST836_H
#define __CST836_H
#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include "bsp_i2c_touch.h"
#include "AHMITouchDefine.h"
#include "capacitiveTouch_Interface.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef TOUCH_IC_CST836U
/* Includes ------------------------------------------------------------------*/
#include "bsp_i2c_touch.h"


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CST836U_RESET_PIN_SUPPORT    1           // 0: not support, 1: support

#define CST836U_IIC_ADDR             0x15
#define CST836U_IIC_WR_ADDR          0x2A
#define CST836U_RD_ADDR              0x2B


#define CST836U_CHIP_ID              0x13

#define CST836U_REG_CHIP_ID          0xAA

#define CST836U_ADDR_LENGTH    1

/* 表示读数据 */
#define CST836U_I2C_M_RD        0x0001
 /*
 * 存储I2C通讯的信息
 * @addr：  从设备的I2C设备地址
 * @flags: 控制标志
 * @len：  读写数据的长度
 * @buf：  存储读写数据的指针
 **/
struct CST836U_i2c_msg {
    uint8_t addr;       /*从设备的I2C设备地址 */
    uint16_t flags; /*控制标志*/
    uint16_t len;       /*读写数据的长度           */
    uint8_t *buf;       /*存储读写数据的指针 */
};


/* Exported functions --------------------------------------------------------*/
uint8_t CST836U_vInit(void);
uint8_t CST836U_u8GetConState(void);
uint8_t CST836U_vGetTouchInfo(uint32_t *px, uint32_t *py, uint32_t *state);

#endif

#ifdef __cplusplus
}
#endif


#endif /* __CST816_H__ */








