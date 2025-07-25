/**
  ******************************************************************************
  * @file    display_gc9307n.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/13
  * @brief   LCD gc9307n driver functions.
  ******************************************************************************
  */
#include "display_gc9307n.h"
#if(defined DRV_DISPLAY_GC9307N_ENABLED && DRV_DISPLAY_GC9307N_ENABLED == 1U)

static void GC9307N_Reset(void);
static void GC9307N_Set(void);
static void gc9307n_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay);

static void GC9307N_HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LCD_GC9307N_RESET_PIN;
	HAL_GPIO_Init(LCD_GC9307N_RESET_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_GC9307N_RESET_GPIO, GPIO_InitStruct.Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_CSN_AF;
    GPIO_InitStruct.Pin = LCD_GC9307N_CSN_PIN;
    HAL_GPIO_Init(LCD_GC9307N_CSN_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DCN_AF;
    GPIO_InitStruct.Pin = LCD_GC9307N_DCN_PIN;
    HAL_GPIO_Init(LCD_GC9307N_DCN_GPIO, &GPIO_InitStruct);

    /* GC9307N SPI */
    if(0 == hdisplay->Init.para_extra)
    {
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_SCK_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_SCK_PIN;
        HAL_GPIO_Init(LCD_GC9307N_SCK_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DQ0_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DQ0_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DQ0_GPIO, &GPIO_InitStruct);
        

        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_TEARING_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_TEARING_PIN;
        HAL_GPIO_Init(LCD_GC9307N_TEARING_GPIO, &GPIO_InitStruct);
    }
    /* GC9307N 8080 */
    else
    {
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_WRN_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_WRN_PIN;
        HAL_GPIO_Init(LCD_GC9307N_WRN_GPIO, &GPIO_InitStruct);

        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_RDN_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_RDN_PIN;
        HAL_GPIO_Init(LCD_GC9307N_RDN_GPIO, &GPIO_InitStruct);
        
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB0_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB0_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB0_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB1_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB1_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB1_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB2_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB2_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB2_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB3_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB3_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB3_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB4_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB4_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB4_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB5_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB5_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB5_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB6_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB6_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB6_GPIO, &GPIO_InitStruct);
        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_GC9307N_DB7_AF;
        GPIO_InitStruct.Pin = LCD_GC9307N_DB7_PIN;
        HAL_GPIO_Init(LCD_GC9307N_DB7_GPIO, &GPIO_InitStruct);
        
    }
    
    __HAL_RCC_DISPLAY_SET_DIV(hdisplay->Init.disp_duty_l, hdisplay->Init.disp_duty_h);
    __HAL_RCC_DISPLAY_QSPI_SET_DIV(hdisplay->Init.disp_qspi_duty_l, hdisplay->Init.disp_qspi_duty_h);
}
static void GC9307N_Reset(void)
{
	HAL_GPIO_WritePin(LCD_GC9307N_RESET_GPIO, LCD_GC9307N_RESET_PIN, GPIO_PIN_RESET);
}
static void GC9307N_Set(void)
{
	HAL_GPIO_WritePin(LCD_GC9307N_RESET_GPIO, LCD_GC9307N_RESET_PIN, GPIO_PIN_SET);
}
#ifdef HAL_AHMI_MODULE_ENABLED
static AHMI_HandleTypeDef hahmi = {.Instance = DRV_AHMI_INSTANCE};
#endif

void display_gc9307n_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        hdisplay->Init.input_color_mode  = config->input_color_mode; //rgb565
        hdisplay->Init.output_color_mode = config->output_color_mode; //rgb565 
        if(0 == config->PARA_EXTRA)
        {
            hdisplay->Init.interface_type    = DISPLAY_InterfaceType_SPI_LINE4;// SPI
        }
        else
        {
            hdisplay->Init.interface_type    = DISPLAY_InterfaceType_8BIT;// 8080-8bit
        }
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_8 ; //lowest clock
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
        hdisplay->Init.master_param.burst_length   = LCD_GC9307N_BURST_LENGTH; //one byte 8 pixels
        
        hdisplay->Init.disp_duty_l = config->disp_duty_l;
        hdisplay->Init.disp_duty_h = config->disp_duty_h;
        hdisplay->Init.disp_qspi_duty_l = config->disp_qspi_duty_l;
        hdisplay->Init.disp_qspi_duty_h = config->disp_qspi_duty_h;
        
        hdisplay->Init.para_extra = config->PARA_EXTRA;
    }
    else
    {
        hdisplay->Init.timing_param.hpixels  = LCD_GC9307N_PIXELS_H;
        hdisplay->Init.timing_param.hsyncpw  = 0x0F;
        hdisplay->Init.timing_param.hfnprch  = 0x0F;
        hdisplay->Init.timing_param.hbkprch  = 0x0F;
        hdisplay->Init.timing_param.vlines   = LCD_GC9307N_PIXELS_V;
        hdisplay->Init.timing_param.vsyncpw  = 0x0F;
        hdisplay->Init.timing_param.vfnprch  = 0x01;
        hdisplay->Init.timing_param.vbkprch  = 0x01;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = LCD_GC9307N_PIXELS_V-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = LCD_GC9307N_PIXELS_H-1;
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = LCD_GC9307N_INPUT_COLOR; //rgb565
        hdisplay->Init.output_color_mode = LCD_GC9307N_OUTPUT_COLOR; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_SPI_LINE4;// SPI
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_32; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_DISABLE;
        
        hdisplay->Init.master_param.base_addr      = LCD_GC9307N_HYPER_BASEADDR; //hyper ram
        hdisplay->Init.master_param.total_byte_cnt = LCD_GC9307N_PIXELS_H*LCD_GC9307N_PIXELS_V*2;
        hdisplay->Init.master_param.burst_length   = LCD_GC9307N_BURST_LENGTH; //one byte 8 pixels

        hdisplay->Init.para_extra = 0;
    }

    HAL_DISPLAY_RegisterCallback(hdisplay, HAL_DISPLAY_MSPINIT_CB_ID, GC9307N_HAL_DISPLAY_MspInit);
    HAL_DISPLAY_Init(hdisplay);
	
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_Configuration);
    gc9307n_init_lcd_regs(hdisplay);
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? hdisplay->Init.clk_spi_div : DISPLAY_DIV_2));
}
void display_gc9307n_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
{
	HAL_DISPLAY_DitheringSet(hdisplay, false);
    HAL_DISPLAY_UpdateSet(hdisplay, false);
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
        hahmi.Init.X_STOP_TILE          = (LCD_GC9307N_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_GC9307N_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_GC9307N_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_GC9307N_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_AHMI_FRAME_BUFFER_BASE_ADDE;
        hahmi.Init.FRAME_HEIGHT         = LCD_GC9307N_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_GC9307N_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_GC9307N_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}
void display_gc9307n_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        hahmi.Init.X_STOP_TILE          = (LCD_GC9307N_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_GC9307N_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_GC9307N_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_GC9307N_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
        hahmi.Init.FRAME_HEIGHT         = LCD_GC9307N_PIXELS_V;
        hahmi.Init.FRAME_WIDTH          = LCD_GC9307N_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_GC9307N_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}

static void gc9307n_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay)
{
    //------------------------------GC9307N Reset Sequence-------------------------------//
	GC9307N_Reset();
	mdelay(LCD_GC9307N_RESET_DELAY_MS);
	GC9307N_Set();
    mdelay(LCD_GC9307N_SETUP_DELAY_MS); //100ms
	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, LCD_GC9307N_DISPLAY_ON_CMD); //Exit Sleep
	mdelay(LCD_GC9307N_SLEEP_DELAY_MS);

    //----------------------------DISPLAY Control setting--------------------------------------//
    if(0 == hdisplay->Init.para_extra)  /* GC9307N SPI Interface */
    {
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb1);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2c);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2d);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb2);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2c);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2d);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb3);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2c);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2d);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2c);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2d);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb4);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc0);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xa2);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x84);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc1);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xc5);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc2);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x04);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xc3);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x8a);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2a);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc4);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x8a);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xee);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xc5);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0e);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x36);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xc8);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe0);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1a);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x18);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x28);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x22);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1b);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x23);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x37);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xe1);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1b);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x33);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2c);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x29);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2e);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x30);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x30);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x39);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x07);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x2a);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x7f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x2b);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x9f);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf6);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x3a);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);
        HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x29);
        
        
    }
    else /* GC9307N 8080 Interface */
    {
        LCD_WR_REG(0xfe);
        LCD_WR_REG(0xef);
        LCD_WR_REG(0x36);	
        LCD_WR_DATA8(0x48);	
        LCD_WR_REG(0x3a);	
        LCD_WR_DATA8(0x05);
            
        LCD_WR_REG(0x21);
                
        LCD_WR_REG(0x86);	
        LCD_WR_DATA8(0x98);
        LCD_WR_REG(0xc9);	
        LCD_WR_DATA8(0x0a);

        LCD_WR_REG(0x89);	
        LCD_WR_DATA8(0x03);
        LCD_WR_REG(0xe8);
        LCD_WR_DATA8(0x12);
        LCD_WR_DATA8(0x00);

        LCD_WR_REG(0xc3);	
        LCD_WR_DATA8(0x45);
        LCD_WR_REG(0xc4);	
        LCD_WR_DATA8(0x20);
            
        LCD_WR_REG(0x8b);	
        LCD_WR_DATA8(0x80);	
        LCD_WR_REG(0xff);
        LCD_WR_DATA8(0x62);

        LCD_WR_REG(0x8d);	
        LCD_WR_DATA8(0x22);
        LCD_WR_REG(0x99);	
        LCD_WR_DATA8(0x3e);
        LCD_WR_REG(0x9d);	
        LCD_WR_DATA8(0x4b);

        LCD_WR_REG(0x8e);	
        LCD_WR_DATA8(0x0f);	

        LCD_WR_REG(0xF0);
        LCD_WR_DATA8(0x80);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x0C);
        LCD_WR_DATA8(0x0C);
        LCD_WR_DATA8(0x08);
        LCD_WR_DATA8(0x3D);

        LCD_WR_REG(0xF1);
        LCD_WR_DATA8(0x55);
        LCD_WR_DATA8(0x9A);
        LCD_WR_DATA8(0xB8);
        LCD_WR_DATA8(0x2E);
        LCD_WR_DATA8(0x32);
        LCD_WR_DATA8(0xCF);

        LCD_WR_REG(0xF2);
        LCD_WR_DATA8(0x40);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x09);
        LCD_WR_DATA8(0x0B);
        LCD_WR_DATA8(0x09);
        LCD_WR_DATA8(0x30);

        LCD_WR_REG(0xF3);
        LCD_WR_DATA8(0x43);
        LCD_WR_DATA8(0xB7);
        LCD_WR_DATA8(0x98);
        LCD_WR_DATA8(0x11);
        LCD_WR_DATA8(0x13);
        LCD_WR_DATA8(0xCF);

        //2017-11-14 11:11:29

        LCD_WR_REG(0x11);
        mdelay(LCD_GC9307N_OPEN_SREEN_DELAY_MS);
        LCD_WR_REG(0x29);
        LCD_WR_REG(0x2c);
        
    }
    
	mdelay(LCD_GC9307N_OPEN_SREEN_DELAY_MS);
	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, LCD_GC9307N_OPEN_SCREEN_CMD); //display on	
}

#endif /* DRV_DISPLAY_GC9307N_ENABLED */
