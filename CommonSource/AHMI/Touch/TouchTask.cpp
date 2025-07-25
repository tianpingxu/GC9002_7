////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     Mouse.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "TouchTask.h"

#ifdef EMBEDDED
#include "gc90x_initial.h"
#include "AHMITouch.h"
#include "csi_ahmi_hw.h"
#endif

#include "player1c1.h"

extern DynamicPageClassPtr  gPagePtr;
//extern u16             WorkingPageID;


#ifdef PC_SIM
extern "C" QueueHandle_t   MouseQueue;
#endif

#ifdef EMBEDDED
Position ScreenCurPosition;
Position ScreenContactPosition;            //location when touch on screen
Position ScreenLeavePosition;

extern uint32_t flag_Calibration;

extern uint32_t CheckBuzzer(void);
extern void StopBuzzer(void);
extern uint32_t BuzzerStatus;
#endif

//Touch screen or mouse
PIDPoint pressPoint;
PIDPoint releasePoint;

#ifdef PC_SIM
u8 pressflagforSim = 0;
#endif

//-----------------------------
// name£º Touch_vMain
// Get touch event & Touch event exe
//  
//    @param1 void* pvParameters
// addtional comments :
//-----------------------------
extern u8 MULTI_PAGE_ANIMATION_SWIPE_EXECUTING;
void Touch_vMain(void* pvParameters)
{
	ActionTriggerClass tagtrigger;
	
#ifdef PC_SIM
	PIDState MousePtr;
 	BaseType_t xStatus;
#endif

#ifdef EMBEDDED
	uint8_t u8DelayTime = TOUCH_DELAY_TIME_DEFAULT;

    AHMITouch_SetRotateMatrix();
	
 	for(;;)
	{
        if(((GetTouchScreenType() > TOUCHSCREENOFF) && (GetTouchScreenType() < TOUCHSCREENCAPACITIVE)) &&  ((flag_Calibration == 0) || (flag_Calibration == 0xFFFFFFFF)))
        {
            vTaskDelay(10 / portTICK_RATE_MS);

            #if defined(CHIP_GC9002)
            extern k_task_handle_t TouchTaskHandle;
            vTaskPrioritySet((TaskHandle_t)TouchTaskHandle, KPRIO_LOW1);
            #endif
            /* Calibrating. */
            while(
			#ifndef TOUCH_CALIBRATION_ByUI5	
			AHMITouch_CalibrationByUI13()
			#else
			AHMITouch_CalibrationByUI5()
			#endif
			== 0);
            
            /* Goto page 0. */
			//gPagePtr[WorkingPageID].loadPage(0, 1);
			TagPtr[SYSTEM_PAGE_TAG].setValue(1, SYSTEM_PAGE_TAG);
            
            #if defined(CHIP_GC9002)
            vTaskPrioritySet((TaskHandle_t)TouchTaskHandle, KPRIO_LOW3);
            #endif
        }
        else
        {
            //#if defined(CHIP_GC9002)
            AHMITouch_vGetTouchInfo();
            u8DelayTime = 5;
            //#endif
            /* Get touch position. */
            AHMITouch_GetTouchEvent(&ScreenCurPosition, &ScreenContactPosition, &ScreenLeavePosition);
			
			/* Check whether enter debuggingPage after startup. */
			CheckWhetherEnterDebuggingPageAfterStartup(&ScreenCurPosition);
			
			/* Touch event execute. */
            if(ScreenCurPosition.event != NOTHING)
			{
				/* Judge touch event. */
                switch(ScreenCurPosition.event)
				{
                    case CLICK:
						pressPoint.x = ScreenCurPosition.pos_x;
                        pressPoint.y = ScreenCurPosition.pos_y;
                        tagtrigger.mInputType = ACTION_MOUSE_PRESS;
					
						/* Send Cmd to player queue. */
						PLAR__vCmdMouseTouch(&pressPoint, &releasePoint, tagtrigger.mInputType, 0);//0 for double screen touch feature, todo
					
//						if(!MULTI_PAGE_ANIMATION_SWIPE_EXECUTING)
//						{
//							tagtrigger.MouseTouch(&pressPoint, &releasePoint);
//						}
						csi_ahmi_InitBacklightAndTimer();
                        u8DelayTime = TOUCH_DELAY_TIME_UNDER_EVENT;
                        
                        break;
                    
                    case HOLD:                       
						releasePoint.x = ScreenCurPosition.pos_x;
                        releasePoint.y = ScreenCurPosition.pos_y;
                        tagtrigger.mInputType = ACTION_MOUSE_HOLDING;
					
						/* Send Cmd to player queue. */
						PLAR__vCmdMouseTouch(&pressPoint, &releasePoint, tagtrigger.mInputType, 0);//0 for double screen touch feature, todo
					
//						if(!MULTI_PAGE_ANIMATION_SWIPE_EXECUTING)
//						{
//							tagtrigger.MouseTouch(&pressPoint, &releasePoint);
//						}
						//csi_ahmi_InitBacklightAndTimer();
                        u8DelayTime = TOUCH_DELAY_TIME_UNDER_EVENT;

                        break;
                        
                    case RELEASE:
						releasePoint.x = ScreenLeavePosition.pos_x;
                        releasePoint.y = ScreenLeavePosition.pos_y;
                        tagtrigger.mInputType = ACTION_MOUSE_RELEASE;
					
						/* Send Cmd to player queue. */
						PLAR__vCmdMouseTouch(&pressPoint, &releasePoint, tagtrigger.mInputType, 0);//0 for double screen touch feature, todo
					
//						if(!MULTI_PAGE_ANIMATION_SWIPE_EXECUTING)
//						{
//							tagtrigger.MouseTouch(&pressPoint, &releasePoint);
//						}
						//csi_ahmi_InitBacklightAndTimer();
                        u8DelayTime = TOUCH_DELAY_TIME_UNDER_EVENT;

                        break;
						
                    default:
                        break;
                }
            }
			//if buzzer off, need check whether need to stop buzzer again
			if(BuzzerStatus == 0)
			{
				if(csi_ahmi_CheckBuzzer())
				{
					csi_ahmi_StopBuzzer();
				}
			}
        }
		
		vTaskDelay(u8DelayTime / portTICK_RATE_MS);
	}
#endif

#ifdef PC_SIM
 	for(;;)
 	{
 		xStatus=csi_kernel_msgq_get(MouseQueue,
			                  (void*)&MousePtr,
							  portMAX_DELAY
			                   );

 		if(xStatus != 0)
 		{
 			return;
 		}

		if((MousePtr.press == MOUSE_PRESS) && (pressflagforSim == 0))
		{
			pressflagforSim = 1;
			pressPoint.x = MousePtr.x;
			pressPoint.y = MousePtr.y;
			releasePoint.x = MousePtr.x;
			releasePoint.y = MousePtr.y;
			tagtrigger.mInputType= ACTION_MOUSE_PRESS;
		}
		else if((MousePtr.press == MOUSE_RELEASE) && (pressflagforSim == 2))
		{
			pressflagforSim = 0;
			releasePoint.x = MousePtr.x;
			releasePoint.y = MousePtr.y;
			tagtrigger.mInputType= ACTION_MOUSE_RELEASE;
			csi_kernel_msgq_flush(MouseQueue);
		}
		else if(MousePtr.press == MOUSE_HOLD)
		{
			if(pressflagforSim == 1)
			{
				pressflagforSim = 2;
				continue;
			}
			else
			{
				releasePoint.x = MousePtr.x;
				releasePoint.y = MousePtr.y;
				tagtrigger.mInputType= ACTION_MOUSE_HOLDING;
			}
		}
		else 
		{
			continue;
		}
 		//tagtrigger.MouseTouch(&pressPoint, &releasePoint);
		//taskYIELD();

		/* Send Cmd to player queue. */
		PLAR__vCmdMouseTouch(&pressPoint, &releasePoint, tagtrigger.mInputType, MousePtr.screenType);
  		vTaskDelay(10 / portTICK_RATE_MS);
 	}
#endif
}
