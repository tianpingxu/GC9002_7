/**
  ******************************************************************************
  * @file    display_st77903.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/13
  * @brief   LCD st77903 driver functions.
  ******************************************************************************
  */
#include "display_st77903.h"
#if(defined DRV_DISPLAY_ST77903_ENABLED && DRV_DISPLAY_ST77903_ENABLED == 1U)
//#define ST77903_BIST

static void ST77903_Reset(void);
static void ST77903_Set(void);
static void st77903_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay);

static void ST77903_HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LCD_ST77903_RESET_PIN;
	HAL_GPIO_Init(LCD_ST77903_RESET_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_ST77903_RESET_GPIO, GPIO_InitStruct.Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST77903_SCK_AF;
    GPIO_InitStruct.Pin = LCD_ST77903_SCK_PIN;
    HAL_GPIO_Init(LCD_ST77903_SCK_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST77903_CSN_AF;
    GPIO_InitStruct.Pin = LCD_ST77903_CSN_PIN;
    HAL_GPIO_Init(LCD_ST77903_CSN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST77903_DQ0_AF;
    GPIO_InitStruct.Pin = LCD_ST77903_DQ0_PIN;
    HAL_GPIO_Init(LCD_ST77903_DQ0_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST77903_DQ1_AF;
    GPIO_InitStruct.Pin = LCD_ST77903_DQ1_PIN;
    HAL_GPIO_Init(LCD_ST77903_DQ1_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST77903_DQ2_AF;
    GPIO_InitStruct.Pin = LCD_ST77903_DQ2_PIN;
    HAL_GPIO_Init(LCD_ST77903_DQ2_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST77903_DQ3_AF;
    GPIO_InitStruct.Pin = LCD_ST77903_DQ3_PIN;
    HAL_GPIO_Init(LCD_ST77903_DQ3_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = (uint32_t)LCD_ST77903_TEARING_AF;
    GPIO_InitStruct.Pin = LCD_ST77903_TEARING_PIN;
    HAL_GPIO_Init(LCD_ST77903_TEARING_GPIO, &GPIO_InitStruct);
        
    __HAL_RCC_DISPLAY_SET_DIV(hdisplay->Init.disp_duty_l, hdisplay->Init.disp_duty_h);
    __HAL_RCC_DISPLAY_QSPI_SET_DIV(hdisplay->Init.disp_qspi_duty_l, hdisplay->Init.disp_qspi_duty_h);
}
static void ST77903_Reset(void)
{
	HAL_GPIO_WritePin(LCD_ST77903_RESET_GPIO, LCD_ST77903_RESET_PIN, GPIO_PIN_RESET);
}
static void ST77903_Set(void)
{
	HAL_GPIO_WritePin(LCD_ST77903_RESET_GPIO, LCD_ST77903_RESET_PIN, GPIO_PIN_SET);
}
#ifdef HAL_AHMI_MODULE_ENABLED
static AHMI_HandleTypeDef hahmi = {.Instance = DRV_AHMI_INSTANCE};
#endif

void display_st77903_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        hdisplay->Init.master_param.burst_length   = LCD_ST77903_BURST_LENGTH; //longest
        
        hdisplay->Init.disp_duty_l = config->disp_duty_l;
        hdisplay->Init.disp_duty_h = config->disp_duty_h;
        hdisplay->Init.disp_qspi_duty_l = config->disp_qspi_duty_l;
        hdisplay->Init.disp_qspi_duty_h = config->disp_qspi_duty_h;
    }
    else
    {
        hdisplay->Init.timing_param.hpixels  = LCD_ST77903_PIXELS_H;
        hdisplay->Init.timing_param.hsyncpw  = 25;
        hdisplay->Init.timing_param.hfnprch  = 0x0F;
        hdisplay->Init.timing_param.hbkprch  = 0;
        hdisplay->Init.timing_param.vlines   = LCD_ST77903_PIXELS_V;
        hdisplay->Init.timing_param.vsyncpw  = 0x0A;
        hdisplay->Init.timing_param.vfnprch  = 0x06;
        hdisplay->Init.timing_param.vbkprch  = 0x06;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = LCD_ST77903_PIXELS_V-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = LCD_ST77903_PIXELS_H-1;
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = LCD_ST77903_INPUT_COLOR; //rgb565
        hdisplay->Init.output_color_mode = LCD_ST77903_OUTPUT_COLOR; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_QSPI;// QSPI
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_32; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_DISABLE;
        
        hdisplay->Init.master_param.base_addr      = LCD_ST77903_HYPER_BASEADDR; //hyper ram
        hdisplay->Init.master_param.total_byte_cnt = LCD_ST77903_PIXELS_H*LCD_ST77903_PIXELS_V*2;
        hdisplay->Init.master_param.burst_length   = LCD_ST77903_BURST_LENGTH; //longest
    }
    hdisplay->Init.wqspi_inst.wr_4p_inst       = 0xde;
    hdisplay->Init.wqspi_inst.wr_4color_inst   = 0xde;
    hdisplay->Init.wqspi_inst.wr_color_inst    = 0xde;
    hdisplay->Init.wqspi_inst.wr_cmd_inst      = 0xde;
    
    hdisplay->Init.streaming_inst.qspi_rd_type     = DISPLAY_QSPI_1_1_1;
    hdisplay->Init.streaming_inst.qspi_color_type  = DISPLAY_QSPI_1_1_4;
    hdisplay->Init.streaming_inst.qspi_cmd_type    = DISPLAY_QSPI_1_1_1;
    hdisplay->Init.streaming_inst.vs_inst          = 0x61;
    hdisplay->Init.streaming_inst.hs_inst          = 0x60;

    hdisplay->Init.rqspi_inst = 0xdd;

    HAL_DISPLAY_RegisterCallback(hdisplay, HAL_DISPLAY_MSPINIT_CB_ID, ST77903_HAL_DISPLAY_MspInit);
    HAL_DISPLAY_Init(hdisplay);
	
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_Configuration);
    st77903_init_lcd_regs(hdisplay);
}
void display_st77903_master_active(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? config->clk_spi_div : DISPLAY_DIV_2));
	HAL_DISPLAY_DitheringSet(hdisplay, false);
    HAL_DISPLAY_UpdateSet(hdisplay, true);
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_MasterActive);
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
        hahmi.Init.X_STOP_TILE          = (LCD_ST77903_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_ST77903_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_ST77903_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_ST77903_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_AHMI_FRAME_BUFFER_BASE_ADDE;
        hahmi.Init.FRAME_HEIGHT         = LCD_ST77903_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_ST77903_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_ST77903_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}
void display_st77903_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? config->clk_spi_div : DISPLAY_DIV_4));
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
        hahmi.Init.X_STOP_TILE          = (LCD_ST77903_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_ST77903_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_ST77903_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_ST77903_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
        hahmi.Init.FRAME_HEIGHT         = LCD_ST77903_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_ST77903_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_ST77903_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}

static void st77903_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay)
{
    //------------------------------ST77903 Reset Sequence-------------------------------//
    ST77903_Set();
    mdelay(LCD_ST77903_RESET_DELAY_MS);
    ST77903_Reset();
    mdelay(LCD_ST77903_RESET_DELAY_MS);
    ST77903_Set();
    mdelay(LCD_ST77903_SETUP_DELAY_MS); //100ms
    //----------------------------DISPLAY Control setting--------------------------------------//
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xc3);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x96);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xa5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe9);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe7);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x77);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xcc);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x77);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xcf);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x16);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc2);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x77);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xcf);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x16);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc3);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc4);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xed);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe0);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x87);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0c);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x29);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x32);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x49);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1b);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x87);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0c);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x29);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x32);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x49);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1b);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xbe);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xf5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xb1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x25);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe6);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xbe);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xf5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xb1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x25);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xec);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x40);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb2);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb3);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb4);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xa5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x15);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x8a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xa6);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x15);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x8a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xba);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x5a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x23);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x25);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xbb);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x30);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2c);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x82);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x87);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xbc);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x30);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2c);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x82);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x87);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xbd);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xa1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xb2);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2b);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x56);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x43);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x34);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x65);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xff);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xff);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x35);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x36);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0c);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x3a);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);//0x05 means RGB565;0x07 means RGB888
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x21);
	
	#ifdef ST77903_BIST
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb0);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xa5);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xcc);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x40);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3f);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x14);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x14);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
    #endif
	
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x11); //Exit Sleep
    mdelay(LCD_ST77903_SLEEP_DELAY_MS);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x29); //display on	
    mdelay(LCD_ST77903_OPEN_SREEN_DELAY_MS);
}

#endif /* DRV_DISPLAY_ST77903_ENABLED */
