////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     ActionProcessor.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.01 - Change MORE,LESS,MUL,DIV, from unsigned to int, 20151120 by XT
// Revision 2.04 - C++, 20160321 by YuChunYing
// Additional Comments:
//    add comments by zuz 2018/10/29
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "animationDefine.h"
#include "DelayArg.h"
#include "AHMIInterface.h"
#include "drawImmediately_cd.h"
#include "WriteTexture.h"
#include "sm_c1.h"
#include "player1ce.h"
#include "player1c1.h"
#include "gif.h"
#include "semphr.h"

#if defined(EMBEDDED) && defined(VERSION_2)
//#include "gc90x_config.h"
#include "csi_ahmi_fix.h"
#include "gc90x_shmemmanage.h"
#include "gc90x_flash.h"
#include "AHMIInterface.h"
#include "gc90x_heap.h"
#include "gc90x_ahmi.h"
#include "gc90x_display.h"
#include "csi_hal_core.h"
#if defined(HUDDraw_ENABLE)
uint8_t changeFlag = 1;
#endif
#endif
#include "SdlPlayer.h"

#ifdef AHMI_CORE
extern ConfigInfoClass      ConfigData;
extern TagClassPtr  TagPtr;
extern u16            WorkingPageID;
extern DynamicPageClassPtr  gPagePtr;
extern TagClassPtr     TagPtr;
extern TimerClassPtr   TimerPtr;
extern u8 staticRefresh;
extern MyPlayer    gSDLplayer;
extern IntervalManager gIntervalManager;
extern DelayArg*	gDelayArgPtr;
extern StringClassPtr       StringListPtr;
extern u8*					gStringPtr;
extern u8*             gTagValueSpacePtr;
extern ArrayDataClassPtr ArrayDataListPtr; //pointer of ArrayData List
extern u8*               gArrayDataPtr; //start pointer of ArrayData data

extern k_task_handle_t ModBusSlaveTaskHandle;
extern k_task_handle_t ModBusMasterReadWeiteHandle;
extern k_task_handle_t ModBusMasterTaskHandle;
extern k_task_handle_t TouchTaskHandle;
extern k_msgq_handle_t PlayerInstructionQueue;

extern SemaphoreHandle_t xSemaphoreSetValue;
extern ProjectScreenOptions gScreenOptions;

#ifdef PC_SIM
extern "C" extern k_msgq_handle_t   MouseQueue;
#endif
extern k_msgq_handle_t   keyboardQueue;
extern k_msgq_handle_t   RefreshQueue;
extern k_msgq_handle_t   RefreshQueueWithoutDoubleBuffer;
extern k_msgq_handle_t   ActionInstructionQueue;
extern k_msgq_handle_t   AnimationTaskQueue;
extern k_msgq_handle_t   ModbusMasterRWTaskQueue;
#ifdef USE_MODBUS_MASTER
extern k_msgq_handle_t   ModbusMasterRWTaskQueue;
#endif

extern AnimationClass gAnimationClass;
extern StringClassPtr  StringPtr;
extern SoundClass *   gSoundPtr;

extern int gifIntervalIdx;

#ifdef PC_SIM
extern fstream logFile;
#endif

#ifdef PC_SIM
extern u8		GlobalFrameBuffer[ MAX_SCREEN_SIZE *2];
extern u8       OscilloscopeArry[SIZE_OF_SingleOscilloscope_BUF * Chart_Num_MAX];//the buffer to be displayed
extern u8       SnapshotBuffer[SIZE_OF_Snapshot_BUF];
#endif

ActionProcessorClass gAction;


//-----------------------------
// nameï¿½ï¿½ ActionProcessorTask
// handle the instruction from widget 
//  
// @param1 void* pvParameters The instruction parameter list
// addtional comments :
// none
//-----------------------------
void ActionProcessorTask(void* pvParameters)
 {
 	u8* InstructionStartAddr;
 	BaseType_t xStatus;
	gAction.mStackPtr = 0;
 	for(;;)
 	{
 		xStatus=csi_kernel_msgq_get( ActionInstructionQueue,
 							(void*)&InstructionStartAddr,
 							portMAX_DELAY);
 		if(xStatus != 0)
 		{
#if defined(CHIP_GC9005)
 		    if(xStatus == -RT_THREAD_STAT_SIGNAL)
 		    {
 		        continue;
 		    }
#endif

 			return;
 		}
		if(InstructionStartAddr == NULL || (InstructionStartAddr - gPagePtr[WorkingPageID].pActionInstructions) >= gPagePtr[WorkingPageID].mActionInstructionsSize )
		{
			ERROR_PRINT("ERROR in ActionProcessorTask: recive address out of baundary");
			continue;
		}
		else 
		{
			gAction.pPC = InstructionStartAddr;
			gAction.ActionImplement();
		}

  		taskYIELD();
 	}
 }
 
//-----------------------------
// nameï¿½ï¿½ SuspendOtherTask
// suspend all other tasks
//  
//  none
// addtional comments :
// none
//-----------------------------
void SuspendOtherTask()
{
//	vTaskSuspendAll();


	//clear queue

#ifdef PC_SIM
	csi_kernel_msgq_flush(MouseQueue);
	csi_kernel_msgq_flush(RefreshQueue);
	csi_kernel_msgq_flush(RefreshQueueWithoutDoubleBuffer);
	csi_kernel_msgq_flush(AnimationTaskQueue);
#endif
	
	csi_kernel_msgq_flush(ActionInstructionQueue);
	
#ifdef USE_MODBUS_MASTER
	csi_kernel_msgq_flush(ModbusMasterRWTaskQueue);
#endif

	//clear animation queue
	gAnimationClass.animationQueueReset();

}

//-----------------------------
// nameï¿½ï¿½ ResumeOtherTask
// resume all other tasks
//  
//  none
// addtional comments :
// none
//-----------------------------
void ResumeOtherTask()
{
//	xTaskResumeAll();

}

//-----------------------------
// nameï¿½ï¿½ ActionProcessorClass
// construct
//  
// none
// addtional comments :
// none
//-----------------------------
ActionProcessorClass::ActionProcessorClass(void)
{
	int i = 0;
	mStackPtr = 0;
	mPosOfPageParaStack = 0;
	for(i = 0; i < DEPTH; i++)
	{
		mPageStack[i] = 0;
	}
	for(i = 0; i < PARANUMBER; i++)
	{
		mPageParaStack[i] = 0;
	}
}

//-----------------------------
// nameï¿½ï¿½ ~ActionProcessorClass
// deconstruct
//  
// none
// addtional comments :
// none
//-----------------------------
ActionProcessorClass::~ActionProcessorClass(void)
{
    #ifdef PC_SIM
	logFile.close();
    #endif
}

//-----------------------------
// nameï¿½ï¿½ ActionImplement
// Ë³ï¿½ï¿½Ö´ï¿½Ð¸ï¿½ï¿½ï¿½Ö¸ï¿½ï¿½
//  
// @param1 u8* StartAddr ï¿½ï¿½Ê¼ï¿½ï¿½Ö·
// addtional comments :
// none
//-----------------------------
u32 ActionProcessorClass::ActionImplement(bool isBeforeLoadAction)
{
	DynamicPageClassPtr pCurrentPage;
	int err;
 	mStatusRegister = 0;
	
	pCurrentPage = &gPagePtr[WorkingPageID];
 	while(pPC < (pCurrentPage->pActionInstructions + pCurrentPage->mActionInstructionsSize) && (*(u16 *)pPC != 0))
 	{
 		err = this->ActionDecoder(isBeforeLoadAction);
		if(err == AHMI_FUNC_SUCCESS) return AHMI_FUNC_SUCCESS;
 		pPC++;
 	}
	if( pPC >= (pCurrentPage->pActionInstructions + pCurrentPage->mActionInstructionsSize) ) //no ending of actions
	{
		ERROR_PRINT("ERROR in ActionImplement: no ending instructions detected, the pPC is out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_LengthOverrun);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¿½ï¿½ ActionDecoder
// parse each code
//  
//  none
// addtional comments :
// none
//-----------------------------
int ActionProcessorClass::ActionDecoder(bool isBeforeLoadAction)
{
	TagClassPtr targetTag;
	u32 u32_imm;
	u8 u8_CdRun;
	u8 u8_CdRCd;
	u16 widgetID;
    u8 retflag = 0; //whether need stop action processor
	u8 retPageID = 0; //return Page ID

	u8_CdRun =  mStatusRegister ;//ban the conditional running
	                             //use it for conditional branch, if it is true, jump the next code
	u8_CdRCd =  1 ;
	mStatusRegister = 0;

	switch((*(u16 *)pPC) & 0x3FFF)
	{
	case(END_CODE):
		{
			void End();
			pPC=pPC+ActionFormat_Length-1;
			break;
		}
	case(BLANK_CODE):
		{
			pPC=pPC+ActionFormat_Length-1;
			break;
		}
	case(GOTO_CODE):
		{
			ActionFormat_TagID *GOTO_ActionFormat_TagID;
			GOTO_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			u16 pageNum = GOTO_ActionFormat_TagID->DestID;
			u8 numPara = GOTO_ActionFormat_TagID->SourceID1;
			u32 tagValue;
			widgetID = GOTO_ActionFormat_TagID->SourceID2;
			if((*(u16 *)pPC & 0x8000))
			{
				targetTag = &TagPtr->getTag(GOTO_ActionFormat_TagID->SourceID1);
				tagValue = targetTag->getValue();
				if((tagValue > 0) && (tagValue <= ConfigData.NumofPages) && ((tagValue - 1) != WorkingPageID))
				{
					pageNum = tagValue - 1;
				}
				else
				{
					return AHMI_FUNC_SUCCESS;
				}
			}
			GotoPage(u8_CdRun, u8_CdRCd, pageNum, numPara,widgetID);//
//            TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(pageNum + 1);
//			setTagValue(3,nextPage2 + 1);
			return AHMI_FUNC_SUCCESS;

		}
	case(DELAY_CODE):
		{
			ActionFormat_TagID *DELAY_ActionFormat_TagID;
			DELAY_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			u16 delayTime = DELAY_ActionFormat_TagID->DestID;
			Delay(u8_CdRun,u8_CdRCd,(u32)(pPC+ActionFormat_Length),delayTime,WorkingPageID);//next pPC 
			pPC = pPC + ActionFormat_Length - 1;
			return AHMI_FUNC_SUCCESS;
		}
	case(DELAY_TAG_CODE):
		{
			ActionFormat_TagID *DELAY_ActionFormat_TagID;
			DELAY_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			targetTag = &TagPtr->getTag(DELAY_ActionFormat_TagID->SourceID1);
			u16 delayTime = targetTag->getValue();
			Delay(u8_CdRun,u8_CdRCd,(u32)(pPC+ActionFormat_Length),delayTime,WorkingPageID);//next pPC 
			pPC = pPC + ActionFormat_Length - 1;
			return AHMI_FUNC_SUCCESS;
		}
	case(SET_POINTER_TAG_IMM_CODE):
		{
			ActionFormat_TagID *curActionFormat_TagID;
			curActionFormat_TagID = (ActionFormat_TagID *)pPC;
			u32 tagRegister = (curActionFormat_TagID->SourceID1 | (curActionFormat_TagID->SourceID2 << 16));
			SetPointerTag(u8_CdRun,u8_CdRCd,tagRegister,WorkingPageID, isBeforeLoadAction);//next pPC 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SET_POINTER_TAG_TAG_CODE):
		{
			ActionFormat_TagID *curActionFormat_TagID;
			curActionFormat_TagID = (ActionFormat_TagID *)pPC;
			targetTag = &TagPtr->getTag(curActionFormat_TagID->SourceID1);
			u16 tagRegister = targetTag->getValue();
			SetPointerTag(u8_CdRun,u8_CdRCd,tagRegister,WorkingPageID, isBeforeLoadAction);//next pPC 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(RET_CODE):
		{
			ReturnPage(u8_CdRun, u8_CdRCd, &retPageID);
//			TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(retPageID + 1);
			return AHMI_FUNC_SUCCESS;
		}
	case(SET_CODE):
		{
			ActionFormat_TagID *SET_ActionFormat_TagID;
			SET_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(SET_ActionFormat_TagID->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			SetTag(u8_CdRun,u8_CdRCd, SET_ActionFormat_TagID->DestID, SET_ActionFormat_TagID->SourceID1, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
                return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + ActionFormat_Length - 1;		
			break;			
		}					
	case(INC_CODE):			
		{			
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			IncTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;		
			break;		
		}					
	case(DEC_CODE):			
		{			
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			DecTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
                return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;			
			break;			
		}					
	case(MUL_CODE):			
		{		
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			MulTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
	case(DIV_CODE):
		{
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			DivTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;		
			break;
		}
	case(MOD_CODE):
		{
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			ModTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
                return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;		
			break;
		}
	case(AND_CODE):
		{
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			AndTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
	case(OR_CODE):
		{
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			OrTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
                return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;		
			break;
		}
	case(XOR_CODE):
		{
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			XorTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;		
			break;
		}
	case(NOT_CODE):
		{
            u16 DestTagID, SourceTagID;
			DestTagID = *(u16 *)(pPC + 2);
			SourceTagID = *(u16 *)(pPC + 4);

            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			NotTag(u8_CdRun,u8_CdRCd, DestTagID, SourceTagID, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SL_CODE):
		{
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			SLTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
	case(SR_CODE):
		{
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			SRTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		
	case(SETIMM_CODE):
		{
			ActionFormat_TagID *SETIMM_ActionFormat_Imm;
			SETIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(SETIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			SetTagImm(u8_CdRun, u8_CdRCd, SETIMM_ActionFormat_Imm->DestID, SETIMM_ActionFormat_Imm->SourceID1 | (SETIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;	
			break;
		}
		
	case(INCIMM_CODE):
		{
			ActionFormat_TagID *INCIMM_ActionFormat_Imm;
			INCIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(INCIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			IncTagImm(u8_CdRun, u8_CdRCd, INCIMM_ActionFormat_Imm->DestID, INCIMM_ActionFormat_Imm->SourceID1 | (INCIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}

	case(DECIMM_CODE):
		{
			ActionFormat_TagID *DECIMM_ActionFormat_Imm;
			DECIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DECIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			DecTagImm(u8_CdRun, u8_CdRCd, DECIMM_ActionFormat_Imm->DestID, DECIMM_ActionFormat_Imm->SourceID1 | (DECIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(MULIMM_CODE):
		{
			ActionFormat_TagID *MULIMM_ActionFormat_Imm;
			MULIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(MULIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			MulTagImm(u8_CdRun, u8_CdRCd, MULIMM_ActionFormat_Imm->DestID, MULIMM_ActionFormat_Imm->SourceID1 | (MULIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(DIVIMM_CODE):
		{
			ActionFormat_TagID *DIVIMM_ActionFormat_Imm;
			DIVIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DIVIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			DivTagImm(u8_CdRun, u8_CdRCd, DIVIMM_ActionFormat_Imm->DestID, DIVIMM_ActionFormat_Imm->SourceID1 | (DIVIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(MODIMM_CODE):
		{
			ActionFormat_TagID *MODIMM_ActionFormat_Imm;
			MODIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(MODIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			ModTagImm(u8_CdRun, u8_CdRCd, MODIMM_ActionFormat_Imm->DestID, MODIMM_ActionFormat_Imm->SourceID1 | (MODIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(ANDIMM_CODE):
		{
			ActionFormat_TagID *ANDIMM_ActionFormat_Imm;
			ANDIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(ANDIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			AndTagImm(u8_CdRun, u8_CdRCd, ANDIMM_ActionFormat_Imm->DestID, ANDIMM_ActionFormat_Imm->SourceID1 | (ANDIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(ORIMM_CODE):
		{
			ActionFormat_TagID *ORIMM_ActionFormat_Imm;
			ORIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(ORIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			OrTagImm(u8_CdRun, u8_CdRCd, ORIMM_ActionFormat_Imm->DestID, ORIMM_ActionFormat_Imm->SourceID1 | (ORIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(XORIMM_CODE):
		{
			ActionFormat_TagID *XORIMM_ActionFormat_Imm;
			XORIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(XORIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			XorTagImm(u8_CdRun, u8_CdRCd, XORIMM_ActionFormat_Imm->DestID, XORIMM_ActionFormat_Imm->SourceID1 | (XORIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SLIMM_CODE):
		{
			ActionFormat_TagID *SLIMM_ActionFormat_Imm;
			SLIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(SLIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			SLTagImm(u8_CdRun, u8_CdRCd, SLIMM_ActionFormat_Imm->DestID, SLIMM_ActionFormat_Imm->SourceID1 | (SLIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SRIMM_CODE):
		{
			ActionFormat_TagID *SRIMM_ActionFormat_Imm;
			SRIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(SRIMM_ActionFormat_Imm->DestID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			SRTagImm(u8_CdRun, u8_CdRCd, SRIMM_ActionFormat_Imm->DestID, SRIMM_ActionFormat_Imm->SourceID1 | (SRIMM_ActionFormat_Imm->SourceID2 << 16), isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
            
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHEQU_CODE):
		{
			ActionFormat_TagID *BRCHEQU_ActionFormat_TagID;
			BRCHEQU_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			BranchEQU(u8_CdRun,u8_CdRCd, BRCHEQU_ActionFormat_TagID->DestID, BRCHEQU_ActionFormat_TagID->SourceID1);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHEQUIMM_CODE):
		{
			ActionFormat_TagID *BRCHEQUIMM_ActionFormat_Imm;
			BRCHEQUIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			BranchEQUImm(u8_CdRun, u8_CdRCd, BRCHEQUIMM_ActionFormat_Imm->DestID, BRCHEQUIMM_ActionFormat_Imm->SourceID1 | (BRCHEQUIMM_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHNEQ_CODE):
		{
			ActionFormat_TagID *BRCHEQU_ActionFormat_TagID;
			BRCHEQU_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			BranchNEQ(u8_CdRun,u8_CdRCd, BRCHEQU_ActionFormat_TagID->DestID, BRCHEQU_ActionFormat_TagID->SourceID1);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHNEQIMM_CODE):
		{
			ActionFormat_TagID *BRCHEQUIMM_ActionFormat_Imm;
			BRCHEQUIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			BranchNEQImm(u8_CdRun, u8_CdRCd, BRCHEQUIMM_ActionFormat_Imm->DestID, BRCHEQUIMM_ActionFormat_Imm->SourceID1 | (BRCHEQUIMM_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHMORE_CODE):
		{
			ActionFormat_TagID *BRCHMORE_ActionFormat_TagID;
			BRCHMORE_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			BranchMore(u8_CdRun,u8_CdRCd, BRCHMORE_ActionFormat_TagID->DestID, BRCHMORE_ActionFormat_TagID->SourceID1);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHMOREIMM_CODE):
		{
			ActionFormat_TagID *BRCHMOREIMM_ActionFormat_Imm;
			BRCHMOREIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			BranchMoreImm(u8_CdRun, u8_CdRCd, BRCHMOREIMM_ActionFormat_Imm->DestID, BRCHMOREIMM_ActionFormat_Imm->SourceID1 | (BRCHMOREIMM_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHLESS_CODE):
		{
			ActionFormat_TagID *BRCHLESS_ActionFormat_TagID;
			BRCHLESS_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			BranchLess(u8_CdRun,u8_CdRCd, BRCHLESS_ActionFormat_TagID->DestID, BRCHLESS_ActionFormat_TagID->SourceID1);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BRCHLESSIMM_CODE):
		{
			ActionFormat_TagID *BRCHLESSIMM_ActionFormat_Imm;
			BRCHLESSIMM_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			BranchLessImm(u8_CdRun, u8_CdRCd, BRCHLESSIMM_ActionFormat_Imm->DestID, BRCHLESSIMM_ActionFormat_Imm->SourceID1 | (BRCHLESSIMM_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(RAND_INT_CODE):			
		{			
			u16 DestTagID;
            u8 PC_OffSet = 0;

			DestTagID = *(u16 *)(pPC + 2);
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(DestTagID == SYSTEM_PAGE_TAG)
            {
                retflag = 1;
            }
            
			RandTag(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            /* if operate SYSTEM_PAGE_TAG, need return AHMI_FUNC_SUCCESS */
            if(retflag)
            {
				return AHMI_FUNC_SUCCESS;
            }
			pPC = pPC + PC_OffSet - 1;			
			break;			
		}
	case(JUMP_CODE):
		{
			ActionFormat_TagID *JUMP_ActionFormat_TagID;
			JUMP_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			Jump(u8_CdRun,u8_CdRCd, JUMP_ActionFormat_TagID->DestID);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(RWDATA_CODE):
		{
			ActionFormat_TagID *RWDATA_ActionFormat_Imm;
			RWDATA_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			RW_DATA(u8_CdRun,u8_CdRCd, RWDATA_ActionFormat_Imm->DestID, RWDATA_ActionFormat_Imm->SourceID1 | (RWDATA_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		//timer
	case(SET_TIMER_CODE):
		{
			ActionFormat_TagID *SETTIMER_ActionFormat_Imm;
			SETTIMER_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			u32_imm = SETTIMER_ActionFormat_Imm->SourceID1 | (SETTIMER_ActionFormat_Imm->SourceID2 << 16);
			if((*(u16 *)pPC & 0x8000))
			{
				if((SETTIMER_ActionFormat_Imm->SourceID1 > 0) && (SETTIMER_ActionFormat_Imm->SourceID1 <= ConfigData.NumofTags))
				{
					targetTag = &TagPtr->getTag(SETTIMER_ActionFormat_Imm->SourceID1);
					u32_imm = targetTag->getValue();
				}
				else
				{
					ERROR_PRINT("ERROR in function SetTimer: TimerTagID index out of baundary");
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
				}
			}
			set_timer(u8_CdRun,u8_CdRCd, SETTIMER_ActionFormat_Imm->DestID, u32_imm);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SET_TIMER_TAG_CODE):
		{
			ActionFormat_TagID *SETTIMER_TAG_ActionFormat_Imm;
			SETTIMER_TAG_ActionFormat_Imm = (ActionFormat_TagID *)pPC;

			if( (SETTIMER_TAG_ActionFormat_Imm->DestID) >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetTimerTag: TimerTagID index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(SETTIMER_TAG_ActionFormat_Imm->DestID).mTagType ) )
			{
				ERROR_PRINT("ERROR in function SetTimerTag: TimerTag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
            
			set_timer(u8_CdRun,u8_CdRCd, SETTIMER_TAG_ActionFormat_Imm->DestID, SETTIMER_TAG_ActionFormat_Imm->SourceID1 | (SETTIMER_TAG_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(ANIMATION_CODE):
		{
			ActionFormat_TagID *ANIMATION_ActionFormat_Imm;
			ANIMATION_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
            #if defined(HUDDraw_ENABLE)
			if(ANIMATION_ActionFormat_Imm->SourceID1 == 0)
			{
			    if(changeFlag)
			        changeFlag = 0;
			    else
			        changeFlag = 1;
			}
            #endif
			triggerAnimation(u8_CdRun,u8_CdRCd, ANIMATION_ActionFormat_Imm->DestID, ANIMATION_ActionFormat_Imm->SourceID1 | (ANIMATION_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(ANIMATION_TAG_CODE):
		{
			ActionFormat_TagID *ANIMATION_TAG_ActionFormat_TagID;
			ANIMATION_TAG_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			triggerAnimationTag(u8_CdRun,u8_CdRCd, ANIMATION_TAG_ActionFormat_TagID->DestID, ANIMATION_TAG_ActionFormat_TagID->SourceID1);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SET_STR_CODE):
		{
			ActionFormat_TagID *SET_STR_ActionFormat_Imm;
			SET_STR_ActionFormat_Imm = (ActionFormat_TagID *)pPC;

			if((*(u16 *)pPC & 0x8000))
			{
				targetTag = &TagPtr[SET_STR_ActionFormat_Imm->DestID];
				TagClassPtr sourceTag;
				sourceTag = &TagPtr[SET_STR_ActionFormat_Imm->SourceID1];
				targetTag->setString((char*)sourceTag->getStringValue() ,1, true, isBeforeLoadAction);
			}
			else
			{
				u16 stringID = 0;
				stringID = SET_STR_ActionFormat_Imm->SourceID1;
				setTagString(u8_CdRun,u8_CdRCd, SET_STR_ActionFormat_Imm->DestID, stringID, isBeforeLoadAction);
			}
			
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SET_ARR_CODE):
		{
			ActionFormat_TagID *SET_ARR_ActionFormat_Imm;
			u16 id = 0;
			SET_ARR_ActionFormat_Imm = (ActionFormat_TagID *)pPC;

			if((*(u16 *)pPC & 0x8000))
			{
				//TODO
				//Maybe wrong
				targetTag = &TagPtr[SET_ARR_ActionFormat_Imm->SourceID1];
				TagPtr[SET_ARR_ActionFormat_Imm->DestID].setArray(targetTag->getArrayValue(), 1, targetTag->getArrayLength(), targetTag->getElemByteLength());
			}
			else
			{
				id = SET_ARR_ActionFormat_Imm->SourceID1;
				setTagArr(u8_CdRun,u8_CdRCd, SET_ARR_ActionFormat_Imm->DestID, id);
			}

			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(APPEND_ARR_CODE):			
		{			
			u16 DestTagID;
            u8 PC_OffSet = 0;
            
			AppendArray(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet);
            
            
			pPC = pPC + PC_OffSet - 1;		
			break;		
		}
	case(DEL_ARR_BY_IDX_CODE):			
		{			
			TagClassPtr targetTag;
			ActionFormat_TagID *DEL_ARR_ActionFormat_TagID;
			DEL_ARR_ActionFormat_TagID = (ActionFormat_TagID *)pPC;

			targetTag =  &TagPtr->getTag(DEL_ARR_ActionFormat_TagID->SourceID1);
			if (targetTag != NULL)
			{
				DelArrayByIdx(u8_CdRun,u8_CdRCd, DEL_ARR_ActionFormat_TagID->DestID, targetTag->getValue());
			}
			pPC = pPC + ActionFormat_Length - 1;			
			break;		
		}	
	case(DEL_ARR_BY_IDX_IMM_CODE):
		{
			ActionFormat_TagID *DEL_ARR_ActionFormat_Imm;
			DEL_ARR_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			DelArrayByIdx(u8_CdRun,u8_CdRCd, DEL_ARR_ActionFormat_Imm->DestID, DEL_ARR_ActionFormat_Imm->SourceID1 | (DEL_ARR_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(GET_ARRAY_ELEM_CODE):			
		{			
			u16 DestTagID;
            u8 PC_OffSet = 0;
            
			GetArrayElem(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;		
			break;		
		}
	case(SNAPSHOT_CODE):			
		{			
			ActionFormat_TagID *SNAPSHOT_ActionFormat_Imm;
			SNAPSHOT_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			Snapshot(SNAPSHOT_ActionFormat_Imm->SourceID1 | (SNAPSHOT_ActionFormat_Imm->SourceID2 << 16)); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(BLUR_SNAPSHOT_CODE):			
		{			
			ActionFormat_TagID *BLUR_SNAPSHOT_ActionFormat_Imm;
			BLUR_SNAPSHOT_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			BlurSnapshot(BLUR_SNAPSHOT_ActionFormat_Imm->SourceID1 | (BLUR_SNAPSHOT_ActionFormat_Imm->SourceID2 << 16)); 
			pPC = pPC + ActionFormat_Length - 1;
			break;	
		}
	case(BLUR_SNAPSHOT_TAG_CODE):
		{
			ActionFormat_TagID *BLUR_SNAPSHOT_ActionFormat_Imm;
			BLUR_SNAPSHOT_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			BlurSnapshot(TagPtr->getTag(BLUR_SNAPSHOT_ActionFormat_Imm->SourceID1).getValue()); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(CLEAR_SNAPSHOT_CODE):			
		{			
			ActionFormat_TagID *CLEAR_SNAPSHOT_ActionFormat_Imm;
			CLEAR_SNAPSHOT_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			ClearSnapshot(CLEAR_SNAPSHOT_ActionFormat_Imm->SourceID1 | (CLEAR_SNAPSHOT_ActionFormat_Imm->SourceID2 << 16)); 
			pPC = pPC + ActionFormat_Length - 1;
			break;		
		}
	case(CLEAR_DATA_CODE):
		{
			ActionFormat_TagID *CLEAR_DATA_ActionFormat_Imm;
			CLEAR_DATA_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			clearData(u8_CdRun,u8_CdRCd, CLEAR_DATA_ActionFormat_Imm->DestID); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(PLAY_SOUND_CODE):
		{
			ActionFormat_TagID *PLAY_SOUND_ActionFormat_Imm;
			PLAY_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			playSound(PLAY_SOUND_ActionFormat_Imm->SourceID1 | (PLAY_SOUND_ActionFormat_Imm->SourceID2 << 16)); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(PLAY_SOUND_TAG_CODE):
		{
			ActionFormat_TagID *PLAY_SOUND_ActionFormat_Imm;
			PLAY_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			playSound(TagPtr->getTag(PLAY_SOUND_ActionFormat_Imm->SourceID1).getValue()); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(PAUSE_SOUND_CODE):
		{
			ActionFormat_TagID *PAUSE_SOUND_ActionFormat_Imm;
			PAUSE_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			pauseSound(PAUSE_SOUND_ActionFormat_Imm->SourceID1 | (PAUSE_SOUND_ActionFormat_Imm->SourceID2 << 16));  
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(PAUSE_SOUND_TAG_CODE):
		{
			ActionFormat_TagID *PAUSE_SOUND_ActionFormat_Imm;
			PAUSE_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			pauseSound(TagPtr->getTag(PAUSE_SOUND_ActionFormat_Imm->SourceID1).getValue()); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(RESUME_SOUND_CODE):
		{
			ActionFormat_TagID *RESUME_SOUND_ActionFormat_Imm;
			RESUME_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			resumeSound(RESUME_SOUND_ActionFormat_Imm->SourceID1 | (RESUME_SOUND_ActionFormat_Imm->SourceID2 << 16)); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(RESUME_SOUND_TAG_CODE):
		{
			ActionFormat_TagID *RESUME_SOUND_ActionFormat_Imm;
			RESUME_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			resumeSound(TagPtr->getTag(RESUME_SOUND_ActionFormat_Imm->SourceID1).getValue()); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(STOP_SOUND_CODE):
		{
			ActionFormat_TagID *STOP_SOUND_ActionFormat_Imm;
			STOP_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			stopSound(STOP_SOUND_ActionFormat_Imm->SourceID1 | (STOP_SOUND_ActionFormat_Imm->SourceID2 << 16)); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(STOP_SOUND_TAG_CODE):
		{
			ActionFormat_TagID *STOP_SOUND_ActionFormat_Imm;
			STOP_SOUND_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			stopSound(TagPtr->getTag(STOP_SOUND_ActionFormat_Imm->SourceID1).getValue()); 
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(WRITE_LOG_CODE):
		{
			ActionFormat_TagID *WRITE_LOG_ActionFormat_TagID;
			WRITE_LOG_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			writeLog(u8_CdRun, u8_CdRCd, WRITE_LOG_ActionFormat_TagID->DestID, WRITE_LOG_ActionFormat_TagID->SourceID1);//DestID -> Tag1    SourceID1 -> Tag2
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(WRITE_REG_CODE):
		{
			ActionFormat_TagID *WRITE_REG_ActionFormat_Imm;
			WRITE_REG_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			writeReg(u8_CdRun,u8_CdRCd, WRITE_REG_ActionFormat_Imm->DestID, WRITE_REG_ActionFormat_Imm->SourceID1 | (WRITE_REG_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(WRITE_FLASH_CODE):
		{
			ActionFormat_TagID *WRITE_FLASH_ActionFormat_TagID;
			WRITE_FLASH_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			writeFlash(u8_CdRun, u8_CdRCd, WRITE_FLASH_ActionFormat_TagID->DestID, WRITE_FLASH_ActionFormat_TagID->SourceID1 | (WRITE_FLASH_ActionFormat_TagID->SourceID2 << 16));//DestID -> Tag1
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(READ_ERRMESSAGE_CODE):
		{
			ActionFormat_TagID *READ_ERRMESSAGE_ActionFormat_TagID;
			READ_ERRMESSAGE_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			readErrMessage(u8_CdRun, u8_CdRCd, READ_ERRMESSAGE_ActionFormat_TagID->DestID);//DestID -> Tag1
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(COPY_BACKGROUND_1_CODE):
		{
			ActionFormat_TagID *COPY_BACKGROUND_1_ActionFormat_TagID;
			COPY_BACKGROUND_1_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			copyBackground_1(COPY_BACKGROUND_1_ActionFormat_TagID->SourceID1 | (COPY_BACKGROUND_1_ActionFormat_TagID->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(COPY_BACKGROUND_2_CODE):
		{
			ActionFormat_TagID *COPY_BACKGROUND_2_ActionFormat_TagID;
			COPY_BACKGROUND_2_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			copyBackground_2(COPY_BACKGROUND_2_ActionFormat_TagID->SourceID1 | (COPY_BACKGROUND_2_ActionFormat_TagID->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(PASTE_BACKGROUND_CODE):
		{
			ActionFormat_TagID *PASTE_BACKGROUND_ActionFormat_TagID;
			PASTE_BACKGROUND_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			pasteBackground(PASTE_BACKGROUND_ActionFormat_TagID->DestID, PASTE_BACKGROUND_ActionFormat_TagID->SourceID1 | (PASTE_BACKGROUND_ActionFormat_TagID->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SET_COLOR_CODE):
		{
			TagClassPtr targetTag;
			ActionFormat_TagID *SET_COLOR_ActionFormat_TagID;
			SET_COLOR_ActionFormat_TagID = (ActionFormat_TagID *)pPC;

			targetTag =  &TagPtr->getTag(SET_COLOR_ActionFormat_TagID->SourceID1);
			if (targetTag != NULL)
			{
				SetColor(u8_CdRun,u8_CdRCd, SET_COLOR_ActionFormat_TagID->DestID, targetTag->getValue());
			}
			pPC = pPC + ActionFormat_Length - 1;	
			break;			
		}
	case(SET_COLOR_IMM_CODE):
		{
			ActionFormat_TagID *SET_COLOR_ActionFormat_Imm;
			SET_COLOR_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			SetColor(u8_CdRun,u8_CdRCd, SET_COLOR_ActionFormat_Imm->DestID, SET_COLOR_ActionFormat_Imm->SourceID1 | (SET_COLOR_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SET_KB_CODE):
		{
			TagClassPtr targetTag;
			ActionFormat_TagID *SET_KB_ActionFormat_TagID;
			SET_KB_ActionFormat_TagID = (ActionFormat_TagID *)pPC;

			targetTag =  &TagPtr->getTag(SET_KB_ActionFormat_TagID->SourceID1);
			if (targetTag != NULL)
			{
				SetKB(u8_CdRun,u8_CdRCd, SET_KB_ActionFormat_TagID->DestID, targetTag->getValue());
			}
			pPC = pPC + ActionFormat_Length - 1;	
			break;			
		}
	case(SET_KB_IMM_CODE):
		{
			ActionFormat_TagID *SET_KB_ActionFormat_Imm;
			SET_KB_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			SetKB(u8_CdRun,u8_CdRCd, SET_KB_ActionFormat_Imm->DestID, SET_KB_ActionFormat_Imm->SourceID1 | (SET_KB_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(SET_KB_HINT_CODE):
		{
			TagClassPtr targetTag;
			ActionFormat_TagID *SET_KB_HINT_ActionFormat_TagID;
			SET_KB_HINT_ActionFormat_TagID = (ActionFormat_TagID *)pPC;

			targetTag =  &TagPtr->getTag(SET_KB_HINT_ActionFormat_TagID->SourceID1);
			if (targetTag != NULL)
			{
				SetKBHint(u8_CdRun,u8_CdRCd, SET_KB_HINT_ActionFormat_TagID->DestID, targetTag->getValue());
			}
			pPC = pPC + ActionFormat_Length - 1;	
			break;			
		}
	case(SET_KB_HINT_IMM_CODE):
		{
			ActionFormat_TagID *SET_KB_HINT_ActionFormat_Imm;
			SET_KB_HINT_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			SetKBHint(u8_CdRun,u8_CdRCd, SET_KB_HINT_ActionFormat_Imm->DestID, SET_KB_HINT_ActionFormat_Imm->SourceID1 | (SET_KB_HINT_ActionFormat_Imm->SourceID2 << 16));
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(PREV_KB_HINTS_CODE):
		{
			TagClassPtr targetTag;
			ActionFormat_TagID *SET_KB_HINT_ActionFormat_TagID;
			SET_KB_HINT_ActionFormat_TagID = (ActionFormat_TagID *)pPC;

			targetTag =  &TagPtr->getTag(SET_KB_HINT_ActionFormat_TagID->SourceID1);
			if (targetTag != NULL)
			{
				ChangeKBHintsPage(u8_CdRun,u8_CdRCd, SET_KB_HINT_ActionFormat_TagID->DestID, targetTag->getValue(),0);
			}
			pPC = pPC + ActionFormat_Length - 1;	
			break;			
		}
	case(PREV_KB_HINTS_IMM_CODE):
		{
			ActionFormat_TagID *SET_KB_HINT_ActionFormat_Imm;
			SET_KB_HINT_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			ChangeKBHintsPage(u8_CdRun,u8_CdRCd, SET_KB_HINT_ActionFormat_Imm->DestID, SET_KB_HINT_ActionFormat_Imm->SourceID1 | (SET_KB_HINT_ActionFormat_Imm->SourceID2 << 16),0);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(NEXT_KB_HINTS_CODE):
		{
			TagClassPtr targetTag;
			ActionFormat_TagID *SET_KB_HINT_ActionFormat_TagID;
			SET_KB_HINT_ActionFormat_TagID = (ActionFormat_TagID *)pPC;

			targetTag =  &TagPtr->getTag(SET_KB_HINT_ActionFormat_TagID->SourceID1);
			if (targetTag != NULL)
			{
				ChangeKBHintsPage(u8_CdRun,u8_CdRCd, SET_KB_HINT_ActionFormat_TagID->DestID, targetTag->getValue(),1);
			}
			pPC = pPC + ActionFormat_Length - 1;	
			break;			
		}
	case(NEXT_KB_HINTS_IMM_CODE):
		{
			ActionFormat_TagID *SET_KB_HINT_ActionFormat_Imm;
			SET_KB_HINT_ActionFormat_Imm = (ActionFormat_TagID *)pPC;
			ChangeKBHintsPage(u8_CdRun,u8_CdRCd, SET_KB_HINT_ActionFormat_Imm->DestID, SET_KB_HINT_ActionFormat_Imm->SourceID1 | (SET_KB_HINT_ActionFormat_Imm->SourceID2 << 16),1);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	case(GET_KB_CODE):
		{
			ActionFormat_TagID *GET_KB_ActionFormat_TagID;
			GET_KB_ActionFormat_TagID = (ActionFormat_TagID *)pPC;
			GetKB(u8_CdRun,u8_CdRCd, GET_KB_ActionFormat_TagID->DestID, GET_KB_ActionFormat_TagID->SourceID1);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_MOVE_CODE):
		{
			
            u8 PC_OffSet = 0;

            
			SetMove(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;		
			break;		
		}
		case(SET_ZOOM_CODE):
		{
			u8 PC_OffSet = 0;

            
			SetZoom(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(SET_SCALE_CENTER_CODE):
		{
			u8 PC_OffSet = 0;

            
			SetScaleCenter(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(SET_ROTATION_CENTER_CODE):
		{
			u8 PC_OffSet = 0;

            
			SetRotationCenter(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(SET_ROTATION_ANGLE_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			if(isBeforeLoadAction)
			{
				SetRotationAngleExe(Action_TagID->DestID, Action_TagID->SourceID1);
			}
			else
			{
				SetRotationAngle(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1);
			}
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_PROJECTION_CODE):
		{
			u8 PC_OffSet = 0;

            
			SetProjection(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(SET_MOVE_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetMoveTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ZOOM_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetZoomTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_SCALE_CENTER_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetScaleCenterTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ROTATION_CENTER_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetRotationCenterTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ROTATION_ANGLE_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetRotationAngleTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_PROJECTION_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetProjectionTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_MOVE_CODE):
		{
			u8 PC_OffSet = 0;

            
			ChangeMove(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(CHANGE_ZOOM_CODE):
		{
			u8 PC_OffSet = 0;

            
			ChangeZoom(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(CHANGE_SCALE_CENTER_CODE):
		{
			u8 PC_OffSet = 0;

            
			ChangeScaleCenter(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(CHANGE_ROTATION_CENTER_CODE):
		{
			u8 PC_OffSet = 0;

            
			ChangeRotationCenter(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(CHANGE_ROTATION_ANGLE_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			if(isBeforeLoadAction)
			{
				ChangeRotationAngleExe(Action_TagID->DestID, Action_TagID->SourceID1);
			}
			else
			{
				ChangeRotationAngle(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1);
			}
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_PROJECTION_CODE):
		{
			u8 PC_OffSet = 0;

            
			ChangeProjection(u8_CdRun,u8_CdRCd, pPC, &PC_OffSet, isBeforeLoadAction);
            
            
			pPC = pPC + PC_OffSet - 1;	
			break;
		}
		case(CHANGE_MOVE_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeMoveTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ZOOM_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeZoomTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_SCALE_CENTER_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeScaleCenterTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ROTATION_CENTER_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeRotationCenterTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ROTATION_ANGLE_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeRotationAngleTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_PROJECTION_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeProjectionTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_MOVE_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetMoveX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_MOVE_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetMoveY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ZOOM_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetZoomX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ZOOM_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetZoomY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_SCALE_CENTER_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetScaleCenterX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_SCALE_CENTER_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetScaleCenterY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ROTATION_CENTER_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetRotationCenterX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ROTATION_CENTER_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetRotationCenterY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_PROJECTION_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetProjectionX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_PROJECTION_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetProjectionY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_MOVE_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetMoveXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_MOVE_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetMoveYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ZOOM_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetZoomXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ZOOM_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetZoomYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_SCALE_CENTER_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetScaleCenterXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_SCALE_CENTER_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetScaleCenterYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ROTATION_CENTER_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetRotationCenterXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_ROTATION_CENTER_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetRotationCenterYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_PROJECTION_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetProjectionXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(SET_PROJECTION_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			SetProjectionYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_MOVE_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeMoveX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_MOVE_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeMoveY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ZOOM_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeZoomX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ZOOM_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeZoomY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_SCALE_CENTER_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeScaleCenterX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_SCALE_CENTER_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeScaleCenterY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ROTATION_CENTER_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeRotationCenterX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ROTATION_CENTER_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeRotationCenterY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_PROJECTION_X_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeProjectionX(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_PROJECTION_Y_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeProjectionY(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_MOVE_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeMoveXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_MOVE_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeMoveYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ZOOM_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeZoomXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ZOOM_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeZoomYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ROTATION_CENTER_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeRotationCenterXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_ROTATION_CENTER_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeRotationCenterYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_PROJECTION_X_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeProjectionXTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
		case(CHANGE_PROJECTION_Y_TAG_CODE):
		{
			ActionFormat_TagID *Action_TagID;
			Action_TagID = (ActionFormat_TagID *)pPC;
			ChangeProjectionYTag(u8_CdRun,u8_CdRCd, Action_TagID->DestID, Action_TagID->SourceID1, isBeforeLoadAction);
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	default:
		{
			pPC = pPC + ActionFormat_Length - 1;
			break;
		}
	}

	return ACTIONPROCESSOR_CONTINUE;
}

		
//#if defined(EMBEDDED) && defined(VERSION_2)
//extern u32 StoreTagArry[StoreTagMaxSizeOfWord];
//#endif
//-----------------------------
// nameï¿½ï¿½ GotoPage
// jump page
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param u8 u8_page : the ID of page
// @param u8 u8_nPara : the parameters of page
// @param u16 u16_WidgetID : the ID of widget. specify the return value to which widget.
// addtional comments :
//   ï¿½ï¿½ï¿½ï¿½Ö¹Ö¸ï¿½ï¿½ï¿½Ë³ï¿½ï¿½Ö´ï¿½ï¿?
//-----------------------------
u32 ActionProcessorClass::GotoPage  (u8 u8_CdRun , u8 u8_CdRCd , u16 u8_page,u8 u8_nPara,u16 u16_WidgetID)
 {
    PLAR_tstAHMICmd stAHMICmd;
	
	//goto current page return
	if(u8_page == WorkingPageID)
	{
		return AHMI_FUNC_SUCCESS;
	}
	
	//test the input 
	if(u8_page >= ConfigData.NumofPages)
	{
		ERROR_PRINT("ERROR in gotoPage Function: page out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_GotoErrorPage);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_SwitchPage;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = GOTO_CODE;
        stAHMICmd.u32Para[1] = u8_page;
        stAHMICmd.u32Para[2] = u8_nPara;
        stAHMICmd.u32Para[3] = u16_WidgetID;

		if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
        
       
	}
	return AHMI_FUNC_SUCCESS;
}
 
u32 ActionProcessorClass::GotoPageExe(u16 u8_page,u8 u8_nPara,u16 u16_WidgetID)
 {
	// portBASE_TYPE xReturn;
	// char text[200];
	u8* actionAddr;
	DynamicPageClassPtr pageTempPtr,pCurrentPage;
	u16  pointPosRight;
	u16  numofNumber;
	u16	 signFlag;
	
	//goto current page return
	if(u8_page == WorkingPageID)
	{
		return AHMI_FUNC_SUCCESS;
	}

	//test the input 
	if(u8_page >= ConfigData.NumofPages)
	{
		ERROR_PRINT("ERROR in gotoPage Function: page out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_GotoErrorPage);
	}
    
    /* store page ID if needed */
	#if defined(EMBEDDED) && defined(VERSION_2)
	if(TagPtr->getTag(SYSTEM_PAGE_TAG).mStoreID != 0)
	{
		SM_vStoreNumberTag(SYSTEM_PAGE_TAG, u8_page + 1);
	}
	#endif

	gIntervalManager.reset();
	gifIntervalIdx = -1;

	pCurrentPage = &gPagePtr[WorkingPageID];
//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		mPageStack[mStackPtr] = WorkingPageID;
 		if(mStackPtr == DEPTH) mStackPtr = 0;
		else mStackPtr++;
 		SuspendOtherTask();
		staticRefresh = 0;
		//WriteText(text,"Goto Page %d.\r\n",page);
		//ERROR_PRINT(text);
		if(u8_nPara > 0)
		{
			if((mPosOfPageParaStack + u8_nPara) >= PARANUMBER)
				mPosOfPageParaStack = 0;
			if(u8_nPara == 10)
			{
				mPageParaStack[mPosOfPageParaStack++] = WorkingPageID;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pWidgetList[u16_WidgetID].ATTATCH_CANVAS;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pCanvasList[pCurrentPage->pWidgetList[u16_WidgetID].ATTATCH_CANVAS].mFocusedCanvas;
				mPageParaStack[mPosOfPageParaStack++] = u16_WidgetID;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pWidgetList[u16_WidgetID].WidgetAttr;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pWidgetList[u16_WidgetID].BindTagID;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pWidgetList[u16_WidgetID].MinValueL;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pWidgetList[u16_WidgetID].MinValueH;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pWidgetList[u16_WidgetID].MaxValueL;
				mPageParaStack[mPosOfPageParaStack++] = pCurrentPage->pWidgetList[u16_WidgetID].MaxValueH;
			}
		}

		pageTempPtr = gPagePtr + u8_page;
 		pageTempPtr->loadPage(u8_page);
		if(u8_nPara > 0)
		{
			if((gPagePtr[u8_page].mPageAttr == 1) && (u8_nPara == 10))
			{
				gPagePtr[u8_page].mNumOfPara = u8_nPara;
				gPagePtr[u8_page].mStartOfStack = mPosOfPageParaStack - u8_nPara;
				if((mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4] & 0x001f) == NUMBOX_TEX)
				{
					pointPosRight = (gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].WidgetAttr >> 9) & 0xf;
					signFlag = (gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].WidgetAttr >> 14) & 0x1;
					numofNumber = (gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].WidgetAttr >>5) & 0xf;
					if(pointPosRight > 0)
						numofNumber++;
					if(signFlag > 0)
						numofNumber++;
					pointPosRight = (mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4] >> 9) & 0xf;
					signFlag = (mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4] >> 14) & 0x1;
					if(pointPosRight > 0)
						numofNumber--;
					if(signFlag > 0)
						numofNumber--;

					numofNumber <<= 5;
					mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4] = mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4] & 0xfe1f;
					mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4] = mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4] | numofNumber;

					gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].WidgetAttr = mPageParaStack[gPagePtr[u8_page].mStartOfStack + 4];
					gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].MinValueL = mPageParaStack[gPagePtr[u8_page].mStartOfStack + 6];
					gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].MinValueH = mPageParaStack[gPagePtr[u8_page].mStartOfStack + 7];
					gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].MaxValueL = mPageParaStack[gPagePtr[u8_page].mStartOfStack + 8];
					gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 1].MaxValueH = mPageParaStack[gPagePtr[u8_page].mStartOfStack + 9];
				}
					
				//replace enter button set bindID
				actionAddr = (gPagePtr[u8_page].pActionInstructions + gPagePtr[u8_page].pWidgetList[gPagePtr[u8_page].mNumOfWidget - 3].OnRealeaseAction);
				*(actionAddr + 2) = (mPageParaStack[gPagePtr[u8_page].mStartOfStack + 5]);

				//replace page action set bindID
				actionAddr = (gPagePtr[u8_page].pActionStartADDR[0] + gPagePtr[u8_page].pActionInstructions);
				*(actionAddr + 4) = (mPageParaStack[gPagePtr[u8_page].mStartOfStack + 5]);
			}
		}
		
		ResumeOtherTask();
	}
	return AHMI_FUNC_SUCCESS;
}

void ActionProcessorClass::SetPointerTag(u8 u8_CdRun , u8 u8_CdRCd , u32 tagRegister, u16 workingPageId, bool isBeforeLoadAction){
	
//	DelayArg* arg;
//	int idleIdx;
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		TagPtr[SYSTEM_POINTER_TAG].setValue(tagRegister, SYSTEM_POINTER_TAG, true, isBeforeLoadAction);
	}
}

u32 ActionProcessorClass::Delay(u8 u8_CdRun , u8 u8_CdRCd , u32 instructionAddr,u16 delayTime, u16 workingPageId){
	
	DelayArg* arg;
	int idleIdx;
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
 		//SuspendOtherTask();
		idleIdx = gIntervalManager.getIDLEInterval();//get an idle position
		//
		if (idleIdx == -1)
		{
			//no idle interval available
			ERROR_PRINT("no idle status");
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NoIdleForDelay);
		}
		else
		{
			arg = gDelayArgPtr + idleIdx;
			arg->intervalID = -1;
			arg->instructionAddr = instructionAddr;
			arg->workingPageId = workingPageId;
			arg->intervalID = gIntervalManager.setInterval(idleIdx,tickDelayOP,arg,delayTime);
		}
		//ResumeOtherTask();
	}
	return AHMI_FUNC_SUCCESS;
}

void tickDelayOP(void* _arg){
	DelayArg* arg = (DelayArg*)(_arg);
	if (arg->workingPageId == WorkingPageID)
	{
		//send to back
		u32 addr = (u32)arg->instructionAddr;
		gIntervalManager.clearInterval(arg->intervalID);
		csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
	}
}


#if defined(CHIP_GC9002)
#include "csi_ahmi.h"
extern void SetPartialDrawRefreMsg(RefreshMsg *msg);
#endif

void tickGifOP(void* _arg){
	DynamicPageClassPtr pCurrentPage = &gPagePtr[WorkingPageID];
	u16 nowTime;
	GifClass gifWidget;
	u8 refresh = 0;
	for(int i = 0; i < pCurrentPage->mNumofGifWidget; i++)
	{
		WidgetClass w = pCurrentPage->pWidgetList[pCurrentPage->pGifWidgetList[i]];
		u16 widgetType = w.WidgetType;
		if (widgetType == GIF)
		{
			//gif
			nowTime = w.GifNowTime + AnimationIntervalTime;
			if (nowTime > w.GifInterval)
			{
				gifWidget.SetValueCtrl(pCurrentPage, &pCurrentPage->pWidgetList[pCurrentPage->pGifWidgetList[i]],1);
				pCurrentPage->pWidgetList[pCurrentPage->pGifWidgetList[i]].GifNowTime = 0;
				refresh = 1;
				
				#if defined(CHIP_GC9002)	
				if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
				{
					RefreshMsg refreshMsg;
					refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
					refreshMsg.mElementPtr.wptr = &pCurrentPage->pWidgetList[pCurrentPage->pGifWidgetList[i]];
					SetPartialDrawRefreMsg(&refreshMsg);
				}
				#endif
			}
			else
			{
				pCurrentPage->pWidgetList[pCurrentPage->pGifWidgetList[i]].GifNowTime = nowTime;
			}
		}
	}
	if (refresh)
	{
		PLAR_tstAHMICmd stAHMICmd;
    
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
		stAHMICmd.u32CmdType = enCmdType_RefreshGif;
		stAHMICmd.u32PageID = WorkingPageID;

		if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return;
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return;
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return;
			}
		}
	}
}

//-----------------------------
// nameï¿½ï¿½ ReturnPage
// return
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// addtional comments :
//  end the code's run
//-----------------------------
void ActionProcessorClass::ReturnPage(u8 u8_CdRun, u8 u8_CdRCd, u8 *pretPageID)
{
    PLAR_tstAHMICmd stAHMICmd;
    
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_SwitchPage;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = RET_CODE;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return;
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return;
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return;
			}
		}
	}
}

//-----------------------------
// name:ReturnPage
// return
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// addtional comments:
//  end the code's run
//-----------------------------
void ActionProcessorClass::ReturnPageExe(void)
{
	// portBASE_TYPE xReturn;
	// char text[200];
	int i;
	DynamicPageClassPtr pageTempPtr;
//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if(gPagePtr[WorkingPageID].mPageAttr != 1)
		{
			if(mStackPtr == 0) mStackPtr = DEPTH;
			else mStackPtr--;
 			SuspendOtherTask();
			pageTempPtr = gPagePtr + (u8)mPageStack[mStackPtr];
			//WriteText(text,"Return Page %d.\r\n",mPageStack[mStackPtr]);
			//ERROR_PRINT(text);
 			pageTempPtr->loadPage((u8)mPageStack[mStackPtr]);
//			*pretPageID = mPageStack[mStackPtr];
			ResumeOtherTask();
		}
		else
		{
			if(mStackPtr == 0) mStackPtr = DEPTH;
			else mStackPtr--;
			SuspendOtherTask();
			//WriteText(text,"Return Page %d.\r\n",mPageStack[mStackPtr]);
			//ERROR_PRINT(text);
//			*pretPageID = mPageStack[mStackPtr];
			pageTempPtr = gPagePtr + (mPageParaStack[gPagePtr[WorkingPageID].mStartOfStack] & 0xff);
 			pageTempPtr->loadPage(mPageParaStack[gPagePtr[WorkingPageID].mStartOfStack] & 0xff);
			ResumeOtherTask();
			for(i = 0;i < gPagePtr[WorkingPageID].mNumOfPara;i++)
			{
				mPageParaStack[mPosOfPageParaStack--] = 0;
			}
		}
	}
}

//-----------------------------
// nameï¿½ï¿½ RandTag
// set the rand value to tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
extern uint32_t AHMITick;
u32 ActionProcessorClass::RandTag(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	int Rand_Num = 0;
	s32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;
	static uint32_t Old_Rand_AHMITick = 0;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function RandTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function RandTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);
			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function RandTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function RandTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function RandTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function RandTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function RandTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function RandTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(Old_Rand_AHMITick != AHMITick)
	{
		Old_Rand_AHMITick = AHMITick;
		srand(Old_Rand_AHMITick*AHMITick);
	}
	
	if(Source1Value <= Source2Value)
	{
		Rand_Num = Source1Value + rand() % (Source2Value - Source1Value + 1);
		TagPtr->getTag(DestTagClassListTag).setValue(Rand_Num, DestTagClassListTag, true,isBeforeLoadAction);
	}
	else
	{
		Rand_Num = Source2Value + rand() % (Source1Value - Source2Value + 1);
		TagPtr->getTag(DestTagClassListTag).setValue(Rand_Num, DestTagClassListTag, true, isBeforeLoadAction);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¿½ï¿½ SetTag
// set the value of tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetTag(u8 u8_CdRun,u8 u8_CdRCd,u16 DestTagClassListTag,u16 SourceTagClassListTag, bool isBeforeLoadAction)
{
	//char text[200];
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags || SourceTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if(TAG_IS_INT(TagPtr->getTag(DestTagClassListTag).mTagType))  TagPtr->getTag(DestTagClassListTag).setValue(TagPtr->getTag(SourceTagClassListTag).getValue(), DestTagClassListTag, true, isBeforeLoadAction);
		else if((TAG_IS_STR(TagPtr->getTag(SourceTagClassListTag).mTagType)) && (TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType))) TagPtr->getTag(DestTagClassListTag).setString((char*)gTagValueSpacePtr + TagPtr->getTag(SourceTagClassListTag).mAddr, 1, true, isBeforeLoadAction);
	}
	return AHMI_FUNC_SUCCESS;
}

s32 getElemValue(u8* arr, u8 elemBytes, u8 elemSign)
{
	u32 value = 0;
	u8 j;
	for (j = 0; j < elemBytes; j++)
	{
		value = (value << 8) + arr[j];
	}
	if (elemSign)
	{
		switch (elemBytes)
		{
		case 4:
			return value;
		case 2:
			return (s16)((u16)value);
		default:
			//1
			return (s8)((u8)value);
		}
	}
	else
	{
		return (s32)value;
	}
}

int compareElem(u8* arr, u8 elemBytes, u8 elemSign, s32 value)
{
	s32 checkValue = getElemValue(arr, elemBytes, elemSign);
	
	return checkValue - value;
}

int compareElem(u8* a, u8* b, u8 elemBytes, u8 elemSign)
{
	s32 checkValue;
	s32 value;
	checkValue = getElemValue(a, elemBytes, elemSign);
	value = getElemValue(b, elemBytes, elemSign);
	return checkValue - value;
}

int assignElem(u8* arr, u8 elemBytes, u8 elemSign, s32 value)
{
	u8 i;
	u32 valueU32;
	if (elemSign)
	{
		switch (elemBytes)
		{
		case 4:
			valueU32 = (u32)value;
			break;
		case 2:
			valueU32 = (s16)value;
			break;
		default:
			//1
			valueU32 = (s8)value;
			break;
		}
	}
	else
	{
		switch (elemBytes)
		{
		case 4:
			valueU32 = (u32)value;
			break;
		case 2:
			valueU32 = (u16)value;
			break;
		default:
			//1
			valueU32 = (u8)value;
			break;
		}
	}
	for(i=0;i<elemBytes;i++)
	{
		arr[i] = (valueU32 >> (8*((elemBytes - 1)-i))) & 0xFF;
	}
	return 0;
}

int insertArr(u8* arr, u8 elemBytes, u8 elemSign, u16 *len, u16 limit, s32 value, u8 unique, u8 fromBefore, u8 sort, u8 sortMode)
{
	int i,j;
	int dupIdx = -1;
	u8 swapped = 0;
	u8 temp[4];
	int sign;
	if (unique)
	{
		for (i = 0; i < *len; i++)
		{
			if (compareElem(arr+elemBytes*i, elemBytes, elemSign, value)==0)
			{
				dupIdx = i;
				break;
			}
		}
	}
	if (dupIdx != -1)
	{
		return 0;
	}
	if ((*len)+1 > limit)
	{
		return -1;
	}
	if (fromBefore)
	{
		for (i = *len; i >= 1; i--)
		{
			//arr[i] = arr[i-1];
			memcpy(arr+i*elemBytes,arr+(i-1)*elemBytes,elemBytes);
		}
		//arr[0] = value;
		assignElem(arr, elemBytes, elemSign, value);
	}
	else
	{
		//arr[*len] = value;
		assignElem(arr + elemBytes * (*len), elemBytes, elemSign, value);
	}
	*len += 1;
	if (sort)
	{
		//bubble sort
		sign = sortMode ? 1:-1;
		for (i=(*len)-1;i>=1;i--)
		{
			swapped = 0;
			for(j=0;j<i;j++)
			{
				if (compareElem(arr+(j+1)*elemBytes,arr+j*elemBytes,elemBytes,elemSign) * sign > 0)
				{
					//swap
					memcpy(temp, arr+j*elemBytes,elemBytes);
					memcpy(arr+j*elemBytes, arr+(j+1)*elemBytes, elemBytes);
					memcpy(arr+(j+1)*elemBytes, temp, elemBytes);
					swapped = 1;
				}
			}
			if (!swapped)
			{
				break;
			}
		}
	}
	return 0;
}

int deleteArrElem(u8* arr, u8 elemBytes, u16 idx, u16* len)
{
	int i;
	if (idx >= 0 && idx < *len)
	{
		for(i=idx;i<(*len)-1;i++)
		{
			memcpy(arr + i*elemBytes, arr + (i+1)*elemBytes, elemBytes);
		}
		*len -= 1;
		return 0;
	}
	return -1;
}

u32 ActionProcessorClass::AppendArray  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;
	u8 offset;
	TagClassPtr curTag;
	u16 elemNum;
	u8 unique;
	u8 fromBefore;
	u8 sort;
	u8 sortMode;
	
	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function append array: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( !TAG_IS_ARR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function append array: tag type is not array");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);


			offset = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function append array: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( !TAG_IS_INT(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function append array: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			offset = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function append array: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( !TAG_IS_INT(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function append array: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			offset = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function append array: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( !TAG_IS_INT(TagPtr->getTag(Source1TagClassListTag).mTagType) || !TAG_IS_INT(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function append array: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			offset = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		curTag = &TagPtr->getTag(DestTagClassListTag);
		elemNum = curTag->getCurArryElemNum();
		unique = Source2Value & 1;
        fromBefore = Source2Value & 2;
        sort = Source2Value & 4;
        sortMode = Source2Value & 8;
		if (insertArr(curTag->getArrayValue(), curTag->getElemByteLength(), curTag->getElemSign(), &elemNum, curTag->getArrayLength()/curTag->getElemByteLength(),Source1Value,unique,fromBefore,sort, sortMode)!=0)
		{
			ERROR_PRINT("ERROR in function append array: insert error");
		}
		else
		{
			curTag->mCurArryElemNum = elemNum;
			curTag->setStringTrigger(curTag);
		}
		

	}
	*pOffSet = offset;

	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::DelArrayByIdx(u8 u8_CdRun,u8 u8_CdRCd,u16 tagID,u32 idx)
{    
	TagClassPtr curTag;
    PLAR_tstAHMICmd stAHMICmd;
	u16 elemNum;
	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function del array by idx: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if( !TAG_IS_ARR(TagPtr->getTag(tagID).mTagType) )
	{
		ERROR_PRINT("ERROR in function del array by idx: tag type is not array");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        curTag = &TagPtr->getTag(tagID);
		elemNum = curTag->getCurArryElemNum();
		if (deleteArrElem(curTag->getArrayValue(), curTag->getElemByteLength(), idx, & elemNum) == 0)
		{
			curTag->mCurArryElemNum = elemNum;
			curTag->setStringTrigger(curTag);
		}
	}

	return AHMI_FUNC_SUCCESS;
}


u32 ActionProcessorClass::GetArrayElem  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;
	u8 offset;
	TagClassPtr curTag;
	u16 elemNum;
	u8 unique;
	u8 fromBefore;
	u8 sort;
	u8 sortMode;
	
	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function get array elem: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( !TAG_IS_INT(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function get array elem: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			ERROR_PRINT("ERROR in function get array elem: tag type is not array");
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function get array elem: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( !TAG_IS_ARR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function get array elem: tag type is not arr");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			
			offset = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			ERROR_PRINT("ERROR in function get array elem: tag type is not array");
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function append array: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( !TAG_IS_ARR(TagPtr->getTag(Source1TagClassListTag).mTagType) || !TAG_IS_INT(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function get array elem: tag type is not correct");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			offset = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		curTag = &TagPtr->getTag(Source1TagClassListTag);

		TagPtr->getTag(DestTagClassListTag).setValue(getElemValue(curTag->getArrayValue() + Source2Value * curTag->getElemByteLength(), curTag->getElemByteLength(), curTag->getElemSign()), DestTagClassListTag, true, isBeforeLoadAction);

	}
	*pOffSet = offset;

	return AHMI_FUNC_SUCCESS;
}

#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
u8 SnapshotFlag = 0;
u32 StartAddrOfSnapshotBuffer = 0;
u32 StartAddrOfSnapshotBuffer_tmp = 0;
static u32 MallocAddrOfSnapshotBuffer = 0;

uint8_t im_up_byGPU_flag = 0;
void im_up_byGPU(uint32_t srcimage, uint32_t dstimage, int scalevalue)
{
	uint32_t old_fbaddr;
    #if defined(CHIP_GC9005)
	uint32_t oldsourcebufferarry[16] = {0};
    #endif
	uint32_t sourcebufferarry[16] = {0};
	uint32_t sourcebufferAdd = 0x0200C000;
	uint32_t irq;
	
	AHMI_WaitDrawDone();
	
	#if defined(CHIP_GC9002)
	irq = csi_irq_save();
    #elif defined(CHIP_GC9005)
	rt_base_t level;
	level = rt_hw_interrupt_disable();
	#endif
	
	AHMI_ITConfig(IT_DRAW_DONE, DISABLE);
	
	#if (defined VERSION_2) && (defined EMBEDDED)  
	if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGBA8888)
	{
		sourcebufferarry[0] = (RGBA8888);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB565)
	{
		sourcebufferarry[0] = (RGB565);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_Y8)
	{
		sourcebufferarry[0] = (Y8);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC)
	{
		sourcebufferarry[0] = (SFBC);
	}
	else
	{
		#if defined(CHIP_GC9002)
		csi_irq_restore(irq);
        #elif defined(CHIP_GC9005)
		rt_hw_interrupt_enable(level);
		#endif
		return;
	}

	#if (defined CHIP_GC9005) 
	sourcebufferarry[0] |= (1 << 18);//CHIP_GC9005 sourcebuffer tilesize 16x16 {18bit: 0 means 32x32; 1 means 16x16};
	#endif
	#endif 

	sourcebufferarry[0] |= 0x8000;//last texture flag
	sourcebufferarry[1] = 0;
	sourcebufferarry[2] = ((gScreenWidth) << 20);
	sourcebufferarry[3] = ((gScreenWidth) << 20) | ((gScreenHeight) << 4);
	sourcebufferarry[4] = ((gScreenHeight) << 4);
	sourcebufferarry[5] = csi_fix_div(1, scalevalue, 29);
	sourcebufferarry[6] = 0x0;
	sourcebufferarry[7] = 0x0;
	sourcebufferarry[8] = 0x0;
	sourcebufferarry[9] = csi_fix_div(1, scalevalue, 29);
	sourcebufferarry[10] = 0x0;
	sourcebufferarry[11] = 0;
	sourcebufferarry[12] = 0;
	sourcebufferarry[13] = (gScreenWidth << 16) | gScreenHeight;
	sourcebufferarry[14] = srcimage;
	extern uint32_t START_ADDR_OFFSET_FROM_GCHEAP;
	sourcebufferarry[14] += (START_ADDR_OFFSET_FROM_GCHEAP);
	sourcebufferarry[15] = ((((((gScreenWidth) + TILESIZE - 1) / TILESIZE - 1) & 0xFF) << 16)) | (((((gScreenHeight) + TILESIZE - 1) / TILESIZE - 1) & 0xFF) << 24);

    #if defined(CHIP_GC9005)
	//store old sourcebuffer
	sourcebufferAdd = AHMI_GetSBAdress();
	for(int j = 0; j < 16; j++)
    {
	    oldsourcebufferarry[j] = *(((uint32_t *)(sourcebufferAdd + 4 * j)));
    }
    #endif

	for(int j = 0; j < 16; j++)
	{
		*(((uint32_t *)(sourcebufferAdd + 4 * j))) = sourcebufferarry[j];
	}
	
	old_fbaddr = AHMI_GetFBAdress();
	AHMI_SetFBAdress(dstimage);
	
	csi_hal_DCache_Flush();
	GPE_Cmd(ENABLE);
	
	while( ((AHMI->STR & 0x1) == 1))
    {
    ;};

    while( ((AHMI->STR & 0x1) == 0))
    {
    ;};
	
    #if defined(CHIP_GC9005)
    //restore old sourcebuffer
    for(int j = 0; j < 16; j++)
    {
        *(((uint32_t *)(sourcebufferAdd + 4 * j))) = oldsourcebufferarry[j];
    }
    #endif

	AHMI_SetFBAdress(old_fbaddr);
	
	im_up_byGPU_flag = 1;
	AHMI_ITConfig(IT_DRAW_DONE, ENABLE);
	
	#if defined(CHIP_GC9002)
	csi_irq_restore(irq);
    #elif defined(CHIP_GC9005)
	rt_hw_interrupt_enable(level);
	#endif
	return;
}
#endif
u32 ActionProcessorClass::Snapshot(u32 index)
{
	#ifdef PC_SIM
	u32 color = 0, x,y;
	u8 color_r = 0; u8 color_g = 0; u8 color_b = 0;
	
	for(y = 0; y < SCREEN_HEIGHT; y++)
	{
		for(x = 0; x < SCREEN_WIDTH; x++)
		{
			color = *((u32 *)&GlobalFrameBuffer[3U * ((SCREEN_HEIGHT - (1 + y)) * SCREEN_WIDTH + x)]);
			color_b = ((color & 0xFF0000U) >> 16U) + 1U;
			color_g = ((color & 0xFF00U) >> 8U) + 1U;
			color_r = ((color & 0xFFU)) + 1U;
			color = (0xFF000000U | (color_r << 16) | (color_g << 8) | color_b);

			memcpy(&SnapshotBuffer[(SCREEN_WIDTH * y + x) * 4], &color, 4);
		}
	}
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
	#if defined(CHIP_GC9002)
	if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	{
		return AHMI_FUNC_SUCCESS;
	}
    #endif

	uint32_t mallocsizeforSnapshot = SizeOfDisplayBuffer;

	if(SnapshotFlag == 0)
	{
		SnapshotFlag = 1;
		/* Malloc sdram space for snapshot by mallocsizeforSnapshot. */
		MallocAddrOfSnapshotBuffer = (uint32_t)gcMalloc(mallocsizeforSnapshot);
		if(MallocAddrOfSnapshotBuffer == 0 && mallocsizeforSnapshot != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		StartAddrOfSnapshotBuffer = MallocAddrOfSnapshotBuffer;

		/* Malloc sdram space for snapshot by StartAddrOfSnapshotBuffer_tmp. */
		StartAddrOfSnapshotBuffer_tmp = (uint32_t)gcMalloc(mallocsizeforSnapshot);
		if(StartAddrOfSnapshotBuffer_tmp == 0 && mallocsizeforSnapshot != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		
	}
	else
	{
		StartAddrOfSnapshotBuffer = MallocAddrOfSnapshotBuffer;
	}
	
	if(StartAddrOfSnapshotBuffer != 0)
	{
		#if defined(CHIP_GC9002)
		im_up_byGPU(DISPLAY_GetPriAddress(), StartAddrOfSnapshotBuffer, 0x200);
		#elif defined(CHIP_GC9005)
		im_up_byGPU(DISPLAY_GetChannel0Address(), StartAddrOfSnapshotBuffer, 0x200);
		#endif
		
//        #if defined(CHIP_GC9002)
//		memcpy((void*)StartAddrOfSnapshotBuffer, (void*)DISPLAY_GetPriAddress(), mallocsizeforSnapshot);
//        #elif defined(CHIP_GC9005)
//		memcpy((void*)StartAddrOfSnapshotBuffer, (void*)DISPLAY_GetChannel0Address(), mallocsizeforSnapshot);
//        #endif
	}
	else
	{
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}
	#endif

	PLAR_tstAHMICmd stAHMICmd;
    
    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
	stAHMICmd.u32CmdType = enCmdType_RefreshSnapshot;
	stAHMICmd.u32PageID = WorkingPageID;

	if(gScreenOptions.enable)
	{
		stAHMICmd.screenType = RefreshScreenMain;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}

		vTaskDelay(30 / portTICK_RATE_MS);
		stAHMICmd.screenType = RefreshScreenSub;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
	}
	else
	{
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
	}

	return AHMI_FUNC_SUCCESS;
}

#ifdef PC_SIM
void im_up_double(uint8_t *srcimage, uint8_t *dstimage, int srcwidth, int srcheight, double scalevalue)
{
	double offset = 0;
	//if(scalevalue < 1)
	//	offset = -0.5;
	//else
	//	offset = 0.5;
	for(int i = 0; i < srcwidth * scalevalue; i++)
	{
		for(int j = 0; j < srcheight * scalevalue; j++)
		{
			int u_int = ((i+offset)/scalevalue);
			int v_int = ((j)/scalevalue);
			double u_dec = ((i+offset)/scalevalue - u_int);
			double v_dec = ((j)/scalevalue - v_int);
			double p00 = (1-u_dec)*(1-v_dec);
			double p10 = u_dec*(1-v_dec);
			double p01 = (1-u_dec)*v_dec;
			double p11 = u_dec*v_dec;

			int s00,s01,s10,s11;
			for(int k = 0; k < 4; k++)
			{
				s00 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * 4 + k);
				if ((v_int+1) > srcheight)
					s01 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * 4 + k);
				else
					s01 = *(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * 4 + k);

				if ((u_int+1) > srcwidth)
					s10 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * 4 + k);
				else
					s10 = *(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * 4 + k);

				if (((v_int+1) > srcheight) && ((u_int+1) > srcwidth))
					s11 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * 4 + k);
				else if ((v_int+1) > srcheight)
					s11 = *(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * 4 + k);
				else if ((u_int+1) > srcwidth)
					s11 = *(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * 4 + k);  
				else
					s11 = *(srcimage + (srcwidth * (u_int+1) + (v_int+1)) * 4 + k);

				*(dstimage + (int(srcwidth * scalevalue) * i + j) * 4 + k) = p00*s00 + p01*s01 + p10*s10 + p11*s11;
			}
		}
	}
}

void im_up(uint8_t *srcimage, uint8_t *dstimage, uint8_t pixelBytes, int srcwidth, int srcheight, int scalevalue)//scalevalue 1.22.9
{
	int offset = 0;
	for(int i = 0; i < ((srcwidth * scalevalue) >> 9); i++)
	{
		for(int j = 0; j < ((srcheight * scalevalue) >> 9); j++)
		{
			int u_int_9 = (((i+offset) << 18)/scalevalue);
			int v_int_9 = ((j << 18)/scalevalue);
			int u_dec_9 = (u_int_9 & 0x1FF);
			int v_dec_9 = (v_int_9 & 0x1FF);
			int p00 = ((1 << 9)-u_dec_9)*((1 << 9)-v_dec_9);
			int p10 = u_dec_9*((1 << 9)-v_dec_9);
			int p01 = ((1 << 9)-u_dec_9)*v_dec_9;
			int p11 = u_dec_9*v_dec_9;

			int u_int = u_int_9 >> 9;
			int v_int = v_int_9 >> 9;

			int s00,s01,s10,s11;
			if(pixelBytes > 2)//RGB888 or RGBA8888
			{
				for(int k = 0; k < pixelBytes; k++)
				{
					s00 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + k);
					if ((v_int+1) > srcheight)
						s01 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + k);
					else
						s01 = *(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes + k);

					if ((u_int+1) > srcwidth)
						s10 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + k);
					else
						s10 = *(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes + k);

					if (((v_int+1) > srcheight) && ((u_int+1) > srcwidth))
						s11 = *(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + k);
					else if ((v_int+1) > srcheight)
						s11 = *(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes + k);
					else if ((u_int+1) > srcwidth)
						s11 = *(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes + k);  
					else
						s11 = *(srcimage + (srcwidth * (u_int+1) + (v_int+1)) * pixelBytes + k);

					*(dstimage + (int(srcwidth * scalevalue >> 9) * i + j) * pixelBytes + k) = (p00*s00 >> 18) + (p01*s01 >> 18) + (p10*s10 >> 18) + (p11*s11 >> 18);
				}
			}
			else if(pixelBytes == 2)//RGB565
			{
				int s00_r, s00_g, s00_b;
				int s01_r, s01_g, s01_b;
				int s10_r, s10_g, s10_b;
				int s11_r, s11_g, s11_b;
				int final_r, final_g, final_b;
				s00_r = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0xF8) >> 3);
				s00_g = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0xE0) >> 5);
				s00_b = (*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0x1F);
				
				if ((v_int+1) > srcheight)
				{
					s01_r = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0xF8) >> 3);
					s01_g = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0xE0) >> 5);
					s01_b = (*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0x1F);
				}
				else
				{
					s01_r = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes + 1) & 0xF8) >> 3);
					s01_g = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes) & 0xE0) >> 5);
					s01_b = (*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes) & 0x1F);
				}

				if ((u_int+1) > srcwidth)
				{
					s10_r = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0xF8) >> 3);
					s10_g = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0xE0) >> 5);
					s10_b = (*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0x1F);
				}
				else
				{
					s10_r = ((*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes + 1) & 0xF8) >> 3);
					s10_g = ((*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes) & 0xE0) >> 5);
					s10_b = (*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes) & 0x1F);
				}

				if (((v_int+1) > srcheight) && ((u_int+1) > srcwidth))
				{
					s11_r = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0xF8) >> 3);
					s11_g = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0xE0) >> 5);
					s11_b = (*(srcimage + (srcwidth * (u_int+0) + (v_int+0)) * pixelBytes) & 0x1F);
				}
				else if ((v_int+1) > srcheight)
				{
					s11_r = ((*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes + 1) & 0xF8) >> 3);
					s11_g = ((*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes) & 0xE0) >> 5);
					s11_b = (*(srcimage + (srcwidth * (u_int+1) + (v_int+0)) * pixelBytes) & 0x1F);
				}
				else if ((u_int+1) > srcwidth)
				{
					s11_r = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes + 1) & 0xF8) >> 3);
					s11_g = ((*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes) & 0xE0) >> 5);
					s11_b = (*(srcimage + (srcwidth * (u_int+0) + (v_int+1)) * pixelBytes) & 0x1F);
				}
				else
				{
					s11_r = ((*(srcimage + (srcwidth * (u_int+1) + (v_int+1)) * pixelBytes + 1) & 0xF8) >> 3);
					s11_g = ((*(srcimage + (srcwidth * (u_int+1) + (v_int+1)) * pixelBytes + 1) & 0x7) << 3) | ((*(srcimage + (srcwidth * (u_int+1) + (v_int+1)) * pixelBytes) & 0xE0) >> 5);
					s11_b = (*(srcimage + (srcwidth * (u_int+1) + (v_int+1)) * pixelBytes) & 0x1F);
				}
				
				//final_r = (p00*s00_r >> 18) + (p01*s01_r >> 18) + (p10*s10_r >> 18) + (p11*s11_r >> 18);
				//final_g = (p00*s00_g >> 18) + (p01*s01_g >> 18) + (p10*s10_g >> 18) + (p11*s11_g >> 18);
				//final_b = (p00*s00_b >> 18) + (p01*s01_b >> 18) + (p10*s10_b >> 18) + (p11*s11_b >> 18);

				final_r = ((p00*s00_r + p01*s01_r + p10*s10_r + p11*s11_r) >> 18);
				final_g = ((p00*s00_g + p01*s01_g + p10*s10_g + p11*s11_g) >> 18);
				final_b = ((p00*s00_b + p01*s01_b + p10*s10_b + p11*s11_b) >> 18);
				
				*(dstimage + (int(srcwidth * scalevalue >> 9) * i + j) * pixelBytes) = (((final_g & 0x7) << 5) | (final_b & 0x1F));
				*(dstimage + (int(srcwidth * scalevalue >> 9) * i + j) * pixelBytes + 1) = ((final_r << 3) | ((final_g & 0x38) >> 3));
			}
			else//frame error
			{
				return;
			}
		}
	}
}
#endif

#ifdef PC_SIM
u8 SnapshotBuffer_tmp[SIZE_OF_Snapshot_BUF] = {0}; //buffer of snapshot
#endif
u32 ActionProcessorClass::BlurSnapshot(u32 index)
{
	uint32_t tmp;
	if(((int)index <= 1))
		tmp = 1;
	else if(((int)index >= 100))
	    tmp = 100;
	else
		tmp = index;

	#ifdef PC_SIM
	
	//im_up_double(SnapshotBuffer, SnapshotBuffer_tmp, SCREEN_WIDTH, SCREEN_HEIGHT, 0.5);
	//im_up_double(SnapshotBuffer_tmp, SnapshotBuffer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 2);

	//im_up_double(SnapshotBuffer, SnapshotBuffer_tmp, SCREEN_WIDTH, SCREEN_HEIGHT, 0.4);
	//im_up_double(SnapshotBuffer_tmp, SnapshotBuffer, SCREEN_WIDTH*2/5, SCREEN_HEIGHT*2/5, 2.5);

	//im_up(SnapshotBuffer, SnapshotBuffer_tmp, 4, SCREEN_WIDTH, SCREEN_HEIGHT, 256);
	//im_up(SnapshotBuffer_tmp, SnapshotBuffer, 4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 1024);
	for(int i = 0; i < tmp; i++)
	{
		im_up(SnapshotBuffer, SnapshotBuffer_tmp, 4, SCREEN_WIDTH, SCREEN_HEIGHT, 205);
		im_up(SnapshotBuffer_tmp, SnapshotBuffer, 4, SCREEN_WIDTH*2/5, SCREEN_HEIGHT*2/5, 1280);
	}

	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
	#if defined(CHIP_GC9002)
	if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	{
		return AHMI_FUNC_SUCCESS;
	}
	#endif
	if((StartAddrOfSnapshotBuffer != 0) && (StartAddrOfSnapshotBuffer_tmp != 0))
	{
		for(uint32_t i = 0; i < tmp; i++)
		{
			//im_up((uint8_t *)StartAddrOfSnapshotBuffer, (uint8_t *)StartAddrOfSnapshotBuffer_tmp, 2, SCREEN_WIDTH, SCREEN_HEIGHT, 205);
			//im_up((uint8_t *)StartAddrOfSnapshotBuffer_tmp, (uint8_t *)StartAddrOfSnapshotBuffer, 2, SCREEN_WIDTH*2/5, SCREEN_HEIGHT*2/5, 1280);
		
			im_up_byGPU(StartAddrOfSnapshotBuffer, StartAddrOfSnapshotBuffer_tmp, 205);
			im_up_byGPU(StartAddrOfSnapshotBuffer_tmp, StartAddrOfSnapshotBuffer, 1280);
		}
	}
	else
	{
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}
	#endif
	
	PLAR_tstAHMICmd stAHMICmd;
    
    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
	stAHMICmd.u32CmdType = enCmdType_RefreshSnapshot;
	stAHMICmd.u32PageID = WorkingPageID;

	if(gScreenOptions.enable)
	{
		stAHMICmd.screenType = RefreshScreenMain;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}

		vTaskDelay(30 / portTICK_RATE_MS);
		stAHMICmd.screenType = RefreshScreenSub;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
	}
	else
	{
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
	}

	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::ClearSnapshot(u32 index)
{
	#ifdef PC_SIM
	memset(SnapshotBuffer, 0, SIZE_OF_Snapshot_BUF);
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
	#if defined(CHIP_GC9002)
	if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	{
		return AHMI_FUNC_SUCCESS;
	}
	#endif
	StartAddrOfSnapshotBuffer = 0;
	#endif

	PLAR_tstAHMICmd stAHMICmd;
    
    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
	stAHMICmd.u32CmdType = enCmdType_RefreshSnapshot;
	stAHMICmd.u32PageID = WorkingPageID;

	if(gScreenOptions.enable)
	{
		stAHMICmd.screenType = RefreshScreenMain;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}

		vTaskDelay(30 / portTICK_RATE_MS);
		stAHMICmd.screenType = RefreshScreenSub;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
	}
	else
	{
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
	}

	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// nameï¿½ï¿½ IncTag
// tag = tag + source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::IncTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function IncTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function IncTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value + Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function IncTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function IncTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() + Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function IncTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function IncTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value + TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function IncTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function IncTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() + TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// nameï¿½ï¿½ DecTag
// tag = tag - source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::DecTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function DecTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function DecTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value - Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function DecTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function DecTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() - Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function DecTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function DecTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value - TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function DecTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function DecTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() - TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// tag = tag * source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::MulTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function MulTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function MulTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)Source1Value * (s32)Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function MulTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function MulTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)TagPtr->getTag(Source1TagClassListTag).getValue() * (s32)Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function MulTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function MulTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)Source1Value * (s32)TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function MulTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function MulTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)TagPtr->getTag(Source1TagClassListTag).getValue() * (s32)TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// tag = tag / source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::DivTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function DivTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function DivTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)Source1Value / (s32)Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function DivTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function DivTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)TagPtr->getTag(Source1TagClassListTag).getValue() / (s32)Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function DivTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function DivTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)Source1Value / (s32)TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function DivTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function DivTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue(((s32)TagPtr->getTag(Source1TagClassListTag).getValue() / (s32)TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// tag = tag % source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::ModTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ModTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function ModTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value % Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ModTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ModTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() % Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ModTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ModTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value % TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ModTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ModTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() % TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// tag = tag & source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::AndTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function AndTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function AndTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value & Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function AndTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function AndTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() & Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function AndTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function AndTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value & TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function AndTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function AndTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() & TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// tag = tag or source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::OrTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function OrTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function OrTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value | Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function OrTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function OrTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() | Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function OrTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function OrTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value | TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function OrTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function OrTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() | TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;	
}

 //-----------------------------
// tag = tag XOR source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::XorTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function XorTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function XorTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value ^ Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function XorTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function XorTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() ^ Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function XorTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function XorTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value ^ TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function XorTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function XorTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() ^ TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// tag = ~tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::NotTag  (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u16 SourceTagClassListTag, bool isBeforeLoadAction)
{
	int  i = 0;
	//char text[200];
	
	// check mTagType of tag
	if((SourceTagClassListTag >= ConfigData.NumofTags) || (DestTagClassListTag >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function NotTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(SourceTagClassListTag).mTagType ) || TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function NotTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd )){
		u32 mValue = 0;
		for(i = 0; i < 32; i++)
		{
			mValue |= ((((TagPtr->getTag(SourceTagClassListTag).getValue() >> i) & 0x1)?0:1) << i);	
		}
		TagPtr->getTag(DestTagClassListTag).setValue(mValue,DestTagClassListTag, true, isBeforeLoadAction);
	}
	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// tag = tag << source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SLTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SLTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SLTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value << Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SLTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SLTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() << Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SLTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SLTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value << TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SLTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SLTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() << TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag = tag >> source_tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SRTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SRTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SRTag: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value >> Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SRTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SRTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() >> Source2Value), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SRTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SRTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((Source1Value >> TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);

			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SRTag: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SRTag: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
				TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(Source1TagClassListTag).getValue() >> TagPtr->getTag(Source2TagClassListTag).getValue()), DestTagClassListTag, true, isBeforeLoadAction);
			
			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}
	return AHMI_FUNC_SUCCESS;	
}

//-----------------------------
// tag = imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function SetTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function SetTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue(u32_ImmValue,DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag += imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::IncTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function IncTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function IncTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()+u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;	
}

//-----------------------------
// tag -= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::DecTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function DecTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function DecTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()-u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag *= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::MulTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function MulTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function MulTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue(((s32)TagPtr->getTag(DestTagClassListTag).getValue()*(s32)u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag /= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::DivTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function DivTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function DivTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue(((s32)TagPtr->getTag(DestTagClassListTag).getValue()/(s32)u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag %= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::ModTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function ModTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function ModTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()%u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag &= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::AndTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function AndTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function AndTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()&u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag |= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::OrTagImm (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function OrTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function OrTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()|u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag XOR= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::XorTagImm(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function XorTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function XorTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()^u32_ImmValue),DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag <<= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SLTagImm (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{	
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function SLTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function SLTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()<<u32_ImmValue), DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// tag >>= imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SRTagImm (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction)
{
	if(DestTagClassListTag >= ConfigData.NumofTags )
	{
		ERROR_PRINT("ERROR in function SRTagImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function SRTagImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
		TagPtr->getTag(DestTagClassListTag).setValue((TagPtr->getTag(DestTagClassListTag).getValue()>>u32_ImmValue), DestTagClassListTag, true, isBeforeLoadAction);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// if source_tag == dest_tag, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source of tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchEQU (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u16 SourceTagClassListTag)
{
	if(DestTagClassListTag >= ConfigData.NumofTags || SourceTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchEQU: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) || TAG_IS_STR(TagPtr->getTag(SourceTagClassListTag).mTagType))
	{
		ERROR_PRINT("ERROR in function BranchEQU: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() == (s32)TagPtr->getTag(SourceTagClassListTag).getValue())
		{
			mStatusRegister = 1;
		}
		else mStatusRegister = 0;
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// if source_tag == imm, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchEQUImm (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue)
{
	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchEQUImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function BranchEQUImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() == (s32)u32_ImmValue)
			mStatusRegister = 1;
		else mStatusRegister = 0;
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// if source_tag != dest_tag, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source of tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchNEQ (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u16 SourceTagClassListTag)
{
	if(DestTagClassListTag >= ConfigData.NumofTags || SourceTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchEQU: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) || TAG_IS_STR(TagPtr->getTag(SourceTagClassListTag).mTagType))
	{
		ERROR_PRINT("ERROR in function BranchEQU: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() != (s32)TagPtr->getTag(SourceTagClassListTag).getValue())
		{
			mStatusRegister = 1;
		}
		else mStatusRegister = 0;
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// if source_tag != imm, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchNEQImm (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue)
{
	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchEQUImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function BranchEQUImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() != (s32)u32_ImmValue)
			mStatusRegister = 1;
		else mStatusRegister = 0;
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// if source_tag > dest_tag, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source of tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchMore (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u16 SourceTagClassListTag)
{
	if(DestTagClassListTag >= ConfigData.NumofTags || SourceTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchMore: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) || TAG_IS_STR(TagPtr->getTag(SourceTagClassListTag).mTagType))
	{
		ERROR_PRINT("ERROR in function BranchMore: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() > (s32)TagPtr->getTag(SourceTagClassListTag).getValue())
			mStatusRegister = 1;
		else mStatusRegister = 0;
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// if source_tag > imm, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchMoreImm (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue)
{
	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchMoreImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function BranchMoreImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() > (s32)u32_ImmValue)
			mStatusRegister = 1;
		else mStatusRegister = 0;
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// if source_tag < dest_tag, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param SourceTagClassListTag : the source of tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchLess (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u16 SourceTagClassListTag)
{
	// check mTagType of tag
	if(DestTagClassListTag >= ConfigData.NumofTags || SourceTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchLess: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) || TAG_IS_STR(TagPtr->getTag(SourceTagClassListTag).mTagType))
	{
		ERROR_PRINT("ERROR in function BranchLess: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() < (s32)TagPtr->getTag(SourceTagClassListTag).getValue())
			mStatusRegister = 1;
		else mStatusRegister = 0;
	}

    return AHMI_FUNC_SUCCESS;	
}

//-----------------------------
// if source_tag < imm, jump
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param DestTagClassListTag : the destination tag number
// @param u32_ImmValue : the value of imm
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::BranchLessImm (u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagClassListTag, u32 u32_ImmValue)
{
	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function BranchLessImm: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType ) )
	{
		ERROR_PRINT("ERROR in function BranchLessImm: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		if((s32)TagPtr->getTag(DestTagClassListTag).getValue() < (s32)u32_ImmValue)
			mStatusRegister = 1;
		else mStatusRegister = 0;
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¿½ï¿½ Jump
// pc = pc+imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param u32_ImmValue : the value of imm
// addtional comments :
//   not the number of code. imm need to account for each code size
//-----------------------------
void ActionProcessorClass::Jump (u8 u8_CdRun, u8 u8_CdRCd, u16 u32_ImmValue)
{
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))  
	{
		pPC = pPC + (s16)u32_ImmValue;
	}
}

//-----------------------------
// nameï¿½ï¿½ WriteReadData
// write the modbus regs form imm address
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param u32_ImmValue : the value of imm
// addtional comments :
//   ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½×ªï¿½ï¿½ï¿½Ù¸ï¿½ï¿½ï¿½Ã¿ï¿½ï¿½Ö¸ï¿½î²»Í¬ï¿½ï¿½ï¿½ï¿½
//-----------------------------
void ActionProcessorClass::RW_DATA(u8 u8_CdRun , u8 u8_CdRCd, u16 u16_DestTagClassListTag, u32 u32_ImmValue)
{
	return;
}

//-----------------------------
// nameï¿½ï¿½ set_timer
// set the parameters of timer
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param3 u16 source [7:3] timer ID [2:0] tiemr variable ID
// @param4 u32 imm    the value of timer tag
// addtional comments :
//   
//-----------------------------
void ActionProcessorClass::set_timer(u8 u8_CdRun , u8 u8_CdRCd , u16 source, u32 u32_ImmValue)
{
	u8 timerID;
	u8 timerVariableID;
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd )) 
	{
		timerID = source >> 3;
		timerVariableID = source & 0x07;
		TimerPtr[timerID].TimerSetValue(timerID, timerVariableID, u32_ImmValue);
	}
}

//-----------------------------
// nameï¿½ï¿½ triggerAnimation
// trigger the animation of canvas
// find canvas according to tag, find animation according to imm
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID    : the number of tag
// @param u8_animationID : the ID of animation
// addtional comments :
//   
//-----------------------------
u32 ActionProcessorClass::triggerAnimation(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u8 u8_animationID)
{
	TagClassPtr curTag;
	CanvasClassPtr curCanvas = NULL;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	int i;

	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function triggerAnimation: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		curTag = &TagPtr->getTag(tagID);
		if(curTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < curTag->mNumOfElementLinker; i++ )
			{
				if(curTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_CANVAS)
				{
					curCanvasLinkDataPtr = (CanvasLinkDataPtr)curTag->pTagELmementLinker[i].mLinkElementPtr;
					curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];
					if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) continue;
					curCanvas->triggerAnimation(u8_animationID);
				}
			}
		}
		
		if(curCanvas == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¿½ï¿½ triggerAnimationTag
// trigger the animation of animation tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID    : the number of tag
// @param u8_animationID : the ID of animationTag
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::triggerAnimationTag(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u16 u8_animationTagID)
{
	TagClassPtr curTag;
	TagClassPtr animationTag;
	CanvasClassPtr curCanvas = NULL;
	u8 u8_animationID;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	int i;

	if((tagID >= ConfigData.NumofTags) || (u8_animationTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function triggerAnimationTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		curTag = &TagPtr->getTag(tagID);
		animationTag = &TagPtr->getTag(u8_animationTagID);
		if(animationTag != NULL)
			u8_animationID = animationTag->getValue();
		else 
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		if(curTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < curTag->mNumOfElementLinker; i++ )
			{
				if(curTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_CANVAS)
				{
					curCanvasLinkDataPtr = (CanvasLinkDataPtr)curTag->pTagELmementLinker[i].mLinkElementPtr;
					curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];
					curCanvas->triggerAnimation(u8_animationID);
				}
			}
		}
		
		if(curCanvas == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::setTagString(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u16 stringID, bool isBeforeLoadAction)
{
	if(tagID >= ConfigData.NumofTags /*|| SourceTagClassListTag >= ConfigData.NumofTags*/)
	{
		ERROR_PRINT("ERROR in function setTagString: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( !TAG_IS_STR(TagPtr->getTag(tagID).mTagType ))
	{
		ERROR_PRINT("ERROR in function setTagString: tag type is not string");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd)){
		if (/*stringID>=0 &&*/stringID<ConfigData.NumofStrings)
		{
			TagPtr->getTag(tagID).setString((char*)gStringPtr + StringListPtr[stringID].addr, 1, true, isBeforeLoadAction);
		}else{
			ERROR_PRINT("ERROR in function setTagString: string id out of boundry");
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

void ActionProcessorClass::setTagArr(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u16 arrID)
{
	if(tagID >= ConfigData.NumofTags /*|| SourceTagClassListTag >= ConfigData.NumofTags*/)
	{
		ERROR_PRINT("ERROR in function SetTagArr: tag index out of baundary");
		return ;
	}
	if( !TAG_IS_ARR(TagPtr->getTag(tagID).mTagType ))
	{
		ERROR_PRINT("ERROR in function SetTagArr: tag type is not array");
		return;
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd)){
		if (/*arrID >= 0 &&*/arrID < ConfigData.NumofArrayDatas)
		{
			TagPtr->getTag(tagID).setArray(gArrayDataPtr + ArrayDataListPtr[arrID].addr, 1, ArrayDataListPtr[arrID].length, 4);
		}else{
			ERROR_PRINT("ERROR in function SetTagArr: array id out of boundry");
			return;
		}
	}
}

#if defined(EMBEDDED) && defined(VERSION_2)
extern uint8_t* p_Oscilloscopetexture[Chart_Num_MAX];
#endif
extern uint8_t ChangeModeFlag[Chart_Num_MAX];
extern uint32_t olddata[Chart_Num_MAX];
extern uint32_t datanumber[Chart_Num_MAX];
extern Point DataPoint[Chart_Num_MAX][3];
u32 ActionProcessorClass::clearData(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID)
{
	#ifndef CHIP_GC9002_NoHyper
	TagClassPtr curTag;
//	TagClassPtr colorTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	u16 heighttemp;
	int i;

	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetColor: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		curTag = &TagPtr->getTag(tagID);
		if (curTag == NULL)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}

		if(curTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < curTag->mNumOfElementLinker; i++ )
			{
				if(curTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_WIDGET)
				{
					curWidgetLinkDataPtr = (WidgetLinkDataPtr)curTag->pTagELmementLinker[i].mLinkElementPtr;
					curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];
					if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) continue;

					//yPadding means display mode for CHART: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
					if((curWidget->WidgetType == CHART) && (curWidget->YPADDING != 3))
					{
						if(curWidget->YPADDING == 2)
						{
							ChangeModeFlag[curWidget->IDnumber] = 0;
						}
						datanumber[curWidget->IDnumber] = 0;
						olddata[curWidget->IDnumber] = 0;
						DataPoint[curWidget->IDnumber][0].X = 0;
						DataPoint[curWidget->IDnumber][0].Y = 0;
						DataPoint[curWidget->IDnumber][1].X = 0;
						DataPoint[curWidget->IDnumber][1].Y = 0;
						DataPoint[curWidget->IDnumber][2].X = curWidget->WidgetWidth / (curWidget->XCOUNT + 1);
						DataPoint[curWidget->IDnumber][2].Y = 0;
						#ifdef PC_SIM
						memset(OscilloscopeArry + curWidget->IDnumber * SIZE_OF_SingleOscilloscope_BUF, 0, SIZE_OF_SingleOscilloscope_BUF);
						#endif
						#ifdef EMBEDDED	
						//widget height supplement length (64bit align)
						heighttemp = curWidget->WidgetHeight;
						if( (heighttemp & 0x3f) != 0)
						{
							heighttemp = ((heighttemp >> 6) << 6) + 64;
						}

						if(p_Oscilloscopetexture[curWidget->IDnumber] != NULL)
						{
							memset(p_Oscilloscopetexture[curWidget->IDnumber], 0, (curWidget->WidgetWidth * heighttemp >> 3));
						}
						#endif 
					}
					else
					{
						curWidget->WidgetAttr &= ~0x8000;
						curWidget->OldValueL = 0x7FFF;
						curWidget->OldValueH = 0x7FFF;
						#ifdef PC_SIM
						memset(OscilloscopeArry + curWidget->IDnumber * SIZE_OF_SingleOscilloscope_BUF, 0, SIZE_OF_SingleOscilloscope_BUF);
						#endif
						#ifdef EMBEDDED	
						if(curWidget->WidgetWidth * curWidget->WidgetHeight % 8 != 0)
						{
							if(p_Oscilloscopetexture[curWidget->IDnumber] != NULL)
							{
								memset(p_Oscilloscopetexture[curWidget->IDnumber], 0, ((curWidget->WidgetWidth * curWidget->WidgetHeight + 8) >> 3));
							}
						}
						else
						{
							if(p_Oscilloscopetexture[curWidget->IDnumber] != NULL)
							{
								memset(p_Oscilloscopetexture[curWidget->IDnumber], 0, (curWidget->WidgetWidth * curWidget->WidgetHeight >> 3));
							}
						}
						#endif 
					}
				}
			}
		}
		
		if(curWidget == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::playSound(u32 index){
	uint16_t i = 0, j = 0;

	for(i = 0; i < ConfigData.NumofSound; i++)
	{
		if(gSoundPtr[i].mSoundID > j)
		{
			j = gSoundPtr[i].mSoundID;
		}
	}

	if((index&0xFF) > j)
	{
		ERROR_PRINT("ERROR in function playSound: sound id out of boundry");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_SoundIDOverrun);
	}
	
	gSDLplayer.PlayerInit();
	gSDLplayer.play(index&0xFF,(index>>8)&0xFFFF, j);
	
	return AHMI_FUNC_SUCCESS;
}

void ActionProcessorClass::pauseSound(u32 index){
	gSDLplayer.pause((index >> 8) & 0xFFFF);
}

void ActionProcessorClass::resumeSound(u32 index){
	gSDLplayer.resume();
}

void ActionProcessorClass::stopSound(u32 index){
	gSDLplayer.stop((index >> 8) & 0xFFFF);
}

u32 ActionProcessorClass::writeLog(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag1ID, u16 tag2ID)
{
	TagClassPtr tag1;
	TagClassPtr tag2;
    TagClassPtr tagdate;
	TagClassPtr tagtime;
    u32 tagdateValue;
	u32 tagtimeValue;
	u32 tag1Value;
	u32 tag2Value;
	u32 tag1RegID;
	u32 tag2RegID;

	if((tag1ID >= ConfigData.NumofTags) || (tag2ID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function writeLog: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

#ifdef PC_SIM
	char text[64];
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		tag1 = &TagPtr->getTag(tag1ID);
		tag2 = &TagPtr->getTag(tag2ID);
		tag1Value = tag1->getValue();
		tag2Value = tag2->getValue();
		
		tag1RegID = tag1->mRegID;
		tag2RegID = tag2->mRegID;

		time_t now ;
        struct tm *tm_now ;
        time(&now) ;
        tm_now = localtime(&now) ;
		//print for debug
        /*sprintf(text,"%d/%d/%d %d:%d:%d,%d,%d", tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, tag1Value, tag2Value);
		ERROR_PRINT(text);*/

		//init log file
		if(!logFile.is_open())
		{
			logFile.open(AHMI_LOG_FILE_NAME, ios::out | ios::app);
			if (!logFile.is_open())
			{
				ERROR_PRINT("connot create file log.csv");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
			}
			
		}
		
		//write to file
		logFile <<  tm_now->tm_year+1900 << "/" <<tm_now->tm_mon+1 << "/" << tm_now->tm_mday << " " <<tm_now->tm_hour << ":" << tm_now->tm_min << ":" << tm_now->tm_sec << ", " << tag1RegID << ", " << tag1Value << ", " << tag2RegID << ", " << tag2Value << endl; 	
	}
#endif
    
#ifdef EMBEDDED
    SendValueType Value[3];
//    uint32_t index;
    if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        tagdate = &TagPtr->getTag(SYSTEM_RTC_YEAR_TAG);
		tagtime = &TagPtr->getTag(SYSTEM_RTC_HOUR_MINUITE_TAG);
        tagdateValue = tagdate->getValue();
		tagtimeValue = tagtime->getValue();
        tag1 = &TagPtr->getTag(tag1ID);
		tag2 = &TagPtr->getTag(tag2ID);
		tag1Value = tag1->getValue();
		tag2Value = tag2->getValue();
		
		tag1RegID = tag1->mRegID;
		tag2RegID = tag2->mRegID;
        
        Value[0].mID = tagdateValue;
        Value[0].mValue = tagtimeValue;
        Value[1].mID = tag1RegID;
        Value[1].mValue = tag1Value;
        Value[2].mID = tag2RegID;
        Value[2].mValue = tag2Value;
    
        ShareMemory_SendLogFrame(0, 3, &Value[0], 50, 0, 0);
    }
#endif
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::writeReg(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag1ID,u32 option)
{
	TagClassPtr tag1;
	u32 tag1Value;
	u32 tag1RegID;

	if(tag1ID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function writeReg: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

#ifdef PC_SIM
	char text[64];
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		tag1 = &TagPtr->getTag(tag1ID);
		if (!TAG_IS_STR(tag1->mTagType)) //send number tag value
		{
			tag1Value = tag1->getValue();
			tag1RegID = tag1->mRegID;

			char text[128];
			sprintf(text,"writeReg tagID: %d regID: %d  option:%d mID: %d value: %d",tag1ID, tag1RegID,option,(tag1RegID & 0x00FFFFFF) | ( option << 24 ), tag1->getValue());

			ERROR_PRINT(text);
		}
		else//send string tag value
		{
			tag1Value = tag1->getValue();
			tag1RegID = tag1->mRegID;

			char text[800];
			sprintf(text,"writeReg tagID: %d regID: %d string: %s",tag1ID, tag1RegID, tag1->getStringValue());

			ERROR_PRINT(text);
		}
	}
#endif
    
#ifdef EMBEDDED
    SendValueType Value;
//    uint32_t index;
	uint16_t stringtaglength;
    uint8_t* stringtagvalue;
    
    if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		tag1 = &TagPtr->getTag(tag1ID);
		if (!TAG_IS_STR(tag1->mTagType)) //send number tag value
		{
            tag1Value = tag1->getValue();
            tag1RegID = tag1->mRegID;
            
            //high 8 store option
            Value.mID = (tag1RegID & 0x00FFFFFF) | ( option << 24 );
            Value.mValue = tag1Value;
            
			ShareMemory_SendDataFrame(0, 1, &Value, 50, 0, 0);
		}
		else//send string tag value
		{
			tag1Value = tag1->getValue();
            tag1RegID = tag1->mRegID;

            stringtagvalue = tag1->getStringValue();
			stringtaglength = tag1->getStringLength();
            
            //high 8 store option
            tag1RegID = (tag1RegID & 0x00FFFFFF) | ( option << 24 );
            
			ShareMemory_SendStringFrame(0, tag1RegID, stringtaglength, stringtagvalue, 50, 0, 0);
		}
    }
#endif
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::writeFlash(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag1ID, u32 u32_ImmValue)
{
	TagClassPtr tag1;
	u32 tag1Value;
	u32 tag1RegID;

	if(tag1ID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function writeFlash: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

#ifdef PC_SIM
	char text[64];
	
	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		tag1 = &TagPtr->getTag(tag1ID);
		if(tag1->mStoreID != 0)
		{
			if(!TAG_IS_STR(tag1->mTagType))
			{
				if(u32_ImmValue == 0xFFFFFFFF)
				{
					SM_vClear(StoreNumberTag);
					char text[128];
					sprintf(text,"erase store num tag space");
					ERROR_PRINT(text);
				}
				else
				{
					tag1Value = tag1->getValue();
					tag1RegID = tag1->mRegID;
					SM_vStoreNumberTag(tag1RegID, tag1Value);
					char text[128];
					sprintf(text,"write to flash tagID: %d regID: %d value: %d",tag1ID, tag1RegID, tag1->getValue());
					ERROR_PRINT(text);
				}
			}
			else
			{
				if(u32_ImmValue == 0xFFFFFFFF)
				{
					SM_vClear(StoreStringTag);
					char text[128];
					sprintf(text,"erase store string tag space");
					ERROR_PRINT(text);
				}
				else
				{
					tag1RegID = tag1->mRegID;
					SM_vStoreStringTag(tag1RegID, tag1->getStringLength(), tag1->getStringValue());
					char text[800];
					sprintf(text,"write to flash tagID: %d regID: %d string: %s",tag1ID, tag1RegID, tag1->getStringValue());
					ERROR_PRINT(text);
				}
			}
		}
	}
#endif

#if defined(EMBEDDED) && defined(VERSION_2)

    if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        tag1 = &TagPtr->getTag(tag1ID);
		tag1Value = tag1->getValue();
		tag1RegID = tag1->mRegID;

		if(tag1->mStoreID != 0)
		{
			if(!TAG_IS_STR(tag1->mTagType))
			{
				if(u32_ImmValue == 0xFFFFFFFF)
				{
					SM_vClear(StoreNumberTag);
				}
				else
				{
					SM_vStoreNumberTag(tag1RegID, tag1Value);
				}
			}
			else
			{
				if(u32_ImmValue == 0xFFFFFFFF)
				{
					SM_vClear(StoreStringTag);
				}
				else
				{
					SM_vStoreStringTag(tag1RegID, tag1->getStringLength(), tag1->getStringValue());
				}
			}
		}
    }
#endif
	return AHMI_FUNC_SUCCESS;
}

#ifdef __cplusplus
extern "C"{
#endif
extern EM_tstErrItem astErrItem[EM_ERROR_MESSAGE_COUNT_MAX];
extern uint8_t error_write_pos;
#ifdef __cplusplus
};
#endif

u32 ActionProcessorClass::readErrMessage(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag1ID)
{
	#ifndef CHIP_GC9002_NoHyper
	TagClassPtr tag1;
	u32 tag1Value;
//	u32 tag1RegID;

	if(tag1ID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function readErrMessage: tagID out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	tag1 = &TagPtr->getTag(SYSTEM_ERROR_MESSAGE_PAGE_NUM_TAG);
	tag1Value = tag1->getValue();

	if((s32)tag1Value >= EM_ERROR_MESSAGE_PAGE_COUNT_MAX)
	{
		tag1->updateTagValueToValueSpace(EM_ERROR_MESSAGE_PAGE_COUNT_MAX - 1);
		tag1Value = tag1->getValue();
	}
	else if((s32)tag1Value < 0)
	{
		tag1->updateTagValueToValueSpace( 0 );
		tag1Value = tag1->getValue();
	}

	int i = 0, start_num = 0, err_num = 0, err_page = 0;

	char tmp_buf0[25];
	char tmp_buf[240];
	memset(tmp_buf, 0, sizeof(tmp_buf));

	if(error_write_pos == 0xFF)
	{
		for(i = 0; i < EM_ERROR_MESSAGE_COUNT_MAX; i++)
		{
			if((EM_enErrLevelNoErr >= astErrItem[i].u16ErrLevel) || (EM_enErrLevelRecover < astErrItem[i].u16ErrLevel))
			{
				err_num = i;
				break;
			}
		}
	}
	else
	{
		err_num = EM_ERROR_MESSAGE_COUNT_MAX;
	}

	err_page = err_num / EM_ERROR_MESSAGE_ONEPAGE_NUMBER;
	if((err_num % EM_ERROR_MESSAGE_ONEPAGE_NUMBER) != 0)
	{
		err_page++;
	}

	if(((s32)tag1Value >= err_page) && (err_page > 0))
	{
		tag1->updateTagValueToValueSpace( err_page - 1);
		tag1Value = tag1->getValue();
	}

	start_num = EM_ERROR_MESSAGE_ONEPAGE_NUMBER * tag1Value;
	for(i = start_num; i < start_num + EM_ERROR_MESSAGE_ONEPAGE_NUMBER; i++)
	{
		if((EM_enErrLevelNoErr < astErrItem[i].u16ErrLevel) && ((EM_enErrLevelRecover > astErrItem[i].u16ErrLevel)) && (0 != astErrItem[i].u16OccurCount))
		{
			#if 0
			#ifdef PC_SIM
			sprintf_s(tmp_buf0, sizeof(tmp_buf0), "%2d,%2d,%4d,%3d,%3d,%3d,%3d,%5d,%8d\n",
			#endif
			#if defined(EMBEDDED) && defined(VERSION_2)
			snprintf(tmp_buf0, sizeof(tmp_buf0), "%2d,%2d,%4d,%3d,%3d,%3d,%3d,%5d,%8d\n",
			#endif
				astErrItem[i].u16ErrType, astErrItem[i].u16ErrLevel, astErrItem[i].u16OccurCount, astErrItem[i].u16PageID, astErrItem[i].u16CanvasID,
				astErrItem[i].u16SubcanvasID, astErrItem[i].u16WidgetID, astErrItem[i].u16TagID, astErrItem[i].u32TagValue);
			strcat(tmp_buf, tmp_buf0);
			#endif
			#ifdef PC_SIM
			sprintf_s(tmp_buf0, sizeof(tmp_buf0), "%2d %5d %11d\n",
			#endif
			#if defined(EMBEDDED) && defined(VERSION_2)
			snprintf(tmp_buf0, sizeof(tmp_buf0), "%2d %5d %11d\n",
			#endif
				astErrItem[i].u16ErrType, astErrItem[i].u16TagID, astErrItem[i].u32TagValue);
			strcat(tmp_buf, tmp_buf0);
		}
	}
	setStrTagValue(SYSTEM_ERROR_MESSAGE_TAG, tmp_buf);
	#endif
	
	return AHMI_FUNC_SUCCESS;
}

#ifndef CHIP_GC9002_NoHyper
CopyPaste_Struct mCopyPaste_Struct[CopyPaste_Number_MAX] = {0};
PointClass LTPoint(0x5A5A5A5A, 0x5A5A5A5A), RBPoint(0x5A5A5A5A, 0x5A5A5A5A);
u8 CopyPastePageID = 0;
#endif //endof #ifndef CHIP_GC9002_NoHyper
void ActionProcessorClass::copyBackground_1(u32 u32_ImmValue)
{
	#ifndef CHIP_GC9002_NoHyper
	CopyPastePageID = (u32_ImmValue >> 24) & 0xFF;
	LTPoint.mPointX = (u32_ImmValue >> 12) & 0xFFF;
	LTPoint.mPointY = u32_ImmValue & 0xFFF;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
}

void ActionProcessorClass::copyBackground_2(u32 u32_ImmValue)
{
	#ifndef CHIP_GC9002_NoHyper
	u8 pagenum;

	pagenum = (u32_ImmValue >> 24) & 0xFF;

	if(pagenum == CopyPastePageID)
	{
		RBPoint.mPointX = (u32_ImmValue >> 12) & 0xFFF;
		RBPoint.mPointY = u32_ImmValue & 0xFFF;
	}
	#endif //endof #ifndef CHIP_GC9002_NoHyper
}

void ActionProcessorClass::pasteBackground(u16 u16_WidgetTagID, u32 u32_ImmValue)
{
	#ifndef CHIP_GC9002_NoHyper
	u16 widgetID, paste_pointx, paste_pointy, i;

	widgetID = u16_WidgetTagID;
	paste_pointx = (u32_ImmValue >> 12) & 0xFFF;
	paste_pointy = u32_ImmValue & 0xFFF;

	for(i = 0; i < CopyPaste_Number_MAX; i++)
	{
		if(((mCopyPaste_Struct[i].DestWidgetID == 0) || (mCopyPaste_Struct[i].DestWidgetID == widgetID)) &&
		   ((LTPoint.mPointX != 0x5A5A5A5A) && (LTPoint.mPointY != 0x5A5A5A5A) && (RBPoint.mPointX != 0x5A5A5A5A) && (RBPoint.mPointY != 0x5A5A5A5A)))
		{
			mCopyPaste_Struct[i].SourcePageID = CopyPastePageID;
			mCopyPaste_Struct[i].LTPointX = LTPoint.mPointX;
			mCopyPaste_Struct[i].LTPointY = LTPoint.mPointY;
			mCopyPaste_Struct[i].RBPointX = RBPoint.mPointX;
			mCopyPaste_Struct[i].RBPointY = RBPoint.mPointY;
			mCopyPaste_Struct[i].DestWidgetID = widgetID;
			mCopyPaste_Struct[i].PastePointX = paste_pointx;
			mCopyPaste_Struct[i].PastePointY = paste_pointy;

			//initial current CopyPastePageID and LT RB Point
			CopyPastePageID = 0;
			LTPoint.mPointX = 0x5A5A5A5A;
			LTPoint.mPointY = 0x5A5A5A5A;
			RBPoint.mPointX = 0x5A5A5A5A;
			RBPoint.mPointY = 0x5A5A5A5A;
			return;
		}
	}
	return;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
}

 //-----------------------------
// nameï¿½ï¿½ SetColor
// set the color of widget
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param colorTagID : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetColorExe(u16 tagID,u32 color)
{
	TagClassPtr curTag;
//	TagClassPtr colorTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	int i;

	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetColor: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		curTag = &TagPtr->getTag(tagID);
		if (curTag == NULL)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		
		if(curTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < curTag->mNumOfElementLinker; i++ )
			{
				if(curTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_WIDGET)
				{
					curWidgetLinkDataPtr = (WidgetLinkDataPtr)curTag->pTagELmementLinker[i].mLinkElementPtr;
					curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];
					//if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) continue;
					curWidget->setColor(&gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID], color);
				}
			}
		}
		
		if(curWidget == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}

	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// name:SetColor
// set the color of widget
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param colorTagID : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetColor(u8 u8_CdRun,u8 u8_CdRCd,u16 tagID,u32 color)
{    
    PLAR_tstAHMICmd stAHMICmd;
	
	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_COLOR_CODE;
        stAHMICmd.u32Para[1] = tagID;
        stAHMICmd.u32Para[2] = color;
        
		if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// name:SetKB
// set the keyboard container
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param colorTagID : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetKBExe(u16 tagID,u32 keyCode)
{
	TagClassPtr curTag;
//	TagClassPtr colorTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	int i;

	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetKB: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		curTag = &TagPtr->getTag(tagID);
		if (curTag == NULL)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		
		if(curTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < curTag->mNumOfElementLinker; i++ )
			{
				if(curTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_WIDGET)
				{
					curWidgetLinkDataPtr = (WidgetLinkDataPtr)curTag->pTagELmementLinker[i].mLinkElementPtr;
					curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];
					if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) continue;
					curWidget->setKB(keyCode);
				}
			}
		}
		
		if(curWidget == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}

	return AHMI_FUNC_SUCCESS;
}

 //-----------------------------
// name:SetKB
// set the keyboard container
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param colorTagID : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetKB(u8 u8_CdRun,u8 u8_CdRCd,u16 tagID,u32 keyCode)
{    
    PLAR_tstAHMICmd stAHMICmd;
	
	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_KB_CODE;
        stAHMICmd.u32Para[1] = tagID;
        stAHMICmd.u32Para[2] = keyCode;
      
		if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: SetKBHintExe
// set the keyboard container
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param idx :index of hint choiced
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetKBHintExe(u16 tagID,u32 idx)
{
	TagClassPtr curTag;
//	TagClassPtr colorTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	int i;

	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetKB: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		curTag = &TagPtr->getTag(tagID);
		if (curTag == NULL)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		
		if(curTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < curTag->mNumOfElementLinker; i++ )
			{
				if(curTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_WIDGET)
				{
					curWidgetLinkDataPtr = (WidgetLinkDataPtr)curTag->pTagELmementLinker[i].mLinkElementPtr;
					curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];
					if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) continue;
					curWidget->setKBHint(idx);
				}
			}
		}
		
		if(curWidget == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: SetKBHint
// set the keyboard container
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param idx :index of hint choiced
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::SetKBHint(u8 u8_CdRun,u8 u8_CdRCd,u16 tagID,u32 idx)
{    
    PLAR_tstAHMICmd stAHMICmd;
	
	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_KB_HINT_CODE;
        stAHMICmd.u32Para[1] = tagID;
        stAHMICmd.u32Para[2] = idx;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: ChangeKBHintsPage
// set the keyboard container
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param idx :index of hint choiced
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::ChangeKBHintsPageExe(u16 tagID,u32 step, u8 direction)
{
	TagClassPtr curTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	int i;

	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetKB: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		curTag = &TagPtr->getTag(tagID);
		if (curTag == NULL)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		
		if(curTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < curTag->mNumOfElementLinker; i++ )
			{
				if(curTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_WIDGET)
				{
					curWidgetLinkDataPtr = (WidgetLinkDataPtr)curTag->pTagELmementLinker[i].mLinkElementPtr;
					curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];
					if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) continue;
					curWidget->setKBHintsPage(step, direction);
				}
			}
		}
		
		if(curWidget == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: ChangeKBHintsPage
// set the keyboard container
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param idx :index of hint choiced
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::ChangeKBHintsPage(u8 u8_CdRun,u8 u8_CdRCd,u16 tagID,u32 step, u8 direction)
{   
    PLAR_tstAHMICmd stAHMICmd;
	
	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = PREV_KB_HINTS_CODE;
        stAHMICmd.u32Para[1] = tagID;
        stAHMICmd.u32Para[2] = step;
        stAHMICmd.u32Para[3] = direction;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}


//-----------------------------
// name: getKBExe
// set the keyboard container value to tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param colorTagID : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::GetKBExe(u16 kbTagId,u16 targetTagId)
{
	TagClassPtr kbTag;
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	int i;
	u16 widgetType;

	if((kbTagId >= ConfigData.NumofTags) || (targetTagId >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function GetKB: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		kbTag = &TagPtr->getTag(kbTagId);
		if (kbTag == NULL)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		
		targetTag = &TagPtr->getTag(targetTagId);
		if (targetTag == NULL)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		}
		
		if(kbTag->mNumOfElementLinker != (u8)-1)
		{
			for(i = 0; i < kbTag->mNumOfElementLinker; i++ )
			{
				if(kbTag->pTagELmementLinker[i].mElementType == ELEMENT_TYPE_WIDGET)
				{
					curWidgetLinkDataPtr = (WidgetLinkDataPtr)kbTag->pTagELmementLinker[i].mLinkElementPtr;
					curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];
					if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) continue;
					widgetType=curWidget->WidgetType;
					if (widgetType == KBCONTAINER)
					{
						//targetTag->setValue(curWidget->getKB(),targetTagId);
						curWidget->getKB(targetTagId);

						csi_kernel_sem_post( xSemaphoreSetValue );
						return AHMI_FUNC_SUCCESS;
					}
				}
			}
		}
		
		if(curWidget == NULL)
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: getKB
// set the keyboard container value to tag
//  
// @param u8_CdRun : 1bit whether to run this code, 0 to run directly
// @param u8_CdRCd : 1bit conditional running, if status == this, run
// @param tagID : the destination tag number
// @param colorTagID : the source tag number
// addtional comments :
//  
//-----------------------------
u32 ActionProcessorClass::GetKB(u8 u8_CdRun,u8 u8_CdRCd,u16 kbTagId,u16 targetTagId)
{  
    PLAR_tstAHMICmd stAHMICmd;
	
	if((kbTagId >= ConfigData.NumofTags) && (targetTagId >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = GET_KB_CODE;
        stAHMICmd.u32Para[1] = kbTagId;
        stAHMICmd.u32Para[2] = targetTagId;
		stAHMICmd.u32Para[3] = (uint32_t)1;//bNeedSyn = true;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
			csi_kernel_sem_wait( xSemaphoreSetValue, 1000);

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
			csi_kernel_sem_wait( xSemaphoreSetValue, 1000);
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
			csi_kernel_sem_wait( xSemaphoreSetValue, 1000);
		}
	}

	return AHMI_FUNC_SUCCESS;
}
//-----------------------------
// name:SetMove
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetX:offset X
// @param offsetY:offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetMoveExe(uint32_t mode, u16 DestTagID, u16 offsetX,u16 offsetY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);
                
                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curWidget->WidgetOffsetX = (s16)offsetX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curWidget->WidgetOffsetY = (s16)offsetY;
                }

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);
                
                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curCanvas->moffsetX = (s16)offsetX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curCanvas->moffsetY = (s16)offsetY;
                }

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetMove
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetX:offset X
// @param offsetY:offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetMove(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 offsetX,u16 offsetY)
{
    PLAR_tstAHMICmd stAHMICmd;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_MOVE_CODE;
        stAHMICmd.u32Para[1] = mode | DestTagID;
        stAHMICmd.u32Para[2] = offsetX;
        stAHMICmd.u32Para[3] = offsetY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}


u32 ActionProcessorClass::SetMove(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetMove: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}
	
	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetMove: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetMove: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetMove: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetMove: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		SetMoveExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		SetMove(u8_CdRun, u8_CdRCd, DestTagClassListTag, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetZoomExe
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomX:zoom X
// @param zoomY:zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetZoomExe(uint32_t mode, u16 DestTagID, u16 zoomX,u16 zoomY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curWidget->MatrixZoomX = (s16)zoomX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curWidget->MatrixZoomY = (s16)zoomY;
                }

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curCanvas->mZoomX = (s16)zoomX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curCanvas->mZoomY = (s16)zoomY;
                }

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetZoom
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomX:zoom X
// @param zoomY:zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetZoom(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 zoomX,u16 zoomY)
{
	PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_ZOOM_CODE;
        stAHMICmd.u32Para[1] = mode | DestTagID;
        stAHMICmd.u32Para[2] = zoomX;
        stAHMICmd.u32Para[3] = zoomY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::SetZoom(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetZoom: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetZoom: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetZoom: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetZoom: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		SetZoomExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		SetZoom(u8_CdRun, u8_CdRCd, DestTagClassListTag, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationCenterExe
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:rotate center pointer X
// @param centerY:rotate center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationCenterExe(uint32_t mode, u16 DestTagID, u16 centerX, u16 centerY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curWidget->MatrixRotateCenterX = (s16)centerX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curWidget->MatrixRotateCenterY = (s16)centerY;
                }

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curCanvas->mRotateCenterX = (s16)centerX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curCanvas->mRotateCenterY = (s16)centerY;
                }

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationCenter
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:rotate center pointer X
// @param centerY:rotate center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationCenter(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 centerX,u16 centerY)
{
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_ROTATION_CENTER_CODE;
        stAHMICmd.u32Para[1] = mode | DestTagID;
        stAHMICmd.u32Para[2] = centerX;
        stAHMICmd.u32Para[3] = centerY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::SetRotationCenter(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		SetRotationCenterExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		SetRotationCenter(u8_CdRun, u8_CdRCd, DestTagClassListTag, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetScaleCenterExe
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:scale center pointer X
// @param centerY:scale center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetScaleCenterExe(uint32_t mode, u16 DestTagID, u16 centerX, u16 centerY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetScaleCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curWidget->MatrixScaleCenterX = (s16)centerX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curWidget->MatrixScaleCenterY = (s16)centerY;
                }

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curCanvas->mScaleCenterX = (s16)centerX;
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curCanvas->mScaleCenterY = (s16)centerY;
                }

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetScaleCenter
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:scale center pointer X
// @param centerY:scale center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetScaleCenter(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 centerX,u16 centerY)
{
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetScaleCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_SCALE_CENTER_CODE;
        stAHMICmd.u32Para[1] = mode | DestTagID;
        stAHMICmd.u32Para[2] = centerX;
        stAHMICmd.u32Para[3] = centerY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::SetScaleCenter(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetScaleCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetScaleCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetScaleCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetScaleCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetScaleCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetScaleCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		SetScaleCenterExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		SetScaleCenter(u8_CdRun, u8_CdRCd, DestTagClassListTag, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationAngle
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param angle:rotete angle
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationAngleExe(u16 DestTagID, u16 angle)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationAngle: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				curWidget->MatrixRotateAngle = (s16)(angle << 4);

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				curCanvas->mRotateAngle = (s16)(angle << 4);

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationAngle
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param angle:rotete angle
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationAngle(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angle)
{
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_ROTATION_ANGLE_CODE;
        stAHMICmd.u32Para[1] = DestTagID;
        stAHMICmd.u32Para[2] = angle;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetProjection
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionX: projection angle X
// @param projectionY: projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetProjectionExe(uint32_t mode, u16 DestTagID, u16 projectionX,u16 projectionY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetProjection: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curWidget->MatrixProjectionX = (s16)(projectionX << 4);
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curWidget->MatrixProjectionY = (s16)(projectionY << 4);
                }

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                if(mode & MODIFY_VIEW_ELEMENT_X)
                {
                    curCanvas->mProjectionX = (s16)(projectionX << 4);
                }
                if(mode & MODIFY_VIEW_ELEMENT_Y)
                {
                    curCanvas->mProjectionY = (s16)(projectionX << 4);
                }

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetProjection
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionX: projection angle X
// @param projectionY: projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetProjection(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 projectionX,u16 projectionY)
{  
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = SET_PROJECTION_CODE;
        stAHMICmd.u32Para[1] = mode | DestTagID;
        stAHMICmd.u32Para[2] = projectionX;
        stAHMICmd.u32Para[3] = projectionY;

        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::SetProjection(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetProjection: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetProjection: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetProjection: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetProjection: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function SetProjection: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function SetProjection: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		SetProjectionExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		SetProjection(u8_CdRun, u8_CdRCd, DestTagClassListTag, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetMoveTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetTagID: tag id, offset X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetMoveTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetTagID, bool isBeforeLoadAction)
{
	TagClassPtr targetTag;
	TagClassPtr offsetTag;
    uint16_t OffsetX;
    uint16_t OffsetY;
	
	if((DestTagID >= ConfigData.NumofTags) || (offsetTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetMoveTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag = &TagPtr->getTag(DestTagID);
	offsetTag = &TagPtr->getTag(offsetTagID);

	if((targetTag == NULL) || (offsetTag == NULL))
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    OffsetX = (u16)((offsetTag->getValue() >> 16) & 0xFFFF);
	OffsetY = (u16)(offsetTag->getValue() & 0xFFFF);
    
	if(isBeforeLoadAction)
	{
		SetMoveExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagID, OffsetX, OffsetY);
	}
	else
	{
		SetMove(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, OffsetX, OffsetY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetZoomTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomTagID:tag id, zoom X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetZoomTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomTagID, bool isBeforeLoadAction)
{
	TagClassPtr zoomTag;
    uint16_t zoomX;
    uint16_t zoomY;
	
	if((DestTagID >= ConfigData.NumofTags) || (zoomTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetZoomTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	zoomTag = &TagPtr->getTag(zoomTagID);

	if(zoomTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

    zoomX = (u16)((zoomTag->getValue() >> 16) & 0xFFFF);
	zoomY = (u16)(zoomTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		SetZoomExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagID, zoomX, zoomY);
	}
	else
	{
		SetZoom(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, zoomX, zoomY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetScaleCenterTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerTagID:tag id, scale center pointer X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetScaleCenterTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerTag;
    uint16_t centerX;
    uint16_t centerY;

	if((DestTagID >= ConfigData.NumofTags) || (centerTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetScaleCenterTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerTag = &TagPtr->getTag(centerTagID);

	if(centerTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerX = (u16)((centerTag->getValue() >> 16) & 0xFFFF);
    centerY = (u16)(centerTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		SetScaleCenterExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagID, centerX, centerY);
	}
	else
	{
		SetScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, centerX, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationCenterTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerTagID:tag id, rotate center pointer X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationCenterTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerTag;
    uint16_t centerX;
    uint16_t centerY;

	if((DestTagID >= ConfigData.NumofTags) || (centerTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetRotationCenterTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerTag = &TagPtr->getTag(centerTagID);

	if(centerTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerX = (u16)((centerTag->getValue() >> 16) & 0xFFFF);
    centerY = (u16)(centerTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		SetRotationCenterExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagID, centerX, centerY);
	}
	else
	{
		SetRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, centerX, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationAngleTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param angleTagID:tag id, rotete angle
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationAngleTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angleTagID, bool isBeforeLoadAction)
{
	TagClassPtr angleTag;
    uint16_t angle;
	
	if((DestTagID >= ConfigData.NumofTags) || (angleTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetRotationAngleTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	angleTag = &TagPtr->getTag(angleTagID);

	if(angleTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
		 
    angle = angleTag->getValue();

	if(isBeforeLoadAction)
	{
		SetRotationAngleExe(DestTagID, angle);
	}
	else
	{
		SetRotationAngle(u8_CdRun, u8_CdRCd, DestTagID, angle);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetProjectionTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionTagID:tag id,  projection angle X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetProjectionTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionTagID, bool isBeforeLoadAction)
{
	TagClassPtr projectionTag;
    uint16_t projectionX;
    uint16_t projectionY;
	
	if((DestTagID >= ConfigData.NumofTags) || (projectionTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetProjectionTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	projectionTag = &TagPtr->getTag(projectionTagID);

	if(projectionTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    projectionX = (u16)((projectionTag->getValue() >> 16) & 0xFFFF);
	projectionY = (u16)(projectionTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		SetProjectionExe(MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, DestTagID, projectionX, projectionY);
	}
	else
	{
		SetProjection(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X | MODIFY_VIEW_ELEMENT_Y, projectionX, projectionY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeMoveExe
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetX:offset X
// @param offsetY:offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeMoveExe(u16 DestTagID, u16 offsetX,u16 offsetY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeMove: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				if((s16(curWidget->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT) || (s16(curWidget->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_MoveOverrun);
				if((s16(curWidget->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT) || (s16(curWidget->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_MoveOverrun);

				curWidget->WidgetOffsetX += (s16)offsetX;
				curWidget->WidgetOffsetY += (s16)offsetY;

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				if((s16(curCanvas->moffsetX) > MAX_WIDTH_AND_HEIGHT) || (s16(curCanvas->moffsetX) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_MoveOverrun);
				if((s16(curCanvas->moffsetY) > MAX_WIDTH_AND_HEIGHT) || (s16(curCanvas->moffsetY) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_MoveOverrun);

				curCanvas->moffsetX += (s16)offsetX;
				curCanvas->moffsetY += (s16)offsetY;

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeMove
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetX:offset X
// @param offsetY:offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeMove(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetX,u16 offsetY)
{   
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = CHANGE_MOVE_CODE;
        stAHMICmd.u32Para[1] = DestTagID;
        stAHMICmd.u32Para[2] = offsetX;
        stAHMICmd.u32Para[3] = offsetY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::ChangeMove(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeMove: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeMove: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeMove: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeMove: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeMove: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeMove: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		ChangeMoveExe(DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		ChangeMove(u8_CdRun, u8_CdRCd, DestTagClassListTag, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeZoom
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomX:zoom X
// @param zoomY:zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeZoomExe(u16 DestTagID, u16 zoomX,u16 zoomY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	s32 temp = 0;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                #if (defined PC_SIM) 
				temp = (s16)curWidget->MatrixZoomX;
				temp *= (s16)zoomX;

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
				curWidget->MatrixZoomX = (s16)(temp >> 9);

				temp = (s16)curWidget->MatrixZoomY;
				temp *= (s16)zoomY;

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
				curWidget->MatrixZoomY = (s16)(temp >> 9);
                #endif
                
                #if (defined VERSION_2) && (defined EMBEDDED) 
                temp = (s16)curWidget->MatrixZoomX;
                temp = csi_fix_mult(temp, (s16)zoomX, 9);

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
                curWidget->MatrixZoomX = (s32)temp;
                
                temp = (s16)curWidget->MatrixZoomY;
                temp = csi_fix_mult(temp, (s16)zoomY, 9);

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
                curWidget->MatrixZoomY = (s32)temp;
                #endif

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

                #if (defined PC_SIM) 
				temp = curCanvas->mZoomX;
				temp *= (s16)zoomX;

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
				curCanvas->mZoomX = (s16)(temp >> 9);

				temp = curCanvas->mZoomY;
				temp *= (s16)zoomY;

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
				curCanvas->mZoomY = (s16)(temp >> 9);
                #endif
                
                
                #if (defined VERSION_2) && (defined EMBEDDED) 
                temp = curCanvas->mZoomX;
                temp = csi_fix_mult(temp, (s16)zoomX, 9);

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
                curCanvas->mZoomX = (s32)temp;
                
                temp = curCanvas->mZoomY;
                temp = csi_fix_mult(temp, (s16)zoomY, 9);

				if((temp > MAX_ZOOM) || (temp < -MAX_ZOOM))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ZoomOverrun);
                curCanvas->mZoomY = (s32)temp;
                #endif

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeZoom
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomX:zoom X
// @param zoomY:zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeZoom(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomX,u16 zoomY)
{   
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = CHANGE_ZOOM_CODE;
        stAHMICmd.u32Para[1] = DestTagID;
        stAHMICmd.u32Para[2] = zoomX;
        stAHMICmd.u32Para[3] = zoomY;
        
       if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::ChangeZoom(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{
	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeZoom: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeZoom: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeZoom: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeZoom: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeZoom: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeZoom: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		ChangeZoomExe(DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		ChangeZoom(u8_CdRun, u8_CdRCd, DestTagClassListTag, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeScaleCenter
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:scale center pointer X
// @param centerY:scale center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeScaleCenterExe(u16 DestTagID, u16 centerX,u16 centerY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeScaleCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				if((s16(curWidget->MatrixScaleCenterX) > MAX_WIDTH_AND_HEIGHT) || (s16(curWidget->MatrixScaleCenterX) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ScaleCenterOverrun);
				if((s16(curWidget->MatrixScaleCenterY) > MAX_WIDTH_AND_HEIGHT) || (s16(curWidget->MatrixScaleCenterY) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ScaleCenterOverrun);

				curWidget->MatrixScaleCenterX += (s16)centerX;
				curWidget->MatrixScaleCenterY += (s16)centerY;

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				if((s16(curCanvas->mScaleCenterX) > MAX_WIDTH_AND_HEIGHT) || (s16(curCanvas->mScaleCenterX) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ScaleCenterOverrun);
				if((s16(curCanvas->mScaleCenterY) > MAX_WIDTH_AND_HEIGHT) || (s16(curCanvas->mScaleCenterY) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ScaleCenterOverrun);

				curCanvas->mScaleCenterX += (s16)centerX;
				curCanvas->mScaleCenterY += (s16)centerY;

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeScaleCenter
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:scale center pointer X
// @param centerY:scale center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeScaleCenter(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX,u16 centerY)
{ 
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeScaleCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = CHANGE_SCALE_CENTER_CODE;
        stAHMICmd.u32Para[1] = DestTagID;
        stAHMICmd.u32Para[2] = centerX;
        stAHMICmd.u32Para[3] = centerY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::ChangeScaleCenter(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{

	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeScaleCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function ChangeSclaeCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeScaleCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeScaleCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeScaleCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeScaleCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeScaleCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeScaleCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		ChangeScaleCenterExe(DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		ChangeScaleCenter(u8_CdRun, u8_CdRCd, DestTagClassListTag, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationCenter
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:rotate center pointer X
// @param centerY:rotate center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationCenterExe(u16 DestTagID, u16 centerX,u16 centerY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				if((s16(curWidget->MatrixRotateCenterX) > MAX_WIDTH_AND_HEIGHT) || (s16(curWidget->MatrixRotateCenterX) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_RotateCenterOverrun);
				if((s16(curWidget->MatrixRotateCenterY) > MAX_WIDTH_AND_HEIGHT) || (s16(curWidget->MatrixRotateCenterY) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_RotateCenterOverrun);

				curWidget->MatrixRotateCenterX += (s16)centerX;
				curWidget->MatrixRotateCenterY += (s16)centerY;

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				if((s16(curCanvas->mRotateCenterX) > MAX_WIDTH_AND_HEIGHT) || (s16(curCanvas->mRotateCenterX) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_RotateCenterOverrun);
				if((s16(curCanvas->mRotateCenterY) > MAX_WIDTH_AND_HEIGHT) || (s16(curCanvas->mRotateCenterY) < -MAX_WIDTH_AND_HEIGHT))
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_RotateCenterOverrun);

				curCanvas->mRotateCenterX += (s16)centerX;
				curCanvas->mRotateCenterY += (s16)centerY;

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationCenter
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:rotate center pointer X
// @param centerY:rotate center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationCenter(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX,u16 centerY)
{ 
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = CHANGE_ROTATION_CENTER_CODE;
        stAHMICmd.u32Para[1] = DestTagID;
        stAHMICmd.u32Para[2] = centerX;
        stAHMICmd.u32Para[3] = centerY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::ChangeRotationCenter(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{

	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeRotationCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeRotationCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeRotationCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeRotationCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeRotationCenter: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeRotationCenter: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		ChangeRotationCenterExe(DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		ChangeRotationCenter(u8_CdRun, u8_CdRCd, DestTagClassListTag, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationAngle
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param angle:rotete angle
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationAngleExe(u16 DestTagID, u16 angle)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeRotationAngle: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				curWidget->MatrixRotateAngle += (s16)(angle << 4);

				while ((s16)curWidget->MatrixRotateAngle >= 360 * 16)
					curWidget->MatrixRotateAngle -= 360 * 16;
				while ((s16)curWidget->MatrixRotateAngle <= -360 * 16)
					curWidget->MatrixRotateAngle += 360 * 16;

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				curCanvas->mRotateAngle += (s16)(angle << 4);

				while ((s16)curCanvas->mRotateAngle >= 360 * 16)
					curCanvas->mRotateAngle -= 360 * 16;
				while ((s16)curCanvas->mRotateAngle <= -360 * 16)
					curCanvas->mRotateAngle += 360 * 16;

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationAngle
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param angle:rotete angle
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationAngle(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angle)
{    
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = CHANGE_ROTATION_ANGLE_CODE;
        stAHMICmd.u32Para[1] = DestTagID;
        stAHMICmd.u32Para[2] = angle;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeProjectionExe
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionX: projection angle X
// @param projectionY: projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeProjectionExe(u16 DestTagID, u16 projectionX,u16 projectionY)
{
	TagClassPtr targetTag;
	WidgetClassPtr curWidget = NULL;
	WidgetLinkDataPtr curWidgetLinkDataPtr;
	CanvasLinkDataPtr curCanvasLinkDataPtr;
	CanvasClassPtr curCanvas = NULL;
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeProjection: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	targetTag =  &TagPtr->getTag(DestTagID);

	if(targetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

//	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
		if(targetTag->mNumOfElementLinker == 1)
		{
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_WIDGET)
			{
				curWidgetLinkDataPtr = (WidgetLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curWidget = &gPagePtr[curWidgetLinkDataPtr->LinkWidgetPageID].pWidgetList[curWidgetLinkDataPtr->LinkWidgetID];

				if(curWidgetLinkDataPtr->LinkWidgetPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				curWidget->MatrixProjectionX += (s16)(projectionX << 4);
				curWidget->MatrixProjectionY += (s16)(projectionY << 4);

				if((s16(curWidget->MatrixProjectionX) >= (2*MAX_ANGLE << 4)) || (s16(curWidget->MatrixProjectionX) <= (-2*MAX_ANGLE << 4)))
				{
					s16 temp_X = 0, temp_Y = 0, temp1 = 0;
					int value = 0;

					temp_X = projectionX;
					temp_Y = projectionY;
					temp1 = s16(curWidget->MatrixProjectionX);
					value = s32(temp1) / (s32(temp_X) << 4);

					curWidget->MatrixProjectionX -= (value * (temp_X << 4));
					curWidget->MatrixProjectionY -= (value * (temp_Y << 4));
				}
				if((s16(curWidget->MatrixProjectionY) >= (2*MAX_ANGLE << 4)) || (s16(curWidget->MatrixProjectionY) <= (-2*MAX_ANGLE << 4)))
				{
					s16 temp_X = 0, temp_Y = 0, temp1 = 0;
					int value = 0;

					temp_X = projectionX;
					temp_Y = projectionY;
					temp1 = s16(curWidget->MatrixProjectionY);
					value = s32(temp1) / (s32(temp_Y) << 4);

					curWidget->MatrixProjectionX -= (value * (temp_X << 4));
					curWidget->MatrixProjectionY -= (value * (temp_Y << 4));
				}

				curWidget->SetWidgetMatrixRefreshFlag();

			}
			if(targetTag->pTagELmementLinker->mElementType == ELEMENT_TYPE_CANVAS)
			{
				curCanvasLinkDataPtr = (CanvasLinkDataPtr)targetTag->pTagELmementLinker[0].mLinkElementPtr;
				curCanvas = &gPagePtr[curCanvasLinkDataPtr->LinkCanvasPageID].pCanvasList[curCanvasLinkDataPtr->LinkCanvasID];

				if(curCanvasLinkDataPtr->LinkCanvasPageID != WorkingPageID) 
					return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_ElementIndexErr);

				if((s16(curCanvas->mProjectionX) >= (2*MAX_ANGLE << 4)) || (s16(curCanvas->mProjectionX) <= (-2*MAX_ANGLE << 4)))
				{
					s16 temp_X = 0, temp_Y = 0, temp1 = 0;
					int value = 0;

					temp_X = projectionX;
					temp_Y = projectionY;
					temp1 = s16(curCanvas->mProjectionX);
					value = s32(temp1) / (s32(temp_X) << 4);

					curCanvas->mProjectionX -= (value * (temp_X << 4));
					curCanvas->mProjectionY -= (value * (temp_Y << 4));
				}
				if((s16(curCanvas->mProjectionY) >= (2*MAX_ANGLE << 4)) || (s16(curCanvas->mProjectionY) <= (-2*MAX_ANGLE << 4)))
				{
					s16 temp_X = 0, temp_Y = 0, temp1 = 0;
					int value = 0;

					temp_X = projectionX;
					temp_Y = projectionY;
					temp1 = s16(curCanvas->mProjectionY);
					value = s32(temp1) / (s32(temp_Y) << 4);

					curCanvas->mProjectionX -= (value * (temp_X << 4));
					curCanvas->mProjectionY -= (value * (temp_Y << 4));
				}

				curCanvas->mProjectionX += (s16)(projectionX << 4);
				curCanvas->mProjectionY += (s16)(projectionY << 4);

				curCanvas->SetCanvasMatrixRefreshFlag();
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeProjection
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionX: projection angle X
// @param projectionY: projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeProjection(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionX,u16 projectionY)
{
    PLAR_tstAHMICmd stAHMICmd;
    
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoom: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(!u8_CdRun || (u8_CdRun && mStatusRegister == u8_CdRCd ))
	{
        memset(&stAHMICmd, 0, sizeof(stAHMICmd));
        stAHMICmd.u32CmdType = enCmdType_ActionExe;
        stAHMICmd.u32PageID = WorkingPageID;
        stAHMICmd.u32Para[0] = CHANGE_PROJECTION_CODE;
        stAHMICmd.u32Para[1] = DestTagID;
        stAHMICmd.u32Para[2] = projectionX;
        stAHMICmd.u32Para[3] = projectionY;
        
        if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

u32 ActionProcessorClass::ChangeProjection(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction)
{

	u32 Source1Value, Source2Value;
	u16 DestTagClassListTag, Source1TagClassListTag, Source2TagClassListTag;

	// check mTagType of tag
	DestTagClassListTag = *(u16 *)(pPC + 2);

	if(DestTagClassListTag >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
	if( TAG_IS_STR(TagPtr->getTag(DestTagClassListTag).mTagType) )
	{
		ERROR_PRINT("ERROR in function SetRotationCenter: tag type is not number");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
	}

	switch((*(u16 *)pPC) & 0xC000)
	{
		case SOURCE1IMM_SOURCE2IMM:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2Value = (*(u16 *)(pPC + 8)) | ((*(u16 *)(pPC + 10)) << 16);

			*pOffSet = 12;
			break;

		case SOURCE1TAG_SOURCE2IMM:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2Value =  (*(u16 *)(pPC + 6)) | ((*(u16 *)(pPC + 8)) << 16);

			if(Source1TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeProjection: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeProjection: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1IMM_SOURCE2TAG:
			Source1Value = (*(u16 *)(pPC + 4)) | ((*(u16 *)(pPC + 6)) << 16);
			Source2TagClassListTag = *(u16 *)(pPC + 8);

			if(Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeProjection: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeProjection: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();
			
			*pOffSet = 10;
			break;

		case SOURCE1TAG_SOURCE2TAG:
			Source1TagClassListTag = *(u16 *)(pPC + 4);
			Source2TagClassListTag = *(u16 *)(pPC + 6);

			if(Source1TagClassListTag >= ConfigData.NumofTags || Source2TagClassListTag >= ConfigData.NumofTags)
			{
				ERROR_PRINT("ERROR in function ChangeProjection: tag index out of baundary");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
			}
			if( TAG_IS_STR(TagPtr->getTag(Source1TagClassListTag).mTagType) || TAG_IS_STR(TagPtr->getTag(Source2TagClassListTag).mTagType) )
			{
				ERROR_PRINT("ERROR in function ChangeProjection: tag type is not number");
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagTypeErr);
			}
			Source1Value = TagPtr->getTag(Source1TagClassListTag).getValue();
			Source2Value = TagPtr->getTag(Source2TagClassListTag).getValue();

			*pOffSet = 8;
			break;

		default:
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_DataFrameErr);

	}

	if(isBeforeLoadAction)
	{
		ChangeProjectionExe(DestTagClassListTag, Source1Value, Source2Value);
	}
	else
	{
		ChangeProjection(u8_CdRun, u8_CdRCd, DestTagClassListTag, Source1Value, Source2Value);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeMoveTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetTagID: tag id, offset X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeMoveTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetTagID, bool isBeforeLoadAction)
{
	TagClassPtr offsetTag;
    uint16_t offsetX;
    uint16_t offsetY;
	
	if((DestTagID >= ConfigData.NumofTags) || (offsetTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeMoveTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	offsetTag = &TagPtr->getTag(offsetTagID);

	if(offsetTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

    offsetX = (u16)(offsetTag->getValue() >> 16) & 0xFFFF;
    offsetY = (u16)(offsetTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		ChangeMoveExe(DestTagID, offsetX, offsetY);
	}
	else
	{
		ChangeMove(u8_CdRun, u8_CdRCd, DestTagID, offsetX, offsetY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeZoomTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomTagID:tag id, zoom X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeZoomTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomTagID, bool isBeforeLoadAction)
{
	TagClassPtr zoomTag;
    uint16_t zoomX;
    uint16_t zoomY;
	
	if((DestTagID >= ConfigData.NumofTags) || (zoomTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeZoomTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	zoomTag = &TagPtr->getTag(zoomTagID);

	if(zoomTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

    zoomX = (u16)((zoomTag->getValue() >> 16) & 0xFFFF);
    zoomY = (u16)(zoomTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		ChangeZoomExe(DestTagID, zoomX, zoomY);
	}
	else
	{
		ChangeZoom(u8_CdRun, u8_CdRCd, DestTagID, zoomX, zoomY);
	}
	
	return AHMI_FUNC_SUCCESS;
}


//-----------------------------
// name:ChangeScaleCenterTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerTagID:tag id, scale center pointer X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeScaleCenterTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerTag;
    uint16_t centerX;
    uint16_t centerY;

	if((DestTagID >= ConfigData.NumofTags) || (centerTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeScaleCenterTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerTag = &TagPtr->getTag(centerTagID);

	if(centerTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerX = (u16)((centerTag->getValue() >> 16) & 0xFFFF);
    centerY = (u16)(centerTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		ChangeScaleCenterExe(DestTagID, centerX, centerY);
	}
	else
	{
		ChangeScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, centerX, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationCenterTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerTagID:tag id, rotate center pointer X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationCenterTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerTag;
    uint16_t centerX;
    uint16_t centerY;

	if((DestTagID >= ConfigData.NumofTags) || (centerTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeRotationCenterTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerTag = &TagPtr->getTag(centerTagID);

	if(centerTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerX = (u16)((centerTag->getValue() >> 16) & 0xFFFF);
    centerY = (u16)(centerTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		ChangeRotationCenterExe(DestTagID, centerX, centerY);
	}
	else
	{
		ChangeRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, centerX, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationAngleTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param angleTagID:tag id, rotete angle
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationAngleTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angleTagID, bool isBeforeLoadAction)
{
	TagClassPtr angleTag;
    uint16_t angle;
	
	if((DestTagID >= ConfigData.NumofTags) || (angleTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeRotationAngleTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	angleTag = &TagPtr->getTag(angleTagID);

	if(angleTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

    angle = (u16)angleTag->getValue();

	if(isBeforeLoadAction)
	{
		ChangeRotationAngleExe(DestTagID, angle);
	}
	else
	{
		ChangeRotationAngle(u8_CdRun, u8_CdRCd, DestTagID, angle);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeProjectionTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionTagID:tag id,  projection angle X & Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeProjectionTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionTagID, bool isBeforeLoadAction)
{
	TagClassPtr projectionTag;
    uint16_t projectionX;
    uint16_t projectionY;
	
	if((DestTagID >= ConfigData.NumofTags) || (projectionTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeProjectionTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	projectionTag = &TagPtr->getTag(projectionTagID);

	if(projectionTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    projectionX = (u16)((projectionTag->getValue() >> 16) & 0xFFFF);
    projectionY = (u16)(projectionTag->getValue() & 0xFFFF);

	if(isBeforeLoadAction)
	{
		ChangeProjectionExe(DestTagID, projectionX, projectionY);
	}
	else
	{
		ChangeProjection(u8_CdRun, u8_CdRCd, DestTagID, projectionX, projectionY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetMoveX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetX:offset X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetMoveX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMoveX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		SetMoveExe(MODIFY_VIEW_ELEMENT_X, DestTagID, offsetX, 0);
	}
	else
	{
		SetMove(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, offsetX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetMoveY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetY:offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetMoveY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetY, bool isBeforeLoadAction)
{	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetMoveY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		SetMoveExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, offsetY);
	}
	else
	{
		SetMove(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, offsetY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetZoomX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomX:zoom X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetZoomX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoomX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}	
    
	if(isBeforeLoadAction)
	{
		SetZoomExe(MODIFY_VIEW_ELEMENT_X, DestTagID, zoomX, 0);
	}
	else
	{
		SetZoom(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, zoomX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetZoomY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomY:zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetZoomY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomY, bool isBeforeLoadAction)
{	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetZoomY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		SetZoomExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, zoomY);
	}
	else
	{
		SetZoom(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, zoomY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetScaleCenterX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:scale center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetScaleCenterX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetScaleCenterX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(isBeforeLoadAction)
	{
		SetScaleCenterExe(MODIFY_VIEW_ELEMENT_X, DestTagID, centerX, 0);
	}
	else
	{
		SetScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, centerX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetScaleCenterY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerY:scale center pointer Y	
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetScaleCenterY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction)
{
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetScaleCenterY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		SetScaleCenterExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, centerY);
	}
	else
	{
		SetScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}


//-----------------------------
// name:SetRotationCenterX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:rotate center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationCenterX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenterX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(isBeforeLoadAction)
	{
		SetRotationCenterExe(MODIFY_VIEW_ELEMENT_X, DestTagID, centerX, 0);
	}
	else
	{
		SetRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, centerX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationCenterY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerY:rotate center pointer Y	
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationCenterY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction)
{
	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetRotationCenterY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		SetRotationCenterExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, centerY);
	}
	else
	{
		SetRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetProjectX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionX: projection angle X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetProjectionX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetProjectionX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		SetProjectionExe(MODIFY_VIEW_ELEMENT_X, DestTagID, projectionX, 0);
	}
	else
	{
		SetProjection(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, projectionX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetProjectY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionY: projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetProjectionY(u8 u8_CdRun, u8 u8_CdRCd, u16 DestTagID, u16 projectionY, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function SetProjectionY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(isBeforeLoadAction)
	{
		SetProjectionExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, projectionY);
	}
	else
	{
		SetProjection(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, projectionY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetMoveXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetXTagID: tag id, offset X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetMoveXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetXTagID, bool isBeforeLoadAction)
{
	TagClassPtr offsetXTag;
	uint16_t OffsetX;
	
	if((DestTagID >= ConfigData.NumofTags) || (offsetXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetMoveXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	offsetXTag = &TagPtr->getTag(offsetXTagID);

	if(offsetXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

    
    OffsetX = (u16)offsetXTag->getValue();
    
	if(isBeforeLoadAction)
	{
		SetMoveExe(MODIFY_VIEW_ELEMENT_X, DestTagID, OffsetX, 0);
	}
	else
	{
		SetMove(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, OffsetX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetMoveYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetYTagID: tag id, offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetMoveYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetYTagID, bool isBeforeLoadAction)
{
	TagClassPtr offsetYTag;
	uint16_t OffsetY;
	
	if((DestTagID >= ConfigData.NumofTags) || (offsetYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetMoveYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	offsetYTag = &TagPtr->getTag(offsetYTagID);

	if(offsetYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
	OffsetY = (u16)offsetYTag->getValue();
    
	if(isBeforeLoadAction)
	{
		SetMoveExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, OffsetY);
	}
	else
	{
		SetMove(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, OffsetY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetZoomXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomXTagID:tag id, zoom X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetZoomXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomXTagID, bool isBeforeLoadAction)
{
	TagClassPtr zoomXTag;
    uint16_t zoomX;
	
	if((DestTagID >= ConfigData.NumofTags) || (zoomXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetZoomXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	zoomXTag = &TagPtr->getTag(zoomXTagID);

	if(zoomXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    zoomX = (u16)zoomXTag->getValue();

	if(isBeforeLoadAction)
	{
		SetZoomExe(MODIFY_VIEW_ELEMENT_X, DestTagID, zoomX, 0);
	}
	else
	{
		SetZoom(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, zoomX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetZoomYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomYTagID:tag id, zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetZoomYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomYTagID, bool isBeforeLoadAction)
{
	TagClassPtr zoomYTag;
    uint16_t zoomY;
	
	if((DestTagID >= ConfigData.NumofTags) || (zoomYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetZoomYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	zoomYTag = &TagPtr->getTag(zoomYTagID);

	if(zoomYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    zoomY = (u16)zoomYTag->getValue();

	if(isBeforeLoadAction)
	{
		SetZoomExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, zoomY);
	}
	else
	{
		SetZoom(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, zoomY);
	}
	
	return AHMI_FUNC_SUCCESS;
}


//-----------------------------
// name:SetScaleCenterXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerXTagID:tag id, scale center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetScaleCenterXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerXTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerXTag;
    uint16_t centerX;
	
	if((DestTagID >= ConfigData.NumofTags) || (centerXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetScaleCenterXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerXTag = &TagPtr->getTag(centerXTagID);

	if(centerXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerX = (u16)centerXTag->getValue();

	if(isBeforeLoadAction)
	{
		SetScaleCenterExe(MODIFY_VIEW_ELEMENT_X, DestTagID, centerX, 0);
	}
	else
	{
		SetScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, centerX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetScaleCenterYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerYTagID:tag id, scale center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetScaleCenterYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerYTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerYTag;
    uint16_t centerY;
	
	if((DestTagID >= ConfigData.NumofTags) || (centerYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetScaleCenterYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerYTag = &TagPtr->getTag(centerYTagID);

	if(centerYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerY = (u16)centerYTag->getValue();

	if(isBeforeLoadAction)
	{
		SetScaleCenterExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, centerY);
	}
	else
	{
		SetScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}


//-----------------------------
// name:SetRotationCenterXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerXTagID:tag id, rotate center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationCenterXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerXTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerXTag;
    uint16_t centerX;
	
	if((DestTagID >= ConfigData.NumofTags) || (centerXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetRotationCenterXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerXTag = &TagPtr->getTag(centerXTagID);

	if(centerXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerX = (u16)centerXTag->getValue();

	if(isBeforeLoadAction)
	{
		SetRotationCenterExe(MODIFY_VIEW_ELEMENT_X, DestTagID, centerX, 0);
	}
	else
	{
		SetRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, centerX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetRotationCenterYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerYTagID:tag id, rotate center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetRotationCenterYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerYTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerYTag;
    uint16_t centerY;
	
	if((DestTagID >= ConfigData.NumofTags) || (centerYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetRotationCenterYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerYTag = &TagPtr->getTag(centerYTagID);

	if(centerYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerY = (u16)centerYTag->getValue();

	if(isBeforeLoadAction)
	{
		SetRotationCenterExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, centerY);
	}
	else
	{
		SetRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetProjectionXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionXTagID:tag id,  projection angle X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetProjectionXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionXTagID, bool isBeforeLoadAction)
{
	TagClassPtr projectionXTag;
    uint16_t projectionX;
	
	if((DestTagID >= ConfigData.NumofTags) || (projectionXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetProjectionXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	projectionXTag = &TagPtr->getTag(projectionXTagID);

	if(projectionXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

    projectionX = (u16)(projectionXTag->getValue());

	if(isBeforeLoadAction)
	{
		SetProjectionExe(MODIFY_VIEW_ELEMENT_X, DestTagID, projectionX, 0);
	}
	else
	{
		SetProjection(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_X, projectionX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:SetProjectionYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionYTagID:tag id,  projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::SetProjectionYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionYTagID, bool isBeforeLoadAction)
{
	TagClassPtr projectionYTag;
    uint16_t projectionY;
	
	if((DestTagID >= ConfigData.NumofTags) || (projectionYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function SetProjectionYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	projectionYTag = &TagPtr->getTag(projectionYTagID);

	if(projectionYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
	projectionY = (u16)(projectionYTag->getValue());

	if(isBeforeLoadAction)
	{
		SetProjectionExe(MODIFY_VIEW_ELEMENT_Y, DestTagID, 0, projectionY);
	}
	else
	{
		SetProjection(u8_CdRun, u8_CdRCd, DestTagID, MODIFY_VIEW_ELEMENT_Y, 0, projectionY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeMoveX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetX:offset X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeMoveX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeMoveX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeMoveExe(DestTagID, offsetX, 0);
	}
	else
	{
		ChangeMove(u8_CdRun, u8_CdRCd, DestTagID, offsetX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeMoveY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetY:offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeMoveY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetY, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeMoveY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeMoveExe(DestTagID, 0, offsetY);
	}
	else
	{
		ChangeMove(u8_CdRun, u8_CdRCd, DestTagID, 0, offsetY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeZoomX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomX:zoom X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeZoomX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeZoomX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeZoomExe(DestTagID, zoomX, 0x200);
	}
	else
	{
		ChangeZoom(u8_CdRun, u8_CdRCd, DestTagID, zoomX, 0x200);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeZoomY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomY:zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeZoomY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomY, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeZoomY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeZoomExe(DestTagID, 0x200, zoomY);
	}
	else
	{
		ChangeZoom(u8_CdRun, u8_CdRCd, DestTagID, 0x200, zoomY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeScaleCenterX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:scale center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeScaleCenterX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeScaleCenterX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeScaleCenterExe(DestTagID, centerX, 0);
	}
	else
	{
		ChangeScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, centerX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeScaleCenterY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerY:scale center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeScaleCenterY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeScaleCenterY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeScaleCenterExe(DestTagID, 0, centerY);
	}
	else
	{
		ChangeScaleCenter(u8_CdRun, u8_CdRCd, DestTagID, 0, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationCenterX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerX:rotate center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationCenterX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeRotationCenterX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeRotationCenterExe(DestTagID, centerX, 0);
	}
	else
	{
		ChangeRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, centerX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationCenterY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerY:rotate center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationCenterY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeRotationCenterY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}
    
	if(isBeforeLoadAction)
	{
		ChangeRotationCenterExe(DestTagID, 0, centerY);
	}
	else
	{
		ChangeRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, 0, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeProjectionX
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionX: projection angle X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeProjectionX(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionX, bool isBeforeLoadAction)
{	
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeProjectionX: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	if(isBeforeLoadAction)
	{
		ChangeProjectionExe(DestTagID, projectionX, 0);
	}
	else
	{
		ChangeProjection(u8_CdRun, u8_CdRCd, DestTagID, projectionX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeProjectionY
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionY: projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeProjectionY(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionY, bool isBeforeLoadAction)
{
	if(DestTagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in function ChangeProjectionY: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}	
    
	if(isBeforeLoadAction)
	{
		ChangeProjectionExe(DestTagID, 0, projectionY);
	}
	else
	{
		ChangeProjection(u8_CdRun, u8_CdRCd, DestTagID, 0, projectionY);
	}
    
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeMoveXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetXTagID: tag id, offset X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeMoveXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetXTagID, bool isBeforeLoadAction)
{
	TagClassPtr offsetXTag;
    uint16_t offsetX;
	
	if((DestTagID >= ConfigData.NumofTags) || (offsetXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeMoveXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	offsetXTag = &TagPtr->getTag(offsetXTagID);

	if(offsetXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    offsetX = (u16)offsetXTag->getValue();

	if(isBeforeLoadAction)
	{
		ChangeMoveExe(DestTagID, offsetX, 0);
	}
	else
	{
		ChangeMove(u8_CdRun, u8_CdRCd, DestTagID, offsetX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeMoveYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param offsetYTagID: tag id, offset Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeMoveYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetYTagID, bool isBeforeLoadAction)
{
	TagClassPtr offsetYTag;
    uint16_t offsetY;
	
	if((DestTagID >= ConfigData.NumofTags) || (offsetYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeMoveYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	offsetYTag =  &TagPtr->getTag(offsetYTagID);

	if(offsetYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    offsetY = (u16)offsetYTag->getValue();

	if(isBeforeLoadAction)
	{
		ChangeMoveExe(DestTagID, 0, offsetY);
	}
	else
	{
		ChangeMove(u8_CdRun, u8_CdRCd, DestTagID, 0, offsetY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeZoomXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomXTagID:tag id, zoom X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeZoomXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomXTagID, bool isBeforeLoadAction)
{
	TagClassPtr zoomXTag;
    uint16_t zoomX;
	
	if((DestTagID >= ConfigData.NumofTags) || (zoomXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeZoomXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	zoomXTag = &TagPtr->getTag(zoomXTagID);

	if(zoomXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);

    zoomX = (u16)(zoomXTag->getValue());

	if(isBeforeLoadAction)
	{
		ChangeZoomExe(DestTagID, zoomX, 0x200);
	}
	else
	{
		ChangeZoom(u8_CdRun, u8_CdRCd, DestTagID, zoomX, 0x200);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeZoomYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param zoomYTagID:tag id, zoom Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeZoomYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomYTagID, bool isBeforeLoadAction)
{
	TagClassPtr zoomYTag;
    uint16_t zoomY;
	
	if((DestTagID >= ConfigData.NumofTags) || (zoomYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeZoomYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	zoomYTag =  &TagPtr->getTag(zoomYTagID);

	if(zoomYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    zoomY = (u16)(zoomYTag->getValue());

	if(isBeforeLoadAction)
	{
		ChangeZoomExe(DestTagID, 0x200, zoomY);
	}
	else
	{
		ChangeZoom(u8_CdRun, u8_CdRCd, DestTagID, 0x200, zoomY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationCenterXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerXTagID:tag id, rotate center pointer X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationCenterXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerXTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerXTag;
    uint16_t centerX;
	
	if((DestTagID >= ConfigData.NumofTags) || (centerXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeRotationCenterXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerXTag = &TagPtr->getTag(centerXTagID);

	if(centerXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerX = (u16)(centerXTag->getValue());

	if(isBeforeLoadAction)
	{
		ChangeRotationCenterExe(DestTagID, centerX, 0);
	}
	else
	{
		ChangeRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, centerX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeRotationCenterYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param centerYTagID:tag id, rotate center pointer Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeRotationCenterYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerYTagID, bool isBeforeLoadAction)
{
	TagClassPtr centerYTag;
    uint16_t centerY;
	
	if((DestTagID >= ConfigData.NumofTags) || (centerYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeRotationCenterYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	centerYTag =  &TagPtr->getTag(centerYTagID);

	if(centerYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    centerY = (u16)(centerYTag->getValue());

	if(isBeforeLoadAction)
	{
		ChangeRotationCenterExe(DestTagID, 0, centerY);
	}
	else
	{
		ChangeRotationCenter(u8_CdRun, u8_CdRCd, DestTagID, 0, centerY);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeProjectionXTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionXTagID:tag id,  projection angle X
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeProjectionXTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionXTagID, bool isBeforeLoadAction)
{
	TagClassPtr projectionXTag;
    uint16_t projectionX;
	
	if((DestTagID >= ConfigData.NumofTags) || (projectionXTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeProjectionXTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	projectionXTag = &TagPtr->getTag(projectionXTagID);

	if(projectionXTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    projectionX = (u16)(projectionXTag->getValue());

	if(isBeforeLoadAction)
	{
		ChangeProjectionExe(DestTagID, projectionX, 0);
	}
	else
	{
		ChangeProjection(u8_CdRun, u8_CdRCd, DestTagID, projectionX, 0);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ChangeProjectionYTag
// set the keyboard container value to tag
//  
// @param u8_CdRun:1bit whether to run this code, 0 to run directly
// @param u8_CdRCd:1bit conditional running, if status == this, run
// @param DestTagID: tag id,find canvas or widget
// @param projectionYTagID:tag id,  projection angle Y
// addtional comments:
//  
//-----------------------------
u32 ActionProcessorClass::ChangeProjectionYTag(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionYTagID, bool isBeforeLoadAction)
{
	TagClassPtr projectionYTag;
    uint16_t projectionY;
	
	if((DestTagID >= ConfigData.NumofTags) || (projectionYTagID >= ConfigData.NumofTags))
	{
		ERROR_PRINT("ERROR in function ChangeProjectionYTag: tag index out of baundary");
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_TagIDOverrun);
	}

	projectionYTag =  &TagPtr->getTag(projectionYTagID);

	if(projectionYTag == NULL)
		return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NULLPointerErr);
    
    projectionY = (u16)(projectionYTag->getValue());
	
	if(isBeforeLoadAction)
	{
		ChangeProjectionExe(DestTagID, 0, projectionY);
	}
	else
	{
		ChangeProjection(u8_CdRun, u8_CdRCd, DestTagID, 0, projectionY);
	}
	
	return AHMI_FUNC_SUCCESS;
}
#endif 

