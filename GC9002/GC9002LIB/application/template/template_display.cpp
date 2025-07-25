/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-25     huyt        the first version
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"
#include "csi_irq_vectors.h"
#include "display.h"
#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_DISPLAY_


void TEST01(void)
{
    DISPLAY_MasterParamType *param_ptr;
    DISPLAY_MasterParamType param;
    while (1)
    {
        uint16_t tempreg = HAL_UART_ReceiveData(CONFIG_CONSOLE_DEBUG_UART);
        switch(tempreg)
        {
            case 0x01://pop
            if(Display::DisplayPop(&param_ptr))
            {
                printf("pop 0x%x success\r\n", param_ptr->base_addr);
            }
            else
            {
                printf("pop failed\r\n");
            }
            break;
            case 0x02://push 0x02
            case 0x03://push 0x03
            case 0x04://push 0x04
            param.base_addr = tempreg;
            if(Display::DisplayPush(&param))
            {
                printf("push 0x%x success\r\n", tempreg);
            }
            else
            {
                printf("push 0x%x faileed\r\n", tempreg);
            }
            break;
            case 0x05://Get State 0x02
            param.base_addr = 0x02;
            if(Display::GetDisplayDone(&param))
            {
                printf("Display 0x02 Done\r\n");
            }
            else
            {
                printf("Display 0x02 Wait\r\n");
            }
            break;
            case 0x06://Get State 0x03
            param.base_addr = 0x03;
            if(Display::GetDisplayDone(&param))
            {
                printf("Display 0x03 Done\r\n");
            }
            else
            {
                printf("Display 0x03 Wait\r\n");
            }
            break;
            case 0x07://Get State 0x04
            param.base_addr = 0x04;
            if(Display::GetDisplayDone(&param))
            {
                printf("Display 0x04 Done\r\n");
            }
            else
            {
                printf("Display 0x04 Wait\r\n");
            }
            break;
            default: break;
        }
    }
}
void TEST02(void)
{
    struct display_conf config;
    config.lcd_type = LCD_MRB3205;
    config.run_mode = DISPLAY_Mode_SlavePassive;
    config.clk_spi_div = DISPLAY_DIV_2;
    config.disp_qspi_duty_l = 4;
    config.disp_qspi_duty_h = 4;
    config.disp_duty_l = 4;
    config.disp_duty_h = 4;
    config.input_color_mode = DISPLAY_InputColorMode_RGB565;
    config.output_color_mode = DISPLAY_OutputColorMode_RGB565;
    config.frame_buffer_addr = 0x0200C000;
    config.HPIXEL_VALUE = 240;
    config.VLINES_VALUE = 320;
    config.HSYNCPW_VALUE = 0x0F;
    config.VSYNCPW_VALUE = 0x0F;
    config.HFNPRCH_VALUE = 0x0F;
    config.VFNPRCH_VALUE = 0x01;
    config.HBKPRCH_VALUE = 0x0F;
    config.VBKPRCH_VALUE = 0x01;

    Display::Init(&config);
}
void TEST03(void)
{
    unsigned char mul[12];
    mul[0] = multip_fixed_signed(0xC0, 0xC0, 6);
    mul[1] = multip_fixed_signed(0xC8, 0xC8, 6);
    mul[2] = multip_fixed_signed(0x70, 0x70, 6);
    mul[3] = multip_fixed_signed(0x78, 0x78, 6);
    mul[4] = multip_fixed_signed(0x40, 0x40, 6);
    mul[5] = multip_fixed_signed(0x48, 0x48, 6);
    mul[6] = multip_fixed_signed(0x48, 0x44, 6);
    mul[7] = multip_fixed_signed(0x48, 0x70, 6);
    mul[8] = multip_fixed_signed(0x48, 0xC0, 6);
}
void TEST04(void)
{
    unsigned char mul[12];
    mul[0] = multip_fixed_unsigned(0xC0, 0xC0, 7);//超出unsigned char范围
    mul[1] = multip_fixed_unsigned(0xC8, 0xC8, 7);
    mul[2] = multip_fixed_unsigned(0x70, 0x70, 7);
    mul[3] = multip_fixed_unsigned(0x78, 0x78, 7);
    mul[4] = multip_fixed_unsigned(0x40, 0x40, 7);
    mul[5] = multip_fixed_unsigned(0x48, 0x48, 7);
    mul[6] = multip_fixed_unsigned(0x48, 0x44, 7);
    mul[7] = multip_fixed_unsigned(0x48, 0x70, 7);
    mul[8] = multip_fixed_unsigned(0x48, 0xC0, 7);
}
void TEST05(void)
{
    mat4_t a, b, r;
    a.ccm.Mr0 = 1;
    a.ccm.Mr1 = 0;
    a.ccm.Mr2 = 0;
    a.ccm.Mr3 = 0;

    a.ccm.Mg0 = 0;
    a.ccm.Mg1 = 1;
    a.ccm.Mg2 = 0;
    a.ccm.Mg3 = 0;
    
    a.ccm.Mb0 = 0;
    a.ccm.Mb1 = 0;
    a.ccm.Mb2 = 1;
    a.ccm.Mb3 = 0;
    a.t[0] = 0;
    a.t[1] = 0;
    a.t[2] = 0;
    a.t[3] = 1;
    
    b.ccm.Mr0 = 1;
    b.ccm.Mr1 = 0;
    b.ccm.Mr2 = 0;
    b.ccm.Mr3 = 1;

    b.ccm.Mg0 = 0;
    b.ccm.Mg1 = 1;
    b.ccm.Mg2 = 0;
    b.ccm.Mg3 = 1;
    
    b.ccm.Mb0 = 0;
    b.ccm.Mb1 = 0;
    b.ccm.Mb2 = 1;
    b.ccm.Mb3 = 1;
    b.t[0] = 0;
    b.t[1] = 0;
    b.t[2] = 0;
    b.t[3] = 1;
    // matrix_multiply(&b, &a, &r);
}
// Display display;
void TEST06(void)
{
    struct display_conf config;
    config.lcd_type = LCD_TEST_SOFT;
    config.run_mode = DISPLAY_Mode_SlavePassive;
    config.clk_spi_div = DISPLAY_DIV_2;
    config.disp_qspi_duty_l = 4;
    config.disp_qspi_duty_h = 4;
    config.disp_duty_l = 4;
    config.disp_duty_h = 4;
    config.input_color_mode = DISPLAY_InputColorMode_RGB565;
    config.output_color_mode = DISPLAY_OutputColorMode_RGB565;
    config.frame_buffer_addr = 0x0200C000;
    config.HPIXEL_VALUE = 240;
    config.VLINES_VALUE = 320;
    config.HSYNCPW_VALUE = 0x0F;
    config.VSYNCPW_VALUE = 0x0F;
    config.HFNPRCH_VALUE = 0x0F;
    config.VFNPRCH_VALUE = 0x01;
    config.HBKPRCH_VALUE = 0x0F;
    config.VBKPRCH_VALUE = 0x01;

    Display::Init(&config);

    Display::SetBright(50);
    Display::SetContrast(50);
    Display::SetHueSaturation(50, 0);
    Display::GammaCorrection(0x80008000);//γ = -0.5
    while(1)//This is AHMI core irq handler draw done callback
    {
        DISPLAY_MasterParamType param;
        param.base_addr = 0x02008000;
        param.total_byte_cnt = 320*320;
        param.burst_length = 4;
        //查询地址为base_addr的FrameBuffer是否发送完成，如果完成推到display的FIFO中去
        if(Display::GetDisplayDone(&param))
        {
            Display::DisplayPush(&param);
        }
    }
    while(1)//This is Display irq handler vs_int callback
    {
        DISPLAY_MasterParamType *param_ptr;
        if(Display::DisplayPop(&param_ptr))
        {
        }
    }
}
int main(void)
{
    // TEST01();
    // TEST02();
    // TEST03();
    // TEST04();
    // TEST05();
    TEST06();
    while (1)
    {
		HAL_Delay(100);
    }
    return 0;
}

#endif/* _TEMPLATE_DISPLAY_ */
#endif/* BSP_DEBUG */