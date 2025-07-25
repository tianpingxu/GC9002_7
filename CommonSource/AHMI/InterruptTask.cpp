////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMIÐ¡×é³ÉÔ±
//
// Create Date:   2016/03/21
// File Name:     DynamicPageClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/08/11 by Zhou Yuzhi(ÖÜîÚÖÂ)
// Additional Comments:
//    handle the interruption
// Revision:
// Revision 1.6.2 - Modify 2019/07/16 by YuChunYing
// Additional Comments:
//   只保留一路视频输入处理代码，另外三路去掉�?
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "animationDefine.h"
#include "semphr.h"
#include "AHMICfgDefinition.h"
#include "drawImmediately_cd.h"
#include "videoClass.h"

#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi_bt656.h"
#include "player1c1.h"
#include "string.h"
#if defined(CHIP_GC9005)
#include "h264decode.h"
#endif

//extern k_sem_handle_t interuptSemaphore_cvbs;
extern TagClassPtr     		TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern u16    				WorkingPageID;

extern uint16_t CameraWidth ;
extern uint16_t CameraHeight;
extern ConfigInfoClass		ConfigData;
extern u16              	WorkingPageID; // the ID of current page
extern DynamicPageClassPtr  gPagePtr; // pointer of page

static u8 calibration_done = 0; //1:calibration done
u8 normalvideoframe = 0;

#define SAMPLE_FRAME 0            // interval of the sample frame
#define SAMPLE_RESOLUTION_FRAME 2 //after the first frame is drawn, the resolution is stable

#ifdef VIDEO_EN//enable video module
#ifdef __cplusplus
extern "C" {
#endif
extern QueueHandle_t PlayerInstructionQueue;  

funcStatus setDrawVideoTrigger(uint32_t irqFlag)
{
    PLAR_tstAHMICmd stAHMICmd;
    //static BaseType_t xHigherPriorityTaskWoken;
    
    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
    stAHMICmd.u32CmdType = enCmdType_DrawVideo;
    stAHMICmd.u32PageID = WorkingPageID;
    
    if(irqFlag != 0)
    {
        if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, 0) != 0)
        {
            return AHMI_FUNC_FAILURE;
        } 
    }
    else
    {
        if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
        {
            return AHMI_FUNC_FAILURE;
        }
    }
    
    return AHMI_FUNC_SUCCESS;
}

/**
  * @brief  CVBS refresh interrupt service function
  * @note
  * @retval None
  */
static uint16_t vedioState = 0;
static uint8_t flag_frame = 1;//1 is even and 0 is odd
static uint8_t frame_count = 0; //after the first frame is drawn, the resolution is stable
void ExtiTrigCvbsHandler( void )
{
    DynamicPageClassPtr pCurrentPage = &gPagePtr[WorkingPageID];
    static BaseType_t xHigherPriorityTaskWoken;

    if(pCurrentPage->VideoType != 0 || calibration_done == 0) //if donnot calibration, needs to start video too
    {
        vedioState = 0x01;

        //first sample the resolution
        if(frame_count < SAMPLE_RESOLUTION_FRAME) //get over the unstable frame
        {
            frame_count ++;
        }
        else if(frame_count == SAMPLE_RESOLUTION_FRAME)  //sample the resolution state
        {
            csi_ahmi_bt656_getParam(&CameraHeight, &CameraWidth);
            if(CameraHeight != 0 && CameraWidth != 0)
                frame_count ++;
            else
                calibration_done = 1; //calibration done
        }
        else   //the normal frame
        {
            //the changing buffer logic
            // condition    initial    cvbs_done     ahmi_done
            // 0             r         r
            // 1             w                       r
            // 2                       w             w
            if(vedioState & 0x1)
            {
                if(normalvideoframe < VIDEO_FIRST_FRAME_NUMBER)
                {
                    normalvideoframe++;
                }

                csi_ahmi_bt656_switchbuffer(&flag_frame, gPagePtr[WorkingPageID].VideoType);
                setDrawVideoTrigger(1);
            }//end of the normal frame
        }
//        if(interuptSemaphore_cvbs != NULL)
//        {
//            xSemaphoreGiveFromISR(interuptSemaphore_cvbs, &xHigherPriorityTaskWoken);
//        }
//        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}



/**
* the task to change the write address of cvbs data
* changes every time cvbs interrupt comes
*/
//void interuptTask(void* pvParameters)
//{
//	static BaseType_t xHigherPriorityTaskWoken; 
//	uint16_t state = 0;
//	uint8_t interupt_count0 = 0;
//	uint8_t flag_frame0 = 1;//1 is even and 0 is odd
//	uint8_t frame_count = 0; //after the first frame is drawn, the resolution is stable
//	
//	for(;;)
//	{
//		//get the video done interrupt
//		if(xSemaphoreTakeFromISR(interuptSemaphore_cvbs,  &xHigherPriorityTaskWoken) == pdTRUE)
//		{
//			//first sample the resolution
//			if(frame_count < SAMPLE_RESOLUTION_FRAME) //get over the unstable frame
//			{
//				frame_count ++;
//			}
//			else if(frame_count == SAMPLE_RESOLUTION_FRAME)  //sample the resolution state
//			{
//				csi_ahmi_bt656_getParam(&CameraHeight, &CameraWidth);
//				if(CameraHeight != 0 && CameraWidth != 0)
//				    frame_count ++;
//				else 
//					calibration_done = 1; //calibration done
//			}
//			else   //the normal frame
//			{
//			
//		    	state = vedioState;            
//		    	vedioState = 0;
//		    	//the changing buffer logic
//		    	// condition    initial    cvbs_done     ahmi_done
//		    	// 0             r         r 
//		    	// 1             w                       r
//		    	// 2                       w             w
//		    	if(state & 0x1)
//		    	{
//		    		if(interupt_count0 == SAMPLE_FRAME) //every SAMPLE_FRAME change the trible buffer
//		    		{
//		    			taskENTER_CRITICAL();
//						if(normalvideoframe < VIDEO_FIRST_FRAME_NUMBER)
//						{
//							normalvideoframe++;
//						}
//						
//						#ifdef CHIP_GC9002
//						if(gPagePtr[WorkingPageID].VideoType != 3)//cvbs(1) or dvp(2), not ahd(3)
//						#endif
//						{
//							csi_ahmi_bt656_switchbuffer(&flag_frame0, gPagePtr[WorkingPageID].VideoType);
//						}
//						setDrawVideoTrigger(0);
//		    			taskEXIT_CRITICAL();
//		    		}
//		    		interupt_count0 = (interupt_count0 + 1) % (SAMPLE_FRAME + 1);
//					
//				}//end of the normal frame	
//			}
//		}
//			vTaskDelay(2 / portTICK_RATE_MS);
//	}
//}


/**
* the task to calibration the resolution
* activate the video module at the first frame if video exists. then disable it if current page donnot have video module
*/
void calibrationTask(void* pvParameters)
{
	u8 videoExistInAllPage = 0;
	u16 i;
	
	/*judge whether video widget exist in the project*/
	for(i = 0; i < ConfigData.NumofPages; i++)
	{
		if(gPagePtr[i].VideoType != 0)
		{
			videoExistInAllPage = 1;
			break;
		}
	}
	
	if(videoExistInAllPage == 0) //no video, suspend the task
	{
		while(1)
		{
			calibration_done = 1;
			vTaskDelay(10000 / portTICK_RATE_MS);
		}
	}
	
	while(1)
	{
		/*calibration didn't start*/
		if(CameraHeight == 0 || CameraWidth == 0) 
		{
			csi_ahmi_bt656_enable();
			vTaskDelay(100 / portTICK_RATE_MS);  //judge whether the calibration done next 100 ms
		}
		/*if calibration done and current page donnot have video, then we need to close video*/
		else if(CameraHeight != 0 && CameraWidth != 0 && gPagePtr[WorkingPageID].VideoType == 0) 
		{
			csi_ahmi_bt656_disable();
			calibration_done = 1;
			vTaskDelay(10000 / portTICK_RATE_MS); //suspend the task forever
		}
		/*if calibration done and current page have video, then just suspend the task*/
		else if(CameraHeight != 0 && CameraWidth != 0 && gPagePtr[WorkingPageID].VideoType != 0) 
		{
			calibration_done = 1;
			vTaskDelay(10000 / portTICK_RATE_MS); //suspend the task forever
		}
	}
}

#ifdef __cplusplus
}
#endif

#endif
#endif

#if defined(CHIP_GC9005) || defined(PC_SIM)
#if defined(PC_SIM)
#include "player1c1.h"
#endif
#include "VideoPlayer.h"

extern u16 WorkingPageID;
extern DynamicPageClassPtr gPagePtr;
extern k_sem_handle_t startPlayH264Semaphore;
extern QueueHandle_t PlayerInstructionQueue;
extern volatile uint32_t gCurrentH264DecodePageId;
#if !defined(PC_SIM)
extern H264BaseInfoSt*  gH264BaseInfoPtr;
#else
extern uint32_t *gVideoPlayWidgetClassPtr;
#endif

#define H264_FRAME_RATE_25_TIME_INTERVAL  40  //ms

#ifdef __cplusplus
extern "C" {
#endif

void ExtiTrigH264PlayHandler(uint32_t curAHMITick)
{
#if !defined(PC_SIM)
    Check_Mp4AudioPause(WorkingPageID);

    if(gH264BaseInfoPtr == NULL)
        return ;

    if(gH264BaseInfoPtr[gCurrentH264DecodePageId].dataRamPtr == NULL)
        return ;

    if(startPlayH264Semaphore == NULL)
        return ;

    if((curAHMITick - gH264BaseInfoPtr[gCurrentH264DecodePageId].lastPlayPicAHMITickMs) >= H264_FRAME_RATE_25_TIME_INTERVAL)
    {
        gH264BaseInfoPtr[gCurrentH264DecodePageId].lastPlayPicAHMITickMs = curAHMITick;
        xSemaphoreGive(startPlayH264Semaphore);
    }
#else
    xSemaphoreGive(startPlayH264Semaphore);
#endif
}

funcStatus setDrawVideoPlayerTrigger(uint32_t pageId)
{
    PLAR_tstAHMICmd stAHMICmd;

    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
    stAHMICmd.u32CmdType = enCmdType_DrawVideoPlayer;
    stAHMICmd.u32PageID = pageId;

    if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
    {
        return AHMI_FUNC_FAILURE;
    }

    return AHMI_FUNC_SUCCESS;
}

/**
* the task to decode the h264 data
*/
void h264PlayTask(void* pvParameters)
{
    for(;;)
    {
        //get the 40ms beat interrupt
        xSemaphoreTake((QueueHandle_t)startPlayH264Semaphore, portMAX_DELAY);

        if(gCurrentH264DecodePageId != WorkingPageID)
        {
            #if !defined(PC_SIM)
            gH264BaseInfoPtr[gCurrentH264DecodePageId].decodeStopFlag = 1;
            #endif
            continue;
        }

        DynamicPageClassPtr pCurrentPage = &gPagePtr[WorkingPageID];

#if !defined(PC_SIM)
        if(gH264BaseInfoPtr[WorkingPageID].decodeStopFlag) continue;
        H264_DecodeNextFrame(WorkingPageID);
        if(gH264BaseInfoPtr[WorkingPageID].decPicture.picId == 0) continue;
        WidgetClass *wptr = (WidgetClass *)(gH264BaseInfoPtr[WorkingPageID].widgetClassPtr);
#else
        WidgetClass *wptr = (WidgetClass *)(gVideoPlayWidgetClassPtr);
#endif

        if(NULL != wptr)
        {
            VideoPlayerClass vpWidget;
            vpWidget.SetValueCtrl(pCurrentPage, wptr, NULL, 1);
            setDrawVideoPlayerTrigger(WorkingPageID);
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif


