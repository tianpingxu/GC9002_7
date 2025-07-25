////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:    2024/8/7
// File Name:     ThreeDimensionalClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 1.0 - File Created  2024/8/7 by Yn
// Additional Comments:
//    definition of ThreeDimensional widget
// 
////////////////////////////////////////////////////////////////////////////////

#include "ThreeDimensionalClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
#include "sxGLApplication.h"
#include "NumberClass.h"
#ifdef AHMI_DEBUG
#include <stdio.h>
#include "trace.h"
#endif
#if (defined VERSION_2) && (defined EMBEDDED)
#include "csi_ahmi_bt656.h"
extern uint32_t SBAddress;
#endif

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;

extern QueueHandle_t		ActionInstructionQueue;
extern Easing gEasing;

extern uint32_t gNewSoureBufferSize;

#ifdef PC_SIM
extern u8       sourceBuffer[SoureBufferSize];
unsigned int refresh_cnt = 0;
//#define SXGL_TOUCH_TEST
#endif

//#define SXGL_ROTATE_TEST

#ifdef SXGL_ROTATE_TEST
extern GLushort cubeRotateAngleShort;
#endif

ThreeDimensionalClass::ThreeDimensionalClass(void)
{
}


ThreeDimensionalClass::~ThreeDimensionalClass(void)
{
}

//**************************************
//initialize the widget
//
//@param WidgetClassPtr p_wptr Pointer of widget
//@param u32 *u32p_sourceShift,   //pointer of sourcebuffer
//@param u8_pageRefresh          //whether refresh the page. if refresh, the value of the widget also refresh.
//@param u8 RefreshType ,        //the type of animation
//@param TileBoxClassPtr pTileBox, //tile box @see TileBoxClass
//@param u8 staticTextureEn          //whether to use the static buffer, not used now
//**************************************
funcStatus ThreeDimensionalClass::initWidget(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,   
	u32 *u32p_sourceShift,  
	NewMatrixPtr pWidgetMatrix,
	u8 u8_pageRefresh,       
	u8 RefreshType ,       
	TileBoxClassPtr pTileBox,  
	u8 staticTextureEn         
	)
{
	//#ifndef CHIP_GC9002_NoHyper
#if !defined(CHIP_GC9002_NoHyper) && defined(SXGL_LIB)
	WidgetClassInterface myWidgetClassInterface;
    ActionTriggerClass tagtrigger;
	TagClassPtr bindTag;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 Op_sourceShift = 0;
	funcStatus status;
	#endif

	if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
	{
		//find the binding tag
        bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		tagtrigger.mTagPtr = bindTag;
		//refresh the widget value
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}
	
	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	unsigned char touch_en = 0;
	unsigned char press_en = 0;
	unsigned short x = 0;
	unsigned short y = 0;
	#ifdef SXGL_TOUCH_TEST
	refresh_cnt++;
	if(refresh_cnt == 2)
	{
		touch_en = 1;
		press_en = 1;
		x = 20 * refresh_cnt;
		y = 20 * refresh_cnt;
	}
	else if(refresh_cnt > 2 && refresh_cnt <= 10) //touch simulation
	{
		touch_en = 1;
		press_en = 0;
		x = -20 * refresh_cnt;
		y = 20 * refresh_cnt;
	}
	else if(refresh_cnt > 10 && refresh_cnt <= 15) //untouch simulation
	{
		touch_en = 0;
		press_en = 0;
		x = 20 * 10;
		y = 20 * 10;
	}
	else if(refresh_cnt == 16) //touch again
	{
		touch_en = 1;
		press_en = 1;
		x = 20 * (refresh_cnt - 16);
		y = 20 * (refresh_cnt - 16);
	}
	else if(refresh_cnt > 16 && refresh_cnt < 40 && (refresh_cnt%2) == 0) //touch again
	{
		touch_en = 1;
		press_en = 0;
		x = 20 * (refresh_cnt - 16);
		y = 20 * (refresh_cnt - 16);
	}
	else 
	{
		touch_en = 0;
		press_en = 0;
	}

	#endif
	setTexAttr(0, (STCRGBA888 >> 4), 1024, 1024, 0x14000, 0, 2048, 1024);
	setTexAttr(1, (STCRGBA888 >> 4), 128, 128, 0x8000, 0, 256, 128);
	setTexAttr(2, (PURECOLOR >> 4), 128, 128, 0xffffffff, 0, 128, 128);
	generate3dSourcebuffer((uint32_t *)SoureBufferAddr, u32p_sourceShift,1, touch_en, press_en, x, y);
	gNewSoureBufferSize = 64;
	#ifdef SXGL_ROTATE_TEST
	char debug_code[1000];
	sprintf(debug_code, "rotate angle: %d", cubeRotateAngleShort);
	ERROR_PRINT(debug_code);
	TagPtr[27].setValue(cubeRotateAngleShort, 27);
	#endif
	//draw all texture
	//if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
	//	return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	uint32_t addr = 0;
    #if defined(CHIP_GC9002)
    addr = 0x104000;
    #elif defined(CHIP_GC9005)
    addr = 0x4000;
    #endif
    if(1)
    {
        setTexAttr(0, (STCRGBA888 >> 4), 1024, 1024, 0x14000, 0, 2048, 1024);
		setTexAttr(1, (STCRGBA888 >> 4), 128, 128, 0x8000, 0, 256, 128);
		setTexAttr(2, (PURECOLOR >> 4), 128, 128, 0xffffffff, 0, 128, 128);
    }
    else
    {
        u8 video_next_r_temp = 0;
        extern uint16_t CameraWidth;
        extern uint16_t CameraHeight;
        extern  u8 video_cur_w0;
        extern  u8 video_cur_r0 ;
        extern  u8 video_next_r0;

        #if defined(CHIP_GC9002)
        if(video_cur_w0 == 1)
            video_next_r0 = 0;
        else
            video_next_r0 = 1;
        #elif defined(CHIP_GC9005)
        #if (VIDEO_BUFFER_NUM == 3)
        if(video_cur_w0 == (video_cur_r0 + 2) % 3)
            video_next_r0 = (video_cur_r0 + 1) % 3;
        else
            video_next_r0 = video_cur_r0;
        #elif (VIDEO_BUFFER_NUM == 2)
        if(video_cur_w0 == 1)
            video_next_r0 = 0;
        else
            video_next_r0 = 1;
        #endif
        #else
        #endif
        video_next_r_temp = video_next_r0;

        if((CameraWidth != 0) && (CameraHeight != 0))
        {
            setTexAttr(1, (RGB565 >> 4), CameraWidth, CameraHeight, (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer), 1, CameraWidth, CameraHeight);
        }
        else
        {
            setTexAttr(1, (STCRGBA888 >> 4), 256, 256, 0x234000, 0, 512, 256);
        }

		setTexAttr(0, (STCRGBA888 >> 4), 1024, 1024, 0x14000, 0, 2048, 1024);
		setTexAttr(2, (STCRGBA888 >> 4), 128, 128, 0x8000, 0, 256, 128);
    }
    //generate3dSourcebuffer((unsigned int *)SBAddress, u32p_sourceShift);
    generate3dSourcebuffer((unsigned int *)SBAddress, u32p_sourceShift, 1, 0, 0, 0, 0);
	gNewSoureBufferSize = 64;
	#ifdef SXGL_ROTATE_TEST
	TagPtr[27].setValue(cubeRotateAngleShort, 27);
	#endif
//	Op_sourceShift = *u32p_sourceShift;
//	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE))
//	{
//		//todo
//		status = myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr);
//		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
//		{
//			*u32p_sourceShift = Op_sourceShift;
//			if(myWidgetClassInterface.drawTexture_withCopySourceBuffer(p_wptr, u32p_sourceShift) != AHMI_FUNC_SUCCESS)
//				return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
//		}
//		else if(status != AHMI_FUNC_SUCCESS)
//		{
//			return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
//		}
//
//		p_wptr->REFRESH_FLAG &= (~WidgetSourceBufferRefreshFlag);
//	}
//	else
//	{
//		if(myWidgetClassInterface.drawTexture_withCopySourceBuffer(p_wptr, u32p_sourceShift) != AHMI_FUNC_SUCCESS)
//			return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
//	}
	#endif
	#endif
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Tag.setValue

//**************************************
funcStatus ThreeDimensionalClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	#if !defined(CHIP_GC9002_NoHyper) && defined(SXGL_LIB)
	
	#endif
    
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Tag.setValue

//**************************************
funcStatus ThreeDimensionalClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_widgetRefresh      
	)
{
	#if !defined(CHIP_GC9002_NoHyper) && defined(SXGL_LIB)
	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//Do not support touch propagation.
//**************************************
funcStatus ThreeDimensionalClass::MouseTouchCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
    PIDPoint* pPressPoint,
    PIDPoint* pReleasePoint,
	PIDPoint* pAbsolutePressPoint,
    PIDPoint* pAbsoluteReleasePoint,
	u8 u8_widgetRefresh      
	)
{
	#if !defined(CHIP_GC9002_NoHyper) && defined(SXGL_LIB)
	u8 actionType;
	unsigned char touch_en = 1;
	unsigned char press_en = 0;
	unsigned short x;
	unsigned short y;

	actionType = ActionPtr->mInputType;

	if(actionType == ACTION_MOUSE_PRESS)
	{
	    p_wptr->PressFlag = 1;
		press_en = 1;
	}
	else if(actionType == ACTION_MOUSE_RELEASE)
	{
	    p_wptr->PressFlag = 0;
    }

	if(actionType == ACTION_MOUSE_PRESS)
	{
		x = pPressPoint->x;
		y = pPressPoint->y;
	}
	else 
	{
		x = pReleasePoint->x;
		y = pReleasePoint->y;
	}

	//if(actionType == ACTION_MOUSE_RELEASE || actionType == ACTION_MOUSE_PRESS || actionType == ACTION_MOUSE_HOLDING)
	//{
		//touch_en = 1;
		//if(actionType == ACTION_MOUSE_RELEASE || actionType == ACTION_MOUSE_HOLDING)
		//{
			
		//}
		//else
		//{
		//	x = pPressPoint->x;
		//	y = pPressPoint->y;
		//}
        #if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
		//generate3dSourcebuffer((unsigned int *)SoureBufferAddr, 0,0, touch_en, x, y);
		rotateCntl(touch_en, press_en,x, y);
        #endif
        #if (defined VERSION_2) && (defined EMBEDDED)
		//generate3dSourcebuffer((unsigned int *)SBAddress, 0,0, touch_en, x, y);
		rotateCntl(touch_en, press_en,x, y);
        #endif
	//}

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

#ifdef PC_SIM
	char debug_code[1000];
	sprintf(debug_code, "action_type: %1d, touch_en %1d, press_en %1d, y %d, x %d", actionType, touch_en, press_en, y, x);
	ERROR_PRINT(debug_code);
#endif

	//TagPtr[p_wptr->BindTagID].setValue(debug_value, p_wptr->BindTagID, false, false);

	#endif
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Keyboard.Touch
//**************************************
funcStatus ThreeDimensionalClass::KeyboardTouchCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	#if !defined(CHIP_GC9002_NoHyper) && defined(SXGL_LIB)
	
	#endif
	
	return AHMI_FUNC_SUCCESS;
}


//-----------------------------
// name�� setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
funcStatus ThreeDimensionalClass::setATag
	(
	WidgetClassPtr p_wptr,  
	u16 ATagValue
	)
{
	#if !defined(CHIP_GC9002_NoHyper) && defined(SXGL_LIB)
	
	#endif
	
	return AHMI_FUNC_SUCCESS;
}
#endif
