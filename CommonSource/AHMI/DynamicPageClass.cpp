////////////////////////////////////////////////////////////////////////////////
// Company:		Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     DynamicPageClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    definition used only by widget
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "AHMIBasicStructure.h"
#include "AHMIBasicDefine.h"
#include "publicDefine.h"
#include "myMathClass.h"
#include "drawImmediately_cd.h"
#include "FileLoad.h"
#include "validation.h"
#include "player1c1.h"
#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi.h"
#include "csi_ahmi_bt656.h"
#include "AHMITouch.h"
#endif
#ifdef AHMI_DEBUG
#include "trace.h"
#endif
#ifdef PC_SIM
#include <stdio.h>
#endif
#include "animationDefine.h"

#ifdef AHMI_CORE
#define Press2DraggingLength    (5)

#ifdef PC_SIM
extern FILE *fp;
#endif
extern ConfigInfoClass ConfigData;
extern k_msgq_handle_t   RefreshQueueWithoutDoubleBuffer;
extern AnimationClass  gAnimationClass;
extern Easing		   gEasing;

extern u16             WorkingPageID;
extern DynamicPageClassPtr  gPagePtr;
extern void*           PageSpace;
extern TagClassPtr     TagPtr;

extern k_msgq_handle_t   ActionInstructionQueue;
extern TagClassPtr     TagPtr;
//extern u8 gPageNeedRefresh;

extern s32             windowPressX;
extern s32             windowPressY;
extern s32             windowHoldX;
extern s32             windowHoldY;
extern s32             windowSpeedX;
extern s32             windowSpeedY;
extern u8              windowPressed;

extern u8 force_cancel_subcanvas_page_mousetouch;

extern u16 KeyBoard_PageID;
extern u16 TouchCalibration_PageID;
extern u16 ErrorManagement_PageID;

extern IntervalManager gIntervalManager;

extern uint32_t Display_done_fps;

uint8_t TriggerAfterLoadActionFlag = 1;
//uint8_t LodingNewPage = 0;
uint8_t LastOffsetForPageAnimation = 0;
uint8_t MULTI_PAGE_ANIMATION_SWIPE_EXECUTING = 0;
uint8_t gPageAnimationFlag = 0;
uint8_t gPageReleasingAnimationFlag = 0;
int gPageReleasingSpeed = 0;

#define PageSwipeSpeedMax       		(0xA0)
#define SwipeToFocusTouchThreshold      (0x14)

extern ProjectScreenOptions gScreenOptions;

//u16 gPageReleasingAnimationDuration = 0;
s16 lastOffset_H = 0;
s16 lastOffset_V = 0;
u32 OldTickForWindowHold;
extern uint32_t AHMITick;
uint8_t PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Init;
int gifIntervalIdx = -1;

#if defined(EMBEDDED) && defined(VERSION_2)
extern u8 normalvideoframe;
#endif

extern "C"
{
extern struct globalArgs_t globalArgs;
}

DynamicPageClass::DynamicPageClass(void)
{
}


DynamicPageClass::~DynamicPageClass(void)
{
}

//-----------------------------
// nameï¼? InitPage
// initialize page
//
// @param1 void*   BassAddress            , //Base address for storing dynamic data
// @param2 u32     StoreOffset            , //Data offset
// @param3 u16     PageAttr               , //Page attribute
// @param3 u16     NumOfWidget            , //Number of widget
// @param3 u8 	   NumOfCanvas			  , //Number of Canvas
// @param3 u8      NumOfSubCanvas         , //Number of subCanvas
// @param3 u16     ActionInstructionsSize , //Total instruction length
// @param3 u16     TotalNumOfTexture	  , //Numbe of Texture
// @param3 u16     NumofTouchWidget       , //Numbe of touch widget
// @param3 u16     NumOfPageAction        , //Numbe of page action
// @param3 u16	   NumOfCanvasAction	  ,	//Numbe of CanvasAction
// @param3 u8      animationType          , //Switch animation type
// @param3 u8      curEnlightedWidgetID   , //Current enlighted widget ID
// @param3 u16     animationDuration      , //Animated frame number
// @param3 u8      numOfAnimationActions    //Number of user defined animations
// additional comments
//   none
//-----------------------------
funcStatus DynamicPageClass::InitPage(
		void*          BassAddress            , //Base address for storing dynamic data
		u32            StoreOffset            , //Data offset
		u32			   TotalSize			  , //total size
		u32			   CheckCode			  , //check code
		u16            PageAttr               , //Page attribute
		u16            NumOfWidget            , //Number of widget
		u16 			   NumOfCanvas			  , //Number of Canvas
		u16            NumOfSubCanvas         , //Number of subCanvas
		u16            ActionInstructionsSize , //Total instruction length
		u16            TotalNumOfTexture	  , //Numbe of Texture
		u16            NumofTouchWidget       , //Numbe of touch widget
		u16            NumofGifWidget         , //Numbe of gif widget
		u16            NumOfPageAction        , //Numbe of page action
		u16            NumOfCanvasAction	  ,	//Numbe of CanvasAction
		u8             animationType          , //Switch animation type
		u8             curEnlightedWidgetID   , //Current enlighted widget ID
		u16            duration               , //Animated frame number
		u8			   timingFun			  , //easing
		u16            numOfAnimationKeyFrames, //Number of animation key frames
		u8             numOfAnimationActions  , //Number of user defined animations
		u8			   mVideoType,
		PageScreenOptions _screenAreas
	)
{
			
		//u8 i;
		this->mStoreOffset            = StoreOffset           ;
		this->mPageAttr               = PageAttr             ;
		this->mNumOfCanvas			  = NumOfCanvas			;
		this->pCanvasList             = (CanvasClass*)BassAddress     ;
		this->mNumOfSubCanvas         = NumOfSubCanvas        ;
		this->pSubCanvasList          = (SubCanvasClassPtr)(this->pCanvasList + this->mNumOfCanvas);
		this->mNumOfWidget            = NumOfWidget           ;
		this->pWidgetList			  = (WidgetClass *)(this->pSubCanvasList + this->mNumOfSubCanvas);
		this->mActionInstructionsSize =  ActionInstructionsSize;
		this->pAnimationKeyFrameList = (AnimationKeyFramePtr)(this->pWidgetList + this->mNumOfWidget);
		this->mNumOfAnimationKeyFrames = numOfAnimationKeyFrames;
		this->pAnimationActionList    = (AnimationActionClassPtr)(this->pAnimationKeyFrameList + this->mNumOfAnimationKeyFrames);
		this->mNumOfAnimationActions  = numOfAnimationActions;
		this->pActionInstructions     = (u8*)(this->pAnimationActionList + this->mNumOfAnimationActions);
		this->mTotalNumOfTexture      = TotalNumOfTexture  ;
		this->pBasicTextureList       = (TextureClass*)(this->pActionInstructions + this->mActionInstructionsSize);
		this->mNumofTouchWidget       = NumofTouchWidget;
		this->pTouchWidgetList        = (touchWidgetRange*)(this->pBasicTextureList + this->mTotalNumOfTexture);
		this->mNumofGifWidget         = NumofGifWidget;
		this->pGifWidgetList        = (u16*)(this->pTouchWidgetList + this->mNumofTouchWidget);
		this->mNumOfPageAction        = NumOfPageAction;
		this->pActionStartADDR        = (u16*)(this->pGifWidgetList + this->mNumofGifWidget);
		this->mNumOfCanvasAction		= NumOfCanvasAction;
		this->pCanvasActionStartADDR	= (u16*)(this->pActionStartADDR+this->mNumOfPageAction);
		this->pBackgroundTexture      = (this->pBasicTextureList);	
		this->pMouseTexture           = this->pBasicTextureList + 1;
		this->pMouseWidget            = (this->pWidgetList) +1 ;
		this->mTotalSize              = TotalSize;
		this->mAnimationType          = animationType;
		this->curEnlightedWidgetID    = curEnlightedWidgetID;
		this->pageAnimationDuration            = duration;
		this->pageReleasingAnimationDuration   = duration;
		this->timingFun				  = timingFun;
		this->curEnlightedWidgetLighted = 0;
		this->mATag = 0;
		this->mPageMatrix.NewMatrix_init();
		this->mPageMatrix.NewMatrixInv_init();
		this->mSubPageMatrix.NewMatrix_init();
		this->mSubPageMatrix.NewMatrixInv_init();

		this->VideoType = mVideoType;
		this->mCheckCode = CheckCode;
		this->screenAreas = _screenAreas;

		this->screenType = RefreshScreenSingle;
		
		return AHMI_FUNC_SUCCESS;
}

funcStatus DynamicPageClass::loadLastOnePageData(u16 page, u8 systemInitial)
{
//	u32 baseaddr;
//	u32 totalsize;
#ifdef PC_SIM
	size_t count;
#endif
//	RefreshMsg refreshMsg;
//	funcStatus AHMI_status;
//	ElementPtr  pNewElement;
	DynamicPageClassPtr pCurrentPage;
//	int i;
//	u32 addr;
//	u32 checkCode;
	//WidgetClassPtr canvasWidgetList=gPagePtr[WorkingPageID].pWidgetList;
	//touchWidgetRange* canvasTouchWidgetList=gPagePtr[WorkingPageID].pTouchWidgetList;
//	u8* canvasActionStartAddr=gPagePtr[WorkingPageID].pActionStartADDR+gPagePtr[WorkingPageID].mNumOfPageAction;
    
    //LodingNewPage = 1;

#if 0	
	baseaddr = gPagePtr[page].mStoreOffset;
	totalsize = gPagePtr[page].mTotalSize;

	do{
#ifdef PC_SIM
		fseek(fp,baseaddr,SEEK_SET);

		if (totalsize >ConfigData.DynamicPageSize)  
		{
#ifdef AHMI_DEBUG
			ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
			return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
		}

		count = fread_s((void*)((u8*)PageSpace + ConfigData.DynamicPageSize),totalsize,1,totalsize,fp);
		if(count!=totalsize)
		{
#ifdef AHMI_DEBUG
			ERROR_PRINT("There is no enough data to load page.\r\n");
#endif
			return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
		}
#endif
#ifdef EMBEDDED
		csi_ahmi_SeekReadConfigDataAddr(baseaddr);
	
		if (totalsize >ConfigData.DynamicPageSize) 
		{
#ifdef AHMI_DEBUG
			ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
			return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
		}
		csi_ahmi_ReadConfigData((u32*)((u8*)PageSpace + ConfigData.DynamicPageSize),totalsize);
#endif

		checkCode = XORCheck((u32*)((u8*)PageSpace + ConfigData.DynamicPageSize), totalsize / 4);

	}while(checkCode != gPagePtr[page].mCheckCode);

	gPagePtr[page].InitPage(
			 (void*)((u8*)PageSpace + ConfigData.DynamicPageSize),
			 gPagePtr[page].mStoreOffset,
			 gPagePtr[page].mTotalSize,
			 gPagePtr[page].mCheckCode,
			 gPagePtr[page].mPageAttr,
			 gPagePtr[page].mNumOfWidget,
			 gPagePtr[page].mNumOfCanvas,
			 gPagePtr[page].mNumOfSubCanvas,
			 gPagePtr[page].mActionInstructionsSize,
			 gPagePtr[page].mTotalNumOfTexture,
			 gPagePtr[page].mNumofTouchWidget,
			 gPagePtr[page].mNumofGifWidget,
			 gPagePtr[page].mNumOfPageAction,
			 gPagePtr[page].mNumOfCanvasAction,
			 gPagePtr[page].mAnimationType,
			 gPagePtr[page].curEnlightedWidgetID,
			 gPagePtr[page].pageAnimationDuration,
			 gPagePtr[page].timingFun,
			 gPagePtr[page].mNumOfAnimationKeyFrames,
			 gPagePtr[page].mNumOfAnimationActions,
			 gPagePtr[page].VideoType
		    );
#endif

	//Initializing canvas and widget matrices
	pCurrentPage = &gPagePtr[page];

//	for(i = 0; i < pCurrentPage->mNumOfCanvas; i++)
//	{
//		pCurrentPage->pCanvasList[i].initCanvas();
//		pCurrentPage->pCanvasList[i].CalculateMatrix(&(pCurrentPage->pCanvasList[i].mCanvasMatrix));
//	}
//	for(i = 0; i < pCurrentPage->mNumOfSubCanvas; i++)
//	{
//		pCurrentPage->pSubCanvasList[i].initSubCanvas();
//	}
//	for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
//	{
//		pCurrentPage->pWidgetList[i].initWidget();
//		pCurrentPage->pWidgetList[i].CalculateMatrix(&(pCurrentPage->pWidgetList[i].mWidgetMatrix));
//	}
//    pCurrentPage->UpdateCanvas();
    
	//for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
	//{
	//	pCurrentPage->pWidgetList[i].NOW_DURATION = 0;
	//}

	initHighlightWidget(systemInitial);
	pCurrentPage->shouldShow = 1;
	
    //LodingNewPage = 0;
    
	return AHMI_FUNC_SUCCESS;
}

funcStatus DynamicPageClass::loadLastTwoPageData(u16 page, u8 systemInitial)
{
//	u32 baseaddr;
//	u32 totalsize;
#ifdef PC_SIM
	size_t count;
#endif
//	RefreshMsg refreshMsg;
//	funcStatus AHMI_status;
//	ElementPtr  pNewElement;
//	DynamicPageClassPtr pCurrentPage;
//	int i;
	int j;
//	u32 addr;
//	u32 checkCode;
	//WidgetClassPtr canvasWidgetList=gPagePtr[WorkingPageID].pWidgetList;
	//touchWidgetRange* canvasTouchWidgetList=gPagePtr[WorkingPageID].pTouchWidgetList;
//	u8* canvasActionStartAddr=gPagePtr[WorkingPageID].pActionStartADDR+gPagePtr[WorkingPageID].mNumOfPageAction;
    
    //LodingNewPage = 1;
    
	for(j = 0; j < 2; j++)
	{
#if 0
		baseaddr = gPagePtr[page + j].mStoreOffset;
		totalsize = gPagePtr[page + j].mTotalSize;

		do{
#ifdef PC_SIM
			fseek(fp,baseaddr,SEEK_SET);

			if (totalsize >ConfigData.DynamicPageSize)  
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}

			count = fread_s((void*)((u8*)PageSpace + ConfigData.DynamicPageSize * (1-j)),totalsize,1,totalsize,fp);
			if(count!=totalsize)
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("There is no enough data to load page.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
#endif
#ifdef EMBEDDED
			csi_ahmi_SeekReadConfigDataAddr(baseaddr);
		
			if (totalsize >ConfigData.DynamicPageSize) 
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
			csi_ahmi_ReadConfigData((u32*)((u8*)PageSpace + ConfigData.DynamicPageSize * (1-j)),totalsize);
#endif

			checkCode = XORCheck((u32*)((u8*)PageSpace + ConfigData.DynamicPageSize * (1-j)), totalsize / 4);

		}while(checkCode != gPagePtr[page + j].mCheckCode);

		gPagePtr[page + j].InitPage(
				 (void*)((u8*)PageSpace + ConfigData.DynamicPageSize * (1-j)),
				 gPagePtr[page + j].mStoreOffset,
				 gPagePtr[page + j].mTotalSize,
				 gPagePtr[page + j].mCheckCode,
				 gPagePtr[page + j].mPageAttr,
				 gPagePtr[page + j].mNumOfWidget,
				 gPagePtr[page + j].mNumOfCanvas,
				 gPagePtr[page + j].mNumOfSubCanvas,
				 gPagePtr[page + j].mActionInstructionsSize,
				 gPagePtr[page + j].mTotalNumOfTexture,
				 gPagePtr[page + j].mNumofTouchWidget,
				 gPagePtr[page + j].mNumofGifWidget,
				 gPagePtr[page + j].mNumOfPageAction,
				 gPagePtr[page + j].mNumOfCanvasAction,
				 gPagePtr[page + j].mAnimationType,
				 gPagePtr[page + j].curEnlightedWidgetID,
				 gPagePtr[page + j].pageAnimationDuration,
				 gPagePtr[page + j].timingFun,
				 gPagePtr[page + j].mNumOfAnimationKeyFrames,
				 gPagePtr[page + j].mNumOfAnimationActions,
				 gPagePtr[page + j].VideoType
			    );
#endif

		//Initializing canvas and widget matrices
		//pCurrentPage = &gPagePtr[page + j];

//		for(i = 0; i < pCurrentPage->mNumOfCanvas; i++)
//		{
//			pCurrentPage->pCanvasList[i].initCanvas();
//			pCurrentPage->pCanvasList[i].CalculateMatrix(&(pCurrentPage->pCanvasList[i].mCanvasMatrix));
//		}
//		for(i = 0; i < pCurrentPage->mNumOfSubCanvas; i++)
//		{
//			pCurrentPage->pSubCanvasList[i].initSubCanvas();
//		}
//		for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
//		{
//			pCurrentPage->pWidgetList[i].initWidget();
//			pCurrentPage->pWidgetList[i].CalculateMatrix(&(pCurrentPage->pWidgetList[i].mWidgetMatrix));
//		}
//		pCurrentPage->UpdateCanvas();
		
		//for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
		//{
		//	pCurrentPage->pWidgetList[i].NOW_DURATION = 0;
		//}

		initHighlightWidget(systemInitial);
	}
    
    //LodingNewPage = 0;
    
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? loadPage
// Initializing member variables
//  
//   @param1 u8 page : page 
//   @param2 u8 systemInitial : Represents the most initial page of the system for highlighting controls.
// addtional comments :
//   none
//-----------------------------
extern PLAR_tstWorkBuffer PLAR_stWorkBuf;
u32 DynamicPageClass::loadPage(u16 page, u8 systemInitial)
{
	#ifdef CHIP_GC9002_NoHyper
	u32 baseaddr;
	u32 totalsize;
	#endif
#ifdef PC_SIM
	size_t count;
#endif
//	RefreshMsg refreshMsg;
	funcStatus AHMI_status;
	ElementPtr  pNewElement;
	DynamicPageClassPtr pCurrentPage;
	DynamicPageClassPtr pLastPage;
	TagClassPtr pPageAnimationTag;
	ActionProcessorClass gActionForBeforeLoad;
	int i,PageHiddenTotalNum = 0;
	u32 addr;
	#ifdef CHIP_GC9002_NoHyper
	u32 checkCode;
	#endif
	//WidgetClassPtr canvasWidgetList=gPagePtr[WorkingPageID].pWidgetList;
	//touchWidgetRange* canvasTouchWidgetList=gPagePtr[WorkingPageID].pTouchWidgetList;
//	u8* canvasActionStartAddr=gPagePtr[WorkingPageID].pActionStartADDR+gPagePtr[WorkingPageID].mNumOfPageAction;
	s32 lastPageOffsetX;
	s32 lastPageOffsetY;
	s32 pageOffsetX;
	s32 pageOffsetY;
	//u16 currentPageID;
	s16 lastPageID;
	s16 lastShowPageID = 0;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page
	
	pCurrentPage = &gPagePtr[WorkingPageID];
	if((pCurrentPage->mAnimationType == PAGE_ANIMATION_SWIPE_H) || (pCurrentPage->mAnimationType == PAGE_ANIMATION_SWIPE_V) || (pCurrentPage->mAnimationType == PAGE_ANIMATION_MULTI_CARD))
	{
		if(pPageHiddenTag->getArrayS32Value(page))
		{
			return AHMI_FUNC_SUCCESS;
		}
	}

	if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Idle)
	{
		int DisplayPageNum = numofNormalPage;

		for(int tmp = 0; tmp < numofNormalPage; tmp++)
		{
			if(pPageHiddenTag->getArrayS32Value(tmp))
			{
				DisplayPageNum--;
			}
		}

		if(DisplayPageNum <= 1)
			return AHMI_FUNC_SUCCESS;
		
		if((s16(page) >= 0) && (s16(page) < numofNormalPage))
		{
			if(page >= WorkingPageID)
			{
				for(i = WorkingPageID; i < page; i++)
				{
					if(pPageHiddenTag->getArrayS32Value(i))
					{
						PageHiddenTotalNum++;
					}
				}
			}
			else
			{
				for(i = page; i < WorkingPageID; i++)
				{
					if(pPageHiddenTag->getArrayS32Value(i))
					{
						PageHiddenTotalNum--;
					}
				}
			}
		}
		
		pCurrentPage = &gPagePtr[WorkingPageID];
		lastPageOffsetX = pCurrentPage->mPageMatrix.G / 8192;
		lastPageOffsetY = pCurrentPage->mPageMatrix.H / 8192;
		if((s16(page) >= 0) && (s16(page) < numofNormalPage))
		{
			pageOffsetX = lastPageOffsetX + SCREEN_WIDTH/2 * (page - WorkingPageID - PageHiddenTotalNum);
		}
		else
		{
			if((ConfigData.PageSwipeMode & PageSwipeMode_SwitchMultiPage) == PageSwipeMode_Normal)
			{
				if(s16(page) < 0)
				{
					pageOffsetX = lastPageOffsetX + SCREEN_WIDTH/2 * (-1 - PageHiddenTotalNum);
				}
				else if(s16(page) >=  numofNormalPage)
				{
					pageOffsetX = lastPageOffsetX + SCREEN_WIDTH/2 * (1 - PageHiddenTotalNum);
				}
			}
			else
			{
				pageOffsetX = gPagePtr[WorkingPageID].mPageMatrix.G / 8192;
			}
		}
		if(WorkingPageID == page)
		{
			if(pageOffsetX > (s16)SCREEN_WIDTH/2)
				pageOffsetX = SCREEN_WIDTH/2;
			if(pageOffsetX < (s16)(-SCREEN_WIDTH/2))
				pageOffsetX = -SCREEN_WIDTH/2;
		}
		//pCurrentPage->mPageMatrix.TranslateToMatrix(pageOffsetX<<13, 0);
		pCurrentPage->animationStartOffset = pageOffsetX;

		pNewElement.pageptr = gPagePtr + WorkingPageID;
		PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Auto;
		pCurrentPage->pageAnimationDuration = pCurrentPage->pageReleasingAnimationDuration*2;
		gPageReleasingAnimationFlag = 1;
		AHMI_status = sendToAnimationQueue(
			ANIMATION_REFRESH_PAGE,
			pNewElement,
			pNewElement
			);
		if(AHMI_status != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularPage | EM_ErrRetPage_SendPageAnimationErr);
		
		return AHMI_FUNC_SUCCESS;
	}

	TriggerAfterLoadActionFlag = 1;
	//LodingNewPage = 1;
	force_cancel_subcanvas_page_mousetouch = 1;

	if(s16(page) < 0)
	{
		int tmp = (s16(page) + numofNormalPage);
		while(pPageHiddenTag->getArrayS32Value(tmp))
		{
			tmp--;
		}
		pCurrentPage = &gPagePtr[tmp];
	}
	else if(s16(page) >= numofNormalPage)
	{
		int tmp = (s16(page) - numofNormalPage);
		while(pPageHiddenTag->getArrayS32Value(tmp))
		{
			tmp++;
		}
		pCurrentPage = &gPagePtr[tmp];
	}
	else
	{
		pCurrentPage = &gPagePtr[page];
	}

	if((pCurrentPage->mAnimationType != PAGE_ANIMATION_SWIPE_H) && (pCurrentPage->mAnimationType != PAGE_ANIMATION_SWIPE_V) && (pCurrentPage->mAnimationType != PAGE_ANIMATION_MULTI_CARD))
	{
#ifdef CHIP_GC9002_NoHyper
		baseaddr = gPagePtr[page].mStoreOffset;
		totalsize = gPagePtr[page].mTotalSize;
		
		do{
#ifdef PC_SIM
			fseek(fp,baseaddr,SEEK_SET);

			if (totalsize >ConfigData.DynamicPageSize)  
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}

			count = fread_s(PageSpace,totalsize,1,totalsize,fp);
			if(count!=totalsize)
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("There is no enough data to load page.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
#endif
#ifdef EMBEDDED
			csi_ahmi_SeekReadConfigDataAddr(baseaddr);
	
			if (totalsize >ConfigData.DynamicPageSize) 
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
			csi_ahmi_ReadConfigData(PageSpace,totalsize);
#endif

			checkCode = XORCheck((u32*)PageSpace, totalsize / 4);

		}while(checkCode != gPagePtr[page].mCheckCode);
		
		gPagePtr[page].InitPage(
				 (void*)((u8*)PageSpace),
				 gPagePtr[page].mStoreOffset,
				 gPagePtr[page].mTotalSize,
				 gPagePtr[page].mCheckCode,
				 gPagePtr[page].mPageAttr,
				 gPagePtr[page].mNumOfWidget,
				 gPagePtr[page].mNumOfCanvas,
				 gPagePtr[page].mNumOfSubCanvas,
				 gPagePtr[page].mActionInstructionsSize,
				 gPagePtr[page].mTotalNumOfTexture,
				 gPagePtr[page].mNumofTouchWidget,
				 gPagePtr[page].mNumofGifWidget,
				 gPagePtr[page].mNumOfPageAction,
				 gPagePtr[page].mNumOfCanvasAction,
				 gPagePtr[page].mAnimationType,
				 gPagePtr[page].curEnlightedWidgetID,
				 gPagePtr[page].pageAnimationDuration,
				 gPagePtr[page].timingFun,
				 gPagePtr[page].mNumOfAnimationKeyFrames,
				 gPagePtr[page].mNumOfAnimationActions,
				 gPagePtr[page].VideoType,
				 gPagePtr[page].screenAreas
			    );
#endif
		
		WorkingPageID = page;
		PLAR_stWorkBuf.u16CurrentPage = WorkingPageID;
		pNewElement.pageptr = gPagePtr + WorkingPageID;
		
		//Initializing canvas and widget matrices
		pCurrentPage = &gPagePtr[WorkingPageID];
		pCurrentPage->mPageMatrix.NewMatrix_init();
		pCurrentPage->mPageMatrix.NewMatrixInv_init();

		pCurrentPage->mSubPageMatrix.NewMatrix_init();
		pCurrentPage->mSubPageMatrix.NewMatrixInv_init();
		
		#ifdef CHIP_GC9002_NoHyper
		for(i = 0; i < pCurrentPage->mNumOfCanvas; i++)
		{
			pCurrentPage->pCanvasList[i].initCanvas();
			pCurrentPage->pCanvasList[i].CalculateMatrix(&(pCurrentPage->pCanvasList[i].mCanvasMatrix));
		}
		for(i = 0; i < pCurrentPage->mNumOfSubCanvas; i++)
		{
			pCurrentPage->pSubCanvasList[i].initSubCanvas();
		}
		for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
		{
			pCurrentPage->pWidgetList[i].initWidget();
			pCurrentPage->pWidgetList[i].CalculateMatrix(&(pCurrentPage->pWidgetList[i].mWidgetMatrix));
		}
		pCurrentPage->UpdateCanvas();
		#endif
		
		for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
		{
			pCurrentPage->pWidgetList[i].NOW_DURATION = 0;
			//pCurrentPage->pWidgetList[i].WidgetAttr &= 0x7FFF;
		}

		initHighlightWidget(systemInitial);

		pPageAnimationTag = &TagPtr->getTag(SYSTEM_DISABLEPAGEANIMATION_TAG); 
		if((pCurrentPage->mAnimationType == NO_ANIMATION) || pPageAnimationTag->getValue()) //loading page without animation
		{
			pCurrentPage->shouldShow = 1;
			for(i = 0; i < ConfigData.NumofPages; i++)
			{
				if(i != page)
				{
					gPagePtr[i].shouldShow = 0;
				}
			}

			gAnimationClass.animationQueueReset();

			//execute Beforeload actions
			for(i=0;i != gPagePtr[page].mNumOfPageAction; i++)
			{
				if((gPagePtr[page].pActionStartADDR[i] & 0x8000) != 0)
				{
					addr = (u32)((gPagePtr[page].pActionStartADDR[i] & 0x7FFF) + gPagePtr[page].pActionInstructions);
					//xQueueSendToBack(ActionInstructionQueue,&addr,portMAX_DELAY);
					if((u8 *)addr == NULL || ((u8 *)addr - gPagePtr[page].pActionInstructions) >= gPagePtr[page].mActionInstructionsSize )
					{
						ERROR_PRINT("ERROR in ActionProcessorTask: recive address out of baundary");
					}
					else 
					{
						gActionForBeforeLoad.pPC = (u8 *)addr;
						gActionForBeforeLoad.ActionImplement(true);
					}
				}
			}

			//send Load actions
			for(i=0;i != gPagePtr[page].mNumOfPageAction; i++)
			{
				if((gPagePtr[page].pActionStartADDR[i] & 0x8000) == 0)
				{
					addr = (u32)((gPagePtr[page].pActionStartADDR[i] & 0x7FFF) + gPagePtr[page].pActionInstructions);
					csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
				}
			}
//			gPageNeedRefresh = PageNormalRefreshFlag;
		}
		else 
		{
			pCurrentPage->shouldShow = 1;
			for(i = 0; i < ConfigData.NumofPages; i++)
			{
				if(i != page)
				{
					gPagePtr[i].shouldShow = 0;
				}
			}

			AHMI_status= sendToAnimationQueue(
				ANIMATION_REFRESH_PAGE,
				pNewElement,
				pNewElement
				);
			if(AHMI_status != AHMI_FUNC_SUCCESS)
				return (EM_enErrRetModularPage | EM_ErrRetPage_SendPageAnimationErr);

			//send Beforeload actions
			for(i=0;i != gPagePtr[page].mNumOfPageAction; i++)
			{
				if((gPagePtr[page].pActionStartADDR[i] & 0x8000) != 0)
				{
					addr = (u32)((gPagePtr[page].pActionStartADDR[i] & 0x7FFF) + gPagePtr[page].pActionInstructions);
					csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
				}
			}
		}
	}
	else
	{
		//store last page offset
		lastPageOffsetX = gPagePtr[WorkingPageID].mPageMatrix.G / 8192;
		lastPageOffsetY = gPagePtr[WorkingPageID].mPageMatrix.H / 8192;

		if((page == WorkingPageID) || (s16(page) < 0) || (s16(page) >= numofNormalPage))
		{
			//currentPageID = page;
			if(s16(page) < 0)
			{
				int tmp = (s16(page) + numofNormalPage);
				while(pPageHiddenTag->getArrayS32Value(tmp))
				{
					tmp--;
				}
				lastShowPageID = tmp;
			}
			else if(s16(page) >= numofNormalPage)
			{
				int tmp = (s16(page) - numofNormalPage);
				while(pPageHiddenTag->getArrayS32Value(tmp))
				{
					tmp++;
				}
				lastShowPageID = tmp;
			}
			else if(lastPageOffsetX < 0 || lastPageOffsetY < 0)
			{
				lastShowPageID = WorkingPageID + 1;

				if (lastShowPageID < 0)
				{
					lastShowPageID = 0;
				}
				if (lastShowPageID >= numofNormalPage)
				{
					lastShowPageID = numofNormalPage - 1;
				}
			}
			else if(lastPageOffsetX > 0 || lastPageOffsetY > 0)
			{
				lastShowPageID = WorkingPageID - 1;

				if (lastShowPageID < 0)
				{
					lastShowPageID = 0;
				}
				if (lastShowPageID >= numofNormalPage)
				{
					lastShowPageID = numofNormalPage - 1;
				}
			}
		}
		else
		{
			//if((s16(page - WorkingPageID) == -1) || (s16(page - WorkingPageID) == 1))
			//{
				lastShowPageID = WorkingPageID;
			//	currentPageID = page;
			//}
			//else
			//{
			//	if(page > WorkingPageID)
			//	{
			//		lastShowPageID = WorkingPageID;
			//		currentPageID = WorkingPageID + 1;
			//	}
			//	else if(page < WorkingPageID)
			//	{
			//		lastShowPageID = WorkingPageID;
			//		currentPageID = WorkingPageID - 1;
			//	}
			//}
		}

		pLastPage = &gPagePtr[lastShowPageID];

#ifdef CHIP_GC9002_NoHyper		
		//load last page
		baseaddr = pLastPage->mStoreOffset;
		totalsize = pLastPage->mTotalSize;

		do{
#ifdef PC_SIM
			fseek(fp,baseaddr,SEEK_SET);

			if (totalsize >ConfigData.DynamicPageSize)  
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}

			count = fread_s((void*)((u8*)PageSpace + ConfigData.DynamicPageSize),totalsize,1,totalsize,fp);
			if(count!=totalsize)
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("There is no enough data to load page.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
#endif
#ifdef EMBEDDED
			csi_ahmi_SeekReadConfigDataAddr(baseaddr);
	
			if (totalsize >ConfigData.DynamicPageSize) 
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
			csi_ahmi_ReadConfigData((u32*)((u8*)PageSpace + ConfigData.DynamicPageSize),totalsize);
#endif

			checkCode = XORCheck((u32*)((u8*)PageSpace + ConfigData.DynamicPageSize), totalsize / 4);

		}while(checkCode != pLastPage->mCheckCode);

		pLastPage->InitPage(
				 (void*)((u8*)PageSpace + ConfigData.DynamicPageSize),
				 pLastPage->mStoreOffset,
				 pLastPage->mTotalSize,
				 pLastPage->mCheckCode,
				 pLastPage->mPageAttr,
				 pLastPage->mNumOfWidget,
				 pLastPage->mNumOfCanvas,
				 pLastPage->mNumOfSubCanvas,
				 pLastPage->mActionInstructionsSize,
				 pLastPage->mTotalNumOfTexture,
				 pLastPage->mNumofTouchWidget,
				 pLastPage->mNumofGifWidget,
				 pLastPage->mNumOfPageAction,
				 pLastPage->mNumOfCanvasAction,
				 pLastPage->mAnimationType,
				 pLastPage->curEnlightedWidgetID,
				 pLastPage->pageAnimationDuration,
				 pLastPage->timingFun,
				 pLastPage->mNumOfAnimationKeyFrames,
				 pLastPage->mNumOfAnimationActions,
				 pLastPage->VideoType,
				 pLastPage->screenAreas
			    );
#endif
		#ifdef CHIP_GC9002_NoHyper
		for(i = 0; i < pLastPage->mNumOfCanvas; i++)
		{
			pLastPage->pCanvasList[i].initCanvas();
			pLastPage->pCanvasList[i].CalculateMatrix(&(pLastPage->pCanvasList[i].mCanvasMatrix));
		}
		for(i = 0; i < pLastPage->mNumOfSubCanvas; i++)
		{
			pLastPage->pSubCanvasList[i].initSubCanvas();
		}
		for(i = 0; i < pLastPage->mNumOfWidget; i++)
		{
			pLastPage->pWidgetList[i].initWidget();
			pLastPage->pWidgetList[i].CalculateMatrix(&(pLastPage->pWidgetList[i].mWidgetMatrix));
		}
		pLastPage->UpdateCanvas();
		#endif
		
		//initHighlightWidget(systemInitial);

#ifdef CHIP_GC9002_NoHyper
		//load new page
		baseaddr = gPagePtr[page].mStoreOffset;
		totalsize = gPagePtr[page].mTotalSize;

		do{
#ifdef PC_SIM
			fseek(fp,baseaddr,SEEK_SET);

			if (totalsize >ConfigData.DynamicPageSize)  
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}

			count = fread_s((void*)((u8*)PageSpace),totalsize,1,totalsize,fp);
			if(count!=totalsize)
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("There is no enough data to load page.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
#endif
#ifdef EMBEDDED
			csi_ahmi_SeekReadConfigDataAddr(baseaddr);
		
			if (totalsize >ConfigData.DynamicPageSize) 
			{
#ifdef AHMI_DEBUG
				ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
				return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
			}
			csi_ahmi_ReadConfigData((u32*)((u8*)PageSpace),totalsize);
#endif

			checkCode = XORCheck((u32*)((u8*)PageSpace), totalsize / 4);

		}while(checkCode != gPagePtr[page].mCheckCode);
#endif
		
		lastPageID = WorkingPageID;
		if(s16(page) < 0)
		{
			int tmp = (s16(page) + numofNormalPage);
			while(pPageHiddenTag->getArrayS32Value(tmp))
			{
				tmp--;
			}
			WorkingPageID = tmp;
		}
		else if(s16(page) >= numofNormalPage)
		{
			int tmp = (s16(page) - numofNormalPage);
			while(pPageHiddenTag->getArrayS32Value(tmp))
			{
				tmp++;
			}
			WorkingPageID = tmp;
		}
		else
		{
			WorkingPageID = page;
		}
		
		PLAR_stWorkBuf.u16CurrentPage = WorkingPageID;
		pNewElement.pageptr = gPagePtr + WorkingPageID;
		
#ifdef CHIP_GC9002_NoHyper
		gPagePtr[WorkingPageID].InitPage(
				 (void*)((u8*)PageSpace),
				 gPagePtr[WorkingPageID].mStoreOffset,
				 gPagePtr[WorkingPageID].mTotalSize,
				 gPagePtr[WorkingPageID].mCheckCode,
				 gPagePtr[WorkingPageID].mPageAttr,
				 gPagePtr[WorkingPageID].mNumOfWidget,
				 gPagePtr[WorkingPageID].mNumOfCanvas,
				 gPagePtr[WorkingPageID].mNumOfSubCanvas,
				 gPagePtr[WorkingPageID].mActionInstructionsSize,
				 gPagePtr[WorkingPageID].mTotalNumOfTexture,
				 gPagePtr[WorkingPageID].mNumofTouchWidget,
				 gPagePtr[WorkingPageID].mNumofGifWidget,
				 gPagePtr[WorkingPageID].mNumOfPageAction,
				 gPagePtr[WorkingPageID].mNumOfCanvasAction,
				 gPagePtr[WorkingPageID].mAnimationType,
				 gPagePtr[WorkingPageID].curEnlightedWidgetID,
				 gPagePtr[WorkingPageID].pageAnimationDuration,
				 gPagePtr[WorkingPageID].timingFun,
				 gPagePtr[WorkingPageID].mNumOfAnimationKeyFrames,
				 gPagePtr[WorkingPageID].mNumOfAnimationActions,
				 gPagePtr[WorkingPageID].VideoType,
				 gPagePtr[WorkingPageID].screenAreas
			    );
#endif
				 
		//Initializing canvas and widget matrices
		pCurrentPage = &gPagePtr[WorkingPageID];
		
		#ifdef CHIP_GC9002_NoHyper
		for(i = 0; i < pCurrentPage->mNumOfCanvas; i++)
		{
			pCurrentPage->pCanvasList[i].initCanvas();
			pCurrentPage->pCanvasList[i].CalculateMatrix(&(pCurrentPage->pCanvasList[i].mCanvasMatrix));
		}
		for(i = 0; i < pCurrentPage->mNumOfSubCanvas; i++)
		{
			pCurrentPage->pSubCanvasList[i].initSubCanvas();
		}
		for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
		{
			pCurrentPage->pWidgetList[i].initWidget();
			pCurrentPage->pWidgetList[i].CalculateMatrix(&(pCurrentPage->pWidgetList[i].mWidgetMatrix));
		}
		pCurrentPage->UpdateCanvas();
		#endif
		
		for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
		{
			pCurrentPage->pWidgetList[i].NOW_DURATION = 0;
			pCurrentPage->pWidgetList[i].WidgetAttr &= 0x7FFF;
		}
		
		//initHighlightWidget(systemInitial);

		pPageAnimationTag = &TagPtr->getTag(SYSTEM_DISABLEPAGEANIMATION_TAG); 
		if(pPageAnimationTag->getValue() || pPageHiddenTag->getArrayS32Value(WorkingPageID))
		{
			pCurrentPage->shouldShow = 1;
			pCurrentPage->mPageMatrix.TranslateToMatrix(0, 0);
			for(i = 0; i < ConfigData.NumofPages; i++)
			{
				if(i != WorkingPageID)
				{
					gPagePtr[i].shouldShow = 0;
				}
			}

			gAnimationClass.animationQueueReset();
			
			//execute Beforeload actions
			for(i=0;i != gPagePtr[WorkingPageID].mNumOfPageAction; i++)
			{
				if((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x8000) != 0)
				{
					addr = (u32)((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x7FFF) + gPagePtr[WorkingPageID].pActionInstructions);
					//xQueueSendToBack(ActionInstructionQueue,&addr,portMAX_DELAY);
					if((u8 *)addr == NULL || ((u8 *)addr - gPagePtr[WorkingPageID].pActionInstructions) >= gPagePtr[WorkingPageID].mActionInstructionsSize )
					{
						ERROR_PRINT("ERROR in ActionProcessorTask: recive address out of baundary");
					}
					else 
					{
						gActionForBeforeLoad.pPC = (u8 *)addr;
						gActionForBeforeLoad.ActionImplement(true);
					}
				}
			}

			//send Load actions
			for(i=0;i != gPagePtr[WorkingPageID].mNumOfPageAction; i++)
			{
				if((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x8000) == 0)
				{
					addr = (u32)((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x7FFF) + gPagePtr[WorkingPageID].pActionInstructions);
                    csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
				}
			}
//			gPageNeedRefresh = PageNormalRefreshFlag;
		}
		else
		{
			if((s16(page) >= 0) && (s16(page) < numofNormalPage))
			{
				if(WorkingPageID >= lastPageID)
				{
					for(i = lastPageID; i < WorkingPageID; i++)
					{
						if(pPageHiddenTag->getArrayS32Value(i))
						{
							PageHiddenTotalNum++;
						}
					}
				}
				else
				{
					for(i = WorkingPageID; i < lastPageID; i++)
					{
						if(pPageHiddenTag->getArrayS32Value(i))
						{
							PageHiddenTotalNum--;
						}
					}
				}
			}

			//additional offset for swipe_h swipe_v
			if (mAnimationType == PAGE_ANIMATION_SWIPE_H)
			{
				if((s16(page) >= 0) && (s16(page) < numofNormalPage))
				{
					pageOffsetX = lastPageOffsetX + SCREEN_WIDTH * (WorkingPageID - lastPageID - PageHiddenTotalNum);
				}
				else
				{
					if((ConfigData.PageSwipeMode & PageSwipeMode_SwitchMultiPage) == PageSwipeMode_Normal)
					{
						if(s16(page) < 0)
						{
							pageOffsetX = lastPageOffsetX + SCREEN_WIDTH * (-1 - PageHiddenTotalNum);
						}
						else if(s16(page) >=  numofNormalPage)
						{
							pageOffsetX = lastPageOffsetX + SCREEN_WIDTH * (1 - PageHiddenTotalNum);
						}
					}
					else
					{
						pageOffsetX = gPagePtr[WorkingPageID].mPageMatrix.G / 8192;
					}
				}
				if(WorkingPageID == lastPageID)
				{
					if(pageOffsetX > (s16)SCREEN_WIDTH)
						pageOffsetX = SCREEN_WIDTH;
					if(pageOffsetX < (s16)(-SCREEN_WIDTH))
						pageOffsetX = -SCREEN_WIDTH;
				}
				pCurrentPage->mPageMatrix.TranslateToMatrix(pageOffsetX<<13, 0);
				pCurrentPage->animationStartOffset = pageOffsetX;
				
			}else if (mAnimationType == PAGE_ANIMATION_SWIPE_V)
			{
				if((s16(page) >= 0) && (s16(page) < numofNormalPage))
				{
					pageOffsetY = lastPageOffsetY + SCREEN_HEIGHT * (WorkingPageID - lastPageID - PageHiddenTotalNum);
				}
				else
				{
					if((ConfigData.PageSwipeMode & PageSwipeMode_SwitchMultiPage) == PageSwipeMode_Normal)
					{
						if(s16(page) < 0)
						{
							pageOffsetY = lastPageOffsetY + SCREEN_HEIGHT * (-1 - PageHiddenTotalNum);
						}
						else if(s16(page) >=  numofNormalPage)
						{
							pageOffsetY = lastPageOffsetY + SCREEN_HEIGHT * (1 - PageHiddenTotalNum);
						}
					}
					else
					{
						pageOffsetY = gPagePtr[WorkingPageID].mPageMatrix.H / 8192;
					}
				}
				if(WorkingPageID == lastPageID)
				{
					if(pageOffsetY > (s16)SCREEN_HEIGHT)
						pageOffsetY = SCREEN_HEIGHT;
					if(pageOffsetY < (s16)(-SCREEN_HEIGHT))
						pageOffsetY = -SCREEN_HEIGHT;
				}
				pCurrentPage->mPageMatrix.TranslateToMatrix(0, pageOffsetY<<13);
				pCurrentPage->animationStartOffset = pageOffsetY;
				
			}else if (mAnimationType == PAGE_ANIMATION_MULTI_CARD)
			{
				if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Init)
				{
					PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Idle;
					pCurrentPage->animationStartOffset = 0;
				}
				else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Focus)
				{
					pageOffsetX = gPagePtr[WorkingPageID].mPageMatrix.G / 8192;
					pCurrentPage->animationStartOffset = pageOffsetX;
				}
			}else
			{
				pCurrentPage->shouldShow = 1;
				if (lastPageID != WorkingPageID)
				{
					pLastPage->shouldShow = 0;
				}
			}

			if(pCurrentPage->animationStartOffset == 0)
			{
				pCurrentPage->shouldShow = 1;
				for(i = 0; i < ConfigData.NumofPages; i++)
				{
					if(i != WorkingPageID)
					{
						gPagePtr[i].shouldShow = 0;
					}
				}

				gAnimationClass.animationQueueReset();
				
				//execute Beforeload actions
				for(i=0;i != gPagePtr[WorkingPageID].mNumOfPageAction; i++)
				{
					if((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x8000) != 0)
					{
						addr = (u32)((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x7FFF) + gPagePtr[WorkingPageID].pActionInstructions);
						//xQueueSendToBack(ActionInstructionQueue,&addr,portMAX_DELAY);
						if((u8 *)addr == NULL || ((u8 *)addr - gPagePtr[WorkingPageID].pActionInstructions) >= gPagePtr[WorkingPageID].mActionInstructionsSize )
						{
							ERROR_PRINT("ERROR in ActionProcessorTask: recive address out of baundary");
						}
						else 
						{
							gActionForBeforeLoad.pPC = (u8 *)addr;
							gActionForBeforeLoad.ActionImplement(true);
						}
					}
				}

				//send Load actions
				for(i=0;i != gPagePtr[WorkingPageID].mNumOfPageAction; i++)
				{
					if((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x8000) == 0)
					{
						addr = (u32)((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x7FFF) + gPagePtr[WorkingPageID].pActionInstructions);
						csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
					}
				}
//				gPageNeedRefresh = PageNormalRefreshFlag;
			}
			else
			{
				if(!MULTI_PAGE_ANIMATION_SWIPE_EXECUTING)
				{
					//send Beforeload actions
					for(i=0;i != gPagePtr[WorkingPageID].mNumOfPageAction; i++)
					{
						if((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x8000) != 0)
						{
							addr = (u32)((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x7FFF) + gPagePtr[WorkingPageID].pActionInstructions);
							csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
						}
					}

					if (lastPageID == WorkingPageID)
					{
						TriggerAfterLoadActionFlag = 0;
					}

					windowPressed = 0;
					this->pageAnimationDuration = this->pageReleasingAnimationDuration;
					AHMI_status = sendToAnimationQueue(
						ANIMATION_REFRESH_PAGE,
						pNewElement,
						pNewElement
						);
					if(AHMI_status != AHMI_FUNC_SUCCESS)
						return (EM_enErrRetModularPage | EM_ErrRetPage_SendPageAnimationErr);
				}
			}
		}
	}

#if defined(EMBEDDED) && defined(VERSION_2)
	#ifdef VIDEO_EN
    if(gPagePtr[WorkingPageID].VideoType != 0)
    {
		normalvideoframe = 0;
        csi_ahmi_bt656_enable();
    }
    else
    {
        csi_ahmi_bt656_disable();
    }
	#endif
#endif	
    
	
#ifdef EMBEDDED    
    if(WorkingPageID == TouchCalibration_PageID)
    {
        ResetCalibrationflag();
    }
#endif

	//gif interval
	//init gif
	if (gifIntervalIdx == -1 && gPagePtr[WorkingPageID].mNumofGifWidget > 0)
	{
		gifIntervalIdx = gIntervalManager.getIDLEInterval();//get an idle position
		//
		if (gifIntervalIdx == -1)
		{
			//no idle interval available
			ERROR_PRINT("no idle status");
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_NoIdleForDelay);
		}
		else
		{
			gIntervalManager.setInterval(gifIntervalIdx,tickGifOP,NULL,AnimationIntervalTime);
		}
	}
	
    
    //LodingNewPage = 0;
    
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? loadPageWithAnimation
// load page with animation
//  
//     @param1 DynamicPageClassPtr pCurPage  , //current page
//     @param2 DynamicPageClassPtr pNextPage , //next page
//     @param3 u8 curFrame                   , //current frame
//     @param4 u8 totalFrame                 , //total frame
//     @param5 u8 animationType                //animation type
// addtional comments :
//   none
//-----------------------------
u32 DynamicPageClass::loadPageWithAnimation(
		u8 curPageID,   //current page
		u8 nextPageID,  //next page
        u8 curFrame,    //current frame
		u8 totalFrame,  //total frame
		u8 animationType//animation type
		)
{
#ifdef PC_SIM
	ElementPtr pOldElement, pNewElement;
	DynamicPageClassPtr pCurrentPage;
	funcStatus AHMI_status;
//	u8 i;
	u32 baseaddr;
	u16 totalsize;
	size_t count;
//	u32 addr;
	WidgetClassPtr canvasWidgetList=gPagePtr[WorkingPageID].pWidgetList;
	touchWidgetRange* canvasTouchWidgetList=gPagePtr[WorkingPageID].pTouchWidgetList;
	u16* canvasActionStartAddr=gPagePtr[WorkingPageID].pActionStartADDR+gPagePtr[WorkingPageID].mNumOfPageAction;
	u16 i;

	pOldElement.pageptr = gPagePtr+curPageID;
	pNewElement.pageptr = gPagePtr+nextPageID;

	if(totalFrame == 0)
		return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);

	baseaddr = (gPagePtr+nextPageID)->mStoreOffset;
	totalsize = (gPagePtr+nextPageID)->mTotalSize;
#ifdef PC_SIM
	fseek(fp,baseaddr,SEEK_SET);
#endif

	if (totalsize >ConfigData.DynamicPageSize) 
	{
#ifdef AHMI_DEBUG
		ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
		return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
	}

#ifdef PC_SIM
	count = fread_s(PageSpace,totalsize,1,totalsize,fp);
	if(count!=totalsize)
	{
#ifdef AHMI_DEBUG
		ERROR_PRINT("There is no enough data to load page.\r\n");
#endif
		return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
	}	
#endif

#ifdef EMBEDDED
	csi_ahmi_SeekReadConfigDataAddr(baseaddr);

	if (totalsize >ConfigData.DynamicPageSize) 
	{
#ifdef AHMI_DEBUG
		ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
		return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
	}
	csi_ahmi_ReadConfigData(PageSpace,totalsize);
#endif

	WorkingPageID = nextPageID;
	//Initializing canvas and widget matrices
	pCurrentPage = &gPagePtr[WorkingPageID];
	for(i = 0; i < this->mNumOfCanvas; i++)
	{
		pCurrentPage->pCanvasList[i].initCanvas();
	}
	for(i = 0; i < this->mNumOfSubCanvas; i++)
	{
		pCurrentPage->pSubCanvasList[i].initSubCanvas();
	}
	for(i = 0; i < this->mNumOfWidget; i++)
	{
		pCurrentPage->pWidgetList[i].initWidget();
	}
    pCurrentPage->UpdateCanvas();
    
	initHighlightWidget(1);

//	gPageNeedRefresh = PageNormalRefreshFlag;
	AHMI_status = sendToAnimationQueue(
		ANIMATION_REFRESH_PAGE,
		pNewElement,
		pOldElement
		);
	if(AHMI_status != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularPage | EM_ErrRetPage_SendPageAnimationErr);
	//send the action after animation
	//for(i=0;i != gPagePtr[nextPageID].mNumOfPageAction; i++)
	//{
	//	addr = (u32)(gPagePtr[nextPageID].pActionStartADDR[i] + gPagePtr[nextPageID].pActionInstructions);
	//	xQueueSendToBack(ActionInstructionQueue,&addr,portMAX_DELAY);
	//}
	return AHMI_FUNC_SUCCESS;
#endif
	
#ifdef EMBEDDED
	ElementPtr pOldElement, pNewElement;
	DynamicPageClassPtr pCurrentPage;
	funcStatus AHMI_status;
	u8 i;
	u32 baseaddr;
	u16 totalsize;
	u32 addr;
//	WidgetClassPtr canvasWidgetList=gPagePtr[WorkingPageID].pWidgetList;
//	touchWidgetRange* canvasTouchWidgetList=gPagePtr[WorkingPageID].pTouchWidgetList;
//	u8* canvasActionStartAddr=gPagePtr[WorkingPageID].pActionStartADDR+gPagePtr[WorkingPageID].mNumOfPageAction;

	pOldElement.pageptr = gPagePtr+curPageID;
	pNewElement.pageptr = gPagePtr+nextPageID;

	if(totalFrame == 0)
		return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);

	baseaddr = (gPagePtr+nextPageID)->mStoreOffset;
	totalsize = (gPagePtr+nextPageID)->mTotalSize;
	
	csi_ahmi_SeekReadConfigDataAddr(baseaddr);

	if (totalsize >ConfigData.DynamicPageSize) 
	{
#ifdef AHMI_DEBUG
		ERROR_PRINT("DynamicPageSize too small.\r\n");
#endif
		return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
	}

	csi_ahmi_ReadConfigData(PageSpace,totalsize);

	WorkingPageID = nextPageID;

	//initital canvas and widget
	pCurrentPage = &gPagePtr[WorkingPageID];
	for(i = 0; i < pCurrentPage->mNumOfCanvas; i++)
	{
		pCurrentPage->pCanvasList[i].initCanvas();
	}
	for(i = 0; i < pCurrentPage->mNumOfSubCanvas; i++)
	{
		pCurrentPage->pSubCanvasList[i].initSubCanvas();
	}
	for(i = 0; i < pCurrentPage->mNumOfWidget; i++)
	{
		pCurrentPage->pWidgetList[i].initWidget();
	}
	initHighlightWidget(1);


	AHMI_status = sendToAnimationQueue(
		ANIMATION_REFRESH_PAGE,
		pNewElement,
		pOldElement
		);
	if(AHMI_status != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularPage | EM_ErrRetPage_SendPageAnimationErr);
	
	for(i=0;i != gPagePtr[nextPageID].mNumOfPageAction; i++)
	{
		addr = (u32)(gPagePtr[nextPageID].pActionStartADDR[i] + gPagePtr[nextPageID].pActionInstructions);
		csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
	}
	
	return AHMI_FUNC_SUCCESS;
#endif

}

//-----------------------------
// name: drawPage
// Update focused subCanvas.
// addtional comments :
//   none
//-----------------------------
u32 DynamicPageClass::UpdateCanvas(void)
{
    CanvasClassPtr cptr;
    TagClassPtr bindTag;
    int numOfCanvas = 0;
    int CanvasCount = 0;
    
    if(this->pCanvasList == NULL)
	{
		ERROR_PRINT("ERROR in drawPage: canvas list data corruption");
		return (EM_enErrRetModularPage | EM_ErrRetPage_NULLPointerErr);
	}
    
    numOfCanvas = this->mNumOfCanvas;
    
    for(CanvasCount = 0; CanvasCount < numOfCanvas; CanvasCount ++)
	{
        cptr = (CanvasClassPtr)(this->pCanvasList) + CanvasCount;
        
        if(cptr->bindTagID < ConfigData.NumofTags)
			bindTag = &TagPtr->getTag(cptr->bindTagID);
		else 
		{
			ERROR_PRINT("ERROR in drawPage: canvas tag num out of baundary");
			return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
		}
        
        if(cptr->bindTagID == 0) 
		  //Judge if it is number 0 tag, if it is number 0 tag, make tagtmp=-0
			cptr->mFocusedCanvas = 0;
		else
			cptr->mFocusedCanvas = bindTag->getValue();
        
        //If the value of tag exceeds the number of sub canvas, if it exceeds, it is equal to the number of sub canvas.
		if(cptr->mFocusedCanvas >= cptr->mNumOfSubCanvas) 
			cptr->mFocusedCanvas = cptr->mNumOfSubCanvas - 1;
		if(this->pSubCanvasList == NULL)
		{
			ERROR_PRINT("ERROR in drawPage: subcanvas list data corruption");
			return (EM_enErrRetModularPage | EM_ErrRetPage_NULLPointerErr);
		}
        
    }
	return AHMI_FUNC_SUCCESS;
}

#ifdef CHIP_GC9002	
extern funcStatus  setCanvasSourebufferRefreshFlag(DynamicPageClass* pageptr, CanvasClassPtr cptr);
funcStatus  getCanvasSoureBoxOverlap(CanvasClassPtr cptr, TileBoxClassPtr tileBox)
{
	TileBoxClass SourceBox;
	
	if((cptr == NULL) || (tileBox == NULL))
		return AHMI_FUNC_FAILURE;
	
	
	SourceBox.LeftBox = cptr->moffsetX / TILESIZE;
	SourceBox.TopBox = cptr->moffsetY / TILESIZE;
	SourceBox.RightBox = (cptr->moffsetX + cptr->mwidth + TILESIZE - 1) / TILESIZE - 1;
	SourceBox.ButtomBox = (cptr->moffsetY + cptr->mheight - 1) / TILESIZE - 1;
	
	return tileBox->sourceBoxOverlap(&SourceBox);
}
#endif

//-----------------------------
// nameï¼? drawPage
// draw page
//  
//     @param1 TileBoxClassPtr tileBox, //source box
//     @param2 u32 *sourceShift       , //sourcebuffer pointer
//     @param3 u8  pageEnable         , //whether refresh total page
//     @param4 u8 RefreshType         , //Animation is being drawn, and the bounding box of the control is cancelled.
//     @param5 u8 staticTextureEn       //Whether to draw to static storage space
// addtional comments :
//   none
//-----------------------------
u32 DynamicPageClass::drawPage(
	TileBoxClassPtr tileBox, //source box
	u32 *sourceShift       , //sourcebuffer pointer
	u8  pageEnable         , //whether refresh total page
	u8 RefreshType         , //Animation is being drawn, and the bounding box of the control is cancelled.
    u8 staticTextureEn       //Whether to draw to static storage space
	)
{
//	funcStatus status;
	CanvasClassPtr cptr;
	SubCanvasClassPtr scptr;
		
	TagClassPtr bindTag;
//	u32 TagValueTmp = 0;
	if(tileBox == NULL || sourceShift == NULL)
	{
		ERROR_PRINT("ERROR in drawPage: input tilebox or sourceshift donnot exist");
		return (EM_enErrRetModularPage | EM_ErrRetPage_NULLPointerErr);
	}

	int numOfCanvas = this->mNumOfCanvas;
	for(int CanvasCount = 0; CanvasCount < numOfCanvas; CanvasCount ++)
	{
		if(this->pCanvasList == NULL)
		{
			ERROR_PRINT("ERROR in drawPage: canvas list data corruption");
			return (EM_enErrRetModularPage | EM_ErrRetPage_NULLPointerErr);
		}
		cptr = (CanvasClassPtr)(this->pCanvasList) + CanvasCount;
		if(cptr->bindTagID < ConfigData.NumofTags)
			bindTag = &TagPtr->getTag(cptr->bindTagID);
		else 
		{
			ERROR_PRINT("ERROR in drawPage: canvas tag num out of baundary");
			return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
		}

		if(cptr->animationType < SUBCANVAS_ANIMATION_SWIPE_H)
		{
			if(cptr->bindTagID == 0)
			{
				//Judge if it is number 0 tag, if it is number 0 tag, make tagtmp=-0
				cptr->mFocusedCanvas = 0;
			}
			else
			{
				//If the value of tag exceeds the number of sub canvas, if it exceeds, it is equal to the number of sub canvas.
				if(bindTag->getValue() >= cptr->mNumOfSubCanvas) 
					cptr->mFocusedCanvas = cptr->mNumOfSubCanvas - 1;
				else
					cptr->mFocusedCanvas = bindTag->getValue();
			}
			
		}
		if(this->pSubCanvasList == NULL)
		{
			ERROR_PRINT("ERROR in drawPage: subcanvas list data corruption");
			return (EM_enErrRetModularPage | EM_ErrRetPage_NULLPointerErr);
		}
#ifdef CHIP_GC9002	
		if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)		
		{
			if(getCanvasSoureBoxOverlap(cptr, tileBox) != AHMI_FUNC_SUCCESS)
			{
				setCanvasSourebufferRefreshFlag(this, cptr);
				continue;
			}
		}
#endif
		
		if (cptr->animationType == SUBCANVAS_ANIMATION_PULL_DOWN || cptr->animationType == SUBCANVAS_ANIMATION_PULL_UP || cptr->animationType == SUBCANVAS_ANIMATION_PULL_LEFT || cptr->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT)
		{
			
			if (cptr->pullState)
			{
				//pulling
				for (int i = 0; i < cptr->mNumOfSubCanvas; i++)
				{
					scptr = (SubCanvasClassPtr)(this->pSubCanvasList) + cptr->mStartAddrOfSubCanvas + i;
					if(pageEnable)
					{
						scptr->initWidgetValueInSubCanvas(this);
					}
					////Call drawCanvas function
					//if (scptr->shouldShow || i == cptr->mFocusedCanvas)
					//{
					//	cptr->drawCanvas(tileBox, sourceShift, pageEnable, this, scptr, RefreshType, staticTextureEn);
					//}
				}
				for (int i = 0; i < cptr->mNumOfSubCanvas; i++)
				{
					scptr = (SubCanvasClassPtr)(this->pSubCanvasList) + cptr->mStartAddrOfSubCanvas + i;
					if(pageEnable)
					{
						scptr->initWidgetValueInSubCanvas(this);
					}
					//Call drawCanvas function
					if (scptr->shouldShow || i == cptr->mFocusedCanvas)
					{
						cptr->drawCanvas(tileBox, sourceShift, pageEnable, this, scptr, RefreshType, staticTextureEn);
					}
				}
			}
			else
			{
				//another should show sc
				for (int i = 0; i < cptr->mNumOfSubCanvas; i++)
				{
					scptr = (SubCanvasClassPtr)(this->pSubCanvasList) + cptr->mStartAddrOfSubCanvas + i;
					
					//Call drawCanvas function
					if (scptr->shouldShow && i != cptr->mFocusedCanvas)
					{
						cptr->drawCanvas(tileBox, sourceShift, pageEnable, this, scptr, RefreshType, staticTextureEn);
					}
				}

				//focused sc
				scptr = (SubCanvasClassPtr)(this->pSubCanvasList) + cptr->mStartAddrOfSubCanvas +  cptr->mFocusedCanvas;
				cptr->drawCanvas(tileBox, sourceShift, pageEnable, this, scptr, RefreshType, staticTextureEn);

				
			}
		}
		else
		{
			for (int i = 0; i < cptr->mNumOfSubCanvas; i++)
			{
				scptr = (SubCanvasClassPtr)(this->pSubCanvasList) + cptr->mStartAddrOfSubCanvas + i;
				if(pageEnable)
				{
					scptr->initWidgetValueInSubCanvas(this);
					if(i == cptr->mFocusedCanvas)
						scptr->shouldShow = 1;
					else
						scptr->shouldShow = 0;
				}
				//Call drawCanvas function
				if (scptr->shouldShow)
				{
					cptr->drawCanvas(tileBox, sourceShift, pageEnable, this, scptr, RefreshType, staticTextureEn);
				}
			}
		}
		
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? drawPageWithAnimation
// draw page with animation
//  
//     @param1 DynamicPageClass* pCurPage    ,//Current page pointer
//     @param2 TileBoxClassPtr tileBox       ,//source box
//     @param3 u32* pSourceShift             ,//sourcebuffer offset
//     @param4 u8  staticTextureEn            //Whether to draw to static storage space
// addtional comments :
//   none
//-----------------------------
funcStatus DynamicPageClass::drawPageWithAnimation(
	DynamicPageClass* pCurPage    ,//Current page pointer
	TileBoxClassPtr tileBox       ,//source box
	u32* pSourceShift             ,//sourcebuffer offset
    u8  staticTextureEn            //Whether to draw to static storage space
	)
{
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? refreshWidgetBox
// refresh widget box
//  
//     @param1 WidgetClassPtr pWidget,
//     @param2 CanvasClassPtr pCanvas,
//     @param3 TextureClassPtr pTexture,
//     @param4 u8 textureEnable,
//     @param5 u8 widgetEnable,
//     @param6 u8 canvasEnable,
//     @param7 u8 doubleBufferEnable,
//     @param8 TileBoxClassPtr pTileBox
// addtional comments :
//   none
//-----------------------------
funcStatus DynamicPageClass::refreshWidgetBox(
    WidgetClassPtr pWidget,
	CanvasClassPtr pCanvas,
	TextureClassPtr pTexture,
	u8 textureEnable,
	u8 widgetEnable,
	u8 canvasEnable,
	u8 doubleBufferEnable,
	TileBoxClassPtr pTileBox
)
{
	return AHMI_FUNC_SUCCESS;
}
	
//-----------------------------
// nameï¼? RefreshOSCWidgetBox
// refresh OSC widget box
//  
//     @param1 WidgetClassPtr pWidget,
//     @param2 TileBoxClassPtr pTileBox
// addtional comments :
//   none
//-----------------------------
funcStatus DynamicPageClass::RefreshOSCWidgetBox(
	WidgetClassPtr pWidget,
	TileBoxClassPtr pTileBox
)
{
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? GetWidgetList
// get widget list
//  
//     @param1 u8 Value
// addtional comments :
//   none
//-----------------------------
WidgetClassPtr DynamicPageClass::GetWidgetList(u8 Value)
{
	// assert(Value<Ptr->NumOfWidget); 
	return &(this->pWidgetList[Value]);
}

//-----------------------------
// nameï¼? GetBackgroundTexture
// get background texture
//  
//     @param1 none
// addtional comments :
//   none
//-----------------------------
TextureClassPtr DynamicPageClass::GetBackgroundTexture()
{
	return this->pBackgroundTexture;
}

//-----------------------------
// nameï¼? initHighlightWidget
// initialize highlight widget
//  
//     @param1 u8 systemInitial
// addtional comments :
//   none
//-----------------------------
funcStatus DynamicPageClass::initHighlightWidget(u8 systemInitial)
{
	WidgetClassPtr curWidgetPtr;
	u8 widgetType;
	//u8 buttonType;
	ButtonClass button;
	ActionTriggerClass actionTrigger;
	if( ( (s8)this->curEnlightedWidgetID ) < 0)
		return AHMI_FUNC_SUCCESS;
	curWidgetPtr = &(this->pWidgetList[this->curEnlightedWidgetID]);
	widgetType = curWidgetPtr->WidgetType;
	//buttonType = (curWidgetPtr->WidgetAttr & 0x60) >> 5;
	if(widgetType == BUTTON_TEX && this->curEnlightedWidgetLighted)
	{
		actionTrigger.mInputType = ACTION_KEYBOARD_NEXT;
		actionTrigger.mTagPtr = NULL;
		button.widgetCtrl(&gPagePtr[WorkingPageID], curWidgetPtr, &actionTrigger, 1);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? setATag
// Animation tag changed
//  
//     @param1 u8 value  Changed value
// addtional comments :
//   none
//-----------------------------
u8 firstload = 0;
u8 PAGE_ANIMATION_EXECUTING = 0;
u32 DynamicPageClass::setATag(u16 value)
{
	s32 curPageOffsetX ; //1.11.4
	s32 curPageOffsetY ; //1.11.4
	u16 transitionParamIn; //0.10
	u16 transitionParamOut; //0.10
	s32 pageScale;
	u32 i;
	u32 addr;
	u8 animationType = this->mAnimationType;
	u16 screenWidth;
	u16 screenHeight;
	u16 subScreenWidth;
	u16 subScreenHeight;
	u16 curPageAnimationDuration;
	s16 lastShowPageID = 0;
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page

	int releasingStep = 2;

	int nextPageID;

	//No need to animate.
	if( pageAnimationDuration == 0 || animationType == NO_ANIMATION)
	{
		//ATag changes will not change.
		return AHMI_FUNC_SUCCESS;
	}
	//Need to animate
	else
	{
		if(gPageReleasingAnimationFlag)
		{
			curPageAnimationDuration = pageAnimationDuration;
		}
		else
		{
			curPageAnimationDuration = pageReleasingAnimationDuration;
		}
		
		if(value > curPageAnimationDuration) //out of baundary
		{
			ERROR_PRINT("ERROR in setting page animaion tag: value out of baundary");
			return (EM_enErrRetModularPage | EM_ErrRetPage_DataOverrun);
		}
		this->mATag = value;

		if (gScreenOptions.enable)
		{
			screenWidth = gScreenOptions.mainWidth;
			screenHeight = gScreenOptions.mainHeight;
			subScreenWidth = gScreenOptions.subWidth;
			subScreenHeight = gScreenOptions.subHeight;
		}
		else
		{
			screenWidth = SCREEN_WIDTH;
			screenHeight = SCREEN_HEIGHT;
		}
		
		switch(animationType)
		{
			case(PAGE_ANIMATION_SHIFTING_L):
				//Horizontal translation animation
				gPageAnimationFlag = 1;
				this->mPageMatrix.NewMatrix_init();
				this->mPageMatrix.NewMatrixInv_init();

				transitionParamIn = mATag * 1024 / pageAnimationDuration;//0.10
				gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
				//inoutTransitioin(transitionParamIn, transitionParamOut);
				curPageOffsetY = 0;

				if (gScreenOptions.enable)
				{
					this->mSubPageMatrix.NewMatrix_init();
					this->mSubPageMatrix.NewMatrixInv_init();
					curPageOffsetX = - (s32)(subScreenWidth << 4) + (s32)( (transitionParamOut * (subScreenWidth) ) / 64 ) ;
					this->mSubPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
				}
				curPageOffsetX = - (s32)(screenWidth << 4) + (s32)( (transitionParamOut * (screenWidth) ) / 64 ) ;
				this->mPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
//				gPageNeedRefresh = PageNormalRefreshFlag;


				//Horizontal translation animation ends
				if(curPageOffsetX == 0)
				{
					gPageAnimationFlag = 0;
				}
				break;

				case(PAGE_ANIMATION_SHIFTING_R):
					//shift from right to left
					gPageAnimationFlag = 1;
					this->mPageMatrix.NewMatrix_init();
					this->mPageMatrix.NewMatrixInv_init();

					transitionParamIn = (mATag) * 1024 / pageAnimationDuration;//0.10
					gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
					//inoutTransitioin(transitionParamIn, transitionParamOut);
					curPageOffsetY = 0;

					if (gScreenOptions.enable)
					{
						this->mSubPageMatrix.NewMatrix_init();
						this->mSubPageMatrix.NewMatrixInv_init();
						curPageOffsetX = ((subScreenWidth) << 4) - (s32)( (transitionParamOut * (subScreenWidth) ) / 64 ) ;
						this->mSubPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
					}
					curPageOffsetX = ((screenWidth) << 4) - (s32)( (transitionParamOut * (screenWidth) ) / 64 ) ;
					
					this->mPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
//					gPageNeedRefresh = PageNormalRefreshFlag;


					//end of shift from left to right
					if(curPageOffsetX == 0)
					{
						gPageAnimationFlag = 0;
					}
					break;

				case(PAGE_ANIMATION_SHIFTING_UP):
					//shifting from up to down
					gPageAnimationFlag = 1;
					this->mPageMatrix.NewMatrix_init();
					this->mPageMatrix.NewMatrixInv_init();

					transitionParamIn = (mATag) * 1024 / pageAnimationDuration;//0.10
					gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
					//inoutTransitioin(transitionParamIn, transitionParamOut);
					curPageOffsetX = 0;
					
					if (gScreenOptions.enable)
					{
						this->mSubPageMatrix.NewMatrix_init();
						this->mSubPageMatrix.NewMatrixInv_init();
						curPageOffsetY =  (s32)( (transitionParamOut * (subScreenHeight) ) / 64 ) - (s32)(subScreenHeight << 4) ;
						this->mSubPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
					}
					curPageOffsetY =  (s32)( (transitionParamOut * (screenHeight) ) / 64 ) - (s32)(screenHeight << 4) ;
					this->mPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
//					gPageNeedRefresh = PageNormalRefreshFlag;


					//end of shift from up to down
					if(curPageOffsetY == 0)
					{
						gPageAnimationFlag = 0;
					}
					break;

				case(PAGE_ANIMATION_SHIFTING_DOWN):
					//shifting from down to top
					gPageAnimationFlag = 1;
					this->mPageMatrix.NewMatrix_init();
					this->mPageMatrix.NewMatrixInv_init();

					transitionParamIn = (mATag) * 1024 / pageAnimationDuration;//0.10
					gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
					//inoutTransitioin(transitionParamIn, transitionParamOut);
					curPageOffsetX = 0;
					if (gScreenOptions.enable)
					{
						this->mSubPageMatrix.NewMatrix_init();
						this->mSubPageMatrix.NewMatrixInv_init();
						curPageOffsetY =  (s32)(subScreenHeight << 4) - (s32)( (transitionParamOut * (subScreenHeight) ) / 64 ) ;
						this->mSubPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
					}
					curPageOffsetY =  (s32)(screenHeight << 4) - (s32)( (transitionParamOut * (screenHeight) ) / 64 ) ;
					this->mPageMatrix.translate( (curPageOffsetX << 9),  (curPageOffsetY << 9));
//					gPageNeedRefresh = PageNormalRefreshFlag;

					

					//end of shift from down to top
					if(curPageOffsetY == 0)
					{
						gPageAnimationFlag = 0;
					}
					break;

				case( PAGE_ANIMATION_SWIPE_H ):
					if (windowPressed || (gPageReleasingAnimationFlag == 0 && this->animationStartOffset == 0))
					{
						if(this->animationStartOffset == 0 && (!firstload))
						{
							firstload = 1;
//							gPageNeedRefresh = PageNormalRefreshFlag;
							updatePageListOffsetAbsolute(0,0);
						}
						//pressing, abort animation
						break;
					}

					if (gPageReleasingAnimationFlag)
					{
						if (gPageReleasingSpeed >= 0)
						{
							if(gPageReleasingSpeed - releasingStep <= 0)
							{
								//end of release
								gPageReleasingSpeed = 0;
								gPageReleasingAnimationFlag = 0;
							}
							else
							{
								//release animation
								gPageReleasingSpeed -= releasingStep;
							}
						}
						else
						{
							if(gPageReleasingSpeed + releasingStep >= 0)
							{
								//end of release
								gPageReleasingSpeed = 0;
								gPageReleasingAnimationFlag = 0;
							}
							else
							{
								//release animation
								gPageReleasingSpeed += releasingStep;
							}
						}
						
						if (gPageReleasingAnimationFlag == 0)
						{
							this->LoadNextPageAfterReleasingAnimation();
						}
						else
						{
//							//releasing animation
//							curPageOffsetX = (this->mPageMatrix.G/8192);
//							//if(((cur_x - windowPressX) > 0) && (lastOffset_H <= 0))
//							if((curPageOffsetX >= 0) && (lastOffset_H <= 0))
//							{
//								lastShowPageID = WorkingPageID - 1;
//
//								if (lastShowPageID < 0)
//								{
//									lastShowPageID = 0;
//								}
//								if (lastShowPageID >= numofNormalPage)
//								{
//									lastShowPageID = numofNormalPage - 1;
//								}
//								//lastOffset_H = cur_x - windowPressX;
//								lastOffset_H = curPageOffsetX;
//								if(lastShowPageID == 0 && WorkingPageID == 0)
//								{
//
//								}
//								else
//								{
//									this->loadLastOnePageData(lastShowPageID, 1);
//								}
//							}
//							//else if(((cur_x - windowPressX) < 0) && (lastOffset_H >= 0))
//							else if((curPageOffsetX < 0) && (lastOffset_H >= 0))
//							{
//								lastShowPageID = WorkingPageID + 1;
//
//								if (lastShowPageID < 0)
//								{
//									lastShowPageID = 0;
//								}
//								if (lastShowPageID >= numofNormalPage)
//								{
//									lastShowPageID = numofNormalPage - 1;
//								}
//								//lastOffset_H = cur_x - windowPressX;
//								lastOffset_H = curPageOffsetX;
//								this->loadLastOnePageData(lastShowPageID, 1);
//							}
							updatePageListOffset(gPageReleasingSpeed, 0);
						}
					}
					else
					{
						//normal animation
						if(this->animationStartOffset > (s16)screenWidth || this->animationStartOffset < (s16)(-screenWidth))
						{
							MULTI_PAGE_ANIMATION_SWIPE_EXECUTING = 1;
							transitionParamIn = (mATag) * 1024 / curPageAnimationDuration;//0.10
							gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
							//inoutTransitioin(transitionParamIn, transitionParamOut);
							curPageOffsetX = this->animationStartOffset  - transitionParamOut * this->animationStartOffset / 1024;
							curPageOffsetY = 0;

							if(curPageOffsetX > 0)
							{
								if(((curPageOffsetX) / screenWidth) != LastOffsetForPageAnimation)
								{
									LastOffsetForPageAnimation = (curPageOffsetX) / screenWidth;
									this->loadLastTwoPageData((this->animationStartOffset - curPageOffsetX) / screenWidth + WorkingPageID - (this->animationStartOffset / screenWidth),1);
								}
							}
							else if(curPageOffsetX < 0)
							{
								if(((-curPageOffsetX) / screenWidth) != LastOffsetForPageAnimation)
								{
									LastOffsetForPageAnimation = (-curPageOffsetX) / screenWidth;
									if(((-curPageOffsetX) % screenWidth) == 0)
									{
										this->loadLastTwoPageData((-curPageOffsetX) / screenWidth - 1 + WorkingPageID,1);
									}
									else
									{
										this->loadLastTwoPageData((-curPageOffsetX) / screenWidth + WorkingPageID,1);
									}
								}
							}
							else if(curPageOffsetX == 0)
							{
								this->loadPage(WorkingPageID,1);
								MULTI_PAGE_ANIMATION_SWIPE_EXECUTING = 0;
							}
						
	//						gPageNeedRefresh = PageNormalRefreshFlag;
							updatePageListOffsetAbsolute(curPageOffsetX,0);

						}
						else
						{
							gPageAnimationFlag = 1;
							transitionParamIn = (mATag) * 1024 / curPageAnimationDuration;//0.10
							gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
							//inoutTransitioin(transitionParamIn, transitionParamOut);
							curPageOffsetX = this->animationStartOffset  - transitionParamOut * this->animationStartOffset / 1024;
							curPageOffsetY = 0;
						
	//						gPageNeedRefresh = PageNormalRefreshFlag;
							updatePageListOffsetAbsolute(curPageOffsetX,0);

							if(curPageOffsetX == 0)
								gPageAnimationFlag = 0;
						
						}
					}
					break;

				case( PAGE_ANIMATION_SWIPE_V ):
					if (windowPressed || (gPageReleasingAnimationFlag == 0 && this->animationStartOffset == 0))
					{
						if(this->animationStartOffset == 0 && (!firstload))
						{
							firstload = 1;
//							gPageNeedRefresh = PageNormalRefreshFlag;
							updatePageListOffsetAbsolute(0,1);
						}
						//pressing, abort animation
						break;
					}

					if (gPageReleasingAnimationFlag)
					{
						if (gPageReleasingSpeed >= 0)
						{
							if(gPageReleasingSpeed - releasingStep <= 0)
							{
								//end of release
								gPageReleasingSpeed = 0;
								gPageReleasingAnimationFlag = 0;
							}
							else
							{
								//release animation
								gPageReleasingSpeed -= releasingStep;
							}
						}
						else
						{
							if(gPageReleasingSpeed + releasingStep >= 0)
							{
								//end of release
								gPageReleasingSpeed = 0;
								gPageReleasingAnimationFlag = 0;
							}
							else
							{
								//release animation
								gPageReleasingSpeed += releasingStep;
							}
						}
						
						if (gPageReleasingAnimationFlag == 0)
						{
							this->LoadNextPageAfterReleasingAnimation();
						}
						else
						{
//							//releasing animation
//							curPageOffsetY = (this->mPageMatrix.H/8192);
//							//if(((cur_x - windowPressX) > 0) && (lastOffset_H <= 0)
//							if((curPageOffsetY >= 0) && (lastOffset_V <= 0))
//							{
//								lastShowPageID = WorkingPageID - 1;
//
//								if (lastShowPageID < 0)
//								{
//									lastShowPageID = 0;
//								}
//								if (lastShowPageID >= numofNormalPage)
//								{
//									lastShowPageID = numofNormalPage - 1;
//								}
//								//char text[100];
//								//WriteText(text,"111lastShowPageID :%d.\r\n",lastShowPageID);
//								//ERROR_PRINT(text);
//								//lastOffset_V = cur_y - windowPressY;
//								lastOffset_V = curPageOffsetY;
//								if(lastShowPageID == 0 && WorkingPageID == 0)
//								{
//
//								}
//								else
//								{
//									this->loadLastOnePageData(lastShowPageID, 1);
//								}
//							}
//							//else if(((cur_y - windowPressY) < 0) && (lastOffset_V >= 0))
//							else if((curPageOffsetY < 0) && (lastOffset_V >= 0))
//							{
//								lastShowPageID = WorkingPageID + 1;
//
//								if (lastShowPageID < 0)
//								{
//									lastShowPageID = 0;
//								}
//								if (lastShowPageID >= numofNormalPage)
//								{
//									lastShowPageID = numofNormalPage - 1;
//								}
//								//char text[100];
//								//WriteText(text,"222lastShowPageID :%d.\r\n",lastShowPageID);
//								//ERROR_PRINT(text);
//								//lastOffset_V = cur_y - windowPressY;
//								lastOffset_V = curPageOffsetY;
//								this->loadLastOnePageData(lastShowPageID, 1);
//							}
							updatePageListOffset(gPageReleasingSpeed, 1);
						}
					}
					else
					{
						if(this->animationStartOffset > (s16)screenHeight || this->animationStartOffset < (s16)(-screenHeight))
						{
							MULTI_PAGE_ANIMATION_SWIPE_EXECUTING = 1;
							transitionParamIn = (mATag) * 1024 / curPageAnimationDuration;//0.10
							gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
							//inoutTransitioin(transitionParamIn, transitionParamOut);
							curPageOffsetX = 0;
							curPageOffsetY = this->animationStartOffset  - transitionParamOut * this->animationStartOffset / 1024;

							if(curPageOffsetY > 0)
							{
								if(((curPageOffsetY) / screenHeight) != LastOffsetForPageAnimation)
								{
									LastOffsetForPageAnimation = (curPageOffsetY) / screenHeight;
									this->loadLastTwoPageData((this->animationStartOffset - curPageOffsetY) / screenHeight + WorkingPageID - (this->animationStartOffset / screenHeight),1);
								}
							}
							else if(curPageOffsetY < 0)
							{
								if(((-curPageOffsetY) / screenHeight) != LastOffsetForPageAnimation)
								{
									LastOffsetForPageAnimation = (-curPageOffsetY) / screenHeight;
									if(((-curPageOffsetY) % screenHeight) == 0)
									{
										this->loadLastTwoPageData((-curPageOffsetY) / screenHeight - 1 + WorkingPageID,1);
									}
									else
									{
										this->loadLastTwoPageData((-curPageOffsetY) / screenHeight + WorkingPageID,1);
									}
								}
							}
							else if(curPageOffsetY == 0)
							{
								this->loadPage(WorkingPageID,1);
								MULTI_PAGE_ANIMATION_SWIPE_EXECUTING = 0;
							}
						
	//						gPageNeedRefresh = PageNormalRefreshFlag;
							updatePageListOffsetAbsolute(curPageOffsetY,1);

						}
						else
						{
							gPageAnimationFlag = 1;
							transitionParamIn = (mATag) * 1024 / curPageAnimationDuration;//0.10
							gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
							//inoutTransitioin(transitionParamIn, transitionParamOut);
							curPageOffsetX = 0;
							curPageOffsetY = this->animationStartOffset  - transitionParamOut * this->animationStartOffset / 1024;
						
	//						gPageNeedRefresh = PageNormalRefreshFlag;
							updatePageListOffsetAbsolute(curPageOffsetY,1);

							if(curPageOffsetX == 0)
								gPageAnimationFlag = 0;
						}
					}
					break;

				case( PAGE_ANIMATION_MULTI_CARD ):
					if (windowPressed || (gPageReleasingAnimationFlag == 0 && this->animationStartOffset == 0))
					{
						if(this->animationStartOffset == 0 && (!firstload))
						{
							firstload = 1;
//							gPageNeedRefresh = PageNormalRefreshFlag;
							updatePageListOffsetAbsolute(0,0);
						}
						//pressing, abort animation
						break;
					}

					if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Auto)
					{
						gPageAnimationFlag = 1;

						if(mATag < curPageAnimationDuration/2)
						{
							transitionParamIn = (mATag) * 1024 / (curPageAnimationDuration / 2);//0.10
							gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
						
							pageScale = - transitionParamOut * (screenHeight / 2) / 1024;
						
							updatePageListScale(0, pageScale, 0);
						}
						else if(mATag >= curPageAnimationDuration/2)
						{
							transitionParamIn = (mATag - curPageAnimationDuration/2) * 1024 / (curPageAnimationDuration / 2);//0.10
							gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);

							curPageOffsetX = - transitionParamOut * this->animationStartOffset / 1024;

							updatePageListOffsetForMultiCard(curPageOffsetX, 0, 0, 1);

							if(mATag == curPageAnimationDuration)
							{
								gPageReleasingAnimationFlag = 0;
								this->LoadNextPageAfterMultiCardReleasingAnimation(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
							}
						}
					}
					else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_PullUp)
					{
						gPageAnimationFlag = 1;

						transitionParamIn = (mATag) * 1024 / curPageAnimationDuration;//0.10
						gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
						
						pageScale = this->animationStartOffset - transitionParamOut * (screenHeight / 2 + this->animationStartOffset) / 1024;
						
						updatePageListScale(0, pageScale, 0);
						
						if(mATag == curPageAnimationDuration)
							PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Swipe;
					}
					else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Swipe)
					{
						if (gPageReleasingAnimationFlag)
						{
							if (gPageReleasingSpeed >= 0)
							{
								if(gPageReleasingSpeed - releasingStep <= 0)
								{
									gPageReleasingSpeed = 0;
									gPageReleasingAnimationFlag = 0;
								}
								else
								{
									gPageReleasingSpeed -= releasingStep;
								}
							}
							else
							{
								if(gPageReleasingSpeed + releasingStep >= 0)
								{
									gPageReleasingSpeed = 0;
									gPageReleasingAnimationFlag = 0;
								}
								else
								{
									gPageReleasingSpeed += releasingStep;
								}
							}
							
							if (gPageReleasingAnimationFlag == 0)
							{
								//PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Focus;
								gAnimationClass.animationQueueReset();
							}
							else
							{
								updatePageListOffsetForMultiCard(gPageReleasingSpeed, 0);
							}
						}
					}
					else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Focus)
					{
						transitionParamIn = (mATag) * 1024 / curPageAnimationDuration;//0.10
						gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
						
						pageScale = -screenHeight / 2 + transitionParamOut * (screenHeight / 2) / 1024;
						
						updatePageListScale(this->animationStartOffset - screenWidth / 4, pageScale, 0);
						
						if(mATag == curPageAnimationDuration)
						{
							gPageAnimationFlag = 0;
							PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Idle;
						}
					}
					break;

				case(PAGE_ANIMATION_ZOOM):
					//zoom from center
					gPageAnimationFlag = 1;
					this->mPageMatrix.NewMatrix_init();
					this->mPageMatrix.NewMatrixInv_init();

					

					transitionParamIn = (mATag) * 1024 / pageAnimationDuration;//0.10
					gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, this->timingFun);
					//inoutTransitioin(transitionParamIn, transitionParamOut);  //scalter parameter
					//M_scale * 
					if(transitionParamOut < 2) //in case that A exceeds the boundary
						transitionParamOut = 2;
					//pageScale = (s32)( ((long long)(1 << 30) / transitionParamOut) ); //1.11.20
					pageScale = (transitionParamOut >> 1);//0.10 â? 1.6.9
					if(pageScale > (1 << 9) )
						pageScale = (1 << 9);
					else if(pageScale == 0)
						pageScale = 1;
					//offset = h/2
					

					if (gScreenOptions.enable)
					{
						//main
						curPageOffsetX = (s32)(this->screenAreas.mainOffsetX * 16 + gScreenOptions.mainWidth  * 8);  
						curPageOffsetY =  (s32)(this->screenAreas.mainOffsetY * 16 + gScreenOptions.mainHeight * 8); 
						this->mPageMatrix.translate( -(curPageOffsetX << 9), -(curPageOffsetY << 9) );
						this->mPageMatrix.scale(pageScale ,pageScale);
						this->mPageMatrix.translate( (curPageOffsetX << 9), (curPageOffsetY << 9) );
						//sub
						this->mSubPageMatrix.NewMatrix_init();
						this->mSubPageMatrix.NewMatrixInv_init();
						curPageOffsetX = (s32)(this->screenAreas.subOffsetX * 16 + gScreenOptions.subWidth  * 8);  
						curPageOffsetY =  (s32)(this->screenAreas.subOffsetY * 16 + gScreenOptions.subHeight * 8); 
						this->mSubPageMatrix.translate( -(curPageOffsetX << 9), -(curPageOffsetY << 9) );
						this->mSubPageMatrix.scale(pageScale ,pageScale);
						this->mSubPageMatrix.translate( (curPageOffsetX << 9), (curPageOffsetY << 9) );
					}
					else
					{
						curPageOffsetX = (s32)(SCREEN_WIDTH * 8);  
						curPageOffsetY =  (s32)(SCREEN_HEIGHT * 8); 
						this->mPageMatrix.translate( -(curPageOffsetX << 9), -(curPageOffsetY << 9) );
						this->mPageMatrix.scale(pageScale ,pageScale);
						this->mPageMatrix.translate( (curPageOffsetX << 9), (curPageOffsetY << 9) );
					}
					
					
					//end of zoom animation
					if(pageScale == (1 << 9))
					{
						gPageAnimationFlag = 0;
					}
					break;

				//Other circumstances
			default:
#ifdef AHMI_DEBUG
				ERROR_PRINT("This type of animation doesn't exist.\r\n");
#endif
				this->mPageMatrix.NewMatrix_init();
				this->mPageMatrix.NewMatrixInv_init();

				return (EM_enErrRetModularPage | EM_ErrRetPage_PageAnimationTypeErr);
		}
	}
	
	if((value == pageAnimationDuration) && (pageAnimationDuration != 0) && (gPageReleasingAnimationFlag == 0) && (TriggerAfterLoadActionFlag))
	{
		for(i=0;i != gPagePtr[WorkingPageID].mNumOfPageAction; i++)
		{
			if((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x8000) == 0)
			{
				addr = (u32)((gPagePtr[WorkingPageID].pActionStartADDR[i] & 0x7FFF) + gPagePtr[WorkingPageID].pActionInstructions);
                csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
			}
		}
//		gPageNeedRefresh = PageNormalRefreshFlag;
	}

	return AHMI_FUNC_SUCCESS;
}

void DynamicPageClass::LoadNextPageAfterReleasingAnimation(void)
{
	int oldNextPageID = 0;
	int nextPageID = 0, PageHiddenNumCur2Next = 0;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page

	s32 offset;

	if (this->mAnimationType == PAGE_ANIMATION_SWIPE_H )
	{
		offset = - this->mPageMatrix.G /8192;
		nextPageID = WorkingPageID + ( offset >= 0 ? (offset + SCREEN_WIDTH/2) : (offset - SCREEN_WIDTH/2))/(SCREEN_WIDTH);
	}
	else if (this->mAnimationType == PAGE_ANIMATION_SWIPE_V )
	{
		offset = - this->mPageMatrix.H /8192;
		nextPageID = WorkingPageID + ( offset >= 0 ? (offset + SCREEN_HEIGHT/2) : (offset - SCREEN_HEIGHT/2))/(SCREEN_HEIGHT);
	}
	else if (this->mAnimationType == PAGE_ANIMATION_MULTI_CARD )
	{
		offset = - this->mPageMatrix.G /8192;
		nextPageID = WorkingPageID + ( offset >= 0 ? (offset + SCREEN_WIDTH/2) : (offset - SCREEN_WIDTH/2))/(SCREEN_WIDTH);
	}
	
	for(int i = WorkingPageID; i != nextPageID;)
	{
		if(pPageHiddenTag->getArrayS32Value(i))
		{
			PageHiddenNumCur2Next++;
		}
		( offset >= 0 ? (i++):(i--) );
	}
	nextPageID = ( offset >= 0 ? (nextPageID + PageHiddenNumCur2Next):(nextPageID - PageHiddenNumCur2Next) );
	
	oldNextPageID = nextPageID;
	while(pPageHiddenTag->getArrayS32Value(nextPageID))
	{
		if(nextPageID >= WorkingPageID)
		{
			nextPageID++;
		}
		else
		{
			nextPageID--;
		}
	}
	
	if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
	{
		if (nextPageID < 0)
		{
//			int tmp = numofNormalPage - 1;
//			while(pPageHiddenTag->getArrayS32Value(tmp))
//			{
//				tmp--;
//			}
			int tmp = (nextPageID + numofNormalPage);
			TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(tmp + 1);
		}
		else if (nextPageID >= numofNormalPage)
		{
//			int tmp = 0;
//			while(pPageHiddenTag->getArrayS32Value(tmp))
//			{
//				tmp++;
//			}
			int tmp = (nextPageID - numofNormalPage);
			TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(tmp + 1);
		}
		else
		{
			TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace( nextPageID + 1);
		}
	}
	else
	{
		if (nextPageID < 0)
		{
			if(oldNextPageID != nextPageID)
			{
				nextPageID = WorkingPageID;
			}
			else
			{
				nextPageID = 0;
			}
		}
		if (nextPageID >= numofNormalPage)
		{
			if(oldNextPageID != nextPageID)
			{
				nextPageID = WorkingPageID;
			}
			else
			{
				nextPageID = numofNormalPage - 1;
			}
		}
		TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace( nextPageID + 1);
	}

	this->loadPage(nextPageID, 1);
}

void DynamicPageClass::LoadNextPageAfterMultiCardReleasingAnimation(s32 x, s32 y)
{
	DynamicPageClassPtr tempPagePtr;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page
	
	int curPageX;
	int curPageY;
	
	for(int i = 0; i < numofNormalPage; i++)
	{
		if(pPageHiddenTag->getArrayS32Value(i))
		{
			continue;
		}
		
		tempPagePtr = &gPagePtr[i];
		curPageX = tempPagePtr->mPageMatrix.G / 8192;
		curPageY = tempPagePtr->mPageMatrix.H / 8192;
		
		if((x >= curPageX) && (x <= curPageX + SCREEN_WIDTH / 2))
		{
			PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Focus;
			TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(i + 1);
			this->loadPage(i, 1);
			break;
		}
	}
}

u32 DynamicPageClass::mouseTouchCtrl(
		ActionTriggerClassPtr ActionPtr,
		PIDPoint* pPressPoint,
		PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
		PIDPoint* pAbsoluteReleasePoint,
		u8 u8_pageRefresh      
	)
{
	//DynamicPageClassPtr pCurrentPage;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);

	u16 cur_x;
	u16 cur_y;
	///s16 lastShowPageID = 0;
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page

	ElementPtr pNewElement;
	pNewElement.pageptr = this;

	//s32 pageOffsetX;
	//s32 pageOffsetY;

	int duration;

	funcStatus AHMI_status;

//	DynamicPageClassPtr tempPagePtr;
	
	//TextureClassPtr curTexture;
//	int i;

	//int nextPageID;

	//check the parameters
	if( NULL == ActionPtr)
	{
		ERROR_PRINT("ERROR: canvas illegal input");
		return (EM_enErrRetModularPage | EM_ErrRetPage_NULLPointerErr);
	}

	//this->mAnimationType = SUBCANVAS_ANIMATION_SWIPE_H;

	if (this->mAnimationType == PAGE_ANIMATION_SWIPE_H || this->mAnimationType == PAGE_ANIMATION_SWIPE_V || this->mAnimationType == PAGE_ANIMATION_MULTI_CARD)
	{

	}else{
		//if( ActionPtr->mInputType == ACTION_MOUSE_PRESS ) //mouse press
		//{
		//	cur_x  = (s16)(pPressPoint->x);
		//	cur_y  = (s16)(pPressPoint->y);
		//	windowPressX = cur_x;
		//	windowPressY = cur_y;
		//	windowHoldX = cur_x;
		//	windowHoldY = cur_y;
		//	windowPressed = 1;
		//}else if(ActionPtr->mInputType == ACTION_MOUSE_RELEASE || ActionPtr->mInputType == ACTION_MOUSE_CANCLE)
		//{
		//	windowPressed = 0;
		//}

		return WIDGET_TOUCH_CANCEL;
	}
	
	//get texture
	//pCurrentPage = &gPagePtr[WorkingPageID];
	
	if( ActionPtr->mInputType == ACTION_MOUSE_PRESS ) //mouse press
	{
		int DisplayPageNum = numofNormalPage;

		for(int tmp = 0; tmp < numofNormalPage; tmp++)
		{
			if(pPageHiddenTag->getArrayS32Value(tmp))
			{
				DisplayPageNum--;
			}
		}

		if(DisplayPageNum <= 1)
			return AHMI_FUNC_SUCCESS;

		cur_x  = (s16)(pPressPoint->x);
		cur_y  = (s16)(pPressPoint->y);
		windowPressX = cur_x;
		windowPressY = cur_y;
		windowHoldX = cur_x;
		windowHoldY = cur_y;
		windowSpeedX = 0;
		windowSpeedY = 0;
		windowPressed = 1;
		
		OldTickForWindowHold = AHMITick;
		//gAnimationClass.animationQueueReset();

		if(this->mAnimationType == PAGE_ANIMATION_MULTI_CARD)
		{
			if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Idle)
			{
				if(windowPressY < SCREEN_HEIGHT/2)
				{
					windowPressed = 0;
					PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_Idle;
				}
				else
				{
					PAGE_ANIMATION_MULTI_CARD_Status = PAGE_ANIMATION_MULTI_CARD_PullUp;
				}
			}
			else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Focus || 
					PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Auto ||
					PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Init)
			{
				windowPressed = 0;
			}
		}
	}
	else if(ActionPtr->mInputType == ACTION_MOUSE_HOLDING) //mouse up
	{
		if (windowPressed)
		{
			duration = AHMITick - OldTickForWindowHold;
			OldTickForWindowHold = AHMITick;
//			gPageNeedRefresh = PageMatrixRefreshFlag;
			cur_x  = (s16)(pReleasePoint->x);
            cur_y  = (s16)(pReleasePoint->y);
			if((cur_x == 0 && cur_y == 0) || (((cur_x - windowHoldX) == 0) && ((cur_y - windowHoldY) == 0)) || (duration == 0))
			{
				return AHMI_FUNC_SUCCESS;
			}

			if ((((s16)cur_x - (s16)windowPressX) < -Press2DraggingLength) || (((s16)cur_x - (s16)windowPressX) > Press2DraggingLength) 
				|| (((s16)cur_y - (s16)windowPressY) < -Press2DraggingLength) || (((s16)cur_y - (s16)windowPressY) > Press2DraggingLength))
			{
			}else{
				return AHMI_FUNC_SUCCESS;
			}

			if (this->mAnimationType == PAGE_ANIMATION_SWIPE_H )
			{
//				pageOffsetX = (pCurrentPage->mPageMatrix.G/8192);
//				//if(((cur_x - windowPressX) > 0) && (lastOffset_H <= 0))
//				if((pageOffsetX >= 0) && (lastOffset_H <= 0))
//				{
//					lastShowPageID = WorkingPageID - 1;
//
//					if (lastShowPageID < 0)
//					{
//						lastShowPageID = 0;
//					}
//					if (lastShowPageID >= numofNormalPage)
//					{
//						lastShowPageID = numofNormalPage - 1;
//					}
//					//lastOffset_H = cur_x - windowPressX;
//					lastOffset_H = pageOffsetX;
//					if(lastShowPageID == 0 && WorkingPageID == 0)
//					{
//
//					}
//					else
//					{
//						this->loadLastOnePageData(lastShowPageID, 1);
//					}
//				}
//				//else if(((cur_x - windowPressX) < 0) && (lastOffset_H >= 0))
//				else if((pageOffsetX < 0) && (lastOffset_H >= 0))
//				{
//					lastShowPageID = WorkingPageID + 1;
//
//					if (lastShowPageID < 0)
//					{
//						lastShowPageID = 0;
//					}
//					if (lastShowPageID >= numofNormalPage)
//					{
//						lastShowPageID = numofNormalPage - 1;
//					}
//					//lastOffset_H = cur_x - windowPressX;
//					lastOffset_H = pageOffsetX;
//					this->loadLastOnePageData(lastShowPageID, 1);
//				}
				updatePageListOffset(cur_x - windowHoldX, 0);
			}
			else if (this->mAnimationType == PAGE_ANIMATION_SWIPE_V )
			{
//				pageOffsetY = (pCurrentPage->mPageMatrix.H/8192);
//				//if(((cur_y - windowPressY) > 0) && (lastOffset_V <= 0))
//				if((pageOffsetY >= 0) && (lastOffset_V <= 0))
//				{
//					lastShowPageID = WorkingPageID - 1;
//
//					if (lastShowPageID < 0)
//					{
//						lastShowPageID = 0;
//					}
//					if (lastShowPageID >= numofNormalPage)
//					{
//						lastShowPageID = numofNormalPage - 1;
//					}
//					//char text[100];
//					//WriteText(text,"111lastShowPageID :%d.\r\n",lastShowPageID);
//					//ERROR_PRINT(text);
//					//lastOffset_V = cur_y - windowPressY;
//					lastOffset_V = pageOffsetY;
//					if(lastShowPageID == 0 && WorkingPageID == 0)
//					{
//
//					}
//					else
//					{
//						this->loadLastOnePageData(lastShowPageID, 1);
//					}
//				}
//				//else if(((cur_y - windowPressY) < 0) && (lastOffset_V >= 0))
//				else if((pageOffsetY < 0) && (lastOffset_V >= 0))
//				{
//					lastShowPageID = WorkingPageID + 1;
//
//					if (lastShowPageID < 0)
//					{
//						lastShowPageID = 0;
//					}
//					if (lastShowPageID >= numofNormalPage)
//					{
//						lastShowPageID = numofNormalPage - 1;
//					}
//					//char text[100];
//					//WriteText(text,"222lastShowPageID :%d.\r\n",lastShowPageID);
//					//ERROR_PRINT(text);
//					//lastOffset_V = cur_y - windowPressY;
//					lastOffset_V = pageOffsetY;
//					this->loadLastOnePageData(lastShowPageID, 1);
//				}
				updatePageListOffset(cur_y - windowHoldY, 1);
			}
			else if (this->mAnimationType == PAGE_ANIMATION_MULTI_CARD )
			{
				if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_PullUp)
				{
					updatePageListScale(0, cur_y - windowPressY, 0);
				}
				else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Swipe)
				{
					updatePageListOffsetForMultiCard(cur_x - windowHoldX, 0);
				}
			}
			windowSpeedX = (cur_x - windowHoldX) * 1000 / duration;
			windowSpeedY = (cur_y - windowHoldY) * 1000 / duration;
			windowHoldX = cur_x;
			windowHoldY = cur_y;
		}
	}else if(ActionPtr->mInputType == ACTION_MOUSE_RELEASE || ActionPtr->mInputType == ACTION_MOUSE_CANCLE)
	{
		if (!windowPressed)
			return AHMI_FUNC_SUCCESS;

		windowPressed = 0;

		cur_x  = (s16)(pReleasePoint->x);
		cur_y  = (s16)(pReleasePoint->y);

		if (((((s16)cur_x - (s16)windowPressX) < -Press2DraggingLength) || (((s16)cur_x - (s16)windowPressX) > Press2DraggingLength) 
				|| (((s16)cur_y - (s16)windowPressY) < -Press2DraggingLength) || (((s16)cur_y - (s16)windowPressY) > Press2DraggingLength)) && 
				(this->mAnimationType == PAGE_ANIMATION_SWIPE_H || this->mAnimationType == PAGE_ANIMATION_SWIPE_V))
		{
		}else{
			return AHMI_FUNC_SUCCESS;
		}

		//if ((this->mAnimationType == PAGE_ANIMATION_SWIPE_H ) && (cur_x == windowPressX))
		//{
		//	return AHMI_FUNC_SUCCESS;
		//}
		//else if ((this->mAnimationType == PAGE_ANIMATION_SWIPE_V ) && (cur_y == windowPressY))
		//{
		//	return AHMI_FUNC_SUCCESS;
		//}

		/*int duration = AHMITick - OldTickForWindowHold;
		if(duration < 0)
			duration  = 0 - duration;

		int moveTmp = 0;
		int moveDir = 0;
		if (this->mAnimationType == PAGE_ANIMATION_SWIPE_H )
		{
			moveTmp = (s16)(pReleasePoint->x) - (s16)(pPressPoint->x);
		}
		else
		{
			moveTmp = (s16)(pReleasePoint->y) - (s16)(pPressPoint->y);
		}
		if(moveTmp < 0)
		{
			moveDir = 1;
			moveTmp = 0 - moveTmp;
		}

		int movespeed = moveTmp * 1000 / duration;
		*/
		
		int movespeed;
		int moveDir = 0;
		if (this->mAnimationType == PAGE_ANIMATION_SWIPE_H )
		{
			movespeed = windowSpeedX;
		}
		else if (this->mAnimationType == PAGE_ANIMATION_SWIPE_V )
		{
			movespeed = windowSpeedY;
		}
		else if (this->mAnimationType == PAGE_ANIMATION_MULTI_CARD )
		{
			movespeed = windowSpeedX;
			if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_PullUp)
			{
				this->pageAnimationDuration = pageReleasingAnimationDuration;
				this->animationStartOffset = (s16)(pReleasePoint->y) - (s16)(pPressPoint->y);
				AHMI_status= sendToAnimationQueue(
						ANIMATION_REFRESH_PAGE,
						pNewElement,
						pNewElement
						);
				if(AHMI_status != AHMI_FUNC_SUCCESS)
					return (EM_enErrRetModularPage | EM_ErrRetPage_SendPageAnimationErr);
			
				return AHMI_FUNC_SUCCESS;
			}
			else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Swipe)
			{
				if((s16)(pReleasePoint->y) - (s16)(pPressPoint->y) >= -SwipeToFocusTouchThreshold && (s16)(pReleasePoint->y) - (s16)(pPressPoint->y) <= SwipeToFocusTouchThreshold &&
				   (s16)(pReleasePoint->x) - (s16)(pPressPoint->x) >= -SwipeToFocusTouchThreshold && (s16)(pReleasePoint->x) - (s16)(pPressPoint->x) <= SwipeToFocusTouchThreshold &&
				   gPageReleasingSpeed == 0 && gPageReleasingAnimationFlag == 0)
				{
					this->LoadNextPageAfterMultiCardReleasingAnimation(pReleasePoint->x, pPressPoint->y);
					return AHMI_FUNC_SUCCESS;
				}
			}
			else if(PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Focus)
			{
				return AHMI_FUNC_SUCCESS;
			}
		}
		if(movespeed < 0)
		{
			moveDir = 1;
			movespeed = 0 - movespeed;
		}
		
		int step = 2;
		
		//extern uint32_t AHMIDone_fps;
		int movePerFrame = movespeed / 33;//33 means FPS Max

		if((this->mAnimationType == PAGE_ANIMATION_SWIPE_H || this->mAnimationType == PAGE_ANIMATION_SWIPE_V) &&
		   (ConfigData.PageSwipeMode & PageSwipeMode_SwitchMultiPage) == PageSwipeMode_Normal)
		{
			int nextPageID = WorkingPageID;
			int oldNextPageID = 0;

			if(movespeed > 0x200)
			{
				nextPageID = WorkingPageID + (moveDir>0 ? 1:-1);
			}
			else
			{
				DynamicPageClassPtr pCurrentPage;
				int pageOffsetX;
				int pageOffsetY;
			
				pCurrentPage = &gPagePtr[WorkingPageID];
				if (this->mAnimationType == PAGE_ANIMATION_SWIPE_H )
				{
					pageOffsetX = (pCurrentPage->mPageMatrix.G/8192);
					if((pageOffsetX + (pageOffsetX > 0 ?SCREEN_WIDTH/2:-SCREEN_WIDTH/2)) / SCREEN_WIDTH)
						nextPageID = WorkingPageID - (( pageOffsetX+ (pageOffsetX > 0 ?SCREEN_WIDTH/2:-SCREEN_WIDTH/2)) / SCREEN_WIDTH);
				}else
				{
					pageOffsetY = (pCurrentPage->mPageMatrix.H/8192);
					if((pageOffsetY + (pageOffsetY > 0 ?SCREEN_HEIGHT/2:-SCREEN_HEIGHT/2)) / SCREEN_HEIGHT)
						nextPageID = WorkingPageID - (( pageOffsetY+ (pageOffsetY > 0 ?SCREEN_HEIGHT/2:-SCREEN_HEIGHT/2)) / SCREEN_HEIGHT);
				}
			}
			
			oldNextPageID = nextPageID;
			while(pPageHiddenTag->getArrayS32Value(nextPageID))
			{
				if(nextPageID >= WorkingPageID)
				{
					nextPageID++;
				}
				else
				{
					nextPageID--;
				}
			}
				
			windowPressed = 0;
			lastOffset_H = 0;
			lastOffset_V = 0;
		
			if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
			{
				if (nextPageID < 0)
				{
					int tmp = numofNormalPage - 1;
					while(pPageHiddenTag->getArrayS32Value(tmp))
					{
						tmp--;
					}
					TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(tmp + 1);
				}
				else if (nextPageID >= numofNormalPage)
				{
					int tmp = 0;
					while(pPageHiddenTag->getArrayS32Value(tmp))
					{
						tmp++;
					}
					TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace(tmp + 1);
				}
				else
				{
					TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace( nextPageID + 1);
				}
			}
			else
			{
				if (nextPageID < 0)
				{
					if(oldNextPageID != nextPageID)
					{
						nextPageID = WorkingPageID;
					}
					else
					{
						nextPageID = 0;
					}
				}
				if (nextPageID >= numofNormalPage)
				{
					if(oldNextPageID != nextPageID)
					{
						nextPageID = WorkingPageID;
					}
					else
					{
						nextPageID = numofNormalPage - 1;
					}
				}
				TagPtr->getTag(SYSTEM_PAGE_TAG).updateTagValueToValueSpace( nextPageID + 1);
			}

			this->loadPage(nextPageID, 1);
		}
		else
		{
			gPageReleasingAnimationFlag = 1;

			gPageReleasingSpeed = movePerFrame * (moveDir > 0 ? -1 : 1);
			if(gPageReleasingSpeed > PageSwipeSpeedMax)
				gPageReleasingSpeed = PageSwipeSpeedMax;
				
			if(gPageReleasingSpeed < -PageSwipeSpeedMax)
				gPageReleasingSpeed = -PageSwipeSpeedMax;

			//u16 oldPageAnimationDuration = this->pageAnimationDuration;
			//gPageReleasingAnimationDuration = ((movePerFrame + step-1) / step + 1) * 1000/AHMIDone_fps;
			//this->pageAnimationDuration = gPageReleasingAnimationDuration;
			this->pageAnimationDuration = 0xFFFF;//((movePerFrame + step-1) / step + 1) * 1000/AHMIDone_fps;
			AHMI_status= sendToAnimationQueue(
					ANIMATION_REFRESH_PAGE,
					pNewElement,
					pNewElement
					);
			//this->pageAnimationDuration = oldPageAnimationDuration;
			if(AHMI_status != AHMI_FUNC_SUCCESS)
				return (EM_enErrRetModularPage | EM_ErrRetPage_SendPageAnimationErr);
		}
		
	}
	return AHMI_FUNC_SUCCESS;
}

void DynamicPageClass::updatePageListScale(s16 offsetforAlign, s32 offset, u8 direction, u8 isReset)
{
	int i = 0, currentPageHidden = 0, PageHiddenTotalNum = 0;
	DynamicPageClassPtr tempPagePtr;
	DynamicPageClassPtr currentPagePtr;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page
	currentPagePtr = &gPagePtr[WorkingPageID];

	int curOffset;

	int curPageX;
	int curPageY;
	int curPageScale;
	
	int tempPageX;
	int tempPageY;

	//uint8_t CurDisplayPageNum = 0;
	uint16_t TotalDisplayPageNum = 0;

	if(offset > 0)
		return;

	for(i = 0; i < numofNormalPage; i++)
	{
		if(!pPageHiddenTag->getArrayS32Value(i))
		{
			TotalDisplayPageNum++;
		}
	}

	//update current sc offset
	if (direction)
	{
		offset = -offset;
		if(offset >= (SCREEN_WIDTH / 2))
			offset = (SCREEN_WIDTH / 2);
		curPageScale = 0x200 - 0x200 * offset / SCREEN_WIDTH;
		currentPagePtr->mPageMatrix.NewMatrix_init();
		currentPagePtr->mPageMatrix.NewMatrixInv_init();	
		currentPagePtr->mPageMatrix.translate( -((SCREEN_WIDTH / 2) << 13), -((SCREEN_HEIGHT / 2) << 13) );
		currentPagePtr->mPageMatrix.scale(curPageScale ,curPageScale);
		currentPagePtr->mPageMatrix.translate( (((SCREEN_WIDTH / 2) << 13)), ((SCREEN_HEIGHT / 2) << 13) );
		
		curOffset = offset * ((offsetforAlign) << 13) / (SCREEN_WIDTH / 2);
		currentPagePtr->mPageMatrix.translate(0, curOffset);
	}else
	{
		offset = -offset;
		if(offset >= (SCREEN_HEIGHT / 2))
			offset = (SCREEN_HEIGHT / 2);
		curPageScale = 0x200 - 0x200 * offset / SCREEN_HEIGHT;
		currentPagePtr->mPageMatrix.NewMatrix_init();
		currentPagePtr->mPageMatrix.NewMatrixInv_init();	
		currentPagePtr->mPageMatrix.translate( -((SCREEN_WIDTH / 2) << 13), -((SCREEN_HEIGHT / 2) << 13) );
		currentPagePtr->mPageMatrix.scale(curPageScale ,curPageScale);
		currentPagePtr->mPageMatrix.translate( (((SCREEN_WIDTH / 2) << 13)), ((SCREEN_HEIGHT / 2) << 13) );
		
		curOffset = offset * ((offsetforAlign) << 13) / (SCREEN_HEIGHT / 2);
		currentPagePtr->mPageMatrix.translate(curOffset, 0);
	}

	for (i = WorkingPageID; i < ConfigData.NumofPages; i++)
	{
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden || (i >= numofNormalPage))
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				tempPagePtr->mPageMatrix.NewMatrix_init();
				tempPagePtr->mPageMatrix.NewMatrixInv_init();	
				tempPagePtr->mPageMatrix.translate( -((SCREEN_WIDTH / 2) << 13), -((SCREEN_HEIGHT / 2) << 13) );
				tempPagePtr->mPageMatrix.scale(curPageScale ,curPageScale);
				tempPagePtr->mPageMatrix.translate( (((SCREEN_WIDTH / 2) << 13)), ((SCREEN_HEIGHT / 2) << 13) );
		
				curPageY = (s32)(SCREEN_HEIGHT * curPageScale / 0x200) * (i - WorkingPageID - PageHiddenTotalNum) + (curOffset >> 13);
				if((curPageY + currentPagePtr->mPageMatrix.H/8192) >= (SCREEN_HEIGHT * curPageScale / 0x200) * (TotalDisplayPageNum - 1))
				//if((i - WorkingPageID - PageHiddenTotalNum) >= (TotalDisplayPageNum - 1))
				{
					if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
					{
						curPageY -= (SCREEN_HEIGHT * curPageScale / 0x200) * TotalDisplayPageNum;
					}
				}
				tempPagePtr->mPageMatrix.translate(0, (curPageY << 13));
		
				tempPageX = 0;
				tempPageY = tempPagePtr->mPageMatrix.H/8192;
				
			}else
			{
				tempPagePtr->mPageMatrix.NewMatrix_init();
				tempPagePtr->mPageMatrix.NewMatrixInv_init();	
				tempPagePtr->mPageMatrix.translate( -((SCREEN_WIDTH / 2) << 13), -((SCREEN_HEIGHT / 2) << 13) );
				tempPagePtr->mPageMatrix.scale(curPageScale ,curPageScale);
				tempPagePtr->mPageMatrix.translate( (((SCREEN_WIDTH / 2) << 13)), ((SCREEN_HEIGHT / 2) << 13) );
				
				curPageX = (s32)(SCREEN_WIDTH * curPageScale / 0x200) * (i - WorkingPageID - PageHiddenTotalNum) + (curOffset >> 13);
				if((curPageX + currentPagePtr->mPageMatrix.G/8192) >= (SCREEN_WIDTH * curPageScale / 0x200) * (TotalDisplayPageNum - 1))
				//if((i - WorkingPageID - PageHiddenTotalNum) >= (TotalDisplayPageNum - 1))
				{
					if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
					{
						curPageX -= (SCREEN_WIDTH * curPageScale / 0x200) * TotalDisplayPageNum;
					}
				}
				tempPagePtr->mPageMatrix.translate((curPageX << 13), 0);
				
				tempPageX = tempPagePtr->mPageMatrix.G/8192;
				tempPageY = 0;
			}
		}
		else
		{
			tempPageX = currentPagePtr->mPageMatrix.G/8192;
			tempPageY = currentPagePtr->mPageMatrix.H/8192;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}

	PageHiddenTotalNum = 0;
	for (i = WorkingPageID - 1; i >= 0; i--)
	{
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden)
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				tempPagePtr->mPageMatrix.NewMatrix_init();
				tempPagePtr->mPageMatrix.NewMatrixInv_init();	
				tempPagePtr->mPageMatrix.translate( -((SCREEN_WIDTH / 2) << 13), -((SCREEN_HEIGHT / 2) << 13) );
				tempPagePtr->mPageMatrix.scale(curPageScale ,curPageScale);
				tempPagePtr->mPageMatrix.translate( (((SCREEN_WIDTH / 2) << 13)), ((SCREEN_HEIGHT / 2) << 13) );
		
				curPageY = (s32)(SCREEN_HEIGHT * curPageScale / 0x200) * (i - WorkingPageID + PageHiddenTotalNum) + (curOffset >> 13);
				if((curPageY + currentPagePtr->mPageMatrix.H/8192) <= -((SCREEN_HEIGHT * curPageScale / 0x200) * (TotalDisplayPageNum) - SCREEN_HEIGHT))
				//if((i - WorkingPageID + PageHiddenTotalNum) <= -(TotalDisplayPageNum - 1))
				{
					if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
					{
						curPageY += (SCREEN_HEIGHT * curPageScale / 0x200) * TotalDisplayPageNum;
					}
				}
				tempPagePtr->mPageMatrix.translate(0, (curPageY << 13));
		
				tempPageX = 0;
				tempPageY = tempPagePtr->mPageMatrix.H/8192;
				
			}else
			{
				tempPagePtr->mPageMatrix.NewMatrix_init();
				tempPagePtr->mPageMatrix.NewMatrixInv_init();	
				tempPagePtr->mPageMatrix.translate( -((SCREEN_WIDTH / 2) << 13), -((SCREEN_HEIGHT / 2) << 13) );
				tempPagePtr->mPageMatrix.scale(curPageScale ,curPageScale);
				tempPagePtr->mPageMatrix.translate( (((SCREEN_WIDTH / 2) << 13)), ((SCREEN_HEIGHT / 2) << 13) );
				
				curPageX = (s32)(SCREEN_WIDTH * curPageScale / 0x200) * (i - WorkingPageID + PageHiddenTotalNum) + (curOffset >> 13);
				if((curPageX + currentPagePtr->mPageMatrix.G/8192) <= -((SCREEN_WIDTH * curPageScale / 0x200) * (TotalDisplayPageNum) - SCREEN_WIDTH))
				//if((i - WorkingPageID + PageHiddenTotalNum) <= -(TotalDisplayPageNum - 1))
				{
					if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
					{
						curPageX += (SCREEN_WIDTH * curPageScale / 0x200) * TotalDisplayPageNum;
					}
				}
				tempPagePtr->mPageMatrix.translate((curPageX << 13), 0);
				
				tempPageX = tempPagePtr->mPageMatrix.G/8192;
				tempPageY = 0;
			}
		}
		else
		{
			tempPageX = curPageX;
			tempPageY = curPageY;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}
}

void DynamicPageClass::updatePageListOffsetForMultiCard(s16 offset, u8 direction, u8 isReset, u8 offsetMode)
{
	int i = 0, currentPageHidden = 0, PageHiddenTotalNum = 0;
	DynamicPageClassPtr tempPagePtr;
	DynamicPageClassPtr currentPagePtr;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page
	currentPagePtr = &gPagePtr[WorkingPageID];

	int curOffset;

	int curPageX;
	int curPageY;

	int tempPageX;
	int tempPageY;

	//uint8_t CurDisplayPageNum = 0;
	uint16_t TotalDisplayPageNum = 0;

	for(i = 0; i < numofNormalPage; i++)
	{
		if(!pPageHiddenTag->getArrayS32Value(i))
		{
			TotalDisplayPageNum++;
		}
	}

	curOffset = offset;
	//update current sc offset
	if (direction)
	{
		//vertical
		curPageX = 0;
		curPageY = currentPagePtr->mPageMatrix.H /8192;
		curPageY += offset;
		if(offsetMode)
		{
			curPageY = offset + SCREEN_HEIGHT/4;
			curOffset = curPageY;
		}
		if(curPageY > 0)
		{
			if(curPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT/2)
			{
				if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
				{
					//curPageY -= TotalDisplayPageNum * SCREEN_HEIGHT/2;
					curOffset = offset - TotalDisplayPageNum * SCREEN_HEIGHT/2;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		else
		{
			if(curPageY < -(TotalDisplayPageNum * SCREEN_HEIGHT/2 - SCREEN_HEIGHT))
			{
				if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
				{
					//curPageY += TotalDisplayPageNum * SCREEN_HEIGHT/2;
					curOffset = offset + TotalDisplayPageNum * SCREEN_HEIGHT/2;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		//curPos = curPageY;
		//currentPagePtr->mPageMatrix.TranslateToMatrix(0,0);
		if(offsetMode)
			curOffset -= currentPagePtr->mPageMatrix.H /8192;
		currentPagePtr->mPageMatrix.translate(0, curOffset<<13);
	}else
	{
		curPageX = currentPagePtr->mPageMatrix.G /8192;
		curPageX += offset;
		curPageY = 0;
		if(offsetMode)
		{
			curPageX = offset + SCREEN_WIDTH/4;
			curOffset = curPageX;
		}
		if(curPageX > 0)
		{
			if(curPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH/2)
			{
				if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
				{
					//curPageX -= TotalDisplayPageNum * SCREEN_WIDTH/2;
					curOffset = offset - TotalDisplayPageNum * SCREEN_WIDTH/2;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		else
		{
			if(curPageX < -(TotalDisplayPageNum * SCREEN_WIDTH/2 - SCREEN_WIDTH))
			{
				if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
				{
					//curPageX += TotalDisplayPageNum * SCREEN_WIDTH/2;
					curOffset = offset + TotalDisplayPageNum * SCREEN_WIDTH/2;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		//curPos = curPageX;
		//currentPagePtr->mPageMatrix.TranslateToMatrix(0,0);
		if(offsetMode)
			curOffset -= currentPagePtr->mPageMatrix.G /8192;
		currentPagePtr->mPageMatrix.translate(curOffset<<13,0);	
	}

	for (i = WorkingPageID; i < ConfigData.NumofPages; i++)
	{
		curOffset = offset;
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden || (i >= numofNormalPage))
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				//tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPageX = 0;
				tempPageY = tempPagePtr->mPageMatrix.H /8192;//(i-WorkingPageID - PageHiddenTotalNum)*SCREEN_HEIGHT/2 + curPageY;
				tempPageY += offset;
				if(offsetMode)
				{
					tempPageY = offset + SCREEN_HEIGHT/4 + (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_HEIGHT/2;
					curOffset = tempPageY;
				}
				if(tempPageY > 0)
				{
					if(tempPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT/2 )
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageY -= TotalDisplayPageNum * SCREEN_HEIGHT/2;
							curOffset = offset - TotalDisplayPageNum * SCREEN_HEIGHT/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageY < -(TotalDisplayPageNum * SCREEN_HEIGHT/2 - SCREEN_HEIGHT))
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageY += TotalDisplayPageNum * SCREEN_HEIGHT/2;
							curOffset = offset + TotalDisplayPageNum * SCREEN_HEIGHT/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				if(offsetMode)
					curOffset -= tempPagePtr->mPageMatrix.H /8192;
				tempPagePtr->mPageMatrix.translate(0, curOffset << 13);
				
			}else
			{
				//tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPageX = tempPagePtr->mPageMatrix.G /8192;//(i-WorkingPageID - PageHiddenTotalNum)*SCREEN_WIDTH/2 + curPageX;
				tempPageX += offset;
				tempPageY = 0;
				if(offsetMode)
				{
					tempPageX = offset + SCREEN_WIDTH/4 + (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_WIDTH/2;
					curOffset = tempPageX;
				}
				if(tempPageX > 0)
				{
					if(tempPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH/2)
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageX -= TotalDisplayPageNum * SCREEN_WIDTH/2;
							curOffset = offset - TotalDisplayPageNum * SCREEN_WIDTH/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageX < -(TotalDisplayPageNum * SCREEN_WIDTH/2 - SCREEN_WIDTH))
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageX += TotalDisplayPageNum * SCREEN_WIDTH/2;
							curOffset = offset + TotalDisplayPageNum * SCREEN_WIDTH/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				if(offsetMode)
					curOffset -= tempPagePtr->mPageMatrix.G /8192;
				tempPagePtr ->mPageMatrix.translate(curOffset<<13,0);
			}
		}
		else
		{
			tempPageX = curPageX;
			tempPageY = curPageY;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}

	PageHiddenTotalNum = 0;
	for (i = WorkingPageID - 1; i >= 0; i--)
	{
		curOffset = offset;
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden)
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				//tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPageX = 0;
				//tempPageY = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_HEIGHT/2 + curPageY;
				tempPageY = tempPagePtr->mPageMatrix.H /8192;
				tempPageY += offset;
				if(offsetMode)
				{
					tempPageY = offset + SCREEN_HEIGHT/4 + (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_HEIGHT/2;
					curOffset = tempPageY;
				}
				if(tempPageY > 0)
				{
					if(tempPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT/2)
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageY -= TotalDisplayPageNum * SCREEN_HEIGHT/2;
							curOffset = offset - TotalDisplayPageNum * SCREEN_HEIGHT/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageY < -(TotalDisplayPageNum * SCREEN_HEIGHT/2 - SCREEN_HEIGHT))
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageY += TotalDisplayPageNum * SCREEN_HEIGHT/2;
							curOffset = offset + TotalDisplayPageNum * SCREEN_HEIGHT/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				if(offsetMode)
					curOffset -= tempPagePtr->mPageMatrix.H /8192;
				tempPagePtr->mPageMatrix.translate(0, curOffset << 13);
				
			}else
			{
				//tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				//tempPageX = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_WIDTH/2 + curPageX;
				tempPageX = tempPagePtr->mPageMatrix.G /8192;
				tempPageX += offset;
				tempPageY = 0;
				if(offsetMode)
				{
					tempPageX = offset + SCREEN_WIDTH/4 + (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_WIDTH/2;
					curOffset = tempPageX;
				}
				if(tempPageX > 0)
				{
					if(tempPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH/2)
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageX -= TotalDisplayPageNum * SCREEN_WIDTH/2;
							curOffset = offset - TotalDisplayPageNum * SCREEN_WIDTH/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageX < -(TotalDisplayPageNum * SCREEN_WIDTH/2 - SCREEN_WIDTH))
					{
						if((ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (TotalDisplayPageNum > 2))
						{
							//tempPageX += TotalDisplayPageNum * SCREEN_WIDTH/2;
							curOffset = offset + TotalDisplayPageNum * SCREEN_WIDTH/2;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				if(offsetMode)
					curOffset -= tempPagePtr->mPageMatrix.G /8192;
				tempPagePtr ->mPageMatrix.translate(curOffset<<13,0);
			}
		}
		else
		{
			tempPageX = curPageX;
			tempPageY = curPageY;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}
}

void DynamicPageClass::updatePageListOffset(s16 offset, u8 direction, u8 isReset)
{
	int i = 0, currentPageHidden = 0, PageHiddenTotalNum = 0;
	DynamicPageClassPtr tempPagePtr;
	DynamicPageClassPtr currentPagePtr;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page
	currentPagePtr = &gPagePtr[WorkingPageID];

	int curPos;

	int curPageX;
	int curPageY;

	int tempPageX;
	int tempPageY;

	//uint8_t CurDisplayPageNum = 0;
	uint16_t TotalDisplayPageNum = 0;

	for(i = 0; i < numofNormalPage; i++)
	{
		if(!pPageHiddenTag->getArrayS32Value(i))
		{
			TotalDisplayPageNum++;
		}
	}

	//update current sc offset
	if (direction)
	{
		//vertical
		curPageX = 0;
		curPageY = currentPagePtr->mPageMatrix.H /8192;
		curPageY += offset;
		if(curPageY > 0)
		{
			if(curPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageY -= TotalDisplayPageNum * SCREEN_HEIGHT;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		else
		{
			if(curPageY < -(TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageY += TotalDisplayPageNum * SCREEN_HEIGHT;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		curPos = curPageY;
		currentPagePtr->mPageMatrix.TranslateToMatrix(0,0);
		currentPagePtr->mPageMatrix.translate(0, curPageY<<13);
	}else
	{
		curPageX = currentPagePtr->mPageMatrix.G /8192;
		curPageX += offset;
		curPageY = 0;
		if(curPageX > 0)
		{
			if(curPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageX -= TotalDisplayPageNum * SCREEN_WIDTH;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		else
		{
			if(curPageX < -(TotalDisplayPageNum - 1) * SCREEN_WIDTH)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageX += TotalDisplayPageNum * SCREEN_WIDTH;
				}
				else
				{
					gPageReleasingSpeed = 0;
				}
			}
		}
		curPos = curPageX;
		currentPagePtr->mPageMatrix.TranslateToMatrix(0,0);
		currentPagePtr->mPageMatrix.translate(curPageX<<13,0);	
	}

	for (i = WorkingPageID; i < ConfigData.NumofPages; i++)
	{
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden || (i >= numofNormalPage))
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPageX = 0;
				tempPageY = (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_HEIGHT + curPageY;
				if(curPos > 0)
				{
					if(tempPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY -= TotalDisplayPageNum * SCREEN_HEIGHT;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageY < -(TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY += TotalDisplayPageNum * SCREEN_HEIGHT;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
				
			}else
			{
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPageX = (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_WIDTH + curPageX;
				tempPageY = 0;
				if(curPos > 0)
				{
					if(tempPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX -= TotalDisplayPageNum * SCREEN_WIDTH;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageX < -(TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX += TotalDisplayPageNum * SCREEN_WIDTH;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
			}
		}
		else
		{
			tempPageX = curPageX;
			tempPageY = curPageY;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}

	PageHiddenTotalNum = 0;
	for (i = WorkingPageID - 1; i >= 0; i--)
	{
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden)
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPageX = 0;
				tempPageY = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_HEIGHT + curPageY;
				if(curPos > 0)
				{
					if(tempPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY -= TotalDisplayPageNum * SCREEN_HEIGHT;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageY < -(TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY += TotalDisplayPageNum * SCREEN_HEIGHT;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
				
			}else
			{
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPageX = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_WIDTH + curPageX;
				tempPageY = 0;
				if(curPos > 0)
				{
					if(tempPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX -= TotalDisplayPageNum * SCREEN_WIDTH;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				else
				{
					if(tempPageX < -(TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX += TotalDisplayPageNum * SCREEN_WIDTH;
						}
						else
						{
							gPageReleasingSpeed = 0;
						}
					}
				}
				tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
			}
		}
		else
		{
			tempPageX = curPageX;
			tempPageY = curPageY;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}

	//for (i = WorkingPageID; i < ConfigData.NumofPages; i++)
	//{	
	//	tempPagePtr = &gPagePtr[i];
	//	currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
	//	if(currentPageHidden)
	//	{
	//		PageHiddenTotalNum++;
	//	}
	//	if((i!=WorkingPageID) && (!currentPageHidden))
	//	{
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = 0;
	//			tempPageY = (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_HEIGHT + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//			
	//		}else
	//		{
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_WIDTH + curPageX;
	//			tempPageY = 0;
	//			tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
	//		}
	//	}else
	//	{
	//		tempPageX = curPageX;
	//		tempPageY = curPageY;
	//	}
	//	
	//	if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT) || (currentPageHidden))
	//	{
	//		//out of canvas rectangle, no need to draw
	//		tempPagePtr->shouldShow = 0;
	//	}else
	//	{
	//		if (isReset || (i >= numofNormalPage))
	//		{
	//			tempPagePtr->shouldShow = 0;
	//		}else
	//		{
	//			CurDisplayPageNum++;
	//			tempPagePtr->shouldShow = 1;
	//		}
	//	}
	//}

	//PageHiddenTotalNum = 0;
	//for (i = WorkingPageID - 1; i >= 0; i--)
	//{	
	//	tempPagePtr = &gPagePtr[i];
	//	currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
	//	if(currentPageHidden)
	//	{
	//		PageHiddenTotalNum++;
	//	}
	//	if((i!=WorkingPageID) && (!currentPageHidden))
	//	{
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = 0;
	//			tempPageY = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_HEIGHT + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//			
	//		}else
	//		{
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_WIDTH + curPageX;
	//			tempPageY = 0;
	//			tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
	//		
	//		}
	//	}else
	//	{
	//		tempPageX = curPageX;
	//		tempPageY = curPageY;
	//	}
	//	
	//	if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT) || (currentPageHidden))
	//	{
	//		//out of canvas rectangle, no need to draw
	//		tempPagePtr->shouldShow = 0;
	//	}else
	//	{
	//		if (isReset)
	//		{
	//			tempPagePtr->shouldShow = 0;
	//		}else
	//		{
	//			CurDisplayPageNum++;
	//			tempPagePtr->shouldShow = 1;
	//		}
	//	}
	//}

	////need display first page or last page
	//if((CurDisplayPageNum != 2) && (ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (curPos != 0))
	//{
	//	//need display last page
	//	if(curPos > 0)
	//	{
	//		int i = numofNormalPage - 1;
	//		while(pPageHiddenTag->getArrayS32Value(i))
	//		{
	//			i--;
	//		}
	//		tempPagePtr = &gPagePtr[i];
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = 0;
	//			tempPageY = -SCREEN_HEIGHT + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//			
	//		}else
	//		{
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = -SCREEN_WIDTH + curPageX;
	//			tempPageY = 0;
	//			tempPagePtr->mPageMatrix.translate(tempPageX<<13,0);
	//		}

	//		tempPagePtr->shouldShow = 1;
	//	}
	//	//need display first page
	//	else
	//	{
	//		int i = 0;
	//		while(pPageHiddenTag->getArrayS32Value(i))
	//		{
	//			i++;
	//		}
	//		tempPagePtr = &gPagePtr[i];
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = 0;
	//			tempPageY = SCREEN_HEIGHT + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//			
	//		}else
	//		{
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = SCREEN_WIDTH + curPageX;
	//			tempPageY = 0;
	//			tempPagePtr->mPageMatrix.translate(tempPageX<<13,0);
	//		}

	//		tempPagePtr->shouldShow = 1;
	//	}
	//}

	//char text[128];
	//sprintf(text,"CurDisplayPageNum is : %d ",CurDisplayPageNum);
	//ERROR_PRINT(text);
}


void DynamicPageClass::updatePageListOffsetAbsolute(s16 offset, u8 direction, u8 isReset)
{
	int i = 0, currentPageHidden = 0, PageHiddenTotalNum = 0;
	DynamicPageClassPtr tempPagePtr;
	DynamicPageClassPtr currentPagePtr;
//	u16 screenWidth;
//	u16 screenHeight;
//	u16 subScreenWidth;
//	u16 subScreenHeight;
//	int nextPageID;
	TagClassPtr pPageHiddenTag = &TagPtr->getTag(SYSTEM_PAGE_HIDDEN_TAG);
	s16 numofNormalPage = ConfigData.NumofPages - ConfigData.NumofSysPage - 1;// -1 for keyboard system page

	currentPagePtr = &gPagePtr[WorkingPageID];

	int curPageX;
	int curPageY;

	int tempPageX;
	int tempPageY;

	//uint8_t CurDisplayPageNum = 0;
	uint16_t TotalDisplayPageNum = 0;

	for(i = 0; i < numofNormalPage; i++)
	{
		if(!pPageHiddenTag->getArrayS32Value(i))
		{
			TotalDisplayPageNum++;
		}
	}

	//if (gScreenOptions.enable)
	//{
	//	currentPagePtr->mSubPageMatrix = currentPagePtr->mPageMatrix;
	//	screenWidth = gScreenOptions.mainWidth;
	//	screenHeight = gScreenOptions.mainHeight;
	//	subScreenWidth = gScreenOptions.subWidth;
	//	subScreenHeight = gScreenOptions.subHeight;
	//}
	//else
	//{
	//	screenWidth = SCREEN_WIDTH;
	//	screenHeight = SCREEN_HEIGHT;
	//}

	//update current sc offset
	if (direction)
	{
		//vertical
		curPageX = 0;
		//curPageY = currentPagePtr->mPageMatrix.H /8192;
		curPageY = offset;
		if(curPageY > 0)
		{
			if(curPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageY -= TotalDisplayPageNum * SCREEN_HEIGHT;
				}
			}
		}
		else
		{
			if(curPageY < -(TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageY += TotalDisplayPageNum * SCREEN_HEIGHT;
				}
			}
		}
		currentPagePtr->mPageMatrix.TranslateToMatrix(0,0);
		currentPagePtr->mPageMatrix.translate(0, curPageY<<13);
	}else
	{
		//curPageX = currentPagePtr->mPageMatrix.G /8192;
		curPageX = offset;
		curPageY = 0;
		if(curPageX > 0)
		{
			if(curPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageX -= TotalDisplayPageNum * SCREEN_WIDTH;
				}
			}
		}
		else
		{
			if(curPageX < -(TotalDisplayPageNum - 1) * SCREEN_WIDTH)
			{
				if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
				{
					curPageX += TotalDisplayPageNum * SCREEN_WIDTH;
				}
			}
		}
		currentPagePtr->mPageMatrix.TranslateToMatrix(0,0);
		currentPagePtr->mPageMatrix.translate(curPageX<<13,0);	
	}

	for (i = WorkingPageID; i < ConfigData.NumofPages; i++)
	{
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden || (i >= numofNormalPage))
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				tempPageX = 0;
				tempPageY = (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_HEIGHT + curPageY;
				if(offset > 0)
				{
					if(tempPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY -= TotalDisplayPageNum * SCREEN_HEIGHT;
						}
					}
				}
				else
				{
					if(tempPageY < -(TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY += TotalDisplayPageNum * SCREEN_HEIGHT;
						}
					}
				}
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
				
			}else
			{
				tempPageX = (i-WorkingPageID - PageHiddenTotalNum)*SCREEN_WIDTH + curPageX;
				tempPageY = 0;
				if(offset > 0)
				{
					if(tempPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX -= TotalDisplayPageNum * SCREEN_WIDTH;
						}
					}
				}
				else
				{
					if(tempPageX < -(TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX += TotalDisplayPageNum * SCREEN_WIDTH;
						}
					}
				}
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
			}
		}
		else
		{
			tempPageX = curPageX;
			tempPageY = curPageY;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}

	PageHiddenTotalNum = 0;
	for (i = WorkingPageID - 1; i >= 0; i--)
	{
		tempPagePtr = &gPagePtr[i];
		currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
		if(currentPageHidden)
		{
			PageHiddenTotalNum++;
			tempPagePtr->shouldShow = 0;
			continue;
		}
		if(i!=WorkingPageID)
		{
			if (direction)
			{
				//vertical
				tempPageX = 0;
				tempPageY = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_HEIGHT + curPageY;
				if(offset > 0)
				{
					if(tempPageY > (TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY -= TotalDisplayPageNum * SCREEN_HEIGHT;
						}
					}
				}
				else
				{
					if(tempPageY < -(TotalDisplayPageNum - 1) * SCREEN_HEIGHT)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageY += TotalDisplayPageNum * SCREEN_HEIGHT;
						}
					}
				}
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
				
			}else
			{
				tempPageX = (i-WorkingPageID + PageHiddenTotalNum)*SCREEN_WIDTH + curPageX;
				tempPageY = 0;
				if(offset > 0)
				{
					if(tempPageX > (TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX -= TotalDisplayPageNum * SCREEN_WIDTH;
						}
					}
				}
				else
				{
					if(tempPageX < -(TotalDisplayPageNum - 1) * SCREEN_WIDTH)
					{
						if(ConfigData.PageSwipeMode & PageSwipeMode_Loop)
						{
							tempPageX += TotalDisplayPageNum * SCREEN_WIDTH;
						}
					}
				}
				tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
				tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
			}
		}
		else
		{
			tempPageX = curPageX;
			tempPageY = curPageY;
		}

		if (((tempPageX + SCREEN_WIDTH) <= 0) || (tempPageX >= SCREEN_WIDTH) || ((tempPageY + SCREEN_HEIGHT) <= 0) || (tempPageY >= SCREEN_HEIGHT))
		{
			//out of canvas rectangle, no need to draw
			tempPagePtr->shouldShow = 0;
		}else
		{
			if (isReset)
			{
				tempPagePtr->shouldShow = 0;
			}else
			{
				tempPagePtr->shouldShow = 1;
			}
		}
	}

	//for (i = WorkingPageID; i < ConfigData.NumofPages; i++)
	//{
	//	tempPagePtr = &gPagePtr[i];
	//	currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
	//	if(currentPageHidden)
	//	{
	//		PageHiddenTotalNum++;
	//	}
	//	if((i!=WorkingPageID) && (!currentPageHidden))
	//	{
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPageX = 0;
	//			if (gScreenOptions.enable)
	//			{
	//				tempPagePtr->mSubPageMatrix.TranslateToMatrix(0,0);
	//				tempPageY = (i-WorkingPageID - PageHiddenTotalNum)*subScreenHeight + curPageY;
	//				tempPagePtr->mSubPageMatrix.translate(0, tempPageY << 13);
	//			}
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageY = (i-WorkingPageID - PageHiddenTotalNum)*screenHeight + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//		}
	//		else
	//		{
	//			tempPageY = 0;
	//			if (gScreenOptions.enable)
	//			{
	//				tempPagePtr->mSubPageMatrix.TranslateToMatrix(0,0);
	//				tempPageX = (i-WorkingPageID - PageHiddenTotalNum)*subScreenWidth + curPageX;
	//				tempPagePtr ->mSubPageMatrix.translate(tempPageX<<13,0);
	//			}
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = (i-WorkingPageID - PageHiddenTotalNum)*screenWidth + curPageX;
	//			tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
	//		}
	//	}else
	//	{
	//		tempPageX = curPageX;
	//		tempPageY = curPageY;
	//	}

	//	if (((tempPageX + screenWidth) <= 0) || (tempPageX >= screenWidth) || ((tempPageY + screenHeight) <= 0) || (tempPageY >= screenHeight) || (currentPageHidden))
	//	{
	//		//out of canvas rectangle, no need to draw
	//		tempPagePtr->shouldShow = 0;
	//	}else
	//	{
	//		if (isReset || (i >= numofNormalPage))
	//		{
	//			tempPagePtr->shouldShow = 0;
	//		}else
	//		{
	//			CurDisplayPageNum++;
	//			tempPagePtr->shouldShow = 1;
	//		}
	//	}
	//}
	//
	//PageHiddenTotalNum = 0;
	//for (i = WorkingPageID - 1; i >= 0; i--)
	//{
	//	tempPagePtr = &gPagePtr[i];
	//	currentPageHidden = pPageHiddenTag->getArrayS32Value(i);
	//	if(currentPageHidden)
	//	{
	//		PageHiddenTotalNum++;
	//	}
	//	if((i!=WorkingPageID) && (!currentPageHidden))
	//	{
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPageX = 0;
	//			if (gScreenOptions.enable)
	//			{
	//				tempPagePtr->mSubPageMatrix.TranslateToMatrix(0,0);
	//				tempPageY = (i-WorkingPageID + PageHiddenTotalNum)*subScreenHeight + curPageY;
	//				tempPagePtr->mSubPageMatrix.translate(0, tempPageY << 13);
	//			}
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageY = (i-WorkingPageID + PageHiddenTotalNum)*screenHeight + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//		}
	//		else
	//		{
	//			tempPageY = 0;
	//			if (gScreenOptions.enable)
	//			{
	//				tempPagePtr->mSubPageMatrix.TranslateToMatrix(0,0);
	//				tempPageX = (i-WorkingPageID + PageHiddenTotalNum)*subScreenWidth + curPageX;
	//				tempPagePtr ->mSubPageMatrix.translate(tempPageX<<13,0);
	//			}
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = (i-WorkingPageID + PageHiddenTotalNum)*screenWidth + curPageX;
	//			tempPagePtr ->mPageMatrix.translate(tempPageX<<13,0);
	//		}
	//	}else
	//	{
	//		tempPageX = curPageX;
	//		tempPageY = curPageY;
	//	}

	//	if (((tempPageX + screenWidth) <= 0) || (tempPageX >= screenWidth) || ((tempPageY + screenHeight) <= 0) || (tempPageY >= screenHeight) || (currentPageHidden))
	//	{
	//		//out of canvas rectangle, no need to draw
	//		tempPagePtr->shouldShow = 0;
	//	}else
	//	{
	//		if (isReset)
	//		{
	//			tempPagePtr->shouldShow = 0;
	//		}else
	//		{
	//			CurDisplayPageNum++;
	//			tempPagePtr->shouldShow = 1;
	//		}
	//	}
	//}

	////need display first page or last page
	//if((CurDisplayPageNum != 2) && (ConfigData.PageSwipeMode & PageSwipeMode_Loop) && (offset != 0))
	//{
	//	//need display last page
	//	if(offset > 0)
	//	{
	//		int i = numofNormalPage - 1;
	//		while(pPageHiddenTag->getArrayS32Value(i))
	//		{
	//			i--;
	//		}
	//		tempPagePtr = &gPagePtr[i];
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = 0;
	//			tempPageY = -SCREEN_HEIGHT + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//			
	//		}else
	//		{
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = -SCREEN_WIDTH + curPageX;
	//			tempPageY = 0;
	//			tempPagePtr->mPageMatrix.translate(tempPageX<<13,0);
	//		}

	//		tempPagePtr->shouldShow = 1;
	//	}
	//	//need display first page
	//	else
	//	{
	//		int i = 0;
	//		while(pPageHiddenTag->getArrayS32Value(i))
	//		{
	//			i++;
	//		}
	//		tempPagePtr = &gPagePtr[i];
	//		//update other sc offset
	//		if (direction)
	//		{
	//			//vertical
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = 0;
	//			tempPageY = SCREEN_HEIGHT + curPageY;
	//			tempPagePtr->mPageMatrix.translate(0, tempPageY << 13);
	//			
	//		}else
	//		{
	//			tempPagePtr->mPageMatrix.TranslateToMatrix(0,0);
	//			tempPageX = SCREEN_WIDTH + curPageX;
	//			tempPageY = 0;
	//			tempPagePtr->mPageMatrix.translate(tempPageX<<13,0);
	//		}

	//		tempPagePtr->shouldShow = 1;
	//	}
	//}
}

//-----------------------------
// nameï¼? inoutTransitioin
// Computation of slowly varying functions
// if(x < 0.5) y = 4*x^3
// else        y = 4(x-1)^3 + 1
//  
//     @param1 s16 in  0.10 
//     @param2 s16 out 0.10
// addtional comments :
//   none
//-----------------------------
funcStatus DynamicPageClass::inoutTransitioin(
	u16 in,   //0.10
	u16 &out  //0.10
	)
{
	if(in < 512)
	{
		out = (in * in * in) >> 18;
		return AHMI_FUNC_SUCCESS;
	}
	else if(in <= 1024)
	{

		out = 1024 - (((1024 - in) * (1024 - in) * (1024 - in)) >> 18) ;
		return AHMI_FUNC_SUCCESS;
	}
	out = 0;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? writeBackGround
// Background for writing static caching
//  
// addtional comments :
//   none
//-----------------------------
funcStatus DynamicPageClass::writeBackGround(u32 *psourceshift, NewMatrixPtr addtionalMatrix)
{
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? clearPage
// Write a solid black background.
//  
// addtional comments :
//   none
//-----------------------------
funcStatus DynamicPageClass::clearPage(
		u32 *psourceshift
		)
{
	return AHMI_FUNC_SUCCESS;
}
#endif
