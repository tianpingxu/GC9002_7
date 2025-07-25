/**
  ******************************************************************************
  * @file    display_mrb3205.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/12
  * @brief   LCD mrb3205 driver functions.
  ******************************************************************************
  */
#include "display_mrb3205.h"
#if(defined DRV_DISPLAY_MRB3205_ENABLED && DRV_DISPLAY_MRB3205_ENABLED == 1U)

static void MRB3205_Reset(void);
static void MRB3205_Set(void);
static void mrb3205_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay);

static void MRB3205_HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LCD_MRB3205_RESET_PIN;
	HAL_GPIO_Init(LCD_MRB3205_RESET_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_MRB3205_RESET_GPIO, GPIO_InitStruct.Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        
    if(480 == hdisplay->Init.timing_param.hpixels && 320 == hdisplay->Init.timing_param.vlines)
    {
    }
    else
    {        
        GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_SCK_AF;
        GPIO_InitStruct.Pin = LCD_MRB3205_SCK_PIN;
        HAL_GPIO_Init(LCD_MRB3205_SCK_GPIO, &GPIO_InitStruct);
    }
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_WRN_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_WRN_PIN;
    HAL_GPIO_Init(LCD_MRB3205_WRN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DCN_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DCN_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DCN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_RDN_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_RDN_PIN;
    HAL_GPIO_Init(LCD_MRB3205_RDN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_CSN_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_CSN_PIN;
    HAL_GPIO_Init(LCD_MRB3205_CSN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB0_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB0_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB0_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB1_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB1_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB1_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB2_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB2_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB2_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB3_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB3_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB3_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB4_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB4_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB4_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB5_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB5_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB5_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB6_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB6_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB6_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_DB7_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_DB7_PIN;
    HAL_GPIO_Init(LCD_MRB3205_DB7_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = (uint32_t)LCD_MRB3205_TEARING_AF;
    GPIO_InitStruct.Pin = LCD_MRB3205_TEARING_PIN;
    HAL_GPIO_Init(LCD_MRB3205_TEARING_GPIO, &GPIO_InitStruct);
    
	__HAL_RCC_DISPLAY_SET_DIV(hdisplay->Init.disp_duty_l, hdisplay->Init.disp_duty_h);
    __HAL_RCC_DISPLAY_QSPI_SET_DIV(hdisplay->Init.disp_qspi_duty_l, hdisplay->Init.disp_qspi_duty_h);
}
static void MRB3205_Reset(void)
{
	HAL_GPIO_WritePin(LCD_MRB3205_RESET_GPIO, LCD_MRB3205_RESET_PIN, GPIO_PIN_RESET);
}
static void MRB3205_Set(void)
{
	HAL_GPIO_WritePin(LCD_MRB3205_RESET_GPIO, LCD_MRB3205_RESET_PIN, GPIO_PIN_SET);
}
#ifdef HAL_AHMI_MODULE_ENABLED
static AHMI_HandleTypeDef hahmi = {.Instance = DRV_AHMI_INSTANCE};
#endif

void display_mrb3205_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_8BIT;// 8080-8bit
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
        hdisplay->Init.master_param.burst_length   = LCD_MRB3205_BURST_LENGTH; //one byte 8 pixels
        
        hdisplay->Init.disp_duty_l = config->disp_duty_l;
        hdisplay->Init.disp_duty_h = config->disp_duty_h;
        hdisplay->Init.disp_qspi_duty_l = config->disp_qspi_duty_l;
        hdisplay->Init.disp_qspi_duty_h = config->disp_qspi_duty_h;
    }
    else
    {
        hdisplay->Init.timing_param.hpixels  = LCD_MRB3205_PIXELS_H;
        hdisplay->Init.timing_param.hsyncpw  = 0x0F;
        hdisplay->Init.timing_param.hfnprch  = 0x0F;
        hdisplay->Init.timing_param.hbkprch  = 0x0F;
        hdisplay->Init.timing_param.vlines   = LCD_MRB3205_PIXELS_V;
        hdisplay->Init.timing_param.vsyncpw  = 0x0F;
        hdisplay->Init.timing_param.vfnprch  = 0x01;
        hdisplay->Init.timing_param.vbkprch  = 0x01;

        hdisplay->Init.update_area.start_row = 0;
        hdisplay->Init.update_area.end_row   = LCD_MRB3205_PIXELS_V-1;
        hdisplay->Init.update_area.start_col = 0;
        hdisplay->Init.update_area.end_col   = LCD_MRB3205_PIXELS_H-1;
        
        hdisplay->Init.invert_oclk_en    = DISPLAY_DISABLE;
        hdisplay->Init.invert_hs         = DISPLAY_DISABLE;
        hdisplay->Init.invert_vs         = DISPLAY_DISABLE;
        hdisplay->Init.update_ram        = DISPLAY_ENABLE;
        hdisplay->Init.fg_enable         = DISPLAY_ENABLE;
        hdisplay->Init.dithering         = DISPLAY_DISABLE;
        hdisplay->Init.input_color_mode  = LCD_MRB3205_INPUT_COLOR; //rgb565
        hdisplay->Init.output_color_mode = LCD_MRB3205_OUTPUT_COLOR; //rgb565 
        hdisplay->Init.interface_type    = DISPLAY_InterfaceType_8BIT;// 8080-8bit
        hdisplay->Init.running_type      = DISPLAY_Mode_Configuration;
        hdisplay->Init.tearing_en        = DISPLAY_DISABLE;
        hdisplay->Init.update_en         = DISPLAY_DISABLE;
        hdisplay->Init.clk_spi_div       = DISPLAY_DIV_32; //lowest clock
        hdisplay->Init.icg_en            = DISPLAY_DISABLE;
        hdisplay->Init.lcd_mv            = DISPLAY_DISABLE;
        
        hdisplay->Init.master_param.base_addr      = LCD_MRB3205_HYPER_BASEADDR; //hyper ram
        hdisplay->Init.master_param.total_byte_cnt = LCD_MRB3205_PIXELS_H*LCD_MRB3205_PIXELS_V*2;
        hdisplay->Init.master_param.burst_length   = LCD_MRB3205_BURST_LENGTH; //one byte 8 pixels
    }

    HAL_DISPLAY_RegisterCallback(hdisplay, HAL_DISPLAY_MSPINIT_CB_ID, MRB3205_HAL_DISPLAY_MspInit);
    HAL_DISPLAY_Init(hdisplay);
	
    HAL_DISPLAY_SwitchMode(hdisplay, DISPLAY_Mode_Configuration);
    mrb3205_init_lcd_regs(hdisplay);
    HAL_DISPLAY_SetDiv(hdisplay, (config != NULL ? config->clk_spi_div : DISPLAY_DIV_2));
}
void display_mrb3205_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        hahmi.Init.X_STOP_TILE          = (LCD_MRB3205_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_MRB3205_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_MRB3205_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_MRB3205_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_AHMI_FRAME_BUFFER_BASE_ADDE;
        hahmi.Init.FRAME_HEIGHT         = LCD_MRB3205_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_MRB3205_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_MRB3205_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}
void display_mrb3205_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config)
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
        hahmi.Init.X_STOP_TILE          = (LCD_MRB3205_PIXELS_H/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_MRB3205_PIXELS_H%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_WIDTH/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.Y_START_TILE         = 0;
        hahmi.Init.Y_STOP_TILE          = (LCD_MRB3205_PIXELS_V/DRV_AHMI_TILE_BUFFER_SIZE - ((LCD_MRB3205_PIXELS_V%DRV_AHMI_TILE_BUFFER_SIZE) > 0 ? 0 : 1));//FRAME_HEIGHT/DRV_AHMI_TILE_BUFFER_SIZE-1
        hahmi.Init.MAX_SIZE_OF_SB_BUFFR = 280;
        hahmi.Init.SB_BASE_ADDR	        = DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        hahmi.Init.FB_BASE_ADDR         = DRV_DISPLAY_AHB_SLAVE_BASE_ADDR;
        hahmi.Init.FRAME_HEIGHT         = LCD_MRB3205_PIXELS_V; 
        hahmi.Init.FRAME_WIDTH          = LCD_MRB3205_PIXELS_H;
        hahmi.Init.OUTPUT_COLOR_BITS    = LCD_MRB3205_AHMI_OUTPUT_COLOR;
        hahmi.Init.YR_W                 = 85;
        hahmi.Init.YG_W                 = 85;
        hahmi.Init.YB_W                 = 85;
        hahmi.Init.M_DRAW_CALL_CYCLE    = 0x00000100;
        hahmi.Init.M_CACHE_REQ_CYCLE    = 0x00000200;
    }
    HAL_AHMI_Init(&hahmi);
#endif
}


static void mrb3205_init_lcd_regs(DISPLAY_HandleTypeDef *hdisplay)
{
    uint8_t u8Cmd3A;
    //------------------------------MRB3205 Reset Sequence-------------------------------//
	MRB3205_Reset();
	mdelay(LCD_MRB3205_RESET_DELAY_MS);
	MRB3205_Set();
    mdelay(LCD_MRB3205_SETUP_DELAY_MS); //100ms
	
	//--------------------------------ST7789SFrameratesetting----------------------------------//
    if(DISPLAY_InputColorMode_RGBA8888 == hdisplay->Init.input_color_mode)
    {
        u8Cmd3A = 0x66;
    }
    else
    {   
        u8Cmd3A = 0x55;
    }
    /* 3.5 inch */
    if(480 == hdisplay->Init.timing_param.hpixels && 320 == hdisplay->Init.timing_param.vlines)
    {
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x36);  // Memory Access Control 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xe8); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x3A);   
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, u8Cmd3A); 

    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xc3); 

    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x96); 

    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb4);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01); 

    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb7);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xc6); 

    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xb9);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xe0); //C1 

    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC0);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x80); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x75); //C1 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC1);  //Power control 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x13); //SAP[2:0];BT[3:0] //0x01
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC2);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xa7); 

    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC5);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x27);  
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xE8);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x40); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x8a); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x29); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x19); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xa5); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x33); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xE0);  //Set Gamma
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xd0);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0d);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2b);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x35);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x44);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x47);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x12);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x17);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1b);
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0XE1);  //Set Gamma
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xd0);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x05);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0B);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0b);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0c);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x36);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x54);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x47);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3e);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1b);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1a);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1c);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20);

    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x35); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);

    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3c);
     
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xf0); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x69);
    }
    else
    {
        //----------------------------DISPLAY Control setting--------------------------------------//
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xCF);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xD9); //C1 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0X30); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xED);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x64); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0X12); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0X81); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xE8);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x85); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x10); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x7A); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xCB);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x39); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2C); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x34); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xF7);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x20); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xEA);  
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC0);  //Power control 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1B); //VRH[5:0] 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC1);  //Power control 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x12); //SAP[2:0];BT[3:0] //0x01
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC5);  //VCM control 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26); //3F
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x26); //3C
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xC7);  //VCM control2 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0XB0); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x36);  // Memory Access Control 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x3A);   
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x55); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xB1);   
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);   
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1A); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xB6);  // DISPLAY Function Control 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0A); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xA2); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xF2);  // 3Gamma Function Disable 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x26);  //Gamma curve selected 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01); 
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0xE0);  //Set Gamma
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1F);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x24);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x24);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0D);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x12);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x52);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xB7);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3F);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0C);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x15);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0E);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x08);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0XE1);  //Set Gamma
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1B);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1B);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x02);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0E);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x06);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x2E);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x48);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3F);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x03);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x0A);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x09);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x31);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x37);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x1F);
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x2B); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x01);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x3f);
    	
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x2A); 
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0x00);
    	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, 0xef);	 
    }
	// mdelay(LCD_MRB3205_SLEEP_DELAY_MS);
	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x11); //Exit Sleep
	mdelay(LCD_MRB3205_OPEN_SREEN_DELAY_MS);
	HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, 0x29); //display on	
}

#endif /* DRV_DISPLAY_MRB3205_ENABLED */
