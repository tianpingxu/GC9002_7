////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     main.c
// Project Name:  AHMISimulator
//
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.02 - add the modbus definition
// Revision 1.5.0 - add annotation by Yu Chunying
// Additional Comments:
//    in 2.00, add the canvas initiation
// Revision:
// Revision 1.6.2 - Modify 2019/07/16 by Yu ChunYing
// Additional Comments:
//   �޸�Ƕ��ʽ�����FPGA�汾��ȡ����
// 
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "trace.h"
#include "TouchTask.h"
#include "FileLoad.h"
#include "keyboard.h"
#include "semphr.h"
#include "interruptTask.h"
#include "AHMIInterface.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "drawImmediately_cd.h"
#include "em_c1.h"
#include "sm_c1.h"

#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi.h"
#include "csi_ahmi_bt656.h"
#include "csi_ahmi_hw.h"

#if defined(CHIP_GC9002)
#include "SBBatchDraw.h"
#endif

//#include "gc90x_config.h"
//#include "gc90x_initial.h"
//#include "gc90x_flash.h"
#include "gc90x_heap.h"

//#include "gc90x_wdg2.h"
#endif

#ifdef PC_SIM
extern u8				TextureBuffer[TEXBUFSIZE];
//extern u8 animationDuration;
extern FILE *fp;
extern u8 shouldUpdateMainWindow;
extern u8 shouldUpdateSubWindow;
extern void hideSubScreen(void);
extern "C" extern int embedded_platform;
#endif

#ifdef AHMI_CORE

#ifdef __cplusplus
extern "C"{
#endif
extern SM_tstStoreItem astStoreItem[MaxStoreTagNum];
extern u32 StoreStringTagArry[MaxStoreStringTagWordSize];
#ifdef __cplusplus
};
#endif

#ifdef EMBEDDED
#define Lightness               359            
#define EMPTYPOINTERSIZE                    16

#if defined(EMBEDDED) && defined(VERSION_2)
//u32 StoreTagArry[StoreTagMaxSizeOfWord] = {0};
//u32 StoreStringTagArry[StoreTagMaxSizeOfWord] = {0};
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
extern uint32_t SBAddress;
#endif
extern uint32_t StartAddrOfSoundBuffer;
//extern uint32_t SDRAMStartAddr;
//extern uint32_t SDRAMStopAddrForSound;
extern uint16_t* SoundSpace;
extern uint8_t DisplayContrast;
extern uint8_t DisplaySaturation;
extern uint8_t DisplayBrightness;
extern uint32_t AHMIOutputRotation;
extern uint32_t BacklightWaitDuration;
extern uint32_t BacklightNormalBrightness;
#endif
#endif

u32 InitialAHMIStruct( void );

extern  ConfigInfoClass		ConfigData;
extern TimerClassPtr		TimerPtr;
extern DynamicPageClassPtr  gPagePtr;
extern k_msgq_handle_t		RefreshQueue;
extern k_msgq_handle_t        RefreshQueueWithoutDoubleBuffer;
extern TagClassPtr			TagPtr;
extern WaveFilter*			gWaveFilterPtr; //pointer of wave filters
extern SoundClass*          gSoundPtr;
extern WaveFilterStatus*	gWaveFilterStatusPtr; //pointer of wave filter status
extern WaveFilterTickArg*	gWaveFilterTickArgPtr;
extern DelayArg*			gDelayArgPtr;
extern StringClassPtr       StringListPtr;
extern u8*					gStringPtr;
extern ArrayDataClassPtr ArrayDataListPtr; //pointer of ArrayData List
extern u8*               gArrayDataPtr; //start pointer of ArrayData data

extern FontRangeClass* FontRangeListPtr;
extern EncodingRange* gRangesPtr;

extern HintClass*           gHintClass;
extern TagUpdateClassPtr    TagUpdatePtr;
extern u8*                  gTagValueSpacePtr;
extern u8*					TagStringPtr;
extern u8*					TagStringListPtr;
extern u8*                  TagChangedListPtr;
extern u32*					gBackgroundTextureAddrPtr;
extern u16 gScreenWidth;
extern u16 gScreenHeight;
extern ProjectScreenOptions gScreenOptions;
extern IntervalManager gIntervalManager;
//extern u8 animationDuration;  //animation duration
extern AnimationClass  gAnimationClass;
extern KBState*       gKBState;
extern MenuState*     gMenuState;

extern k_msgq_handle_t		ActionInstructionQueue;
#ifdef PC_SIM
extern "C" extern k_msgq_handle_t		MouseQueue;
extern"C" extern struct globalArgs_t globalArgs;
#endif
extern k_msgq_handle_t		ModbusMasterRWTaskQueue;
extern k_msgq_handle_t		AnimationTaskQueue;
extern k_msgq_handle_t        keyboardQueue;



extern k_task_handle_t TouchTaskHandle;
#ifdef VIDEO_EN
extern k_task_handle_t InterruptTaskHandle;
extern k_task_handle_t CalibrationTaskHande;
#endif
#if defined(CHIP_GC9005) || defined(PC_SIM)
extern k_task_handle_t H264DecodeTaskHandle;
#endif
extern k_task_handle_t ActionProcessorTaskHandle;
extern k_task_handle_t TimerProcessTaskHandle;
extern k_task_handle_t AnimationTimerProcessTaskHandle;
extern k_task_handle_t AnimationTaskHandle;
extern k_task_handle_t Cyclic10msTaskHandle;
#if defined(EMBEDDED) && defined(VERSION_2)
extern k_task_handle_t SetValueTaskHandle;
#endif

#ifdef WHOLE_TRIBLE_BUFFER
extern k_task_handle_t screenUpdateTaskHandle;
#endif

u16 KeyBoard_PageID = 0;
u16 TouchCalibration_PageID = 0;
u16 ErrorManagement_PageID = 0;
u16 Debugging_PageID = 0;

extern void*    PageSpace;
extern 	u16    	WorkingPageID;
extern u16 startAddrOfTagChangedList;
extern u16 stopAddrOfTagChangedList;
typedef u32 		pointersize;

extern uint32_t *pPageCacheAddr;
extern uint32_t *pSubPageCacheAddr;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

//k_sem_handle_t flashSemaphore;
//extern k_sem_handle_t interuptSemaphore_cvbs;
#if defined(CHIP_GC9005) || defined(PC_SIM)
extern k_sem_handle_t startPlayH264Semaphore;
#endif
//extern k_sem_handle_t interuptSemaphore_animation;
//extern k_sem_handle_t interuptSemaphore_pagerefresh;
//extern k_sem_handle_t interuptSemaphore_touch;

//config information string
extern char* cfgString;
#if (defined WHOLE_TRIBLE_BUFFER) || (defined PARTIAL_TRIBLE_BUFFER)
extern u16     addr_combine;
#endif

//extern 	u8    sourceBuffer[];

#ifdef EMBEDDED

extern uint32_t flag_Calibration;
extern u32 StartOfConfigdata;
extern u8 		initAHMI;
#endif

#ifdef PC_SIM
//extern u8 animationDuration;
#endif

extern uint32_t TouchScreenType;

uint32_t* TimerIntervalListPtr;
uint32_t  NumberOfTimer = 0;

SemaphoreHandle_t xSemaphoreSetValue = NULL;
SemaphoreHandle_t xSemaphoreSetString = NULL;

//-----------------------------
// function: initSemaphore.
// @brief: Create Semaphore for AHMI.
// parameters:
//-----------------------------
void initSemaphore(void)
{
    xSemaphoreSetValue = xSemaphoreCreateBinary();
    if (xSemaphoreSetValue == NULL)
    {
        ERROR_PRINT("Fail to create xSemaphoreSetValue.\r\n");
    }

    xSemaphoreSetString = xSemaphoreCreateBinary();
    if (xSemaphoreSetString == NULL)
    {
        ERROR_PRINT("Fail to create xSemaphoreSetString.\r\n");
    }
}

//-----------------------------
// function: initQueue.
// @brief: Create queues for AHMI.
// parameters:
//-----------------------------
void initQueue()
{
    /* Create a queue for action instruction. */
    ActionInstructionQueue = csi_kernel_msgq_new(ActionInstructionQueueSize,sizeof(pointersize));

#ifdef PC_SIM
    /* Create a queue for Refresh message. */
    RefreshQueue = csi_kernel_msgq_new(50,sizeof(RefreshMsg));
    
    /* Create a queue for Refresh message without double buffer. */
    RefreshQueueWithoutDoubleBuffer = csi_kernel_msgq_new(3,sizeof(RefreshMsg));
    
    /* Create a queue for keyboard. */
    keyboardQueue = csi_kernel_msgq_new(5,sizeof(u8));
    
    /* Create a queue for mouse message. */	
	MouseQueue = csi_kernel_msgq_new(5,sizeof(PIDState));
	
	/* Create a queue for animation. */	
	AnimationTaskQueue = csi_kernel_msgq_new(10, sizeof(AnimationMsg));
#endif

}

#ifdef WATCH_DOG_CPU1
extern void SetWDG2(void);
#endif


void Cyclic10msTask(void* pvParameters)
{
    static uint32_t u32Counter = 0;


#ifdef WATCH_DOG_CPU1
    /* set WDG2 enable if need */
	SetWDG2();
#endif

 	for(;;)
 	{
        u32Counter++;
        /* Container for 10ms Task*/
        {
#ifdef WATCH_DOG_CPU1
        	/* WDG2 feed dog */
	        WDG2_ReloadCounter();
#endif
            /* Add 10ms Task here */
            
        }

        /* Container for 5000ms Task*/
        if(0 == (u32Counter % (5000 / portTICK_RATE_MS)))
        {
            /* Add 5000ms Task here */
            ErrorManagementTask();
        }

        /* Container for 10000ms Task*/
        if(0 == (u32Counter % (10000 / portTICK_RATE_MS)))
        {
            /* Add 10000ms Task here */

        }
            
        vTaskDelay( 10 / portTICK_RATE_MS );
    }
}

//-----------------------------
// function: startAHMI.
// @brief: Create tasks for AHMI, init data structures and strat AHMI.
// parameters:
//-----------------------------
int startAHMI( void )
{
	int err;
#ifdef EMBEDDED	
	uint32_t i,k;
#endif
	portBASE_TYPE xReturn;

	#if defined(EMBEDDED) && defined(VERSION_2)
	#ifndef CHIP_GC9002_NoHyper
	/* Create a task for sharememory management and drawdone led control. */
    xReturn = csi_kernel_task_new(SetValueTask, "SetValueTask", NULL, KPRIO_LOW2, 0, NULL, configMINIMAL_STACK_SIZE, &SetValueTaskHandle);
    //xReturn = xTaskCreate(SetValueTask, "SetValueTask", configMINIMAL_STACK_SIZE, NULL, 3, &SetValueTaskHandle);
    if (xReturn != 0)
    {
        ERROR_PRINT("Fail to create SetValueTask Task.\r\n");
        return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }
	#endif
    #endif

	#ifndef CHIP_GC9002_NoHyper
	/* Create a task for error management. */
    xReturn = csi_kernel_task_new(Cyclic10msTask, "Cyclic10msTask", NULL, KPRIO_LOW1, 0, NULL, configMINIMAL_STACK_SIZE, &Cyclic10msTaskHandle);
	//xReturn = xTaskCreate(Cyclic10msTask, "Cyclic10msTask", configMINIMAL_STACK_SIZE , NULL, 2, &Cyclic10msTaskHandle);
    if (xReturn != 0)
    {
        ERROR_PRINT("Fail to create Cyclic10msTask Task.\r\n");
        return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }
	#endif
	
#if defined(EMBEDDED) && defined(VERSION_2)
    /* If use touch screen. */
    if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENTYPEEND))
    {
        /* Create a task for touch event. */
        xReturn = csi_kernel_task_new(Touch_vMain, "TouchTask", NULL, KPRIO_LOW3, 0, NULL, configMINIMAL_STACK_SIZE, &TouchTaskHandle);
        //xReturn = xTaskCreate(Touch_vMain, "TouchTask", configMINIMAL_STACK_SIZE*2, NULL, 2, &TouchTaskHandle);
        if (xReturn != 0)
        {
            ERROR_PRINT("Fail to create TouchTask Task.\r\n");
            return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
        }
    }
#endif

#ifdef PC_SIM
    /* Create a task for touch screen. */
    xReturn = csi_kernel_task_new(Touch_vMain, "TouchTask", NULL, KPRIO_LOW1, 0, NULL, configMINIMAL_STACK_SIZE, &TouchTaskHandle);
	//xReturn = xTaskCreate(Touch_vMain, "TouchTask", configMINIMAL_STACK_SIZE , NULL, 2, &TouchTaskHandle);
    if (xReturn != 0)
    {
        ERROR_PRINT("Fail to create TouchTask Task.\r\n");
        return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }
#endif /*enable touch function*/

    /* Create a task for page refresh. */
#if 0  /* by Zjh, 2022-09-28 */
    xReturn = xTaskCreate(PageRefreshTask,"PageRefreshTask",configMINIMAL_STACK_SIZE * 8,NULL,4,&PageRefreshTaskHandle);
	if(xReturn != pdTRUE)
	{
		ERROR_PRINT("Fail to create PageRefreshTask Task.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
	}
#endif

    /* Create a task for action processor. */
    xReturn = csi_kernel_task_new(ActionProcessorTask, "ActionProcessorTask", NULL, KPRIO_LOW2, 0, NULL, configMINIMAL_STACK_SIZE*2, &ActionProcessorTaskHandle);
    //xReturn =  xTaskCreate(ActionProcessorTask,"ActionProcessorTask",configMINIMAL_STACK_SIZE*2 ,NULL,2,&ActionProcessorTaskHandle);
    if(xReturn != 0)
    {
        ERROR_PRINT("Fail to create ActionProcessorTask Task.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }

    /* Create a task for timer processor. */
    xReturn = csi_kernel_task_new(TimerProcessTask, "TimerProcessTask", NULL, KPRIO_LOW2, 0, NULL, configMINIMAL_STACK_SIZE, &TimerProcessTaskHandle);
    //xReturn =  xTaskCreate(TimerProcessTask,"TimerProcessTask",configMINIMAL_STACK_SIZE ,NULL,2,&TimerProcessTaskHandle);
    if(xReturn != 0)
    {
        ERROR_PRINT("Fail to create TimerProcessTask Task.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }


#ifdef EMBEDDED
    /* Create a task for animation timer processor. */
    xReturn = csi_kernel_task_new(AnimationTimerProcessTask, "AnimationTimerProcessTask", NULL, KPRIO_LOW1, 0, NULL, configMINIMAL_STACK_SIZE, &AnimationTimerProcessTaskHandle);
    //xReturn =  xTaskCreate(AnimationTimerProcessTask,"AnimationTimerProcessTask",configMINIMAL_STACK_SIZE ,NULL,2,&AnimationTimerProcessTaskHandle);
    if(xReturn != 0)
    {
        ERROR_PRINT("Fail to create AnimationTimerProcessTask Task.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }
#endif


	/* Create Semaphore for animation. */
    //interuptSemaphore_animation  = csi_kernel_sem_new(1, 0);

    /* Create Semaphore for page refresh. */
    //interuptSemaphore_pagerefresh  = csi_kernel_sem_new(1, 0);

	#ifdef PC_SIM
    /* Create a task for animation queue processor. */
    xReturn = csi_kernel_task_new(animationQueueTask, "animationQueueTask", NULL, KPRIO_LOW3, 0, NULL, 256, &AnimationTaskHandle);
	//xReturn  = xTaskCreate(animationQueueTask,"animationQueueTask",64,NULL,4,&AnimationTaskHandle);
	if(xReturn != 0)
	{
		ERROR_PRINT("Fail to create animationQueueTask Task.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
	}
	#endif
	
#ifdef VIDEO_EN
    /* Create a task for interupt processor for cvbs. */
//    xReturn = csi_kernel_task_new(interuptTask, "interuptTask", NULL, KPRIO_LOW2, 0, NULL, configMINIMAL_STACK_SIZE, &InterruptTaskHandle);
//    //xReturn  = xTaskCreate(interuptTask,"interuptTask",configMINIMAL_STACK_SIZE,NULL,2,&InterruptTaskHandle);
//    if(xReturn != 0)
//    {
//		ERROR_PRINT("Fail to create interuptTask Task.\r\n");
//		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
//    }

    /* Create a task for cvbs resolution calibration.  */
    xReturn = csi_kernel_task_new(calibrationTask, "calibrationTask", NULL, KPRIO_LOW1, 0, NULL, configMINIMAL_STACK_SIZE, &CalibrationTaskHande);
    //xReturn  = xTaskCreate(calibrationTask,"calibrationTask",configMINIMAL_STACK_SIZE,NULL,2,&CalibrationTaskHande);
    if(xReturn != 0)
    {
		ERROR_PRINT("Fail to create calibrationTask Task.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }
	
	/* Create Semaphore for cvbs. */
//	interuptSemaphore_cvbs = csi_kernel_sem_new( 10, 0 ); 
//	if(interuptSemaphore_cvbs == NULL)
//	{
//		ERROR_PRINT("Fail to create semaphore interuptSemaphore_cvbs.\r\n");
//		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
//	}
#endif

//	/* Create Semaphore for flash. */
//    flashSemaphore = csi_kernel_sem_new( 1, 1 );
//	if(flashSemaphore == NULL)
//	{
//		ERROR_PRINT("Fail to create semaphore flashSemaphore.\r\n");
//		return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
//	}
	
#if defined(CHIP_GC9005) || defined(PC_SIM)
	/* Create Semaphore for h264. */
	startPlayH264Semaphore = csi_kernel_sem_new( 1, 0 );
    if(startPlayH264Semaphore == NULL)
    {
        ERROR_PRINT("Fail to create semaphore startPlayH264Semaphore.\r\n");
        return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }

    /* Create a task for interupt processor for h264. */
    xReturn = csi_kernel_task_new(h264PlayTask, "h264PlayTask", NULL, KPRIO_LOW2, 0, NULL, configMINIMAL_STACK_SIZE*4, &H264DecodeTaskHandle);
    if(xReturn != 0)
    {
        ERROR_PRINT("Fail to create h264DecodeTask Task.\r\n");
        return (EM_enErrRetModularInit | EM_ErrRetInit_CreateTaskErr);
    }
#endif

#ifdef WHOLE_TRIBLE_BUFFER
#if 0
    /* Create a task for screen update. */
    xReturn = xTaskCreate(screenUpdateTask,"screenUpdateTask",configMINIMAL_STACK_SIZE,NULL,2,&screenUpdateTaskHandle);
    if(xReturn != pdTRUE)
    {
        ERROR_PRINT("Fail to create swtichDispBuffer Task.\r\n");
        return 0;
    }
#endif
#endif

//    REF_vInit();    //add by Zhu Jiahai
    
    /* Initial AHMI data struct. */
    err = InitialAHMIStruct();
#ifdef EMBEDDED
  //err = 0;
#ifndef DDR_TEST
    /* If failed to initial AHMI data struct. */
    if (err != AHMI_FUNC_SUCCESS)
    {
      /* Enable backlight. */
      csi_ahmi_SetBacklightBrightness(100);
      csi_ahmi_StartBacklightForFivePoint();
#endif

		for(;;)
		{
			/* Display five red point page. */
			for (k=0; k<3; k++)
			{
				DrawFivePointPage(
					SCREEN_WIDTH,
					SCREEN_HEIGHT,
					0xFFFFFFFF, //BackgroundColor:white
					EMPTYPOINTERSIZE,
					//LeftTop position of LeftTop Point
					0,
					0,
					0xFFFF0000, //Pointer0Color:red
					//LeftTop position of RightTop Point
					SCREEN_WIDTH - EMPTYPOINTERSIZE,
					0,
					0xFFFF0000, //Pointer1Color:black
					//LeftTop position of LeftBottum Point
					0,
					SCREEN_HEIGHT - EMPTYPOINTERSIZE,
					0xFFFF0000, //Pointer1Color:black
					//LeftTop position of RightBottum Point
					SCREEN_WIDTH - EMPTYPOINTERSIZE,
					SCREEN_HEIGHT - EMPTYPOINTERSIZE,
					0xFFFF0000, //Pointer1Color:black
					//LeftTop position of Center Point
					SCREEN_WIDTH / 2,
					SCREEN_HEIGHT / 2,
					0xFFFF0000 //Pointer1Color:black
					);
					for(i = 0; i < 720000; i++)
					{
						i++;
					}
			  }
			
		}
#ifndef	DDR_TEST
	}
#endif
#endif	
    
#ifdef PC_SIM
	if(err != AHMI_FUNC_SUCCESS) 
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
	else
		return AHMI_FUNC_SUCCESS;
#endif
#ifdef EMBEDDED		
	return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
#endif
}

//-----------------------------
// function: InitialAHMIStruct.
// @brief: init data structures and load the first page.
// parameters:
//-----------------------------
u32 InitialAHMIStruct( void )
{
#ifdef PC_SIM
	char text[200];
	size_t count;
#endif
    
	u32 baseaddr;
	u32 totalsize;
	u32 checkCode;
	u32 currentPagePos;
	int ID;
	int err;
	u32 i;
	int numOfTagChangedList;
    DynamicPageCfgClass tempDynamicPageCfgClass;
	DynamicPageCfgClass* newPage = &tempDynamicPageCfgClass;
	u16* SoundPtrInTextureFlash = 0;
	u16 currentTagID = 0;
	u16 currentStringTagLength = 0;
	u16 currentStringRegID = 0;
	u16 currentStringPos = 0;
	u16 pageID = 0;
	#ifdef CHIP_GC9002_NoHyper
	u8 PageSlidingSwitch_Support = 0;
	#endif
	
#ifdef PC_SIM	
	if( OpenACFFile() != AHMI_FUNC_SUCCESS)
	{
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
	}
#endif
	
	/* Read ConfigData and check vision. */
	err = LoadConfigData(&ConfigData);
	if(err != AHMI_FUNC_SUCCESS)
	{
		ERROR_PRINT("File read error.\r\n");
		return err;
	}

	/* Get System PageID. */
	KeyBoard_PageID = (ConfigData.NumofPages - (ConfigData.NumofSysPage + 1 - KeyBoard));
	TouchCalibration_PageID = (ConfigData.NumofPages - (ConfigData.NumofSysPage + 1 - TouchCalibration));
	ErrorManagement_PageID = (ConfigData.NumofPages - (ConfigData.NumofSysPage + 1 - ErrorManagement));
	Debugging_PageID = (ConfigData.NumofPages - (ConfigData.NumofSysPage + 1 - Debugging));
	
	/* Set screen's width and heigh. */
	gScreenWidth = ConfigData.AHMIInfo.ScreenWidth;
	gScreenHeight = ConfigData.AHMIInfo.ScreenHeight;
	gScreenOptions = ConfigData.screenOptions;
	#ifdef PC_SIM
	if(gScreenOptions.enable)
	{
		if(embedded_platform != EMBEDDED_GC9005)
		{
			ERROR_PRINT("Current embedded platform don't support double screen!.\r\n");
			return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
		}
		shouldUpdateMainWindow = 1;
		shouldUpdateSubWindow = 1;
	}
	else
	{
		hideSubScreen();
	}
	#endif
    
	#if defined(EMBEDDED) && defined(VERSION_2)
	#if defined(CHIP_GC9002)
	#ifndef CHIP_GC9002_NoHyper
	/* Malloc sdram space for sourcebuffer. */
	StartAddrOfSourcebuffer = (uint32_t)gcMalloc(SOURCE_BUFFER_SIZE * 2);//sourcebuffer double buffer
	if(StartAddrOfSourcebuffer == 0)
	{
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	SBAddress = StartAddrOfSourcebuffer;
	
	//if((DISPLAY_GetMode() == DISPLAY_Mode_MasterActive) || (DISPLAY_GetMode() == DISPLAY_Mode_MasterPassive))
	if(1)
	{
		uint8_t PixelBytes = 0;
		if(DISPLAY_InputColorMode_RGBA8888 == DISPLAY_u32GetInputColorMode())
		{
			PixelBytes = 4;//8bitR 8bitG 8bitB 8bitA
		}
		else if(DISPLAY_InputColorMode_RGB565 == DISPLAY_u32GetInputColorMode())
		{
			PixelBytes = 2;//5bitR 6bitG 5bitB
		}
		else
		{
			PixelBytes = 2;//warning
		}
		
		if(CFG_Get_sys_if_partial_refresh_en() == 0)
		{
			/* Malloc sdram space for display by screen's width and heigh. */
			StartAddrOfDisplayBuffer = (uint32_t)gcMalloc(gScreenWidth * gScreenHeight * PixelBytes * (2 + PAGE_CACHE_NUM));//RGB565 double buffer
			if(StartAddrOfDisplayBuffer == 0 && gScreenWidth != 0 && gScreenHeight != 0)
			{
				return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
			}
			SizeOfDisplayBuffer = gScreenWidth * gScreenHeight * PixelBytes;
		
			BatchDraw_vSetBatchDrawType(BatchDraw_SB);
			
			SetAHMIParticalDrawType(AHMI_PARTIAL_DRAW_NONE);
		}
		else 
		{
			/* Malloc sdram space for display by screen's width and heigh. */
			StartAddrOfDisplayBuffer = (uint32_t)gcMalloc(gScreenWidth * gScreenHeight * PixelBytes * (2 + PAGE_CACHE_NUM));//RGB565 double buffer
			if(StartAddrOfDisplayBuffer != 0 && gScreenWidth != 0 && gScreenHeight != 0)
			{
				SizeOfDisplayBuffer = gScreenWidth * gScreenHeight * PixelBytes;
		
				BatchDraw_vSetBatchDrawType(BatchDraw_SB);
			
				SetAHMIParticalDrawType(AHMI_PARTIAL_DRAW_DOUBLE_BUFFER);
				
			}
			else
			{
				StartAddrOfDisplayBuffer = (uint32_t)gcMalloc(gScreenWidth * gScreenHeight * PixelBytes * (1 + PAGE_CACHE_NUM));
				if(StartAddrOfDisplayBuffer != 0 && gScreenWidth != 0 && gScreenHeight != 0)
				{
					SizeOfDisplayBuffer = gScreenWidth * gScreenHeight * PixelBytes;
		
					BatchDraw_vSetBatchDrawType(BatchDraw_SB);
			
					SetAHMIParticalDrawType(AHMI_PARTIAL_DRAW_SINGLE_BUFFER);
				}
				else
				{
					return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
				}
			}
			
		}
	}
	else
	{
		extern const FBBatchDraw_Box mFBBatchDraw_Box[4];
		extern uint32_t StartAddrOfSourcebufferForFBBatch;
		/* Malloc sdram space for sourcebuffer. */
		StartAddrOfSourcebufferForFBBatch = (uint32_t)gcMalloc(SBBatchDrawSBAddrMax * sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box));
		if(StartAddrOfSourcebufferForFBBatch == 0)
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
		
		BatchDraw_vSetBatchDrawType(BatchDraw_FB);
	}
	#else
	#if 0
	//only for debug test
	SBAddress = StartAddrOfSourcebuffer;
	if((DISPLAY_GetMode() == DISPLAY_Mode_MasterActive) || (DISPLAY_GetMode() == DISPLAY_Mode_MasterPassive))
	{
		/* Malloc sdram space for display by screen's width and heigh. */
		StartAddrOfDisplayBuffer = (uint32_t)gcMalloc(gScreenWidth * gScreenHeight * 2 * 2);//RGB565 double buffer
		if(StartAddrOfDisplayBuffer == 0 && gScreenWidth != 0 && gScreenHeight != 0)
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
		SizeOfDisplayBuffer = gScreenWidth * gScreenHeight * 2;
	}
	#endif /* end of #if 0 */
	#endif
	#elif defined(CHIP_GC9005)
	/* Malloc sdram space for sourcebuffer. */
	StartAddrOfSourcebuffer = (uint32_t)gcMalloc(SOURCE_BUFFER_SIZE * 2);//sourcebuffer double buffer
	if(StartAddrOfSourcebuffer == 0)
	{
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	SBAddress = StartAddrOfSourcebuffer;
	
	if(1)
	{
	    BatchDraw_vSetBatchDrawType(BatchDraw_SB);
	}
	else
	{
	    extern const FBBatchDraw_Box mFBBatchDraw_Box[4];
        extern uint32_t StartAddrOfSourcebufferForFBBatch;
        /* Malloc sdram space for sourcebuffer. */
        StartAddrOfSourcebufferForFBBatch = (uint32_t)gcMalloc(SBBatchDrawSBAddrMax * sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box));
        if(StartAddrOfSourcebufferForFBBatch == 0)
        {
            return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
        }

        BatchDraw_vSetBatchDrawType(BatchDraw_FB);
    }

	if(gScreenOptions.enable)
	{
	    /* Malloc sdram space for display by screen's width and heigh. */
	    SizeOfDisplayBuffer = DISPLAY->CH0_FTB;
        StartAddrOfDisplayBuffer = (uint32_t)gcMalloc(SizeOfDisplayBuffer * (2 + PAGE_CACHE_NUM));//maybe SFBC, double buffer
        if(StartAddrOfDisplayBuffer == 0 && SizeOfDisplayBuffer != 0)
        {
            return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
        }

        /* Malloc sdram space for sub display by screen's width and heigh. */
        //SizeOfDisplayBuffer = DISPLAY->CH1_FTB;//hardware bug this register read always return 0 !!!
        StartAddrOfSubDisplayBuffer = (uint32_t)gcMalloc(SizeOfDisplayBuffer * (2 + PAGE_CACHE_NUM));//maybe SFBC, double buffer
        if(StartAddrOfSubDisplayBuffer == 0 && SizeOfDisplayBuffer != 0)
        {
            return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
        }
	}
	else
	{
	    /* Malloc sdram space for display by screen's width and heigh. */
	    SizeOfDisplayBuffer = DISPLAY->CH0_FTB;
        StartAddrOfDisplayBuffer = (uint32_t)gcMalloc(SizeOfDisplayBuffer * (2 + PAGE_CACHE_NUM));//maybe SFBC, double buffer
        if(StartAddrOfDisplayBuffer == 0 && SizeOfDisplayBuffer != 0)
        {
            return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
        }
    }
	#endif
	#endif

	#ifdef PAGE_CACHE_ENABLE	
	pPageCacheAddr = (uint32_t *)csi_kernel_malloc(sizeof(uint32_t) * ConfigData.NumofPages, NULL);
	if(pPageCacheAddr == NULL)
	{
		ERROR_PRINT("pPageCacheAddr Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	memset(pPageCacheAddr, 0, sizeof(uint32_t) * ConfigData.NumofPages);

	if(gScreenOptions.enable)
	{
		pSubPageCacheAddr = (uint32_t *)csi_kernel_malloc(sizeof(uint32_t) * ConfigData.NumofPages, NULL);
		if(pSubPageCacheAddr == NULL)
		{
			ERROR_PRINT("pSubPageCacheAddr Initial failed.\r\n");
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
		memset(pSubPageCacheAddr, 0, sizeof(uint32_t) * ConfigData.NumofPages);
	}
	#endif

    /* Get the number of software Timer. */
    NumberOfTimer = ConfigData.NumofTimers;

    /* Malloc memory for software Timers. */
	TimerPtr= (TimerClassPtr)csi_kernel_malloc(sizeof(TimerClass)* ConfigData.NumofTimers, NULL);
	if(TimerPtr == NULL && ConfigData.NumofTimers != 0 )
	{
		ERROR_PRINT("Timer Class Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
    
    /* Malloc memory for software Timers inter value. */
    TimerIntervalListPtr = (uint32_t *)csi_kernel_malloc(sizeof(uint32_t)* ConfigData.NumofTimers, NULL);
	if(TimerIntervalListPtr == NULL && ConfigData.NumofTimers != 0 )
	{
		ERROR_PRINT("Timer Class Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
    
    /* Malloc memory for dynamic pages. */
	gPagePtr = (DynamicPageClassPtr)csi_kernel_malloc(sizeof(DynamicPageClass) * ConfigData.NumofPages, NULL);
	if(gPagePtr == NULL && ConfigData.NumofPages != 0 )
	{
		ERROR_PRINT("Page Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	
	/* Malloc memory for wave filter. */
	gWaveFilterPtr = (WaveFilter*)csi_kernel_malloc(sizeof(WaveFilter)*ConfigData.NumOfWaveFilter, NULL);
	if (gWaveFilterPtr==NULL && ConfigData.NumOfWaveFilter != 0)
	{
		ERROR_PRINT("wave filter Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	
	/* Malloc memory for wave filter status. */
	gWaveFilterStatusPtr = (WaveFilterStatus*)csi_kernel_malloc(sizeof(WaveFilterStatus)*ConfigData.NumOfWaveFilterStatus, NULL);
	if (gWaveFilterStatusPtr==NULL && ConfigData.NumOfWaveFilterStatus != 0)
	{
		ERROR_PRINT("wave filter status Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}

	/* Initial IntervalManager. */
	if(gIntervalManager.init() != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularInterval | EM_ErrRetInterval_NULLPointerErr);
	
	/* Malloc memory for wave filter tick. */
	if (ConfigData.NumOfWaveFilterStatus > 0)
	{
		gWaveFilterTickArgPtr = (WaveFilterTickArg*)csi_kernel_malloc(sizeof(WaveFilterTickArg)*IntervalMaxNum, NULL);
		if ((gWaveFilterTickArgPtr == NULL) && (IntervalMaxNum != 0))
		{
			ERROR_PRINT("wave filter tick args Initial failed.\r\n"); 
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
	}
	
	/* Malloc memory for delay arg. */
	gDelayArgPtr = (DelayArg*)csi_kernel_malloc(sizeof(DelayArg)*IntervalMaxNum, NULL);
	if ((gDelayArgPtr == NULL) && (IntervalMaxNum != 0))
	{
		ERROR_PRINT("wave filter tick args Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}

    /* Malloc memory for tags. */
	TagPtr  = (TagClassPtr)csi_kernel_malloc(sizeof(TagClass) * ConfigData.NumofTags, NULL);
	if(TagPtr == NULL && ConfigData.NumofTags != 0 )
	{
		ERROR_PRINT("Tag Class Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	
	/* Malloc memory for sounds*/
	gSoundPtr = (SoundClass*)csi_kernel_malloc(sizeof(SoundClass) *ConfigData.NumofSound, NULL);
	if(gSoundPtr == NULL && ConfigData.NumofSound != 0 )
	{
		ERROR_PRINT("Sound Class Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	
	/* Malloc memory for stringList. */
	StringListPtr = (StringClassPtr)csi_kernel_malloc(sizeof(StringClass) * ConfigData.NumofStrings, NULL);
	if(StringListPtr == NULL && ConfigData.NumofStrings != 0 )
	{
		ERROR_PRINT("StringPtr Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}

	/* Malloc memory for ArrayDataList. */
	ArrayDataListPtr = (ArrayDataClassPtr)csi_kernel_malloc(sizeof(ArrayDataClass) * ConfigData.NumofArrayDatas, NULL);
	if(ArrayDataListPtr == NULL && ConfigData.NumofArrayDatas != 0 )
	{
		ERROR_PRINT("ArrayData List Initial failed.\r\n");
	}

	/* Malloc memory for font range list. */
	FontRangeListPtr = (FontRangeClass*)csi_kernel_malloc(sizeof(FontRangeClass) * ConfigData.NumOfFontRanges, NULL);
	if(FontRangeListPtr == NULL && ConfigData.NumOfFontRanges != 0 )
	{
		ERROR_PRINT("Font Range List Initial failed.\r\n");
	}

	/* Malloc memory for TagUpdateList. */
	TagUpdatePtr = (TagUpdateClassPtr)csi_kernel_malloc(sizeof(TagUpdateClass) * ConfigData.NumofTagUpdateQueue, NULL);
	if(TagUpdatePtr == NULL && ConfigData.NumofTagUpdateQueue != 0 )
	{
		ERROR_PRINT("Tag Update Queue Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	
    /*Get the number of tags. */
    numOfTagChangedList = ConfigData.NumofTags;
    
	/* Malloc memory for TagChangedList. */
	TagChangedListPtr = (u8*)csi_kernel_malloc(numOfTagChangedList * sizeof(u8), NULL);
	if(TagChangedListPtr == NULL && ConfigData.NumofTags != 0)
	{
		ERROR_PRINT("TagChangedListPtr Initial failed.\r\n"); 
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	
    /* Init TagChangedList. */
	for(ID = 0; ID < numOfTagChangedList; ID ++)
	{
		TagChangedListPtr[ID] = 0;
	}
	
	/* Init encoding ranges. */
	initEncodingRanges();
	
	/* Malloc memory for KB state. */
	gKBState = (KBState*)csi_kernel_malloc(sizeof(KBState)*ConfigData.NumOfKBStates, NULL);
	if (gKBState==NULL && ConfigData.NumOfKBStates!=0)
	{
		ERROR_PRINT("kb state Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	memset(gKBState, 0, sizeof(KBState) * ConfigData.NumOfKBStates);

	/* Malloc memory for Menu state. */
	gMenuState = (MenuState*)csi_kernel_malloc(sizeof(MenuState)*ConfigData.NumOfMenuStates, NULL);
	if (gMenuState==NULL && ConfigData.NumOfMenuStates!=0)
	{
		ERROR_PRINT("Menu state Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}

	/* Malloc memory for every page background texture address. */
	gBackgroundTextureAddrPtr = (u32*)csi_kernel_malloc(sizeof(u32)*ConfigData.NumofPages, NULL);
	if(gBackgroundTextureAddrPtr == NULL && ConfigData.NumofPages != 0 )
	{
		ERROR_PRINT("BackgroundTextureAddr Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}

	/* Initial animationDuration. */
	//animationDuration = 0;

	/* Get file current read ptr. */
	#ifdef PC_SIM
	currentPagePos = ftell(fp);
	#endif
	#ifdef EMBEDDED
	currentPagePos = csi_ahmi_GetCurConfigDataAddr();
	#endif
	
	/* Find if there are video controls . */
	for(ID =0;ID<ConfigData.NumofPages;ID++)
	{
#ifdef PC_SIM
		fseek(fp,currentPagePos + sizeof(DynamicPageCfgClass) * ID,SEEK_SET);
		if(ReadPageDataFromFile(newPage) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
#endif
#ifdef EMBEDDED
		csi_ahmi_SeekReadConfigDataAddr(currentPagePos + sizeof(DynamicPageCfgClass) * ID);
		
        /* Read data from flash. */
		csi_ahmi_ReadConfigData(newPage,sizeof(DynamicPageCfgClass));
		if(newPage == NULL)
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
		}
#endif
		#ifdef CHIP_GC9002_NoHyper
		/* Init a dynamic page. */
		if(newPage->AnimationType == PAGE_ANIMATION_SWIPE_H || newPage->AnimationType == PAGE_ANIMATION_SWIPE_V)
		{
			PageSlidingSwitch_Support = 1;
		}
		#endif

		#if defined(EMBEDDED) && defined(VERSION_2) && !defined(CHIP_GC9002_NoHyper)
		if(newPage->VideoType != 0)
		{
			#if defined(CHIP_GC9002)
			int TotalSizeOfCVBSForMalloc = SizeOfCVBSBuffer * 2;//RGB565 double buffer
			#elif defined(CHIP_GC9005)
			int TotalSizeOfCVBSForMalloc = SizeOfCVBSBuffer * VIDEO_BUFFER_NUM;//RGB565 VIDEO_BUFFER_NUM buffer
			#endif
		    if(newPage->VideoType == Video_AHD)//cvbs(1), dvp(2), ahd(3), rgb(4)
		    {
                #if defined(CHIP_GC9002)
		        SizeOfCVBSBuffer = 0xE1000;//720P(1280x720): 640x720x2 (RGB565 & DownSample_Enable -> 640=1280/2) 0xE1000
		        #elif defined(CHIP_GC9005)
                SizeOfCVBSBuffer = 0x1FA400; //1080P(1920x1080): 960x1080x2 (RGB565 & DownSample_Enable -> 960=1920/2) 0x1FA400
                #endif
                #if defined(CHIP_GC9002)
		        TotalSizeOfCVBSForMalloc = SizeOfCVBSBuffer * 2;//RGB565 double buffer
                #elif defined(CHIP_GC9005)
		        TotalSizeOfCVBSForMalloc = SizeOfCVBSBuffer * VIDEO_BUFFER_NUM;//RGB565 VIDEO_BUFFER_NUM buffer
				#endif
		    }

            #if defined(CHIP_GC9005)
            if(newPage->VideoType == Video_RGB)
            {
                SizeOfCVBSBuffer = 0xBB800;  //800x480x2
                TotalSizeOfCVBSForMalloc = SizeOfCVBSBuffer * VIDEO_BUFFER_NUM;//RGB565 VIDEO_BUFFER_NUM buffer
            }
            #endif

			/* Malloc sdram space for bt656. */
			StartAddrOfCVBS0Even = (uint32_t)gcMalloc(TotalSizeOfCVBSForMalloc);
			if(StartAddrOfCVBS0Even == 0 && SizeOfCVBSBuffer != 0)
			{
				return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
			}
			StartAddrOfCVBS0Odd = StartAddrOfCVBS0Even + SizeOfCVBSBuffer/2;
			csi_ahmi_bt656_init(newPage->VideoType);
			break;
		}
		#endif
	}
	
	/* Resume file current read ptr for read page data. */
	#ifdef PC_SIM
	fseek(fp,currentPagePos,SEEK_SET);
	#endif
	#ifdef EMBEDDED
	csi_ahmi_SeekReadConfigDataAddr(currentPagePos);
	#endif
	
	#ifdef CHIP_GC9002_NoHyper
	/* Malloc memory for page. */
	if(PageSlidingSwitch_Support)
	{
		PageSpace = csi_kernel_malloc(ConfigData.DynamicPageSize * 2, NULL);
		if(PageSpace == NULL && ConfigData.DynamicPageSize != 0 )
		{
			ERROR_PRINT("PageSpace Initial failed.\r\n");
			return AHMI_FUNC_FAILURE;
		}
		else
		{
			startOfDynamicPage = (u32)PageSpace;
			endOfDynamicPage = (u32)PageSpace + ConfigData.DynamicPageSize * 2;
		}
	}
	else
	{
		PageSpace = csi_kernel_malloc(ConfigData.DynamicPageSize, NULL);
		if(PageSpace == NULL && ConfigData.DynamicPageSize != 0 )
		{
			ERROR_PRINT("PageSpace Initial failed.\r\n");
			return AHMI_FUNC_FAILURE;
		}
		else
		{
			startOfDynamicPage = (u32)PageSpace;
			endOfDynamicPage = (u32)PageSpace + ConfigData.DynamicPageSize;
		}
	}
	#endif
	
	#ifdef PC_SIM
	currentPagePos = ftell(fp);
	#endif
	#ifdef EMBEDDED
	currentPagePos = csi_ahmi_GetCurConfigDataAddr();
	#endif
	
	/* Init all dynamic pages. */
	for(ID =0;ID<ConfigData.NumofPages;ID++)
	{
#ifdef PC_SIM
		fseek(fp,currentPagePos + sizeof(DynamicPageCfgClass) * ID,SEEK_SET);
		if(ReadPageDataFromFile(newPage) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
		
		PageSpace = csi_kernel_malloc(newPage->TotalSize, NULL);
		if(PageSpace == NULL && newPage->TotalSize != 0)
		{
			ERROR_PRINT("PageSpace Initial failed.\r\n");
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}

		if(ID == 0)
		{
			startOfDynamicPage = (u32)PageSpace;
		}

		if(ID == ConfigData.NumofPages - 1)
		{
			endOfDynamicPage = (u32)PageSpace + newPage->TotalSize;
		}
#endif
#ifdef EMBEDDED
		csi_ahmi_SeekReadConfigDataAddr(currentPagePos + sizeof(DynamicPageCfgClass) * ID);
        /* Read data from flash. */
		csi_ahmi_ReadConfigData(newPage,sizeof(DynamicPageCfgClass));
		if(newPage == NULL)
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
		}
		newPage->StoreOffset += StartOfConfigdata;
		
		#ifndef CHIP_GC9002_NoHyper
		PageSpace = (void*)gcMalloc(newPage->TotalSize);
		if(PageSpace == NULL && newPage->TotalSize != 0)
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}

		if(ID == 0)
		{
			startOfDynamicPage = (u32)PageSpace;
		}

		if(ID == ConfigData.NumofPages - 1)
		{
			endOfDynamicPage = (u32)PageSpace + newPage->TotalSize;
		}
		#endif
#endif
		
		baseaddr = newPage->StoreOffset;
		totalsize = newPage->TotalSize;

		do{
#ifdef PC_SIM
			fseek(fp,baseaddr,SEEK_SET);

			if (totalsize >ConfigData.DynamicPageSize)  
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularInit | EM_ErrRetInit_DataOverrun);
			}

			count = fread_s(PageSpace,totalsize,1,totalsize,fp);
			if(count!=totalsize)
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("There is no enough data to load page.\r\n");
#endif
				return (EM_enErrRetModularInit | EM_ErrRetInit_DataOverrun);
			}
#endif
#ifdef EMBEDDED
			csi_ahmi_SeekReadConfigDataAddr(baseaddr);
	
			if (totalsize >ConfigData.DynamicPageSize) 
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularInit | EM_ErrRetInit_DataOverrun);
			}
			csi_ahmi_ReadConfigData(PageSpace,totalsize);
#endif

			checkCode = XORCheck((u32*)PageSpace, totalsize / 4);

		}while(checkCode != newPage->CheckCode);

		/* Init a dynamic page. */
		if(gPagePtr[ID].InitPage(
			 (void*)((u8*)PageSpace),
			 newPage->StoreOffset,
			 newPage->TotalSize,
			 newPage->CheckCode,
			 newPage->PageAttr,
			 newPage->NumOfWidget,
			 newPage->NumOfCanvas,
			 newPage->NumOfSubCanvas,
			 newPage->ActionInstructionsSize,
			 newPage->totalNumOfTexture,
			 newPage->NumofTouchWidget,
			 newPage->NumofGifWidget,
			 newPage->NumOfPageAction,
			 newPage->NumOfCanvasAction,
			 newPage->AnimationType,
			 newPage->enlightedWidgetID,
			 newPage->pageAnimationDuration,
			 newPage->timingFun,
			 newPage->mNumOfAnimationKeyFrames,
			 newPage->mNunOfAnimationActions,
			 newPage->VideoType,
			 newPage->screenAreas
			) != AHMI_FUNC_SUCCESS)
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
		}
		
		/* Init page background pointer. */
		if(((gPagePtr[ID].pBackgroundTexture->mTexAttr & 0xf0) == RGBA8888) || ((gPagePtr[ID].pBackgroundTexture->mTexAttr & 0xf0) == PURECOLOR))
		{
			gBackgroundTextureAddrPtr[ID] = gPagePtr[ID].pBackgroundTexture->TexAddr;
		}
		#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
		else if((gPagePtr[ID].pBackgroundTexture->mTexAttr & 0xf0) == STCRGBA888)
		#else
		else if((gPagePtr[ID].pBackgroundTexture->mTexAttr & 0xf0) == DXT5)
		#endif
		{
			gBackgroundTextureAddrPtr[ID] = gPagePtr[ID].pBackgroundTexture->TexAddr | 0x80000000;
		}
		else
		{
			gBackgroundTextureAddrPtr[ID] = 0x5A5A5A5A;
		}

#ifdef PC_SIM
		sprintf_s(text,200, "Reading %d Page:\r\nStoreOffset is %d.\r\n",
			ID,newPage->StoreOffset);
		ERROR_PRINT(text);
		WriteText(text,"Widget Number is %d.\r\nInstruction size is %d.\r\nTexture number is %d.\r\n",
			newPage->NumOfWidget,newPage->ActionInstructionsSize,newPage->totalNumOfTexture);
		ERROR_PRINT(text);
#endif
	}
	#ifdef PC_SIM
	fseek(fp,currentPagePos + sizeof(DynamicPageCfgClass) * ID,SEEK_SET);
	#endif
	#ifdef EMBEDDED
	csi_ahmi_SeekReadConfigDataAddr(currentPagePos + sizeof(DynamicPageCfgClass) * ID);
	#endif
	
	/* Init all wavefilters. */
	for(ID=0;ID<ConfigData.NumOfWaveFilter;ID++)
	{
		if(ReadWaveFilter(&gWaveFilterPtr[ID]) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadWaveFilterErr);
	}

	/* Init all wavefiltersStatus. */
	for(ID=0;ID<ConfigData.NumOfWaveFilterStatus;ID++)
	{
		if(ReadWaveFilterStatus(&gWaveFilterStatusPtr[ID]) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadWaveFilterStatusErr);
	}
	
	/* Init all Sounds. */
	for(ID=0;ID<ConfigData.NumofSound;ID++){
		if(ReadSoundFromFile(&gSoundPtr[ID]) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadSoundErr);
		
		#if defined(EMBEDDED) && defined(VERSION_2)
		SoundPtrInTextureFlash = (uint16_t*)(TEXTURE_START_ADDR_FOR_GPU + gSoundPtr[ID].mStartAddr);
		#ifdef CHIP_GC9002_NoHyper
		gSoundPtr[ID].mStartAddr = (uint32_t)csi_kernel_malloc(gSoundPtr[ID].mSize, NULL);
		#else
		gSoundPtr[ID].mStartAddr = (uint32_t)gcMalloc(gSoundPtr[ID].mSize);
		#endif
		if(gSoundPtr[ID].mStartAddr == 0 && gSoundPtr[ID].mSize != 0)
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
		SoundSpace = (uint16_t *)(gSoundPtr[ID].mStartAddr);
		if(gSoundPtr[ID].mStartAddr != 0)
		{
			for(i = 0; i < gSoundPtr[ID].mSize/2; i++)
			{
				*SoundSpace = *SoundPtrInTextureFlash;
				SoundSpace++;
				SoundPtrInTextureFlash++;
			}
		}
		else
		{
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadSoundErr);
		}
		#endif
	}
	
	/* Init all tags. */
	for(ID=0;ID<ConfigData.NumofTags;ID++)
	{
#ifdef PC_SIM
		if(ReadTagDataFromFile(&TagPtr[ID], ID) != AHMI_FUNC_SUCCESS)
#endif
#ifdef EMBEDDED
		if(ReadTagData(&TagPtr[ID], ID) != AHMI_FUNC_SUCCESS)
#endif
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadTagErr);
#ifdef PC_SIM		
		WriteText(text,"Reading %d Tag \r\nAddr is %d.\r\n",
			ID,(TagPtr+ID)->mAddr);
		ERROR_PRINT(text);
		WriteText(text,"element Link number is %d.\r\n",
			(TagPtr+ID)->mNumOfElementLinker);
		ERROR_PRINT(text);
#endif
	}

	/* Malloc memory for tag value. */
	gTagValueSpacePtr = (u8*)csi_kernel_malloc(sizeof(u8) * ConfigData.TagValueTotalSize, NULL);
	if (gTagValueSpacePtr == NULL && ConfigData.TagValueTotalSize != 0)
	{
		ERROR_PRINT("Tag Value Space Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}

	/* Init all tag value. */
	if(ReadTagValueData(gTagValueSpacePtr, ConfigData.TagValueTotalSize) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadTagErr);

	////init stringTag strings
	//gStringTagStringPtr = (StringClassPtr)pvPortMalloc(sizeof(StringClass) * numOfStringTags);
	//if (gStringTagStringPtr==NULL && numOfStringTags!=0)
	//{
	//	ERROR_PRINT("StringTag String Space Initial failed.\r\n");
	//	return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	//}

	/* init all stringsList. */
	for(ID=0;ID<ConfigData.NumofStrings;ID++)
    {	        
		if(ReadStringList(&StringListPtr[ID]) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadStringTagErr);     
	}

	/* init all strings space. */
	if (ID > 0)
	{
		gStringPtr = (u8*)csi_kernel_malloc(sizeof(u8) * (StringListPtr[ID - 1].addr + StringListPtr[ID - 1].length + 4), NULL);//string1 + 00 00 00 00 + string2 + 00 00 00 00 + string3 + ... + last string + 00 00 00 00
		if (gStringPtr == NULL)
		{
			ERROR_PRINT("Strings Space Initial failed.\r\n");
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
	}
	
	/* init all strings. */
	for(ID=0;ID<ConfigData.NumofStrings;ID++)
    {	        
		if(ReadStringData(gStringPtr + StringListPtr[ID].addr, StringListPtr[ID].length + 4) != AHMI_FUNC_SUCCESS)//string1 + 00 00 00 00 + string2 + 00 00 00 00 + string3 + ... + last string + 00 00 00 00
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadStringTagErr);       
	}


	/* init all ArrayDatas. */
	for(ID=0;ID<ConfigData.NumofArrayDatas;ID++)
    {	        
		if(ReadArrayDataList(&ArrayDataListPtr[ID]) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadArryDataErr);
	}

	//init ArrayData space
	if (ID > 0)
	{
		gArrayDataPtr = (u8*)csi_kernel_malloc(sizeof(u8) * (ArrayDataListPtr[ID - 1].addr + ArrayDataListPtr[ID - 1].length), NULL);
		if (gArrayDataPtr == NULL)
		{
			ERROR_PRINT("Array Data Space Initial failed.\r\n");
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
	}
	

	/* init all ArrayData data. */
	for(ID=0;ID<ConfigData.NumofArrayDatas;ID++)
    {	        
		if(ReadU8Data(gArrayDataPtr + ArrayDataListPtr[ID].addr, ArrayDataListPtr[ID].length) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadArryDataErr);     
	}

	/* init all font ranges. */
	for(ID=0;ID<ConfigData.NumOfFontRanges;ID++)
    {	        
		if(ReadFontRangeList(&FontRangeListPtr[ID]) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadFontRangesErr);       
	}

	//init fontRange ranges
	if (ID > 0)
	{
		gRangesPtr = (EncodingRange*)csi_kernel_malloc(sizeof(EncodingRange) * (FontRangeListPtr[ID - 1].addr + FontRangeListPtr[ID - 1].rangeNum), NULL);
		if (gRangesPtr == NULL)
		{
			ERROR_PRINT("FontRange ranges Initial failed.\r\n");
			return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
		}
	}
	

	for(ID=0;ID<ConfigData.NumOfFontRanges;ID++)
    {	        
		if(ReadFontRangeRanges(gRangesPtr + FontRangeListPtr[ID].addr, FontRangeListPtr[ID].rangeNum) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadFontRangesErr);       

	}
	
	/* Malloc memory for HintClass. */
	gHintClass = (HintClass*)csi_kernel_malloc(sizeof(HintClass)*ConfigData.NumOfHints, NULL);
	if (gHintClass == NULL && ConfigData.NumOfHints != 0)
	{
		ERROR_PRINT("Hint Class Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	
	/* init all Hints. */
	for(ID=0;ID<ConfigData.NumOfHints;ID++)
    {	        
		if(ReadHintClass(&gHintClass[ID]) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularInit | EM_ErrRetInit_ReadHintErr);
	}
	
	//init stringTag initValue
	//numOfStringTags = 0;
	//for(ID=0;ID<ConfigData.NumofTags;ID++)
	//{
	//	if (TAG_IS_STR((TagPtr+ID)->mTagType))
	//	{
	//		//set string 
	//		(TagPtr+ID)->initTagStr(numOfStringTags++);
	//	}
	//}
	
	/* Initial animation array. */
	if(gAnimationClass.initAnimationMsg() != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularInit | EM_ErrRetInit_InitAnimationMsgErr);

	/* Initial all software timers. */
	for(ID=0;ID<ConfigData.NumofTimers;ID++)
	{
		(TimerPtr+ID)->initTimer(ID);
	}
    
    /* Initial TagUpdateQueue. */
	initTagUpdateQueue(ConfigData.NumofTagUpdateQueue, ConfigData.NumofTags, TagPtr, TagUpdatePtr);
	
#ifdef PC_SIM
	#ifdef VERSION_2
    LoadTextureToSDRAM(TextureBuffer, TEXBUFSIZE, ConfigData.TotalSize);
	#endif
#endif
    
	/* Initial System Tag Value. */
	pageID = 0;
	TagPtr[SYSTEM_PAGE_TAG].updateTagValueToValueSpace(1);
	#if defined(EMBEDDED) && defined(VERSION_2)
	TagPtr[SYSTEM_CONTRAST_TAG].updateTagValueToValueSpace(DisplayContrast);
	TagPtr[SYSTEM_SATURATION_TAG].updateTagValueToValueSpace(DisplaySaturation);
	TagPtr[SYSTEM_BRIGHTNESS_TAG].updateTagValueToValueSpace(DisplayBrightness);
	TagPtr[SYSTEM_DisplayRotate_TAG].updateTagValueToValueSpace(AHMIOutputRotation*90);
	TagPtr[SYSTEM_BacklightDuration_TAG].updateTagValueToValueSpace(BacklightWaitDuration);
	TagPtr[SYSTEM_BACKLIGHT_TAG].updateTagValueToValueSpace(BacklightNormalBrightness);
	#endif
	
#ifdef EMBEDDED
    
    if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENCAPACITIVE))
    {
        /* If not calibrate the touch screen. */
        if ((flag_Calibration == 0) || (flag_Calibration == 0xFFFFFFFF))
        {
            /* Entry for calibration page,set the calibration page ID. */
            pageID = TouchCalibration_PageID;
        }
    }
#endif	/* defined(EMBEDDED) */
	
	/* Load the first page. */
    if (gPagePtr[WorkingPageID].loadPage(pageID, 1) != AHMI_FUNC_SUCCESS)
    {
    #ifdef DEBUG
        WriteText(text,"Reading page %d failure.\r\n", WorkingPageID);
    #endif
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadFirstPageErr);
    }


	/* Determine whether the watchdog is reset. */
	#if defined(EMBEDDED) && defined(VERSION_2)
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	#else
	if((*(uint32_t *)(0x40102020) & 0xFFFF0000) == 0x00800000)
	{
		setTagValue(SYSTEM_RESET_TYPE_TAG, 0);
		*(uint32_t *)(0x40102020) |= 0x00010000;
	}
	else
	{
		setTagValue(SYSTEM_RESET_TYPE_TAG, 1);
	}
	#endif
	#endif
	
	#ifndef CHIP_GC9002_NoHyper
	/* Read err log. */
	EM_vInit();
    
	/* Read store tag data. */
	SM_vInit();

	/* load store number tag data. */
	for(i = 0; i < MaxStoreTagNum; i++)
	{
		if((astStoreItem[i].u32RegID != 0xFFFFFFFF) && (astStoreItem[i].u32RegID < 0xFFFF))
		{
			currentTagID = regID2TagID(astStoreItem[i].u32RegID);
			if(currentTagID < ConfigData.NumofTags)
			{
				if((TagPtr[currentTagID].mStoreID != 0) && (TAG_IS_INT(TagPtr[currentTagID].mTagType)))
				{
					TagPtr[currentTagID].setRawValue(astStoreItem[i].u32TagValue);
					//setTagValue(currentTagID, astStoreItem[i].u32TagValue);
				}
			}
		}
	}

	/* load store string tag data. */
	for(i = 0; i < MaxStoreStringTagWordSize;)
	{
		currentStringRegID = (StoreStringTagArry[i] & 0xFFFF);
		currentStringTagLength = ((StoreStringTagArry[i] & 0xFFFF0000) >> 16);
		currentStringPos = (i + 1);

		if((currentStringRegID != 0xFFFF) && (currentStringTagLength != 0xFFFF))
		{
			currentTagID = regID2TagID(currentStringRegID);
			if(currentTagID < ConfigData.NumofTags)
			{
				if((TagPtr[currentTagID].mStoreID != 0) && (TAG_IS_STR(TagPtr[currentTagID].mTagType)))
				{
					TagPtr[currentTagID].setStringForStoreStringTag((char*)&StoreStringTagArry[currentStringPos], 1);
					//setStrTagValue(currentTagID, (char*)&StoreStringTagArry[currentStringPos]);
				}
			}
		}
		else
		{
			break;
		}
		i = (i + 1 + currentStringTagLength/4);
	}
	#endif
    
    return AHMI_FUNC_SUCCESS;
}

#ifdef EMBEDDED
/*******************************************************************************
* Function Name: DrawTouchCalibPage
* Description  : draw calibration page/five point page.
* Input        : - ScreenWidth:         width of screen
*              : - ScreenHeight:        height of screen
*              : - BackGroundColor:     color of background
*              : - PointSize:           size of point
*              : - LeftTopX:            X coordinate of LeftTop point
*              : - LeftTopY:            Y coordinate of LeftTop point
*              : - Point0Color:         color of LeftTop point
*              : - RightTopX:           X coordinate of RightTop point
*              : - RightTopY:           Y coordinate of RightTop point
*              : - Point1Color:         color of RightTop point
*              : - LeftBottumX:         X coordinate of LeftBottum point
*              : - LeftBottumY:         Y coordinate of LeftBottum point
*              : - Point2Color:         color of LeftBottum point
*              : - RightBottumX:        X coordinate of RightBottum point
*              : - RightBottumY:        Y coordinate of RightBottum point
*              : - Point3Color:         color of RightBottum point
*              : - CenterX:             X coordinate of Center point
*              : - CenterY:             Y coordinate of Center point
*              : - Point4Color:         color of Center point
* Output       : None
* Return       : None
*******************************************************************************/
void DrawFivePointPage(
    uint16_t ScreenWidth,
    uint16_t ScreenHeight,
    uint32_t BackGroundColor,
    uint16_t PointSize,
    //LeftTop position of LeftTop Point
    uint16_t LeftTopX,
    uint16_t LeftTopY,
    uint32_t Point0Color,
    //LeftTop position of RightTop Point
    uint16_t RightTopX,
    uint16_t RightTopY,
    uint32_t Point1Color,
    //LeftTop position of LeftBottum Point
    uint16_t LeftBottumX,
    uint16_t LeftBottumY,
    uint32_t Point2Color,
    //LeftTop position of RightBottum Point
    uint16_t RightBottumX,
    uint16_t RightBottumY,
    uint32_t Point3Color,
    //LeftTop position of Center Point
    uint16_t CenterX,
    uint16_t CenterY,
    uint32_t Point4Color
)
{
//	u8* sourcebufferaddr;
	u32 sourceshift = 0;
//    uint16_t rcvData;
	
	NewMatrix 	matrixTemp;
	TextureClass 	texture;

	#if defined(CHIP_GC9002)
	SBAddress = 0x0200c000;
	AHMI_WaitDrawDone();
	#endif

	texture.mTexAttr = DRAWING | PURECOLOR | NONMASKTEX | EFMATRIX | ADDRTEXTURE;
	texture.TexWidth = 0;
	texture.TexHeight = 0;
	texture.FocusedSlice = 0;
	texture.OffsetX = 0*16;
	texture.OffsetY = 0*16;
	texture.RotateAngle = (short)(0);
	texture.ShearAngleX = (short)(0);
    texture.ShearAngleY = (short)(0);
    texture.ShearAngleZX = (short)(0);
    texture.ShearAngleZY = (short)(0);
	texture.ScalerX = (short)(1<<9);
	texture.ScalerY = (short)(1<<9);
    texture.useCanvasPoint = 0;
	texture.SliceFlagsHeaderAddr = 0;
	
	texture.TexLeftTileBox = 0;
	texture.TexTopTileBox = 0;
	texture.TexRightTileBox = 0;
	texture.TexButtomTileBox = 0;
	texture.SingleSliceSize = 0;
	texture.TexAddr = 0;
	
//	sourcebufferaddr = (u8 *)SoureBufferAddr;
	
	/* background texture. */
	//***************************************
	texture.TexWidth =  ScreenWidth;
	texture.TexHeight = ScreenHeight;
	texture.OffsetX = 0;
	texture.OffsetY = 0;
	texture.TexLeftTileBox = 0;
	texture.TexTopTileBox = 0;
	texture.TexRightTileBox = ScreenWidth / TILESIZE;
	texture.TexButtomTileBox = ScreenHeight / TILESIZE;
	texture.TexAddr = BackGroundColor;
	
	matrixTemp.NewMatrix_init();
	matrixTemp.NewMatrixInv_init();
	//texture.writeSourceBuffer(&sourceshift,&matrixTemp,NULL, NULL);
	texture.WriteNewSourceBuffer(&sourceshift, &matrixTemp, &texture, NULL);
	
	/* LeftTop texture. */
	//***************************************
	texture.TexWidth =  PointSize;
	texture.TexHeight = PointSize;
	texture.OffsetX = LeftTopX << 4;
	texture.OffsetY = LeftTopY << 4;
	texture.TexLeftTileBox = LeftTopX / TILESIZE;
	texture.TexTopTileBox = LeftTopY / TILESIZE ;
	texture.TexRightTileBox  = (LeftTopX + PointSize) / TILESIZE;
	texture.TexButtomTileBox = (LeftTopY + PointSize) / TILESIZE;
	texture.TexAddr = Point0Color;

	//texture.writeSourceBuffer(&sourceshift,&matrixTemp,NULL, NULL);
	texture.WriteNewSourceBuffer(&sourceshift, &matrixTemp, &texture, NULL);
	
	/* RightTop texture. */
	//***************************************	
	texture.OffsetX = RightTopX << 4;
	texture.OffsetY = RightTopY << 4;
	texture.TexLeftTileBox = RightTopX / TILESIZE;
	texture.TexTopTileBox  = RightTopY / TILESIZE;
	texture.TexRightTileBox  = (RightTopX+PointSize) / TILESIZE;
	texture.TexButtomTileBox = (RightTopY+PointSize) / TILESIZE;
	texture.TexAddr = Point1Color;
	
	//texture.writeSourceBuffer(&sourceshift,&matrixTemp,NULL, NULL);
	texture.WriteNewSourceBuffer(&sourceshift, &matrixTemp, &texture, NULL);

	/* LeftBottum texture. */
	//***************************************	
	texture.OffsetX = LeftBottumX << 4;
	texture.OffsetY = LeftBottumY << 4;
	texture.TexLeftTileBox = LeftBottumX / TILESIZE;
	texture.TexTopTileBox  = LeftBottumY / TILESIZE;
	texture.TexRightTileBox  = (LeftBottumX+PointSize) / TILESIZE;
	texture.TexButtomTileBox = (LeftBottumY+PointSize) / TILESIZE;	
    texture.TexAddr = Point2Color;	
	
	//texture.writeSourceBuffer(&sourceshift,&matrixTemp,NULL, NULL);
	texture.WriteNewSourceBuffer(&sourceshift, &matrixTemp, &texture, NULL);

	/* RightBottum texture. */
	//***************************************	
	texture.OffsetX = RightBottumX << 4;
	texture.OffsetY = RightBottumY << 4;
	texture.TexLeftTileBox = RightBottumX / TILESIZE;
	texture.TexTopTileBox  = RightBottumY / TILESIZE;
	texture.TexRightTileBox  = (RightBottumX+PointSize) / TILESIZE;
	texture.TexButtomTileBox = (RightBottumY+PointSize) / TILESIZE;
	texture.TexAddr = Point3Color;
	
	//texture.writeSourceBuffer(&sourceshift,&matrixTemp,NULL, NULL);
	texture.WriteNewSourceBuffer(&sourceshift, &matrixTemp, &texture, NULL);

	/* Center texture. */
	//***************************************	
	texture.OffsetX = CenterX << 4;
	texture.OffsetY = CenterY << 4;
	texture.TexLeftTileBox = CenterX / TILESIZE;
	texture.TexTopTileBox  = CenterY / TILESIZE;
	texture.TexRightTileBox  = (CenterX+PointSize) / TILESIZE;
	texture.TexButtomTileBox = (CenterY+PointSize) / TILESIZE;
    texture.TexAddr = Point4Color;	
#if defined(EMBEDDED) && defined(VERSION_2)	
    texture.mTexAttr |= 0x01;	
#endif
	
	//texture.writeSourceBuffer(&sourceshift,&matrixTemp,NULL, NULL);
	texture.WriteNewSourceBuffer(&sourceshift, &matrixTemp, &texture, NULL);
	
    /* switch fram buffer and send start drawing instruction. */
    csi_ahmi_DrawForFivePoint();
}
#endif

/**
  * @brief  Return a tag's value by ID.
  * @param  ID: ID of the tag, which you want read it's value.
  * @retval The tag's value. 
  */
unsigned int getTagValue(unsigned int ID)
{
	return TagPtr[ID].getValue();
}

/**
  * @brief  Set a tag's value by ID.
  * @param  ID: ID of the tag, which you want set it's value.
  *         value:the value you want set for the tag.
  * @retval none. 
  */
void setTagValue(unsigned int ID, unsigned int value)
{
    TagPtr[ID].setValue(value,ID);
}

/**
  * @brief  Set a tag's value by RegID.
  * @param  ID: ID of the Reg, which you want set it's value.
  *         value:the value you want set for the tag.
  * @retval none. 
  */
void setRegValue(unsigned int ID, unsigned int value)
{
	unsigned int i;
	for(i = 0; i < ConfigData.NumofTags; i++)
	{
		if(TagPtr[i].mRegID == ID)
		{
            if(i == SYSTEM_PAGE_TAG)
            {
                if(value == (unsigned int)(WorkingPageID + 1))
                    return;
            }
            
			TagPtr[i].setValue(value,i);
			return;
		}
	}
}

/**
  * @brief  Get a tag's ID by RegID.
  * @param  reg_id: ID of the Reg, which binding a tag.
  * @retval The tag's ID. 
  */
unsigned short int regID2TagID(unsigned int reg_id)
{
    unsigned int i;
    for (i = 0; i < ConfigData.NumofTagUpdateQueue; i++)
    {
        if (TagUpdatePtr[i].mRegID == reg_id)
        {
            return TagUpdatePtr[i].mTagID;
        }
    }

    return 0;
}

/**
  * @brief  Set a tag's array by RegID.
  * @param  ID: ID of the Reg, which you want set it's array.
  *         arr:the array you want set for the tag.
  *         size:the total size of arr.
  * @retval none. 
  */
unsigned int setArrRegValue(unsigned int ID, char* arr, unsigned short size)
{
	unsigned int i;
	for(i = 0; i < ConfigData.NumofTags; i++)
	{
		if (TagPtr[i].mRegID == ID)
		{
			char tmpArr[480] = {0};
			int ElemByte = TagPtr[i].getElemByteLength();
			if(ElemByte == 2 || ElemByte == 4)//u16 s16 u32 s32
			{
				for(int j = 0; j < size/ElemByte; j++)
				{
					for(int k = 0; k < ElemByte/2; k++)
					{
						tmpArr[j * ElemByte + k] = arr[j * ElemByte + (ElemByte - k - 1)];
						tmpArr[j * ElemByte + (ElemByte - k - 1)] = arr[j * ElemByte + k];
					}
				}
				TagPtr[i].setArray((u8*)tmpArr, 1, size);
				return 1;
			}
			else if(ElemByte == 1)//u8 s8
			{
				TagPtr[i].setArray((u8*)arr, 1, size);
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}

/**
  * @brief  Get a tag's array by RegID.
  * @param  ID: ID of the Reg, which you want get it's array.
  *         arr:the array you want get for the tag.
  *         size:the total size of arr.
  * @retval none. 
  */
void getArrRegValue(unsigned int ID, char* arr, unsigned short size)
{
	unsigned int i,j,k;
	for(i = 0; i < ConfigData.NumofTags; i++)
	{
		if (TagPtr[i].mRegID == ID)
		{
			unsigned int ElemByte = TagPtr[i].getElemByteLength();
			unsigned int updateLength = TagPtr[i].getArrayLength();
			int value = 0;
			
			if((size < ElemByte) || (arr == 0))
				return;

			if(updateLength > size)
				updateLength = size;
			if((updateLength % ElemByte) != 0)
				updateLength -= ElemByte;

			updateLength = updateLength / ElemByte;
			for(j = 0; j < updateLength; j++)
			{
				value = TagPtr[i].getArrayS32Value(j);
				for(k = 0; k < ElemByte; k++)
				{
					arr[ElemByte * j + k] = (value >> (8 * k));
				}
			}
			return;
		}
	}
}

/**
  * @brief  Set a tag's string by RegID.
  * @param  ID: ID of the Reg, which you want set it's string.
  *         str:the string you want set for the tag.
  * @retval none. 
  */
unsigned int setStrRegValue(unsigned int ID, char* str, unsigned short size)
{
	unsigned int i;
	for(i = 0; i < ConfigData.NumofTags; i++)
	{
		if (TagPtr[i].mRegID == ID)
		{
			if(TAG_IS_STR(TagPtr[i].mTagType))
			{
				TagPtr[i].setString(str, size, 1);
				return 1;
			}
			else if(TAG_IS_ARR(TagPtr[i].mTagType))
			{
				return setArrRegValue(ID, str, size);
			}
			else
			{

            }
		}
	}
    
    return 0;
}

/**
  * @brief  Set a tag's string by RegID.
  * @param  ID: ID of the Reg, which you want set it's string.
  *         str:the string you want set for the tag.
  * @retval none. 
  */
void getStrRegValue(unsigned int ID, char* str)
{
	unsigned int i;
	for(i = 0; i < ConfigData.NumofTags; i++)
	{
		if (TagPtr[i].mRegID == ID)
		{
			TagPtr[i].getString(str);
			return;
		}
	}
}

/*
  * @brief  Set a tag's string by RegID.
  * @param  ID: ID of the Reg, which you want set it's string.
  *         str:the string you want set for the tag.
  * @retval none. 
  */
void setStrTagValue(unsigned int ID, char* str)
{
    TagPtr[ID].setString(str, 1);
	return;
}

/*
  * @brief  Set a tag's string by RegID.
  * @param  ID: ID of the Reg, which you want set it's string.
  *         str:the string you want set for the tag.
  * @retval none. 
  */
void setStringTag(unsigned int ID, char* str, unsigned short size)
{
    TagPtr[ID].setString(str, size, 1);
	return;
}

extern ActionProcessorClass gAction;

void playSoundIF(u32 id)
{
    gAction.playSound(id);
}

void pauseSoundIF(u32 id)
{
    gAction.pauseSound(id);
}

void resumeSoundIF(u32 id)
{
    gAction.resumeSound(id);
}

void stopSoundIF(u32 id)
{
    gAction.stopSound(id);
}

void CopyPaste_background(unsigned int copy1_IMM, unsigned int copy2_IMM, unsigned short widgetID, unsigned int paste_IMM)
{
	unsigned short tag_ID = 0;
	
    gAction.copyBackground_1(copy1_IMM);
	gAction.copyBackground_2(copy2_IMM);
	tag_ID = regID2TagID(widgetID);
	if(tag_ID != 0)
	{
		gAction.pasteBackground(tag_ID, paste_IMM);
	}
}

#if defined(EMBEDDED) && defined(VERSION_2) 
uint32_t MallocFromGCHeap(uint32_t size)
{
	#ifndef CHIP_GC9002_NoHyper
    return (uint32_t)gcMalloc(size);
	#else
	return 0;
	#endif
}

/**
  * @brief  Return a tag's value by ID.
  * @param  ID: ID of the tag, which you want read it's value.
  * @retval The tag's value. 
  */
unsigned int GetTagValueC(unsigned int ID)
{
	return TagPtr[ID].getValue();
}

/**
  * @brief  Set a tag's value by ID.
  * @param  ID: ID of the tag, which you want set it's value.
  *         value:the value you want set for the tag.
  * @retval none. 
  */
void SetTagValueC(unsigned int ID, unsigned int value)
{
    TagPtr[ID].setValue(value,ID);
}

void SetStrTagValueC(unsigned int ID, char* str)
{
    TagPtr[ID].setString(str, 1);
	return;
}
#endif
#endif
