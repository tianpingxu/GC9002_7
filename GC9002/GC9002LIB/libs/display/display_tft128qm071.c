/**
  ******************************************************************************
  * @file    display_tft128qm071.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/13
  * @brief   LCD tft128qm071 driver functions.
  ******************************************************************************
  */
#include "display_tft128qm071.h"
#if(defined DRV_DISPLAY_TFT128QM071_ENABLED && DRV_DISPLAY_TFT128QM071_ENABLED == 1U)

static void TFT128QM071_Reset(void);
static void TFT128QM071_Set(void);
static void tft128qm071_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay);

static void TFT128QM071_HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LCD_TFT128QM071_RESET_PIN;
	HAL_GPIO_Init(LCD_TFT128QM071_RESET_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_TFT128QM071_RESET_GPIO, GPIO_InitStruct.Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_TFT128QM071_SCK_AF;
    GPIO_InitStruct.Pin = LCD_TFT128QM071_SCK_PIN;
    HAL_GPIO_Init(LCD_TFT128QM071_SCK_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_TFT128QM071_CSN_AF;
    GPIO_InitStruct.Pin = LCD_TFT128QM071_CSN_PIN;
    HAL_GPIO_Init(LCD_TFT128QM071_CSN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_TFT128QM071_DQ0_AF;
    GPIO_InitStruct.Pin = LCD_TFT128QM071_DQ0_PIN;
    HAL_GPIO_Init(LCD_TFT128QM071_DQ0_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_TFT128QM071_DCN_AF;
    GPIO_InitStruct.Pin = LCD_TFT128QM071_DCN_PIN;
    HAL_GPIO_Init(LCD_TFT128QM071_DCN_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = (uint32_t)LCD_TFT128QM071_TEARING_AF;
    GPIO_InitStruct.Pin = LCD_TFT128QM071_TEARING_PIN;
    HAL_GPIO_Init(LCD_TFT128QM071_TEARING_GPIO, &GPIO_InitStruct);
        
    __HAL_RCC_DISPLAY_SET_DIV(hdisplay->Init.disp_duty_l, hdisplay->Init.disp_duty_h);
    __HAL_RCC_DISPLAY_QSPI_SET_DIV(hdisplay->Init.disp_qspi_duty_l, hdisplay->Init.disp_qspi_duty_h);
}
static void TFT128QM071_Reset(void)
{
	HAL_GPIO_WritePin(LCD_TFT128QM071_RESET_GPIO, LCD_TFT128QM071_RESET_PIN, GPIO_PIN_RESET);
}
static void TFT128QM071_Set(void)
{
	HAL_GPIO_WritePin(LCD_TFT128QM071_RESET_GPIO, LCD_TFT128QM071_RESET_PIN, GPIO_PIN_SET);
}
#ifdef HAL_AHMI_MODULE_ENABLED
static AHMI_HandleTypeDef hahmi = {.Instance = DRV_AHMI_INSTANCE};
#endif

void display_tft128qm071_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        hdisplay->Init.master_param.burst_length   = LCD_TFT128QM071_BURST_LENGTH; //longest
        
        hdisplay->Init.disp_duty_l = config->disp_duty_l;
        hdisplay->Init.disp_duty_h = config->disp_duty_h;
        hdisplay->Init.disp_qspi_duty_l = config->disp_qspi_duty_l;
        hdisplay->Init.disp_qspi_duty_h = config->disp_qspi_duty_h;
    }
    else
    {
        hdisplay->Init.timing_param.hpixels  = LCD_TFT128QM071_PIXELS_H;
        hdisplay->Init.timing_param.hsyncpw  = 0x0F;
        hdisplay->Init.timing_param.hfnprch  = 0x0F;
        hdisplay->Init.timing_param.hbkprch  = 0x0F;
        hdisplay->Init.timing_param.vlines   = LCD_TFT128QM071_PIXELS_V;
        hdisplay->Init.timing_param.vsyncpw  = 0x0F;
        hdisplay->Init.timing_param.vfnprch  = 0x01;
        hdisplay->Init.timing_param.vbkprch  = 0x01;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = LCD_TFT128QM071_PIXELS_V-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = LCD_TFT128QM071_PIXELS_H-1;
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = LCD_TFT128QM071_INPUT_COLOR; //rgb565
        hdisplay->Init.output_color_mode = LCD_TFT128QM071_OUTPUT_COLOR; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_SPI_LINE4;// SPI
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_32; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_DISABLE;
        
        hdisplay->Init.master_param.base_addr      = LCD_TFT128QM071_HYPER_BASEADDR; //hyper ram
        hdisplay->Init.master_param.total_byte_cnt = LCD_TFT128QM071_PIXELS_H*LCD_TFT128QM071_PIXELS_V*2;
        hdisplay->Init.master_param.burst_length   = LCD_TFT128QM071_BURST_LENGTH; //longest
        hdisplay->Init.tearing_threshold = 20000;
    }

    HAL_DISPLAY_RegisterCallback(hdisplay, HAL_DISPLAY_MSPINIT_CB_ID, TFT128QM071_HAL_DISPLAY_MspInit);
    HAL_DISPLAY_Init(hdisplay);
	
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_Configuration);
    tft128qm071_init_lcd_regs(hdisplay);
}
void display_tft128qm071_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? config->clk_spi_div : DISPLAY_DIV_2));
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
        hahmi.Init.X_STOP_TILE          = (LCD_TFT128QM071_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_TFT128QM071_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_TFT128QM071_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_TFT128QM071_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_AHMI_FRAME_BUFFER_BASE_ADDE;
        hahmi.Init.FRAME_HEIGHT         = LCD_TFT128QM071_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_TFT128QM071_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_TFT128QM071_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}
void display_tft128qm071_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? config->clk_spi_div : DISPLAY_DIV_2));
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
        hahmi.Init.X_STOP_TILE          = (LCD_TFT128QM071_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_TFT128QM071_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_TFT128QM071_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_TFT128QM071_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
        hahmi.Init.FRAME_HEIGHT         = LCD_TFT128QM071_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_TFT128QM071_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_TFT128QM071_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}

static void tft128qm071_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay)
{
    //------------------------------TFT128QM071 Reset Sequence-------------------------------//
    TFT128QM071_Set();
    mdelay(LCD_TFT128QM071_RESET_DELAY_MS);
    TFT128QM071_Reset();
    mdelay(LCD_TFT128QM071_RESET_DELAY_MS);
    TFT128QM071_Set();
    mdelay(LCD_TFT128QM071_SETUP_DELAY_MS); //100ms
    //----------------------------DISPLAY Control setting--------------------------------------//
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xFE);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xEF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xEB);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x14);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x84);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x40);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x85);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xF1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x86);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x98);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x87);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x28);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x88);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0A);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x89);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x21);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8A);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8B);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8D);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xDF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x8F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x52);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xB6);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x36);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x48);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x3A);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x90);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xBD);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA6);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x74);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xBF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA7);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x45);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xA9);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xBB);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xB8);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x63);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xBC);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xFF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x60);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC3);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17); //bit[6:0] VREG1A   35
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC4);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17); //bit[6:0] VREG2A   01
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC9);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x25); //bit[5:0] 
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xBE);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11); //new
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xE1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xDF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x21);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
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
    //------------VGH VGL
    //---------    1.VGH VGL_BT
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xED);			
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1B); //bit7 VGH �Ҧ�0/1
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0B); //bit7 VGL �Ҧ�0/1
    //-----------  2.VGH VGL_AD
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xAC);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x47); //bit[2:0] VGH/VGL_AD �ƭȼW�j���ȼW�j
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xAE);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x77);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xCB);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xCD);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x63);
    //-----------  3.VGH VGL_CLK
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0E); //bit[4:0] VGH
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0F); //bit[4:0] VGL
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
    //-----------  inversion
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xE8);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x34);	// 04:column 14:1-dot 24:2-dot inversion
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x62);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0D);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x71);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xED);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0F);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x71);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xEF);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x63);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x71);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xF1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x13);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x71);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xF3);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x64);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x28);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x29);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xF1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xF1);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x66);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xCD);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x67);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x45);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x45);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x67);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3C);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x54);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x32);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x98);
    //-----------  frame rate
    /*
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x74);			HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);//40Hz
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x70);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGH
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGL
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x4E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    */
    /*
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x74);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x11);//30Hz
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xC0);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGH
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGL
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x4E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    */
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x74);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);	//60Hz
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGH
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGL
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x4E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    /*
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x74);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x69);	//90Hz
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGH
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); //bit[4:0] VGL
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x4E);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    */
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x35);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    //enable tearing
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xba);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x21);
    
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x11); //Exit Sleep
    mdelay(LCD_TFT128QM071_SLEEP_DELAY_MS);
    HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x29); //display on	
    mdelay(LCD_TFT128QM071_OPEN_SREEN_DELAY_MS);
}

#endif /* DRV_DISPLAY_TFT128QM071_ENABLED */
