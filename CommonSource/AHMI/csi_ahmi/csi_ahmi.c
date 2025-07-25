#include "csi_ahmi.h"
#include "csi_ahmi_hw.h"
#include "csi_kernel.h"

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "csi_hal_driver_flash.h"
#include "csi_ahmi_bt656.h"
#if defined(CHIP_GC9002)
#include "csi_irq.h"
#include "gc90x_display.h"
#endif
#endif

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#if defined(CHIP_GC9002)
extern void DISPLAY_HandlerISR(void);
extern void AHMI_HandlerISR(void);
#else
extern void DISPLAY_IRQHandler(void);
extern void AHMI_CORE_IRQHandler(void);
#endif
#endif

//extern u8 sourceBuffer[SoureBufferSize];
extern uint32_t SBAddress;
extern uint32_t sourceshift;
extern u8 video_cur_r0 ; 
extern u8 video_next_r0;
extern uint32_t AHMIOutputRotation;
//extern k_sem_handle_t flashSemaphore;

extern void csi_hal_internal_flash_take(uint32_t intflag);
extern void csi_hal_internal_flash_give(uint32_t intflag);

void csi_ahmi_ClearSourceShift(void)
{
	sourceshift = 0;
}

u8 initAHMI = 0;
void csi_ahmi_DrawForFivePoint(void)
{
	AHMI_SetFBAdress(FRAME_BUFFER_START_ADDR);
	#if defined(CHIP_GC9002)
    DISPLAY_SetPriAddress(FRAME_BUFFER_START_ADDR);
    #elif defined(CHIP_GC9005)
    DISPLAY_SetChannel0Address(FRAME_BUFFER_START_ADDR);
    #endif
    AHMI_SetSBAdress(SBAddress);
	
	if(initAHMI)
	{
		
	}
    else
    {
		#if defined(CHIP_GC9002)
		csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_HandlerISR);
		csi_hal_irq_enable(DISPLAY_IRQn);
		csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_HandlerISR);
		csi_hal_irq_enable(AHMI_IRQn);
        #elif defined(CHIP_GC9005)
		csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_IRQHandler);
		csi_hal_irq_enable(DISPLAY_IRQn);
		csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_CORE_IRQHandler);
		csi_hal_irq_enable(AHMI_IRQn);
		#else
        NVIC_DisableIRQ(DISPLAY_IRQn);	
        NVIC_ClearPendingIRQ(DISPLAY_IRQn);
        NVIC_SetPriority(DISPLAY_IRQn, 0);
        NVIC_EnableIRQ(DISPLAY_IRQn);
		NVIC_DisableIRQ(AHMI_IRQn);	
        NVIC_ClearPendingIRQ(AHMI_IRQn);
        NVIC_SetPriority(AHMI_IRQn, 0);
        NVIC_EnableIRQ(AHMI_IRQn);
        #endif
		
        AHMI_Config();
        AHMI_SetRot(AHMIOutputRotation);
    }
    
	initAHMI = 1;	
		
	//change the buffer of video
	video_cur_r0 = video_next_r0;

    //csi_hal_disable_irq();				
    csi_hal_DCache_Flush();
    //csi_hal_enable_irq();
    
    GPE_Cmd(ENABLE);
  
    csi_ahmi_ClearSourceShift();
}

u8 ahmi_first = 0;
u8 ahmi_init_int = 0;
u8 ahmi_init1 = 0;
u8 addr_w = 0;          //the write pointer for AHMI to write. 3 buffer to choose from. count be 0,1,2
u8 addr_r = 0;          //the read pointer for DISPLAY to read. 3 buffer to choose from. count be 0,1,2
#if defined(CHIP_GC9005)
u8 ahmi_first_sub = 0;
u8 ahmi_init_int_sub = 0;
u8 ahmi_init1_sub = 0;
u8 addr_w_sub = 0;
u8 addr_r_sub = 0;
#endif
void csi_ahmi_SwitchBuffer(void)
{
    #if defined(CHIP_GC9002)
	
	if(GetAHMIParticalDrawType() < AHMI_PARTIAL_DRAW_SINGLE_BUFFER)
	{
		/* Switch frame buffer. */
		if(ahmi_first == 0)
		{
			//addr_combine = 0x102; // r->0, w->2, n->4
//			addr_n = 0; 
			// r->0, w->0
			addr_r = 0;
			addr_w = 0;
			ahmi_init_int = 1;
			ahmi_first = 1;
		}
		else if(ahmi_init_int == 1 && ahmi_init1 == 0)
		{
			// r->0, w->1
			addr_r = 0;
			addr_w = 1;
			ahmi_init1 = 1;
		}
		else 
		{
			if(addr_w == addr_r)
			{
				addr_r = (addr_r + 1) % 2; //r += 2
				addr_w = (addr_w + 2) % 2; //w += 2
			}
			else 
			{
				addr_r = (addr_r + 1) % 2; //r += 1
				addr_w = (addr_w + 1) % 2; //w += 1
			}
			ahmi_init_int = 0;
		}
	}
	else
	{
		addr_w = 0;
		addr_r = 0;
	}

    AHMI_SetFBAdress(StartAddrOfDisplayBuffer + addr_w * (SizeOfDisplayBuffer));
    DISPLAY_SetPriAddress(StartAddrOfDisplayBuffer + addr_r * (SizeOfDisplayBuffer));
    #endif

    #if defined(CHIP_GC9005)
    if(StartAddrOfSubDisplayBuffer == 0)
    {
        DISPLAY_SetChannel0Address(AHMI_GetFBAdress());
    }
    else if(AHMI_GetFBAdress() < StartAddrOfSubDisplayBuffer)
    {
        DISPLAY_SetChannel0Address(AHMI_GetFBAdress());
    }
    else
    {
        DISPLAY_SetChannel1Address(AHMI_GetFBAdress());
    }
    #endif
}

#if defined(CHIP_GC9005)
void csi_ahmi_SetFBAdress(uint8_t type)
{
    if(type < 2)//RefreshScreenSingle:0,RefreshScreenMain:1,RefreshScreenSub:2
    {
        /* Switch frame buffer. */
        if(ahmi_first == 0)
        {
            //addr_combine = 0x102; // r->0, w->2, n->4
            //addr_n = 0;
            // r->0, w->0
            addr_r = 0;
            addr_w = 0;
            ahmi_init_int = 1;
            ahmi_first = 1;
        }
        else if(ahmi_init_int == 1 && ahmi_init1 == 0)
        {
            // r->0, w->1
            addr_r = 0;
            addr_w = 1;
            ahmi_init1 = 1;
        }
        else
        {
            if(addr_w == addr_r)
            {
                addr_r = (addr_r + 1) % 2; //r += 2
                addr_w = (addr_w + 2) % 2; //w += 2
            }
            else
            {
                addr_r = (addr_r + 1) % 2; //r += 1
                addr_w = (addr_w + 1) % 2; //w += 1
            }
            ahmi_init_int = 0;
        }

        AHMI_SetFBAdress(StartAddrOfDisplayBuffer + addr_w * (SizeOfDisplayBuffer));
    }
    else
    {
        /* Switch frame buffer. */
        if(ahmi_first_sub == 0)
        {
            //addr_combine = 0x102; // r->0, w->2, n->4
            //addr_n = 0;
            // r->0, w->0
            addr_r_sub = 0;
            addr_w_sub = 0;
            ahmi_init_int_sub = 1;
            ahmi_first_sub = 1;
        }
        else if(ahmi_init_int_sub == 1 && ahmi_init1_sub == 0)
        {
            // r->0, w->1
            addr_r_sub = 0;
            addr_w_sub = 1;
            ahmi_init1_sub = 1;
        }
        else
        {
            if(addr_w_sub == addr_r_sub)
            {
                addr_r_sub = (addr_r_sub + 1) % 2; //r += 2
                addr_w_sub = (addr_w_sub + 2) % 2; //w += 2
            }
            else
            {
                addr_r_sub = (addr_r_sub + 1) % 2; //r += 1
                addr_w_sub = (addr_w_sub + 1) % 2; //w += 1
            }
            ahmi_init_int_sub = 0;
        }

        AHMI_SetFBAdress(StartAddrOfSubDisplayBuffer + addr_w_sub * (SizeOfDisplayBuffer));
    }
}

void csi_ahmi_Draw_ForSBBatch(const FBBatchDraw_Box *pFBBatchDraw_Box, uint8_t boxID)
{
    if(initAHMI)
    {
        initAHMI = 0;
        if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
        {
            AHMI_TileBox(pFBBatchDraw_Box[boxID].startX /TILESIZE, pFBBatchDraw_Box[boxID].startY /TILESIZE,
                     (pFBBatchDraw_Box[boxID].startX + pFBBatchDraw_Box[boxID].width + TILESIZE - 1) /TILESIZE - 1,
                     (pFBBatchDraw_Box[boxID].startY + pFBBatchDraw_Box[boxID].height + TILESIZE - 1) /TILESIZE - 1);
        }
    }
    else
    {
        csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_IRQHandler);
        csi_hal_irq_enable(DISPLAY_IRQn);
        csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_CORE_IRQHandler);
        csi_hal_irq_enable(AHMI_IRQn);

        AHMI_Config();
        AHMI_SetRot(AHMIOutputRotation);
    }

    initAHMI = 1;

    csi_hal_DCache_Flush();

    GPE_Cmd(ENABLE);
}

#ifdef HUDDraw_ENABLE
void csi_ahmi_Draw_ForHUD(uint32_t tmpFBAddr, HUDDraw_teQuadrant qID)
{
    if(initAHMI)
    {

    }
    else
    {
        csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_IRQHandler);
        csi_hal_irq_enable(DISPLAY_IRQn);
        csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_CORE_IRQHandler);
        csi_hal_irq_enable(AHMI_IRQn);

        AHMI_Config();
        AHMI_SetRot(AHMIOutputRotation);
    }

    initAHMI = 1;

    switch(qID)
    {
        case HUDDraw_LeftTop:
            AHMI_TileBox(0, 0, (SCREEN_WIDTH/2 + TILESIZE - 1) /TILESIZE - 1, (SCREEN_HEIGHT/2 + TILESIZE - 1) / TILESIZE - 1);
        break;

        case HUDDraw_RightTop:
            AHMI_TileBox((SCREEN_WIDTH/2) /TILESIZE, 0, (SCREEN_WIDTH + TILESIZE - 1) /TILESIZE - 1, (SCREEN_HEIGHT/2 + TILESIZE - 1) / TILESIZE - 1);
        break;

        case HUDDraw_RightBottom:
            AHMI_TileBox((SCREEN_WIDTH/2) /TILESIZE, (SCREEN_HEIGHT/2) / TILESIZE, (SCREEN_WIDTH + TILESIZE - 1) /TILESIZE - 1, (SCREEN_HEIGHT + TILESIZE - 1) / TILESIZE - 1);
        break;

        case HUDDraw_LeftBottom:
            AHMI_TileBox(0, (SCREEN_HEIGHT/2) / TILESIZE, (SCREEN_WIDTH/2 + TILESIZE - 1) /TILESIZE - 1, (SCREEN_HEIGHT + TILESIZE - 1) / TILESIZE - 1);
        break;

        case HUDDraw_Total:
            AHMI_TileBox(0, 0, (SCREEN_WIDTH + TILESIZE - 1) /TILESIZE - 1, (SCREEN_HEIGHT + TILESIZE - 1) / TILESIZE - 1);
        break;

        default:
            AHMI_TileBox(0, 0, (SCREEN_WIDTH + TILESIZE - 1) /TILESIZE - 1, (SCREEN_HEIGHT + TILESIZE - 1) / TILESIZE - 1);
        break;
    }

    AHMI_SetFBAdress(tmpFBAddr);
    csi_hal_DCache_Flush();

    GPE_Cmd(ENABLE);
}
#endif
#endif

#if defined(CHIP_GC9002)
uint16_t XStart = 0; 
uint16_t YStart = 0;
uint16_t XEnd = 0; 
uint16_t YEnd = 0;
void csi_ahmi_Draw_ForSBBatch(const FBBatchDraw_Box *pFBBatchDraw_Box, uint8_t boxID)
{
	static BaseType_t xHigherPriorityTaskWoken;
	
//	if(flashSemaphore != NULL)
//    {
//        xSemaphoreTakeFromISR(flashSemaphore, &xHigherPriorityTaskWoken);
//    }
	
	csi_hal_internal_flash_take(1);
	
	if(initAHMI)
	{
//		uint16_t ahmi_XStart = 0; 
//		uint16_t ahmi_YStart = 0;
		uint16_t ahmi_XEnd = 0; 
		uint16_t ahmi_YEnd = 0;
		
		initAHMI = 0;
		if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
		{
			DISPLAY_SetUpdateRange(pFBBatchDraw_Box[boxID].startX, pFBBatchDraw_Box[boxID].startY, pFBBatchDraw_Box[boxID].width, pFBBatchDraw_Box[boxID].height);
		}
		
		//Be careful
		if((GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_SINGLE_BUFFER) || GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_DOUBLE_BUFFER)
		{
//			ahmi_XStart = XStart * (TILESIZE / 16); 
//			ahmi_YStart = YStart * (TILESIZE / 16);
			ahmi_XEnd = XEnd * (TILESIZE / 16) + 1; 
			ahmi_YEnd = YEnd * (TILESIZE / 16) + 1;
			if(ahmi_XEnd > ((SCREEN_WIDTH + 15)/16 - 1))
			{
				ahmi_XEnd = (SCREEN_WIDTH + 15)/16 - 1;
			}
		
			if(ahmi_YEnd > ((SCREEN_HEIGHT + 15)/16 - 1))
			{
				ahmi_YEnd = (SCREEN_HEIGHT + 15)/16 - 1;
			}
			
			AHMI_TileBox(XStart * (TILESIZE / 16), YStart * (TILESIZE / 16), ahmi_XEnd, ahmi_YEnd);
		}
		else if(GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_WITHOUT_BUFFER)
		{
			DISPLAY_SetUpdateRange(XStart * TILESIZE, YStart * TILESIZE, (XEnd - XStart + 1) * TILESIZE, (YEnd - YStart + 1) * TILESIZE);
		}
	}
    else
    {
		#if defined(CHIP_GC9002)
		csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_HandlerISR);
		csi_hal_irq_enable(DISPLAY_IRQn);
		csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_HandlerISR);
		csi_hal_irq_enable(AHMI_IRQn);
        #elif defined(CHIP_GC9005)
		csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_IRQHandler);
		csi_hal_irq_enable(DISPLAY_IRQn);
		csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_CORE_IRQHandler);
		csi_hal_irq_enable(AHMI_IRQn);
		#else
        NVIC_DisableIRQ(DISPLAY_IRQn);
        NVIC_ClearPendingIRQ(DISPLAY_IRQn);
        NVIC_SetPriority(DISPLAY_IRQn, 0);
        NVIC_EnableIRQ(DISPLAY_IRQn);
		NVIC_DisableIRQ(AHMI_IRQn);
        NVIC_ClearPendingIRQ(AHMI_IRQn);
        NVIC_SetPriority(AHMI_IRQn, 0);
        NVIC_EnableIRQ(AHMI_IRQn);
        #endif

        AHMI_Config();
        AHMI_SetRot(AHMIOutputRotation);
    }

	initAHMI++;

    csi_hal_DCache_Flush();

    #if defined(CHIP_GC9002)
	#ifdef DRAWDONETEST_ENBALE
	HAL_GPIO_WritePin(DRAWDONETEST_GPIOX, DRAWDONETEST_GPIOPin, GPIO_PIN_SET);
	#endif
    #endif
    GPE_Cmd(ENABLE);
}
#endif


#ifdef CHIP_GC9002	
uint32_t AHMIParticalDrawType = AHMI_PARTIAL_DRAW_NONE;

uint32_t GetAHMIParticalDrawType(void)
{
	return AHMIParticalDrawType;
}

void SetAHMIParticalDrawType(uint32_t type)
{
	if(type >= AHMI_PARTIAL_DRAW_TYPE_END)
		type = AHMI_PARTIAL_DRAW_NONE;
		
	AHMIParticalDrawType = type;
}
#endif

void csi_ahmi_Draw(void)
{
	#ifndef CHIP_GC9002_NoHyper
	AHMI_SetSBAdress(SBAddress);
	if(SBAddress == SOURCE_BUFFER_START_ADDR)
	{
		SBAddress = SOURCE_BUFFER_START_ADDR + SOURCE_BUFFER_SIZE;
	}
	else
	{
		SBAddress = SOURCE_BUFFER_START_ADDR;
	}
	#endif

//    if(flashSemaphore != NULL)
//    {
//        xSemaphoreTake(flashSemaphore, portMAX_DELAY);
//    }
	
	csi_hal_internal_flash_take(0);
	
	if(initAHMI)
	{
        #if defined(CHIP_GC9002)
		
//		uint16_t ahmi_XStart = 0; 
//		uint16_t ahmi_YStart = 0;
		uint16_t ahmi_XEnd = 0; 
		uint16_t ahmi_YEnd = 0;
		//Be careful
		if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
		{
			if(sourceshift != 0)
			{
				DISPLAY_SetUpdateRange(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			}
		}
		
		//Be careful
		if((GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_SINGLE_BUFFER) || GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_DOUBLE_BUFFER)
		{
//			ahmi_XStart = XStart * (TILESIZE / 16); 
//			ahmi_YStart = YStart * (TILESIZE / 16);
			ahmi_XEnd = XEnd * (TILESIZE / 16) + 1; 
			ahmi_YEnd = YEnd * (TILESIZE / 16) + 1;
			if(ahmi_XEnd > ((SCREEN_WIDTH + 15)/16 - 1))
			{
				ahmi_XEnd = (SCREEN_WIDTH + 15)/16 - 1;
			}
		
			if(ahmi_YEnd > ((SCREEN_HEIGHT + 15)/16 - 1))
			{
				ahmi_YEnd = (SCREEN_HEIGHT + 15)/16 - 1;
			}
			
			AHMI_TileBox(XStart * (TILESIZE / 16), YStart * (TILESIZE / 16), ahmi_XEnd, ahmi_YEnd);
		}
		else if(GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_WITHOUT_BUFFER)
		{
			DISPLAY_SetUpdateRange(XStart * TILESIZE, YStart * TILESIZE, (XEnd - XStart + 1) * TILESIZE, (YEnd - YStart + 1) * TILESIZE);
		}
		#endif
	}
    else
    {
		#if defined(CHIP_GC9002)
		csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_HandlerISR);
		csi_hal_irq_enable(DISPLAY_IRQn);
		csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_HandlerISR);
		csi_hal_irq_enable(AHMI_IRQn);
        #elif defined(CHIP_GC9005)
		csi_hal_irq_register(DISPLAY_IRQn, (void *)DISPLAY_IRQHandler);
		csi_hal_irq_enable(DISPLAY_IRQn);
		csi_hal_irq_register(AHMI_IRQn, (void *)AHMI_CORE_IRQHandler);
		csi_hal_irq_enable(AHMI_IRQn);
		#else
        NVIC_DisableIRQ(DISPLAY_IRQn);	
        NVIC_ClearPendingIRQ(DISPLAY_IRQn);
        NVIC_SetPriority(DISPLAY_IRQn, 0);
        NVIC_EnableIRQ(DISPLAY_IRQn);
		NVIC_DisableIRQ(AHMI_IRQn);	
        NVIC_ClearPendingIRQ(AHMI_IRQn);
        NVIC_SetPriority(AHMI_IRQn, 0);
        NVIC_EnableIRQ(AHMI_IRQn);
        #endif
        
        AHMI_Config();
        AHMI_SetRot(AHMIOutputRotation);
    }
    
	initAHMI = 1;	
		
	//change the buffer of video
	video_cur_r0 = video_next_r0;

    //csi_hal_disable_irq();				
    csi_hal_DCache_Flush();
    //csi_hal_enable_irq();
    
    #if defined(CHIP_GC9002)
	#ifdef DRAWDONETEST_ENBALE
	HAL_GPIO_WritePin(DRAWDONETEST_GPIOX, DRAWDONETEST_GPIOPin, GPIO_PIN_SET);
	#endif
    #endif
    GPE_Cmd(ENABLE);
	
    csi_ahmi_ClearSourceShift();
}

uint8_t drawFlag = 0;
#ifdef VIDEO_Vertical_Synchronization_ENABLE
uint8_t drawVideoFlag = 0;
#endif
void SetDrawFlag(void)
{
    drawFlag = 1;
	#ifdef VIDEO_Vertical_Synchronization_ENABLE
	drawVideoFlag = BT656_AHMIDone;
	#endif
}

void ClearDrawFlag(void)
{
    drawFlag = 0;
	#ifdef VIDEO_Vertical_Synchronization_ENABLE
	drawVideoFlag = BT656_Init;
	#endif
}

uint8_t GetDrawFlag(void)
{
    return drawFlag;
}

extern uint32_t AHMITick;
uint8_t displayconuter = 1;
uint32_t Start_AHMI_tick = 0;
uint32_t AHMIDone_fps = 30;
void csi_ahmi_DrawDoneHandler(void)
{
	int AHMITick_diff = 0;
	static BaseType_t xHigherPriorityTaskWoken;

    #if defined(CHIP_GC9002)
	#ifdef DRAWDONETEST_ENBALE
	HAL_GPIO_WritePin(DRAWDONETEST_GPIOX, DRAWDONETEST_GPIOPin, GPIO_PIN_RESET);
	#endif
    #endif

//	if(flashSemaphore != NULL)
//	{
//	    xSemaphoreGiveFromISR(flashSemaphore, &xHigherPriorityTaskWoken);
//	}
	
	csi_hal_internal_flash_give(1);
	
    #if defined(EMBEDDED) && defined(VERSION_2)
    /* Stop the timer for drawing.*/
    csi_ahmi_StopDrawingTimer();
    #endif

    #if defined(CHIP_GC9002) || defined(CHIP_GC9005)
    /* whether SBBatchDraw done */
    if(SBBatchDraw_u8GetFlag())
    {
        SBBatchDraw_vSetDrawDone();
        return;
    }
    #endif

    #ifdef HUDDraw_ENABLE
    /* whether HUDDraw done */
    if(HUDDraw_u8SetDrawDone())
        return;
    #endif

	//Get AHMI_done time diff, calculate fps by 1000ms/tick diff
	AHMITick_diff = AHMITick - Start_AHMI_tick;
	if(AHMITick_diff < 0)
	{
		AHMITick_diff = -AHMITick_diff;
	}
	AHMIDone_fps = 1000 / AHMITick_diff;
	//Start_AHMI_tick = AHMITick;
	
	//change sourcebuffer and framebuffer
	csi_ahmi_SwitchBuffer();
	
	//set drawFlag
	SetDrawFlag();

	//calculate displayconuter
	if(displayconuter <= BACKLIGHTONFRAMENUMBER)
	{
		displayconuter++;
	}
	
	//start Backlight if needed
	if(csi_ahmi_CheckBacklight())
	{
		csi_ahmi_StartBacklight();
	}
}

extern uint32_t Display_done_fps;
uint32_t Display_times = 0;
uint32_t Last_AHMI_tick = 0;
//static uint32_t Display_u32LastBaseAddr = 0;

//extern funcStatus setDrawControlTrigger(uint32_t irqFlag);

void csi_ahmi_DisplayDoneHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult;
//    #if defined(CHIP_GC9002)
//    uint32_t u32DisplayBaseAddr = 0;
//    #endif

	/***Display_times add 1,when a display_done occurs***/
	Display_times++;
	
	/***if beyound 1s?***/
	if(AHMITick >= (Last_AHMI_tick + 1000))
	{
		Last_AHMI_tick = AHMITick;
		Display_done_fps = Display_times / 3;
		Display_times = 0;
	}
    
//	#if defined(CHIP_GC9002)
//    u32DisplayBaseAddr = StartAddrOfDisplayBuffer + addr_r * (SizeOfDisplayBuffer);
//    if((u32DisplayBaseAddr != Display_u32LastBaseAddr) || (displayconuter <= BACKLIGHTONFRAMENUMBER))
//    {
//        setDrawControlTrigger(1);
//        Display_u32LastBaseAddr = u32DisplayBaseAddr;
//    }
//	#endif

	#ifdef VIDEO_Vertical_Synchronization_ENABLE
    if(GetDrawFlag() && ((drawVideoFlag == BT656_CaptureDone && BT656_GetStartStatus()) || !BT656_GetStartStatus()))
	#else
	if(GetDrawFlag())
	#endif
    {
        /* notify that GPU has done */
        xResult = EVP_i32EventGroupSetBitsIsrGpuDone(&xHigherPriorityTaskWoken);
        if( xResult != pdFAIL )
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }

        ClearDrawFlag();
    }
}

AHMITimerHandle hTimer4 = 
{
	AHMITIM4,
	100,
	TIMCYCLE
};

void AHMITIMCB4(void)
{
    csi_ahmi_DrawingTimeoutHandler();
}

void csi_ahmi_StartDrawingTimer(void)
{
	/* Init the ahmi timer. */
    //InitAHMITimer(&hTimer4);
    
    /* Start the ahmi timer. */
    //StartAHMITimer(&hTimer4);
}

void csi_ahmi_StopDrawingTimer(void)
{
	/* Stop the ahmi timer. */
    //StopAHMITimer(&hTimer4);
}

void csi_ahmi_DrawingTimeoutHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult;
    
    /* notify that Timeout happened */
    xResult = EVP_i32EventGroupSetBitsIsrTimeout(&xHigherPriorityTaskWoken);
    if( xResult != pdFAIL )
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    /* Reset AHMI. */
	Reset_AHMI();
	GPE_Cmd(ENABLE);
}

void csi_ahmi_EnableDisplay(void)
{
	DISPLAY_Cmd(ENABLE);
}

void csi_ahmi_DisableDisplay(void)
{
	DISPLAY_Cmd(DISABLE);
}

void csi_ahmi_SetDisplayContrast(uint32_t contrast)
{
	SetDisplayContrast(contrast);
}

void csi_ahmi_SetDisplaySaturation(uint32_t saturation)
{
	SetDisplaySaturation(saturation);
}

void csi_ahmi_SetDisplayBrightness(uint32_t brightness)
{
	SetDisplayBrightness(brightness);
}

void csi_ahmi_SetDisplayPriParamForRotate(void)
{
    DISPLAY_SetPriParamForRotate();
}

static int STMFlashReadBaseAddr = TEXTUREFLASH_START_ADDR;
void csi_ahmi_ReadConfigData(void* data_addr, uint32_t len)
{
	csi_hal_extern_flash_read(STMFlashReadBaseAddr, len, data_addr);
	STMFlashReadBaseAddr +=len;
}

void csi_ahmi_SeekReadConfigDataAddr(uint32_t addr)
{
	STMFlashReadBaseAddr = TEXTUREFLASH_START_ADDR + addr;
}

uint32_t csi_ahmi_GetCurConfigDataAddr(void)
{
	return (STMFlashReadBaseAddr - TEXTUREFLASH_START_ADDR);
}


void AHMI_HandlerISRCB(void)
{
	if(AHMI_GetITStatus(IT_DC_RDOVER))    
		AHMI_ClearITStatus(IT_DC_RDOVER);
    
	if(AHMI_GetITStatus(IT_SB_ENF)) 
		AHMI_ClearITStatus(IT_SB_ENF);
    
	if(AHMI_GetITStatus(IT_SB_RDOVER)) 
		AHMI_ClearITStatus(IT_SB_RDOVER);
    
	if(AHMI_GetITStatus(IT_DRAW_DONE)) 
	{
		AHMI_ClearITStatus(IT_DRAW_DONE);
		
		#if (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
		extern uint8_t im_up_byGPU_flag;
		if(im_up_byGPU_flag)
		{
			im_up_byGPU_flag = 0;
			return;
		}
		#endif
		
		csi_ahmi_DrawDoneHandler();
    }
}


void DISPLAY_HandlerISRCB(void)
{
    if(DISPLAY_GetITStatus(DISPLAY_IT_PFDD))
    {
		DISPLAY_ClearITStatus(DISPLAY_IT_PFDD);
        csi_ahmi_DisplayDoneHandler();   
    }
	
    if(DISPLAY_GetITStatus(DISPLAY_IT_PREF))
        DISPLAY_ClearITStatus(DISPLAY_IT_PREF);
}


/**
  * @name       csi_ahmi_GetTagValue
  * @brief      Return a tag's value by ID
  * @note       
  * @param      tagID: ID of the tag, which you want read it's value.
  * @retval     The tag's value. 
  * @author     
  * @date       2024-01-02
  */
uint32_t csi_ahmi_GetTagValue(unsigned int tagID)
{
    return getTagValue(tagID);
}


/**
  * @name       csi_ahmi_GetRegValue
  * @brief      Return a reg's value by ID
  * @note       
  * @param      regID: ID of the reg, which you want read it's value.
  * @retval     The reg's value. 
  * @author     
  * @date       2024-01-26
  */
uint32_t csi_ahmi_GetRegValue(unsigned int regID)
{
    return getTagValue(regID2TagID(regID));
}


/**
  * @name       csi_ahmi_SetTagValue
  * @brief      Set a tag's value by ID.
  * @note       
  * @param      ID: ID of the tag, which you want set it's value
  * @param      value:the value you want set for the tag
  * @retval     none 
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_SetTagValue(uint32_t ID, uint32_t value)
{
    setTagValue(ID, value);
}


/**
  * @name       csi_ahmi_SetRegValue
  * @brief      Set a tag's value by RegID.
  * @note       
  * @param      ID: ID of the Reg, which you want set it's value
  * @param      value: the value you want set for the tag
  * @retval     none 
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_SetRegValue(unsigned int ID, unsigned int value)
{
    setRegValue(ID, value);
}


/**
  * @name       csi_ahmi_RegID2TagID
  * @brief      Get a tag's ID by RegID.
  * @note       
  * @param      reg_id: ID of the Reg, which binding a tag
  * @retval     The tag's ID.
  * @author     
  * @date       2024-01-02
  */
unsigned short int csi_ahmi_RegID2TagID(unsigned int reg_id)
{
    return regID2TagID(reg_id);
}


/**
  * @name       csi_ahmi_SetStrRegValue
  * @brief      Set a tag's string by RegID.
  * @note       
  * @param      ID: ID of the Reg, which you want set it's string
  * @param      str: the string you want set for the tag
  * @param      size: size of the string
  * @retval     1: set Reg value success, 0: set Reg value failed 
  * @author     
  * @date       2024-01-02
  */
unsigned int csi_ahmi_SetStrRegValue(unsigned int ID, char* str, unsigned short size)
{
	return setStrRegValue(ID, str, size);
}


/**
  * @name       csi_ahmi_SetArrRegValue
  * @brief      Set a tag's array by RegID.
  * @note       
  * @param      ID: ID of the Reg, which you want set it's array
  * @param      arr: the array you want set for the tag
  * @param      size: size of the arr
  * @retval     1: set Reg value success, 0: set Reg value failed 
  * @author     
  * @date       2024-04-12
  */
unsigned int csi_ahmi_SetArrRegValue(unsigned int ID, char* arr, unsigned short size)
{
	return setArrRegValue(ID, arr, size);
}


/**
  * @name       csi_ahmi_GetStrRegValue
  * @brief      Get a tag's string by RegID.
  * @note       
  * @param      ID: ID of the Reg
  * @param      str: the string to get for the tag
  * @retval     none 
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_GetStrRegValue(unsigned int ID, char* str)
{
	getStrRegValue(ID, str);
}


/**
  * @name       csi_ahmi_SetStrTagValue
  * @brief      Set a tag's string by Tag ID.
  * @note       
  * @param      ID: ID of the Tag, which you want set it's string
  * @param      str: the string you want set for the tag
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_SetStrTagValue(unsigned int ID, char* str)
{
    setStrTagValue(ID, str);
}


/**
  * @name       csi_ahmi_SetStringTag
  * @brief      Set a tag's string by RegID.
  * @note       
  * @param      ID: ID of the Tag, which you want set it's string
  * @param      str: the string you want set for the tag
  * @param      size: size of the string
  * @retval     none
  * @author     
  * @date       2024-01-02
  */
void csi_ahmi_SetStringTag(unsigned int ID, char* str, unsigned short size)
{
    setStringTag(ID, str, size);
}

