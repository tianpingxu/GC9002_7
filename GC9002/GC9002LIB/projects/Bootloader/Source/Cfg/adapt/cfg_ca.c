#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cfg_ca.h"

#ifndef CONFIG_RUN_BOOTLOADER
uint32_t g_buzzerPin; 
uint32_t g_backLightPin;
/* Touch Resistance Pin */
uint32_t g_touchX_PlusPin; 
uint32_t g_touchX_SubPin;
uint32_t g_touchY_PlusPin; 
uint32_t g_touchY_SubPin;
/* Touch IIC Pin */
uint32_t g_touchI2c_SclPin;
uint32_t g_touchI2c_SdaPin;
uint32_t g_touchI2c_IrqPin;
uint32_t g_touchI2c_RstPin;

uint32_t g_sysTestPin;

GPIO_TypeDef* g_buzzerPort;
GPIO_TypeDef* g_backLightPort;
/* Touch Resistance Port */
GPIO_TypeDef* g_touchX_PlusPort;
GPIO_TypeDef* g_touchX_SubPort;
GPIO_TypeDef* g_touchY_PlusPort;
GPIO_TypeDef* g_touchY_SubPort;
/* Touch IIC Port */
GPIO_TypeDef* g_touchI2c_SclPort;
GPIO_TypeDef* g_touchI2c_SdaPort;
GPIO_TypeDef* g_touchI2c_IrqPort;
GPIO_TypeDef* g_touchI2c_RstPort;

GPIO_TypeDef* g_sysTestPort;


#endif



/**
  * @name       CFG_u32Cfg2Pin
  * @brief      Translate cfg to PIN 
  * @note       
  * @param      cfg  
  * @retval     PIN
  * @author     
  * @date       2023-08-15
  */
uint32_t CFG_u32Cfg2Pin(uint8_t cfg)
{
    uint32_t res = GPIO_PIN_0;
    
    switch(cfg)
    {
        case CFG_enPin_0:
            res = GPIO_PIN_0;
            break;
        case CFG_enPin_1:
            res = GPIO_PIN_1;
            break;
        case CFG_enPin_2:
            res = GPIO_PIN_2;
            break;
        case CFG_enPin_3:
            res = GPIO_PIN_3;
            break;
        case CFG_enPin_4:
            res = GPIO_PIN_4;
            break;
        case CFG_enPin_5:
            res = GPIO_PIN_5;
            break;
        case CFG_enPin_6:
            res = GPIO_PIN_6;
            break;
        case CFG_enPin_7:
            res = GPIO_PIN_7;
            break;
        case CFG_enPin_8:
            res = GPIO_PIN_8;
            break;
        case CFG_enPin_9:
            res = GPIO_PIN_9;
            break;
        case CFG_enPin_10:
            res = GPIO_PIN_10;
            break;
        case CFG_enPin_11:
            res = GPIO_PIN_11;
            break;
        case CFG_enPin_12:
            res = GPIO_PIN_12;
            break;
        case CFG_enPin_13:
            res = GPIO_PIN_13;
            break;
        case CFG_enPin_14:
            res = GPIO_PIN_14;
            break;
        case CFG_enPin_15:
            res = GPIO_PIN_15;
            break;
        case CFG_enPin_16:
            res = GPIO_PIN_16;
            break;
        case CFG_enPin_17:
            res = GPIO_PIN_17;
            break;
        case CFG_enPin_18:
            res = GPIO_PIN_18;
            break;
        case CFG_enPin_19:
            res = GPIO_PIN_19;
            break;
        case CFG_enPin_20:
            res = GPIO_PIN_20;
            break;
        case CFG_enPin_21:
            res = GPIO_PIN_21;
            break;
        case CFG_enPin_22:
            res = GPIO_PIN_22;
            break;
        case CFG_enPin_23:
            res = GPIO_PIN_23;
            break;
        case CFG_enPin_24:
            res = GPIO_PIN_24;
            break;
        case CFG_enPin_25:
            res = GPIO_PIN_25;
            break;
        case CFG_enPin_26:
            res = GPIO_PIN_26;
            break;
        case CFG_enPin_27:
            res = GPIO_PIN_27;
            break;
        case CFG_enPin_28:
            res = GPIO_PIN_28;
            break;
        case CFG_enPin_29:
            res = GPIO_PIN_29;
            break;
        case CFG_enPin_30:
            res = GPIO_PIN_30;
            break;
        case CFG_enPin_31:
            res = GPIO_PIN_31;
            break;
        default:
            break; 
    }
    
    return res;
}




/**
  * @name       CFG_pCfg2Port
  * @brief      Translate cfg to Port
  * @note       
  * @param      cfg:  read from Cfg 
  * @retval     GPIO Port
  * @author     
  * @date       2023-08-15
  */
GPIO_TypeDef* CFG_pCfg2Port(uint8_t cfg)
{
    GPIO_TypeDef* res = GPIOB;
    
    switch(cfg)
    {
        case CFG_enPort_A:
            res = GPIOA;
            break;
        case CFG_enPort_B:
            res = GPIOB;
            break;
        default:
            break;
    }

    return res;
}


/**
  * @name       CFG_u32Cfg2UartBaudRate
  * @brief      Translate cfg to Uart Baudrate
  * @note       
  * @param      cfg:  read from Cfg 
  * @retval     Uart Baudrate
  * @author     
  * @date       2023-08-16
  */
uint32_t CFG_u32Cfg2UartBaudRate(uint8_t cfg)
{
    uint32_t res = UART_BAUDRATE_115200;
    
    switch(cfg)
    {
        case CFG_enUartBaud_9600:
            res = UART_BAUDRATE_9600;
            break;
        case CFG_enUartBaud_19200:
            res = UART_BAUDRATE_19200;
            break;
        case CFG_enUartBaud_57600:
            res = UART_BAUDRATE_57600;
            break;
        case CFG_enUartBaud_115200:
            res = UART_BAUDRATE_115200;
            break;
        default:
            break;
    }

    return res;
}


/**
  * @name       CFG_u32Cfg2CanBaudRate
  * @brief      Translate cfg to Uart Baudrate
  * @note       
  * @param      cfg:  read from Cfg 
  * @retval     Uart Baudrate
  * @author     
  * @date       2023-08-16
  */
uint32_t CFG_u32Cfg2CanBaudRate(uint8_t cfg)
{
    uint32_t res = CANFD_BAUDRATE_500kbps;
    
    switch(cfg)
    {
        case CFG_enCanBaud_250K:
            res = CANFD_BAUDRATE_250kbps;
            break;
        case CFG_enCanBaud_500K:
            res = CANFD_BAUDRATE_500kbps;
            break;
        case CFG_enCanBaud_833K:
            res = CANFD_BAUDRATE_833kbps;
            break;
        case CFG_enCanBaud_1000K:
            res = CANFD_BAUDRATE_1mbps;
            break;
        case CFG_enCanBaud_1538K:
            res = CANFD_BAUDRATE_1538kbps;
            break;
        case CFG_enCanBaud_2000K:
            res = CANFD_BAUDRATE_2mbps;
            break;
        case CFG_enCanBaud_3077K:
            res = CANFD_BAUDRATE_3077kbps;
            break;
        case CFG_enCanBaud_5000K:
            res = CANFD_BAUDRATE_5mbps;
            break;
        case CFG_enCanBaud_6667K:
            res = CANFD_BAUDRATE_6667kbps;
            break;
        case CFG_enCanBaud_8000K:
            res = CANFD_BAUDRATE_8mbps;
            break;
        case CFG_enCanBaud_10000K:
            res = CANFD_BAUDRATE_10mbps;
            break;
        default:
            break;
    }

    return res;
}


/**
  * @name       CFG_pCfg2UartChannel
  * @brief      Translate cfg to Uart Channel
  * @note       
  * @param      cfg:  read from Cfg 
  * @retval     UART_TypeDef*
  * @author     
  * @date       2023-08-16
  */
UART_TypeDef* CFG_pCfg2UartChannel(uint8_t cfg)
{
    UART_TypeDef* res = UART2;
    
    switch(cfg)
    {
        case CFG_enUartChanel_0:
            res = UART0;
            break;
        case CFG_enUartChanel_1:
            res = UART1;
            break;
        case CFG_enUartChanel_2:
            res = UART2;
            break;

        default:
            break;
    }

    return res;
}


/**
  * @name       CFG_pCfg2CanChannel
  * @brief      Translate cfg to CAN Channel
  * @note       
  * @param      cfg:  read from Cfg 
  * @retval     CANFD_TypeDef*
  * @author     
  * @date       2023-08-16
  */
CANFD_TypeDef* CFG_pCfg2CanChannel(uint8_t cfg)
{
    CANFD_TypeDef* res = CANFD;
    
    switch(cfg)
    {
        case CFG_enCanChanel_0:
            res = CANFD;
            break;

        default:
            break;
    }

    return res;
}

/**
  * @name       CFG_pCfg2SpiChannel
  * @brief      Translate cfg to SPI Channel
  * @note       
  * @param      cfg:  read from Cfg 
  * @retval     SPI_TypeDef*
  * @author     
  * @date       2023-08-16
  */
SPI_TypeDef* CFG_pCfg2SpiChannel(uint8_t cfg)
{
    SPI_TypeDef* res = SPI0;
    
    switch(cfg)
    {
        case CFG_enSpiChanel_0:
            res = SPI0;
            break;
        case CFG_enSpiChanel_1:
            res = SPI1;
            break;
        case CFG_enSpiChanel_2:
            res = SPI2;
            break;

        default:
            break;
    }

    return res;
}



#ifndef CONFIG_RUN_BOOTLOADER
/**
  * @name       CFG_vGetPinAndPortInfo
  * @brief      Get Pin/Port information for AHMI project
  * @note       
  * @param      none  
  * @retval     none
  * @author     
  * @date       2023-10-18
  */
void CFG_vGetPinAndPortInfo(void)
{
    g_buzzerPin = CFG_u32Cfg2Pin(CFG_Get_sys_buzzer_pin());
    g_backLightPin = CFG_u32Cfg2Pin(CFG_Get_sys_ctrl_bl_pin());
    g_touchX_PlusPin = CFG_u32Cfg2Pin(CFG_Get_touch_x_plus_pin());
    g_touchX_SubPin = CFG_u32Cfg2Pin(CFG_Get_touch_x_sub_pin());
    g_touchY_PlusPin = CFG_u32Cfg2Pin(CFG_Get_touch_y_plus_pin());
    g_touchY_SubPin = CFG_u32Cfg2Pin(CFG_Get_touch_y_sub_pin());

    g_touchI2c_SclPin = CFG_u32Cfg2Pin(CFG_Get_touch_i2c_scl_pin());
    g_touchI2c_SdaPin = CFG_u32Cfg2Pin(CFG_Get_touch_i2c_sda_pin());
    g_touchI2c_IrqPin = CFG_u32Cfg2Pin(CFG_Get_touch_i2c_irq_pin());
    g_touchI2c_RstPin = CFG_u32Cfg2Pin(CFG_Get_touch_i2c_rst_pin());
    
    g_sysTestPin = CFG_u32Cfg2Pin(CFG_Get_sys_test_pin());

    g_buzzerPort = CFG_pCfg2Port(CFG_Get_sys_buzzer_port());
    g_backLightPort = CFG_pCfg2Port(CFG_Get_sys_ctrl_bl_port());
    g_touchX_PlusPort = CFG_pCfg2Port(CFG_Get_touch_x_plus_port());
    g_touchX_SubPort = CFG_pCfg2Port(CFG_Get_touch_x_sub_port());
    g_touchY_PlusPort = CFG_pCfg2Port(CFG_Get_touch_y_plus_port());
    g_touchY_SubPort = CFG_pCfg2Port(CFG_Get_touch_y_sub_port());
    
    g_touchI2c_SclPort = CFG_pCfg2Port(CFG_Get_touch_i2c_scl_port());
    g_touchI2c_SdaPort = CFG_pCfg2Port(CFG_Get_touch_i2c_sda_port());
    g_touchI2c_IrqPort = CFG_pCfg2Port(CFG_Get_touch_i2c_irq_port());
    g_touchI2c_RstPort = CFG_pCfg2Port(CFG_Get_touch_i2c_rst_port());
    
    g_sysTestPort = CFG_pCfg2Port(CFG_Get_sys_test_port());
}
#endif


