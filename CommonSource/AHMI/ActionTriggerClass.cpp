////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2015/11/17
// File Name:     ActionTriggerClass.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.04 - C++, 20160321 by Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "ButtonClass.h"
#include "ClockClass.h"
#include "DynamicTexClass.h"
#include "IOClass.h"
#include "KnobClass.h"
#include "MeterClass.h"
#include "NumberClass.h"
#include "OnOffTexture.h"
#include "OscilloscopeClass.h"
#include "ProgBarClass.h"
#include "SliderClass.h"
#include "TextureNumberClass.h"
#include "TextClass.h"
#include "VideoClass.h"
#include "TextureTimeClass.h"
#include "TouchTrack.h"
#include "AlphaSlide.h"
#include "TextInput.h"
#include "AlphaImg.h"
#include "ButtonSwitch.h"
#include "GalleryClass.h"
#include "Chart.h"
#include "QRcodeClass.h"
#include "SelectorClass.h"
#include "ClockBoardClass.h"
#include "SwipeResponderClass.h"
#include "CombinationSwitch.h"
#include "MenuContainer.h"
#include "CircleSliderClass.h"
#include "DataImage.h"
#include "BarChart.h"
#include "PieChart.h"
#include "ListClass.h"
#include "ThreeDimensionalClass.h"
#include "Custom.h"
#ifdef AHMI_DEBUG
#include "trace.h"
#endif

#if defined(EMBEDDED) && defined(VERSION_2)
#include "gc90x_initial.h"
#include "gc90x_flash.h"
#include "AHMIInterface.h"
#include "csi_ahmi_hw.h"
#endif

#ifdef AHMI_CORE

extern ActionTriggerClass gWidgetTagTrigger;
extern ActionTriggerClass gMouseTagTrigger;
extern ActionTriggerClass gCanvasTagTrigger;

extern u16            WorkingPageID;
extern DynamicPageClassPtr gPagePtr;
extern QueueHandle_t   ActionInstructionQueue;
extern TagClassPtr     TagPtr;

extern ConfigInfoClass		ConfigData;
extern MenuState*			gMenuState;
extern u8 animationDuration;

extern ProjectScreenOptions gScreenOptions;

extern uint8_t PAGE_ANIMATION_MULTI_CARD_Status;
extern u8 windowPressed;
u8 force_cancel_subcanvas_page_mousetouch = 0;// 1 for cancel; 0 for don't cancel

ActionTriggerClass::ActionTriggerClass()
{}

ActionTriggerClass::~ActionTriggerClass()
{}

//-----------------------------
// function: initActionTrigger
// @brief: 
// parameters:
//-----------------------------
funcStatus ActionTriggerClass::initActionTrigger(){
	mInputType = 0;
	mTagPtr = NULL;
	return AHMI_FUNC_SUCCESS;
}

void translateMousePosWithPageScreenArea(PIDPoint * pos, DynamicPageClassPtr pagePtr, RefreshScreenTypes screenType)
{
	if (gScreenOptions.enable)
	{
		switch (screenType)
		{
		case RefreshScreenMain:
			pos->x += pagePtr->screenAreas.mainOffsetX;
			pos->y += pagePtr->screenAreas.mainOffsetY;
			break;
		case RefreshScreenSub:
			pos->x += pagePtr->screenAreas.subOffsetX;
			pos->y += pagePtr->screenAreas.subOffsetY;
			break;
		default:
			break;
		}
	}
}

#if defined(CHIP_GC9002)
#include "csi_ahmi.h"
extern void SetPartialDrawRefreMsg(RefreshMsg *msg);
#endif
//-----------------------------
// function: MouseTouch
// @brief: According the mouse/touchscreen event, decide to trigger which element.
// parameters:
// @param1       DynamicPagePtr    Ptr                      pointer to current page
// @param2		 ActionTriggerClass*  TagTrigger            pointer to mouse/touchscreen event
//-----------------------------
#if defined(EMBEDDED) && defined(VERSION_2)
extern uint32_t TouchCount;
#endif
u32 ActionTriggerClass::MouseTouch(PIDPoint* pPressPoint,PIDPoint* pReleasePoint,  RefreshScreenTypes screenType)
{
	u16 i,j;
	s16 X, tempX, difX;
	s16 Y, tempY, difY;
	s16 releaseX, releaseY;
	PointClass point(0, 0);
	PointClass pointCanvas(0, 0);
	PointClass pointWidget(0, 0);
	PIDPoint   tempPressPoint;
	PIDPoint   tempReleasePoint;
	DynamicPageClassPtr curPage = &gPagePtr[WorkingPageID];
	CanvasClassPtr curCanvas;
	SubCanvasClassPtr curSubCan;
	WidgetClassPtr curWidget;
	u32 ahmiStatus = WIDGET_TOUCH_CANCEL, CanvasStatus = WIDGET_TOUCH_CANCEL;
    u8 WidgetTouchFlag = 0;

	translateMousePosWithPageScreenArea(pPressPoint, curPage, screenType);
	translateMousePosWithPageScreenArea(pReleasePoint, curPage, screenType);
	
    /* Check the type of mouse/touchscreen event. */
	if (this->mInputType == ACTION_MOUSE_PRESS || this->mInputType == ACTION_MOUSE_HOLDING)
    {
        /* The type of mouse/touchscreen event is press. */
		if(this->mInputType == ACTION_MOUSE_PRESS)
		{
			force_cancel_subcanvas_page_mousetouch = 0;
			X = ((s16)(pPressPoint->x));
			Y = ((s16)(pPressPoint->y));
            this->mMousePID.x = pPressPoint->x;
            this->mMousePID.y = pPressPoint->y;
		}
        /* The type of mouse/touchscreen event is holding. */
		else if(this->mInputType == ACTION_MOUSE_HOLDING)
		{
			X = (s16)(pReleasePoint->x);
			Y = (s16)(pReleasePoint->y);
            this->mMousePID.x = pReleasePoint->x;
            this->mMousePID.y = pReleasePoint->y;
		}
		
		//if(windowPressed == 0 && animationDuration == 0)
		if(windowPressed == 0 && (PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Idle || PAGE_ANIMATION_MULTI_CARD_Status == PAGE_ANIMATION_MULTI_CARD_Init))
		{
			/* Seek the touch point on which canvas. */
			for(i = 0; i < curPage->mNumOfCanvas; i++)
			{
				curCanvas = &(curPage->pCanvasList[curPage->mNumOfCanvas - 1 - i]);

				point.mPointX = X * 16;
				point.mPointY = Y * 16;
				point.leftMulMatrix(&(curCanvas->mCanvasMatrix));
				tempX = point.mPointX / 16;
				tempY = point.mPointY / 16;
				pointCanvas.mPointX = point.mPointX;
				pointCanvas.mPointY = point.mPointY;

				/* If the touch point on scroll slider. */
				if(( 0 <= tempX &&
					(s16)(curCanvas->mheight - 5) <= tempY &&
					(s16)(curCanvas->mwidth)  >= tempX &&
					(s16)(curCanvas->mheight) >= tempY 
				) || ( (curCanvas->mwidth - 5) <= tempX &&
					(s16)0 <= tempY &&
					(s16)(curCanvas->mwidth)  >= tempX &&
					(s16)(curCanvas->mheight) >= tempY 
				))
				{
					curSubCan = &(curPage->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);

					if(((curSubCan->contentWidth != curCanvas->mwidth) || (curSubCan->contentHeight != curCanvas->mheight)) && (curSubCan->shouldShowScrollBar == 1))
					{
						/* Seek the touch point on which widget. */
						for(j = curSubCan->StartAddrOfTouchWidget; j < (curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget); j++ )
						{
							curWidget = &curPage->pWidgetList[(curPage->pTouchWidgetList[(curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget) - 1 - (j - curSubCan->StartAddrOfTouchWidget)])];

							pointWidget.mPointX = point.mPointX;
							pointWidget.mPointY = point.mPointY;
							pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));

							tempX = pointWidget.mPointX / 16;
							tempY = pointWidget.mPointY / 16;

							if( 0 <= tempX &&
								0 <= tempY &&
								(s16)(curWidget->WidgetWidth)  >= tempX &&
								(s16)(curWidget->WidgetHeight) >= tempY 
								)
							{
								if((curWidget->WidgetType == BUTTON_TEX) && (curWidget->MENUSTATEID < ConfigData.NumOfMenuStates) && (gMenuState[curWidget->MENUSTATEID].state == 0))
								{
									return WidgetNormalRefreshFlag;
								}

								if(this->mInputType == ACTION_MOUSE_PRESS)
								{
									tempPressPoint.x = tempX;
									tempPressPoint.y = tempY;

									pointWidget.mPointX = pReleasePoint->x * 16;
									pointWidget.mPointY = pReleasePoint->y * 16;
									pointWidget.leftMulMatrix(&(curCanvas->mCanvasMatrix));
									pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));

									tempReleasePoint.x = pointWidget.mPointX / 16;
									tempReleasePoint.y = pointWidget.mPointY / 16;
								}
								else if(this->mInputType == ACTION_MOUSE_HOLDING)
								{
									tempReleasePoint.x = tempX;
									tempReleasePoint.y = tempY;

									pointWidget.mPointX = pPressPoint->x * 16;
									pointWidget.mPointY = pPressPoint->y * 16;
									pointWidget.leftMulMatrix(&(curCanvas->mCanvasMatrix));
									pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));

									tempPressPoint.x = pointWidget.mPointX / 16;
									tempPressPoint.y = pointWidget.mPointY / 16;
								}

								#if defined(EMBEDDED) && defined(VERSION_2)
						        if((this->mInputType == ACTION_MOUSE_PRESS) && (TagPtr->getTag(SYSTEM_ALARM_TAG).getValue() != 0))
								{
									csi_ahmi_StartBuzzerAndTimer();
									TouchCount = 0;
								}
								#endif
						        /* Trigger the widget touch press/holding action. */
								WidgetTouchFlag = 1;
							    /*ahmiStatus = this->widgetMouseTouchActionTrigger(curWidget, pPressPoint, pReleasePoint, 1);*/
								ahmiStatus = this->widgetMouseTouchActionTrigger(curPage,curWidget, &tempPressPoint, &tempReleasePoint, pPressPoint, pReleasePoint, 1);
								if(ahmiStatus == WIDGET_TOUCH_CANCEL)
								{
									#if defined(CHIP_GC9002)	
									if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
									{
										RefreshMsg refreshMsg;
										refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
										refreshMsg.mElementPtr.wptr = curWidget;
										SetPartialDrawRefreMsg(&refreshMsg);
									}
									#endif
									continue;
								}
								else if(ahmiStatus == AHMI_FUNC_SUCCESS)
								{
									#if defined(CHIP_GC9002)	
									if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
									{
										RefreshMsg refreshMsg;
										refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
										refreshMsg.mElementPtr.wptr = curWidget;
										SetPartialDrawRefreMsg(&refreshMsg);
									}
									#endif
									return WidgetNormalRefreshFlag;
								}
								else
								{
									return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetActionTriggerErr);
								}
							}
						}
					}
				}

				/* If the touch point on a special cavans. */
				if( 0 <= tempX &&
					0 <= tempY &&
					(s16)(curCanvas->mwidth)  >= tempX &&
					(s16)(curCanvas->mheight) >= tempY 
				)
				{
					curSubCan = &(curPage->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
					point.leftMulMatrix(&(curSubCan->mSubCanvasMatrix));
					/* Seek the touch point on which widget. */
					for(j = curSubCan->StartAddrOfTouchWidget; j < (curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget); j++ )
					{
						curWidget = &curPage->pWidgetList[(curPage->pTouchWidgetList[(curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget) - 1 - (j - curSubCan->StartAddrOfTouchWidget)])];

						if( 
							(s16)(curWidget->WidgetOffsetX) > (s16)(curSubCan->contentWidth) ||//大于右边界 
							( (s16)(curWidget->WidgetOffsetX) + (curWidget->WidgetWidth) ) < 0 || //小于左边界
							(s16)(curWidget->WidgetOffsetY) > (s16)(curSubCan->contentHeight)  || //大于下边界
							( (s16)(curWidget->WidgetOffsetY) + (curWidget->WidgetHeight) ) < 0 ||
							curWidget->TouchDisable
							)
							continue;//If it is not inside, it will not be trigger

						pointWidget.mPointX = point.mPointX;
						pointWidget.mPointY = point.mPointY;
						pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));

						tempX = pointWidget.mPointX / 16;
						tempY = pointWidget.mPointY / 16;

						if((0 <= tempX &&
							0 <= tempY &&
							(s16)(curWidget->WidgetWidth)  >= tempX &&
							(s16)(curWidget->WidgetHeight) >= tempY 
							) || ((curWidget->WidgetType == SELECTOR) && (this->mInputType == ACTION_MOUSE_HOLDING) && curWidget->PressFlag)
							  || ((curWidget->WidgetType == SLIDER_TEX) && (this->mInputType == ACTION_MOUSE_HOLDING) && curWidget->PressFlag)
							  || ((curWidget->WidgetType == TOUCH_TRACK) && (this->mInputType == ACTION_MOUSE_HOLDING) && curWidget->PressFlag)
							  || ((curWidget->WidgetType == BUTTON_TEX) && (this->mInputType == ACTION_MOUSE_HOLDING) && curWidget->PressFlag))
						{
							if(this->mInputType == ACTION_MOUSE_HOLDING && curWidget->PressFlag == 0)
							{
								continue;
							}
							if((curWidget->WidgetType == BUTTON_TEX) && (curWidget->MENUSTATEID < ConfigData.NumOfMenuStates) && (gMenuState[curWidget->MENUSTATEID].state == 0))
							{
								continue;
							}

							if(this->mInputType == ACTION_MOUSE_PRESS)
							{
								tempPressPoint.x = tempX;
								tempPressPoint.y = tempY;

								pointWidget.mPointX = pPressPoint->x * 16;
								pointWidget.mPointY = pPressPoint->y * 16;
								pointWidget.leftMulMatrix(&(curCanvas->mCanvasMatrix));
								pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));

								tempReleasePoint.x = pointWidget.mPointX / 16;
								tempReleasePoint.y = pointWidget.mPointY / 16;
							}
							else if(this->mInputType == ACTION_MOUSE_HOLDING)
							{
								tempReleasePoint.x = tempX;
								tempReleasePoint.y = tempY;

								pointWidget.mPointX = pPressPoint->x * 16;
								pointWidget.mPointY = pPressPoint->y * 16;
								pointWidget.leftMulMatrix(&(curCanvas->mCanvasMatrix));
								pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));

								tempPressPoint.x = pointWidget.mPointX / 16;
								tempPressPoint.y = pointWidget.mPointY / 16;
							}

							#if defined(EMBEDDED) && defined(VERSION_2)
			                if((this->mInputType == ACTION_MOUSE_PRESS) && (TagPtr->getTag(SYSTEM_ALARM_TAG).getValue() != 0))
							{
								csi_ahmi_StartBuzzerAndTimer();
								TouchCount = 0;
							}
							#endif
			                /* Trigger the widget touch press/holding action. */
						    /*ahmiStatus = this->widgetMouseTouchActionTrigger(curWidget, pPressPoint, pReleasePoint, 1);*/
							WidgetTouchFlag = 1;
							ahmiStatus = this->widgetMouseTouchActionTrigger(curPage,curWidget, &tempPressPoint, &tempReleasePoint, pPressPoint, pReleasePoint, 1);
							if(ahmiStatus == WIDGET_TOUCH_CANCEL)
							{
								#if defined(CHIP_GC9002)	
								if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
								{
									RefreshMsg refreshMsg;
									refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
									refreshMsg.mElementPtr.wptr = curWidget;
									SetPartialDrawRefreMsg(&refreshMsg);
								}
								#endif
								continue;
							}
							else if(ahmiStatus == AHMI_FUNC_SUCCESS)
							{
								#if defined(CHIP_GC9002)	
								if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
								{
									RefreshMsg refreshMsg;
									refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
									refreshMsg.mElementPtr.wptr = curWidget;
									SetPartialDrawRefreMsg(&refreshMsg);
								}
								#endif
								
								return WidgetNormalRefreshFlag;
							}
							else
							{
								return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetActionTriggerErr);
							}
						}
					}

					if(!force_cancel_subcanvas_page_mousetouch)
					{
						if(CanvasStatus == WIDGET_TOUCH_CANCEL)
						{
							//trigger canvas mousetouch
							tempReleasePoint.x = pointCanvas.mPointX/16;
							tempReleasePoint.y = pointCanvas.mPointY/16;

							//point.mPointX = pPressPoint->x * 16;
							//point.mPointY = pPressPoint->y * 16;
							//point.leftMulMatrix(&(curCanvas->mCanvasMatrix));

							tempPressPoint.x = pointCanvas.mPointX/16;
							tempPressPoint.y = pointCanvas.mPointY/16;

							CanvasStatus = curCanvas->mouseTouchCtrl(this, &tempPressPoint, &tempReleasePoint, pPressPoint, pReleasePoint, 1);
							
							#if defined(CHIP_GC9002)	
							if((CanvasStatus == AHMI_FUNC_SUCCESS) && (GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE))
							{
								RefreshMsg refreshMsg;
								refreshMsg.mElementType = ANIMATION_REFRESH_CANVAS;
								refreshMsg.mElementPtr.cptr = curCanvas;
								SetPartialDrawRefreMsg(&refreshMsg);
							}
							#endif
						}
					}
				}
			}
		}

		if(!force_cancel_subcanvas_page_mousetouch)
		{
			if(CanvasStatus == WIDGET_TOUCH_CANCEL)
			{
				tempPressPoint.x = X;
				tempPressPoint.y = Y;
						
				tempReleasePoint.x =  X;
				tempReleasePoint.y = Y;
				CanvasStatus = curPage->mouseTouchCtrl(this, &tempPressPoint, & tempReleasePoint, pPressPoint, pReleasePoint, 0);
				
				#if defined(CHIP_GC9002)
				if((CanvasStatus == AHMI_FUNC_SUCCESS) && (GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE))
				{
					RefreshMsg refreshMsg;
					refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
					refreshMsg.mElementPtr.pageptr = curPage;
					SetPartialDrawRefreMsg(&refreshMsg);
				}
				#endif
			}
		}

		if(WidgetTouchFlag || (CanvasStatus != WIDGET_TOUCH_CANCEL))
		{
			return PageMatrixRefreshFlag;
		}
	}/* end of MOUSE_PRESS. */
    
    /* Check the type of mouse/touchscreen event. */
	else if(this->mInputType == ACTION_MOUSE_RELEASE)
	{
		X = (s16)(pPressPoint->x);
		Y = (s16)(pPressPoint->y);
		releaseX = (s16)(pReleasePoint->x);
		releaseY = (s16)(pReleasePoint->y);
		
		difX = X - releaseX;
		if(difX < 0)
			difX = -difX;
		
		difY = Y - releaseY;
		if(difY < 0)
			difY = -difY;
		
		if(difX < WIDGET_TOUCH_DIFF && difY < WIDGET_TOUCH_DIFF)
		{
			releaseX = X;
			releaseY = Y;
		}
        
		//if(windowPressed == 0 && animationDuration == 0)
		{
			/* Seek the touch point on which canvas. */
			for(i = 0; i < curPage->mNumOfCanvas; i++)
			{
				curCanvas = &(curPage->pCanvasList[curPage->mNumOfCanvas - 1 - i]);

				point.mPointX = X * 16;
				point.mPointY = Y * 16;
				point.leftMulMatrix(&(curCanvas->mCanvasMatrix));
				tempX = point.mPointX / 16;
				tempY = point.mPointY / 16;

			    /* If the touch point on a special cavans. */
				if( 0 <= tempX &&
					0 <= tempY &&
					(s16)(curCanvas->mwidth)  >= tempX &&
					(s16)(curCanvas->mheight) >= tempY 
				)
				{
					curSubCan = &(curPage->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
					point.leftMulMatrix(&(curSubCan->mSubCanvasMatrix));
					/* Seek the touch point on which widget. */
					for(j = curSubCan->StartAddrOfTouchWidget; j < (curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget); j++ )
					{
						curWidget = &curPage->pWidgetList[(curPage->pTouchWidgetList[(curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget) - 1 - (j - curSubCan->StartAddrOfTouchWidget)])];
			            
						if( 
							(s16)(curWidget->WidgetOffsetX) > (s16)(curSubCan->contentWidth) ||//大于右边界 
							( (s16)(curWidget->WidgetOffsetX) + (curWidget->WidgetWidth) ) < 0 || //小于左边界
							(s16)(curWidget->WidgetOffsetY) > (s16)(curSubCan->contentHeight)  || //大于下边界
							( (s16)(curWidget->WidgetOffsetY) + (curWidget->WidgetHeight) ) < 0 ||
							curWidget->TouchDisable
							)
							continue;//If it is not inside, it will not be trigger


						pointWidget.mPointX = point.mPointX;
						pointWidget.mPointY = point.mPointY;
						pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));

						tempX = pointWidget.mPointX / 16;
						tempY = pointWidget.mPointY / 16;

						if(( 0 <= tempX &&
							0 <= tempY &&
							(s16)(curWidget->WidgetWidth)  >= tempX && 
							(s16)(curWidget->WidgetHeight) >= tempY
						) || (curWidget->PressFlag != 0))
						{
							if((curWidget->WidgetType == BUTTON_TEX) && (curWidget->MENUSTATEID < ConfigData.NumOfMenuStates) && (gMenuState[curWidget->MENUSTATEID].state == 0))
							{
								continue;
							}
							tempPressPoint.x = tempX;
							tempPressPoint.y = tempY;

							pointWidget.mPointX = releaseX * 16;
							pointWidget.mPointY = releaseY * 16;
							pointWidget.leftMulMatrix(&(curCanvas->mCanvasMatrix));
							pointWidget.leftMulMatrix(&(curSubCan->mSubCanvasMatrix));
							pointWidget.leftMulMatrix(&(curWidget->mWidgetMatrix));
							tempX = pointWidget.mPointX / 16;
							tempY = pointWidget.mPointY / 16;

							tempReleasePoint.x = tempX;
							tempReleasePoint.y = tempY;

			                /*if the release position is on the widget trigger the release action*/
							if( 0 <= tempX &&
								0 <= tempY &&
								(s16)(curWidget->WidgetWidth ) >= tempX && 
								(s16)(curWidget->WidgetHeight) >= tempY
								)
								this->mInputType = ACTION_MOUSE_RELEASE;
							else /* Else cancle the release trigger.*/
								this->mInputType = ACTION_MOUSE_CANCLE;
			                
			                /* Trigger the widget touch  release action. */
						    /*ahmiStatus = this->widgetMouseTouchActionTrigger(curWidget, pPressPoint, pReleasePoint, 1);*/
							WidgetTouchFlag = 1;
							ahmiStatus = this->widgetMouseTouchActionTrigger(curPage,curWidget, &tempPressPoint, &tempReleasePoint, pPressPoint, pReleasePoint, 1);
							if(ahmiStatus == WIDGET_TOUCH_CANCEL)
							{
								#if defined(CHIP_GC9002)	
								if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
								{
									RefreshMsg refreshMsg;
									refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
									refreshMsg.mElementPtr.wptr = curWidget;
									SetPartialDrawRefreMsg(&refreshMsg);
								}
								#endif
								continue;
							}
							else if(ahmiStatus == AHMI_FUNC_SUCCESS)
							{
								#if defined(CHIP_GC9002)	
								if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
								{
									RefreshMsg refreshMsg;
									refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
									refreshMsg.mElementPtr.wptr = curWidget;
									SetPartialDrawRefreMsg(&refreshMsg);
								}
								#endif
								
								/* clear all touch widget press flag. */
								for(i = 0; i < curPage->mNumOfCanvas; i++)
								{
									curSubCan = &(curPage->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
									for(j = curSubCan->StartAddrOfTouchWidget; j < (curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget); j++ )
									{
										curWidget = &curPage->pWidgetList[(curPage->pTouchWidgetList[(curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget) - 1 - (j - curSubCan->StartAddrOfTouchWidget)])];
										curWidget->PressFlag = 0;
									}
								}
								
								if(windowPressed)
								{
									break;
								}
								else
								{
									return WidgetNormalRefreshFlag;
								}
							}
							else
							{
								/* clear all touch widget press flag. */
								for(i = 0; i < curPage->mNumOfCanvas; i++)
								{
									curSubCan = &(curPage->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
									for(j = curSubCan->StartAddrOfTouchWidget; j < (curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget); j++ )
									{
										curWidget = &curPage->pWidgetList[(curPage->pTouchWidgetList[(curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget) - 1 - (j - curSubCan->StartAddrOfTouchWidget)])];
										curWidget->PressFlag = 0;
									}
								}

								if(windowPressed)
								{
									break;
								}
								else
								{
									return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetActionTriggerErr);
								}
							}
						}
					}

					if(!force_cancel_subcanvas_page_mousetouch)  // 1 for cancel; 0 for don't cancel
					{
						if(CanvasStatus == WIDGET_TOUCH_CANCEL)
						{
							//trigger canvas mousetouch
							tempPressPoint.x = point.mPointX/16;
							tempPressPoint.y = point.mPointY/16;

							pointWidget.mPointX = releaseX * 16;
							pointWidget.mPointY = releaseY * 16;
							pointWidget.leftMulMatrix(&(curCanvas->mCanvasMatrix));
							
							tempReleasePoint.x = pointWidget.mPointX / 16;
							tempReleasePoint.y = pointWidget.mPointY / 16;
							CanvasStatus = curCanvas->mouseTouchCtrl(this,&tempPressPoint, & tempReleasePoint, pPressPoint, pReleasePoint, 1);
							
							#if defined(CHIP_GC9002)	
							if((CanvasStatus == AHMI_FUNC_SUCCESS) && (GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE))
							{
								RefreshMsg refreshMsg;
								refreshMsg.mElementType = ANIMATION_REFRESH_CANVAS;
								refreshMsg.mElementPtr.cptr = curCanvas;
								SetPartialDrawRefreMsg(&refreshMsg);
							}
							#endif
						}
					}

				}
			}

			/* clear all touch widget press flag. */
			for(i = 0; i < curPage->mNumOfCanvas; i++)
			{
				curSubCan = &(curPage->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
				for(j = curSubCan->StartAddrOfTouchWidget; j < (curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget); j++ )
				{
					curWidget = &curPage->pWidgetList[(curPage->pTouchWidgetList[(curSubCan->StartAddrOfTouchWidget) + (curSubCan->NumofTouchWidget) - 1 - (j - curSubCan->StartAddrOfTouchWidget)])];
					curWidget->PressFlag = 0;
				}
			}
		}

		if(!force_cancel_subcanvas_page_mousetouch)
		{
			if(CanvasStatus == WIDGET_TOUCH_CANCEL)
			{
				if(windowPressed == 1)
				{
					CanvasStatus = curPage->mouseTouchCtrl(this, pPressPoint, pReleasePoint, pPressPoint, pReleasePoint, 0);
					
					#if defined(CHIP_GC9002)	
					if((CanvasStatus == AHMI_FUNC_SUCCESS) && (GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE))
					{
						RefreshMsg refreshMsg;
						refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
						refreshMsg.mElementPtr.pageptr = curPage;
						SetPartialDrawRefreMsg(&refreshMsg);
					}
					#endif
				}
			}
		}
		
		if(WidgetTouchFlag || (CanvasStatus != WIDGET_TOUCH_CANCEL))
		{
			return PageMatrixRefreshFlag;
		}
	}/* end of mouse/touchscreen realse. */
	
	return NullRefreshFlag;
}


u32 ActionTriggerClass::triggerWidgetHighlight(WidgetClassPtr widgetClassPtr)
{
	u8 widgetType;
	u8 button_type;
	ButtonClass widget;
	ClockClass clockWidget;
	TextureTimeClass TextureTimeWidget;
	DynamicPageClassPtr curPagePtr = &gPagePtr[WorkingPageID];
	ListClass listWidget;
	if (widgetClassPtr != NULL)
	{
		widgetType = widgetClassPtr->WidgetType;
		if(widgetType == BUTTON_TEX)
		{
			widget.KeyboardTouchCtrl(curPagePtr, widgetClassPtr, this, 0);
		}
		else if(widgetType == CLOCK_TEX)
		{
			clockWidget.KeyboardTouchCtrl(curPagePtr, widgetClassPtr, this, 0);
		}
		else if(widgetType == TEXTURE_TIME)
		{
			TextureTimeWidget.KeyboardTouchCtrl(curPagePtr, widgetClassPtr, this, 0);
		}
		else if(widgetType == LIST)
		{
			listWidget.KeyboardTouchCtrl(curPagePtr, widgetClassPtr, this, 0);
		}
	}
	
	return 1;

	
}

//-----------------------------
// function: keyboardTouch
// @brief: According the keyboard event, decide to trigger which element.
// parameters:
//-----------------------------
u32 ActionTriggerClass::keyboardTouch()
{
	u8 preWidgetID = 0, nextWidgetID = 0;
	u8 preOfPreWidgetID = 0, nextOfNextWidgetID = 0;
	WidgetClassPtr curEnlightedWidgetClassPtr, preWidgetClassPtr, nextWidgetClassPtr;
	DynamicPageClassPtr curPagePtr = &gPagePtr[WorkingPageID];
	u8 widgetType;
	u8 changedWidgetType;
	u8 button_type;
	ButtonClass widget;
	ClockClass clockWidget;
	TextureTimeClass TextureTimeWidget;
	u8 button_num;
	u8 curEnlighted;
	if (this->mInputType==ACTION_KEYBOARD_NEXT || this->mInputType == ACTION_KEYBOARD_PRE || this->mInputType == ACTION_KEYBOARD_OK ||  this->mInputType == ACTION_KEYBOARD_OK_PRESS || this->mInputType == ACTION_KEYBOARD_CLEAR)
	{
        /* If there is no enlighted widget. */
		if( ( (s8)(curPagePtr->curEnlightedWidgetID) ) < 0 )
			return AHMI_FUNC_SUCCESS;
        
        /* Get the enlighted widget's pointer. */
		curEnlightedWidgetClassPtr = &(curPagePtr->pWidgetList[curPagePtr->curEnlightedWidgetID]);
		widgetType = curEnlightedWidgetClassPtr->WidgetType;
		this->mTagPtr = &(TagPtr->getTag(curEnlightedWidgetClassPtr->BindTagID));
        
        /* Enlighted only support butten, clock and texture timer. */
		if(!(widgetType == BUTTON_TEX || widgetType == CLOCK_TEX || widgetType == TEXTURE_TIME || widgetType == LIST))
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
		else 
			curPagePtr->curEnlightedWidgetLighted = 1;
		if(widgetType == BUTTON_TEX)
			button_type = (curEnlightedWidgetClassPtr->WidgetAttr & 0x60) >> 5;
		else if(widgetType == CLOCK_TEX || widgetType == TEXTURE_TIME)
			button_type = COMPLEXBUTTON;
		else if(widgetType == LIST)
			button_type = SINGLEBUTTON;

		/* If a clock widget is ont modifiable, will not be enlighted. */
		if(widgetType == CLOCK_TEX && ( (curEnlightedWidgetClassPtr->WidgetAttr) & 0x80) && this->mInputType != ACTION_KEYBOARD_CLEAR)
		{
			return clockWidget.KeyboardTouchCtrl(curPagePtr, curEnlightedWidgetClassPtr, this, 0);
		}
		if(widgetType == TEXTURE_TIME&& ( (curEnlightedWidgetClassPtr->WidgetAttr) & 0x80) && this->mInputType != ACTION_KEYBOARD_CLEAR)
		{
			return TextureTimeWidget.KeyboardTouchCtrl(curPagePtr, curEnlightedWidgetClassPtr, this, 0);
		}
		if (widgetType == LIST && (curEnlightedWidgetClassPtr->ListHighlightOptions & 0x01) == 1 )
		{
			//in editing
			return triggerWidgetHighlight(curEnlightedWidgetClassPtr);
		}
		preWidgetID = (curEnlightedWidgetClassPtr->KeyboardPreAndNextWidget) >> 8;
		nextWidgetID = (u8)(curEnlightedWidgetClassPtr->KeyboardPreAndNextWidget);
		button_num = (u16)(curEnlightedWidgetClassPtr->NumOfButtonAndCurHighLight) >> 8;
		curEnlighted = (u8)(curEnlightedWidgetClassPtr->NumOfButtonAndCurHighLight);
        
		/* The touch point on the left button. */
		if(this->mInputType == ACTION_KEYBOARD_PRE)
		{
            /* Single button. */
			if(button_type == SINGLEBUTTON)
			{
				if(preWidgetID == curPagePtr->curEnlightedWidgetID)
					return AHMI_FUNC_SUCCESS;
				else 
				{
					preWidgetClassPtr = &(curPagePtr->pWidgetList[preWidgetID]);
					while(preWidgetClassPtr->displayEn(curPagePtr, preWidgetID) != AHMI_FUNC_SUCCESS)
					{
						preOfPreWidgetID = (preWidgetClassPtr->KeyboardPreAndNextWidget) >> 8;
						if(preOfPreWidgetID == preWidgetID)
							return AHMI_FUNC_SUCCESS;
						preWidgetID = preOfPreWidgetID;
						preWidgetClassPtr = &(curPagePtr->pWidgetList[preWidgetID]);
					}
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
					triggerWidgetHighlight(preWidgetClassPtr);
					curPagePtr->curEnlightedWidgetID = preWidgetID;
				}
			}
			else if(button_type == COMPLEXBUTTON) 
			{
				if(preWidgetID == curPagePtr->curEnlightedWidgetID && (curEnlighted == 1 || curEnlighted == 0) ) 
					return AHMI_FUNC_SUCCESS;
				else if(preWidgetID == curPagePtr->curEnlightedWidgetID) 
				{
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
				}
				else if(preWidgetID != curPagePtr->curEnlightedWidgetID && curEnlighted == 1)
				{
					preWidgetClassPtr = &(curPagePtr->pWidgetList[preWidgetID]);
					while(preWidgetClassPtr->displayEn(curPagePtr, preWidgetID) != AHMI_FUNC_SUCCESS) 
					{
						preOfPreWidgetID = (preWidgetClassPtr->KeyboardPreAndNextWidget) >> 8;
						if(preOfPreWidgetID == preWidgetID) 
							return AHMI_FUNC_SUCCESS;
						preWidgetID = preOfPreWidgetID;
						preWidgetClassPtr = &(curPagePtr->pWidgetList[preWidgetID]);
					}
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
					triggerWidgetHighlight(preWidgetClassPtr);
					curPagePtr->curEnlightedWidgetID = preWidgetID;
				}
				else 
				{
					preWidgetClassPtr = &(curPagePtr->pWidgetList[preWidgetID]);
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
				}
			}
			else
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
		}//end of keboard left
		/* The touch point on the right button. */
		else if(this->mInputType == ACTION_KEYBOARD_NEXT)
		{
			if(button_type == SINGLEBUTTON)
			{
				if(nextWidgetID == curPagePtr->curEnlightedWidgetID && curEnlighted == button_num) 
					return AHMI_FUNC_SUCCESS;
				else if(curEnlighted == 0)
				{
					while(curEnlightedWidgetClassPtr->displayEn(curPagePtr, curPagePtr->curEnlightedWidgetID) != AHMI_FUNC_SUCCESS) 
					{
						nextOfNextWidgetID = (curEnlightedWidgetClassPtr->KeyboardPreAndNextWidget) & 0xff ;
						if(nextOfNextWidgetID == curPagePtr->curEnlightedWidgetID) 
							return AHMI_FUNC_SUCCESS;
						curPagePtr->curEnlightedWidgetID = nextOfNextWidgetID;
						curEnlightedWidgetClassPtr =  &(curPagePtr->pWidgetList[curPagePtr->curEnlightedWidgetID]);
					}
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
				}
				else 
				{
					nextWidgetClassPtr = &(curPagePtr->pWidgetList[nextWidgetID]);
					while(nextWidgetClassPtr->displayEn(curPagePtr, nextWidgetID) != AHMI_FUNC_SUCCESS) 
					{
						nextOfNextWidgetID = (nextWidgetClassPtr->KeyboardPreAndNextWidget) & 0xff ;
						if(nextOfNextWidgetID == nextWidgetID) 
							return AHMI_FUNC_SUCCESS;
						nextWidgetID = nextOfNextWidgetID;
						nextWidgetClassPtr =  &(curPagePtr->pWidgetList[nextWidgetID]);
					}
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
					triggerWidgetHighlight(nextWidgetClassPtr);
					curPagePtr->curEnlightedWidgetID = nextWidgetID;
				}
			}
			else if(button_type == COMPLEXBUTTON) 
			{
				if(nextWidgetID == curPagePtr->curEnlightedWidgetID && (curEnlighted == button_num) ) 
					return AHMI_FUNC_SUCCESS;
				else if(nextWidgetID == curPagePtr->curEnlightedWidgetID && (curEnlighted == 0)) 
				{
					while(curEnlightedWidgetClassPtr->displayEn(curPagePtr, curPagePtr->curEnlightedWidgetID) != AHMI_FUNC_SUCCESS) 
					{
						nextOfNextWidgetID = (curEnlightedWidgetClassPtr->KeyboardPreAndNextWidget) & 0xff ;
						if(nextOfNextWidgetID == curPagePtr->curEnlightedWidgetID) 
							return AHMI_FUNC_SUCCESS;
						curPagePtr->curEnlightedWidgetID = nextOfNextWidgetID;
						curEnlightedWidgetClassPtr =  &(curPagePtr->pWidgetList[curPagePtr->curEnlightedWidgetID]);
					}
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
				}
				else if(nextWidgetID == curPagePtr->curEnlightedWidgetID) 
				{
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
				}
				else if(nextWidgetID != curPagePtr->curEnlightedWidgetID && curEnlighted == button_num) 
				{
					nextWidgetClassPtr = &(curPagePtr->pWidgetList[nextWidgetID]);
					while(nextWidgetClassPtr->displayEn(curPagePtr, nextWidgetID) != AHMI_FUNC_SUCCESS) 
					{
						nextOfNextWidgetID = (nextWidgetClassPtr->KeyboardPreAndNextWidget) & 0xff ;
						if(nextOfNextWidgetID == nextWidgetID) 
							return AHMI_FUNC_SUCCESS;
						nextWidgetID = nextOfNextWidgetID;
						nextWidgetClassPtr =  &(curPagePtr->pWidgetList[nextWidgetID]);
					}
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
					triggerWidgetHighlight(nextWidgetClassPtr);
					curPagePtr->curEnlightedWidgetID = nextWidgetID;
				}
				else
				{
					nextWidgetClassPtr = &(curPagePtr->pWidgetList[nextWidgetID]);
					triggerWidgetHighlight(curEnlightedWidgetClassPtr);
				}
			}
			else
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
		}//end of keyboard right
		/* The touch point on the enter button. */
		else if(this->mInputType == ACTION_KEYBOARD_OK || this->mInputType == ACTION_KEYBOARD_OK_PRESS)
		{
			triggerWidgetHighlight(curEnlightedWidgetClassPtr);
		}//end of keyboard entered
		/* The touch point on the clear button. */
		else if(this->mInputType == ACTION_KEYBOARD_CLEAR)
		{
			//clear the highlight of the widget
			triggerWidgetHighlight(curEnlightedWidgetClassPtr);
			//reset the highlight pointer
			preWidgetID = (curEnlightedWidgetClassPtr->KeyboardPreAndNextWidget) >> 8;
			while(preWidgetID != curPagePtr->curEnlightedWidgetID)
			{
				curPagePtr->curEnlightedWidgetID = preWidgetID;
				curEnlightedWidgetClassPtr = &(curPagePtr->pWidgetList[curPagePtr->curEnlightedWidgetID]);
				preWidgetID = (curEnlightedWidgetClassPtr->KeyboardPreAndNextWidget) / 256;
			}
			curPagePtr->curEnlightedWidgetID = preWidgetID;

		}
		/* defaule. */
		else 
		{
#ifdef AHMI_DEBUG
			ERROR_PRINT("no such kind of keyboard trigger!");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetActionTriggerErr);
#endif
		}
	}
	return AHMI_FUNC_SUCCESS;
}

#if defined(CHIP_GC9002)
#include "csi_ahmi.h"
extern void SetPartialDrawRefreMsg(RefreshMsg *msg);
#endif
//-----------------------------
// name:widgetActionTrigger
//
//  
// @param1 WidgetClassPtr pWidgetPtr widget
// @param2 ActionTriggerClassPtr pTagtrigger
// addtional comments :
//    
//-----------------------------
u32 ActionTriggerClass::widgetActionTrigger(
	DynamicPageClassPtr pageptr,
		 WidgetClassPtr pWidgetPtr,
		 u8 u8_widgetRefresh    //
		 )
{
	u16 widgetType = pWidgetPtr->WidgetType;
	TagClassPtr bindTag = &TagPtr->getTag(pWidgetPtr->BindTagID);
	u32 value = bindTag->getValue();
	u8 widgetRefresh = (u8_widgetRefresh == 1) ? 0 : 1;

	this->mTagPtr = bindTag;

#if defined(CHIP_GC9002)	
	if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
	{
		RefreshMsg refreshMsg;
		
		if((widgetType < SX_WIDGETTYPE_END) || (widgetType == CUSTOMWIDGET))
		{
			refreshMsg.mElementType = ANIMATION_REFRESH_WIDGET;
			refreshMsg.mElementPtr.wptr = pWidgetPtr;
			SetPartialDrawRefreMsg(&refreshMsg);
		}
	}
#endif

	if(widgetType == BUTTON_TEX)
	{
		ButtonClass button;
		return button.SetValueCtrl(pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == DYNAMIC_TEX)
	{
		DynamicTexClass widget;
		return widget.SetValueCtrl(pageptr,pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == METER_TEX)
	{
		MeterClass widget ;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == PROGBAR_TEX)
	{
		ProgBarClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh, 1);
	}
	else if(widgetType == SLIDER_TEX)
	{
		SliderClass widget;
		return widget.SetValueCtrl(pageptr,pWidgetPtr, this, widgetRefresh);
	}
//	else if(widgetType == OSCSCP_TEX)
//	{
//		OSCClass widget;
//		return widget.SetValueCtrl(pageptr,pWidgetPtr, this, widgetRefresh);
//	}
	else if(widgetType == TEXTBOX_TEX)
	{
		TextClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == NUMBOX_TEX)
	{
		NumberClass widget ;
		if(u8_widgetRefresh == 0) //indicating that this widget runs in the background
		{
			pWidgetPtr->CurValueL = (u16)value;
			pWidgetPtr->CurValueH = (u16)(value >> 16);
		}
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == VIDEO_TEX)
	{
		VideoClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, 0, widgetRefresh);
	}
	else if(widgetType == KNOB_TEX)
	{
		KnobClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == CLOCK_TEX)
	{
		ClockClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == CLOCKBOARD)
	{
		ClockBoardClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == ON_OFF_TEX)
	{
		OnOffTextureClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == CombinationSwitch)
	{
		CombinationSwitchClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == TEXTURE_NUM)
	{
		TextureNumberClass widget;
		if(u8_widgetRefresh == 0) //indicating that this widget runs in the background
		{
			pWidgetPtr->CurValueL = (u16)value;
			pWidgetPtr->CurValueH = (u16)(value >> 16);
		}
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if(widgetType == TEXTURE_TIME)
	{
		TextureTimeClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}else if(widgetType == TOUCH_TRACK)
	{
		TouchTrackClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}else if (widgetType == ALPHA_SLIDE)
	{
		AlphaSlideClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == TEXT_INPUT)
	{
		TextInputClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == ALPHA_IMG)
	{
		AlphaImgClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == BUTTON_SWITCH)
	{
		ButtonSwitchClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == GALLERY)
	{
		GalleryClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == CHART)
	{
		ChartClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == SELECTOR)
	{
		SelectorClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == CIRCLESLIDER_TEX)
	{
		CircleSliderClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == BARCHART)
	{
		BarChartClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == PIECHART)
	{
		PieChartClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == CUSTOMWIDGET)
	{
		CustomClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
#ifdef VERSION_2
	else if (widgetType == QRCode)
	{
		QRcodeClass widget;
		return widget.SetValueCtrl(pWidgetPtr, this, widgetRefresh);
	}
	else if (widgetType == DATAIMAGE)
	{
		DataImageClass widget;
		return widget.SetValueCtrl(pageptr,pWidgetPtr, this, widgetRefresh);
	}
#endif
	else if (widgetType == LIST)
	{
		ListClass widget;
		return widget.SetValueCtrl(pageptr, pWidgetPtr, this, widgetRefresh);
	}
	else
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
}

//-----------------------------
// name:widgetTouchActionTrigger
//
//  
// @param1 WidgetClassPtr pWidgetPtr widget
// @param2 ActionTriggerClassPtr pTagtrigger
// addtional comments :
//    
//-----------------------------
u32 ActionTriggerClass::widgetMouseTouchActionTrigger(
			DynamicPageClassPtr pageptr,
            WidgetClassPtr pWidgetPtr,
            PIDPoint* pPressPoint,
            PIDPoint* pReleasePoint,
			PIDPoint* pAbsolutePressPoint,
            PIDPoint* pAbsoluteReleasePoint,
            u8 u8_widgetRefresh    //
            )
{
	u16 widgetType=pWidgetPtr->WidgetType;
	TagClassPtr bindTag = &TagPtr->getTag(pWidgetPtr->BindTagID);
	u32 value = bindTag->getValue();
	u8 widgetRefresh = (u8_widgetRefresh == 1) ? 0 : 1;

	this->mTagPtr = bindTag;

	if(widgetType == BUTTON_TEX)
	{
		ButtonClass button;
		return button.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint, widgetRefresh);
	}
	//else if(widgetType == PROGBAR_TEX)
	//{
	//	ProgBarClass widget;
	//	return widget.MouseTouchCtrl(pWidgetPtr, this, pPressPoint, pReleasePoint,  widgetRefresh);
	//}
	else if(widgetType == SLIDER_TEX)
	{
		SliderClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint,  widgetRefresh);
	}
	else if(widgetType == KNOB_TEX)
	{
		KnobClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint,  widgetRefresh);
	}
    else if(widgetType == TOUCH_TRACK)
	{
		TouchTrackClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint,  widgetRefresh);
	}
    else if (widgetType == BUTTON_SWITCH)
	{
		ButtonSwitchClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint, widgetRefresh);
	}
	else if (widgetType == SELECTOR)
	{
		SelectorClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint, widgetRefresh);
	}
	else if (widgetType == SWIPERESPONDER)
	{
		SwipeResponderClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint, widgetRefresh);
	}
	else if (widgetType == MENUCONTAINER)
	{
		MenuContainerClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint, widgetRefresh);
	}
	else if (widgetType == CHART)
	{
		ChartClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint, widgetRefresh);
	}
	else if(widgetType == CIRCLESLIDER_TEX)
	{
		CircleSliderClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint,  widgetRefresh);
	}
	else if(widgetType == LIST)
	{
		ListClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint,  widgetRefresh);
	}
	else if(widgetType == THREE_DIMENSIONAL)
	{
		ThreeDimensionalClass widget;
		return widget.MouseTouchCtrl(pageptr, pWidgetPtr, this, pPressPoint, pReleasePoint, pAbsolutePressPoint, pAbsoluteReleasePoint,  widgetRefresh);
	}
	else
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
}

#endif
