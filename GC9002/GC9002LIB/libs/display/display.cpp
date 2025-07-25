#include "display.h"
#ifdef CONFIG_DRV_DISPLAY_ENABLED

#define DISPLAY_DEBUG   1
uint32_t Display::m_bright;
uint32_t Display::m_contrast;
//uint32_t Display::m_saturation;
//int64_t Display::m_hue_angle;
struct DisplayStreamCtrl Display::m_field;
DISPLAY_HandleTypeDef Display::m_hdisplay;
//mat4_t Display::m_mat4_bright;
//mat4_t Display::m_mat4_contrast;
mat4_t Display::m_mat4_ccm;
//mat2_t Display::m_mat2_yct;

/**
  * @brief  定点4x4矩阵相乘算法
  * @note   矩阵参数需要是无符号位
  * @retval none
  */
void Display::matrix_multiply_fixed_unsigned(const mat4_t *mat_a, const mat4_t *mat_b, mat4_t *mat_r, unsigned char q)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        for (unsigned int j = 0; j < 4; ++j)
        {
            for (unsigned int k = 0; k < 4; ++k)
            {
                mat_r->m[i][j] += multip_fixed_unsigned(mat_a->m[i][k], mat_b->m[k][j], q);
            }
        }
    }
}
void Display::set_mat4_bright(void)
{
    uint8_t div_fix;
    uint8_t val_fix;
    div_fix = (uint8_t)Divfix(itofix(Display::m_bright, 16), itofix(DISPLAY_BRIGHT_MAX, 16), 7);
    val_fix = (uint8_t)Mulfix(div_fix, 0xFF, 7);
    Display::m_mat4_ccm.ccm.Mr0 = val_fix;
    Display::m_mat4_ccm.ccm.Mr1 = 0;
    Display::m_mat4_ccm.ccm.Mr2 = 0;
    Display::m_mat4_ccm.ccm.Mr3 = 0;

    Display::m_mat4_ccm.ccm.Mg0 = 0;
    Display::m_mat4_ccm.ccm.Mg1 = val_fix;
    Display::m_mat4_ccm.ccm.Mg2 = 0;
    Display::m_mat4_ccm.ccm.Mg3 = 0;

    Display::m_mat4_ccm.ccm.Mb0 = 0;
    Display::m_mat4_ccm.ccm.Mb1 = 0;
    Display::m_mat4_ccm.ccm.Mb2 = val_fix;
    Display::m_mat4_ccm.ccm.Mb3 = 0;
}

void Display::set_mat4_contrast(void)
{
    uint8_t div_fix;
    uint8_t val_fix;
    div_fix = (uint8_t)Divfix(itofix(Display::m_contrast, 16), itofix(DISPLAY_CONTRAST_MAX, 16), 7);
    val_fix = (uint8_t)Mulfix(div_fix, 0xFF, 7);
	
    Display::m_mat4_ccm.ccm.Mr3 = DISPLAY_COLOR_MEDIAN - val_fix;//此时的val_fix作为0.8.0的定点数
	
    Display::m_mat4_ccm.ccm.Mg3 = DISPLAY_COLOR_MEDIAN - val_fix;
	
    Display::m_mat4_ccm.ccm.Mb3 = DISPLAY_COLOR_MEDIAN - val_fix;
}

//#include <csi_triangle.h>
//void Display::set_mat2_hue_saturation(void)
//{
//    uint8_t div_fix;
//    uint8_t val_fix;
//    uint8_t val_u8_cos_fix;
//    uint8_t val_u8_sin_fix;
//    int cos_hue_fix;
//    int sin_hue_fix;
//    div_fix = (uint8_t)Divfix(itofix(Display::m_saturation, 16), itofix(DISPLAY_SATURATION_MAX, 16), 6);
//    if(div_fix >= 0x40)
//    {
//        val_fix = (uint8_t)Mulfix(div_fix, 0x7F, 6);
//    }
//    else
//    {
//        div_fix = 0x40 - div_fix;
//        val_fix = (uint8_t)Mulfix(div_fix, 0x7F, 6);
//        val_fix |= 0x80;
//    }
//    cos_hue_fix = csi_cos_fixed(Display::m_hue_angle);//1.15.16
//    sin_hue_fix = csi_sin_fixed(Display::m_hue_angle);//1.15.16
//    val_u8_cos_fix = (uint8_t)(cos_hue_fix >> 10);//1.1.6
//    val_u8_sin_fix = (uint8_t)(sin_hue_fix >> 10);//1.1.6
//    Display::m_mat2_yct.yct.Mco0 = multip_fixed_signed(val_fix, val_u8_cos_fix, 6);
//    Display::m_mat2_yct.yct.Mco1 = multip_fixed_signed(val_fix, val_u8_sin_fix, 6);
//    Display::m_mat2_yct.yct.Mcg0 = multip_fixed_signed(val_fix, val_u8_sin_fix, 6) + 0x80;
//    Display::m_mat2_yct.yct.Mcg1 = multip_fixed_signed(val_fix, val_u8_cos_fix, 6);
//}

bool Display::SetBright(uint32_t percentage)
{
    if(percentage > DISPLAY_BRIGHT_MAX)
    {
        return false;
    }
    Display::m_bright = percentage;
    Display::set_mat4_bright();
    Display::set_mat4_contrast();
    //matrix_multiply_fixed_unsigned(&Display::m_mat4_bright, &Display::m_mat4_contrast, &Display::m_mat4_ccm, 7);
    HAL_DISPLAY_SetCCM(&Display::m_hdisplay, &Display::m_mat4_ccm.ccm);
    return true;
}
bool Display::SetContrast(uint32_t percentage)
{
    if(percentage > DISPLAY_CONTRAST_MAX)
    {
        return false;
    }
    Display::m_contrast = percentage;
    Display::set_mat4_bright();
    Display::set_mat4_contrast();
    //matrix_multiply_fixed_unsigned(&Display::m_mat4_bright, &Display::m_mat4_contrast, &Display::m_mat4_ccm, 7);
    HAL_DISPLAY_SetCCM(&Display::m_hdisplay, &Display::m_mat4_ccm.ccm);
    return true;
}
bool Display::SetHueSaturation(uint32_t percentage, int64_t angle)
{
	return true;//hardware bug, no used !!!
//	if(percentage > DISPLAY_SATURATION_MAX)
//    {
//        return false;
//    }
//    Display::m_hue_angle = angle;
//    Display::m_saturation = percentage;
//    Display::set_mat2_hue_saturation();
//    HAL_DISPLAY_SetYCT(&Display::m_hdisplay, &Display::m_mat2_yct.yct);
//    return true;
}

bool Display::Init(struct display_conf *config)
{
    if(config == NULL)
    {
        return false;
    }
    Display();
    switch (config->lcd_type)
    {
    #if(defined DRV_DISPLAY_RGB565_ENABLED && DRV_DISPLAY_RGB565_ENABLED == 1U)
	case LCD_RGB565:{
        display_rgb565_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_rgb565_master_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_888_ENABLED && DRV_DISPLAY_888_ENABLED == 1U)
    case LCD_888:{
        display_888_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_888_master_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_MRB3205_ENABLED && DRV_DISPLAY_MRB3205_ENABLED == 1U)
    case LCD_MRB3205:{
        display_mrb3205_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_mrb3205_master_passive(&Display::m_hdisplay, config);
        }
        else if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_mrb3205_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_SJXD_ENABLED && DRV_DISPLAY_SJXD_ENABLED == 1U)
    case LCD_SJXD:{
        display_sjxd_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_sjxd_master_passive(&Display::m_hdisplay, config);
        }
        else if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_sjxd_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_ST7512_ENABLED && DRV_DISPLAY_ST7512_ENABLED == 1U)
    case LCD_ST7512:{
        display_st7512_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_st7512_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_ST7735S_ENABLED && DRV_DISPLAY_ST7735S_ENABLED == 1U)
    case LCD_ST7735S:{
        display_st7735s_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_st7735s_master_passive(&Display::m_hdisplay, config);
        }
        else if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_st7735s_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_ST77903_ENABLED && DRV_DISPLAY_ST77903_ENABLED == 1U)
    case LCD_ST77903:{
        display_st77903_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterActive)
        {
            display_st77903_master_active(&Display::m_hdisplay, config);
        }
        else if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_st77903_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_LEIKE_ENABLED && DRV_DISPLAY_LEIKE_ENABLED == 1U)
    case LCD_LEIKE:{
        display_leike_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_leike_master_passive(&Display::m_hdisplay, config);
        }
        else if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_leike_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_TFT128QM071_ENABLED && DRV_DISPLAY_TFT128QM071_ENABLED == 1U)
    case LCD_TFT128QM071:{
        display_tft128qm071_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_tft128qm071_master_passive(&Display::m_hdisplay, config);
        }
        else if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_tft128qm071_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    #if(defined DRV_DISPLAY_GC9307N_ENABLED && DRV_DISPLAY_GC9307N_ENABLED == 1U)
    case LCD_GC9307N:{
        display_gc9307n_init(&Display::m_hdisplay, config);
        if(config->run_mode == DISPLAY_Mode_MasterPassive)
        {
            display_gc9307n_master_passive(&Display::m_hdisplay, config);
        }
        else if(config->run_mode == DISPLAY_Mode_SlavePassive)
        {
            display_gc9307n_slave_passive(&Display::m_hdisplay, config);
        }
        else
        {
            return false;
        }
    }break;
    #endif
    default:
        break;
    }
    Display::SetBright(Display::m_bright);
    if(Display::m_hdisplay.Init.running_type == DISPLAY_Mode_MasterActive ||
       Display::m_hdisplay.Init.running_type == DISPLAY_Mode_MasterPassive)
    {
        HAL_DISPLAY_RegisterCallback(&Display::m_hdisplay, HAL_DISPLAY_VS_INT_CB_ID, Display::VSIntCallback);
        __HAL_DISPLAY_IT_ENABLE(&Display::m_hdisplay, DISPLAY_IT_vs_int);
    }
    return true;
}
void Display::VSIntCallback(DISPLAY_HandleTypeDef *hdisplay)
{
    DISPLAY_MasterParamType *param_ptr;
    //
    if(Display::DisplayPop(&param_ptr))
    {
        HAL_DISPLAY_SetFrameBuffer(&Display::m_hdisplay, param_ptr->base_addr, param_ptr->burst_length, param_ptr->total_byte_cnt);
    }
}
/**
  * @brief  查询FIFO是否空
  * @retval true-FIFO空，false-FIFO未空
  */
bool Display::IsEmpty(void)
{
    return (Display::m_field.m_PushIdx == Display::m_field.m_PopIdx);
}
/**
  * @brief  查询FIFO是否满
  * @retval true-FIFO满，false-FIFO未满
  */
bool Display::IsFull(void)
{
    return (((Display::m_field.m_PushIdx + 1U) % DISPLAY_BUFFER_SIZE) == Display::m_field.m_PopIdx);
}
bool Display::DisplayPush(DISPLAY_MasterParamType *para_ptr)
{
#ifdef DISPLAY_DEBUG
    printf("m_PushIdx:%d, m_PopIdx:%d\n", Display::m_field.m_PushIdx, Display::m_field.m_PopIdx);
#endif // DISPLAY_DEBUG
    if(!IsFull() && para_ptr != NULL)
    {
        uint8_t index = Display::m_field.m_PushIdx;
        Display::m_field.framebuffer[index].base_addr = para_ptr->base_addr;
        Display::m_field.framebuffer[index].total_byte_cnt = para_ptr->total_byte_cnt;
        Display::m_field.framebuffer[index].burst_length = para_ptr->burst_length;
        Display::m_field.state[index] = DisplayWait;
        Display::m_field.m_PushIdx = (Display::m_field.m_PushIdx + 1U) % DISPLAY_BUFFER_SIZE;
        return true;
    }
    return false;
}
bool Display::DisplayPop(DISPLAY_MasterParamType **p_para_ptr)
{
#ifdef DISPLAY_DEBUG
    printf("m_PushIdx:%d, m_PopIdx:%d\n", Display::m_field.m_PushIdx, Display::m_field.m_PopIdx);
#endif // DISPLAY_DEBUG
    if(!IsEmpty() && p_para_ptr != NULL)
    {
        uint8_t index = Display::m_field.m_PopIdx;
        *p_para_ptr = &(Display::m_field.framebuffer[index]);
        index = (Display::m_field.m_PopIdx + DISPLAY_BUFFER_SIZE - 1) % DISPLAY_BUFFER_SIZE;
        Display::m_field.state[index] = DisplayDone;
        Display::m_field.m_PopIdx = (Display::m_field.m_PopIdx + 1U) % DISPLAY_BUFFER_SIZE;
        return true;
    }
    else
    {
        uint8_t index = (Display::m_field.m_PopIdx + DISPLAY_BUFFER_SIZE - 1) % DISPLAY_BUFFER_SIZE;
        Display::m_field.state[index] = DisplayDone;
    }
    return false;
}
bool Display::GetDisplayDone(DISPLAY_MasterParamType *p_para)
{
    if(p_para != NULL)
    {
        for(int i = 0; i < DISPLAY_BUFFER_SIZE; i++)
        {
            if(Display::m_field.state[i] == DisplayWait)
            {
                if(Display::m_field.framebuffer[i].base_addr == p_para->base_addr)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
#if(defined CONFIG_CSI_FLOAT_SOFT_ENABLE && CONFIG_CSI_FLOAT_SOFT_ENABLE == 1U)
#include "math.h"
#endif
bool Display::GammaCorrection(int gamma)
{
#if(defined CONFIG_CSI_FLOAT_SOFT_ENABLE && CONFIG_CSI_FLOAT_SOFT_ENABLE == 1U)
    float gamma_float = int_to_float(gamma, 16);
    float y_out = 0.0;
    DISPLAY_Y_Table y_table;
    for(int i = 0; i < 64; i++)
    {
        y_out = powf((float)(i*4 + 0)/255.0, gamma_float)*255.0;
        y_table.bit.Y0 = (uint8_t)y_out;
        y_out = powf((float)(i*4 + 1)/255.0, gamma_float)*255.0;
        y_table.bit.Y1 = (uint8_t)y_out;
        y_out = powf((float)(i*4 + 2)/255.0, gamma_float)*255.0;
        y_table.bit.Y2 = (uint8_t)y_out;
        y_out = powf((float)(i*4 + 3)/255.0, gamma_float)*255.0;
        y_table.bit.Y3 = (uint8_t)y_out;
        Display::m_hdisplay.Instance->Y_Table[i].reg_u32val = y_table.reg_u32val;
    }
#else
    return false;
#endif
    return true;
}

#endif /* CONFIG_DRV_DISPLAY_ENABLED */