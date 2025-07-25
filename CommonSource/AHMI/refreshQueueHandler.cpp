////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi
// Additional Comments:
//    the original structure in C
// 
////////////////////////////////////////////////////////////////////////////////
#include "AHMICfgDefinition.h"
#ifdef PC_SIM
#include "stdafx.h"
#endif
#include "FreeRTOS.h"
#include "queue.h"
#include "publicInclude.h"
#include "Simulator\ahmiv3.h"
#include "Simulator\ahmiv3_vd.h"
#include "AHMIDriver.h"
#include "drawImmediately_cd.h"
#include "semphr.h"
#include "TouchTask.h"
#include "player1c1.h"


#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi.h"
//#include "gc90x_config.h"
//#include "gc90x_initial.h"
//#include "MidTimer.h"
//#include "ahmiv4.h"
#endif
#ifdef EMBEDDED
extern u8 initAHMI;
#endif

#ifdef AHMI_CORE

extern k_msgq_handle_t   RefreshQueue;
extern k_msgq_handle_t   RefreshQueueWithoutDoubleBuffer;
extern u16             WorkingPageID;
extern ConfigInfoClass   ConfigData;
extern DynamicPageClassPtr  gPagePtr;
extern TagClassPtr     TagPtr;


extern uint8_t gPageAnimationFlag;
extern u8 windowPressed;

u8 staticRefresh = 0;
static u32 screenUpdateCount = 0;
//extern u8 animationDuration;
//extern u8 gPageNeedRefresh;

extern ProjectScreenOptions gScreenOptions;

#ifdef PC_SIM
extern u8 sourceBuffer[SoureBufferSize];
extern StructFrameBuffer  GlobalFrameBuffer[ MAX_SCREEN_SIZE *2];
extern StructFrameBuffer  GlobalFrameBufferSub[ MAX_SCREEN_SIZE *2];
extern StructFrameBuffer  GlobalFrameBuffer2[ MAX_SCREEN_SIZE *2]; //used for double buffer

extern u8  TempFrameBuffer[ MAX_SCREEN_SIZE];
extern u8  GlobalPageCacheBuffer[ PageCache_Size *2];  //global page cache bufffer for AHMI to write one frame data
extern u8  GlobalPageCacheBufferSub[ PageCache_Size *2];  //global page cache bufffer for AHMI to write one sub frame data


ahmi *myahmi;
extern HWND ViewHWND;
extern AHMIDriver mAHMIDriver;
#endif
extern PageScreenOptions currentPageScreenAreas;
extern u32  GlobalPageCacheUsed[2]; 
extern u32  GlobalPageCacheSubUsed[2];
extern uint32_t *pPageCacheAddr;
extern uint32_t *pSubPageCacheAddr;

//driver layer
extern uint32_t NumberOFLongTexture;
extern uint32_t NumberOFShortTexture;
extern uint32_t gNewSoureBufferSize;

extern uint32_t Refresh_state;

#ifdef PC_SIM
uint32_t AHMIDone_fps = 30;
#endif

u32 sourceshift = 0;


#if 0
//-----------------------------
// function: PageRefreshTask.
// @brief: Entrance for widget refresh task.
// parameters:
//   @param1 pvParameters  pointer to parameters.
//-----------------------------
extern u8 PAGE_ANIMATION_EXECUTING;
void PageRefreshTask(void* pvParameters)
{
	RefreshMsg refreshMsg;
	BaseType_t xStatus; 

#ifdef EMBEDDED 
    /* The calibration of TouchScreen can set initAHMI,so reset initAHMI before first time drawing. */
    //initAHMI = 0;
#endif
     
 	for(;;) 
 	{
#ifdef PC_SIM  
        /* Be triggered by task cycle. */
		if(RefreshQueue == NULL)
		{
			ERROR_PRINT("ERROR in PageRefreshTask: refresh queeu corruption");
			vTaskDelay( SCREEN_UPDATE_TIME / portTICK_RATE_MS );
			continue;
		}
        
        /* Receive message from RefreshQueue. */
 		xStatus=xQueueReceive(RefreshQueue,
 							(void*)&refreshMsg,
 							portMAX_DELAY);
 		if(xStatus!=pdPASS)
 		{
 			return;
 		}
#endif
        
#ifdef EMBEDDED 
		/* Be triggered by interruption. */
		//if(initAHMI)
		{
			/* wait for ahmi_done and display_done semaphore */
            xStatus = xSemaphoreTake(interuptSemaphore_pagerefresh, portMAX_DELAY);
            if(xStatus!=pdPASS)
            {
                return;
            }
		}
#endif   
		if(PAGE_ANIMATION_EXECUTING)
		{
			/* Only one static refresh type. */
			refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			refreshMsg.mElementPtr.pageptr = &(gPagePtr[WorkingPageID]);
		}
		else
		{
			/* Only one static refresh type. */
			refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
			refreshMsg.mElementPtr.pageptr = &(gPagePtr[WorkingPageID]);
		}
        
        //refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
	    //refreshMsg.mElementPtr.pageptr = &(gPagePtr[WorkingPageID]);
                 	
		/* Update all the tags*/
		//UpdateAllTags();
		if(refreshMsg.mElementPtr.pageptr == NULL)
		{
			ERROR_PRINT("ERROR in widgetRefreshTask: no element pointer given");
			continue;
		}  
            
        /* Calculate drawing parameters. */
		drawimmediately(&refreshMsg);
		
#ifdef EMBEDDED  
        /* Set refresh completion flag. */
        Refresh_state = 1;       
#endif       
		
#ifdef PC_SIM
		InvalidateRect(ViewHWND,NULL,FALSE);

        vTaskDelay(10 / portTICK_RATE_MS);
#endif
 	}
}
#endif 

#if defined(EMBEDDED) && defined(VERSION_2)
#if 0
void AHMIUsrDef_FreshTrigDraw(void)
{
    NVIC_SetPendingIRQ(LVDS_IN_IRQn);
}

//-----------------------------
// function: DrawTheFirstFrame.
// @brief: Entrance for widget refresh task.
// parameters:
//   @param1 pvParameters  pointer to parameters.
//-----------------------------
u32 DrawTheFirstFrame(void)
{
	RefreshMsg refreshMsg;

	/* The calibration of TouchScreen can set initAHMI,so reset initAHMI before first time drawing. */
	initAHMI = 0;
    
	/* Only one static refresh type. */
	refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
	refreshMsg.mElementPtr.pageptr = &(gPagePtr[WorkingPageID]);
	if(refreshMsg.mElementPtr.pageptr == NULL)
	{
		ERROR_PRINT("ERROR in widgetRefreshTask: no element pointer given");
		return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_NULLPointerErr);
	}  
   
	/* Update all the tags*/
	UpdateAllTags();
           
	/* Calculate drawing parameters. */
	drawimmediatelyForPlayer(&refreshMsg);
   
	/* Set refresh completion flag. */
	//Refresh_state = 1;

	/***Trigger the start AHMIDraw function***/
	//AHMIUsrDef_FreshTrigDraw();
	
	return AHMI_FUNC_SUCCESS;
}
#endif
#endif

#if defined(EMBEDDED) && defined(VERSION_2)
uint32_t SBAddress = SOURCE_BUFFER_START_ADDR;
#endif

#if 0
// function: drawimmediately.
// @brief: Calculate drawing parameters for whole page, from high level to low level is:page, canvas, widget, texture.
// parameters:
//   @param1 refreshMsg  pointer to refresh message.
//-----------------------------
u32 drawimmediately(RefreshMsg* refreshMsg)
{
	//ElementPtr curPtr;
	TileBoxClass SourceBox;
	u8 *sourcebufferaddr;
	TextureClassPtr  currenttexture = gPagePtr[WorkingPageID].pBackgroundTexture;
	DynamicPageClassPtr pCurrentPage;
#ifdef PC_SIM
	u32 sourceshift = 0;
#endif
	NewMatrix matrixTemp;

	//data check
	if(refreshMsg == NULL)
	{
		ERROR_PRINT("ERROR in drawimmediately: element pointer corruption");
		return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_NULLPointerErr);
	}
	if(refreshMsg->mElementType == ANIMATION_REFRESH_PAGE || 
		refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER || 
		refreshMsg->mElementType == ANIMAITON_REFRESH_STATIC_BUFFER)
	{
		if(refreshMsg->mElementPtr.pageptr == NULL)
		{
			ERROR_PRINT("ERROR in drawimmediately: element pointer null");
			return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_NULLPointerErr);
		}
		if(((refreshMsg->mElementPtr.pageptr - gPagePtr) >= ConfigData.NumofPages) || ((refreshMsg->mElementPtr.pageptr - gPagePtr) < 0))
		{
			ERROR_PRINT("ERROR in drawimmediately: element pointer out of boundary");
			return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_DataOverrun);
		}
	}
	else
	{
		ERROR_PRINT("ERROR in drawimmediately: element type not supported");
		return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_RefreshTypeErr);
	}
	
    /*If it's the first time for drawing a new page, change the refresh type. */    
    if(gPageNeedRefresh == PageNormalRefreshFlag)
	{
		refreshMsg->mElementType = ANIMATION_REFRESH_PAGE;
		gPageNeedRefresh = 0;
	}
	else if(gPageNeedRefresh == PageMatrixRefreshFlag)
	{
		refreshMsg->mElementType = ANIMATION_MATRIXREFRESH_PAGE;
		gPageNeedRefresh = 0;
	}

    /* Set box for whole page. */
	if(refreshMsg->mElementType == ANIMATION_REFRESH_PAGE || refreshMsg->mElementType == ANIMATION_MATRIXREFRESH_PAGE || refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER || refreshMsg->mElementType == ANIMAITON_REFRESH_STATIC_BUFFER) 
	{
		//curPtr.pageptr = gPagePtr;
		SourceBox.LeftBox = 0;
		SourceBox.TopBox = 0;
		SourceBox.RightBox = TILE_NUM_X - 1;
		SourceBox.ButtomBox = TILE_NUM_Y - 1;
	}
	else
	{
		return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_RefreshTypeErr);
	}
    
    /* Adjust box. */
	SourceBox.adjustSourceBuffer();
	
	taskENTER_CRITICAL();

#ifdef PC_SIM
	//set the drawing address
	mAHMIDriver.AHMI_SetFBAdress((uint32_t)GlobalFrameBuffer);
#endif

	#if defined(PC_SIM)
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	#endif
    
	#if defined(EMBEDDED) && defined(VERSION_2)
	sourcebufferaddr = (u8 *)SBAddress;
	#endif
	NumberOFLongTexture = 0;
	NumberOFShortTexture = 0;

	//pCurrentPage = &gPagePtr[WorkingPageID];
	//currenttexture = pCurrentPage->pBackgroundTexture;
	//if(pCurrentPage->mAnimationType != ANIMATION_REFRESH_NULL)
	//	pCurrentPage->clearPage(&sourceshift);
	//	matrixTemp.NewMatrix_init();
	//	matrixTemp.NewMatrixInv_init();
	//	matrixTemp.copyFromInv(&pCurrentPage->mPageMatrix);
	//	 /* Write background texture parameters to sourcebuffer. */
	//	currenttexture->WriteNewSourceBuffer(&sourceshift,&matrixTemp,currenttexture,NULL);


	for (int i = 0; i < ConfigData.NumofPages; i++)
	{
		pCurrentPage = &gPagePtr[i];
		if (!pCurrentPage->shouldShow)
		{
			continue;
		}

		//draw background
		currenttexture = pCurrentPage->pBackgroundTexture;

		if (pCurrentPage->mPageMatrix.G / 8192 > SCREEN_WIDTH || pCurrentPage->mPageMatrix.G/8192 + SCREEN_WIDTH < 0 || pCurrentPage->mPageMatrix.H/8192 > SCREEN_HEIGHT || pCurrentPage->mPageMatrix.H/8192 + SCREEN_HEIGHT < 0)
		{
			//bg out of screen
		}
		else
		{
			matrixTemp.NewMatrix_init();
			matrixTemp.NewMatrixInv_init();
			matrixTemp.copyFromInv(&pCurrentPage->mPageMatrix);
				/* Write background texture parameters to sourcebuffer. */
			currenttexture->WriteNewSourceBuffer(&sourceshift,&matrixTemp,currenttexture,NULL);
		}
		

		if (i == WorkingPageID)
		{
			/*If it's the first time for drawing a new page. */ 
			if(refreshMsg->mElementType == ANIMATION_REFRESH_PAGE)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,1,ANIMATION_REFRESH_PAGE,0);
			}
			else if(refreshMsg->mElementType == ANIMATION_MATRIXREFRESH_PAGE)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_MATRIXREFRESH_PAGE,0);
			}
			else if(refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_REFRESH_PAGE,0);
			}
			else if(refreshMsg->mElementType == ANIMAITON_REFRESH_STATIC_BUFFER)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_REFRESH_PAGE,1);
			}	
		}
		else
		{		
			pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_MATRIXREFRESH_PAGE,0);
		}
	}

	taskEXIT_CRITICAL();
	
	#ifdef VERSION_2
	if (sourceshift<SoureBufferSize-1)
	{
		*(sourcebufferaddr + sourceshift - (NewSoureBufferSize - 1)) |= (1 << 7);//texsecondheader = \0010 0000
	}
	#endif

#ifdef PC_SIM
	//mAHMIDriver.AHMI_SetSBSize(sourceshift/sizeof(NewSourceBuffer));
	mAHMIDriver.AHMI_SetSBSize(NumberOFLongTexture + NumberOFShortTexture); 
	mAHMIDriver.AHMI_TileBox(0,0,TILE_NUM_X - 1, TILE_NUM_Y - 1);
	if(mAHMIDriver.AHMI_GetStatus() == AHMI_DONE) 
	{
		mAHMIDriver.AHMI_Start();
	}
	else
	{
		return AHMI_FUNC_SUCCESS; 
	}
#endif
	return AHMI_FUNC_SUCCESS;
}

#endif

#ifdef CHIP_GC9002

void GetElementTileBox(RefreshMsg *msg, TileBoxClass* SourceBox)
{
	WidgetClassPtr pWidget;
	CanvasClassPtr pCanvas;
	SubCanvasClassPtr pSubCanvas;
	u16 widgetType;
	
	int16_t startX = 0;
	int16_t startY = 0;
	int16_t stopX = 0;
	int16_t stopY = 0;
	
	if((msg == NULL) || (SourceBox == NULL))
		return;

	if(msg->mElementType == ANIMATION_REFRESH_WIDGET)
	{
		pWidget = msg->mElementPtr.wptr;
		pCanvas = &(gPagePtr[WorkingPageID].pCanvasList[pWidget->ATTATCH_CANVAS]);
		widgetType = pWidget->WidgetType;
		
		pSubCanvas = &(gPagePtr[WorkingPageID].pSubCanvasList[pCanvas->mStartAddrOfSubCanvas + pCanvas->mFocusedCanvas]);
		
		startX = pCanvas->moffsetX + pSubCanvas->mOffsetX + pWidget->WidgetOffsetX;
		startY = pCanvas->moffsetY + pSubCanvas->mOffsetY + pWidget->WidgetOffsetY;
		
		if(startX < pCanvas->moffsetX)
		{
			startX = pCanvas->moffsetX;
		}
		if(startY < pCanvas->moffsetY)
		{
			startY = pCanvas->moffsetY;
		}
		
		stopX = pCanvas->moffsetX + pSubCanvas->mOffsetX + pWidget->WidgetOffsetX + pWidget->WidgetWidth;
		stopY = pCanvas->moffsetY + pSubCanvas->mOffsetY + pWidget->WidgetOffsetY + pWidget->WidgetHeight;
		
		if(stopX > pCanvas->moffsetX + pCanvas->mwidth)
		{
			stopX = pCanvas->moffsetX + pCanvas->mwidth;
		}
		if(stopY > pCanvas->moffsetY + pCanvas->mheight)
		{
			stopY = pCanvas->moffsetY + pCanvas->mheight;
		}
		
		if (widgetType == GALLERY)
		{
			startY = 0;
			stopY = gScreenHeight;
		}
	}
	
	else if(msg->mElementType == ANIMATION_REFRESH_SUBCANVAS)
	{
		pSubCanvas = msg->mElementPtr.scptr;
		pWidget = &(gPagePtr[WorkingPageID].pWidgetList[pSubCanvas->StartAddrOfWidget]);
		pCanvas = &(gPagePtr[WorkingPageID].pCanvasList[pWidget->ATTATCH_CANVAS]);
		
		
		startX = pCanvas->moffsetX + pSubCanvas->mOffsetX;
		startY = pCanvas->moffsetY + pSubCanvas->mOffsetY;
		
		if(startX < pCanvas->moffsetX)
		{
			startX = pCanvas->moffsetX;
		}
		if(startY < pCanvas->moffsetY)
		{
			startY = pCanvas->moffsetY;
		}
		
		stopX = pCanvas->moffsetX + pSubCanvas->mOffsetX + pSubCanvas->mWidth;
		stopY = pCanvas->moffsetY + pSubCanvas->mOffsetY + pSubCanvas->mHeight;
		
		if(stopX > pCanvas->moffsetX + pCanvas->mwidth)
		{
			stopX = pCanvas->moffsetX + pCanvas->mwidth;
		}
		if(stopY > pCanvas->moffsetY + pCanvas->mheight)
		{
			stopY = pCanvas->moffsetY + pCanvas->mheight;
		}
	}
	else if(msg->mElementType == ANIMATION_REFRESH_CANVAS)
	{
		pCanvas = msg->mElementPtr.cptr;
		
		startX = pCanvas->moffsetX;
		startY = pCanvas->moffsetY;
		
		stopX = pCanvas->moffsetX + pCanvas->mwidth;
		stopY = pCanvas->moffsetY + pCanvas->mheight;
	}
	else if(msg->mElementType == ANIMATION_REFRESH_PAGE)
	{
		startX = 0;
		startY = 0;
		stopX = gScreenWidth;
		stopY = gScreenHeight;
	}
	
	SourceBox->LeftBox = startX / TILESIZE;
	SourceBox->TopBox = startY / TILESIZE;
	SourceBox->RightBox = (stopX + TILESIZE - 1) / TILESIZE - 1;
	SourceBox->ButtomBox = (stopY + TILESIZE - 1) / TILESIZE - 1;
	
	if(startX >= stopX)
	{
		SourceBox->RightBox = SourceBox->LeftBox;
	}
	
	if(startY >= stopY)
	{
		SourceBox->ButtomBox = SourceBox->TopBox;
	}
	
	SourceBox->adjustSourceBuffer();
}

void PartialDrawMergeTileBox(RefreshMsg *DstMsg, RefreshMsg *SrcMsg, TileBoxClass* partialDrawBox)
{
	TileBoxClass elementBox;
	
	GetElementTileBox(SrcMsg, &elementBox);

	if(partialDrawBox->sourceBoxContain(&elementBox) == AHMI_FUNC_SUCCESS)
	{
		//DstMsg->mElementType = ANIMATION_REFRESH_WIDGET;
	}
	else if(elementBox.sourceBoxContain(partialDrawBox) == AHMI_FUNC_SUCCESS)
	{
		DstMsg->mElementType = SrcMsg->mElementType;
		DstMsg->mElementPtr.wptr = SrcMsg->mElementPtr.wptr;
		
		partialDrawBox->sourceBoxCopy(&elementBox);
	}
	else if(partialDrawBox->sourceBoxOverlap(&elementBox) == AHMI_FUNC_SUCCESS)
	{
		partialDrawBox->sourceBoxOr(&elementBox);
	}
	else 
	{
		partialDrawBox->sourceBoxOr(&elementBox);
		//DstMsg->mElementType = ANIMATION_REFRESH_PAGE;
	}
}

RefreshMsg gTestRefreshMsg;
TileBoxClass PartialDrawBox;
TileBoxClass OldSourceBox;
extern uint16_t XStart; 
extern uint16_t YStart;
extern uint16_t XEnd; 
extern uint16_t YEnd;

void SetPartialDrawRefreMsg(RefreshMsg *msg)
{

	if(gTestRefreshMsg.mElementType == ANIMATION_REFRESH_NULL)
	{
		if(msg->mElementType == ANIMATION_REFRESH_WIDGET)
		{
			gTestRefreshMsg.mElementType = msg->mElementType;
			gTestRefreshMsg.mElementPtr.wptr = msg->mElementPtr.wptr;
			
			GetElementTileBox(msg, &PartialDrawBox);
		}
		else if(msg->mElementType == ANIMATION_REFRESH_CANVAS)
		{
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_CANVAS;
			gTestRefreshMsg.mElementPtr.cptr = msg->mElementPtr.cptr;
			
			GetElementTileBox(msg, &PartialDrawBox);
		}
		else if(msg->mElementType == ANIMATION_REFRESH_SUBCANVAS)
		{
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_SUBCANVAS;
			gTestRefreshMsg.mElementPtr.scptr = msg->mElementPtr.scptr;
			
			GetElementTileBox(msg, &PartialDrawBox);
		}
		else if(msg->mElementType == ANIMATION_REFRESH_PAGE)
		{
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			
			GetElementTileBox(msg, &PartialDrawBox);
		}
		else
		{
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_NULL;
		}
	}
	else if(gTestRefreshMsg.mElementType == ANIMATION_REFRESH_WIDGET)
	{
		
		if(msg->mElementType == ANIMATION_REFRESH_WIDGET)
		{
			if(gTestRefreshMsg.mElementPtr.wptr == msg->mElementPtr.wptr)
			{
				gTestRefreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
			}
			else
			{
				#if 1
				PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
				#else
				gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
				#endif
			}
		}
		else if(msg->mElementType == ANIMATION_REFRESH_CANVAS)
		{
			#if 1
			PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
			#else	
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			#endif
		}
		else if(msg->mElementType == ANIMATION_REFRESH_SUBCANVAS)
		{
			#if 1
			PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
			#else	
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			#endif
		}
		else if(msg->mElementType == ANIMATION_REFRESH_PAGE)
		{
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
		}
	}
	else if(gTestRefreshMsg.mElementType == ANIMATION_REFRESH_CANVAS)
	{
		if(msg->mElementType == ANIMATION_REFRESH_WIDGET)
		{
			#if 1
			PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
			#else	
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			#endif
		}
		else if(msg->mElementType == ANIMATION_REFRESH_CANVAS)
		{
			if(gTestRefreshMsg.mElementPtr.cptr == msg->mElementPtr.cptr)
			{
				gTestRefreshMsg.mElementType = ANIMATION_REFRESH_CANVAS;
			}
			else
			{
				#if 1
				PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
				#else	
				gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
				#endif
			}
		}
		else if(msg->mElementType == ANIMATION_REFRESH_SUBCANVAS)
		{
			#if 1
			PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
			#else	
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			#endif
		}
		else if(msg->mElementType == ANIMATION_REFRESH_PAGE)
		{
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
		}
	}
	else if(gTestRefreshMsg.mElementType == ANIMATION_REFRESH_SUBCANVAS)
	{
		if(msg->mElementType == ANIMATION_REFRESH_WIDGET)
		{
			#if 1
			PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
			#else	
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			#endif
		}
		else if(msg->mElementType == ANIMATION_REFRESH_CANVAS)
		{
			#if 1
			PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
			#else	
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
			#endif
		}
		else if(msg->mElementType == ANIMATION_REFRESH_SUBCANVAS)
		{
			if(gTestRefreshMsg.mElementPtr.scptr == msg->mElementPtr.scptr)
			{
				gTestRefreshMsg.mElementType = ANIMATION_REFRESH_SUBCANVAS;
			}
			else
			{
				#if 1
				PartialDrawMergeTileBox(&gTestRefreshMsg, msg, &PartialDrawBox);
				#else	
				gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
				#endif
			}
		}
		else if(msg->mElementType == ANIMATION_REFRESH_PAGE)
		{
			gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
		}
	}
	else if(gTestRefreshMsg.mElementType == ANIMATION_REFRESH_PAGE)
	{
		gTestRefreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
	}
	else
	{
		gTestRefreshMsg.mElementType = ANIMATION_REFRESH_NULL;
	}
}

void GetPartialDrawRefreMsg(RefreshMsg *msg)
{
	if(msg == NULL)
		return;
		
	msg->mElementType = gTestRefreshMsg.mElementType;
	msg->mElementPtr.wptr = gTestRefreshMsg.mElementPtr.wptr;
}

void GetPartialDrawRefreBox(TileBoxClass* drawbox)
{
	if(drawbox == NULL)
		return;
		
	drawbox->LeftBox = PartialDrawBox.LeftBox;
	drawbox->TopBox = PartialDrawBox.TopBox;
	drawbox->RightBox = PartialDrawBox.RightBox;
	drawbox->ButtomBox = PartialDrawBox.ButtomBox;
}

void ClearPartialDrawRefreMsg(RefreshMsg *msg)
{
	gTestRefreshMsg.mElementType = ANIMATION_REFRESH_NULL;
	gTestRefreshMsg.mElementPtr.wptr = NULL;
	
	
	PartialDrawBox.LeftBox = 0;
	PartialDrawBox.TopBox = 0;
	PartialDrawBox.RightBox = 0;
	PartialDrawBox.ButtomBox = 0;
}
#endif
u32 lastsbufferaddr = 0;



#ifdef PAGE_CACHE_ENABLE	
void AddTextureFromPageCache(u32 *psourceshift, u32 cacheAddr)
{
	//uint32_t i = 0, j = 0;
	//uint32_t sourcebufferarry[16] = {0};
	
	#if (defined PC_SIM)   
	u8 *sourcebufferaddr;
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)  
	u8 *sourcebufferaddr = 0;	
	sourcebufferaddr = (u8 *)SBAddress;	
	#endif 

	//s32 Texture_OffsetX, Texture_OffsetY;
	//u32 Texture_Width, Texture_Height, TexLeftTileBox, TexTopTileBox, TexRightTileBox, TexButtomTileBox;
	u32 TextureAddr = 0;

	#if (defined PC_SIM)   
	TextureAddr = cacheAddr + START_ADDR_OFFSET;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)
	if(cacheAddr == 0)
		return;
		
	TextureAddr = cacheAddr;
	extern uint32_t START_ADDR_OFFSET_FROM_GCHEAP;
	TextureAddr += (START_ADDR_OFFSET_FROM_GCHEAP);
	#endif

	//modify sourcebuffer attr textype
	*(sourcebufferaddr + *psourceshift - gNewSoureBufferSize) &= 0x0F;

	#if (defined PC_SIM) 
	*(sourcebufferaddr + *psourceshift - gNewSoureBufferSize) |= RGBA8888;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)  
	if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGBA8888)
	{
		*(sourcebufferaddr + *psourceshift - gNewSoureBufferSize) |= (RGBA8888);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB565)
	{
		*(sourcebufferaddr + *psourceshift - gNewSoureBufferSize) |= (RGB565);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_Y8)
	{
		*(sourcebufferaddr + *psourceshift - gNewSoureBufferSize) |= (Y8);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC)
	{
		*(sourcebufferaddr + *psourceshift - gNewSoureBufferSize) |= (SFBC);
	}
	else
	{
		return;
	}
	#endif
	//modify sourcebuffer attr textype end

	//modify sourcebuffer addr
	*(sourcebufferaddr + (*psourceshift) - 5) = ((TextureAddr >> 24) & 0xFF);
	*(sourcebufferaddr + (*psourceshift) - 6) = ((TextureAddr >> 16) & 0xFF);
	*(sourcebufferaddr + (*psourceshift) - 7) = ((TextureAddr >> 8) & 0xFF);
	*(sourcebufferaddr + (*psourceshift) - 8) = ((TextureAddr) & 0xFF);
	//gNewSoureBufferSize = 24;
	//modify sourcebuffer addr end

	//Texture_Width = SCREEN_WIDTH;
	//Texture_Height = SCREEN_HEIGHT;
	//Texture_OffsetX = 0;
	//Texture_OffsetY = 0;

	////Calculation of tile boxes based on min/max coordinate points
	//TexLeftTileBox   = (Texture_OffsetX) / TILESIZE; 
	//TexTopTileBox    = (Texture_OffsetY) / TILESIZE;
	//TexRightTileBox  = ((Texture_OffsetX + SCREEN_WIDTH) + TILESIZE - 1) / TILESIZE - 1;
	//TexButtomTileBox = ((Texture_OffsetY + SCREEN_HEIGHT) + TILESIZE - 1) / TILESIZE - 1;

	//#if (defined PC_SIM) 
	//sourcebufferarry[0] = RGBA8888;
	//#endif

	//#if (defined VERSION_2) && (defined EMBEDDED)  
	//if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGBA8888)
	//{
	//	sourcebufferarry[0] = (RGBA8888);
	//}
	//else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB565)
	//{
	//	sourcebufferarry[0] = (RGB565);
	//}
	//else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_Y8)
	//{
	//	sourcebufferarry[0] = (Y8);
	//}
	//else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC)
	//{
	//	sourcebufferarry[0] = (SFBC);
	//}
	//else
	//{
	//	return;
	//}

	//#if (defined CHIP_GC9005) 
	//sourcebufferarry[0] |= (1 << 18);//CHIP_GC9005 sourcebuffer tilesize 16x16 {18bit: 0 means 32x32; 1 means 16x16};
	//#endif
	//#endif 

	//sourcebufferarry[1] = (Texture_OffsetX << 20) | (Texture_OffsetY << 4);
	//sourcebufferarry[2] = ((Texture_OffsetX + SCREEN_WIDTH) << 20) | (Texture_OffsetY << 4);
	//sourcebufferarry[3] = ((Texture_OffsetX + SCREEN_WIDTH) << 20) | ((Texture_OffsetY + SCREEN_HEIGHT) << 4);
	//sourcebufferarry[4] = (Texture_OffsetX << 20) | ((Texture_OffsetY + SCREEN_HEIGHT) << 4);
	//sourcebufferarry[5] = (1 << 20);
	//sourcebufferarry[6] = 0x0;
	//sourcebufferarry[7] = 0x0;
	//sourcebufferarry[8] = 0x0;
	//sourcebufferarry[9] = (1 << 20);
	//sourcebufferarry[10] = 0x0;
	//sourcebufferarry[11] = (-Texture_OffsetX) << 13;
	//sourcebufferarry[12] = (-Texture_OffsetY) << 13;
	//sourcebufferarry[13] = (gScreenWidth << 16) | gScreenHeight;
	//sourcebufferarry[14] = TextureAddr;
	//sourcebufferarry[15] = (TexLeftTileBox & 0xFF) | ((TexTopTileBox & 0xFF) << 8) | ((TexRightTileBox & 0xFF) << 16) | ((TexButtomTileBox & 0xFF) << 24);

	//for(int j = 0; j < 64; j++)
	//{
	//	*(sourcebufferaddr + (*psourceshift)++) = ((sourcebufferarry[j/4] >> (8*(j%4))) & 0xFF);//texture attribute
	//}
	//gNewSoureBufferSize = 64;
	//NumberOFLongTexture++;
}
//
//void AddTextureFromPageCache2(u32 *psourceshift, NewMatrix* matrixTemp, u32 cacheAddr)
//{
//	TextureClass NewTexture;
//	u16 textype =  RGBA8888 | NONMASKTEX | ABCDEFMATRIX | ADDRTEXTURE;
//	textype &= (~FORCE_UNDISPLAY);
//
//	NewTexture.mTexAttr = textype;
//	NewTexture.TexWidth = SCREEN_WIDTH;
//	NewTexture.TexHeight = SCREEN_HEIGHT;
//	NewTexture.OffsetX = 0;//1.11.4
//	NewTexture.OffsetY = 0;//1.11.4
//	NewTexture.RotateAngle = 0;//1.11.4
//	NewTexture.ShearAngleX = 0;//1.11.4
//	NewTexture.ShearAngleY = 0;
//	NewTexture.ScalerX = 512;//1.6.9
//	NewTexture.ScalerY = 512;//1.6.9
//	#ifdef VERSION_2
//	NewTexture.ShearAngleZX = 0;
//	NewTexture.ShearAngleZY = 0;
//	#endif
//	NewTexture.TexLeftTileBox   = 0 ;//left
//	NewTexture.TexTopTileBox    = 0;//top
//	NewTexture.TexRightTileBox  = ((0 + SCREEN_WIDTH) + TILESIZE - 1) / TILESIZE - 1;;//right
//	NewTexture.TexButtomTileBox = ((0 + SCREEN_HEIGHT) + TILESIZE - 1) / TILESIZE - 1;;//bottom
//	NewTexture.FocusedSlice     = 0;
//	NewTexture.SingleSliceSize  = 0;
//	NewTexture.SliceFlagsHeaderAddr = 0; //indicate whether there is a header for imgOrColorFlags and colors
//	
//	NewTexture.TexAddr = cacheAddr + START_ADDR_OFFSET;
//    NewTexture.staticTexture = 0;
//	NewTexture.useCanvasPoint = 0;
//
//	NewTexture.WriteNewSourceBuffer(&sourceshift,matrixTemp,&NewTexture,NULL);
//
//}

#ifdef PC_SIM
u32 transTempFrameBufferToRGBA8888(u8* buffer)
{
	#ifdef PC_SIM
	u32 color = 0, x,y;
	u8 color_r = 0; u8 color_g = 0; u8 color_b = 0;
	
	for(y = 0; y < SCREEN_HEIGHT; y++)
	{
		for(x = 0; x < SCREEN_WIDTH; x++)
		{
			color = *((u32 *)&TempFrameBuffer[3U * ((SCREEN_HEIGHT - (1 + y)) * SCREEN_WIDTH + x)]);
			color_b = ((color & 0xFF0000U) >> 16U) + 1U;
			color_g = ((color & 0xFF00U) >> 8U) + 1U;
			color_r = ((color & 0xFFU)) + 1U;
			color = (0xFF000000U | (color_r << 16) | (color_g << 8) | color_b);

			memcpy(&(buffer[(SCREEN_WIDTH * y + x) * 4]), &color, 4);
		}
	}
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
	#if defined(CHIP_GC9002)
	if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	{
		return AHMI_FUNC_SUCCESS;
	}
	uint32_t mallocsizeforSnapshot = gScreenWidth * gScreenHeight * 2;//RGB565
	#elif defined(CHIP_GC9005)
	uint32_t mallocsizeforSnapshot = gScreenWidth * gScreenHeight;//SFBC
	#endif
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

	
	return AHMI_FUNC_SUCCESS;
}
#endif

u8 getPageCacheID(u32 cacheAddr, RefreshScreenTypes screenType)
{
	u32 tmp,tmp1;

	#ifdef PC_SIM
	switch (screenType)
	{
		case RefreshScreenSub:
			tmp = (cacheAddr - START_ADDR_OF_PAGE_SUB_CACHE);
			break;
		case RefreshScreenMain:
		case RefreshScreenSingle:
		default:
			tmp = (cacheAddr - START_ADDR_OF_PAGE_CACHE);
			break;
	}
	tmp1 = PageCache_Size;
	tmp = tmp / tmp1;
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
	switch (screenType)
	{
		case RefreshScreenSub:
			tmp = (cacheAddr - StartAddrOfSubDisplayBuffer);
			break;
		case RefreshScreenMain:
		case RefreshScreenSingle:
		default:
			tmp = (cacheAddr - StartAddrOfDisplayBuffer);
			break;
	}
	tmp1 = SizeOfDisplayBuffer;
	tmp = tmp / tmp1 - 2;
	#endif
	return tmp;
}

u32 drawPageCacheForPlayer(u16 pageId, RefreshScreenTypes screenType, u32 *psourceshift, u32* cacheAddr)
{
	//ElementPtr curPtr;
	TileBoxClass SourceBox;
	TileBoxClass ScreenSourceBox;
	u8 *sourcebufferaddr;
	TextureClassPtr  currenttexture = gPagePtr[WorkingPageID].pBackgroundTexture;
	DynamicPageClassPtr pCurrentPage;
	u32 i;
	s16 cachePos = -1;

	u32 sourceshift_tmp = 0;
	u32 old_gNewSoureBufferSize = gNewSoureBufferSize;
	u8 Bsourcebuffer[128];
	u8 BsourcebufferLength = 24;
	NewMatrix matrixTemp;
	NewMatrix screenMatrix;

#if 0
#ifdef CHIP_GC9002	
	RefreshMsg partialRefreshMsg;
	uint32_t refreshFlag = 0;
	uint32_t lastFrameFlag = 0;
	
	int16_t startX = 0;
	int16_t startY = 0;
	int16_t stopX = 0;
	int16_t stopY = 0;
#endif
#endif


	//#if defined(EMBEDDED) && defined(VERSION_2) && !defined(CHIP_GC9002_NoHyper)
	//if(lastsbufferaddr == SBAddress)
	//	return AHMI_FUNC_SUCCESS;
	//#endif

	

    /* Set box for whole page. */
	SourceBox.LeftBox = 0;
	SourceBox.TopBox = 0;
	SourceBox.RightBox = TILE_NUM_X - 1;
	SourceBox.ButtomBox = TILE_NUM_Y - 1;

#if 0
#ifdef CHIP_GC9002	
	refreshFlag = 0;
	
	if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
	{
		if(refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER)
		{
			partialRefreshMsg.mElementType = ANIMATION_REFRESH_NULL;
		
			//temporary
//			refreshMsg->mElementType = ANIMATION_MATRIXREFRESH_PAGE;
		
		
			GetPartialDrawRefreMsg(&partialRefreshMsg);
		
			if((partialRefreshMsg.mElementType >= ANIMATION_REFRESH_SUBCANVAS) && (partialRefreshMsg.mElementType <= ANIMATION_REFRESH_WIDGET))
			//if(partialRefreshMsg.mElementType == ANIMATION_REFRESH_WIDGET)
			{
		
				GetPartialDrawRefreBox(&SourceBox);
		
				startX = SourceBox.LeftBox * TILESIZE;
				startY = SourceBox.TopBox * TILESIZE;
				stopX = (SourceBox.RightBox + 1) * TILESIZE - 1;
				stopY = (SourceBox.ButtomBox + 1) * TILESIZE - 1;
				
//				if((SourceBox.LeftBox == 0) && (SourceBox.RightBox == 0))
//				{
//					SourceBox.LeftBox = SourceBox.RightBox;
//				}
		
				refreshFlag = 1;
			}
		}
		else if(refreshMsg->mElementType == ANIMAITON_REFRESH_LAST_FRAME)
		{
			//temporary
			refreshMsg->mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
			
			SourceBox.LeftBox = OldSourceBox.LeftBox;
			SourceBox.TopBox = OldSourceBox.TopBox;
			SourceBox.RightBox = OldSourceBox.RightBox;	
			SourceBox.ButtomBox = OldSourceBox.ButtomBox;
			
			refreshFlag = 1;
			lastFrameFlag = 1;
		}
		
		ClearPartialDrawRefreMsg(&partialRefreshMsg);
	}
		
#endif

    /* Adjust box. */
	SourceBox.adjustSourceBuffer();
	
//	if((SourceBox.LeftBox == 0) && (SourceBox.RightBox == 0))
//	{
//		SourceBox.LeftBox = 0;
//		SourceBox.TopBox = 0;
//		SourceBox.RightBox = TILE_NUM_X - 1;
//		SourceBox.ButtomBox = TILE_NUM_Y - 1;
//	}

#ifdef CHIP_GC9002	
	if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
	{
//		XStart = SourceBox.LeftBox * TILESIZE;
//		YStart = SourceBox.TopBox * TILESIZE;
//		XEnd = (SourceBox.RightBox + 1) * TILESIZE - 1;	
//		YEnd = (SourceBox.ButtomBox + 1) * TILESIZE - 1;	
	
		XStart = SourceBox.LeftBox;
		YStart = SourceBox.TopBox;
		XEnd = SourceBox.RightBox;	
		YEnd = SourceBox.ButtomBox;
	}
#endif
	
//	taskENTER_CRITICAL();
#endif

	//set the drawing address
	switch (screenType)
	{
	case RefreshScreenSub:
		for (i = 0; i < 2; i++)
		{
			if (GlobalPageCacheSubUsed[i] == 0)
			{
				cachePos = i;
				break;
			}
		}
		if (cachePos == -1)
		{
			return AHMI_FUNC_FAILURE;
		}

		#ifdef PC_SIM
		*cacheAddr = START_ADDR_OF_PAGE_SUB_CACHE + cachePos * PageCache_Size;
		#endif
		#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
		*cacheAddr = StartAddrOfSubDisplayBuffer + (cachePos + 2) * SizeOfDisplayBuffer;
		#endif
		GlobalPageCacheSubUsed[i] = *cacheAddr;
		break;
	case RefreshScreenMain:
	case RefreshScreenSingle:
	default:
		for (i = 0; i < 2; i++)
		{
			if (GlobalPageCacheUsed[i] == 0)
			{
				cachePos = i;
				break;
			}
		}
		if (cachePos == -1)
		{
			return AHMI_FUNC_FAILURE;
		}

		#ifdef PC_SIM
		*cacheAddr = START_ADDR_OF_PAGE_CACHE + cachePos * PageCache_Size;
		#endif
		#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
		*cacheAddr = StartAddrOfDisplayBuffer + (cachePos + 2) * SizeOfDisplayBuffer;
		#endif
		GlobalPageCacheUsed[i] = *cacheAddr;
		break;
	}

	#if defined(PC_SIM)
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	#endif
    
	#if defined(EMBEDDED) && defined(VERSION_2)
	sourcebufferaddr = (u8 *)SBAddress;
	lastsbufferaddr = SBAddress;
	#endif

	//store sourcebuffer
	//if(((*(sourcebufferaddr + *psourceshift + 3)) & 0x80) == 0)
	//{
	//	BsourcebufferLength = 64;
	//}
	for(i = 0; i < *psourceshift; i++)
	{
		Bsourcebuffer[i] = *(sourcebufferaddr + i);
	}
	//store sourcebuffer end

	NumberOFLongTexture = 0;
	NumberOFShortTexture = 0;

	
	pCurrentPage = &gPagePtr[pageId];
	

	pCurrentPage->screenType = screenType;

	screenMatrix.NewMatrix_init();
	screenMatrix.NewMatrixInv_init();
	
	matrixTemp.NewMatrix_init();
	matrixTemp.NewMatrixInv_init();
		

		
	matrixTemp.LeftMultiMatrix(&screenMatrix);

	//draw background
	currenttexture = pCurrentPage->pBackgroundTexture;

	if (matrixTemp.G / 8192 > SCREEN_WIDTH || matrixTemp.G/8192 + SCREEN_WIDTH < 0 || matrixTemp.H/8192 > SCREEN_HEIGHT || matrixTemp.H/8192 + SCREEN_HEIGHT < 0)
	{
		//bg out of screen
	}
	else
	{

#if 0			
#ifdef CHIP_GC9002				
		if(GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_DOUBLE_BUFFER)
		{
			TileBoxClass 	tempSourceBox;
				
			tempSourceBox.sourceBoxCopy(&SourceBox);
				
			if(refreshFlag == 1)
			{
				TextureClass 	texture;
				
				if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGBA8888)
				{
					texture.mTexAttr = RGBA8888;
				}
				else if((AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB565) || (AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB888) || (AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC))
				{
					texture.mTexAttr = RGB565;
				}
				else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_Y8)
				{
					texture.mTexAttr = Y8;
				}
				texture.mTexAttr |= DRAWING | NONMASKTEX | EFMATRIX | ADDRTEXTURE;
				texture.FocusedSlice = 0;

				texture.RotateAngle = (short)(0);
				texture.ShearAngleX = (short)(0);
				texture.ShearAngleY = (short)(0);
				texture.ShearAngleZX = (short)(0);
				texture.ShearAngleZY = (short)(0);
				texture.ScalerX = (short)(1<<9);
				texture.ScalerY = (short)(1<<9);
				texture.useCanvasPoint = 0;
				texture.SliceFlagsHeaderAddr = 0;
				texture.SingleSliceSize = 0;
			
				texture.TexWidth =  SCREEN_WIDTH;
				texture.TexHeight = SCREEN_HEIGHT;
				texture.OffsetX = 0;
				texture.OffsetY = 0;
				texture.TexLeftTileBox = 0;
				texture.TexTopTileBox = 0;
				texture.TexRightTileBox = SCREEN_WIDTH / TILESIZE;
				texture.TexButtomTileBox = SCREEN_HEIGHT / TILESIZE;
				texture.TexAddr = DISPLAY_GetPriAddress(); // AHMI_GetFBAdress();
				
//					texture.WriteNewSourceBuffer(&sourceshift,&matrixTemp,&texture,NULL);
				if(OldSourceBox.LeftBox >= TILE_NUM_X)
				{
					OldSourceBox.sourceBoxCopy(&SourceBox);
				}
				else
				{
//						if(OldSourceBox.sourceBoxContain(&SourceBox) == AHMI_FUNC_SUCCESS)
//						{
//							refreshMsg->mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
//						}
						
					OldSourceBox.sourceBoxOr(&SourceBox);
					OldSourceBox.adjustSourceBuffer();
					SourceBox.sourceBoxCopy(&OldSourceBox);
				}
					
				XStart = OldSourceBox.LeftBox;
				YStart = OldSourceBox.TopBox;
				XEnd = OldSourceBox.RightBox;	
				YEnd = OldSourceBox.ButtomBox;
					
//					ChangeTextureBox(sourceshift - 4, OldSourceBox.LeftBox, OldSourceBox.TopBox, OldSourceBox.RightBox, OldSourceBox.ButtomBox);
			}
				
//				if(lastFrameFlag == 0)
//				{
//					OldSourceBox.sourceBoxCopy(&tempSourceBox);
//				}
//				else
//				{
//					OldSourceBox.LeftBox = TILE_NUM_X;
//				}
			OldSourceBox.LeftBox = tempSourceBox.LeftBox;
			OldSourceBox.TopBox = tempSourceBox.TopBox;
			OldSourceBox.RightBox = tempSourceBox.RightBox;	
			OldSourceBox.ButtomBox = tempSourceBox.ButtomBox;	
		}
#endif		
#endif

		/* Write background texture parameters to sourcebuffer. */	
		currenttexture->WriteNewSourceBuffer(&sourceshift_tmp,&matrixTemp,currenttexture,NULL);
			
#ifdef CHIP_GC9002				
		if(GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_DOUBLE_BUFFER)
		{
			ChangeTextureBox(sourceshift - 4, SourceBox.LeftBox, SourceBox.TopBox, SourceBox.RightBox, SourceBox.ButtomBox);
		}
#endif
	}

	switch (screenType)
	{
	case RefreshScreenSub:
		matrixTemp.copyFrom(&pCurrentPage->mSubPageMatrix);
		matrixTemp.copyFromInv(&pCurrentPage->mSubPageMatrix);

		pCurrentPage->mSubPageMatrix.NewMatrix_init();
		pCurrentPage->mSubPageMatrix.NewMatrixInv_init();

		pCurrentPage->drawPage(&SourceBox,&sourceshift_tmp,1,ANIMATION_REFRESH_PAGE,0);

		pCurrentPage->mSubPageMatrix.copyFrom(&matrixTemp);
		pCurrentPage->mSubPageMatrix.copyFromInv(&matrixTemp);
		break;
	case RefreshScreenMain:
	case RefreshScreenSingle:
	default:
		matrixTemp.copyFrom(&pCurrentPage->mPageMatrix);
		matrixTemp.copyFromInv(&pCurrentPage->mPageMatrix);

		pCurrentPage->mPageMatrix.NewMatrix_init();
		pCurrentPage->mPageMatrix.NewMatrixInv_init();

		pCurrentPage->drawPage(&SourceBox,&sourceshift_tmp,1,ANIMATION_REFRESH_PAGE,0);

		
		pCurrentPage->mPageMatrix.copyFrom(&matrixTemp);
		pCurrentPage->mPageMatrix.copyFromInv(&matrixTemp);
		break;
	}
	
	


	currentPageScreenAreas.mainOffsetX =  pCurrentPage->screenAreas.mainOffsetX;
	currentPageScreenAreas.mainOffsetY =  pCurrentPage->screenAreas.mainOffsetY;
	currentPageScreenAreas.subOffsetX =  pCurrentPage->screenAreas.subOffsetX;
	currentPageScreenAreas.subOffsetY =  pCurrentPage->screenAreas.subOffsetY;

	switch (screenType)
	{
	case RefreshScreenMain:
		ScreenSourceBox.LeftBox = 0;
		ScreenSourceBox.RightBox = (gScreenOptions.mainWidth +TILESIZE - 1)/TILESIZE;
		ScreenSourceBox.TopBox = 0;
		ScreenSourceBox.ButtomBox = (gScreenOptions.mainHeight +TILESIZE - 1)/TILESIZE;
		ScreenSourceBox.adjustSourceBuffer();
		break;
	case RefreshScreenSub:
		ScreenSourceBox.LeftBox = 0;
		ScreenSourceBox.RightBox = (gScreenOptions.subWidth +TILESIZE - 1)/TILESIZE;
		ScreenSourceBox.TopBox = 0;
		ScreenSourceBox.ButtomBox = (gScreenOptions.subHeight +TILESIZE - 1)/TILESIZE;
		ScreenSourceBox.adjustSourceBuffer();
		break;
	case RefreshScreenSingle:
	default:
		ScreenSourceBox = SourceBox;
		break;
	}

//	taskEXIT_CRITICAL();
	
	#ifdef VERSION_2
	if (sourceshift_tmp<SoureBufferSize-1)
	{
		*(sourcebufferaddr + sourceshift_tmp - (NewSoureBufferSize - 1)) |= (1 << 7);//texsecondheader = \0010 0000
	}
	#endif

	#ifdef PC_SIM
	mAHMIDriver.AHMI_SetFBAdress((uint32_t)TempFrameBuffer);
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED) && (!defined CHIP_GC9002_NoHyper)
	uint32_t old_fbaddr, old_sourceshift = sourceshift;
	uint32_t irq;

	AHMI_WaitDrawDone();

	#if defined(CHIP_GC9002)
	irq = csi_irq_save();
    #elif defined(CHIP_GC9005)
	rt_base_t level;
	level = rt_hw_interrupt_disable();
	#endif
	
	if(!initAHMI)
	{
		AHMI_Config();
	}
	
	AHMI_ITConfig(IT_DRAW_DONE, DISABLE);
	
	old_fbaddr = AHMI_GetFBAdress();
	AHMI_SetFBAdress(*cacheAddr);
	
	sourceshift = sourceshift_tmp;
	AHMI_SetSBAdress((uint32_t)sourcebufferaddr);
	
	csi_hal_DCache_Flush();
	GPE_Cmd(ENABLE);
	
	while( ((AHMI->STR & 0x1) == 1))
    {
    ;};

    while( ((AHMI->STR & 0x1) == 0))
    {
    ;};
	
    #if defined(CHIP_GC9005)
    if(AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC)
    {
        u32 SBForSFBC[6];

        SBForSFBC[0] = 0x800481E0;
        SBForSFBC[1] = 0x00000000;
        SBForSFBC[2] = 0x00000000;
        SBForSFBC[3] = (gScreenWidth << 16) | gScreenHeight;
        extern uint32_t START_ADDR_OFFSET_FROM_GCHEAP;
        SBForSFBC[4] = (*cacheAddr) + (START_ADDR_OFFSET_FROM_GCHEAP);
        SBForSFBC[5] = ((((((gScreenWidth) + TILESIZE - 1) / TILESIZE - 1) & 0xFF) << 16)) | (((((gScreenHeight) + TILESIZE - 1) / TILESIZE - 1) & 0xFF) << 24);

        sourceshift = 24;
        AHMI_SetSBAdress((uint32_t)SBForSFBC);

        csi_hal_DCache_Flush();
        GPE_Cmd(ENABLE);

        while( ((AHMI->STR & 0x1) == 1))
        {
        ;};

        while( ((AHMI->STR & 0x1) == 0))
        {
        ;};
    }
    #endif

    sourceshift = old_sourceshift;
	AHMI_SetFBAdress(old_fbaddr);
	
	extern uint8_t im_up_byGPU_flag;
	im_up_byGPU_flag = 1;
	AHMI_ITConfig(IT_DRAW_DONE, ENABLE);
	
	#if defined(CHIP_GC9002)
	csi_irq_restore(irq);
    #elif defined(CHIP_GC9005)
	rt_hw_interrupt_enable(level);
	#endif
	#endif

#ifdef PC_SIM
	//mAHMIDriver.AHMI_SetSBSize(sourceshift/sizeof(NewSourceBuffer));
	mAHMIDriver.AHMI_SetSBSize(NumberOFLongTexture + NumberOFShortTexture); 
	mAHMIDriver.AHMI_TileBox(ScreenSourceBox.LeftBox, ScreenSourceBox.TopBox, ScreenSourceBox.RightBox, ScreenSourceBox.ButtomBox);
	if(mAHMIDriver.AHMI_GetStatus() == AHMI_DONE) 
	{
		mAHMIDriver.AHMI_Start();

		vTaskDelay(30 / portTICK_RATE_MS);

		//transTempFrameBufferToRGBA8888(((u8*)(*cacheAddr)));

		switch (screenType)
		{
		case RefreshScreenSub:
			transTempFrameBufferToRGBA8888(GlobalPageCacheBufferSub + cachePos * PageCache_Size);
			break;
		case RefreshScreenMain:
		case RefreshScreenSingle:
		default:
			transTempFrameBufferToRGBA8888(GlobalPageCacheBuffer + cachePos * PageCache_Size);
			break;
		}
	}
	
#endif

	//restore Background sourcebuffer
	for(i = 0; i < *psourceshift; i++)
	{
		*(sourcebufferaddr + i) = Bsourcebuffer[i];
	}
	//restore Background sourcebuffer end

	//restore gNewSoureBufferSize
	gNewSoureBufferSize = old_gNewSoureBufferSize;
	//restore gNewSoureBufferSize end

	////clear cache pos
	//switch (screenType)
	//{
	//case RefreshScreenSub:
	//	GlobalPageCacheSubUsed[cachePos] = 0;
	//	break;
	//case RefreshScreenMain:
	//case RefreshScreenSingle:
	//default:
	//	
	//	GlobalPageCacheUsed[cachePos] = 0;
	//	
	//	break;
	//}
	return AHMI_FUNC_SUCCESS;
}
#endif

u32 drawimmediatelyForPlayer(RefreshMsg* refreshMsg, RefreshScreenTypes screenType)
{
	//ElementPtr curPtr;
	TileBoxClass SourceBox;
	TileBoxClass ScreenSourceBox;
	u8 *sourcebufferaddr;
	TextureClassPtr  currenttexture = gPagePtr[WorkingPageID].pBackgroundTexture;
	DynamicPageClassPtr pCurrentPage;
	
#ifdef PC_SIM
	u32 sourceshift = 0;
#endif
	u32 cacheAddr;
	NewMatrix matrixTemp;
	NewMatrix screenMatrix;

#ifdef CHIP_GC9002	
	RefreshMsg partialRefreshMsg;
	uint32_t refreshFlag = 0;
	uint32_t lastFrameFlag = 0;
	
	int16_t startX = 0;
	int16_t startY = 0;
	int16_t stopX = 0;
	int16_t stopY = 0;
#endif



	#if defined(EMBEDDED) && defined(VERSION_2) && !defined(CHIP_GC9002_NoHyper)
	if(lastsbufferaddr == SBAddress)
		return AHMI_FUNC_SUCCESS;
	#endif

	//data check
	if(refreshMsg == NULL)
	{
		ERROR_PRINT("ERROR in drawimmediately: element pointer corruption");
		return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_NULLPointerErr);
	}

	if(refreshMsg->mElementPtr.pageptr == NULL)
	{
		ERROR_PRINT("ERROR in drawimmediately: element pointer null");
		return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_NULLPointerErr);
	}
	if(((refreshMsg->mElementPtr.pageptr - gPagePtr) >= ConfigData.NumofPages) || ((refreshMsg->mElementPtr.pageptr - gPagePtr) < 0))
	{
		ERROR_PRINT("ERROR in drawimmediately: element pointer out of boundary");
		return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_DataOverrun);
	}

    /* Set box for whole page. */
	SourceBox.LeftBox = 0;
	SourceBox.TopBox = 0;
	SourceBox.RightBox = TILE_NUM_X - 1;
	SourceBox.ButtomBox = TILE_NUM_Y - 1;

#ifdef CHIP_GC9002	
	refreshFlag = 0;
	
	if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
	{
		if(refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER)
		{
			partialRefreshMsg.mElementType = ANIMATION_REFRESH_NULL;
		
			//temporary
//			refreshMsg->mElementType = ANIMATION_MATRIXREFRESH_PAGE;
		
		
			GetPartialDrawRefreMsg(&partialRefreshMsg);
		
			if((partialRefreshMsg.mElementType >= ANIMATION_REFRESH_SUBCANVAS) && (partialRefreshMsg.mElementType <= ANIMATION_REFRESH_WIDGET))
			//if(partialRefreshMsg.mElementType == ANIMATION_REFRESH_WIDGET)
			{
		
				GetPartialDrawRefreBox(&SourceBox);
		
				startX = SourceBox.LeftBox * TILESIZE;
				startY = SourceBox.TopBox * TILESIZE;
				stopX = (SourceBox.RightBox + 1) * TILESIZE - 1;
				stopY = (SourceBox.ButtomBox + 1) * TILESIZE - 1;
				
//				if((SourceBox.LeftBox == 0) && (SourceBox.RightBox == 0))
//				{
//					SourceBox.LeftBox = SourceBox.RightBox;
//				}
		
				refreshFlag = 1;
			}
		}
		else if(refreshMsg->mElementType == ANIMAITON_REFRESH_LAST_FRAME)
		{
			//temporary
			refreshMsg->mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
			
			SourceBox.LeftBox = OldSourceBox.LeftBox;
			SourceBox.TopBox = OldSourceBox.TopBox;
			SourceBox.RightBox = OldSourceBox.RightBox;	
			SourceBox.ButtomBox = OldSourceBox.ButtomBox;
			
			refreshFlag = 1;
			lastFrameFlag = 1;
		}
		
		ClearPartialDrawRefreMsg(&partialRefreshMsg);
	}
		
#endif

    /* Adjust box. */
	SourceBox.adjustSourceBuffer();
	
//	if((SourceBox.LeftBox == 0) && (SourceBox.RightBox == 0))
//	{
//		SourceBox.LeftBox = 0;
//		SourceBox.TopBox = 0;
//		SourceBox.RightBox = TILE_NUM_X - 1;
//		SourceBox.ButtomBox = TILE_NUM_Y - 1;
//	}

#ifdef CHIP_GC9002	
	if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
	{
//		XStart = SourceBox.LeftBox * TILESIZE;
//		YStart = SourceBox.TopBox * TILESIZE;
//		XEnd = (SourceBox.RightBox + 1) * TILESIZE - 1;	
//		YEnd = (SourceBox.ButtomBox + 1) * TILESIZE - 1;	
	
		XStart = SourceBox.LeftBox;
		YStart = SourceBox.TopBox;
		XEnd = SourceBox.RightBox;	
		YEnd = SourceBox.ButtomBox;
	}
#endif
	
//	taskENTER_CRITICAL();

#ifdef PC_SIM
	//set the drawing address
	switch (screenType)
	{
	case RefreshScreenSub:
		mAHMIDriver.AHMI_SetFBAdress((uint32_t)GlobalFrameBufferSub);
		break;
	case RefreshScreenMain:
	case RefreshScreenSingle:
	default:
		mAHMIDriver.AHMI_SetFBAdress((uint32_t)GlobalFrameBuffer);
		break;
	}
#endif

#ifdef CHIP_GC9005
	//set the drawing address
	csi_ahmi_SetFBAdress(screenType);
	if(gScreenOptions.enable)
	{
	    switch (screenType)
        {
        case RefreshScreenMain:
            AHMI_TileBox(0, 0, (gScreenOptions.mainWidth + TILESIZE - 1) /TILESIZE - 1, (gScreenOptions.mainHeight + TILESIZE - 1) / TILESIZE - 1);
			AHMI_SetFSize(gScreenOptions.mainWidth, gScreenOptions.mainHeight);
            break;
        case RefreshScreenSub:
            AHMI_TileBox(0, 0, (gScreenOptions.subWidth + TILESIZE - 1) /TILESIZE - 1, (gScreenOptions.subHeight + TILESIZE - 1) / TILESIZE - 1);
			AHMI_SetFSize(gScreenOptions.subWidth, gScreenOptions.subHeight);
            break;
        default:
            AHMI_TileBox(0, 0, (gScreenOptions.mainWidth + TILESIZE - 1) /TILESIZE - 1, (gScreenOptions.mainHeight + TILESIZE - 1) / TILESIZE - 1);
			AHMI_SetFSize(gScreenOptions.mainWidth, gScreenOptions.mainHeight);
            break;
        }
	}
#endif

	#if defined(PC_SIM)
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	#endif
    
	#if defined(EMBEDDED) && defined(VERSION_2)
	sourcebufferaddr = (u8 *)SBAddress;
	lastsbufferaddr = SBAddress;
	#endif
	NumberOFLongTexture = 0;
	NumberOFShortTexture = 0;

	#ifdef PAGE_CACHE_ENABLE	
	if ((gPageAnimationFlag == 0) && (windowPressed == 0))
	{
		//clear cache
		for (int i = 0; i < 2; i++)
		{
			GlobalPageCacheUsed[i] = 0;
			GlobalPageCacheSubUsed[i] = 0;
		}
	}
	#endif

	for (int i = 0; i < ConfigData.NumofPages; i++)
	{
		pCurrentPage = &gPagePtr[i];
		if (!pCurrentPage->shouldShow)
		{
			#ifdef PAGE_CACHE_ENABLE	
			switch (screenType)
			{
			case RefreshScreenSub:
				for (int j = 0; j < 2; j++)
				{
					if (pSubPageCacheAddr[i] != 0 && GlobalPageCacheSubUsed[j] == pSubPageCacheAddr[i])
					{
						//invalid pSubPageCacheAddr[i]
						GlobalPageCacheSubUsed[j] = 0;
						pSubPageCacheAddr[i] = 0;
					}
				}
				break;
			case RefreshScreenMain:
			case RefreshScreenSingle:
			default:
				for (int j = 0; j < 2; j++)
				{
					if (pPageCacheAddr[i] != 0 && GlobalPageCacheUsed[j] == pPageCacheAddr[i])
					{
						//invalid pSubPageCacheAddr[i]
						GlobalPageCacheUsed[j] = 0;
						pPageCacheAddr[i] = 0;
					}
				}
				break;
			}
			#endif
			continue;
		}

		pCurrentPage->screenType = screenType;

		screenMatrix.NewMatrix_init();
		screenMatrix.NewMatrixInv_init();
	
		matrixTemp.NewMatrix_init();
		matrixTemp.NewMatrixInv_init();

		switch (screenType)
		{
		case RefreshScreenMain:
			screenMatrix.TranslateToMatrix((-pCurrentPage->screenAreas.mainOffsetX)<<13, (-pCurrentPage->screenAreas.mainOffsetY)<<13);
			matrixTemp.copyFromInv(&pCurrentPage->mPageMatrix);
			break;
		case RefreshScreenSub:
			screenMatrix.TranslateToMatrix((-pCurrentPage->screenAreas.subOffsetX)<<13, (-pCurrentPage->screenAreas.subOffsetY)<<13);
			matrixTemp.copyFromInv(&pCurrentPage->mSubPageMatrix);
			break;
		default:
			matrixTemp.copyFromInv(&pCurrentPage->mPageMatrix);
			break;
		}
	
		

		
		matrixTemp.LeftMultiMatrix(&screenMatrix);

		//draw background
		currenttexture = pCurrentPage->pBackgroundTexture;

 		if (matrixTemp.G / 8192 > SCREEN_WIDTH || matrixTemp.G/8192 + SCREEN_WIDTH < 0 || matrixTemp.H/8192 > SCREEN_HEIGHT || matrixTemp.H/8192 + SCREEN_HEIGHT < 0)
		{
			//bg out of screen
		}
		else
		{
			
#ifdef CHIP_GC9002				
			if(GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_DOUBLE_BUFFER)
			{
				TileBoxClass 	tempSourceBox;
				
				tempSourceBox.sourceBoxCopy(&SourceBox);
				
				if(refreshFlag == 1)
				{
					TextureClass 	texture;
				
					if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGBA8888)
					{
						texture.mTexAttr = RGBA8888;
					}
					else if((AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB565) || (AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB888) || (AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC))
					{
						texture.mTexAttr = RGB565;
					}
					else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_Y8)
					{
						texture.mTexAttr = Y8;
					}
					texture.mTexAttr |= DRAWING | NONMASKTEX | EFMATRIX | ADDRTEXTURE;
					texture.FocusedSlice = 0;

					texture.RotateAngle = (short)(0);
					texture.ShearAngleX = (short)(0);
					texture.ShearAngleY = (short)(0);
					texture.ShearAngleZX = (short)(0);
					texture.ShearAngleZY = (short)(0);
					texture.ScalerX = (short)(1<<9);
					texture.ScalerY = (short)(1<<9);
					texture.useCanvasPoint = 0;
					texture.SliceFlagsHeaderAddr = 0;
					texture.SingleSliceSize = 0;
			
					texture.TexWidth =  SCREEN_WIDTH;
					texture.TexHeight = SCREEN_HEIGHT;
					texture.OffsetX = 0;
					texture.OffsetY = 0;
					texture.TexLeftTileBox = 0;
					texture.TexTopTileBox = 0;
					texture.TexRightTileBox = SCREEN_WIDTH / TILESIZE;
					texture.TexButtomTileBox = SCREEN_HEIGHT / TILESIZE;
					texture.TexAddr = DISPLAY_GetPriAddress(); // AHMI_GetFBAdress();
				
//					texture.WriteNewSourceBuffer(&sourceshift,&matrixTemp,&texture,NULL);
					if(OldSourceBox.LeftBox >= TILE_NUM_X)
					{
						OldSourceBox.sourceBoxCopy(&SourceBox);
					}
					else
					{
//						if(OldSourceBox.sourceBoxContain(&SourceBox) == AHMI_FUNC_SUCCESS)
//						{
//							refreshMsg->mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
//						}
						
						OldSourceBox.sourceBoxOr(&SourceBox);
						OldSourceBox.adjustSourceBuffer();
						SourceBox.sourceBoxCopy(&OldSourceBox);
					}
					
					XStart = OldSourceBox.LeftBox;
					YStart = OldSourceBox.TopBox;
					XEnd = OldSourceBox.RightBox;	
					YEnd = OldSourceBox.ButtomBox;
					
//					ChangeTextureBox(sourceshift - 4, OldSourceBox.LeftBox, OldSourceBox.TopBox, OldSourceBox.RightBox, OldSourceBox.ButtomBox);
				}
				
//				if(lastFrameFlag == 0)
//				{
//					OldSourceBox.sourceBoxCopy(&tempSourceBox);
//				}
//				else
//				{
//					OldSourceBox.LeftBox = TILE_NUM_X;
//				}
				OldSourceBox.LeftBox = tempSourceBox.LeftBox;
				OldSourceBox.TopBox = tempSourceBox.TopBox;
				OldSourceBox.RightBox = tempSourceBox.RightBox;	
				OldSourceBox.ButtomBox = tempSourceBox.ButtomBox;	
			}
#endif		

			/* Write background texture parameters to sourcebuffer. */	
			currenttexture->WriteNewSourceBuffer(&sourceshift,&matrixTemp,currenttexture,NULL);
			
#ifdef CHIP_GC9002				
			if(GetAHMIParticalDrawType() == AHMI_PARTIAL_DRAW_DOUBLE_BUFFER)
			{
				ChangeTextureBox(sourceshift - 4, SourceBox.LeftBox, SourceBox.TopBox, SourceBox.RightBox, SourceBox.ButtomBox);
			}
#endif
		}

		if (i == WorkingPageID)
		{
			#ifdef PAGE_CACHE_ENABLE	
			/*If it's the first time for drawing a new page. */
			if(refreshMsg->mElementType == ANIMATION_REFRESH_PAGE || refreshMsg->mElementType == ANIMATION_MATRIXREFRESH_PAGE || refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER)
			{
				if (gPageAnimationFlag == 1 || windowPressed == 1)
				{
					switch (screenType)
					{
					case RefreshScreenSub:
						if (pSubPageCacheAddr[i] == 0 && drawPageCacheForPlayer(i, screenType, &sourceshift, &cacheAddr) == AHMI_FUNC_FAILURE)
						{
							//draw cache failed
							pCurrentPage->drawPage(&SourceBox,&sourceshift,1,ANIMATION_MATRIXREFRESH_PAGE,0);
						}
						else
						{
							//draw cache
							if (pSubPageCacheAddr[i] == 0)
							{
								pSubPageCacheAddr[i] = cacheAddr;
							}
							GlobalPageCacheSubUsed[getPageCacheID(pSubPageCacheAddr[i], screenType)] = pSubPageCacheAddr[i];
							AddTextureFromPageCache(&sourceshift, pSubPageCacheAddr[i]);
						}
						break;
					case RefreshScreenMain:
					case RefreshScreenSingle:
					default:
						if (pPageCacheAddr[i] == 0 && drawPageCacheForPlayer(i, screenType, &sourceshift, &cacheAddr) == AHMI_FUNC_FAILURE)
						{
							//draw cache failed
							pCurrentPage->drawPage(&SourceBox,&sourceshift,1,ANIMATION_MATRIXREFRESH_PAGE,0);
						}
						else
						{
							//draw cache
							if (pPageCacheAddr[i] == 0)
							{
								pPageCacheAddr[i] = cacheAddr;
							}
							GlobalPageCacheUsed[getPageCacheID(pPageCacheAddr[i], screenType)] = pPageCacheAddr[i];
							AddTextureFromPageCache(&sourceshift, pPageCacheAddr[i]);
						}
						break;
					}
				}
				else
				{
				    if(refreshMsg->mElementType == ANIMATION_REFRESH_PAGE)
                    {
                        pCurrentPage->drawPage(&SourceBox,&sourceshift,1,ANIMATION_REFRESH_PAGE,0);
                    }
                    else if(refreshMsg->mElementType == ANIMATION_MATRIXREFRESH_PAGE)
                    {
                        pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_MATRIXREFRESH_PAGE,0);
                    }
                    else if(refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER)
                    {
                        pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_REFRESH_PAGE,0);
                    }
				}
			}
			#else
			if(refreshMsg->mElementType == ANIMATION_REFRESH_PAGE)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,1,ANIMATION_REFRESH_PAGE,0);
			}
			else if(refreshMsg->mElementType == ANIMATION_MATRIXREFRESH_PAGE)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_MATRIXREFRESH_PAGE,0);
			}
			else if(refreshMsg->mElementType == ANIMATION_REFRESH_DOUBLE_BUFFER)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_REFRESH_PAGE,0);
			}
            #endif
			else if(refreshMsg->mElementType == ANIMAITON_REFRESH_STATIC_BUFFER)
			{
				pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_REFRESH_PAGE,1);
			}

			currentPageScreenAreas.mainOffsetX =  pCurrentPage->screenAreas.mainOffsetX;
			currentPageScreenAreas.mainOffsetY =  pCurrentPage->screenAreas.mainOffsetY;
			currentPageScreenAreas.subOffsetX =  pCurrentPage->screenAreas.subOffsetX;
			currentPageScreenAreas.subOffsetY =  pCurrentPage->screenAreas.subOffsetY;

			switch (screenType)
			{
			case RefreshScreenMain:
				ScreenSourceBox.LeftBox = 0;
				ScreenSourceBox.RightBox = (gScreenOptions.mainWidth +TILESIZE - 1)/TILESIZE;
				ScreenSourceBox.TopBox = 0;
				ScreenSourceBox.ButtomBox = (gScreenOptions.mainHeight +TILESIZE - 1)/TILESIZE;
				ScreenSourceBox.adjustSourceBuffer();
				break;
			case RefreshScreenSub:
				ScreenSourceBox.LeftBox = 0;
				ScreenSourceBox.RightBox = (gScreenOptions.subWidth +TILESIZE - 1)/TILESIZE;
				ScreenSourceBox.TopBox = 0;
				ScreenSourceBox.ButtomBox = (gScreenOptions.subHeight +TILESIZE - 1)/TILESIZE;
				ScreenSourceBox.adjustSourceBuffer();
				break;
			case RefreshScreenSingle:
			default:
				ScreenSourceBox = SourceBox;
				break;
			}
		}
		else
		{
			#ifdef PAGE_CACHE_ENABLE	
			switch (screenType)
			{
			case RefreshScreenSub:
				if (pSubPageCacheAddr[i] == 0 && drawPageCacheForPlayer(i, screenType, &sourceshift, &cacheAddr) == AHMI_FUNC_FAILURE)
				{
					//draw cache failed
					pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_MATRIXREFRESH_PAGE,0);
				}
				else
				{
					//draw cache
					if (pSubPageCacheAddr[i] == 0)
					{
						pSubPageCacheAddr[i] = cacheAddr;
					}
					GlobalPageCacheSubUsed[getPageCacheID(pSubPageCacheAddr[i], screenType)] = pSubPageCacheAddr[i];
					AddTextureFromPageCache(&sourceshift, pSubPageCacheAddr[i]);
				}
				break;
			case RefreshScreenMain:
			case RefreshScreenSingle:
			default:
				if (pPageCacheAddr[i] == 0 && drawPageCacheForPlayer(i, screenType, &sourceshift, &cacheAddr) == AHMI_FUNC_FAILURE)
				{
					//draw cache failed
					pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_MATRIXREFRESH_PAGE,0);
				}
				else
				{
					//draw cache
					if (pPageCacheAddr[i] == 0)
					{
						pPageCacheAddr[i] = cacheAddr;
					}
					GlobalPageCacheUsed[getPageCacheID(pPageCacheAddr[i], screenType)] = pPageCacheAddr[i];
					AddTextureFromPageCache(&sourceshift, pPageCacheAddr[i]);
				}
				break;
			}
			#else
			pCurrentPage->drawPage(&SourceBox,&sourceshift,0,ANIMATION_MATRIXREFRESH_PAGE,0);
			#endif
		}
	}

//	taskEXIT_CRITICAL();
	
	#ifdef VERSION_2
	if (sourceshift<SoureBufferSize-1)
	{
		*(sourcebufferaddr + sourceshift - (NewSoureBufferSize - 1)) |= (1 << 7);//texsecondheader = \0010 0000
	}
	#endif

#ifdef PC_SIM
	//mAHMIDriver.AHMI_SetSBSize(sourceshift/sizeof(NewSourceBuffer));
	mAHMIDriver.AHMI_SetSBSize(NumberOFLongTexture + NumberOFShortTexture); 
	mAHMIDriver.AHMI_TileBox(ScreenSourceBox.LeftBox, ScreenSourceBox.TopBox, ScreenSourceBox.RightBox, ScreenSourceBox.ButtomBox);
	if(mAHMIDriver.AHMI_GetStatus() == AHMI_DONE) 
	{
		mAHMIDriver.AHMI_Start();
	}
	else
	{
		return AHMI_FUNC_SUCCESS; 
	}
#endif
	return AHMI_FUNC_SUCCESS;
}

#if (defined WHOLE_TRIBLE_BUFFER) || (defined PARTIAL_TRIBLE_BUFFER)
#if 0
//-----------------------------
// function: screenUpdateTask
// @brief: send refresh message, only for PC_SIM.
// parameters:
//    @param1 pvParameters  pointer to parameters.
//-----------------------------
void screenUpdateTask(void* pvParameters)
{
	RefreshMsg refreshMsg;
	for(;;)
	{
#ifdef WHOLE_TRIBLE_BUFFER
		//refreshMsg.mElementType = ANIMATION_REFRESH_DOUBLE_BUFFER;
		//refreshMsg.mElementPtr.pageptr = &(gPagePtr[WorkingPageID]);
		if(csi_kernel_msgq_get_count(RefreshQueue) < 10)
			csi_kernel_msgq_put(RefreshQueue,&refreshMsg,0,0);
		
		if(screenUpdateCount < 10 && staticRefresh == 1)
		{
			screenUpdateCount ++;
		}
		else if(screenUpdateCount >= 10 && staticRefresh == 1)
		{
			screenUpdateCount = 0;
			staticRefresh = 0;
		}
#endif	

		vTaskDelay( SCREEN_UPDATE_TIME / portTICK_RATE_MS );
	}
}
#endif
#endif
#endif

#ifdef EMBEDDED	
extern QueueHandle_t PlayerInstructionQueue;  

#ifdef __cplusplus
extern "C" {
#endif
funcStatus setDrawControlTrigger(uint32_t irqFlag)
{
    PLAR_tstAHMICmd stAHMICmd;
    static BaseType_t xHigherPriorityTaskWoken;
    
	if(gScreenOptions.enable)
		return AHMI_FUNC_SUCCESS;

    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
    stAHMICmd.u32CmdType = enCmdType_DrawControl;
    stAHMICmd.u32PageID = WorkingPageID;
    
    if(irqFlag != 0)
    {
        if(csi_kernel_msgq_put( PlayerInstructionQueue, &stAHMICmd, 0, 0) != 0)
        {
			return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_QueueSendErr);
        } 
    }
    else
    {
        if(csi_kernel_msgq_put(PlayerInstructionQueue,&stAHMICmd,0,portMAX_DELAY) != 0)
        {
			return (EM_enErrRetModularRefreshQueue | EM_ErrRetRefreshQueue_QueueSendErr);
        }
    }
    
    return AHMI_FUNC_SUCCESS;
}
#ifdef __cplusplus
}
#endif
/****************************************************Add by Zhu Jiahai****************************************************************************/

/****************************************************End of Add***********************************************************************************/
#endif
