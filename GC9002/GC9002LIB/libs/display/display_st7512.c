/**
  ******************************************************************************
  * @file    display_st7512.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/13
  * @brief   LCD st7512 driver functions.
  ******************************************************************************
  */
#include "display_st7512.h"
#if(defined DRV_DISPLAY_ST7512_ENABLED && DRV_DISPLAY_ST7512_ENABLED == 1U)

static void ST7512_Reset(void);
static void ST7512_Set(void);
static void st7512_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay);

static void ST7512_HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LCD_ST7512_RESET_PIN;
	HAL_GPIO_Init(LCD_ST7512_RESET_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_ST7512_RESET_GPIO, GPIO_InitStruct.Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_SCK_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_SCK_PIN;
    HAL_GPIO_Init(LCD_ST7512_SCK_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_WRN_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_WRN_PIN;
    HAL_GPIO_Init(LCD_ST7512_WRN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DCN_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DCN_PIN;
    HAL_GPIO_Init(LCD_ST7512_DCN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_RDN_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_RDN_PIN;
    HAL_GPIO_Init(LCD_ST7512_RDN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_CSN_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_CSN_PIN;
    HAL_GPIO_Init(LCD_ST7512_CSN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB0_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB0_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB0_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB1_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB1_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB1_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB2_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB2_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB2_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB3_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB3_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB3_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB4_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB4_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB4_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB5_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB5_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB5_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB6_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB6_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB6_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_DB7_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_DB7_PIN;
    HAL_GPIO_Init(LCD_ST7512_DB7_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST7512_TEARING_AF;
    GPIO_InitStruct.Pin = LCD_ST7512_TEARING_PIN;
    HAL_GPIO_Init(LCD_ST7512_TEARING_GPIO, &GPIO_InitStruct);
    
    __HAL_RCC_DISPLAY_SET_DIV(hdisplay->Init.disp_duty_l, hdisplay->Init.disp_duty_h);
}
static void ST7512_Reset(void)
{
	HAL_GPIO_WritePin(LCD_ST7512_RESET_GPIO, LCD_ST7512_RESET_PIN, GPIO_PIN_RESET);
}
static void ST7512_Set(void)
{
	HAL_GPIO_WritePin(LCD_ST7512_RESET_GPIO, LCD_ST7512_RESET_PIN, GPIO_PIN_SET);
}
static DISPLAY_HandleTypeDef hdisplay = {.Instance = DRV_DISPLAY_INSTANCE};
#ifdef HAL_AHMI_MODULE_ENABLED
static AHMI_HandleTypeDef hahmi = {.Instance = DRV_AHMI_INSTANCE};
#endif

void display_st7512_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = config->input_color_mode; // Y8
        hdisplay->Init.output_color_mode = config->output_color_mode; // Y2
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_8BIT;// 8080-8bit
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
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
        hdisplay->Init.master_param.burst_length   = LCD_ST7512_BURST_LENGTH; //one byte 8 pixels
        
        hdisplay->Init.disp_duty_l = config->disp_duty_l;
        hdisplay->Init.disp_duty_h = config->disp_duty_h;
        hdisplay->Init.disp_qspi_duty_l = config->disp_qspi_duty_l;
        hdisplay->Init.disp_qspi_duty_h = config->disp_qspi_duty_h;
    }
    else
    {
        hdisplay->Init.timing_param.hpixels  = LCD_ST7512_PIXELS_H;
        hdisplay->Init.timing_param.hsyncpw  = 0x0F;
        hdisplay->Init.timing_param.hfnprch  = 0x0F;
        hdisplay->Init.timing_param.hbkprch  = 0x0F;
        hdisplay->Init.timing_param.vlines   = LCD_ST7512_PIXELS_V;
        hdisplay->Init.timing_param.vsyncpw  = 0x0F;
        hdisplay->Init.timing_param.vfnprch  = 0x01;
        hdisplay->Init.timing_param.vbkprch  = 0x01;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = LCD_ST7512_PIXELS_V-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = LCD_ST7512_PIXELS_H-1;
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = LCD_ST7512_INPUT_COLOR; // Y8
        hdisplay->Init.output_color_mode = LCD_ST7512_OUTPUT_COLOR; // Y2
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_8BIT;// 8080-8bit
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_32; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_ENABLE;
        
        hdisplay->Init.master_param.base_addr      = LCD_ST7512_HYPER_BASEADDR; //hyper ram
        hdisplay->Init.master_param.total_byte_cnt = LCD_ST7512_PIXELS_H*LCD_ST7512_PIXELS_V*2;
        hdisplay->Init.master_param.burst_length   = LCD_ST7512_BURST_LENGTH; //one byte 8 pixels
    }
    /*!< 发送命令的格式选择，目前有3种
         0代表地址格式是00xx00，写缓存的数据紧跟写缓存命令.
         1代表地址格式是00xxyy，其中yy是帧数，写缓存命令后还需要发送0xa5，帧数固定为0. 
         2代表地址格式是00xxyy，其中yy是帧数，用于单色屏的blinking. */
    hdisplay->Init.rc_inst.inst_type_sel = 0x01;
    hdisplay->Init.rc_inst.wr_ram_inst   = 0x3A;
    hdisplay->Init.rc_inst.set_col_inst  = 0x23;
    hdisplay->Init.rc_inst.set_row_inst  = 0x22;

    HAL_DISPLAY_RegisterCallback(hdisplay, HAL_DISPLAY_MSPINIT_CB_ID, ST7512_HAL_DISPLAY_MspInit);
    HAL_DISPLAY_Init(hdisplay);
	
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_Configuration);
    st7512_init_lcd_regs(hdisplay);
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? config->clk_spi_div : DISPLAY_DIV_32));
}
void display_st7512_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
	HAL_DISPLAY_DitheringSet(hdisplay, false);
    HAL_DISPLAY_UpdateSet(hdisplay, true);
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
        hahmi.Init.X_STOP_TILE          = (LCD_ST7512_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_ST7512_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_ST7512_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_ST7512_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
        hahmi.Init.FRAME_HEIGHT         = LCD_ST7512_PIXELS_V;
        hahmi.Init.FRAME_WIDTH          = LCD_ST7512_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_ST7512_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}

static void st7512_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay)
{
    //------------------------------ST7512 Reset Sequence-------------------------------//
	ST7512_Reset();
	mdelay(LCD_ST7512_RESET_DELAY_MS);
	ST7512_Set();
    mdelay(LCD_ST7512_SETUP_DELAY_MS); //100ms
	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x11); //Exit Sleep
	mdelay(LCD_ST7512_SLEEP_DELAY_MS);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x5E); //sw reset
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);

    //----------------------------DISPLAY Control setting--------------------------------------//
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x49); // Set VCOM Offset
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x61); // Set Power Control
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x8F); // n=2); all power on
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x44);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x5A); // Optional
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x21);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x71);
    //Set Electronic Volume1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1E);
    //VCOM=-0.675V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0B);
    //VGH=15V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0B);
    //VGL=-10V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x72);
    //Set Electronic Volume2
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17);
    //GVDD=5.4V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17);
    //GVCL=-5.4V
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x86);
    //Set Gamma Positive1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x82);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x81);
    //Set Gamma Positive1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x16);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x82);
    //Set Gamma Positive2
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x21);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x83);
    //Set Gamma Positive3
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x23);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x24);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x28);

    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x84);
    //Set Gamma Positive4
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x34);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x89);
    //Set Gamma Negative1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x16);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8A);
    //Set Gamma Negative2
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x21);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8B);
    //Set Gamma Negative3
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x23);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x24);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x28);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8C);
    //Set Gamma Negative4
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x34);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x16);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5); 
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x21);
    //Set Memory Address Control
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x13);
    //Sleep out
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x22);
    //Set Page Address
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    //from page0
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x27);
    //to page 159
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x23);
    //Set Column Address
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    //from col0
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    //to col239
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xEF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x25);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    //	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x24);
    //	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
    //	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    //	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    //	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x15);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x16);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x12);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x3A);//Display on
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA5);
	mdelay(LCD_ST7512_OPEN_SREEN_DELAY_MS);
}

#endif /* DRV_DISPLAY_ST7512_ENABLED */
