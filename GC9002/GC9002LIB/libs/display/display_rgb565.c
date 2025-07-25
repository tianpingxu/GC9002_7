/**
  ******************************************************************************
  * @file    display_rgb565.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/07/24
  * @brief   LCD rgb565 driver functions.
  ******************************************************************************
  */
#include "display_rgb565.h"
#if(defined DRV_DISPLAY_RGB565_ENABLED && DRV_DISPLAY_RGB565_ENABLED == 1U)

static void rgb565_Reset(void);
static void rgb565_Set(void);
static void rgb565_init_lcd_regs(void);

static void rgb565_HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_CK_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_CK_PIN;
    HAL_GPIO_Init(LCD_RGB565_CK_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_HS_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_HS_PIN;
    HAL_GPIO_Init(LCD_RGB565_HS_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_VS_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_VS_PIN;
    HAL_GPIO_Init(LCD_RGB565_VS_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_DE_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_DE_PIN;
    HAL_GPIO_Init(LCD_RGB565_DE_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_R3_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_R3_PIN;
    HAL_GPIO_Init(LCD_RGB565_R3_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_R4_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_R4_PIN;
    HAL_GPIO_Init(LCD_RGB565_R4_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_R5_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_R5_PIN;
    HAL_GPIO_Init(LCD_RGB565_R5_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_R6_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_R6_PIN;
    HAL_GPIO_Init(LCD_RGB565_R6_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_R7_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_R7_PIN;
    HAL_GPIO_Init(LCD_RGB565_R7_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_G2_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_G2_PIN;
    HAL_GPIO_Init(LCD_RGB565_G2_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_G3_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_G3_PIN;
    HAL_GPIO_Init(LCD_RGB565_G3_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_G4_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_G4_PIN;
    HAL_GPIO_Init(LCD_RGB565_G4_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_G5_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_G5_PIN;
    HAL_GPIO_Init(LCD_RGB565_G5_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_G6_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_G6_PIN;
    HAL_GPIO_Init(LCD_RGB565_G6_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_G7_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_G7_PIN;
    HAL_GPIO_Init(LCD_RGB565_G7_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_B3_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_B3_PIN;
    HAL_GPIO_Init(LCD_RGB565_B3_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_B4_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_B4_PIN;
    HAL_GPIO_Init(LCD_RGB565_B4_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_B5_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_B5_PIN;
    HAL_GPIO_Init(LCD_RGB565_B5_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_B6_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_B6_PIN;
    HAL_GPIO_Init(LCD_RGB565_B6_GPIO, &GPIO_InitStruct);
	
    GPIO_InitStruct.Alternate = (uint32_t)LCD_RGB565_B7_AF;
    GPIO_InitStruct.Pin = LCD_RGB565_B7_PIN;
    HAL_GPIO_Init(LCD_RGB565_B7_GPIO, &GPIO_InitStruct);
    
    __HAL_RCC_DISPLAY_SET_DIV(hdisplay->Init.disp_duty_l, hdisplay->Init.disp_duty_h);
}
static void _rgb565_Reset(void)
{
	//HAL_GPIO_WritePin(LCD_rgb565_RESET_GPIO, LCD_rgb565_RESET_PIN, GPIO_PIN_RESET);
}
static void _rgb565_Set(void)
{
	//HAL_GPIO_WritePin(LCD_rgb565_RESET_GPIO, LCD_rgb565_RESET_PIN, GPIO_PIN_SET);
}
#ifdef HAL_AHMI_MODULE_ENABLED
static AHMI_HandleTypeDef hahmi = {.Instance = DRV_AHMI_INSTANCE};
#endif

void display_rgb565_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
	uint8_t PixelBytes = 0;
    HAL_DISPLAY_StructInit(&(hdisplay->Init));
    if(config != NULL)
    {
        hdisplay->Init.timing_param.hpixels  = config->HPIXEL_VALUE;
        hdisplay->Init.timing_param.hsyncpw  = config->HSYNCPW_VALUE;
        hdisplay->Init.timing_param.hfnprch  = config->HFNPRCH_VALUE;
        hdisplay->Init.timing_param.hbkprch  = config->HBKPRCH_VALUE;
        hdisplay->Init.timing_param.vlines   = config->VLINES_VALUE;
        hdisplay->Init.timing_param.vsyncpw  = config->VSYNCPW_VALUE;
        hdisplay->Init.timing_param.vfnprch  = config->VFNPRCH_VALUE;
        hdisplay->Init.timing_param.vbkprch  = config->VBKPRCH_VALUE;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = config->VLINES_VALUE-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = config->HPIXEL_VALUE-1;
        
        hdisplay->Init.invert_oclk_en    = config->invert_oclk_en;
        hdisplay->Init.invert_hs         = config->invert_hs;
        hdisplay->Init.invert_vs         = config->invert_vs;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = config->input_color_mode; //rgb565
        hdisplay->Init.output_color_mode = config->output_color_mode; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_24BIT;// 8080-24bit
        hdisplay->Init.running_type      = DISPLAY_Mode_MasterActive;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_ENABLE;
        hdisplay->Init.clk_spi_div       = config->clk_spi_div; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_DISABLE;
        
        hdisplay->Init.master_param.base_addr      = config->frame_buffer_addr; //hyper ram
		if(hdisplay->Init.input_color_mode == DISPLAY_InputColorMode_RGBA8888)
		{
			PixelBytes = 4;//8bitR 8bitG 8bitB 8bitA
		}
		else if(hdisplay->Init.input_color_mode == DISPLAY_InputColorMode_RGB565)
		{
			PixelBytes = 2;//5bitR 6bitG 5bitB
		}
		else
		{
			PixelBytes = 2;//warning
		}
        hdisplay->Init.master_param.total_byte_cnt = config->HPIXEL_VALUE*config->VLINES_VALUE*PixelBytes;
        hdisplay->Init.master_param.burst_length   = LCD_RGB565_BURST_LENGTH; //one byte 8 pixels
        
        hdisplay->Init.disp_duty_l = config->disp_duty_l;
        hdisplay->Init.disp_duty_h = config->disp_duty_h;
        hdisplay->Init.disp_qspi_duty_l = config->disp_qspi_duty_l;
        hdisplay->Init.disp_qspi_duty_h = config->disp_qspi_duty_h;
    }
    else
    {
        hdisplay->Init.timing_param.hpixels  = LCD_RGB565_PIXELS_H;
        hdisplay->Init.timing_param.hsyncpw  = 20;
        hdisplay->Init.timing_param.hfnprch  = 210;
        hdisplay->Init.timing_param.hbkprch  = 46;
        hdisplay->Init.timing_param.vlines   = LCD_RGB565_PIXELS_V;
        hdisplay->Init.timing_param.vsyncpw  = 10;
        hdisplay->Init.timing_param.vfnprch  = 22;
        hdisplay->Init.timing_param.vbkprch  = 23;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = LCD_RGB565_PIXELS_V-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = LCD_RGB565_PIXELS_H-1;
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = LCD_RGB565_INPUT_COLOR; //rgb565
        hdisplay->Init.output_color_mode = LCD_RGB565_OUTPUT_COLOR; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_24BIT;// 8080-24bit
        hdisplay->Init.running_type      = DISPLAY_Mode_MasterActive;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_ENABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_2; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_DISABLE;
        
        hdisplay->Init.master_param.base_addr      = LCD_RGB565_HYPER_BASEADDR; //hyper ram
        hdisplay->Init.master_param.total_byte_cnt = LCD_RGB565_PIXELS_H*LCD_RGB565_PIXELS_V*2;
        hdisplay->Init.master_param.burst_length   = LCD_RGB565_BURST_LENGTH; //one byte 8 pixels
    }

    HAL_DISPLAY_RegisterCallback(hdisplay, HAL_DISPLAY_MSPINIT_CB_ID, rgb565_HAL_DISPLAY_MspInit);
    HAL_DISPLAY_Init(hdisplay);
}
void display_rgb565_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
	HAL_DISPLAY_DitheringSet(hdisplay, false);
	HAL_DISPLAY_UpdateSet(hdisplay, true);
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_MasterPassive);
#ifdef HAL_AHMI_MODULE_ENABLED
	// AHMI Config
    HAL_AHMI_StructInit(&hahmi.Init);
    if(config != NULL)
    {
        hahmi.Init.Gate_CLK_EN          = AHMI_ENABLE;
        hahmi.Init.DramCacheReset       = AHMI_DISABLE;
        hahmi.Init.AHMI_CoreReset       = AHMI_DISABLE;
        hahmi.Init.ROTATION             = AHMI_Rotation_0;
        hahmi.Init.FilterRange          = 128;
        hahmi.Init.X_START_TILE         = 0;
        hahmi.Init.X_STOP_TILE          = (config->HPIXEL_VALUE/DRV_AHMI_TILE_BUFFER_SIZE - ((config->HPIXEL_VALUE%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (config->VLINES_VALUE/DRV_AHMI_TILE_BUFFER_SIZE - ((config->VLINES_VALUE%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        // hahmi.Init.SB_BASE_ADDR	        = config->source_buffer_addr;
        hahmi.Init.FB_BASE_ADDR         = config->frame_buffer_addr;
        hahmi.Init.FRAME_HEIGHT         = config->VLINES_VALUE; 
        hahmi.Init.FRAME_WIDTH          = config->HPIXEL_VALUE;
        hahmi.Init.OUTPUT_COLOR_BITS    = config->input_color_mode;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    else
    {
        hahmi.Init.Gate_CLK_EN          = AHMI_ENABLE;
        hahmi.Init.DramCacheReset       = AHMI_DISABLE;
        hahmi.Init.AHMI_CoreReset       = AHMI_DISABLE;
        hahmi.Init.ROTATION             = AHMI_Rotation_0;
        hahmi.Init.FilterRange          = 128;
        hahmi.Init.X_START_TILE         = 0;
        hahmi.Init.X_STOP_TILE          = (LCD_rgb565_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_rgb565_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_rgb565_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_rgb565_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_AHMI_FRAME_BUFFER_BASE_ADDE;
        hahmi.Init.FRAME_HEIGHT         = LCD_rgb565_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_rgb565_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_rgb565_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}
#endif /* DRV_DISPLAY_RGB565_ENABLED */
