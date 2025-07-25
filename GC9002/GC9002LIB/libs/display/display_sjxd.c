/**
  ******************************************************************************
  * @file    display_sjxd.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/13
  * @brief   LCD sjxd driver functions.
  ******************************************************************************
  */
#include "display_sjxd.h"
#if(defined DRV_DISPLAY_SJXD_ENABLED && DRV_DISPLAY_SJXD_ENABLED == 1U)

static void SJXD_Reset(void);
static void SJXD_Set(void);
static void sjxd_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay);

static void SJXD_HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LCD_SJXD_RESET_PIN;
	HAL_GPIO_Init(LCD_SJXD_RESET_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_SJXD_RESET_GPIO, GPIO_InitStruct.Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_SJXD_SCK_AF;
    GPIO_InitStruct.Pin = LCD_SJXD_SCK_PIN;
    HAL_GPIO_Init(LCD_SJXD_SCK_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_SJXD_CSN_AF;
    GPIO_InitStruct.Pin = LCD_SJXD_CSN_PIN;
    HAL_GPIO_Init(LCD_SJXD_CSN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_SJXD_DQ0_AF;
    GPIO_InitStruct.Pin = LCD_SJXD_DQ0_PIN;
    HAL_GPIO_Init(LCD_SJXD_DQ0_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_SJXD_DQ1_AF;
    GPIO_InitStruct.Pin = LCD_SJXD_DQ1_PIN;
    HAL_GPIO_Init(LCD_SJXD_DQ1_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_SJXD_DQ2_AF;
    GPIO_InitStruct.Pin = LCD_SJXD_DQ2_PIN;
    HAL_GPIO_Init(LCD_SJXD_DQ2_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_SJXD_DQ3_AF;
    GPIO_InitStruct.Pin = LCD_SJXD_DQ3_PIN;
    HAL_GPIO_Init(LCD_SJXD_DQ3_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = (uint32_t)LCD_SJXD_TEARING_AF;
    GPIO_InitStruct.Pin = LCD_SJXD_TEARING_PIN;
    HAL_GPIO_Init(LCD_SJXD_TEARING_GPIO, &GPIO_InitStruct);
        
    __HAL_RCC_DISPLAY_SET_DIV(hdisplay->Init.disp_duty_l, hdisplay->Init.disp_duty_h);
    __HAL_RCC_DISPLAY_QSPI_SET_DIV(hdisplay->Init.disp_qspi_duty_l, hdisplay->Init.disp_qspi_duty_h);
}
static void SJXD_Reset(void)
{
	HAL_GPIO_WritePin(LCD_SJXD_RESET_GPIO, LCD_SJXD_RESET_PIN, GPIO_PIN_RESET);
}
static void SJXD_Set(void)
{
	HAL_GPIO_WritePin(LCD_SJXD_RESET_GPIO, LCD_SJXD_RESET_PIN, GPIO_PIN_SET);
}
#ifdef HAL_AHMI_MODULE_ENABLED
static AHMI_HandleTypeDef hahmi = {.Instance = DRV_AHMI_INSTANCE};
#endif

void display_sjxd_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
	uint8_t PixelBytes = 0;
    HAL_DISPLAY_StructInit(&hdisplay->Init);

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
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = config->input_color_mode; //rgb565
        hdisplay->Init.output_color_mode = config->output_color_mode; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_QSPI;// QSPI
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_32; //lowest clock
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
        hdisplay->Init.master_param.burst_length   = LCD_SJXD_BURST_LENGTH; //one byte 8 pixels
        
        hdisplay->Init.disp_duty_l = config->disp_duty_l;
        hdisplay->Init.disp_duty_h = config->disp_duty_h;
        hdisplay->Init.disp_qspi_duty_l = config->disp_qspi_duty_l;
        hdisplay->Init.disp_qspi_duty_h = config->disp_qspi_duty_h;
    }
    else
    {
        hdisplay->Init.timing_param.hpixels  = LCD_SJXD_PIXELS_H;
        hdisplay->Init.timing_param.hsyncpw  = 0x0F;
        hdisplay->Init.timing_param.hfnprch  = 0x0F;
        hdisplay->Init.timing_param.hbkprch  = 0x0F;
        hdisplay->Init.timing_param.vlines   = LCD_SJXD_PIXELS_V;
        hdisplay->Init.timing_param.vsyncpw  = 0x0F;
        hdisplay->Init.timing_param.vfnprch  = 0x01;
        hdisplay->Init.timing_param.vbkprch  = 0x01;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = LCD_SJXD_PIXELS_V-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = LCD_SJXD_PIXELS_H-1;
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = LCD_SJXD_INPUT_COLOR; //rgb565
        hdisplay->Init.output_color_mode = LCD_SJXD_OUTPUT_COLOR; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_QSPI;// QSPI
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_32; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_DISABLE;
        
        hdisplay->Init.master_param.base_addr      = LCD_SJXD_HYPER_BASEADDR; //hyper ram
        hdisplay->Init.master_param.total_byte_cnt = LCD_SJXD_PIXELS_H*LCD_SJXD_PIXELS_V*2;
        hdisplay->Init.master_param.burst_length   = LCD_SJXD_BURST_LENGTH; //one byte 8 pixels
    }

    HAL_DISPLAY_RegisterCallback(hdisplay, HAL_DISPLAY_MSPINIT_CB_ID, SJXD_HAL_DISPLAY_MspInit);
    HAL_DISPLAY_Init(hdisplay);
	
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_Configuration);
    sjxd_init_lcd_regs(hdisplay);
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? config->clk_spi_div : DISPLAY_DIV_2));
}
void display_sjxd_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
	HAL_DISPLAY_DitheringSet(hdisplay, true);
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
        hahmi.Init.X_STOP_TILE          = (LCD_SJXD_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_SJXD_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_SJXD_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_SJXD_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_AHMI_FRAME_BUFFER_BASE_ADDE;
        hahmi.Init.FRAME_HEIGHT         = LCD_SJXD_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_SJXD_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_SJXD_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}
void display_sjxd_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
	HAL_DISPLAY_DitheringSet(hdisplay, false);
    HAL_DISPLAY_UpdateSet(hdisplay, false);
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_SlavePassive);
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
        hahmi.Init.X_STOP_TILE          = (LCD_SJXD_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_SJXD_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_SJXD_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_SJXD_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
        hahmi.Init.FRAME_HEIGHT         = LCD_SJXD_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_SJXD_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_SJXD_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}

static void sjxd_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay)
{
    //------------------------------SJXD Reset Sequence-------------------------------//
	SJXD_Reset();
	mdelay(LCD_SJXD_RESET_DELAY_MS);
	SJXD_Set();
    mdelay(LCD_SJXD_SETUP_DELAY_MS); //100ms
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x11);
    mdelay(LCD_SJXD_OPEN_SREEN_DELAY_MS);
    //----------------------------DISPLAY Control setting--------------------------------------//
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xfe);// internal reg enable
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xef);// internal reg enable
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x80);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);//reg_en for 70\74
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x81);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);//reg_en for 7C\7D\7E
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x82);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//reg_en for 90\93
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x83);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);//reg_en for 98\99
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x84);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);//reg_en for B5
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x85);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x42);//reg_en for B9\BE
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x86);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xfc);//reg_en for C2~7
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x87);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//reg_en for C8\CB
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x89);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);//reg_en for EC
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8A);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x4f);//reg_en for F0~3\F6
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8C);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x59);//reg_en for 60\63\64\66
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8D);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x51);//reg_en for 68\6C\6E
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8E);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xae);//reg_en for A1~3\A5\A7
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8F);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xf3);//reg_en for AC~F\A8\A9
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x36);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x3a);  	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);// 565 frame
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xEC);//2COL
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x77);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x74);//rtn 60Hz
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x98);//bvdd 3x
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x99);//bvee -2x
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC3);//VBP
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2A);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC4);//VBN
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);//SRAM RD OPTION
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA2);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);//SRAM RD OPTION
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA9);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1C);//IREF=9.8uA
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);//VDDMA=1.553V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//VDDML=1.24V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xB9);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x8A);//RTERM=101O
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA8);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5E);//VBG_BUF=1.003V, DVDD=1.543V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA7);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x40);//BIAS=10.2uA
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xAF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x73);//VDDSOU=1.715V ,VDDGM=2.002V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xAE);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x44);//VREE=2.475V,VRDD=2.335V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xAD);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x38);//VRGL=1.635V ,VDDSF=2.018V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA3);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5D);//OSC=53.7MHz
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC2);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);//VREG_VREF=2.805V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);//VREG1A=5.99V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC6);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0E);//VREG1B=1.505V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC7);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x13);//VREG2A=-2.995V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC8);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0D);//VREG2B=1.497V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xCB);//bvdd ref_ad
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);//6.09V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x7C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xB6);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26);//13.12V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xAC);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x24);//VGLO=-8.35V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xF6);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);//EPF=2
    //gip start
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xB5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//VFP
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//VBP
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x60);//STV1&2
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x38);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x56);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x63);//STV3&4
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3A);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xE0);//DE
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5B);//MAX=0x61
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x56);//MAX=0x61
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x64);//CLK_group1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x38);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0D);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x72);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xDD);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x56);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x66);//CLK_group1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x38);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x72);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xE1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x56);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x68);//FLC&FLV 1~2
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3B);//FLC12 FREQ
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x29);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5B);	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x6E);//gout_Mapping
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout1_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout2_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout3_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);//gout4_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);//gout5_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x13);//gout6_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);//gout7_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0B);//gout8_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//gout9_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x16);//gout10_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x15);//gout11_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1D);//gout12_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1E);//gout13_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout14_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout15_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout16_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout17_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout18_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout19_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1E);//gout20_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1D);//gout21_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x15);//gout22_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x16);//gout23_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0A);//gout24_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0C);//gout25_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x12);//gout26_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x14);//gout27_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);//gout28_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);//gout29_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout30_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout31_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);//gout32_swap_fw[4:0]
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xBE);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);//SOU_BIAS_FIX=1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x6C);//precharge GATE
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xCC);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xCC);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x84);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xCC);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x50);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x7D);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x72);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x7E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x38);// VGL_BT=1 5X  (BT=0:6X)  RT=0
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);//avdd_clk
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);//avee_clk
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//vdh_clk
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);//vgh_clk
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0C);//vgl_clk
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);//vcl_clk
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);//vdh_clk_porch  0E
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);//vgh_clk_porch  0E
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0C);//vgl_clk_porch  0E 
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);//vcl_clk_porch  0E
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x90);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);//bvdd_clk1_ad1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);//bvdd_clk1_ad1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x93);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x45);//bvdd_shut_ad1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xFF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    //gamma start
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xF0);   
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x45);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2A);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xF1);    
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x43);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x72);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x36);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x37);  
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x6F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xF2);   
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x45);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2A);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xF3);   
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x43);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x72);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x36);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x37); 
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x6F);

	// mdelay(LCD_SJXD_SLEEP_DELAY_MS);
	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x11); //Exit Sleep
	mdelay(LCD_SJXD_OPEN_SREEN_DELAY_MS);
	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x29); //display on	
}

#endif /* DRV_DISPLAY_SJXD_ENABLED */
