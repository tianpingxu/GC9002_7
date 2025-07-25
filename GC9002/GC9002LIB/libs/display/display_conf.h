/**
  ******************************************************************************
  * @file    display_conf.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/17
  * @brief   
  ******************************************************************************
  */
#ifndef __DISPLAY_CONF_H__
#define __DISPLAY_CONF_H__
#include <gc90xx_hal_def.h>
#include "system.h"
#include <soft_libc.h>
#include <csi_config.h>
#ifdef CONFIG_DRV_DISPLAY_ENABLED
// Base, default enable
#define DRV_DISPLAY_888_ENABLED             (1U)
#define DRV_DISPLAY_MRB3205_ENABLED         (1U)
#define DRV_DISPLAY_SJXD_ENABLED            (1U)
#define DRV_DISPLAY_ST7512_ENABLED          (1U)
#define DRV_DISPLAY_ST7735S_ENABLED         (1U)
#define DRV_DISPLAY_ST77903_ENABLED         (1U)
#define DRV_DISPLAY_LEIKE_ENABLED           (1U)
#define DRV_DISPLAY_TFT128QM071_ENABLED     (1U)
#define DRV_DISPLAY_RGB565_ENABLED          (1U)
#define DRV_DISPLAY_GC9307N_ENABLED         (1U)

// Others, default disable

#define DRV_DISPLAY_INSTANCE                DISPLAY
#define DRV_AHMI_INSTANCE                   AHMI
#define DRV_DISPLAY_AHB_SLAVE_BASE_ADDR     (0x30000000U)
#define DRV_AHMI_SOURCE_BUFFER_BASE_ADDE    (0x0200C000U)
#define DRV_AHMI_FRAME_BUFFER_BASE_ADDE     (0x20000100U)
#define DRV_AHMI_TILE_BUFFER_SIZE           (16)
typedef enum
{
	LCD_RGB565 = 0U,
    LCD_888,
    LCD_MRB3205,
    LCD_SJXD,
    LCD_ST7512,
    LCD_ST7735S,
    LCD_ST77903,
    LCD_LEIKE,
    LCD_TFT128QM071,
    LCD_GC9307N,
    LCD_TEST_SOFT = 0xFF,
}LCD_Type;
typedef DISPLAY_Mode Display_RunMode;
typedef DISPLAY_Div_Enum Display_Div;
typedef DISPLAY_InputColorMode Display_InputColorMode;
typedef DISPLAY_OutputColorMode Display_OutputColorMode;

struct display_conf
{
    struct
    {
        uint32_t disp_duty_l       : 9;       /*!< display duty low.          */
        uint32_t disp_duty_h       : 9;       /*!< display duty high.         */
        uint32_t input_color_mode  : 3;		  /*!< Display input color mode.
												This parameter can be a value of @ref Display_InputColorMode */
        uint32_t output_color_mode : 3;       /*!< Display output color mode.
												This parameter can be a value of @ref Display_OutputColorMode */
        uint32_t invert_oclk_en    : 1;
		uint32_t invert_hs    	   : 1;
		uint32_t invert_vs         : 1;
		uint32_t                   : 5;
    };
    struct
    {
        uint32_t disp_qspi_duty_l : 9;  /*!< display qspi duty low.     */
        uint32_t disp_qspi_duty_h : 9;  /*!< display qspi duty high.    */
        uint32_t clk_spi_div      : 4;  /*!< @ref Display_Div      */
        uint32_t lcd_type         : 8;  /*!< @ref LCD_Type              */
        uint32_t run_mode         : 2;  /*!< @ref Display_RunMode       */
    };
    uint32_t frame_buffer_addr;     /*!< Set DRV_DISPLAY_AHB_SLAVE_BASE_ADDR on slave mode, set hyper ram on master mode. */
    
    uint16_t HPIXEL_VALUE;
    uint16_t VLINES_VALUE;
    uint16_t HSYNCPW_VALUE;
    uint16_t VSYNCPW_VALUE;
    uint16_t HFNPRCH_VALUE;
    uint16_t VFNPRCH_VALUE;
    uint16_t HBKPRCH_VALUE;
    uint16_t VBKPRCH_VALUE;
    uint32_t PARA_EXTRA;
};
/* LCD 888 Configuration parameter
 */
#if(defined DRV_DISPLAY_888_ENABLED && DRV_DISPLAY_888_ENABLED == 1U)
    #define LCD_888_RESET_GPIO      GPIOA
    #define LCD_888_RESET_PIN       GPIO_PIN_14

    #define LCD_888_SCK_GPIO        GPIOA
    #define LCD_888_SCK_PIN         GPIO_PIN_0
    #define LCD_888_SCK_AF          GPIO_AF3

    #define LCD_888_WRN_GPIO        GPIOA
    #define LCD_888_WRN_PIN         GPIO_PIN_1
    #define LCD_888_WRN_AF          GPIO_AF3

    #define LCD_888_DCN_GPIO        GPIOA
    #define LCD_888_DCN_PIN         GPIO_PIN_2
    #define LCD_888_DCN_AF          GPIO_AF3

    #define LCD_888_RDN_GPIO        GPIOA
    #define LCD_888_RDN_PIN         GPIO_PIN_3
    #define LCD_888_RDN_AF          GPIO_AF3

    #define LCD_888_CSN_GPIO        GPIOA
    #define LCD_888_CSN_PIN         GPIO_PIN_4
    #define LCD_888_CSN_AF          GPIO_AF3

    #define LCD_888_DB0_GPIO        GPIOA
    #define LCD_888_DB0_PIN         GPIO_PIN_5
    #define LCD_888_DB0_AF          GPIO_AF3

    #define LCD_888_DB1_GPIO        GPIOA
    #define LCD_888_DB1_PIN         GPIO_PIN_6
    #define LCD_888_DB1_AF          GPIO_AF3

    #define LCD_888_DB2_GPIO        GPIOA
    #define LCD_888_DB2_PIN         GPIO_PIN_7
    #define LCD_888_DB2_AF          GPIO_AF3

    #define LCD_888_DB3_GPIO        GPIOA
    #define LCD_888_DB3_PIN         GPIO_PIN_8
    #define LCD_888_DB3_AF          GPIO_AF3

    #define LCD_888_DB4_GPIO        GPIOA
    #define LCD_888_DB4_PIN         GPIO_PIN_9
    #define LCD_888_DB4_AF          GPIO_AF3

    #define LCD_888_DB5_GPIO        GPIOA
    #define LCD_888_DB5_PIN         GPIO_PIN_10
    #define LCD_888_DB5_AF          GPIO_AF3

    #define LCD_888_DB6_GPIO        GPIOA
    #define LCD_888_DB6_PIN         GPIO_PIN_11
    #define LCD_888_DB6_AF          GPIO_AF3

    #define LCD_888_DB7_GPIO        GPIOA
    #define LCD_888_DB7_PIN         GPIO_PIN_12
    #define LCD_888_DB7_AF          GPIO_AF3

    #define LCD_888_TEARING_GPIO    GPIOA
    #define LCD_888_TEARING_PIN     GPIO_PIN_15
    #define LCD_888_TEARING_AF      GPIO_AF3

    #define LCD_888_PIXELS_H                (800)//Horizontal pixel
    #define LCD_888_PIXELS_V                (480)//Vertical pixel
    #define LCD_888_HYPER_BASEADDR          (0x20000100U)
    #define LCD_888_BURST_LENGTH            (4)
    #define LCD_888_INPUT_COLOR             DISPLAY_InputColorMode_RGB565
    #define LCD_888_OUTPUT_COLOR            DISPLAY_OutputColorMode_RGB565
    #define LCD_888_AHMI_OUTPUT_COLOR       LCD_888_INPUT_COLOR
#endif /* DRV_DISPLAY_888_ENABLED */

/* LCD mrb3205 Configuration parameter
 */
#if(defined DRV_DISPLAY_MRB3205_ENABLED && DRV_DISPLAY_MRB3205_ENABLED == 1U)
    #define LCD_MRB3205_RESET_GPIO      GPIOA
    #define LCD_MRB3205_RESET_PIN       GPIO_PIN_13

    #define LCD_MRB3205_SCK_GPIO        GPIOA
    #define LCD_MRB3205_SCK_PIN         GPIO_PIN_0
    #define LCD_MRB3205_SCK_AF          GPIO_AF3

    #define LCD_MRB3205_WRN_GPIO        GPIOA
    #define LCD_MRB3205_WRN_PIN         GPIO_PIN_1
    #define LCD_MRB3205_WRN_AF          GPIO_AF3

    #define LCD_MRB3205_DCN_GPIO        GPIOA
    #define LCD_MRB3205_DCN_PIN         GPIO_PIN_2
    #define LCD_MRB3205_DCN_AF          GPIO_AF3

    #define LCD_MRB3205_RDN_GPIO        GPIOA
    #define LCD_MRB3205_RDN_PIN         GPIO_PIN_3
    #define LCD_MRB3205_RDN_AF          GPIO_AF3

    #define LCD_MRB3205_CSN_GPIO        GPIOA
    #define LCD_MRB3205_CSN_PIN         GPIO_PIN_4
    #define LCD_MRB3205_CSN_AF          GPIO_AF3

    #define LCD_MRB3205_DB0_GPIO        GPIOA
    #define LCD_MRB3205_DB0_PIN         GPIO_PIN_5
    #define LCD_MRB3205_DB0_AF          GPIO_AF3

    #define LCD_MRB3205_DB1_GPIO        GPIOA
    #define LCD_MRB3205_DB1_PIN         GPIO_PIN_6
    #define LCD_MRB3205_DB1_AF          GPIO_AF3

    #define LCD_MRB3205_DB2_GPIO        GPIOA
    #define LCD_MRB3205_DB2_PIN         GPIO_PIN_7
    #define LCD_MRB3205_DB2_AF          GPIO_AF3

    #define LCD_MRB3205_DB3_GPIO        GPIOA
    #define LCD_MRB3205_DB3_PIN         GPIO_PIN_8
    #define LCD_MRB3205_DB3_AF          GPIO_AF3

    #define LCD_MRB3205_DB4_GPIO        GPIOA
    #define LCD_MRB3205_DB4_PIN         GPIO_PIN_9
    #define LCD_MRB3205_DB4_AF          GPIO_AF3

    #define LCD_MRB3205_DB5_GPIO        GPIOA
    #define LCD_MRB3205_DB5_PIN         GPIO_PIN_10
    #define LCD_MRB3205_DB5_AF          GPIO_AF3

    #define LCD_MRB3205_DB6_GPIO        GPIOA
    #define LCD_MRB3205_DB6_PIN         GPIO_PIN_11
    #define LCD_MRB3205_DB6_AF          GPIO_AF3

    #define LCD_MRB3205_DB7_GPIO        GPIOA
    #define LCD_MRB3205_DB7_PIN         GPIO_PIN_12
    #define LCD_MRB3205_DB7_AF          GPIO_AF3

    #define LCD_MRB3205_TEARING_GPIO    GPIOA
    #define LCD_MRB3205_TEARING_PIN     GPIO_PIN_15
    #define LCD_MRB3205_TEARING_AF      GPIO_AF3

    #define LCD_MRB3205_PIXELS_H                (240)//Horizontal pixel
    #define LCD_MRB3205_PIXELS_V                (320)//Vertical pixel
    #define LCD_MRB3205_HYPER_BASEADDR          (0x20000100U)
    #define LCD_MRB3205_BURST_LENGTH            (4)
    #define LCD_MRB3205_INPUT_COLOR             DISPLAY_InputColorMode_RGB565
    #define LCD_MRB3205_OUTPUT_COLOR            DISPLAY_OutputColorMode_RGB565
    #define LCD_MRB3205_AHMI_OUTPUT_COLOR       LCD_MRB3205_INPUT_COLOR
#endif /* DRV_DISPLAY_MRB3205_ENABLED */

/* LCD sjxd Configuration parameter
 */
#if(defined DRV_DISPLAY_SJXD_ENABLED && DRV_DISPLAY_SJXD_ENABLED == 1U)
    #define LCD_SJXD_RESET_GPIO      GPIOA
    #define LCD_SJXD_RESET_PIN       GPIO_PIN_14

    #define LCD_SJXD_SCK_GPIO        GPIOA
    #define LCD_SJXD_SCK_PIN         GPIO_PIN_2
    #define LCD_SJXD_SCK_AF          GPIO_AF2

    #define LCD_SJXD_CSN_GPIO        GPIOA
    #define LCD_SJXD_CSN_PIN         GPIO_PIN_3
    #define LCD_SJXD_CSN_AF          GPIO_AF2

    #define LCD_SJXD_DQ0_GPIO        GPIOA
    #define LCD_SJXD_DQ0_PIN         GPIO_PIN_4
    #define LCD_SJXD_DQ0_AF          GPIO_AF2

    #define LCD_SJXD_DQ1_GPIO        GPIOA
    #define LCD_SJXD_DQ1_PIN         GPIO_PIN_5
    #define LCD_SJXD_DQ1_AF          GPIO_AF2

    #define LCD_SJXD_DQ2_GPIO        GPIOA
    #define LCD_SJXD_DQ2_PIN         GPIO_PIN_6
    #define LCD_SJXD_DQ2_AF          GPIO_AF2

    #define LCD_SJXD_DQ3_GPIO        GPIOA
    #define LCD_SJXD_DQ3_PIN         GPIO_PIN_7
    #define LCD_SJXD_DQ3_AF          GPIO_AF2

    #define LCD_SJXD_TEARING_GPIO    GPIOA
    #define LCD_SJXD_TEARING_PIN     GPIO_PIN_15
    #define LCD_SJXD_TEARING_AF      GPIO_AF3

    #define LCD_SJXD_PIXELS_H                   (360)//Horizontal pixel
    #define LCD_SJXD_PIXELS_V                   (360)//Vertical pixel
    #define LCD_SJXD_HYPER_BASEADDR             (0x20100100U)
    #define LCD_SJXD_BURST_LENGTH               (4)
    #define LCD_SJXD_INPUT_COLOR                DISPLAY_InputColorMode_RGB565
    #define LCD_SJXD_OUTPUT_COLOR               DISPLAY_OutputColorMode_RGB565
    #define LCD_SJXD_AHMI_OUTPUT_COLOR          LCD_SJXD_INPUT_COLOR
#endif /* DRV_DISPLAY_SJXD_ENABLED */

/* LCD st7512 Configuration parameter
 */
#if(defined DRV_DISPLAY_ST7512_ENABLED && DRV_DISPLAY_ST7512_ENABLED == 1U)
    #define LCD_ST7512_RESET_GPIO      GPIOA
    #define LCD_ST7512_RESET_PIN       GPIO_PIN_14

    #define LCD_ST7512_SCK_GPIO        GPIOA
    #define LCD_ST7512_SCK_PIN         GPIO_PIN_0
    #define LCD_ST7512_SCK_AF          GPIO_AF3

    #define LCD_ST7512_WRN_GPIO        GPIOA
    #define LCD_ST7512_WRN_PIN         GPIO_PIN_1
    #define LCD_ST7512_WRN_AF          GPIO_AF3

    #define LCD_ST7512_DCN_GPIO        GPIOA
    #define LCD_ST7512_DCN_PIN         GPIO_PIN_2
    #define LCD_ST7512_DCN_AF          GPIO_AF3

    #define LCD_ST7512_RDN_GPIO        GPIOA
    #define LCD_ST7512_RDN_PIN         GPIO_PIN_3
    #define LCD_ST7512_RDN_AF          GPIO_AF3

    #define LCD_ST7512_CSN_GPIO        GPIOA
    #define LCD_ST7512_CSN_PIN         GPIO_PIN_4
    #define LCD_ST7512_CSN_AF          GPIO_AF3

    #define LCD_ST7512_DB0_GPIO        GPIOA
    #define LCD_ST7512_DB0_PIN         GPIO_PIN_5
    #define LCD_ST7512_DB0_AF          GPIO_AF3

    #define LCD_ST7512_DB1_GPIO        GPIOA
    #define LCD_ST7512_DB1_PIN         GPIO_PIN_6
    #define LCD_ST7512_DB1_AF          GPIO_AF3

    #define LCD_ST7512_DB2_GPIO        GPIOA
    #define LCD_ST7512_DB2_PIN         GPIO_PIN_7
    #define LCD_ST7512_DB2_AF          GPIO_AF3

    #define LCD_ST7512_DB3_GPIO        GPIOA
    #define LCD_ST7512_DB3_PIN         GPIO_PIN_8
    #define LCD_ST7512_DB3_AF          GPIO_AF3

    #define LCD_ST7512_DB4_GPIO        GPIOA
    #define LCD_ST7512_DB4_PIN         GPIO_PIN_9
    #define LCD_ST7512_DB4_AF          GPIO_AF3

    #define LCD_ST7512_DB5_GPIO        GPIOA
    #define LCD_ST7512_DB5_PIN         GPIO_PIN_10
    #define LCD_ST7512_DB5_AF          GPIO_AF3

    #define LCD_ST7512_DB6_GPIO        GPIOA
    #define LCD_ST7512_DB6_PIN         GPIO_PIN_11
    #define LCD_ST7512_DB6_AF          GPIO_AF3

    #define LCD_ST7512_DB7_GPIO        GPIOA
    #define LCD_ST7512_DB7_PIN         GPIO_PIN_12
    #define LCD_ST7512_DB7_AF          GPIO_AF3

    #define LCD_ST7512_TEARING_GPIO    GPIOA
    #define LCD_ST7512_TEARING_PIN     GPIO_PIN_15
    #define LCD_ST7512_TEARING_AF      GPIO_AF3

    #define LCD_ST7512_PIXELS_H                (320)//Horizontal pixel
    #define LCD_ST7512_PIXELS_V                (240)//Vertical pixel
    #define LCD_ST7512_HYPER_BASEADDR          (0x20000100U)
    #define LCD_ST7512_BURST_LENGTH            (4)
    #define LCD_ST7512_INPUT_COLOR              DISPLAY_InputColorMode_Y8
    #define LCD_ST7512_OUTPUT_COLOR             DISPLAY_OutputColorMode_Y2
    #define LCD_ST7512_AHMI_OUTPUT_COLOR        LCD_ST7512_INPUT_COLOR
#endif /* DRV_DISPLAY_ST7512_ENABLED */

/* LCD st7735s Configuration parameter
 */
#if(defined DRV_DISPLAY_ST7735S_ENABLED && DRV_DISPLAY_ST7735S_ENABLED == 1U)
    /* For SPI & 8080 Interface */
    #define LCD_ST7735S_RESET_GPIO      GPIOB
    #define LCD_ST7735S_RESET_PIN       GPIO_PIN_1

    #define LCD_ST7735S_CSN_GPIO        GPIOA
    #define LCD_ST7735S_CSN_PIN         GPIO_PIN_4
    #define LCD_ST7735S_CSN_AF          GPIO_AF3
    
    #define LCD_ST7735S_DCN_GPIO        GPIOA
    #define LCD_ST7735S_DCN_PIN         GPIO_PIN_2
    #define LCD_ST7735S_DCN_AF          GPIO_AF3
    
    #define LCD_ST7735S_TEARING_GPIO    GPIOA
    #define LCD_ST7735S_TEARING_PIN     GPIO_PIN_15
    #define LCD_ST7735S_TEARING_AF      GPIO_AF3
    /* For SPI Interface */
    #define LCD_ST7735S_SCK_GPIO        GPIOA
    #define LCD_ST7735S_SCK_PIN         GPIO_PIN_2
    #define LCD_ST7735S_SCK_AF          GPIO_AF2

    #define LCD_ST7735S_DQ0_GPIO        GPIOA
    #define LCD_ST7735S_DQ0_PIN         GPIO_PIN_4
    #define LCD_ST7735S_DQ0_AF          GPIO_AF2

    /* For 8080 Interface */
    #define LCD_ST7735S_WRN_GPIO        GPIOA
    #define LCD_ST7735S_WRN_PIN         GPIO_PIN_1
    #define LCD_ST7735S_WRN_AF          GPIO_AF3

    #define LCD_ST7735S_RDN_GPIO        GPIOA
    #define LCD_ST7735S_RDN_PIN         GPIO_PIN_3
    #define LCD_ST7735S_RDN_AF          GPIO_AF3

    #define LCD_ST7735S_DB0_GPIO        GPIOA
    #define LCD_ST7735S_DB0_PIN         GPIO_PIN_5
    #define LCD_ST7735S_DB0_AF          GPIO_AF3

    #define LCD_ST7735S_DB1_GPIO        GPIOA
    #define LCD_ST7735S_DB1_PIN         GPIO_PIN_6
    #define LCD_ST7735S_DB1_AF          GPIO_AF3

    #define LCD_ST7735S_DB2_GPIO        GPIOA
    #define LCD_ST7735S_DB2_PIN         GPIO_PIN_7
    #define LCD_ST7735S_DB2_AF          GPIO_AF3

    #define LCD_ST7735S_DB3_GPIO        GPIOA
    #define LCD_ST7735S_DB3_PIN         GPIO_PIN_8
    #define LCD_ST7735S_DB3_AF          GPIO_AF3

    #define LCD_ST7735S_DB4_GPIO        GPIOA
    #define LCD_ST7735S_DB4_PIN         GPIO_PIN_9
    #define LCD_ST7735S_DB4_AF          GPIO_AF3

    #define LCD_ST7735S_DB5_GPIO        GPIOA
    #define LCD_ST7735S_DB5_PIN         GPIO_PIN_10
    #define LCD_ST7735S_DB5_AF          GPIO_AF3

    #define LCD_ST7735S_DB6_GPIO        GPIOA
    #define LCD_ST7735S_DB6_PIN         GPIO_PIN_11
    #define LCD_ST7735S_DB6_AF          GPIO_AF3

    #define LCD_ST7735S_DB7_GPIO        GPIOA
    #define LCD_ST7735S_DB7_PIN         GPIO_PIN_12
    #define LCD_ST7735S_DB7_AF          GPIO_AF3
    
    #define LCD_ST7735S_PIXELS_H                (132)//Horizontal pixel
    #define LCD_ST7735S_PIXELS_V                (132)//Vertical pixel
    #define LCD_ST7735S_HYPER_BASEADDR          (0x20000100U)
    #define LCD_ST7735S_BURST_LENGTH            (4)
    #define LCD_ST7735S_INPUT_COLOR             DISPLAY_InputColorMode_RGB565
    #define LCD_ST7735S_OUTPUT_COLOR            DISPLAY_OutputColorMode_RGB565
    #define LCD_ST7735S_AHMI_OUTPUT_COLOR       LCD_ST7735S_INPUT_COLOR
#endif /* DRV_DISPLAY_ST7735S_ENABLED */

/* LCD st77903 Configuration parameter
 */
#if(defined DRV_DISPLAY_ST77903_ENABLED && DRV_DISPLAY_ST77903_ENABLED == 1U)
    #define LCD_ST77903_RESET_GPIO      GPIOA
    #define LCD_ST77903_RESET_PIN       GPIO_PIN_14

    #define LCD_ST77903_SCK_GPIO        GPIOA
    #define LCD_ST77903_SCK_PIN         GPIO_PIN_2
    #define LCD_ST77903_SCK_AF          GPIO_AF2

    #define LCD_ST77903_CSN_GPIO        GPIOA
    #define LCD_ST77903_CSN_PIN         GPIO_PIN_3
    #define LCD_ST77903_CSN_AF          GPIO_AF2

    #define LCD_ST77903_DQ0_GPIO        GPIOA
    #define LCD_ST77903_DQ0_PIN         GPIO_PIN_4
    #define LCD_ST77903_DQ0_AF          GPIO_AF2

    #define LCD_ST77903_DQ1_GPIO        GPIOA
    #define LCD_ST77903_DQ1_PIN         GPIO_PIN_5
    #define LCD_ST77903_DQ1_AF          GPIO_AF2

    #define LCD_ST77903_DQ2_GPIO        GPIOA
    #define LCD_ST77903_DQ2_PIN         GPIO_PIN_6
    #define LCD_ST77903_DQ2_AF          GPIO_AF2

    #define LCD_ST77903_DQ3_GPIO        GPIOA
    #define LCD_ST77903_DQ3_PIN         GPIO_PIN_7
    #define LCD_ST77903_DQ3_AF          GPIO_AF2

    #define LCD_ST77903_TEARING_GPIO    GPIOA
    #define LCD_ST77903_TEARING_PIN     GPIO_PIN_15
    #define LCD_ST77903_TEARING_AF      GPIO_AF3

    #define LCD_ST77903_PIXELS_H                   (400)//Horizontal pixel
    #define LCD_ST77903_PIXELS_V                   (400)//Vertical pixel
    #define LCD_ST77903_HYPER_BASEADDR             (0x20100000U)
    #define LCD_ST77903_BURST_LENGTH               (16)
    #define LCD_ST77903_INPUT_COLOR                 DISPLAY_InputColorMode_RGB565
    #define LCD_ST77903_OUTPUT_COLOR                DISPLAY_OutputColorMode_RGB565
    #define LCD_ST77903_AHMI_OUTPUT_COLOR           LCD_ST77903_INPUT_COLOR
#endif /* DRV_DISPLAY_ST77903_ENABLED */


/* LCD leike Configuration parameter
 */
#if(defined DRV_DISPLAY_LEIKE_ENABLED && DRV_DISPLAY_LEIKE_ENABLED == 1U)
    #define LCD_LEIKE_RESET_GPIO      GPIOA
    #define LCD_LEIKE_RESET_PIN       GPIO_PIN_14

    #define LCD_LEIKE_SCK_GPIO        GPIOA
    #define LCD_LEIKE_SCK_PIN         GPIO_PIN_2
    #define LCD_LEIKE_SCK_AF          GPIO_AF2

    #define LCD_LEIKE_CSN_GPIO        GPIOA
    #define LCD_LEIKE_CSN_PIN         GPIO_PIN_3
    #define LCD_LEIKE_CSN_AF          GPIO_AF2

    #define LCD_LEIKE_DQ0_GPIO        GPIOA
    #define LCD_LEIKE_DQ0_PIN         GPIO_PIN_4
    #define LCD_LEIKE_DQ0_AF          GPIO_AF2

    #define LCD_LEIKE_DQ1_GPIO        GPIOA
    #define LCD_LEIKE_DQ1_PIN         GPIO_PIN_5
    #define LCD_LEIKE_DQ1_AF          GPIO_AF2

    #define LCD_LEIKE_DQ2_GPIO        GPIOA
    #define LCD_LEIKE_DQ2_PIN         GPIO_PIN_6
    #define LCD_LEIKE_DQ2_AF          GPIO_AF2

    #define LCD_LEIKE_DQ3_GPIO        GPIOA
    #define LCD_LEIKE_DQ3_PIN         GPIO_PIN_7
    #define LCD_LEIKE_DQ3_AF          GPIO_AF2

    #define LCD_LEIKE_TEARING_GPIO    GPIOA
    #define LCD_LEIKE_TEARING_PIN     GPIO_PIN_15
    #define LCD_LEIKE_TEARING_AF      GPIO_AF3

    #define LCD_LEIKE_PIXELS_H                   (368)//Horizontal pixel
    #define LCD_LEIKE_PIXELS_V                   (448)//Vertical pixel
    #define LCD_LEIKE_HYPER_BASEADDR             (0x20100000U)
    #define LCD_LEIKE_BURST_LENGTH               (16)
    #define LCD_LEIKE_INPUT_COLOR                 DISPLAY_InputColorMode_RGB565
    #define LCD_LEIKE_OUTPUT_COLOR                DISPLAY_OutputColorMode_RGB565
    #define LCD_LEIKE_AHMI_OUTPUT_COLOR           LCD_LEIKE_INPUT_COLOR
#endif /* DRV_DISPLAY_LEIKE_ENABLED */


/* LCD TFT128QM071 Configuration parameter
 */
#if(defined DRV_DISPLAY_TFT128QM071_ENABLED && DRV_DISPLAY_TFT128QM071_ENABLED == 1U)
    #define LCD_TFT128QM071_RESET_GPIO      GPIOA
    #define LCD_TFT128QM071_RESET_PIN       GPIO_PIN_14

    #define LCD_TFT128QM071_SCK_GPIO        GPIOA
    #define LCD_TFT128QM071_SCK_PIN         GPIO_PIN_2
    #define LCD_TFT128QM071_SCK_AF          GPIO_AF2

    #define LCD_TFT128QM071_CSN_GPIO        GPIOA
    #define LCD_TFT128QM071_CSN_PIN         GPIO_PIN_3
    #define LCD_TFT128QM071_CSN_AF          GPIO_AF2

    #define LCD_TFT128QM071_DQ0_GPIO        GPIOA
    #define LCD_TFT128QM071_DQ0_PIN         GPIO_PIN_4
    #define LCD_TFT128QM071_DQ0_AF          GPIO_AF2

    #define LCD_TFT128QM071_DCN_GPIO        GPIOA
    #define LCD_TFT128QM071_DCN_PIN         GPIO_PIN_5
    #define LCD_TFT128QM071_DCN_AF          GPIO_AF5

    #define LCD_TFT128QM071_TEARING_GPIO    GPIOA
    #define LCD_TFT128QM071_TEARING_PIN     GPIO_PIN_15
    #define LCD_TFT128QM071_TEARING_AF      GPIO_AF3

    #define LCD_TFT128QM071_PIXELS_H                (240)//Horizontal pixel
    #define LCD_TFT128QM071_HYPER_BASEADDR          (0x20000100U)
    #define LCD_TFT128QM071_PIXELS_V                (240)//Vertical pixel
    #define LCD_TFT128QM071_BURST_LENGTH            (4)
    #define LCD_TFT128QM071_INPUT_COLOR             DISPLAY_InputColorMode_RGB565
    #define LCD_TFT128QM071_OUTPUT_COLOR            DISPLAY_OutputColorMode_RGB565
    #define LCD_TFT128QM071_AHMI_OUTPUT_COLOR       LCD_TFT128QM071_INPUT_COLOR
#endif /* DRV_DISPLAY_TFT128QM071_ENABLED */
/* LCD RGB565 Configuration parameter
 */
#if(defined DRV_DISPLAY_RGB565_ENABLED && DRV_DISPLAY_RGB565_ENABLED == 1U)
    #define LCD_RGB565_CK_GPIO         GPIOA
    #define LCD_RGB565_CK_PIN          GPIO_PIN_0
    #define LCD_RGB565_CK_AF           GPIO_AF1

    #define LCD_RGB565_R3_GPIO         GPIOA
    #define LCD_RGB565_R3_PIN          GPIO_PIN_1
    #define LCD_RGB565_R3_AF           GPIO_AF1

    #define LCD_RGB565_R4_GPIO         GPIOA
    #define LCD_RGB565_R4_PIN          GPIO_PIN_2
    #define LCD_RGB565_R4_AF           GPIO_AF1

    #define LCD_RGB565_R5_GPIO         GPIOA
    #define LCD_RGB565_R5_PIN          GPIO_PIN_3
    #define LCD_RGB565_R5_AF           GPIO_AF1

    #define LCD_RGB565_R6_GPIO         GPIOA
    #define LCD_RGB565_R6_PIN          GPIO_PIN_4
    #define LCD_RGB565_R6_AF           GPIO_AF1

    #define LCD_RGB565_R7_GPIO         GPIOA
    #define LCD_RGB565_R7_PIN          GPIO_PIN_5
    #define LCD_RGB565_R7_AF           GPIO_AF1

    #define LCD_RGB565_G2_GPIO         GPIOA
    #define LCD_RGB565_G2_PIN          GPIO_PIN_6
    #define LCD_RGB565_G2_AF           GPIO_AF1
	
    #define LCD_RGB565_G3_GPIO         GPIOA
    #define LCD_RGB565_G3_PIN          GPIO_PIN_7
    #define LCD_RGB565_G3_AF           GPIO_AF1
    
    #define LCD_RGB565_G4_GPIO         GPIOA
    #define LCD_RGB565_G4_PIN          GPIO_PIN_8
    #define LCD_RGB565_G4_AF           GPIO_AF1

    #define LCD_RGB565_G5_GPIO         GPIOA
    #define LCD_RGB565_G5_PIN          GPIO_PIN_9
    #define LCD_RGB565_G5_AF           GPIO_AF1

    #define LCD_RGB565_G6_GPIO         GPIOA
    #define LCD_RGB565_G6_PIN          GPIO_PIN_10
    #define LCD_RGB565_G6_AF           GPIO_AF1

    #define LCD_RGB565_G7_GPIO         GPIOA
    #define LCD_RGB565_G7_PIN          GPIO_PIN_11
    #define LCD_RGB565_G7_AF           GPIO_AF1
    
    #define LCD_RGB565_B3_GPIO         GPIOA
    #define LCD_RGB565_B3_PIN          GPIO_PIN_12
    #define LCD_RGB565_B3_AF           GPIO_AF1
    
    #define LCD_RGB565_B4_GPIO         GPIOA
    #define LCD_RGB565_B4_PIN          GPIO_PIN_13
    #define LCD_RGB565_B4_AF           GPIO_AF1

    #define LCD_RGB565_B5_GPIO         GPIOA
    #define LCD_RGB565_B5_PIN          GPIO_PIN_14
    #define LCD_RGB565_B5_AF           GPIO_AF1

    #define LCD_RGB565_B6_GPIO         GPIOA
    #define LCD_RGB565_B6_PIN          GPIO_PIN_15
    #define LCD_RGB565_B6_AF           GPIO_AF1

    #define LCD_RGB565_B7_GPIO         GPIOA
    #define LCD_RGB565_B7_PIN          GPIO_PIN_16
    #define LCD_RGB565_B7_AF           GPIO_AF1

    #define LCD_RGB565_HS_GPIO         GPIOA
    #define LCD_RGB565_HS_PIN          GPIO_PIN_17
    #define LCD_RGB565_HS_AF           GPIO_AF1

    #define LCD_RGB565_VS_GPIO         GPIOA
    #define LCD_RGB565_VS_PIN          GPIO_PIN_18
    #define LCD_RGB565_VS_AF           GPIO_AF1

    #define LCD_RGB565_DE_GPIO         GPIOA
    #define LCD_RGB565_DE_PIN          GPIO_PIN_19
    #define LCD_RGB565_DE_AF           GPIO_AF1
	
	#define LCD_RGB565_PIXELS_H                   (800)//Horizontal pixel
    #define LCD_RGB565_PIXELS_V                   (480)//Vertical pixel
    #define LCD_RGB565_HYPER_BASEADDR             (0x20108010U)
    #define LCD_RGB565_BURST_LENGTH               (4)
    #define LCD_RGB565_INPUT_COLOR                 DISPLAY_InputColorMode_RGB565
    #define LCD_RGB565_OUTPUT_COLOR                DISPLAY_OutputColorMode_RGB565
    #define LCD_RGB565_AHMI_OUTPUT_COLOR           LCD_RGB565_INPUT_COLOR
#endif /* DRV_DISPLAY_RGB565_ENABLED */

/* LCD GC9307N Configuration parameter
 */
#if(defined DRV_DISPLAY_GC9307N_ENABLED && DRV_DISPLAY_GC9307N_ENABLED == 1U)
    /* For SPI & 8080 Interface */
    #define LCD_GC9307N_RESET_GPIO      GPIOA
    #define LCD_GC9307N_RESET_PIN       GPIO_PIN_13

    #define LCD_GC9307N_CSN_GPIO        GPIOA
    #define LCD_GC9307N_CSN_PIN         GPIO_PIN_4
    #define LCD_GC9307N_CSN_AF          GPIO_AF3
    
    #define LCD_GC9307N_DCN_GPIO        GPIOA
    #define LCD_GC9307N_DCN_PIN         GPIO_PIN_2
    #define LCD_GC9307N_DCN_AF          GPIO_AF3
    
    #define LCD_GC9307N_TEARING_GPIO    GPIOA
    #define LCD_GC9307N_TEARING_PIN     GPIO_PIN_15
    #define LCD_GC9307N_TEARING_AF      GPIO_AF3
    /* For SPI Interface */
    #define LCD_GC9307N_SCK_GPIO        GPIOA
    #define LCD_GC9307N_SCK_PIN         GPIO_PIN_2
    #define LCD_GC9307N_SCK_AF          GPIO_AF2

    #define LCD_GC9307N_DQ0_GPIO        GPIOA
    #define LCD_GC9307N_DQ0_PIN         GPIO_PIN_4
    #define LCD_GC9307N_DQ0_AF          GPIO_AF2

    /* For 8080 Interface */
    #define LCD_GC9307N_WRN_GPIO        GPIOA
    #define LCD_GC9307N_WRN_PIN         GPIO_PIN_1
    #define LCD_GC9307N_WRN_AF          GPIO_AF3

    #define LCD_GC9307N_RDN_GPIO        GPIOA
    #define LCD_GC9307N_RDN_PIN         GPIO_PIN_3
    #define LCD_GC9307N_RDN_AF          GPIO_AF3

    #define LCD_GC9307N_DB0_GPIO        GPIOA
    #define LCD_GC9307N_DB0_PIN         GPIO_PIN_5
    #define LCD_GC9307N_DB0_AF          GPIO_AF3

    #define LCD_GC9307N_DB1_GPIO        GPIOA
    #define LCD_GC9307N_DB1_PIN         GPIO_PIN_6
    #define LCD_GC9307N_DB1_AF          GPIO_AF3

    #define LCD_GC9307N_DB2_GPIO        GPIOA
    #define LCD_GC9307N_DB2_PIN         GPIO_PIN_7
    #define LCD_GC9307N_DB2_AF          GPIO_AF3

    #define LCD_GC9307N_DB3_GPIO        GPIOA
    #define LCD_GC9307N_DB3_PIN         GPIO_PIN_8
    #define LCD_GC9307N_DB3_AF          GPIO_AF3

    #define LCD_GC9307N_DB4_GPIO        GPIOA
    #define LCD_GC9307N_DB4_PIN         GPIO_PIN_9
    #define LCD_GC9307N_DB4_AF          GPIO_AF3

    #define LCD_GC9307N_DB5_GPIO        GPIOA
    #define LCD_GC9307N_DB5_PIN         GPIO_PIN_10
    #define LCD_GC9307N_DB5_AF          GPIO_AF3

    #define LCD_GC9307N_DB6_GPIO        GPIOA
    #define LCD_GC9307N_DB6_PIN         GPIO_PIN_11
    #define LCD_GC9307N_DB6_AF          GPIO_AF3

    #define LCD_GC9307N_DB7_GPIO        GPIOA
    #define LCD_GC9307N_DB7_PIN         GPIO_PIN_12
    #define LCD_GC9307N_DB7_AF          GPIO_AF3
    
    #define LCD_GC9307N_PIXELS_H                (132)//Horizontal pixel
    #define LCD_GC9307N_PIXELS_V                (132)//Vertical pixel
    #define LCD_GC9307N_HYPER_BASEADDR          (0x20000100U)
    #define LCD_GC9307N_BURST_LENGTH            (4)
    #define LCD_GC9307N_INPUT_COLOR             DISPLAY_InputColorMode_RGB565
    #define LCD_GC9307N_OUTPUT_COLOR            DISPLAY_OutputColorMode_RGB565
    #define LCD_GC9307N_AHMI_OUTPUT_COLOR       LCD_GC9307N_INPUT_COLOR
#endif /* DRV_DISPLAY_GC9307N_ENABLED */

#endif /* CONFIG_DRV_DISPLAY_ENABLED */
#endif /* __DISPLAY_CONF_H__ */