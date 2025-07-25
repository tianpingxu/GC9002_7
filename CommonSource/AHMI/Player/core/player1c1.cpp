/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : player1c1.c
*
* Description  : This file contains the implementation of PLAR interface
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-09-23
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
//#include "CPU1_CMSDK_CM0.h"
#include "AHMIInterface.h"
#include "publicInclude.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "task.h"
#include "trace.h"

#include "player1c1.h"
//#include "ahmiv4.h"


#if defined(EMBEDDED) && defined(VERSION_2)
#include "gc90x_initial.h"
#include "gc90x_cache.h"
#endif

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "SBBatchDraw.h"
#endif

#if defined(CHIP_GC9005)
#include "HUDDraw.h"
#endif

#ifdef AHMI_CORE

#ifdef EMBEDDED
extern uint32_t Start_AHMI_tick;
extern uint32_t AHMITick;
#endif

#if defined(CHIP_GC9002)
#include "csi_ahmi.h"
extern void SetPartialDrawRefreMsg(RefreshMsg *msg);
#endif

extern ConfigInfoClass ConfigData;
extern AnimationClass  gAnimationClass;
extern SemaphoreHandle_t xSemaphoreSetValue;
extern SemaphoreHandle_t xSemaphoreSetString;

extern u16             WorkingPageID;

extern ProjectScreenOptions gScreenOptions;

k_task_handle_t PLARTaskHandle;

/* queue to receive instructions */
k_msgq_handle_t PlayerInstructionQueue = NULL;  

/* the struct of work buffer */
PLAR_tstWorkBuffer PLAR_stWorkBuf;



/**
  * @name       PLAR__i32QueueCreate
  * @brief      Initialize the Player Instruction Queue
  * @note       
  * @param      none
  * @retval     OK: create successful, other: create fail
  * @author     Zhu Jiahai
  * @date       2022-09-23
  */
int32_t PLAR__i32QueueCreate(void)
{
    int32_t res;
    
    PlayerInstructionQueue = xQueueCreate(PLAR_INSTRUCTION_QUEUE_SIZE, sizeof(PLAR_tstAHMICmd));

    if(NULL == PlayerInstructionQueue)
    {
        res = PLAR_MK_ERR_ID(enPLAR_CreateQueueErr);
        ERROR_PRINT("ERROR in PLAR Task, create instruction Queue fail");
    }
    else
    {
        res = PLAYER_OK;
    }

    return res;
}



/**
  * @name       PLAR__i32CreateTask
  * @brief      Create a task for PLAR
  * @note       
  * @param      none
  * @retval     return 0 if create task successfully
  * @author     Zhu Jiahai
  * @date       2022-09-23
  */
int32_t PLAR__i32CreateTask(void)
{
    int32_t res;
    portBASE_TYPE xReturn;
    
    xReturn = csi_kernel_task_new(PLAR_vMainTask, "PLARTask", NULL, KPRIO_LOW2, 0, NULL, PLAR_STACK_SIZE, &PLARTaskHandle); 
    //xReturn = xTaskCreate(PLAR_vMainTask, "PLARTask", PLAR_STACK_SIZE, NULL, 3, &PLARTaskHandle);
    if (xReturn != 0)
    {
        res =  PLAR_MK_ERR_ID(enPLAR_CreateTaskErr);
        ERROR_PRINT("Fail to create PLAR Task.\r\n");
    }
    else
    {
        res = PLAYER_OK;
    }

    return res;
}



int32_t PLAR__i32PreCheck(PLAR_tstAHMICmdPtr pstCommand, PLAR_tstWorkBufferPtr pstWorkbuf) 
{
    int32_t res = PLAYER_OK;

	if(PLAYER_TRUE != pstWorkbuf->u8Inited)
	{
		res =  PLAR_MK_ERR_ID(enPLAR_ModuleUninted);
		ERROR_PRINT("Player module uninited before work!\r\n");
		return res;
	}
	
    if(enPlayStsPlay == pstWorkbuf->u8PlayStatus)
    {
		/* Parameter check */
		switch(pstCommand->u32CmdType)
		{
			case enCmdType_SetValue:
			case enCmdType_SetString:
				#if defined(EMBEDDED) && defined(VERSION_2)
				#if defined(CHIP_GC9002_NoHyper)
				/* pstCommand->u32Para[0] means tagPtr */
				if((pstCommand->u32Para[0] & 0xFF000000) != 0x02000000)
				#else
				/* pstCommand->u32Para[0] means tagPtr */
				if((pstCommand->u32Para[0] & 0xF0000000) != 0x20000000)
				#endif
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdParaErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
				#endif
				#ifdef PC_SIM
				/* pstCommand->u32Para[0] means tagPtr */
				if(pstCommand->u32Para[0] == 0)
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdParaErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
				#endif
			break;
			
			case enCmdType_MouseTouch:
				int16_t x, y, type;
				
				/* get press position from pstCommand->u32Para[tenMouseTouchParaPressPoint] */
				x = ((pstCommand->u32Para[tenMouseTouchParaPressPoint] & 0xFFFF0000) >> 16);
				y = (pstCommand->u32Para[tenMouseTouchParaPressPoint] & 0x0000FFFF);
				
				if((x < 0) || (y < 0) || (x > MAX_WIDTH_AND_HEIGHT) || (y > MAX_WIDTH_AND_HEIGHT))
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdParaErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
				
				/* get release position from pstCommand->u32Para[tenMouseTouchParaReleasePoint] */
				x = ((pstCommand->u32Para[tenMouseTouchParaReleasePoint] & 0xFFFF0000) >> 16);
				y = (pstCommand->u32Para[tenMouseTouchParaReleasePoint] & 0x0000FFFF);
				
				if((x < 0) || (y < 0) || (x > MAX_WIDTH_AND_HEIGHT) || (y > MAX_WIDTH_AND_HEIGHT))
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdParaErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
				
				/* get touch event from pstCommand->u32Para[tenMouseTouchParaEvent] */
				type = (pstCommand->u32Para[tenMouseTouchParaEvent]);
				
				if((type != ACTION_MOUSE_PRESS) && (type != ACTION_MOUSE_HOLDING) && (type != ACTION_MOUSE_RELEASE))
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdParaErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
			break;
			
//			case enCmdType_AnimationTimer:
//				
//			break;
			
			case enCmdType_TriggerAnimation:
				uint8_t id, refreshtime;
			
				/* get animation id from pstCommand->u32Para[0] */
				id = pstCommand->u32Para[0];
			
				if(id >= AnimationMsg_Maxnum)
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdParaErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
			
				/* get animation refreshtime from pstCommand->u32Para[1] */
				refreshtime = pstCommand->u32Para[1];
				
				if(refreshtime != AnimationIntervalTime)
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdParaErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
			break;
			
			case enCmdType_SwitchPage:
				u16 pageid, pagecmd;
			
				/* get pagecmd from pstCommand->u32Para[0] */
				pagecmd = pstCommand->u32Para[0];
				if((pagecmd != GOTO_CODE) && (pagecmd != RET_CODE))
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdSwPageErr);
					ERROR_PRINT("AHMI Switch Page Cmd is illegal.\r\n");
				}
			
				/* get pageid from pstCommand->u32Para[1] */
				pageid = pstCommand->u32Para[1];
				if(pageid > ConfigData.NumofPages)
				{
					res =  PLAR_MK_ERR_ID(enPLAR_CmdPageIDErr);
					ERROR_PRINT("AHMI Cmd parameter is illegal.\r\n");
				}
			break;
			
			default:
				
			break;
		}
    }
    else
    {
        if(enCmdType_StartPlay != pstCommand->u32CmdType)
        {
            res =  PLAR_MK_ERR_ID(enPLAR_InvalidPlay);
            ERROR_PRINT("Play status is not at playing.\r\n");
        }
    }
    
    return res;
}


/**
  * @name       PLAR__vDoStartPlay
  * @brief      Excute starting to play
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-24
  */
void PLAR__vDoStartPlay(void)
{
    PLAR_stWorkBuf.u8PlayStatus = enPlayStsPlay;

	PLAR_stWorkBuf.u16RefreshType |= PageNormalRefreshFlag;
    PLAR_stWorkBuf.u16RefreshStatus = enRefreshStsPlay;

#if defined(CHIP_GC9005)
    xSemaphoreGive( xSemaphoreSetValue );
#endif
}


/**
  * @name       PLAR__vDoStopPlay
  * @brief      Excute stoping to play
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Ning
  * @date       2022-10-11
  */
void PLAR__vDoStopPlay(void)
{
    PLAR_stWorkBuf.u8PlayStatus = enPlayStsStop;
    PLAR_stWorkBuf.u16RefreshStatus = enRefreshStsIdle;
}


/**
  * @name       PLAR__vCmdMouseTouch
  * @brief      Command of mouse touch event
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Ning
  * @date       2022-09-27
  */
int32_t PLAR__vCmdMouseTouch(PIDPoint* pPressPoint, PIDPoint* pReleasePoint, uint32_t event, u8 screenType)
{
    int32_t res = PLAYER_OK;
    PLAR_tstAHMICmd stAHMICmd;

    if(PLAYER_TRUE != PLAR_stWorkBuf.u8Inited)
    {
        res =  PLAR_MK_ERR_ID(enPLAR_ModuleUninted);
        ERROR_PRINT("Call PLAR_vStartPlay() before module Initialization.\r\n");
    }
    else
    {
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_MouseTouch;
		stAHMICmd.u32PageID = PLAR_stWorkBuf.u16CurrentPage;
		stAHMICmd.u32Para[tenMouseTouchParaPressPoint] = ((pPressPoint->x << 16) | pPressPoint->y);
		stAHMICmd.u32Para[tenMouseTouchParaReleasePoint] = ((pReleasePoint->x << 16) | pReleasePoint->y);
		stAHMICmd.u32Para[tenMouseTouchParaEvent] = event;
		
        
		if(gScreenOptions.enable)
		{
			if (screenType)
			{
				//sub
				stAHMICmd.screenType = RefreshScreenSub;
				csi_kernel_msgq_put(PlayerInstructionQueue,&stAHMICmd,0,portMAX_DELAY);
			}
			else
			{
				stAHMICmd.screenType = RefreshScreenMain;
				csi_kernel_msgq_put(PlayerInstructionQueue,&stAHMICmd,0,portMAX_DELAY);
			}
			
			
		}
		else
		{
			csi_kernel_msgq_put(PlayerInstructionQueue,&stAHMICmd,0,portMAX_DELAY);
		}
    }

    return res;
}


/**
  * @name       PLAR__vMouseTouchProcess
  * @brief      MouseTouch process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Ning
  * @date       2022-09-27
  */
extern uint8_t MULTI_PAGE_ANIMATION_SWIPE_EXECUTING;
int32_t PLAR__vMouseTouchProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
	int32_t res = PLAYER_OK;
	uint32_t ahmiStatus = WIDGET_TOUCH_CANCEL;
	ActionTriggerClass tagtrigger;
	PIDPoint tmp_pressPoint;
	PIDPoint tmp_releasePoint;
	
	tmp_pressPoint.x = ((pstCmd->u32Para[tenMouseTouchParaPressPoint] & 0xFFFF0000) >> 16);
    tmp_pressPoint.y = (pstCmd->u32Para[tenMouseTouchParaPressPoint] & 0x0000FFFF);
	
	tmp_releasePoint.x = ((pstCmd->u32Para[tenMouseTouchParaReleasePoint] & 0xFFFF0000) >> 16);
    tmp_releasePoint.y = (pstCmd->u32Para[tenMouseTouchParaReleasePoint] & 0x0000FFFF);
	
    tagtrigger.mInputType = (pstCmd->u32Para[tenMouseTouchParaEvent]);

	if(!MULTI_PAGE_ANIMATION_SWIPE_EXECUTING)
	{
		ahmiStatus = tagtrigger.MouseTouch(&tmp_pressPoint, &tmp_releasePoint, RefreshScreenTypes(pstCmd->screenType));
		if(ahmiStatus < EM_enErrRetModularWidget)
		{
			pstWorkbuf->u16RefreshType |= ahmiStatus;
			pstWorkbuf->u16CurrentPage = WorkingPageID;
		}
	}
	
	return res;
}


/**
  * @name       PLAR__vCmdStartPlay
  * @brief      Command of starting to play
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-24
  */
int32_t PLAR_vStartPlay(void)
{
    int32_t res = PLAYER_OK;
    PLAR_tstAHMICmd stAHMICmd;

    if(PLAYER_TRUE != PLAR_stWorkBuf.u8Inited)
    {
        res =  PLAR_MK_ERR_ID(enPLAR_ModuleUninted);
        ERROR_PRINT("Call PLAR_vStartPlay() before module Initialization.\r\n");
    }
    else
    {
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_StartPlay;

        #if defined(CHIP_GC9005)
            stAHMICmd.screenType = RefreshScreenMain;
            PLAR_xQueueSendToBack(&stAHMICmd, portMAX_DELAY);

			csi_kernel_sem_wait( xSemaphoreSetValue, 1000);

            vTaskDelay(30 / portTICK_RATE_MS);
            stAHMICmd.screenType = RefreshScreenSub;
            PLAR_xQueueSendToBack(&stAHMICmd, portMAX_DELAY);
        #else
            PLAR_xQueueSendToBack(&stAHMICmd, portMAX_DELAY);
        #endif

#if defined(CHIP_GC9005)
        csi_kernel_sem_wait( xSemaphoreSetValue, 1000);
#endif
    }

    return res;
}

/**
  * @name       PLAR__vSetValueExeProcess
  * @brief      Switch page process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Chunying
  * @date       2022-10-20
  */
int32_t PLAR__vSetValueExeProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
	int32_t res = PLAYER_OK;
    uint32_t refresh = NullRefreshFlag;
    bool bNeedSyn;

    TagClassPtr pTag;
	
	pTag = (TagClassPtr)pstCmd->u32Para[0];
    refresh = (pTag->setRawValue(pstCmd->u32Para[1]) & RefreshFlagMusk);
	bNeedSyn = (bool)pstCmd->u32Para[2];
	pstWorkbuf->u16RefreshType |= refresh;
	
	if((refresh & PageNormalRefreshFlag) == PageNormalRefreshFlag)
	{
		pstWorkbuf->u16CurrentPage = WorkingPageID;
	}

	if(bNeedSyn)
    {
        xSemaphoreGive( xSemaphoreSetValue );
    }
	return res;
}

/**
  * @name       PLAR__vSetStringExeProcess
  * @brief      Switch page process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Chunying
  * @date       2022-10-20
  */
int32_t PLAR__vSetStringExeProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
	int32_t res = PLAYER_OK;
    bool bNeedSyn;

    TagClassPtr pTag;
    
	pTag = (TagClassPtr)pstCmd->u32Para[0];
	bNeedSyn = (bool)pstCmd->u32Para[1];
	pstWorkbuf->u16RefreshType |= (pTag->setBindingElement()  & RefreshFlagMusk);
	
	if(bNeedSyn)
    {
        xSemaphoreGive(xSemaphoreSetString);
    }
	return res;
}

/**
  * @name       PLAR__vSwitchPageProcess
  * @brief      Switch page process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Chunying
  * @date       2022-10-14
  */
int32_t PLAR__vAnimationProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
	int32_t res = PLAYER_OK;
    uint32_t animationType = ANIMATION_REFRESH_NULL;
	
	if(pstCmd->u32PageID != WorkingPageID)
    {
        return res;
    }
    
    animationType = gAnimationClass.getAnimationRefreshType(pstCmd->u32Para[0]);
    
    if((animationType == ANIMATION_REFRESH_WIDGET) || (animationType == ANIMATION_REFRESH_SUBCANVAS) || (animationType == ANIMATION_REFRESH_CANVAS))
    {
        pstWorkbuf->u16RefreshType |= WidgetNormalRefreshFlag;
		
		#if defined(CHIP_GC9002)	
		if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
		{
			RefreshMsg refreshMsg;
			if((animationType == ANIMATION_REFRESH_WIDGET) || (animationType == ANIMATION_REFRESH_SUBCANVAS))
			{
				refreshMsg.mElementType = animationType;
			}
			else 
			{
				refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			}
			refreshMsg.mElementPtr.wptr = gAnimationClass.getAnimationRefreshElementPtr(pstCmd->u32Para[0]);
			SetPartialDrawRefreMsg(&refreshMsg);
		}
		#endif
    }
    else if(animationType == ANIMATION_REFRESH_PAGE)
    {
		if(pstCmd->u32Para[2] < 0x64)
		{
			pstWorkbuf->u16RefreshType |= PageNormalRefreshFlag;
		}
		else
		{
			pstWorkbuf->u16RefreshType |= PageMatrixRefreshFlag;
		}
		
		#if defined(CHIP_GC9002)	
		if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
		{
			RefreshMsg refreshMsg;
			refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			SetPartialDrawRefreMsg(&refreshMsg);
		}
		#endif
    }
	else if(animationType == ANIMATION_REFRESH_NULL)
    {
        return res;
    }
    else
    {
        ERROR_PRINT("Animation refresh type error.\r\n");
        return res;
    }
    
    if(gAnimationClass.Animation_maintain(pstCmd->u32Para[0], pstCmd->u32Para[1]) != AHMI_FUNC_SUCCESS)
    {
        ERROR_PRINT("Animation execute error.\r\n");
    }
    
	return res;
}




/**
  * @name       PLAR__vSwitchPageProcess
  * @brief      Switch page process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Chunying
  * @date       2022-10-14
  */
extern ActionProcessorClass gAction;
int32_t PLAR__vSwitchPageProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
	int32_t res = PLAYER_OK;
	
	if(pstCmd->u32PageID != WorkingPageID)
    {
        return res;
    }
    
    if(pstCmd->u32Para[0] == GOTO_CODE)
    {
        gAction.GotoPageExe(pstCmd->u32Para[1], pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    else if(pstCmd->u32Para[0] == RET_CODE)
    {
        gAction.ReturnPageExe();
    }
    else
    {
		res =  PLAR_MK_ERR_ID(enPLAR_CmdSwPageErr);
        ERROR_PRINT("Switch page OpCode error.\r\n");
        return res;
    }
	
	pstWorkbuf->u16CurrentPage = WorkingPageID;
	pstWorkbuf->u16RefreshType |= PageNormalRefreshFlag;
    TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(WorkingPageID + 1);
	
	#if defined(CHIP_GC9002)	
	if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
	{
		RefreshMsg refreshMsg;
		refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
		SetPartialDrawRefreMsg(&refreshMsg);
	}
	#endif
	
	return res;
}

/**
  * @name       PLAR__vActionExeProcess
  * @brief      Action execute process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Chunying
  * @date       2022-10-14
  */
int32_t PLAR__vActionExeProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
	int32_t res = PLAYER_OK;
	
	if(pstCmd->u32PageID != WorkingPageID)
    {
        return res;
    }
    
    if(pstCmd->u32Para[0] == SET_MOVE_CODE)
    {
        gAction.SetMoveExe(pstCmd->u32Para[1] & MODIFY_VIEW_ELEMENT_MUSK, pstCmd->u32Para[1] & ACTIONEXE_CODE_MUSK, pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == SET_ZOOM_CODE)
    {
        gAction.SetZoomExe(pstCmd->u32Para[1] & MODIFY_VIEW_ELEMENT_MUSK, pstCmd->u32Para[1] & ACTIONEXE_CODE_MUSK, pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }

	else if(pstCmd->u32Para[0] == SET_SCALE_CENTER_CODE)
    {
        gAction.SetScaleCenterExe(pstCmd->u32Para[1] & MODIFY_VIEW_ELEMENT_MUSK, pstCmd->u32Para[1] & ACTIONEXE_CODE_MUSK, pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == SET_ROTATION_CENTER_CODE)
    {
        gAction.SetRotationCenterExe(pstCmd->u32Para[1] & MODIFY_VIEW_ELEMENT_MUSK, pstCmd->u32Para[1] & ACTIONEXE_CODE_MUSK, pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == SET_ROTATION_ANGLE_CODE)
    {
        gAction.SetRotationAngleExe(pstCmd->u32Para[1], pstCmd->u32Para[2]);
    }
    
    else if(pstCmd->u32Para[0] == SET_PROJECTION_CODE)
    {
        gAction.SetProjectionExe(pstCmd->u32Para[1] & MODIFY_VIEW_ELEMENT_MUSK, pstCmd->u32Para[1] & ACTIONEXE_CODE_MUSK, pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == CHANGE_MOVE_CODE)
    {
        gAction.ChangeMoveExe(pstCmd->u32Para[1], pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == CHANGE_ZOOM_CODE)
    {
        gAction.ChangeZoomExe(pstCmd->u32Para[1], pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }

	
    else if(pstCmd->u32Para[0] == CHANGE_SCALE_CENTER_CODE)
    {
        gAction.ChangeScaleCenterExe(pstCmd->u32Para[1], pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == CHANGE_ROTATION_CENTER_CODE)
    {
        gAction.ChangeRotationCenterExe(pstCmd->u32Para[1], pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == CHANGE_ROTATION_ANGLE_CODE)
    {
        gAction.ChangeRotationAngleExe(pstCmd->u32Para[1], pstCmd->u32Para[2]);
    }
    
    else if(pstCmd->u32Para[0] == CHANGE_PROJECTION_CODE)
    {
        gAction.ChangeProjectionExe(pstCmd->u32Para[1], pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == SET_COLOR_CODE)
    {
        gAction.SetColorExe(pstCmd->u32Para[1], pstCmd->u32Para[2]);
    }
    
    else if(pstCmd->u32Para[0] == SET_KB_CODE)
    {
        gAction.SetKBExe(pstCmd->u32Para[1], pstCmd->u32Para[2]);
    }
    
    else if(pstCmd->u32Para[0] == SET_KB_HINT_CODE)
    {
        gAction.SetKBHintExe(pstCmd->u32Para[1], pstCmd->u32Para[2]);
    }
    
    else if(pstCmd->u32Para[0] == PREV_KB_HINTS_CODE)
    {
        gAction.ChangeKBHintsPageExe(pstCmd->u32Para[1], pstCmd->u32Para[2], pstCmd->u32Para[3]);
    }
    
    else if(pstCmd->u32Para[0] == GET_KB_CODE)
    {
        gAction.GetKBExe(pstCmd->u32Para[1], pstCmd->u32Para[2]);
    }
    
    else
    {
		res =  PLAR_MK_ERR_ID(enPLAR_CmdActionCodeErr);
        ERROR_PRINT("Action code error.\r\n");
        return res;
    }
    
	pstWorkbuf->u16RefreshType |= WidgetNormalRefreshFlag;
	
	#if defined(CHIP_GC9002)	
	if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
	{
		RefreshMsg refreshMsg;
		refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
		SetPartialDrawRefreMsg(&refreshMsg);
	}
	#endif
    
	return res;
}

/**
  * @name       PLAR__vDrawControlProcess
  * @brief      Switch page process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Chunying
  * @date       2022-10-20
  */
int32_t PLAR__vDrawControlProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
	int32_t res = PLAYER_OK;
    
	
	return res;
}


/**
  * @name       PLAR__vMainProcess
  * @brief      Main process for PLAR
  * @note       
  * @param      pstCmd
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-24
  */
void PLAR__vMainProcess(PLAR_tstAHMICmdPtr pstCmd, PLAR_tstWorkBufferPtr pstWorkbuf)
{
    
    
    switch(pstCmd->u32CmdType)
    {
        case enCmdType_SetValue:
        {
            PLAR__vSetValueExeProcess(pstCmd, pstWorkbuf);
            break;
        }
    
        case enCmdType_SetString:
        {
            PLAR__vSetStringExeProcess(pstCmd, pstWorkbuf);
            break;
        }
    
        case enCmdType_MouseTouch:
			PLAR__vMouseTouchProcess(pstCmd, pstWorkbuf);
        break;
    
//        case enCmdType_AnimationTimer:
//    
//        break;
    
        case enCmdType_TriggerAnimation:
            PLAR__vAnimationProcess(pstCmd, pstWorkbuf);
            break;
    
        case enCmdType_StartPlay:
            
            PLAR__vDoStartPlay();
            break;
    
        case enCmdType_StopPlay:
    
			PLAR__vDoStopPlay();
            break;
    
        case enCmdType_SwitchPage:
        {
            PLAR__vSwitchPageProcess(pstCmd, pstWorkbuf);
            break;
        }
        case enCmdType_ActionExe:
        {
            PLAR__vActionExeProcess(pstCmd, pstWorkbuf);
            break;
        }
        
        case enCmdType_DrawControl:
        {
            PLAR__vDrawControlProcess(pstCmd, pstWorkbuf);
            break;
        }
    
		case enCmdType_DrawVideo:
        {
			pstWorkbuf->u16RefreshType |= WidgetNormalRefreshFlag;
//            pstWorkbuf->u16RefreshStatus = enRefreshStsPlay;
			#if defined(CHIP_GC9002)	
			if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
			{
				RefreshMsg refreshMsg;
				refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
				SetPartialDrawRefreMsg(&refreshMsg);
			}
			#endif
            break;
        }

		case enCmdType_DrawVideoPlayer:
		{
		    pstWorkbuf->u16RefreshType |= WidgetNormalRefreshFlag;
		    break;
		}

		case enCmdType_RefreshGif:
		{
			pstWorkbuf->u16RefreshType |= WidgetNormalRefreshFlag;
//			pstWorkbuf->u16RefreshStatus = enRefreshStsPlay;
			break;
		}

		case enCmdType_RefreshSnapshot:
		{
			pstWorkbuf->u16RefreshType |= WidgetNormalRefreshFlag;
//			pstWorkbuf->u16RefreshStatus = enRefreshStsPlay;
			#if defined(CHIP_GC9002)	
			if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
			{
				RefreshMsg refreshMsg;
				refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
				SetPartialDrawRefreMsg(&refreshMsg);
			}
			#endif
			break;
		}

        default:
            break;
    
    }
    
    if(pstWorkbuf->u16RefreshType != NullRefreshFlag)
    {
        pstWorkbuf->u16RefreshStatus = enRefreshStsPlay;
    }
    else
    {
        if(pstWorkbuf->u16RefreshStatus == enRefreshStsPlay)
        {
            pstWorkbuf->u16RefreshStatus = enRefreshStsLastOne;
        }
        else
        {
//            pstWorkbuf->u16RefreshStatus = enRefreshStsIdle;
        }
    }
    
}



/**
  * @name       PLAR__vRefreshProcess
  * @brief      Refresh process for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Yu Ning
  * @date       2022-09-26
  */
extern DynamicPageClassPtr  gPagePtr;
int32_t PLAR__vRefreshProcess(PLAR_tstWorkBufferPtr pstWorkbuf, RefreshScreenTypes screenType)
{
	int32_t res = PLAYER_OK;
	RefreshMsg refreshMsg;
	
	if((pstWorkbuf->u16RefreshType & PageNormalRefreshFlag) == PageNormalRefreshFlag)
	{
		/* Page Normal Refresh. */
		refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
		refreshMsg.mElementPtr.pageptr = &(gPagePtr[pstWorkbuf->u16CurrentPage]);
	}
	else if((pstWorkbuf->u16RefreshType & PageMatrixRefreshFlag) == PageMatrixRefreshFlag)
	{
		/* Page Matrix Refresh. */
		refreshMsg.mElementType = ANIMATION_MATRIXREFRESH_PAGE;
		refreshMsg.mElementPtr.pageptr = &(gPagePtr[pstWorkbuf->u16CurrentPage]);
	}
	else if((pstWorkbuf->u16RefreshType & WidgetNormalRefreshFlag) == WidgetNormalRefreshFlag)
	{
		/* Only one static refresh type. */
		refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
		refreshMsg.mElementPtr.pageptr = &(gPagePtr[pstWorkbuf->u16CurrentPage]);
	}
    else
    {
		if(pstWorkbuf->u16RefreshType != NullRefreshFlag)
		{
			res = PLAR_MK_ERR_ID(enPLAR_RefreshErr);
			ERROR_PRINT("Call PLAR__vRefreshProcess() error: Refresh type error.\r\n");
			pstWorkbuf->u16RefreshType = NullRefreshFlag;
			return res;
		}
        
		#if defined(EMBEDDED) && defined(VERSION_2)
        if((pstWorkbuf->u16RefreshStatus == enRefreshStsLastOne) || (GetDisplayCounter() <= BACKLIGHTONFRAMENUMBER))
        {
			#if defined(CHIP_GC9002)
			if(GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_NONE)
			{
				refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
			}
			else
			{
				if(GetDisplayCounter() <= BACKLIGHTONFRAMENUMBER)
				{
					refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
				}
				else
				{
					refreshMsg.mElementType = ANIMAITON_REFRESH_LAST_FRAME;
				}
			}
			#else
			refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
			#endif
            refreshMsg.mElementPtr.pageptr = &(gPagePtr[pstWorkbuf->u16CurrentPage]);
            pstWorkbuf->u16RefreshStatus = enRefreshStsIdle;
        }
		#endif
		#ifdef PC_SIM
		if(pstWorkbuf->u16RefreshStatus == enRefreshStsLastOne)
        {
            refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
            refreshMsg.mElementPtr.pageptr = &(gPagePtr[pstWorkbuf->u16CurrentPage]);
            pstWorkbuf->u16RefreshStatus = enRefreshStsIdle;
        }
		#endif
		else
		{
			return res;
		}
    }
	
	if(refreshMsg.mElementPtr.pageptr == NULL)
	{
		res = PLAR_MK_ERR_ID(enPLAR_RefreshErr);
        ERROR_PRINT("Call PLAR__vRefreshProcess() error: no element pointer given.\r\n");
	}  
	else
	{
		#if (defined(CHIP_GC9002) || defined(CHIP_GC9005)) && !defined(CHIP_GC9002_NoHyper)
		SBBatchDraw_vClearRefreshDone();
		#endif
		
		/* Calculate drawing parameters. */
		drawimmediatelyForPlayer(&refreshMsg, screenType);
		
		
		#ifdef PC_SIM
		extern HWND ViewHWND;
		extern HWND SubViewHWND;
		switch (screenType)
		{
		case RefreshScreenSub:
			InvalidateRect(SubViewHWND, NULL, FALSE);
			break;
		case RefreshScreenMain:
		default:
			InvalidateRect(ViewHWND,NULL,FALSE);
			break;
		}
		
		vTaskDelay(10 / portTICK_RATE_MS);
		#endif

		#if defined(EMBEDDED) && defined(VERSION_2)
		#if (defined(CHIP_GC9002) || defined(CHIP_GC9005)) && !defined(CHIP_GC9002_NoHyper)
		SBBatchDraw_vSetRefreshDone();
		while(SBBatchDraw_u8GetFlag())
        {
            vTaskDelay(1);
        }
		#endif
        #if defined(HUDDraw_ENABLE)
        HUDDraw_vSetRefreshDone();
//        while(HUDDraw_u8GetFlag())
//        {
//            vTaskDelay(1);
//        }
        #else
		{
			Start_AHMI_tick = AHMITick;
			/* stage of processing AHMI(GPU) Data */
			EVP_i32EventGroupSetBitsReqDraw();
		}
//		vTaskDelay(10 / portTICK_RATE_MS);
        #endif
		#endif
		
		/* Clear refreshType flag. */
		pstWorkbuf->u16RefreshType = 0;
	}
	
	return res;
}



/**
  * @name       PLAR_vInit
  * @brief      Initialization interface for PLAR
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-23
  */
void PLAR_vInit(void)
{
    int32_t res;
    
    /* variable of module initailization first */
    memset(&PLAR_stWorkBuf, 0, sizeof(PLAR_tstWorkBuffer));
    
    res = PLAR__i32QueueCreate();
    res |= PLAR__i32CreateTask();

    if(PLAYER_OK == res)
    {
        PLAR_stWorkBuf.u8Inited = PLAYER_TRUE;
        
        /* Tmp solution, start Play after initialization */
		//PLAR__vDoStartPlay(); //to do
		PLAR_vStartPlay();
    }
    else
    {
        ERROR_PRINT("Call PLAR_vInit fail.\r\n");
    }
    
}



/**
  * @name       PLAR_vMainTask
  * @brief      Task for PLAR
  * @note       
  * @param      pvParameters
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-23
  */
void PLAR_vMainTask(void* pvParameters)
{
	PLAR_tstAHMICmd stCmd;
	BaseType_t xStatus;
    
	for(;;)
	{
		xStatus=csi_kernel_msgq_get( PlayerInstructionQueue,
							(void*)&stCmd,
							portMAX_DELAY);
		if(0 != xStatus)
		{
			ERROR_PRINT("ERROR in PLAR Task, call csi_kernel_msgq_get() fail");
			continue;
		}

		#if defined(CHIP_GC9005)
		if(gScreenOptions.enable == 0 && RefreshScreenTypes(stCmd.screenType) == RefreshScreenSub)
		{
		    if(stCmd.u32CmdType == enCmdType_StartPlay || stCmd.u32Para[2])//NeedSyn
		    {
		        xSemaphoreGive( xSemaphoreSetValue );
		    }
		    continue;
		}
        #endif

		/* stage of Pre Check */
		if(PLAYER_OK == PLAR__i32PreCheck(&stCmd, &PLAR_stWorkBuf))
		{
            /* stage of processing widget para */
            PLAR__vMainProcess(&stCmd, &PLAR_stWorkBuf);

            #ifdef PC_SIM
            /* stage of processing refresh */
            PLAR__vRefreshProcess(&PLAR_stWorkBuf, RefreshScreenTypes(stCmd.screenType));
            #endif
			#if defined(EMBEDDED) && defined(VERSION_2)
//            if(gScreenOptions.enable)
//            {
//                while(AHMI_vGetGpuStatus() != enGpuStsIdle)
//                {
//                    vTaskDelay(10 / portTICK_RATE_MS);
//                }
//                /* stage of processing refresh */
//                PLAR__vRefreshProcess(&PLAR_stWorkBuf, RefreshScreenTypes(stCmd.screenType));
//            }
//            else
            {
                if(AHMI_vGetGpuStatus() == enGpuStsIdle)
                {
                    /* stage of processing refresh */
                    PLAR__vRefreshProcess(&PLAR_stWorkBuf, RefreshScreenTypes(stCmd.screenType));
                }
            }
            #endif
       }

       
	}
}





#endif	/* AHMI_CORE */

