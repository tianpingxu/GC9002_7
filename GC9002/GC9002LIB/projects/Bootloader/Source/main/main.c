#include "main.h"
#include "ParsePara.h"
#include "gc90x_display.h"
#include "DrawProgramBar.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "upgrade_c1.h"
#include "DiagMgr.h"
#include "cfg_c1.h"
#include "display_conf.h"
#include "gc90x_ahmi.h"
#include "lcd_init.h"

extern uint32_t SCREEN_WIDTH;
extern uint32_t SCREEN_HEIGHT;

//csi_spiflash_t spiflash;
void HardwareInit(void)
{
    bool bFramebufState;
    
	/* Pre-Initialize LCD, if needed(like SPI LCD) */
	lcd_init();
	
	/* Default value.*/
	uint32_t para[DisplayParaNumber] = {0,   //polarity
	#ifndef CHIP_GC9002_NoHyper
                                        800, 480, 20, 10, 210, 22, 46, 23, 8, LCD_RGB565, DISPLAY_Mode_MasterActive, 0,  //diaplay
    #else                                             
										//132, 132, 15, 15, 15, 1, 15, 1, 16, LCD_ST7735S, DISPLAY_Mode_SlavePassive, 0,  //diaplay
										//240, 320, 15, 15, 15, 1, 15, 1, 16, LCD_MRB3205, DISPLAY_Mode_SlavePassive, 0,  //diaplay
										//400, 400, 150, 8, 100, 6, 1, 6, 48, LCD_ST77903, DISPLAY_Mode_MasterActive, 0,  //diaplay
										/* If no Hyper, use 8080 Interface for default config */
										//128, 160, 15, 15, 15, 1, 15, 1, 48, LCD_ST7735S, DISPLAY_Mode_SlavePassive, 0,  //diaplay
										480, 320, 15, 15, 15, 1, 15, 1, 48, LCD_MRB3205, DISPLAY_Mode_SlavePassive, 0,
    #endif
                                        0,  //ResolutionType
                                        1,  //para_extra
                                        0, 5000, 0, 100, 50, 6000,  // BackLight
                                        0, 4000, 300, 3000, 10,     //Buzzer
                                        0, 0, 0, 0,         //TouchScreen
                                        0,                  //System option
                                        100, 100, 100,      //display contrast, brightness, saturation
                                        1000                //ADC delay time
                                        };
										
	
    if(CFG_Get_sys_if_use_pro_bin())
    {   /* Get dislay parameter from product file. */
        UpgradeProductPara(BANK_WRITE_START_ADDR_PRODUCT_PARA, para);
        UpgradeProductPara(BANK_WRITE_START_ADDR_HWCONFIG, para);
    }
    SCREEN_WIDTH = para[1];
    SCREEN_HEIGHT = para[2];

    /* Only DISPLAY_Mode_MasterActive and DISPLAY_Mode_MasterPassive mode has framebuffer  */
    if(DISPLAY_Mode_MasterPassive > para[11])
    {
        bFramebufState = TRUE;
    }
    else
    {
        bFramebufState = FALSE;
    }
    DRAW_vSetFramebufState(bFramebufState);
    
    if(CFG_Get_sys_if_use_display())
    {
        InitDisplayByBin(para[0], para[1], para[2], para[3], para[4], para[5], para[6], para[7], para[8], para[9], para[10], para[11], FRAME_BASE_ADDR, para[14]);
    	
    	AHMI_Config();
    	DRAW_vInit();
    }
}

void SoftwareInit(void)
{
	UPG_Main_vInit();
}


int main(void) 
{
    /* CFG Initialization */
    CFG_vInit();
	/* Initial Hardware for System. */
	HardwareInit();
	
	/* Initial Software for System. */
	SoftwareInit();
	
	/* Start scheduler tasks for System. */
    vTaskStartScheduler(); 
	while (1);
}
