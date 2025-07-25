////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
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
#include "AHMIBasicDefine.h"
#include "publicDefine.h"

#ifdef AHMI_DEBUG
#include "trace.h"
#include "stdio.h"
#endif
#if (defined VERSION_2) && (defined EMBEDDED) 
#include "csi_ahmi_fix.h"
#include "math.h"
#endif

#ifdef AHMI_CORE
#define CanvasSwipeSpeedMax       		(0xA0)

extern ConfigInfoClass ConfigData;
extern DynamicPageClassPtr  gPagePtr;
extern u16           WorkingPageID;
//extern QueueHandle_t   RefreshQueue;
extern QueueHandle_t		ActionInstructionQueue;
extern AnimationClass  gAnimationClass;
extern Easing			gEasing;
//extern u8 gPageNeedRefresh;
extern u8 windowPressed;
extern u8 gPageAnimationFlag;

extern ProjectScreenOptions gScreenOptions;

#if defined(EMBEDDED) && defined(VERSION_2)
extern u8 normalvideoframe;
//extern void EnterSafeSectionForTag(void);
//extern void ExitSafeSectionForTag(void);
#endif

#ifdef PC_SIM
extern"C"
{
extern struct globalArgs_t globalArgs;
}
#endif



s32 pow2(s32 n)
{
	return n * n;
}

#if defined(EMBEDDED) && defined(VERSION_2)
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif



#endif


s16 calWidgetScrollScaleOG(WidgetClassPtr wptr, SubCanvasClassPtr scptr, CanvasClassPtr cptr, u8 padding)
{
	s16 widgetX = wptr->WidgetOffsetX;
	s16 widgetY = wptr->WidgetOffsetY;
	u16 widgetWidth = wptr->WidgetWidth;
	u16 widgetHeight = wptr->WidgetHeight;

	s16 widgetCenterXInCanvas = scptr->mOffsetX + scptr->contentOffsetX + widgetX + widgetWidth/2;
	s16 widgetCenterYInCanvas = scptr->mOffsetY + scptr->contentOffsetY + widgetY + widgetHeight/2;
	
	#ifdef PC_SIM
	s16 minOffsetL = max(0, widgetCenterXInCanvas - 0) * 100 / cptr->mwidth;
	s16 minOffsetR = max(0, cptr->mwidth - widgetCenterXInCanvas) * 100 / cptr->mwidth;
	s16 minOffsetT = max(0, widgetCenterYInCanvas - 0) * 100 / cptr->mheight;
	s16 minOffsetB = max(0, cptr->mheight - widgetCenterYInCanvas) * 100 / cptr->mheight;

	s16 minOffset = min(minOffsetL, minOffsetR);
	minOffset = min(minOffset, minOffsetT);
	minOffset = min(minOffset, minOffsetB);
	#endif
	
	#if defined(EMBEDDED) && defined(VERSION_2)
	s16 minOffsetL = 0;
	if(widgetCenterXInCanvas > 0)
		minOffsetL = widgetCenterXInCanvas * 100 / cptr->mwidth;
	s16 minOffsetR = 0;
	if(cptr->mwidth - widgetCenterXInCanvas > 0)
		minOffsetR = (cptr->mwidth - widgetCenterXInCanvas) * 100 / cptr->mwidth;
	s16 minOffsetT = 0;
	if(widgetCenterYInCanvas > 0)
		minOffsetT = widgetCenterYInCanvas * 100 / cptr->mheight;
	s16 minOffsetB = 0;
	if(cptr->mheight - widgetCenterYInCanvas > 0)
		minOffsetB = (cptr->mheight - widgetCenterYInCanvas) * 100 / cptr->mheight;

	s16 minOffset = minOffsetL;
	if(minOffsetR < minOffset)
		minOffset = minOffsetR;
	if(minOffsetT < minOffset)
		minOffset = minOffsetT;
	if(minOffsetB < minOffset)
		minOffset = minOffsetB;
	#endif
	
	if (minOffset < padding)
	{
		return (minOffset*461) / padding + 51;//(minOffset*512) / padding * ( 1 - 0.1) + 0.1 * 512;
	}
	else
	{
		return 1 << 9;
	}
	
}


s16 calWidgetScrollScale(WidgetClass* wptr, SubCanvasClass* scptr, CanvasClass* cptr, u8 padding, u8 borderRadiusRatio) {
    s16 widgetX = wptr->WidgetOffsetX;
    s16 widgetY = wptr->WidgetOffsetY;
    u16 widgetWidth = wptr->WidgetWidth;
    u16 widgetHeight = wptr->WidgetHeight;

    s16 widgetCenterXInCanvas = scptr->mOffsetX + scptr->contentOffsetX + widgetX + widgetWidth / 2;
    s16 widgetCenterYInCanvas = scptr->mOffsetY + scptr->contentOffsetY + widgetY + widgetHeight / 2;

    u16 canvasWidth = cptr->mwidth;
    u16 canvasHeight = cptr->mheight;

    // Clamp borderRadius to be at most half of the smaller dimension of the canvas
    u16 maxBorderRadius = min(canvasWidth, canvasHeight) / 2;
	s16 borderRadius;
    borderRadius = min(borderRadiusRatio * canvasWidth / 100, maxBorderRadius);
	borderRadius = min(borderRadiusRatio * canvasHeight / 100, borderRadius);

    // Calculate distances from the widget center to the rectangle edges
    s16 left = widgetCenterXInCanvas;
    s16 top = widgetCenterYInCanvas;

    // Determine the distance to the rounded rectangle edge
    s16 distance;
    s32 distanceToBorderCenter;

    if (left < borderRadius && top < borderRadius) {
        // Top-left corner
        distanceToBorderCenter = sqrt(pow2(left - borderRadius) + pow2(top - borderRadius));
        distance = max(0, (borderRadius - distanceToBorderCenter));
    } else if (canvasWidth - left < borderRadius && top < borderRadius) {
        // Top-right corner
        distanceToBorderCenter = sqrt(pow2(left - (canvasWidth - borderRadius)) + pow2(top - borderRadius));
        distance = max(0, (borderRadius - distanceToBorderCenter));
    } else if (left < borderRadius && canvasHeight - top < borderRadius) {
        // Bottom-left corner
        distanceToBorderCenter = sqrt(pow2(left - borderRadius) + pow2(top - (canvasHeight - borderRadius)));
        distance = max(0, (borderRadius - distanceToBorderCenter));
    } else if (canvasWidth - left < borderRadius && canvasHeight - top < borderRadius) {
        // Bottom-right corner
        distanceToBorderCenter = sqrt(pow2(left - (canvasWidth - borderRadius)) + pow2(top - (canvasHeight - borderRadius)));
        distance = max(0, (borderRadius - distanceToBorderCenter));
    } else {
        // Not in the corner region
		distance = min(max(0, left), max(0,((s16)canvasWidth - left)));
		distance = min(distance, max(0, top));
		distance = min(distance, max(0, ((s16)canvasHeight - top)));
    }

    // Calculate the scale based on the distance and padding
    s16 minLen = padding * min(canvasWidth, canvasHeight) / 100.0 ;
	s32 minScale = 0.1 * 512;
    if (distance < minLen) {
       return (distance * (512 - minScale) / minLen  + minScale);
    } else {
        return 512; // 1 << 9 (as 512)
    }
}

#ifdef CHIP_GC9002	
funcStatus  getWidgetSoureBoxOverlap(WidgetClassPtr pWidget, TileBoxClassPtr tileBox)
{
	CanvasClassPtr pCanvas;
	SubCanvasClassPtr pSubCanvas;
	TileBoxClass SourceBox;
	int16_t startX = 0;
	int16_t startY = 0;
	int16_t stopX = 0;
	int16_t stopY = 0;
	
	if((pWidget == NULL) || (tileBox == NULL))
		return AHMI_FUNC_FAILURE;
	
	
	pCanvas = &(gPagePtr[WorkingPageID].pCanvasList[pWidget->ATTATCH_CANVAS]);
		
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
		
	SourceBox.LeftBox = startX / TILESIZE;
	SourceBox.TopBox = startY / TILESIZE;
	SourceBox.RightBox = (stopX + TILESIZE - 1) / TILESIZE - 1;
	SourceBox.ButtomBox = (stopY + TILESIZE - 1) / TILESIZE - 1;
	
	return tileBox->sourceBoxOverlap(&SourceBox);
}

funcStatus setWidgetSourebufferRefreshFlag(WidgetClassPtr pWidget)
{
	
	
	if(pWidget == NULL)
		return AHMI_FUNC_FAILURE;
	
	pWidget->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}

#include "csi_ahmi.h"
#endif

#ifdef CHIP_GC9002	

funcStatus  setCanvasSourebufferRefreshFlag(DynamicPageClass* pageptr, CanvasClassPtr cptr)
{
	SubCanvasClassPtr scptr;
	int NumofWidget;
	
	if(cptr == NULL)
		return AHMI_FUNC_FAILURE;
	
	scptr = (SubCanvasClassPtr)(pageptr->pSubCanvasList) + cptr->mStartAddrOfSubCanvas + cptr->mFocusedCanvas;
	
	NumofWidget = scptr->NumOfWidget;
	
	//Write all controls in a canvas.
	for (int WidgetCount = 0; WidgetCount < NumofWidget; WidgetCount++)
	{
		WidgetClassPtr wptr = &(pageptr->pWidgetList[ (scptr->StartAddrOfWidget + WidgetCount) ]);
		//Determine whether widget is inside canvas.
		if( 
			(s16)(wptr->WidgetOffsetX) > (s16)(scptr->contentWidth) ||//大于右边界 
			( (s16)(wptr->WidgetOffsetX) + (wptr->WidgetWidth) ) < 0 || //小于左边界
			(s16)(wptr->WidgetOffsetY) > (s16)(scptr->contentHeight)  || //大于下边界
			( (s16)(wptr->WidgetOffsetY) + (wptr->WidgetHeight) ) < 0
            )
			continue;//If it is not inside, it will not be drawn

		setWidgetSourebufferRefreshFlag(wptr);
	}
	
	return AHMI_FUNC_SUCCESS;
}

#endif

ROIClass getFinalROI(ROIClass rect1, ROIClass rect2)
{
	ROIClass trueROI;

	#ifdef PC_SIM
	trueROI.lt.mPointX = max(rect1.lt.mPointX, rect2.lt.mPointX);
	trueROI.lt.mPointY = max(rect1.lt.mPointY, rect2.lt.mPointY);

	trueROI.rb.mPointX = min(rect1.rb.mPointX, rect2.rb.mPointX);
	trueROI.rb.mPointY = min(rect1.rb.mPointY, rect2.rb.mPointY);
	#endif

	#if defined(EMBEDDED) && defined(VERSION_2)
	trueROI.lt.mPointX = (((rect1.lt.mPointX) > (rect2.lt.mPointX)) ? (rect1.lt.mPointX) : (rect2.lt.mPointX));//max(rect1.lt.mPointX, rect2.lt.mPointX);
	trueROI.lt.mPointY = (((rect1.lt.mPointY) > (rect2.lt.mPointY)) ? (rect1.lt.mPointY) : (rect2.lt.mPointY));//max(rect1.lt.mPointY, rect2.lt.mPointY);

	trueROI.rb.mPointX = (((rect1.rb.mPointX) < (rect2.rb.mPointX)) ? (rect1.rb.mPointX) : (rect2.rb.mPointX));//min(rect1.rb.mPointX, rect2.rb.mPointX);
	trueROI.rb.mPointY = (((rect1.rb.mPointY) < (rect2.rb.mPointY)) ? (rect1.rb.mPointY) : (rect2.rb.mPointY));//min(rect1.rb.mPointY, rect2.rb.mPointY);
	#endif

	trueROI.lb.mPointX = trueROI.lt.mPointX;
	trueROI.lb.mPointY = trueROI.rb.mPointY;

	trueROI.rt.mPointX = trueROI.rb.mPointX;
	trueROI.rt.mPointY = trueROI.lt.mPointY;

	return trueROI;
}

//-----------------------------
// name:drawCanvas
// draw canvas function
//
// @param1 TileBoxClassPtr tileBox : sourcebox class
// @param2 u32 *sourceShift : sourcebuffer pointer
// @param3 u8  pageEnable : whether refresh total page
// @param4 matrixClassPtr pPageMatrix : Additional transformation matrix pointer
// @param5 SubCanvasClassPtr scptr : subcanvas pointer
// @param6 u8 RefreshType : the type of refresh
// @param7 u8 staticTextureEn : Whether to draw to static storage space
// additional comments
//   none
//-----------------------------
u32 CanvasClass::drawCanvas(
		TileBoxClassPtr pTileBox  , //source box
		u32 *u32p_sourceShift     , //sourcebuffer pointer
		u8 pageEnable             , //whether refresh the entire page
		DynamicPageClassPtr pageptr, //Additional transformation matrix pointer
		SubCanvasClassPtr scptr   , //subcanvas pointer
		u8 RefreshType            , //The source box of canvas is used to ignore the bounding box of the control.
		u8 staticTextureEn          //Whether to draw to static storage space
		)
{
	TileBoxClass drawingCanvasTileBox;
//	ElementPtr   curPtr;
	funcStatus stutas = AHMI_FUNC_SUCCESS;
	NewMatrix curCanvasMatrix;
	NewMatrix subcanvasMatrixForSwipe;
//	NewMatrix widgetScrollScaleMatrix;
	NewMatrix subcanvasMatrix;
	ROIClass canvasROI, subcanvasROI;
	NewMatrix screenMatrix;
	NewMatrixPtr pPageMatrix;// = &(pageptr->mPageMatrix);
	NewMatrix screenAndPageMatrix;
	int lm, rm, tm, bm;
	s16 widgetScrollScale;

	screenMatrix.NewMatrix_init();
	screenMatrix.NewMatrixInv_init();
	
	switch (pageptr->screenType)
	{
	case RefreshScreenMain:
		pPageMatrix = &(pageptr->mPageMatrix);
		screenMatrix.TranslateToMatrix((-pageptr->screenAreas.mainOffsetX)<<13, (-pageptr->screenAreas.mainOffsetY)<<13);
		
		break;
	case RefreshScreenSub:
		pPageMatrix = &(pageptr->mSubPageMatrix);
		screenMatrix.TranslateToMatrix((-pageptr->screenAreas.subOffsetX)<<13, (-pageptr->screenAreas.subOffsetY)<<13);
		
		break;
	default:
		pPageMatrix = &(pageptr->mPageMatrix);
		break;
	}

	screenAndPageMatrix.copyFrom(pPageMatrix);
	screenAndPageMatrix.copyFromInv(pPageMatrix);
	screenAndPageMatrix.LeftMultiMatrix(&screenMatrix);

	if( NULL == pTileBox || NULL == u32p_sourceShift || NULL == pPageMatrix || NULL == scptr ){
		ERROR_PRINT("ERROR: NULL　pointer occcured! ");
		return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_NULLPointerErr);
	}
	int NumofWidget = scptr->NumOfWidget;

	//computing canvas matrix
	// M_curCanvas = Mp*Mpt*Mc
	curCanvasMatrix.NewMatrix_init();
	curCanvasMatrix.NewMatrixInv_init();

	if((this->mRefreshType & CanvasMatrixRefreshFlag) || (pageEnable) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE))
	{
		CalculateMatrix(&this->mCanvasMatrix);

		this->mRefreshType &= (~CanvasMatrixRefreshFlag);
	}

	
	
	

	curCanvasMatrix.LeftMultiMatrix(&this->mCanvasMatrix);
	curCanvasMatrix.LeftMultiMatrix(pPageMatrix);

	curCanvasMatrix.LeftMultiMatrix(&screenMatrix);
	
	//computing subcanvas matrix
	//Msc = Mc * Msc * M_curCanvas
	subcanvasMatrix.NewMatrix_init();
	subcanvasMatrix.NewMatrixInv_init();
	subcanvasMatrix.LeftMultiMatrix(&scptr->mSubCanvasMatrix);
	subcanvasMatrix.LeftMultiMatrix(&curCanvasMatrix);

	subcanvasMatrixForSwipe.NewMatrix_init();
	subcanvasMatrixForSwipe.NewMatrixInv_init();

	//for subcanvas animation
	if(((scptr->contentWidth != this->mwidth) || (scptr->contentHeight != this->mheight)) && (!pageEnable) && (this->CanvasState == 1))
	{
		subcanvasMatrixForSwipe.translate(-((scptr->mWidth / 2) << 13), -((scptr->mHeight / 2) << 13));
		subcanvasMatrixForSwipe.scale(scptr->mZoomX,scptr->mZoomY);
		subcanvasMatrixForSwipe.translate(((scptr->mWidth / 2) << 13), ((scptr->mHeight / 2) << 13));
		subcanvasMatrixForSwipe.translate((scptr->mOffsetX << 13), (scptr->mOffsetY << 13));
		subcanvasMatrixForSwipe.LeftMultiMatrix(&curCanvasMatrix);
	}

//	curPtr.scptr = scptr;

	if((((s16)(this->mZoomX) < 0) && ((s16)(this->mZoomY) > 0)) || (((s16)(this->mZoomX) > 0) && ((s16)(this->mZoomY) < 0)))
	{
		//init roi
		canvasROI.lt.mPointX = 0 << 4;
		canvasROI.lt.mPointY = 0 << 4;
		canvasROI.rt.mPointX = 0 << 4;
		canvasROI.rt.mPointY = (mheight) << 4;
		canvasROI.lb.mPointX = (mwidth) << 4;
		canvasROI.lb.mPointY = 0 << 4;
		canvasROI.rb.mPointX = (mwidth) << 4;
		canvasROI.rb.mPointY = (mheight) << 4;
		
		subcanvasROI.lt.mPointX = 0 << 4;
		subcanvasROI.lt.mPointY = 0 << 4;
		subcanvasROI.rt.mPointX = 0 << 4;
		subcanvasROI.rt.mPointY = (mheight) << 4;
		subcanvasROI.lb.mPointX = (mwidth) << 4;
		subcanvasROI.lb.mPointY = 0 << 4;
		subcanvasROI.rb.mPointX = (mwidth) << 4;
		subcanvasROI.rb.mPointY = (mheight) << 4;
	}
	else
	{
		//init roi
		canvasROI.lt.mPointX = 0 << 4;
		canvasROI.lt.mPointY = 0 << 4;
		canvasROI.rt.mPointX = (mwidth) << 4;
		canvasROI.rt.mPointY = 0 << 4;
		canvasROI.lb.mPointX = 0 << 4;
		canvasROI.lb.mPointY = (mheight) << 4;
		canvasROI.rb.mPointX = (mwidth) << 4;
		canvasROI.rb.mPointY = (mheight) << 4;

		subcanvasROI.lt.mPointX = 0 << 4;
		subcanvasROI.lt.mPointY = 0 << 4;
		subcanvasROI.rt.mPointX = (mwidth) << 4;
		subcanvasROI.rt.mPointY = 0 << 4;
		subcanvasROI.lb.mPointX = 0 << 4;
		subcanvasROI.lb.mPointY = (mheight) << 4;
		subcanvasROI.rb.mPointX = (mwidth) << 4;
		subcanvasROI.rb.mPointY = (mheight) << 4;
	}

	if(this->pressFlag != 0 || scptr->contentWidth != this->mwidth || scptr->contentHeight != this->mheight || ((this->animationType != SUBCANVAS_ANIMATION_ZOOM) && (this->animationType != 0)))
	{
		CalculateROIPoint(&curCanvasMatrix, &canvasROI.lt, &canvasROI.rt, &canvasROI.rb, &canvasROI.lb);

		if((this->CanvasState == 1) && (this->animationType != 0) && (scptr->contentWidth != this->mwidth || scptr->contentHeight != this->mheight))
		{
			CalculateROIPoint(&subcanvasMatrixForSwipe, &subcanvasROI.lt, &subcanvasROI.rt, &subcanvasROI.rb, &subcanvasROI.lb);

			if(this->animationType == SUBCANVAS_ANIMATION_SHIFTING_L)
			{
				canvasROI.rt.mPointX = subcanvasROI.rt.mPointX;
				canvasROI.rt.mPointY = subcanvasROI.rt.mPointY;

				canvasROI.rb.mPointX = subcanvasROI.rb.mPointX;
				canvasROI.rb.mPointY = subcanvasROI.rb.mPointY;
			}
			else if(this->animationType == SUBCANVAS_ANIMATION_SHIFTING_R)
			{
				canvasROI.lt.mPointX = subcanvasROI.lt.mPointX;
				canvasROI.lt.mPointY = subcanvasROI.lt.mPointY;

				canvasROI.lb.mPointX = subcanvasROI.lb.mPointX;
				canvasROI.lb.mPointY = subcanvasROI.lb.mPointY;
			}
			else if(this->animationType == SUBCANVAS_ANIMATION_SHIFTING_UP)
			{
				canvasROI.lb.mPointX = subcanvasROI.lb.mPointX;
				canvasROI.lb.mPointY = subcanvasROI.lb.mPointY;

				canvasROI.rb.mPointX = subcanvasROI.rb.mPointX;
				canvasROI.rb.mPointY = subcanvasROI.rb.mPointY;
			}
			else if(this->animationType == SUBCANVAS_ANIMATION_SHIFTING_DOWN)
			{
				canvasROI.lt.mPointX = subcanvasROI.lt.mPointX;
				canvasROI.lt.mPointY = subcanvasROI.lt.mPointY;

				canvasROI.rt.mPointX = subcanvasROI.rt.mPointX;
				canvasROI.rt.mPointY = subcanvasROI.rt.mPointY;
			}
		}
	}
	else
	{
		if(windowPressed || gPageAnimationFlag)
		{
			switch (pageptr->screenType)
			{
			case RefreshScreenMain:
				canvasROI.lt.mPointX = pageptr->screenAreas.mainOffsetX << 4;
				canvasROI.lt.mPointY = pageptr->screenAreas.mainOffsetY << 4;
				canvasROI.rt.mPointX = (pageptr->screenAreas.mainOffsetX + gScreenOptions.mainWidth) << 4;
				canvasROI.rt.mPointY = pageptr->screenAreas.mainOffsetY << 4;
				canvasROI.lb.mPointX =  pageptr->screenAreas.mainOffsetX << 4;
				canvasROI.lb.mPointY = (pageptr->screenAreas.mainOffsetY + gScreenOptions.mainHeight) << 4;
				canvasROI.rb.mPointX = (pageptr->screenAreas.mainOffsetX + gScreenOptions.mainWidth) << 4;
				canvasROI.rb.mPointY = (pageptr->screenAreas.mainOffsetY + gScreenOptions.mainHeight) << 4;
		
				break;
			case RefreshScreenSub:
				canvasROI.lt.mPointX = pageptr->screenAreas.subOffsetX << 4;
				canvasROI.lt.mPointY = pageptr->screenAreas.subOffsetY << 4;
				canvasROI.rt.mPointX = (pageptr->screenAreas.subOffsetX + gScreenOptions.subWidth) << 4;
				canvasROI.rt.mPointY = pageptr->screenAreas.subOffsetY << 4;
				canvasROI.lb.mPointX =  pageptr->screenAreas.subOffsetX << 4;
				canvasROI.lb.mPointY = (pageptr->screenAreas.subOffsetY + gScreenOptions.subHeight) << 4;
				canvasROI.rb.mPointX = (pageptr->screenAreas.subOffsetX + gScreenOptions.subWidth) << 4;
				canvasROI.rb.mPointY = (pageptr->screenAreas.subOffsetY + gScreenOptions.subHeight) << 4;
		
				break;
			default:
				canvasROI.lt.mPointX = 0 << 4;
				canvasROI.lt.mPointY = 0 << 4;
				canvasROI.rt.mPointX = (SCREEN_WIDTH) << 4;
				canvasROI.rt.mPointY = 0 << 4;
				canvasROI.lb.mPointX = 0 << 4;
				canvasROI.lb.mPointY = (SCREEN_HEIGHT) << 4;
				canvasROI.rb.mPointX = (SCREEN_WIDTH) << 4;
				canvasROI.rb.mPointY = (SCREEN_HEIGHT) << 4;
				break;
			}
			
			ROIClass TrueCanvasROI;
			TrueCanvasROI.lt.mPointX = 0 << 4;
			TrueCanvasROI.lt.mPointY = 0 << 4;
			TrueCanvasROI.rt.mPointX = (mwidth) << 4;
			TrueCanvasROI.rt.mPointY = 0 << 4;
			TrueCanvasROI.lb.mPointX = 0 << 4;
			TrueCanvasROI.lb.mPointY = (mheight) << 4;
			TrueCanvasROI.rb.mPointX = (mwidth) << 4;
			TrueCanvasROI.rb.mPointY = (mheight) << 4;
			CalculateROIPoint(&curCanvasMatrix, &TrueCanvasROI.lt, &TrueCanvasROI.rt, &TrueCanvasROI.rb, &TrueCanvasROI.lb);

			CalculateROIPoint(&screenAndPageMatrix, &canvasROI.lt, &canvasROI.rt, &canvasROI.rb, &canvasROI.lb);

			#ifdef PC_SIM
			lm = min(TrueCanvasROI.lt.mPointX, TrueCanvasROI.rt.mPointX);
			lm = min(lm, TrueCanvasROI.rb.mPointX);
			lm = min(lm, TrueCanvasROI.lb.mPointX);

			rm = max(TrueCanvasROI.lt.mPointX, TrueCanvasROI.rt.mPointX);
			rm = max(rm, TrueCanvasROI.rb.mPointX);
			rm = max(rm, TrueCanvasROI.lb.mPointX);

			tm = min(TrueCanvasROI.lt.mPointY, TrueCanvasROI.rt.mPointY);
			tm = min(tm, TrueCanvasROI.rb.mPointY);
			tm = min(tm, TrueCanvasROI.lb.mPointY);

			bm = max(TrueCanvasROI.lt.mPointY, TrueCanvasROI.rt.mPointY);
			bm = max(bm, TrueCanvasROI.rb.mPointY);
			bm = max(bm, TrueCanvasROI.lb.mPointY);
			#endif
		
			#if defined(EMBEDDED) && defined(VERSION_2)
			lm = (((TrueCanvasROI.lt.mPointX) < (TrueCanvasROI.rt.mPointX)) ? (TrueCanvasROI.lt.mPointX) : (TrueCanvasROI.rt.mPointX));//min(TrueCanvasROI.lt.mPointX, TrueCanvasROI.rt.mPointX);
			lm = (((lm) < (TrueCanvasROI.rb.mPointX)) ? (lm) : (TrueCanvasROI.rb.mPointX));//min(lm, TrueCanvasROI.rb.mPointX);
			lm = (((lm) < (TrueCanvasROI.lb.mPointX)) ? (lm) : (TrueCanvasROI.lb.mPointX));//min(lm, TrueCanvasROI.lb.mPointX);

			rm = (((TrueCanvasROI.lt.mPointX) > (TrueCanvasROI.rt.mPointX)) ? (TrueCanvasROI.lt.mPointX) : (TrueCanvasROI.rt.mPointX));//max(TrueCanvasROI.lt.mPointX, TrueCanvasROI.rt.mPointX);
			rm = (((rm) > (TrueCanvasROI.rb.mPointX)) ? (rm) : (TrueCanvasROI.rb.mPointX));//max(rm, TrueCanvasROI.rb.mPointX);
			rm = (((rm) > (TrueCanvasROI.lb.mPointX)) ? (rm) : (TrueCanvasROI.lb.mPointX));//max(rm, TrueCanvasROI.lb.mPointX);

			tm = (((TrueCanvasROI.lt.mPointY) < (TrueCanvasROI.rt.mPointY)) ? (TrueCanvasROI.lt.mPointY) : (TrueCanvasROI.rt.mPointY));//min(TrueCanvasROI.lt.mPointY, TrueCanvasROI.rt.mPointY);
			tm = (((tm) < (TrueCanvasROI.rb.mPointY)) ? (tm) : (TrueCanvasROI.rb.mPointY));//min(tm, TrueCanvasROI.rb.mPointY);
			tm = (((tm) < (TrueCanvasROI.lb.mPointY)) ? (tm) : (TrueCanvasROI.lb.mPointY));//min(tm, TrueCanvasROI.lb.mPointY);

			bm = (((TrueCanvasROI.lt.mPointY) > (TrueCanvasROI.rt.mPointY)) ? (TrueCanvasROI.lt.mPointY) : (TrueCanvasROI.rt.mPointY));//max(TrueCanvasROI.lt.mPointY, TrueCanvasROI.rt.mPointY);
			bm = (((bm) > (TrueCanvasROI.rb.mPointY)) ? (bm) : (TrueCanvasROI.rb.mPointY));//max(bm, TrueCanvasROI.rb.mPointY);
			bm = (((bm) > (TrueCanvasROI.lb.mPointY)) ? (bm) : (TrueCanvasROI.lb.mPointY));//max(bm, TrueCanvasROI.lb.mPointY);
			#endif
		
			if ((rm <= canvasROI.lt.mPointX || lm >= canvasROI.rt.mPointX || bm <= canvasROI.lt.mPointY || tm >= canvasROI.rb.mPointY))
			{
				//out of screen
				//no need to draw
				return AHMI_FUNC_SUCCESS;
			}
			else
			{
				canvasROI = getFinalROI(TrueCanvasROI, canvasROI);
			}
		}
		else
		{
			CalculateROIPoint(&subcanvasMatrix, &canvasROI.lt, &canvasROI.rt, &canvasROI.rb, &canvasROI.lb);
		}
	}

	if (true)
	{
		#ifdef PC_SIM
		lm = min(canvasROI.lt.mPointX, canvasROI.rt.mPointX);
		lm = min(lm, canvasROI.rb.mPointX);
		lm = min(lm, canvasROI.lb.mPointX);
		lm = lm / 16;

		rm = max(canvasROI.lt.mPointX, canvasROI.rt.mPointX);
		rm = max(rm, canvasROI.rb.mPointX);
		rm = max(rm, canvasROI.lb.mPointX);
		rm = rm / 16;

		tm = min(canvasROI.lt.mPointY, canvasROI.rt.mPointY);
		tm = min(tm, canvasROI.rb.mPointY);
		tm = min(tm, canvasROI.lb.mPointY);
		tm = tm / 16;

		bm = max(canvasROI.lt.mPointY, canvasROI.rt.mPointY);
		bm = max(bm, canvasROI.rb.mPointY);
		bm = max(bm, canvasROI.lb.mPointY);
		bm = bm / 16;
		#endif
		
		#if defined(EMBEDDED) && defined(VERSION_2)
		if(canvasROI.lt.mPointX <= canvasROI.rt.mPointX)
		{
			lm = canvasROI.lt.mPointX;
		}
		else
		{
			lm = canvasROI.rt.mPointX;
		}
		if(canvasROI.rb.mPointX <= lm)
		{
			lm = canvasROI.rb.mPointX;
		}
		if(canvasROI.lb.mPointX <= lm)
		{
			lm = canvasROI.lb.mPointX;
		}
		lm = lm / 16;

		if(canvasROI.lt.mPointX <= canvasROI.rt.mPointX)
		{
			rm = canvasROI.rt.mPointX;
		}
		else
		{
			rm = canvasROI.lt.mPointX;
		}
		if(canvasROI.rb.mPointX >= rm)
		{
			rm = canvasROI.rb.mPointX;
		}
		if(canvasROI.lb.mPointX >= rm)
		{
			rm = canvasROI.lb.mPointX;
		}
		rm = rm / 16;

		if(canvasROI.lt.mPointY <= canvasROI.rt.mPointY)
		{
			tm = canvasROI.lt.mPointY;
		}
		else
		{
			tm = canvasROI.rt.mPointY;
		}
		if(canvasROI.rb.mPointY <= lm)
		{
			tm = canvasROI.rb.mPointY;
		}
		if(canvasROI.lb.mPointY <= lm)
		{
			tm = canvasROI.lb.mPointY;
		}
		tm = tm / 16;

		if(canvasROI.lt.mPointY <= canvasROI.rt.mPointY)
		{
			bm = canvasROI.rt.mPointY;
		}
		else
		{
			bm = canvasROI.lt.mPointY;
		}
		if(canvasROI.rb.mPointY >= bm)
		{
			bm = canvasROI.rb.mPointY;
		}
		if(canvasROI.lb.mPointY >= bm)
		{
			bm = canvasROI.lb.mPointY;
		}
		bm = bm / 16;
		#endif
		
		if ((rm < 0 || lm > SCREEN_WIDTH || bm < 0 || tm > SCREEN_HEIGHT) && (!pageEnable))
		{
			//out of screen
			//no need to draw
			return AHMI_FUNC_SUCCESS;
		}
	}
	

	//Write all controls in a canvas.
	for (int WidgetCount = 0; WidgetCount < NumofWidget; WidgetCount++)
	{
		WidgetClassPtr wptr = &(pageptr->pWidgetList[ (scptr->StartAddrOfWidget + WidgetCount) ]);
		//Determine whether widget is inside canvas.
		if( 
			(s16)(wptr->WidgetOffsetX) > (s16)(scptr->contentWidth) ||//大于右边界 
			( (s16)(wptr->WidgetOffsetX) + (wptr->WidgetWidth) ) < 0 || //小于左边界
			(s16)(wptr->WidgetOffsetY) > (s16)(scptr->contentHeight)  || //大于下边界
			( (s16)(wptr->WidgetOffsetY) + (wptr->WidgetHeight) ) < 0
            )
			continue;//If it is not inside, it will not be drawn

#ifdef CHIP_GC9002	
		if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)	
		{
			if(getWidgetSoureBoxOverlap(wptr, pTileBox) != AHMI_FUNC_SUCCESS)
			{
				setWidgetSourebufferRefreshFlag(wptr);
				continue;
			}
		}
#endif

		if(((scptr->contentWidth != this->mwidth) || (scptr->contentHeight != this->mheight)) && (wptr->WidgetType == SLIDER_TEX) && ((wptr->WidgetAttr & 0x40) == 0x40))
		{
			

			if(pageEnable || windowPressed)
			{
				//Call widget function
				stutas = wptr->drawWidget(pTileBox, u32p_sourceShift, pageEnable,pageptr,&curCanvasMatrix, RefreshType, staticTextureEn, &canvasROI);
			}
			else
			{
				if(this->CanvasState != 1)
				{
					//Call widget function
					stutas = wptr->drawWidget(pTileBox, u32p_sourceShift, pageEnable,pageptr, &this->mCanvasMatrix, RefreshType, staticTextureEn, &canvasROI);
				}
				else
				{
					//Call widget function
					stutas = wptr->drawWidget(pTileBox, u32p_sourceShift, pageEnable,pageptr, &subcanvasMatrixForSwipe, RefreshType, staticTextureEn, &canvasROI);
				}
			}
		}
		else
		{
			//Call widget function
			if (scptr->scrollWidgetScalePadding)
			{
				//need scale
				widgetScrollScale = calWidgetScrollScale(wptr, scptr, this, scptr->scrollWidgetScalePadding, scptr->scrollWidgetScaleBorderRadius);
				//widgetScrollScale = calWidgetScrollScaleOG(wptr, scptr, this, scptr->scrollWidgetScalePadding);
			}
			else
			{
				widgetScrollScale = 1 << 9;
			}
			//widgetScrollScale = 1<< 9;
			//cal widget scale matrix
			//widgetScrollScaleMatrix.NewMatrix_init();
			//widgetScrollScaleMatrix.NewMatrixInv_init();
			//widgetScrollScaleMatrix.translate(-(wptr->WidgetWidth << 13)/2 - (s16)(wptr->WidgetOffsetX), -(wptr->WidgetHeight << 13)/2 - (s16)(wptr->WidgetOffsetX));
			//widgetScrollScaleMatrix.scale(widgetScrollScale, widgetScrollScale);
			//widgetScrollScaleMatrix.translate((wptr->WidgetWidth << 13)/2  + (s16)(wptr->WidgetOffsetX), (wptr->WidgetHeight << 13)/2 + (s16)(wptr->WidgetOffsetX));

			//widgetScrollScaleMatrix.LeftMultiMatrix( &subcanvasMatrix);
			stutas = wptr->drawWidget(pTileBox, u32p_sourceShift, pageEnable,pageptr, &(subcanvasMatrix), RefreshType, staticTextureEn, &canvasROI, widgetScrollScale);
		}
		if(stutas != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_DrawWidgetErr);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name￡o focusNewSubCanvas
// load new subcanvas
//
// @param1 u8 subCanvasID
// additional comments
//   none
//-----------------------------
funcStatus CanvasClass::focusNewSubCanvas(
		u8 subCanvasID
		)
{
	TileBoxClass canvasTileBox;
#if (defined PARTIAL_DOUBLE_BUFFER || defined PARTIAL_TRIBLE_BUFFER || defined STATIC_BUFFER_EN) 
//	RefreshMsg refreshMsg;
#endif
	SubCanvasClassPtr nextSubCan;
	SubCanvasClassPtr curSubCan;
	SubCanvasClassPtr willLoadSubCan;
	u8 widgetCount;
	WidgetClassPtr curWidgetPtr;
//	u32 actionAddr;
	u16 lastSubCanvasIdx;
	u16 nextSubCanvasIdx;

#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"loading subcanvas %d", subCanvasID);
	ERROR_PRINT(text);
#endif
	
	curSubCan = &(gPagePtr[WorkingPageID].pSubCanvasList[this->mFocusedCanvas + this->mStartAddrOfSubCanvas]);
	lastSubCanvasIdx = this->mFocusedCanvas;
	
	if((animationType != SUBCANVAS_ANIMATION_SWIPE_H) && (animationType != SUBCANVAS_ANIMATION_SWIPE_V) && (animationType != SUBCANVAS_ANIMATION_PULL_DOWN) && (animationType != SUBCANVAS_ANIMATION_PULL_UP) && (animationType != SUBCANVAS_ANIMATION_PULL_LEFT) && (animationType != SUBCANVAS_ANIMATION_PULL_RIGHT) )
	{
		if(lastSubCanvasIdx == subCanvasID)
		{
			return AHMI_FUNC_SUCCESS;
		}
	}

	this->mFocusedCanvas = subCanvasID;
	nextSubCanvasIdx = subCanvasID;
	nextSubCan = &(gPagePtr[WorkingPageID].pSubCanvasList[subCanvasID + this->mStartAddrOfSubCanvas]);
	#if defined(EMBEDDED) && defined(VERSION_2)
	if(nextSubCan->videoExist == true)
    {
		normalvideoframe = 0;
	}
	#endif
	nextSubCan->initSubCanvas();
	for(widgetCount = 0; widgetCount < nextSubCan->NumOfWidget; widgetCount++)
	{
		curWidgetPtr = &(gPagePtr[WorkingPageID].pWidgetList[ nextSubCan->StartAddrOfWidget + widgetCount ]);
		curWidgetPtr->initWidget();
		curWidgetPtr->SetWidgetMatrixRefreshFlag();
		curWidgetPtr->REFRESH_FLAG |= WidgetBindCanvasRefreshFlag;
	}
	 
	if(animationType == ANIMATION_REFRESH_NULL)
	{
		curSubCan->shouldShow = 0;
		nextSubCan->shouldShow = 1;
		//gPageNeedRefresh = PageNormalRefreshFlag;
		nextSubCan->CalculateMatrix(&(nextSubCan->mSubCanvasMatrix));
		this->SetCanvasMatrixRefreshFlag();
	}
	else 
	{
		curSubCan->shouldShow = 0;
		nextSubCan->shouldShow = 1;
		//additional offset for swipe_h swipe_v
		if (animationType == SUBCANVAS_ANIMATION_SWIPE_H)
		{
			nextSubCan->mOffsetX = curSubCan->mOffsetX + this->mwidth * (nextSubCanvasIdx - lastSubCanvasIdx);
			nextSubCan->animationStartOffset = nextSubCan->mOffsetX;
			loadSubCanvasWithAnimation(curSubCan, nextSubCan);
		}else if (animationType == SUBCANVAS_ANIMATION_SWIPE_V)
		{
			nextSubCan->mOffsetY = curSubCan->mOffsetY + this->mheight * (nextSubCanvasIdx - lastSubCanvasIdx);
			nextSubCan->animationStartOffset = nextSubCan->mOffsetY;
			loadSubCanvasWithAnimation(curSubCan, nextSubCan);
		}
		else if (animationType == SUBCANVAS_ANIMATION_PULL_DOWN || animationType == SUBCANVAS_ANIMATION_PULL_UP)
		{
			if ((this->pullState & 2))
			{
				willLoadSubCan = &(gPagePtr[WorkingPageID].pSubCanvasList[this->subCanvasWillLoadIdx + this->mStartAddrOfSubCanvas]);

				if (this->pullState & 4)
				{
					//back
					if (nextSubCan == curSubCan || nextSubCan == willLoadSubCan)
					{
						nextSubCan->animationStartOffset = curSubCan->mOffsetY;
					}
					else
					{
						//override will load subcanvas
						this->pullState = 0;
						willLoadSubCan->shouldShow = 0;
						curSubCan->shouldShow = 1;
						nextSubCan->mOffsetY = (animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? -1 : 1) * this->mheight;
						nextSubCan->animationStartOffset = nextSubCan->mOffsetY;
					}
				}
				else
				{
					//forward
					if (nextSubCan == curSubCan || nextSubCan == willLoadSubCan)
					{
						nextSubCan->animationStartOffset = willLoadSubCan->mOffsetY;
					}
					else
					{
						//stop pulling
						this->pullState = 0;
						willLoadSubCan->shouldShow = 0;
						curSubCan->shouldShow = 1;
						nextSubCan->mOffsetY = (animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? -1 : 1) * this->mheight;
						nextSubCan->animationStartOffset = nextSubCan->mOffsetY;
						this->subCanvasWillLoadIdx = nextSubCanvasIdx;
					}
				}
				
				loadSubCanvasWithAnimation(curSubCan, nextSubCan);
			}
			else
			{
				//normal
				if (lastSubCanvasIdx != nextSubCanvasIdx)
				{
					//should change
					this->pullState = 0;
					curSubCan->shouldShow = 1;
					nextSubCan->mOffsetY = (animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? -1 : 1) * this->mheight;
					nextSubCan->animationStartOffset = nextSubCan->mOffsetY;
					loadSubCanvasWithAnimation(curSubCan, nextSubCan);
				}
			}
		}
		else if (animationType == SUBCANVAS_ANIMATION_PULL_RIGHT || animationType == SUBCANVAS_ANIMATION_PULL_LEFT)
		{
			if ((this->pullState & 1))
			{
				willLoadSubCan = &(gPagePtr[WorkingPageID].pSubCanvasList[this->subCanvasWillLoadIdx + this->mStartAddrOfSubCanvas]);
				if (this->pullState & 4)
				{
					//back
					if (nextSubCan == curSubCan || nextSubCan == willLoadSubCan)
					{
						nextSubCan->animationStartOffset = curSubCan->mOffsetX;
					}
					else
					{
						//override will load subcanvas
						this->pullState = 0;
						willLoadSubCan->shouldShow = 0;
						curSubCan->shouldShow = 1;
						nextSubCan->mOffsetX = (animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? -1 : 1) * this->mwidth;
						nextSubCan->animationStartOffset = nextSubCan->mOffsetX;
					}
				}
				else
				{
					//forward
					if (nextSubCan == curSubCan || nextSubCan == willLoadSubCan)
					{
						nextSubCan->animationStartOffset = willLoadSubCan->mOffsetX;
					}
					else
					{
						//override will load subcanvas
						this->pullState = 0;
						willLoadSubCan->shouldShow = 0;
						curSubCan->shouldShow = 1;
						nextSubCan->mOffsetX = (animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? -1 : 1) * this->mwidth;
						nextSubCan->animationStartOffset = nextSubCan->mOffsetX;
					}
				}
				
				loadSubCanvasWithAnimation(curSubCan, nextSubCan);
			}
			else
			{
				//normal
				if (lastSubCanvasIdx != nextSubCanvasIdx)
				{
					//should change
					this->pullState = 0;
					curSubCan->shouldShow = 1;
					nextSubCan->mOffsetX = (animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? -1 : 1) * this->mwidth;
					nextSubCan->animationStartOffset = nextSubCan->mOffsetX;
					loadSubCanvasWithAnimation(curSubCan, nextSubCan);
				}
			}
		}
		else
		{
			if(lastSubCanvasIdx != nextSubCanvasIdx)
			{
				loadSubCanvasWithAnimation(curSubCan, nextSubCan);
			}
		}
	}
	////Sending subcanvas instruction      subcanvas does not support action at present.
	/*if(nextSubCan->startAddrOfSubCanAction < gPagePtr[WorkingPageID].mActionInstructionsSize)
	{
		actionAddr = (u32)(gPagePtr[WorkingPageID].pActionInstructions + nextSubCan->startAddrOfSubCanAction);
		xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
	}*/
	return AHMI_FUNC_SUCCESS;
}


//-----------------------------
// name￡o loadSubCanvasWithAnimation
// load new subcanvas with animation
//
// @param1 SubCanvasClassPtr pCurSubCanvas pointer
// @param2 SubCanvasClassPtr pNextSubCanvas pointer
// additional comments
//   none
//-----------------------------
u32 CanvasClass::loadSubCanvasWithAnimation(
	SubCanvasClassPtr pCurSubCanvas,
	SubCanvasClassPtr pNextSubCanvas
	)
{
	funcStatus AHMI_status;
	ElementPtr pOldElement, pNewElement;
	//u32 actionAddr;
	pOldElement.cptr = this;
	pNewElement.cptr = this;
	pOldElement.scptr = pCurSubCanvas;
	pNewElement.scptr = pNextSubCanvas;

	AHMI_status= gAnimationClass.Create_AnimatianData(
		ANIMATION_REFRESH_SUBCANVAS,
		pNewElement,
		pOldElement
		);

	if(AHMI_status != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_CreateAnimationErr);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name￡o initCanvas
// initial canvas
//
// @param1 none
// additional comments
//   none
//-----------------------------
funcStatus CanvasClass::initCanvas(void)
{
	//todo
	this->curCustomAnimationPtr = 0xff; //indicating that there is no animation
	this->mCanvasMatrix.NewMatrix_init();
	this->mCanvasMatrix.NewMatrixInv_init();
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: setATag
// set the animation tag, compute the matrix
// canvas animation is the custom animation for canvas
//  
//     @param1 u8 value
// addtional comments :
//   none
//-----------------------------
u32 CanvasClass::setATag(u16 value)
{
	AnimationActionClassPtr curCanvasAnimation;
	s16 curCanvasOffsetX; //1.11.4
	s16 curCanvasOffsetY; //1.11.4
	s32 curWidth;     //1.21.10
	s32 curHeight;    //1.21.10
	s16 scalerX;     //1.6.9
	s16 scalerY;     //1.6.9
	u16 transitionParamIn; //0.10
	u16 transitionParamOut; //0.10
	s32 rotateX;
	s32 rotateY;
	s32 scaleCenterX;
	s32 scaleCenterY;
	s32 rotateAngle;
	s32 projectX;
	s32 projectY;
	u16 duration;
	AnimationKeyFramePtr leftFrame;
	AnimationKeyFramePtr rightFrame;
	int i;
#if(defined PARTIAL_DOUBLE_BUFFER) || (defined STATIC_BUFFER_EN) || (defined PARTIAL_TRIBLE_BUFFER)
//	RefreshMsg refreshMsg;
#endif

//taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set canvas aTag %d", value);
	ERROR_PRINT(text);
#endif
	
	curCanvasAnimation = &(gPagePtr[WorkingPageID].pAnimationActionList[this->curCustomAnimationPtr]);
	if(curCanvasAnimation == NULL)
	{
//		taskEXIT_CRITICAL();
		return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_NULLPointerErr);
	}
	duration = gPagePtr[WorkingPageID].pAnimationKeyFrameList[curCanvasAnimation->keyFrameStopID].mTime;
	if((value > duration) || duration == 0)
	{
//		taskEXIT_CRITICAL();
		ERROR_PRINT("ERROR in setting canvas animation tag, value out of baundary");
		return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_DurationOverrun);
	}
	this->mATag = value;
	i = -1;
	for (i = curCanvasAnimation->keyFrameStartID+1; i <= curCanvasAnimation->keyFrameStopID; i++)
	{
		if ( gPagePtr[WorkingPageID].pAnimationKeyFrameList[i].mTime >= value)
		{
			break;
		}
	}
	if (i==-1)
	{
		ERROR_PRINT("ERROR in getting canvas animation key frames, value out of baundary");
		return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_DurationOverrun);
	}
	else
	{
		leftFrame = gPagePtr[WorkingPageID].pAnimationKeyFrameList + i - 1;
		rightFrame = leftFrame + 1;
	}

	duration = rightFrame->mTime - leftFrame->mTime;

	if (duration == 0)
	{
		curCanvasOffsetX = rightFrame->mTranslateX;
		curCanvasOffsetY = rightFrame->mTranslateY;
		curWidth = rightFrame->mScaleX;
		curHeight = rightFrame->mScaleY;
		rotateX = rightFrame->mRotateX;
		rotateY = rightFrame->mRotateY;
		scaleCenterX = rightFrame->mScaleCenterX;
		scaleCenterY = rightFrame->mScaleCenterY;
		rotateY = rightFrame->mRotateY;
		rotateAngle = rightFrame->mRotateRadius;
		projectX = rightFrame->mProjectX;
		projectY = rightFrame->mProjectY;
	}
	else
	{
		transitionParamIn = (mATag-leftFrame->mTime) * 1024 / duration;//0.10

		gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, rightFrame->mTimingFun);

		//relative to canvas position
		curCanvasOffsetX = (leftFrame->mTranslateX * 16) + (s32)(transitionParamOut * (rightFrame->mTranslateX - leftFrame->mTranslateX) / 64 );// - (this->moffsetX << 4);
		curCanvasOffsetY = (leftFrame->mTranslateY * 16) + (s32)(transitionParamOut * (rightFrame->mTranslateY - leftFrame->mTranslateY) / 64 );// - (this->moffsetY << 4);
		curCanvasOffsetX = curCanvasOffsetX / 16;
		curCanvasOffsetY = curCanvasOffsetY / 16;
		
		curWidth = (s32)( (s16)(leftFrame->mScaleX) + (s32)(transitionParamOut * ((s16)rightFrame->mScaleX - (s16)leftFrame->mScaleX) / 1024)    );
		curHeight = (s32)( (s16)(leftFrame->mScaleY) + (s32)(transitionParamOut * ((s16)rightFrame->mScaleY - (s16)leftFrame->mScaleY) / 1024));
		
		scaleCenterX = rightFrame->mScaleCenterX - leftFrame->mScaleCenterX;
		scaleCenterX = scaleCenterX * transitionParamOut / 1024 + leftFrame->mScaleCenterX;

		scaleCenterY = rightFrame->mScaleCenterY - leftFrame->mScaleCenterY;
		scaleCenterY = scaleCenterY * transitionParamOut / 1024 + leftFrame->mScaleCenterY;

		rotateX = rightFrame->mRotateX - leftFrame->mRotateX;
		rotateX = rotateX * transitionParamOut / 1024 + leftFrame->mRotateX;

		rotateY = rightFrame->mRotateY - leftFrame->mRotateY;
		rotateY = rotateY * transitionParamOut / 1024 + leftFrame->mRotateY;

		rotateAngle = rightFrame->mRotateRadius - leftFrame->mRotateRadius;
		rotateAngle = rotateAngle * transitionParamOut / 1024 + leftFrame->mRotateRadius;

		projectX = rightFrame->mProjectX - leftFrame->mProjectX;
		projectX = projectX * transitionParamOut / 1024 + leftFrame->mProjectX;

		projectY = rightFrame->mProjectY - leftFrame->mProjectY;
		projectY = projectY * transitionParamOut / 1024 + leftFrame->mProjectY;
	}

	if(curWidth > (0x8000))
		curWidth = 0x7FFF;//modify to max
	if(curHeight > (0x8000))
		curHeight = 0x7FFF;//modify to max

	scalerX = (curWidth >> 1)  ; //1.21.10 trans into 1.6.9
	scalerY = (curHeight >> 1) ; //1.21.10 trans into 1.6.9

	if(scalerX < 0)
		scalerX = 0xFE00;//-1 << 9
	if(scalerY < 0)
		scalerY = 0xFE00;//-1 << 9

	if(scalerX == 0)
		scalerX = 1;//modify to min
	if(scalerY == 0)
		scalerY = 1;//modify to min

	this->moffsetX = curCanvasOffsetX;
	this->moffsetY = curCanvasOffsetY;
	this->mZoomX = scalerX;
	this->mZoomY = scalerY;

	this->mScaleCenterX = scaleCenterX;
	this->mScaleCenterY = scaleCenterY;
	
	this->mRotateCenterX = rotateX;
	this->mRotateCenterY = rotateY;
	this->mRotateAngle = rotateAngle;
	this->mProjectionX = projectX;
	this->mProjectionY = projectY;

	//CalculateMatrix(&(this->mCanvasMatrix));
	this->SetCanvasMatrixRefreshFlag();

//taskEXIT_CRITICAL();

	return AHMI_FUNC_SUCCESS;

}

s16 getTranslateAttrValue(s16 mTranslate,u16 mTranslateTag){
	if(mTranslateTag!=0){
		//tag exists
		TagClassPtr curTag = &TagPtr->getTag(mTranslateTag);
		return (s16)curTag->getValue();
	}else{
		return mTranslate;
	}
}

u16 getScaleAttrValue(u16 mScale,u16 mScaleTag){
	if(mScaleTag!=0){
		//tag exists
		TagClassPtr curTag = &TagPtr->getTag(mScaleTag);
		return (u16)curTag->getValue();
	}else{
		return mScale;
	}
}

u32 getTagValue(u16 mTag){
	TagClassPtr curTag = &TagPtr->getTag(mTag);
	return (u16)curTag->getValue();
}

//-----------------------------
// name: triggerAnimation
//trigger canvas custom animation
//  
//     @param1 u8 animationID
// addtional comments :
//   none
//-----------------------------
u32 CanvasClass::triggerAnimation(u8 animationID)
{
	ElementPtr curPtr;
	u8 i;
	u16 j;
	AnimationActionClassPtr curAnimationAction;
	AnimationKeyFramePtr frame;
	curPtr.cptr = this;

	//judge whether the animation is available
	//if(animationID >= this->startAddrOfCanvasCustomAnimation && animationID < (this->startAddrOfCanvasCustomAnimation + this->numOfCanvasCustomAnimation) )
	for(i = this->startAddrOfCanvasCustomAnimation; i < this->startAddrOfCanvasCustomAnimation + this->numOfCanvasCustomAnimation;i++)
	{
		curAnimationAction = gPagePtr[WorkingPageID].pAnimationActionList + i;
		if(curAnimationAction->mAnimationID == animationID)
		{
			curCustomAnimationPtr = i;
			//set animation value with tags
			for (int j = curAnimationAction->keyFrameStartID; j <= curAnimationAction->keyFrameStopID; j++)
			{
				frame = gPagePtr[WorkingPageID].pAnimationKeyFrameList + j;
				if (frame->mTranslateXTag!=0)
				{
					frame->mTranslateX = (s16)getTagValue(frame->mTranslateXTag);
				}
				if (frame->mTranslateYTag!=0)
				{
					frame->mTranslateY = (s16)getTagValue(frame->mTranslateYTag);
				}
				if (frame->mScaleXTag!=0)
				{
					frame->mScaleX = (s16)getTagValue(frame->mScaleXTag);
				}
				if (frame->mScaleYTag!=0)
				{
					frame->mScaleY = (s16)getTagValue(frame->mScaleYTag);
				}
				if (frame->mRotateXTag!=0)
				{
					frame->mRotateX = (s16)getTagValue(frame->mRotateXTag);
				}
				if (frame->mRotateYTag!=0)
				{
					frame->mRotateY = (s16)getTagValue(frame->mRotateYTag);
				}
				if (frame->mRotateRadiusTag!=0)
				{
					frame->mRotateRadius = (s16)getTagValue(frame->mRotateRadiusTag);
				}
				if (frame->mProjectXTag!=0)
				{
					frame->mProjectX = (s16)getTagValue(frame->mProjectXTag);
				}
				if (frame->mProjectYTag!=0)
				{
					frame->mProjectY = (s16)getTagValue(frame->mProjectYTag);
				}
			}
			
			

			return gAnimationClass.Create_AnimatianData(ANIMATION_REFRESH_CANVAS, curPtr, curPtr);
		}
	}
	
	return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_TriggerAnimationErr);
}

extern uint32_t AHMITick;
u32 CanvasClass::mouseTouchCtrl(
		ActionTriggerClassPtr ActionPtr,
		PIDPoint* pPressPoint,
		PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
		PIDPoint* pAbsoluteReleasePoint,
		u8 u8_pageRefresh      
	)
{
//	DynamicPageClassPtr pCurrentPage;
	
	u16 cur_x;
	u16 cur_y;

	SubCanvasClassPtr tempScptr;
	SubCanvasClassPtr nextSubCanvas;

	u16 nextSubCanvasIdx;
	
	//TextureClassPtr curTexture;
//	int i;

	int nextFocusSubcanvasID = mFocusedCanvas;

	s32 duration = 0;
	s32 swipelengthThreshold = 0;
	s32 swipelength = 0;
	s32 swipespeed = 0;
	s32 pullSpeed = 0;
	s32 pullSpeedThreshold = 1;
	s32 releaseSpeedThreshold = 500;

	u8 shouldInitNextSubCanvasTranslate = 0;


	//check the parameters
	if( NULL == ActionPtr)
	{
		ERROR_PRINT("ERROR: canvas illegal input");
		return (EM_enErrRetModularCanvas | EM_ErrRetCanvas_NULLPointerErr);
	}

	if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H || this->animationType == SUBCANVAS_ANIMATION_SWIPE_V || this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN || this->animationType == SUBCANVAS_ANIMATION_PULL_UP || this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT || this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT)
	{
		
	}else{
		tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + mFocusedCanvas;
		if((tempScptr->contentWidth == this->mwidth) && (tempScptr->contentHeight == this->mheight))
		{
			return WIDGET_TOUCH_CANCEL;
		}
		else
		{

		}
	}
	
	
	//get texture
//	pCurrentPage = &gPagePtr[WorkingPageID];
	
	if( ActionPtr->mInputType == ACTION_MOUSE_PRESS ) //mouse press
	{
		cur_x  = (s16)(pPressPoint->x);
		cur_y  = (s16)(pPressPoint->y);
		this->pressedPosX = cur_x;
		this->pressedPosY = cur_y;
		this->holdPosX = cur_x;
		this->holdPosY = cur_y;
		this->ReleasingSpeed = 0;
		if (this->pullState)
		{
			//is pulling animation
			//will not respond press
			return AHMI_FUNC_SUCCESS;
		}
		else
		{
			this->pressFlag = 1;
		}
		
		this->CanvasState = 0;
		this->pressInEdges = this->calEdgesPressed(this->pressedPosX, this->pressedPosY);
		this->OldTickForCanvasSwipe = AHMITick;
	}
	else if(ActionPtr->mInputType == ACTION_MOUSE_HOLDING) //mouse up
	{
		tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + mFocusedCanvas;
		if (this->pressFlag)
		{
			cur_x  = (s16)(pReleasePoint->x);
            cur_y  = (s16)(pReleasePoint->y);
			
			//check pull
			if((this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN || this->animationType == SUBCANVAS_ANIMATION_PULL_UP))
			{
				/*if(subCanvas.animating && !subCanvas.triggerYPull){
					handled = 'canceled'
					return handled
				}*/
				if((this->pressInEdges & 1) || (this->pressInEdges & 2))
				{
					//should check if pull 
					duration = AHMITick - this->OldTickForCanvasSwipe;
					if(duration < 0)
						duration  = 0 - duration;
					pullSpeed =  s32(cur_y - this->pressedPosY)*1000/duration;
					if(!(this->pullState & 2))
					{
						if(abs((float)pullSpeed) > pullSpeedThreshold)
						{
							if((this->pressInEdges & 1) && pullSpeed > 0)
							{
								//trigger pull down
								this->pullState &= ~(1 << 2);
								this->pullState |= (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? 0:1) << 2;
								this->pullState |= 2;
								shouldInitNextSubCanvasTranslate = 1;
							}
							else if((this->pressInEdges & 2) && pullSpeed < 0)
							{
								//trigger pull up
								this->pullState &= ~(1 << 2);
								this->pullState |= (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? 1:0) << 2;
								this->pullState |= 2;
								shouldInitNextSubCanvasTranslate = 1;
							}else{
								
							}
						}
					}

					if(this->pullState & 2){
						
						if((this->pullState & 4) ==0)
						{
							//forward
							nextSubCanvasIdx = this->mFocusedCanvas+1;
							
							if(nextSubCanvasIdx < this->mNumOfSubCanvas)
							{
								nextSubCanvas = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + nextSubCanvasIdx;
								this->subCanvasWillLoadIdx = nextSubCanvasIdx;

								//subCanvas.animating = true

								//nextSubCanvas.speedY = pullSpeed
//								if (shouldInitNextSubCanvasTranslate)
//								{
//									nextSubCanvas->mOffsetY = (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? -1 : 1) * this->mheight;
//								}
								//nextSubCanvas.translate = nextSubCanvas.translate||{x:0,y: (method=='PULL_DOWN'? -1 : 1)* canvas.h}
								nextSubCanvas->mOffsetY = ((this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? -1 : 1) * this->mheight) + s16(cur_y - this->pressedPosY);

								nextSubCanvas->CalculateMatrix(&(nextSubCanvas->mSubCanvasMatrix));
								nextSubCanvas->shouldShow = 1;
							}
						}
						else
						{
							if(this->mFocusedCanvas > 0 && this->mFocusedCanvas < this->mNumOfSubCanvas)
							{
								nextSubCanvasIdx = this->mFocusedCanvas-1;
								nextSubCanvas = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + nextSubCanvasIdx;
								this->subCanvasWillLoadIdx = nextSubCanvasIdx;

								//subCanvas.animating = true

								//subCanvas.speedY = pullSpeed
								if (shouldInitNextSubCanvasTranslate)
								{
									tempScptr->mOffsetY = 0;
									nextSubCanvas->mOffsetY = 0;
									nextSubCanvas->CalculateMatrix(&(nextSubCanvas->mSubCanvasMatrix));
								}
								//nextSubCanvas.translate = nextSubCanvas.translate||{x:0,y: (method=='PULL_DOWN'? -1 : 1)* canvas.h}
								tempScptr->mOffsetY = s16(cur_y - this->pressedPosY);
								tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));

								nextSubCanvas->shouldShow = 1;
							}
						}
					}
					
					return AHMI_FUNC_SUCCESS;
				}else{
					//check scroll
				}
			}
			else if(this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT || this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT)
			{
				/*if(subCanvas.animating && !subCanvas.triggerXPull){
					handled = 'canceled'
					return handled
				}*/
				if((this->pressInEdges & 4) || (this->pressInEdges & 8)){
					//should check if pull 
					duration = AHMITick - this->OldTickForCanvasSwipe;
					if(duration < 0)
						duration  = 0 - duration;
					pullSpeed =  s32(cur_x - this->pressedPosX)*1000/duration;
//					if(pullSpeed < 0)
//						pullSpeed = -pullSpeed;
					if(!(this->pullState & 1)){
						if(abs((float)pullSpeed) > pullSpeedThreshold){
							if((this->pressInEdges & 4) && pullSpeed > 0){
								this->pullState &= ~(1 << 2);
								this->pullState |= (this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? 0:1) << 2;
								this->pullState |= 1;
								shouldInitNextSubCanvasTranslate = 1;

							}else if((this->pressInEdges & 8) && pullSpeed < 0){
								this->pullState &= ~(1 << 2);
								this->pullState |= (this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? 1:0) << 2;
								this->pullState |= 1;
								shouldInitNextSubCanvasTranslate = 1;

							}else{

							}
						}
					}

					if(this->pullState & 1){
						if((this->pullState & 4) ==0){
							//forward
							nextSubCanvasIdx = this->mFocusedCanvas+1;
							
							if(nextSubCanvasIdx < this->mNumOfSubCanvas)
							{
								nextSubCanvas = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + nextSubCanvasIdx;
								this->subCanvasWillLoadIdx = nextSubCanvasIdx;

								/*subCanvas.animating = true

								nextSubCanvas.speedX = pullSpeed*/
//								if (shouldInitNextSubCanvasTranslate)
//								{
//									nextSubCanvas->mOffsetX = (this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? -1 : 1) * this->mwidth;
//								}
								
								nextSubCanvas->mOffsetX = ((this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? -1 : 1) * this->mwidth) + s16(cur_x - this->pressedPosX);

								nextSubCanvas->CalculateMatrix(&(nextSubCanvas->mSubCanvasMatrix));
								nextSubCanvas->shouldShow = 1;
							}
						}else{
							if(this->mFocusedCanvas > 0 && this->mFocusedCanvas < this->mNumOfSubCanvas)
							{
								nextSubCanvasIdx = this->mFocusedCanvas-1;
								nextSubCanvas = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + nextSubCanvasIdx;
								this->subCanvasWillLoadIdx = nextSubCanvasIdx;

								//subCanvas.animating = true

								//subCanvas.speedY = pullSpeed
								if (shouldInitNextSubCanvasTranslate)
								{
									tempScptr->mOffsetX = 0;
									nextSubCanvas->mOffsetX = 0;
									nextSubCanvas->CalculateMatrix(&(nextSubCanvas->mSubCanvasMatrix));
								}

								tempScptr->mOffsetX = s16(cur_x - this->pressedPosX);
								
								tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));

								nextSubCanvas->shouldShow = 1;
							}
						}
					}
                
					return AHMI_FUNC_SUCCESS;
				}else{
					//check scroll
				}
			}
			
			if((tempScptr->contentWidth == this->mwidth) && (tempScptr->contentHeight == this->mheight))
			{
				this->CanvasState = 1;
				if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
				{
					if(cur_x == this->holdPosX)
						return AHMI_FUNC_SUCCESS;
						
					duration = AHMITick - this->OldTickForCanvasSwipe;
					this->OldTickForCanvasSwipe = AHMITick;
						
					updateSubCanvasListOffset(cur_x - this->holdPosX, 0);
					this->ReleasingSpeed = (cur_x - this->holdPosX) * 1000 / duration;
				}
				else if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
				{
					if(cur_y == this->holdPosY)
						return AHMI_FUNC_SUCCESS;
						
					duration = AHMITick - this->OldTickForCanvasSwipe;
					this->OldTickForCanvasSwipe = AHMITick;
						
					updateSubCanvasListOffset(cur_y - this->holdPosY, 1);
					this->ReleasingSpeed = (cur_y - this->holdPosY) * 1000 / duration;
				}
			}
			else if((tempScptr->contentWidth != this->mwidth) && (tempScptr->contentHeight == this->mheight))
			{
				if(((cur_y - this->pressedPosY) > 100) || ((this->pressedPosY - cur_y) > 100))
				{
					this->CanvasState = 1;
					if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
					{
						updateSubCanvasListOffset(cur_y - this->holdPosY, 1);
					}
				}
				else
				{
					if(((tempScptr->contentOffsetX) <= 0) && ((tempScptr->contentOffsetX) >= (this->mwidth - tempScptr->contentWidth)) 
					&& (this->CanvasState != 1))
					{
						this->CanvasState = 2;
						updateSubCanvasOffset(cur_x - this->holdPosX, 0);
					}
					else
					{
						this->CanvasState = 1;
						if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
						{
							updateSubCanvasListOffset(cur_x - this->holdPosX, 0);
						}
						else if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
						{
							updateSubCanvasListOffset(cur_y - this->holdPosY, 1);
						}
					}
				}
			}
			else if((tempScptr->contentWidth == this->mwidth) && (tempScptr->contentHeight != this->mheight))
			{
				if(((cur_x - this->pressedPosX) > 100) || ((this->pressedPosX - cur_x) > 100))
				{
					this->CanvasState = 1;
					if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
					{
						updateSubCanvasListOffset(cur_x - this->holdPosX, 0);
					}
				}
				else
				{
					if(((tempScptr->contentOffsetY) <= 0) && ((tempScptr->contentOffsetY) >= (this->mheight - tempScptr->contentHeight)) 
					&& (this->CanvasState != 1))
					{
						this->CanvasState = 2;
						updateSubCanvasOffset(0, cur_y - this->holdPosY);
					}
					else
					{
						this->CanvasState = 1;
						if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
						{
							updateSubCanvasListOffset(cur_x - this->holdPosX, 0);
						}
						else if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
						{
							updateSubCanvasListOffset(cur_y - this->holdPosY, 1);
						}
					}
				}
			}
			else if((tempScptr->contentWidth != this->mwidth) && (tempScptr->contentHeight != this->mheight))
			{
				if(((tempScptr->contentOffsetX) <= 0) && ((tempScptr->contentOffsetX) >= (this->mwidth - tempScptr->contentWidth)) 
				&& ((tempScptr->contentOffsetY) <= 0) && ((tempScptr->contentOffsetY) >= (this->mheight - tempScptr->contentHeight))
				&& (this->CanvasState != 1))
				{
					this->CanvasState = 2;
					updateSubCanvasOffset(cur_x - this->holdPosX, cur_y - this->holdPosY);
				}
				else
				{
					this->CanvasState = 1;
					if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
					{
						updateSubCanvasListOffset(cur_x - this->holdPosX, 0);
					}
					else if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
					{
						updateSubCanvasListOffset(cur_y - this->holdPosY, 1);
					}
				}
			}
			this->holdPosX = cur_x;
			this->holdPosY = cur_y;
		}
	}else if(ActionPtr->mInputType == ACTION_MOUSE_RELEASE || ActionPtr->mInputType == ACTION_MOUSE_CANCLE)
	{
		if (!(this->pressFlag))
			return AHMI_FUNC_SUCCESS;
				
		tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + mFocusedCanvas;
		
		if (this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT || this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT )
		{
			cur_x  = (s16)(pReleasePoint->x);
			
			duration = AHMITick - this->OldTickForCanvasSwipe;
			if(duration < 0)
				duration  = 0 - duration;

			swipelengthThreshold = this->mwidth / 2;
			swipelength = s32(cur_x - this->pressedPosX);
			releaseSpeedThreshold = this->mwidth;
			swipespeed = swipelength*1000/duration;
		}
		else if (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN || this->animationType == SUBCANVAS_ANIMATION_PULL_UP)
		{
			cur_y  = (s16)(pReleasePoint->y);
			
			duration = AHMITick - this->OldTickForCanvasSwipe;
			if(duration < 0)
				duration  = 0 - duration;
			
			swipelengthThreshold = this->mheight / 2;
			swipelength = s32(cur_y - this->pressedPosY);
			releaseSpeedThreshold = this->mheight;
			swipespeed = swipelength*1000/duration;
		}

		//check pull
		if ((this->pullState & 1) || (this->pullState & 2))
		{
			//is pulling
			if (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN || this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT)
			{
				if ((this->pullState & 4) == 0)
				{
					//forward
					if((swipespeed > releaseSpeedThreshold) || (swipelength > swipelengthThreshold))
					{
						nextFocusSubcanvasID = mFocusedCanvas + 1;
					}
					else
					{
						nextFocusSubcanvasID = mFocusedCanvas;
					}
				}
				else
				{
					//backward
					if((swipespeed < -releaseSpeedThreshold) || (swipelength < -swipelengthThreshold))
					{
						nextFocusSubcanvasID = mFocusedCanvas - 1;
					}
					else
					{
						nextFocusSubcanvasID = mFocusedCanvas;
					}
				}
			}
			else if (this->animationType == SUBCANVAS_ANIMATION_PULL_UP || this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT)
			{
				if ((this->pullState & 4) == 0)
				{
					//forward
					if((swipespeed < -releaseSpeedThreshold) || (swipelength < -swipelengthThreshold))
					{
						nextFocusSubcanvasID = mFocusedCanvas + 1;
					}
					else
					{
						nextFocusSubcanvasID = mFocusedCanvas;
					}
				}
				else
				{
					//backward
					if((swipespeed > releaseSpeedThreshold) || (swipelength > swipelengthThreshold))
					{
						nextFocusSubcanvasID = mFocusedCanvas - 1;
					}
					else
					{
						nextFocusSubcanvasID = mFocusedCanvas;
					}
				}
			}

			if (nextFocusSubcanvasID < 0)
			{
				nextFocusSubcanvasID = 0;
			}
			if (nextFocusSubcanvasID >= mNumOfSubCanvas)
			{
				nextFocusSubcanvasID = mNumOfSubCanvas - 1;
			}

			this->pressFlag = 0;
			
			TagPtr->getTag(this->bindTagID).setValue(nextFocusSubcanvasID,this->bindTagID);

			//set after animation finish
			//this->pullState = 0;
			return AHMI_FUNC_SUCCESS;
		}
		
		if((tempScptr->contentWidth == this->mwidth) && (tempScptr->contentHeight == this->mheight))
		{
			this->pressFlag = 0;
			
			int movespeed;
			int moveDir = 0;

			movespeed = this->ReleasingSpeed;

			if(movespeed < 0)
			{
				moveDir = 1;
				movespeed = 0 - movespeed;
			}

			int step = 2;
		
			//extern uint32_t AHMIDone_fps;
			int movePerFrame = movespeed / 33;//33 means FPS Max
			
			if((this->animationType == SUBCANVAS_ANIMATION_SWIPE_H || this->animationType == SUBCANVAS_ANIMATION_SWIPE_V) &&
		       (ConfigData.PageSwipeMode & PageSwipeMode_SwitchMultiPage) == PageSwipeMode_Normal)
			{
				this->ReleasingSpeed = 0;
				this->ReleasingAnimationFlag = 0;
				
				if(movespeed > 0x200)
				{
					nextFocusSubcanvasID = this->mFocusedCanvas + (moveDir>0 ? 1:-1);
				}
				else
				{
					int subcanvasOffsetX;
					int subcanvasOffsetY;
					if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
					{
						subcanvasOffsetX = tempScptr->mOffsetX;
						if((subcanvasOffsetX + (subcanvasOffsetX > 0 ?this->mwidth/2:-this->mwidth/2)) / this->mwidth)
							nextFocusSubcanvasID = this->mFocusedCanvas - (( subcanvasOffsetX+ (subcanvasOffsetX > 0 ?this->mwidth/2:-this->mwidth/2)) / this->mwidth);
					}
					else if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
					{
						subcanvasOffsetY = tempScptr->mOffsetY;
						if((subcanvasOffsetY + (subcanvasOffsetY > 0 ?this->mheight/2:-this->mheight/2)) / this->mheight)
							nextFocusSubcanvasID = this->mFocusedCanvas - (( subcanvasOffsetY+ (subcanvasOffsetY > 0 ?this->mheight/2:-this->mheight/2)) / this->mheight);
					}
				}
				
				if (nextFocusSubcanvasID < 0)
				{
					nextFocusSubcanvasID = 0;
				}
				if (nextFocusSubcanvasID >= mNumOfSubCanvas)
				{
					nextFocusSubcanvasID = mNumOfSubCanvas - 1;
				}
				
				TagPtr->getTag(this->bindTagID).setValue(nextFocusSubcanvasID,this->bindTagID);
			}
			else if((this->animationType == SUBCANVAS_ANIMATION_SWIPE_H || this->animationType == SUBCANVAS_ANIMATION_SWIPE_V) &&
		       (ConfigData.PageSwipeMode & PageSwipeMode_SwitchMultiPage) == PageSwipeMode_SwitchMultiPage)
			{
				this->ReleasingAnimationFlag = 1;

				this->ReleasingSpeed = movePerFrame * (moveDir > 0 ? -1 : 1);
				if(this->ReleasingSpeed > CanvasSwipeSpeedMax)
					this->ReleasingSpeed = CanvasSwipeSpeedMax;
					
				if(this->ReleasingSpeed < -CanvasSwipeSpeedMax)
					this->ReleasingSpeed = -CanvasSwipeSpeedMax;

				this->subcanvasAnimationDuration = 0xFFFF;//((movePerFrame + step-1) / step + 1) * 1000/AHMIDone_fps;
				loadSubCanvasWithAnimation(tempScptr, tempScptr);
			}
		}
		else
		{
			if(((tempScptr->mOffsetX) < 0) && ((tempScptr->mOffsetX) > (this->mwidth - tempScptr->contentWidth)) 
				&& ((tempScptr->mOffsetY) < 0) && ((tempScptr->mOffsetY) > (this->mheight - tempScptr->contentHeight)))
				{

				}
				else
				{
					if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
					{
						nextFocusSubcanvasID = mFocusedCanvas - ((tempScptr->mOffsetX + (tempScptr->mOffsetX > 0 ?this->mwidth/2:-this->mwidth/2)) / this->mwidth);
					}
					else if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
					{
						nextFocusSubcanvasID = mFocusedCanvas - ((tempScptr->mOffsetY + (tempScptr->mOffsetY > 0 ? this->mheight/2 : -this->mheight/2)) / this->mheight);
					}
					
					this->pressFlag = 0;

					if (nextFocusSubcanvasID < 0)
					{
						nextFocusSubcanvasID = 0;
					}
					if (nextFocusSubcanvasID >= mNumOfSubCanvas)
					{
						nextFocusSubcanvasID = mNumOfSubCanvas - 1;
					}

					//focusNewSubCanvas(nextFocusSubcanvasID);
					if((this->animationType == SUBCANVAS_ANIMATION_SWIPE_H) || (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V))
					{
						TagPtr->getTag(this->bindTagID).setValue(nextFocusSubcanvasID,this->bindTagID);
					}
				}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

void CanvasClass::LoadNextSubCanvasAfterReleasingAnimation(void)
{
	SubCanvasClassPtr tempScptr;

	int nextFocusSubcanvasID = mFocusedCanvas;

	tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + mFocusedCanvas;

	if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_H )
	{
		nextFocusSubcanvasID = mFocusedCanvas - ((tempScptr->mOffsetX + (tempScptr->mOffsetX > 0 ?this->mwidth/2:-this->mwidth/2)) / this->mwidth);
	}
	else if (this->animationType == SUBCANVAS_ANIMATION_SWIPE_V )
	{
		nextFocusSubcanvasID = mFocusedCanvas - ((tempScptr->mOffsetY + (tempScptr->mOffsetY > 0 ? this->mheight/2 : -this->mheight/2)) / this->mheight);
	}

	if (nextFocusSubcanvasID < 0)
	{
		nextFocusSubcanvasID = 0;
	}
	if (nextFocusSubcanvasID >= mNumOfSubCanvas)
	{
		nextFocusSubcanvasID = mNumOfSubCanvas - 1;
	}

	this->subcanvasAnimationDuration = this->constSubcanvasAnimationDuration;
	TagPtr->getTag(this->bindTagID).setValue(nextFocusSubcanvasID,this->bindTagID);
}

u8 CanvasClass::calEdgesPressed(s16 x, s16 y)
{
	u8 result = 0;
	if(y >= 0 && y <= (s16)(this->mheight >> 3))
	{
		if((this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN && this->mFocusedCanvas == (this->mNumOfSubCanvas - 1)) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_UP && this->mFocusedCanvas == 0) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT) || (this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT))
		{
			result = 0;
		}
		else
		{
			//in top edge
			result |= 1;
		}
	}
	if(y >= (s16)(this->mheight - (this->mheight >> 3)) && y <= (s16)(this->mheight))
	{
		if((this->animationType == SUBCANVAS_ANIMATION_PULL_UP && this->mFocusedCanvas == (this->mNumOfSubCanvas - 1)) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN && this->mFocusedCanvas == 0) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT) || (this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT))
		{
			result = 0;
		}
		else
		{
			//in bottom edge
			result |= 1 << 1;
		}
	}
	if(x >= 0 && x <= (s16)(this->mwidth >> 3))
	{
		if((this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT && this->mFocusedCanvas == (this->mNumOfSubCanvas - 1)) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT && this->mFocusedCanvas == 0) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_UP) || (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN))
		{
			result = 0;
		}
		else
		{
			//in left edge
			result |= 1 << 2;
		}
	}
	if(x >= (s16)(this->mwidth - (this->mwidth >> 3)) && x <= (s16)(this->mwidth))
	{
		if((this->animationType == SUBCANVAS_ANIMATION_PULL_LEFT && this->mFocusedCanvas == (this->mNumOfSubCanvas - 1)) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT && this->mFocusedCanvas == 0) ||
		   (this->animationType == SUBCANVAS_ANIMATION_PULL_UP) || (this->animationType == SUBCANVAS_ANIMATION_PULL_DOWN))
		{
			result = 0;
		}
		else
		{
			//in right edge
			result |= 1 << 3;
		}
	}
	return result;
}

void CanvasClass::updateSubCanvasListOffset(s16 offset, u8 direction, u8 isReset)
{
	int i = 0;
	SubCanvasClassPtr tempScptr;
	SubCanvasClassPtr currentScptr;

	currentScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + this->mFocusedCanvas;

	//update current sc offset
	if (direction)
	{
		//vertical
		currentScptr->mOffsetX = 0;
		currentScptr->mOffsetY += offset;
		if((currentScptr->mOffsetY > ((this->mNumOfSubCanvas - 1) * this->mheight)) || (currentScptr->mOffsetY < -((this->mNumOfSubCanvas - 1) * this->mheight)))
		{
			this->ReleasingSpeed = 0;
		}
	}else
	{
		currentScptr->mOffsetX += offset;
		currentScptr->mOffsetY = 0;
		if((currentScptr->mOffsetX > ((this->mNumOfSubCanvas - 1) * this->mwidth)) || (currentScptr->mOffsetX < -((this->mNumOfSubCanvas - 1) * this->mwidth)))
		{
			this->ReleasingSpeed = 0;
		}
	}

	for (i = 0; i < this->mNumOfSubCanvas; i++)
	{
		tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + i;

		if(i!=this->mFocusedCanvas)
		{
			//update other sc offset
			if (direction)
			{
				//vertical
				tempScptr->mOffsetX = 0;
				tempScptr->mOffsetY = ((i-this->mFocusedCanvas) * (s32)this->mheight ) + currentScptr->mOffsetY;
				if((tempScptr->mOffsetY > ((this->mNumOfSubCanvas - 1) * this->mheight)) || (tempScptr->mOffsetY < -((this->mNumOfSubCanvas - 1) * this->mheight)))
				{
					this->ReleasingSpeed = 0;
				}
			}else
			{
				tempScptr->mOffsetX = ((i-this->mFocusedCanvas) * (s32)this->mwidth ) + currentScptr->mOffsetX;
				tempScptr->mOffsetY = 0;
				if((tempScptr->mOffsetX > ((this->mNumOfSubCanvas - 1) * this->mwidth)) || (tempScptr->mOffsetX < -((this->mNumOfSubCanvas - 1) * this->mwidth)))
				{
					this->ReleasingSpeed = 0;
				}
			}
		}

		if (tempScptr->mOffsetX + tempScptr->mWidth < 0 || tempScptr->mOffsetX > this->mwidth || tempScptr->mOffsetY + tempScptr->mHeight < 0 || tempScptr->mOffsetY > this->mheight)
		{
			//out of canvas rectangle, no need to draw
			tempScptr->shouldShow = 0;
		}else
		{
			//#if defined(EMBEDDED) && defined(VERSION_2)
			//EnterSafeSectionForTag();
			//tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));
			//ExitSafeSectionForTag();
			//#else
			tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));
			//#endif
			if (isReset)
			{
				tempScptr->shouldShow = 0;
			}else
			{
				tempScptr->shouldShow = 1;
			}
		}
	}
}

void CanvasClass::updateSubCanvasOffset(s16 offsetX, s16 offsetY)
{
	SubCanvasClassPtr currentScptr;

	currentScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + this->mStartAddrOfSubCanvas + this->mFocusedCanvas;

	if(((currentScptr->contentOffsetX + offsetX) > 0) || ((currentScptr->contentOffsetX + offsetX) < (this->mwidth - currentScptr->contentWidth)))
	{
		if(this->animationType == SUBCANVAS_ANIMATION_SWIPE_H)
		{
			this->CanvasState = 1;
		}
	}
	else
	{
		currentScptr->contentOffsetX += offsetX;
	}

	if(((currentScptr->contentOffsetY + offsetY) > 0) || ((currentScptr->contentOffsetY + offsetY) < (this->mheight - currentScptr->contentHeight)))
	{
		if(this->animationType == SUBCANVAS_ANIMATION_SWIPE_V)
		{
			this->CanvasState = 1;
		}
	}
	else
	{
		currentScptr->contentOffsetY += offsetY;
	}

	////update current sc offset
	//if (direction)
	//{
	//	//vertical
	//	currentScptr->mOffsetX = 0;
	//	if(((currentScptr->mOffsetY + offset) > 0) || ((currentScptr->mOffsetY + offset) < (this->mheight - currentScptr->contentHeight)))
	//	{
	//		return;
	//	}
	//	currentScptr->mOffsetY += offset;
	//}else
	//{
	//	if(((currentScptr->mOffsetX + offset) > 0) || ((currentScptr->mOffsetX + offset) < (this->mwidth - currentScptr->contentWidth)))
	//	{
	//		return;
	//	}
	//	currentScptr->mOffsetX += offset;
	//	currentScptr->mOffsetY = 0;	
	//}
	//#if defined(EMBEDDED) && defined(VERSION_2)
	//EnterSafeSectionForTag();
	//currentScptr->CalculateMatrix(&(currentScptr->mSubCanvasMatrix));
	//ExitSafeSectionForTag();
	//#else
	currentScptr->CalculateMatrix(&(currentScptr->mSubCanvasMatrix));
	//#endif
}

//-----------------------------
// name: computeCanvasMatrix
// compute the matrix of canvas
// When page exists scaling,the offset of canvas relative to page should be multiplied by the scaling factor of page.
// Finally, the offset of canvas is offset relative to moffset.
//  
//     @param1 matrixClassPtr pPageMatrix pointer
//     @param2 matrixClassPtr curCanvasMatrix pointer
// addtional comments :
//   this matrix is inverse matrix
//-----------------------------
funcStatus CanvasClass::computeCanvasMatrix(NewMatrixPtr pPageMatrix, NewMatrixPtr curCanvasMatrix)
{
//	curCanvasMatrix->A = (s32)( ((long long)pPageMatrix->A * curCanvasMatrix->A + (long long)pPageMatrix->B * curCanvasMatrix->C  ) / (1024*1024) );
//	curCanvasMatrix->B = (s32)( ((long long)pPageMatrix->A * curCanvasMatrix->B + (long long)pPageMatrix->B * curCanvasMatrix->D  ) / (1024*1024)  );
//	curCanvasMatrix->C = (s32)( ((long long)pPageMatrix->C * curCanvasMatrix->A + (long long)pPageMatrix->D * curCanvasMatrix->C  ) / (1024*1024)  );
//	curCanvasMatrix->D = (s32)( ((long long)pPageMatrix->C * curCanvasMatrix->B + (long long)pPageMatrix->D * curCanvasMatrix->D  ) / (1024*1024)  );
    curCanvasMatrix->A = (s32)( ((s32)(pPageMatrix->A / 1024) * (curCanvasMatrix->A / 1024) + (s32)(pPageMatrix->B / 1024) * (curCanvasMatrix->C / 1024)));
	curCanvasMatrix->B = (s32)( ((s32)(pPageMatrix->A / 1024) * (curCanvasMatrix->B / 1024) + (s32)(pPageMatrix->B / 1024) * (curCanvasMatrix->D / 1024)));
	curCanvasMatrix->C = (s32)( ((s32)(pPageMatrix->C / 1024) * (curCanvasMatrix->A / 1024) + (s32)(pPageMatrix->D / 1024) * (curCanvasMatrix->C / 1024)));
	curCanvasMatrix->D = (s32)( ((s32)(pPageMatrix->C / 1024) * (curCanvasMatrix->B / 1024) + (s32)(pPageMatrix->D / 1024) * (curCanvasMatrix->D / 1024)));
	curCanvasMatrix->E = pPageMatrix->E + ( ((long long)(curCanvasMatrix->E - (this->moffsetX << 13) /*1.18.13*/) <<20 ) / (pPageMatrix->A)) + (this->moffsetX << 13); //?ú????×?±ê?μ??￡?canvas?à??óú±?éíμ???ò?
	curCanvasMatrix->F = pPageMatrix->F + ( ((long long)(curCanvasMatrix->F - (this->moffsetY << 13) /*1.18.13*/) <<20 ) / (pPageMatrix->A)) + (this->moffsetY << 13); 
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:computeCanvasMatrix
// Calculate the matrix parameters from coordinate parameters
//  
//     @param1 pCanvasMatrix Canvas Matrix pointer
// addtional comments :
//-----------------------------
funcStatus CanvasClass::CalculateMatrix(NewMatrix* pCanvasMatrix)
{
	s32 tempRotateCenterX = 0;
	s32 tempRotateCenterY = 0;
	s32 tempScaleCenterX = 0;
	s32 tempScaleCenterY = 0;
	s32 tempOffsetX = 0;
	s32 tempOffsetY = 0;

	pCanvasMatrix->NewMatrix_init();
	pCanvasMatrix->NewMatrixInv_init();

	tempScaleCenterX = this->mScaleCenterX;
	tempScaleCenterY = this->mScaleCenterY;

	if(((s16)(this->mZoomX) > 0) && ((s16)(this->mZoomY) > 0))
	{

		pCanvasMatrix->translate( -(tempScaleCenterX << 13), -(tempScaleCenterY << 13));
		pCanvasMatrix->scale(this->mZoomX,this->mZoomY);
		pCanvasMatrix->translate( (tempScaleCenterX << 13), (tempScaleCenterY << 13));
		

		if((s16)(this->mZoomX) < 0)
		{
			tempOffsetX = this->mZoomX;
			tempOffsetX *= this->mwidth;
			tempOffsetX /= 512; 
			tempOffsetX = -1 - tempOffsetX;
		}
		if((s16)(this->mZoomY) < 0)
		{
			tempOffsetY = this->mZoomY;
			tempOffsetY *= this->mheight;
			tempOffsetY /= 512; 
			tempOffsetY = -1 - tempOffsetY;
		}

		tempOffsetX += this->moffsetX;
		tempOffsetY += this->moffsetY;

		if(this->mRotateAngle != 0)
		{
			tempRotateCenterX = this->mRotateCenterX;
			//if((this->mZoomX & 0x8000) != 0)
			//{
			//	tempRotateCenterX *= (1 << 9);
			//}
			//else
			//{
			//	tempRotateCenterX *= this->mZoomX;
			//}
			//tempRotateCenterX  /=  (1 << 9);

			tempRotateCenterY = this->mRotateCenterY;
			//if((this->mZoomY & 0x8000) != 0)
			//{
			//	tempRotateCenterY *= (1 << 9);
			//}
			//else
			//{
			//	tempRotateCenterY *= this->mZoomY;
			//}
			//tempRotateCenterY  /= (1 << 9);

			pCanvasMatrix->translate( -(tempRotateCenterX << 13), -(tempRotateCenterY << 13));
			pCanvasMatrix->rotate(this->mRotateAngle);
			pCanvasMatrix->translate( (tempRotateCenterX << 13), (tempRotateCenterY << 13));
		}

		if((this->mProjectionX != 0) || (this->mProjectionY != 0))
		{
			this->mProjectionX &= 0xFFF0;
			this->mProjectionY &= 0xFFF0;
			pCanvasMatrix->project(this->mProjectionX, this->mProjectionY, this->mwidth*(s16)(this->mZoomX)/512, this->mheight*(s16)(this->mZoomY)/512);
		}

		pCanvasMatrix->translate(tempOffsetX << 13, tempOffsetY << 13);
	}
	else
	{
		if(this->mRotateAngle != 0)
		{
			tempRotateCenterX = this->mRotateCenterX;
			if((this->mZoomX & 0x8000) != 0)
			{
				tempRotateCenterX *= (1 << 9);
			}
			else
			{
				tempRotateCenterX *= this->mZoomX;
			}
			tempRotateCenterX  /=  (1 << 9);

			tempRotateCenterY = this->mRotateCenterY;
			if((this->mZoomY & 0x8000) != 0)
			{
				tempRotateCenterY *= (1 << 9);
			}
			else
			{
				tempRotateCenterY *= this->mZoomY;
			}
			tempRotateCenterY  /= (1 << 9);

			pCanvasMatrix->translate( -(tempRotateCenterX << 13), -(tempRotateCenterY << 13));
			if(((s16)(this->mZoomX) < 0) && ((s16)(this->mZoomY) < 0))
			{
				pCanvasMatrix->rotate(this->mRotateAngle);
			}
			else
			{
				pCanvasMatrix->rotate(360 * 16 - this->mRotateAngle);
			}
			pCanvasMatrix->translate( (tempRotateCenterX << 13), (tempRotateCenterX << 13));
		}

		if((this->mProjectionX != 0) || (this->mProjectionY != 0))
		{
			this->mProjectionX &= 0xFFF0;
			this->mProjectionY &= 0xFFF0;
			pCanvasMatrix->project(this->mProjectionX, this->mProjectionY, this->mwidth, this->mheight);
		}

		pCanvasMatrix->translate( -(tempScaleCenterX << 13), -(tempScaleCenterY << 13));
		pCanvasMatrix->scale(this->mZoomX,this->mZoomY);
		pCanvasMatrix->translate( (tempScaleCenterX << 13), (tempScaleCenterY << 13));

		if((s16)(this->mZoomX) < 0)
		{
			tempOffsetX = this->mZoomX;
			tempOffsetX *= this->mwidth;
			tempOffsetX /= 512; 
			tempOffsetX = -1 - tempOffsetX;
		}
		if((s16)(this->mZoomY) < 0)
		{
			tempOffsetY = this->mZoomY;
			tempOffsetY *= this->mheight;
			tempOffsetY /= 512; 
			tempOffsetY = -1 - tempOffsetY;
		}

		tempOffsetX += this->moffsetX;
		tempOffsetY += this->moffsetY;

		pCanvasMatrix->translate(tempOffsetX << 13, tempOffsetY << 13);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name：  CalculateROIPoint
// compute canvas tile final point
// param list：
//	@param1 NewMatrix* curCanvasMatrix，	//texture matrix
//	@param1 PointClass* p0，				//left top point
//	@param1 PointClass* p1，				//right top point
//	@param1 PointClass* p2，				//right bottom point
//	@param1 PointClass* p3，				//left bottom point
// Remarks(Revision between versions):
//	compute the canvas's ROI point
//    added by yn 20201104
//-----------------------------
funcStatus CanvasClass::CalculateROIPoint(NewMatrix* curCanvasMatrix, PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3)
{
#ifdef UseFloatMatrix
	long long x0,y0,x1,y1,x2,y2,x3,y3,z0,z1,z2,z3;
	long long x_min, x_max, y_min, y_max;
	z0 = float_To_4(_20_To_float(curCanvasMatrix->C) * _4_To_float(p0->mPointX) + _20_To_float(curCanvasMatrix->F) * _4_To_float(p0->mPointY)+ _13_To_float(curCanvasMatrix->I) * 1);
	z1 = float_To_4(_20_To_float(curCanvasMatrix->C) * _4_To_float(p1->mPointX) + _20_To_float(curCanvasMatrix->F) * _4_To_float(p1->mPointY)+ _13_To_float(curCanvasMatrix->I) * 1);
	z2 = float_To_4(_20_To_float(curCanvasMatrix->C) * _4_To_float(p2->mPointX) + _20_To_float(curCanvasMatrix->F) * _4_To_float(p2->mPointY)+ _13_To_float(curCanvasMatrix->I) * 1);
	z3 = float_To_4(_20_To_float(curCanvasMatrix->C) * _4_To_float(p3->mPointX) + _20_To_float(curCanvasMatrix->F) * _4_To_float(p3->mPointY)+ _13_To_float(curCanvasMatrix->I) * 1);
	x0 = float_To_4(_20_To_float(curCanvasMatrix->A) * _4_To_float(p0->mPointX) + _20_To_float(curCanvasMatrix->D) * _4_To_float(p0->mPointY)+ _13_To_float(curCanvasMatrix->G) * 1);
	y0 = float_To_4(_20_To_float(curCanvasMatrix->B) * _4_To_float(p0->mPointX) + _20_To_float(curCanvasMatrix->E) * _4_To_float(p0->mPointY)+ _13_To_float(curCanvasMatrix->H) * 1);
	x1 = float_To_4(_20_To_float(curCanvasMatrix->A) * _4_To_float(p1->mPointX) + _20_To_float(curCanvasMatrix->D) * _4_To_float(p1->mPointY)+ _13_To_float(curCanvasMatrix->G) * 1);
	y1 = float_To_4(_20_To_float(curCanvasMatrix->B) * _4_To_float(p1->mPointX) + _20_To_float(curCanvasMatrix->E) * _4_To_float(p1->mPointY)+ _13_To_float(curCanvasMatrix->H) * 1);
	x2 = float_To_4(_20_To_float(curCanvasMatrix->A) * _4_To_float(p2->mPointX) + _20_To_float(curCanvasMatrix->D) * _4_To_float(p2->mPointY)+ _13_To_float(curCanvasMatrix->G) * 1);
	y2 = float_To_4(_20_To_float(curCanvasMatrix->B) * _4_To_float(p2->mPointX) + _20_To_float(curCanvasMatrix->E) * _4_To_float(p2->mPointY)+ _13_To_float(curCanvasMatrix->H) * 1);
	x3 = float_To_4(_20_To_float(curCanvasMatrix->A) * _4_To_float(p3->mPointX) + _20_To_float(curCanvasMatrix->D) * _4_To_float(p3->mPointY)+ _13_To_float(curCanvasMatrix->H) * 1);
	y3 = float_To_4(_20_To_float(curCanvasMatrix->B) * _4_To_float(p3->mPointX) + _20_To_float(curCanvasMatrix->E) * _4_To_float(p3->mPointY)+ _13_To_float(curCanvasMatrix->H) * 1);
#else
	int x0,y0,x1,y1,x2,y2,x3,y3,z0,z1,z2,z3;
	#if (defined VERSION_2) && (defined PC_SIM) 
	z0 = (((long long)(curCanvasMatrix->C) * (p0->mPointX)) + ((long long)(curCanvasMatrix->F) * (p0->mPointY)) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
	z1 = (((long long)(curCanvasMatrix->C) * (p1->mPointX)) + ((long long)(curCanvasMatrix->F) * (p1->mPointY)) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
	z2 = (((long long)(curCanvasMatrix->C) * (p2->mPointX)) + ((long long)(curCanvasMatrix->F) * (p2->mPointY)) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
	z3 = (((long long)(curCanvasMatrix->C) * (p3->mPointX)) + ((long long)(curCanvasMatrix->F) * (p3->mPointY)) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
	#endif
	#if (defined PC_SIM) 
	x0 = (((long long)(curCanvasMatrix->A) * (p0->mPointX)) + ((long long)(curCanvasMatrix->D) * (p0->mPointY)) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
	y0 = (((long long)(curCanvasMatrix->B) * (p0->mPointX)) + ((long long)(curCanvasMatrix->E) * (p0->mPointY)) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
	x1 = (((long long)(curCanvasMatrix->A) * (p1->mPointX)) + ((long long)(curCanvasMatrix->D) * (p1->mPointY)) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
	y1 = (((long long)(curCanvasMatrix->B) * (p1->mPointX)) + ((long long)(curCanvasMatrix->E) * (p1->mPointY)) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
	x2 = (((long long)(curCanvasMatrix->A) * (p2->mPointX)) + ((long long)(curCanvasMatrix->D) * (p2->mPointY)) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
	y2 = (((long long)(curCanvasMatrix->B) * (p2->mPointX)) + ((long long)(curCanvasMatrix->E) * (p2->mPointY)) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
	x3 = (((long long)(curCanvasMatrix->A) * (p3->mPointX)) + ((long long)(curCanvasMatrix->D) * (p3->mPointY)) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
	y3 = (((long long)(curCanvasMatrix->B) * (p3->mPointX)) + ((long long)(curCanvasMatrix->E) * (p3->mPointY)) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
#endif
#if (defined VERSION_2) && (defined EMBEDDED)    
	z0 = (csi_fix_mult((curCanvasMatrix->C), (p0->mPointX), 0) + csi_fix_mult((curCanvasMatrix->F), (p0->mPointY), 0) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
	z1 = (csi_fix_mult((curCanvasMatrix->C), (p1->mPointX), 0) + csi_fix_mult((curCanvasMatrix->F), (p1->mPointY), 0) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
	z2 = (csi_fix_mult((curCanvasMatrix->C), (p2->mPointX), 0) + csi_fix_mult((curCanvasMatrix->F), (p2->mPointY), 0) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
	z3 = (csi_fix_mult((curCanvasMatrix->C), (p3->mPointX), 0) + csi_fix_mult((curCanvasMatrix->F), (p3->mPointY), 0) + ((long long)(curCanvasMatrix->I) << 11)) >> 12;
			
    x0 = (csi_fix_mult((curCanvasMatrix->A), (p0->mPointX), 0) + csi_fix_mult((curCanvasMatrix->D), (p0->mPointY), 0) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
    y0 = (csi_fix_mult((curCanvasMatrix->B), (p0->mPointX), 0) + csi_fix_mult((curCanvasMatrix->E), (p0->mPointY), 0) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
    x1 = (csi_fix_mult((curCanvasMatrix->A), (p1->mPointX), 0) + csi_fix_mult((curCanvasMatrix->D), (p1->mPointY), 0) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
    y1 = (csi_fix_mult((curCanvasMatrix->B), (p1->mPointX), 0) + csi_fix_mult((curCanvasMatrix->E), (p1->mPointY), 0) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
    x2 = (csi_fix_mult((curCanvasMatrix->A), (p2->mPointX), 0) + csi_fix_mult((curCanvasMatrix->D), (p2->mPointY), 0) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
    y2 = (csi_fix_mult((curCanvasMatrix->B), (p2->mPointX), 0) + csi_fix_mult((curCanvasMatrix->E), (p2->mPointY), 0) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
    x3 = (csi_fix_mult((curCanvasMatrix->A), (p3->mPointX), 0) + csi_fix_mult((curCanvasMatrix->D), (p3->mPointY), 0) + ((long long)(curCanvasMatrix->G) << 11)) >> 20;
    y3 = (csi_fix_mult((curCanvasMatrix->B), (p3->mPointX), 0) + csi_fix_mult((curCanvasMatrix->E), (p3->mPointY), 0) + ((long long)(curCanvasMatrix->H) << 11)) >> 20;
#endif
#endif

	#ifdef VERSION_2
	if(z0 != 1 << 12)
	{
		if(z0 == 0)
			z0 = 1;

		x0 = (x0 << 12)/ z0;
		y0 = (y0 << 12)/ z0;
	}
	if(z1 != 1 << 12)
	{
		if(z1 == 0)
			z1 = 1;

		x1 = (x1 << 12)/ z1;
		y1 = (y1 << 12)/ z1;
	}
	if(z2 != 1 << 12)
	{
		if(z2 == 0)
			z2 = 1;

		x2 = (x2 << 12)/ z2;
		y2 = (y2 << 12)/ z2;
	}
	if(z3 != 1 << 12)
	{
		if(z3 == 0)
			z3 = 1;

		x3 = (x3 << 12)/ z3;
		y3 = (y3 << 12)/ z3;
	}
	#endif

	//Recompute the point when it's overlapped
	if(x0 <= -2048 * 16)
	{
		x0 = -2048 * 16 + 1;
	}
	if(x0 >= 2048 * 16)
	{
		x0 = 2048 * 16 - 1;
	}
	if(y0 <= -2048 * 16)
	{
		y0 = -2048 * 16 + 1;
	}
	if(y0 >= 2048 * 16)
	{
		y0 = 2048 * 16 - 1;
	}

	if(x1 <= -2048 * 16)
	{
		x1 = -2048 * 16 + 1;
	}
	if(x1 >= 2048 * 16)
	{
		x1 = 2048 * 16 - 1;
	}
	if(y1 <= -2048 * 16)
	{
		y1 = -2048 * 16 + 1;
	}
	if(y1 >= 2048 * 16)
	{
		y1 = 2048 * 16 - 1;
	}

	if(x2 <= -2048 * 16)
	{
		x2 = -2048 * 16 + 1;
	}
	if(x2 >= 2048 * 16)
	{
		x2 = 2048 * 16 - 1;
	}
	if(y2 <= -2048 * 16)
	{
		y2 = -2048 * 16 + 1;
	}
	if(y2 >= 2048 * 16)
	{
		y2 = 2048 * 16 - 1;
	}

	if(x3 <= -2048 * 16)
	{
		x3 = -2048 * 16 + 1;
	}
	if(x3 >= 2048 * 16)
	{
		x3 = 2048 * 16 - 1;
	}
	if(y3 <= -2048 * 16)
	{
		y3 = -2048 * 16 + 1;
	}
	if(y3 >= 2048 * 16)
	{
		y3 = 2048 * 16 - 1;
	}

	p0->mPointX = x0;
	p0->mPointY = y0;
	p1->mPointX = x1;
	p1->mPointY = y1;
	p2->mPointX = x2;
	p2->mPointY = y2;
	p3->mPointX = x3;
	p3->mPointY = y3;

	return AHMI_FUNC_SUCCESS;
}

void CanvasClass::SetCanvasMatrixRefreshFlag(void)
{
	DynamicPageClassPtr pCurrentPage;
	SubCanvasClassPtr scptr;

//	taskENTER_CRITICAL();
	pCurrentPage = &gPagePtr[WorkingPageID];
	for (int i = 0; i < this->mNumOfSubCanvas; i++)
	{
		scptr = (SubCanvasClassPtr)(pCurrentPage->pSubCanvasList) + this->mStartAddrOfSubCanvas + i;

		if (scptr->shouldShow || i == this->mFocusedCanvas)
		{
			for (int WidgetCount = 0; WidgetCount < scptr->NumOfWidget; WidgetCount++)
			{
				WidgetClassPtr wptr = &(gPagePtr[WorkingPageID].pWidgetList[ (scptr->StartAddrOfWidget + WidgetCount)]);
				//Determine whether widget is inside canvas.
				if( 
					(s16)(wptr->WidgetOffsetX) > (s16)(this->mwidth) ||//大于右边界 
					( (s16)(wptr->WidgetOffsetX) + (wptr->WidgetWidth) ) < 0 || //小于左边界
					(s16)(wptr->WidgetOffsetY) > (s16)(this->mheight)  || //大于下边界
					( (s16)(wptr->WidgetOffsetY) + (wptr->WidgetHeight) ) < 0
				    )
					continue;//If it is not inside, it will not be drawn

				//Value refresh is not performed during animation or transformation for widget CHART
				if(wptr->WidgetType != CHART)
				{
					wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
				}
				wptr->SetWidgetMatrixRefreshFlag();
			}
		}
	}
	this->mRefreshType |= CanvasMatrixRefreshFlag;
//	taskEXIT_CRITICAL();
}
#endif
