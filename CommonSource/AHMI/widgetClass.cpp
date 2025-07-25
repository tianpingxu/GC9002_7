////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     tileBoxClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Revision 2.00 - File Modified 2016/04/24 by Yu Chunying 
// Additional Comments:
//    function of widget
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#ifndef PC_SIM_Gen
#include "ButtonClass.h"
#include "ClockClass.h"
#include "KnobClass.h"
#include "MeterClass.h"
#include "NumberClass.h"
#include "OscilloscopeClass.h"
#include "ProgBarClass.h"
#include "SliderClass.h"
#include "TextClass.h"
#include "ClockBoardClass.h"
#include "VideoClass.h"
#include "OnOffTexture.h"
#include "DynamicTexClass.h"
#include "TextureNumberClass.h"
#include "TextureTimeClass.h"
#include "TouchTrack.h"
#include "AlphaSlide.h"
#include "TextInput.h"
#include "AlphaImg.h"
#include "ButtonSwitch.h"
#include "trace.h"
#include "drawImmediately_cd.h"
#include "refreshQueueHandler.h"
#include "GalleryClass.h"
#include "Chart.h"
#include "QRcodeClass.h"
#include "SelectorClass.h"
#include "KeyboardContainer.h"
#include "SwipeResponderClass.h"
#include "CombinationSwitch.h"
#include "MenuContainer.h"
#include "CircleSliderClass.h"
#include "DataImage.h"
#include "BarChart.h"
#include "PieChart.h"
#include "Gif.h"
#include "ListClass.h"
#include "VideoPlayer.h"
#include "ThreeDimensionalClass.h"
#include "Custom.h"

#ifdef AHMI_CORE

extern AnimationClass  gAnimationClass;

WidgetClass::WidgetClass(void)
{
}

WidgetClass::~WidgetClass(void)
{
}

//-----------------------------
// name:CalculateMatrix
// Calculate the matrix parameters from coordinate parameters
//  
//     @param1 pCanvasMatrix Canvas Matrix pointer
// addtional comments :
//-----------------------------
funcStatus WidgetClass::CalculateMatrix(NewMatrix* pMatrix, s16 widgetScale)
{
	s16 tempRotateCenterX_s16 = 0;
	s16 tempRotateCenterY_s16 = 0;
	s32 tempRotateCenterX = 0;
	s32 tempRotateCenterY = 0;
	s16 tempScaleCenterX_s16 = 0;
	s16 tempScaleCenterY_s16 = 0;
	s32 tempScaleCenterX = 0;
	s32 tempScaleCenterY = 0;
	s32 tempOffsetX = 0;
	s32 tempOffsetY = 0;

	pMatrix->NewMatrix_init();
	pMatrix->NewMatrixInv_init();

	tempScaleCenterX_s16 = (s16)(this->MatrixScaleCenterX);
	tempScaleCenterY_s16 = (s16)(this->MatrixScaleCenterY);
	tempScaleCenterX = (s32)tempScaleCenterX_s16;
	tempScaleCenterY = (s32)tempScaleCenterY_s16;

	if(((s16)(this->MatrixZoomX) > 0) && ((s16)(this->MatrixZoomY) > 0))
	{
		pMatrix->translate( -(tempScaleCenterX << 13), -(tempScaleCenterY << 13));
		pMatrix->scale(this->MatrixZoomX,this->MatrixZoomY);
		pMatrix->translate( (tempScaleCenterX << 13), (tempScaleCenterY << 13));

		if((s16)(this->MatrixZoomX) < 0)
		{
			tempOffsetX = (s16)(this->MatrixZoomX);
			tempOffsetX *= this->WidgetWidth;
			tempOffsetX /= 512; 
			tempOffsetX = -1 - tempOffsetX;
		}
		if((s16)(this->MatrixZoomY) < 0)
		{
			tempOffsetY = (s16)(this->MatrixZoomY);
			tempOffsetY *= this->WidgetHeight;
			tempOffsetY /= 512; 
			tempOffsetY = -1 - tempOffsetY;
		}

		tempOffsetX += (s16)(this->WidgetOffsetX);
		tempOffsetY += (s16)(this->WidgetOffsetY);

		if(this->MatrixRotateAngle != 0)
		{
			tempRotateCenterX_s16 = (s16)(this->MatrixRotateCenterX);
			tempRotateCenterY_s16 = (s16)(this->MatrixRotateCenterY);

			tempRotateCenterX = (s32)tempRotateCenterX_s16;
			//if((this->MatrixZoomX & 0x8000) != 0)
			//{
			//	tempRotateCenterX *= (1 << 9);
			//}
			//else
			//{
			//	tempRotateCenterX *= this->MatrixZoomX;
			//}
			//tempRotateCenterX /=  (1 << 9);

			tempRotateCenterY = (s32)tempRotateCenterY_s16;
			//if((this->MatrixZoomY & 0x8000) != 0)
			//{
			//	tempRotateCenterY *= (1 << 9);
			//}
			//else
			//{
			//	tempRotateCenterY *= this->MatrixZoomY;
			//}
			//tempRotateCenterY /=  (1 << 9);

			pMatrix->translate( -(tempRotateCenterX << 13), -(tempRotateCenterY << 13));
			pMatrix->rotate(this->MatrixRotateAngle);
			pMatrix->translate( (tempRotateCenterX << 13), (tempRotateCenterY<< 13));
		}

		if((this->MatrixProjectionX != 0) || (this->MatrixProjectionY != 0))
		{
			pMatrix->project(this->MatrixProjectionX, this->MatrixProjectionY, this->WidgetWidth*(s16)(this->MatrixZoomX)/512*(s16)(this->ADAPTIVE_SCALE_LEVEL)/512, this->WidgetHeight*(s16)(this->MatrixZoomY)/512*(s16)(this->ADAPTIVE_SCALE_LEVEL)/512);
		}

		//additional center scale
		pMatrix->translate( -(this->WidgetWidth << 13)/2, -(this->WidgetHeight << 13)/2);
		pMatrix->scale(widgetScale, widgetScale);
		pMatrix->translate( (this->WidgetWidth << 13)/2, (this->WidgetHeight << 13)/2);


		pMatrix->translate(tempOffsetX << 13, tempOffsetY << 13);
	}
	else
	{
		if(this->MatrixRotateAngle != 0)
		{
			tempRotateCenterX = this->MatrixRotateCenterX;
			if((this->MatrixZoomX & 0x8000) != 0)
			{
				tempRotateCenterX *= (1 << 9);
			}
			else
			{
				tempRotateCenterX *= this->MatrixZoomX;
			}
			tempRotateCenterX /=  (1 << 9);

			tempRotateCenterY = this->MatrixRotateCenterY;
			if((this->MatrixZoomY & 0x8000) != 0)
			{
				tempRotateCenterY *= (1 << 9);
			}
			else
			{
				tempRotateCenterY *= this->MatrixZoomY;
			}
			tempRotateCenterY /=  (1 << 9);

			pMatrix->translate( -(tempRotateCenterX << 13), -(tempRotateCenterY << 13));
			if(((s16)(this->MatrixZoomX) < 0) && ((s16)(this->MatrixZoomY) < 0))
			{
				pMatrix->rotate(this->MatrixRotateAngle);
			}
			else
			{
				pMatrix->rotate(360 * 16 - this->MatrixRotateAngle);
			}
			pMatrix->translate( (tempRotateCenterX << 13), (tempRotateCenterY<< 13));
		}

		if((this->MatrixProjectionX != 0) || (this->MatrixProjectionY != 0))
		{
			pMatrix->project(this->MatrixProjectionX, this->MatrixProjectionY, this->WidgetWidth, this->WidgetHeight);
		}

		pMatrix->translate( -(tempScaleCenterX << 13), -(tempScaleCenterY << 13));
		pMatrix->scale(this->MatrixZoomX,this->MatrixZoomY);
		pMatrix->translate( (tempScaleCenterX << 13), (tempScaleCenterY << 13));

		if((s16)(this->MatrixZoomX) < 0)
		{
			tempOffsetX = (s16)(this->MatrixZoomX);
			tempOffsetX *= this->WidgetWidth;
			tempOffsetX /= 512; 
			tempOffsetX = -1 - tempOffsetX;
		}
		if((s16)(this->MatrixZoomY) < 0)
		{
			tempOffsetY = (s16)(this->MatrixZoomY);
			tempOffsetY *= this->WidgetHeight;
			tempOffsetY /= 512; 
			tempOffsetY = -1 - tempOffsetY;
		}

		tempOffsetX += (s16)(this->WidgetOffsetX);
		tempOffsetY += (s16)(this->WidgetOffsetY);

		//additional center scale
		pMatrix->translate( -(this->WidgetWidth << 13)/2, -(this->WidgetHeight << 13)/2);
		pMatrix->scale(widgetScale, widgetScale);
		pMatrix->translate( (this->WidgetWidth << 13)/2, (this->WidgetHeight << 13)/2);

		pMatrix->translate(tempOffsetX << 13, tempOffsetY << 13);
	}

	return AHMI_FUNC_SUCCESS;
}

void WidgetClass::SetWidgetMatrixRefreshFlag(void)
{
	this->REFRESH_FLAG |= WidgetMatrixRefreshFlag;
	this->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
}

//-----------------------------
// name: initWidgetValue
// set the init tag value for widget
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::initWidgetValue(
	DynamicPageClassPtr pageptr
	)
{
	ActionTriggerClass gWidgetTagTrigger;
	TagClassPtr bindTag = &TagPtr->getTag(this->BindTagID);
	gWidgetTagTrigger.mInputType = ACTION_TAG_SET_VALUE;
	gWidgetTagTrigger.mTagPtr = bindTag;

	u16 widgetType=this->WidgetType;

	if(widgetType == BUTTON_TEX)
	{
		ButtonClass button;
		return button.SetValueCtrl(this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == DYNAMIC_TEX)
	{
		DynamicTexClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == METER_TEX)
	{
		MeterClass widget ;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == PROGBAR_TEX)
	{
		ProgBarClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0, 0);
	}
	else if(widgetType == SLIDER_TEX)
	{
		SliderClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
//	else if(widgetType == OSCSCP_TEX)
//	{
//		OSCClass widget;
//		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
//	}
	else if(widgetType == TEXTBOX_TEX)
	{
		TextClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == NUMBOX_TEX)
	{
		NumberClass widget ;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == VIDEO_TEX)
	{
		VideoClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == KNOB_TEX)
	{
		KnobClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == CLOCK_TEX)
	{
		ClockClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == CLOCKBOARD)
	{
		ClockBoardClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == ON_OFF_TEX)
	{
		OnOffTextureClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == CombinationSwitch)
	{
		CombinationSwitchClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == TEXTURE_NUM)
	{
		TextureNumberClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if(widgetType == TEXTURE_TIME)
	{
		TextureTimeClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}else if(widgetType == TOUCH_TRACK)
	{
		TouchTrackClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}else if (widgetType == ALPHA_SLIDE)
	{
		AlphaSlideClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if (widgetType == TEXT_INPUT)
	{
		TextInputClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if (widgetType == ALPHA_IMG)
	{
		AlphaImgClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if (widgetType == BUTTON_SWITCH)
	{
		ButtonSwitchClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if (widgetType == GALLERY)
	{
		GalleryClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	//else if (widgetType == CHART)
	//{
	//	ChartClass widget;
	//	return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	//}
	else if (widgetType == SELECTOR)
	{
		SelectorClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if (widgetType == BARCHART)
	{
		BarChartClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if (widgetType == PIECHART)
	{
		PieChartClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
#ifdef VERSION_2
	else if (widgetType == QRCode)
	{
		QRcodeClass widget;
		return widget.SetValueCtrl(this, &gWidgetTagTrigger, 0);
	}
//	else if (widgetType == DATAIMAGE)
//	{
//		DataImageClass widget;
//		return widget.SetValueCtrl(pageptr,this, &gWidgetTagTrigger, 0);
//	}
	else if (widgetType == THREE_DIMENSIONAL)
	{
		ThreeDimensionalClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
	else if (widgetType == CUSTOMWIDGET)
	{
		CustomClass widget;
		return widget.SetValueCtrl(pageptr, this, &gWidgetTagTrigger, 0);
	}
#endif
	else
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_SetValueErr);
}

//-----------------------------
// name: drawWidget
// draw widget
//  
//   @param1 TileBoxClassPtr pTileBox,
//   @param2 u32 *sourceShift,
//   @param3 u8  pageEnable ,
//   @param4 matrixClassPtr pCanvasMatrix,
//   @param5 u8 RefreshType,        //Draw the animation type and change the bounding box of the control according to the animation type.
//   @param6 u8 staticTextureEn     //Whether to draw to static storage space
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::drawWidget(
		TileBoxClassPtr pTileBox,
		u32 *sourceShift,
		u8  pageEnable,
		DynamicPageClassPtr pageptr,
		NewMatrixPtr pCanvasMatrix,
		u8 RefreshType,        //Draw the animation type and change the bounding box of the control according to the animation type.
		u8 staticTextureEn,     //Whether to draw to static storage space
		ROIClassPtr canvasROI,
		s16 widgetScale
		)
{
	u16 widgetType = this->WidgetType;
	u32 ahmiStatus;
	TileBoxClass widgetTileBox;
//	ElementPtr   curPtr;
//	CanvasClassPtr pAttatchingCanvas = &pageptr->pCanvasList[this->ATTATCH_CANVAS];
	//NewMatrix M_canvasTranslate;
	NewMatrix curWidgetMatrix;

	if( NULL == pTileBox || NULL == sourceShift || NULL == pCanvasMatrix ){
		ERROR_PRINT("ERROR: data been null pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	curWidgetMatrix.NewMatrix_init();
	curWidgetMatrix.NewMatrixInv_init();

	if((this->REFRESH_FLAG & WidgetMatrixRefreshFlag) || (pageEnable) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE))
	{
		this->CalculateMatrix(&(this->mWidgetMatrix),widgetScale);

		this->REFRESH_FLAG &= (~WidgetMatrixRefreshFlag);
	}
	//Mw = Msc * Msct * Mw
	/*if (pWidgetScaleMatrix != NULL)
	{
		curWidgetMatrix.LeftMultiMatrix(pWidgetScaleMatrix);
	}*/
	curWidgetMatrix.LeftMultiMatrix(&(this->mWidgetMatrix));
	curWidgetMatrix.LeftMultiMatrix(pCanvasMatrix);

//	curPtr.wptr = this;

	this->mROI = canvasROI;

	//all texture use the widget texture together
	if(widgetType == BUTTON_TEX) //button
	{
		ButtonClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == DYNAMIC_TEX)//dynamic texture/roation texture
	{
		DynamicTexClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, pageptr, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == METER_TEX) //meter
	{
		MeterClass widget ;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == PROGBAR_TEX) //progbar
	{
		ProgBarClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == SLIDER_TEX) //slide 
	{
		SliderClass widget;
		ahmiStatus = widget.initWidget(pageptr,this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
//	else if(widgetType == OSCSCP_TEX)//OSC, not used now
//	{
//		OSCClass widget;
//		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable, RefreshType, &widgetTileBox,staticTextureEn);
//		
//		//Oscilloscope is special.
//		if(pageEnable)
//			return AHMI_FUNC_SUCCESS;
//	}
	else if(widgetType == TEXTBOX_TEX) //text box. not used now
	{
		TextClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable, RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == NUMBOX_TEX) //number
	{
		NumberClass widget ;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable, RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == VIDEO_TEX) //cvbs/cellphone projection
	{
		VideoClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable, RefreshType , &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == KNOB_TEX) //knob, not used now
	{
		KnobClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == CLOCK_TEX) //time/date
	{
		ClockClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable, RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == ON_OFF_TEX) // switch
	{
		OnOffTextureClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == TEXTURE_NUM) //number by image
	{
		TextureNumberClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == TEXTURE_TIME)// time by image
	{
		TextureTimeClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == TOUCH_TRACK)// touch track
	{
		TouchTrackClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == ALPHA_SLIDE)// touch track
	{
		AlphaSlideClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == TEXT_INPUT)
	{
		TextInputClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == ALPHA_IMG)
	{
		AlphaImgClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if(widgetType == BUTTON_SWITCH)
	{
		ButtonSwitchClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == GALLERY)
	{
		GalleryClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == CHART)
	{
		ChartClass widget;
		ahmiStatus = widget.initWidget(pageptr,this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == CLOCKBOARD)
	{
		ClockBoardClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == SELECTOR)
	{
		SelectorClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == QRCode)
	{
		QRcodeClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == KBCONTAINER)
	{
		KeyboardContainerClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == SWIPERESPONDER)
	{
		SwipeResponderClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == CombinationSwitch)
	{
		CombinationSwitchClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == MENUCONTAINER)
	{
		MenuContainerClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == CIRCLESLIDER_TEX)
	{
		CircleSliderClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == DATAIMAGE)
	{
		DataImageClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == BARCHART)
	{
		BarChartClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == PIECHART)
	{
		PieChartClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == GIF)
	{
		GifClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, pageptr, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else if (widgetType == LIST)
	{
		ListClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	#if defined(CHIP_GC9005) || defined(PC_SIM)
    else if (widgetType == VIDEO_PLAYER_TEX)
    {
        VideoPlayerClass widget;
        ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable, RefreshType , &widgetTileBox,staticTextureEn);
    }
	#endif
	else if (widgetType == THREE_DIMENSIONAL)
    {
        ThreeDimensionalClass widget;
        ahmiStatus = widget.initWidget(pageptr, this, sourceShift, &curWidgetMatrix, pageEnable, RefreshType , &widgetTileBox,staticTextureEn);
    }
	else if (widgetType == CUSTOMWIDGET)
	{
		CustomClass widget;
		ahmiStatus = widget.initWidget(pageptr, this, sourceShift, pageptr, &curWidgetMatrix, pageEnable,RefreshType, &widgetTileBox,staticTextureEn);
	}
	else 
		ahmiStatus = (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);

	if(ahmiStatus != AHMI_FUNC_SUCCESS)
	{
		return ahmiStatus;
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: initWidget
// initial widget matrix
//  
//   @param1 none
// addtional comments :
//   none
//-----------------------------
funcStatus WidgetClass::initWidget(void)
{
	this->mWidgetMatrix.NewMatrix_init();
	return this->mWidgetMatrix.NewMatrixInv_init();
}

//-----------------------------
// name: loadWidgetWithAnimation
// load widget with animation
//  
//   @param1 none
// addtional comments :
//  Create a list of control animations
//	There is no need to pass the current element and the next element because there is no handover in the control.
//-----------------------------
u32 WidgetClass::loadWidgetWithAnimation(
	DynamicPageClassPtr pagePtr
)
{
	funcStatus AHMI_status;
	ElementPtr pOldElement, pNewElement;
	
	pOldElement.wptr = this;
	pNewElement.wptr = this;
	
	//pOldElement.pageptr = pagePtr;
	//pNewElement.pageptr = pagePtr;

	AHMI_status = gAnimationClass.Create_AnimatianData(
		ANIMATION_REFRESH_WIDGET,
		pNewElement,
		pOldElement
		);

	if(AHMI_status != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_CreateAnimationErr);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: loadTextureWithAnimation
// load texture with animation
//  
//   @param1 none
// addtional comments :
//	 Create texture animation drawing list
//	 There is no need to pass the current element and the next element because there is no handover in the control.
//-----------------------------
u32 WidgetClass::loadTextureWithAnimation(DynamicPageClassPtr pagePtr)
{
	funcStatus AHMI_status;
	ElementPtr pOldElement, pNewElement;

	pOldElement.wptr = this;
	pNewElement.wptr = this;

	//pOldElement.pageptr = pagePtr;
	//pNewElement.pageptr = pagePtr;

	AHMI_status= gAnimationClass.Create_AnimatianData(
		ANIMATION_REFRESH_WIDGET,
		pNewElement,
		pOldElement
		);

	if(AHMI_status != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_CreateAnimationErr);

	return AHMI_FUNC_SUCCESS;
}

u32 WidgetClass::clearAnimation()
{
	ElementPtr pOldElement, pNewElement;

	pOldElement.wptr = this;
	pNewElement.wptr = this;

	//pOldElement.pageptr = pagePtr;
	//pNewElement.pageptr = pagePtr;

	s32 existedAnimationIdx;
	existedAnimationIdx = gAnimationClass.Get_AnimatianData_Index(
		ANIMATION_REFRESH_WIDGET,
		pNewElement,
		pOldElement
		);

	if (existedAnimationIdx < AnimationMsg_Maxnum && existedAnimationIdx >= 0)
	{
		return gAnimationClass.clearAnimationWithIdx(existedAnimationIdx);
	}


	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: calculateCurrentAnimation

//  
//   @param1 none
// addtional comments :
//	 Create texture animation drawing list
//	 There is no need to pass the current element and the next element because there is no handover in the control.
//-----------------------------
funcStatus WidgetClass::calculateCurrentAnimation(DynamicPageClassPtr pagePtr)
{
//	funcStatus AHMI_status;
	ElementPtr pOldElement, pNewElement;

	pOldElement.wptr = this;
	pNewElement.wptr = this;

	//pOldElement.pageptr = pagePtr;
	//pNewElement.pageptr = pagePtr;

	s32 existedAnimationIdx;
	existedAnimationIdx = gAnimationClass.Get_AnimatianData_Index(
		ANIMATION_REFRESH_WIDGET,
		pNewElement,
		pOldElement
		);

	if (existedAnimationIdx < AnimationMsg_Maxnum)
	{
		gAnimationClass.executeAnimationWithElapsedTime(existedAnimationIdx);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: displayEn
// Determine whether widget is displayed.
//  
//   @param1 u8 widgetID
// addtional comments :
//     none
//-----------------------------
u32 WidgetClass::displayEn(
	DynamicPageClassPtr pagePtr,
	u8 widgetID
		)
{
	CanvasClassPtr attatchCanvas = &(pagePtr->pCanvasList[this->ATTATCH_CANVAS]);
	SubCanvasClassPtr displayingSubcanvas = &(pagePtr->pSubCanvasList[(attatchCanvas->mStartAddrOfSubCanvas) +  (attatchCanvas->mFocusedCanvas) ]);
	if( (widgetID >= (displayingSubcanvas->StartAddrOfWidget) ) && (widgetID < (displayingSubcanvas->StartAddrOfWidget) + (displayingSubcanvas->NumOfWidget) ))
		return AHMI_FUNC_SUCCESS;
	else 
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetIDOverrun);
}

//-----------------------------
// name: setATag
// set the animation tag of widget
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::setATag(
	u16 value
	)
{
	u16 widgetType = this->WidgetType;
	u16 widgetDuration = this->ANIMATION_DURATION;

	if(widgetDuration == 0)
	{
#ifdef AHMI_DEBUG
		ERROR_PRINT("current widget contains no animation");
#endif
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetAnimationDurationErr);
	}

	if(value > widgetDuration)
	{
		value = widgetDuration;
	}
	
	//find coresponding widget and set their animation tag
	if(widgetType == NUMBOX_TEX)
	{
		NumberClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == METER_TEX)
	{
		MeterClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == PROGBAR_TEX)
	{
		ProgBarClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == TEXTURE_NUM)
	{
		TextureNumberClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == BUTTON_SWITCH)
	{
		ButtonSwitchClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == GALLERY)
	{
		GalleryClass widget;
		return widget.setATag(this, value);
	}else if(widgetType == ON_OFF_TEX)
	{
		OnOffTextureClass widget;
		return widget.setATag(this, value);
	}else if(widgetType == CombinationSwitch)
	{
		CombinationSwitchClass widget;
		return widget.setATag(this, value);
	}else if(widgetType == SELECTOR)
	{
		SelectorClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == ALPHA_SLIDE)
	{
		AlphaSlideClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == TEXT_INPUT)
	{
		TextInputClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == CIRCLESLIDER_TEX)
	{
		CircleSliderClass widget;
		return widget.setATag(this, value);
	}
	else if(widgetType == THREE_DIMENSIONAL)
	{
		ThreeDimensionalClass widget;
		return widget.setATag(this, value);
	}
	else 
	{
		ERROR_PRINT("current type of widget don't support animation");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
	}
}

//-----------------------------
// name: setColor
// set the color of widget
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::setColor(
	DynamicPageClassPtr pagePtr,
	u32 value
	)
{
	u8 r,g,b,a;
	u16 widgetType = this->WidgetType;
	
	//trans value to rgba
	a = value & 0xFF;
	value = value >> 8;
	b = value & 0xFF;
	value = value >> 8;
	g = value & 0xFF;
	value = value >> 8;
	r = value & 0xFF;
	
	//find coresponding widget and set their animation tag
	if(widgetType == NUMBOX_TEX)
	{
		NumberClass widget;
		return widget.setColor(pagePtr, this,r,g,b,a);
	}
	else if(widgetType == CLOCK_TEX)
	{
		ClockClass widget;
		return widget.setColor(pagePtr, this, r,g,b,a);
	}
	else if(widgetType == TEXT_INPUT)
	{
		TextInputClass widget;
		return widget.setColor(pagePtr, this, r,g,b,a);
	}
	else if(widgetType == ALPHA_SLIDE)
	{
		AlphaSlideClass widget;
		return widget.setColor(pagePtr, this, r,g,b,a);
	}
	else 
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
	}
}

//-----------------------------
// name: setKB
// set the keyboard container
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::setKB(
	u32 value
	)
{
	u16 widgetType = this->WidgetType;
	
	//find coresponding widget and set their animation tag
	if(widgetType == KBCONTAINER)
	{
		KeyboardContainerClass widget;
		return widget.setKB(this,value);
	}
	else 
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
	}
}

//-----------------------------
// name: setKBHint
// set the keyboard container
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::setKBHint(
	u32 value
	)
{
	u16 widgetType = this->WidgetType;
	
	//find coresponding widget and set their animation tag
	if(widgetType == KBCONTAINER)
	{
		KeyboardContainerClass widget;
		return widget.setKBHint(this,value);
	}
	else 
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
	}
}

//-----------------------------
// name: setKBHintsPage
// set the keyboard container
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::setKBHintsPage(
	u32 value,
	u8 direction
	)
{
	u16 widgetType = this->WidgetType;
	
	//find coresponding widget and set their animation tag
	if(widgetType == KBCONTAINER)
	{
		KeyboardContainerClass widget;
		return widget.setKBHintsPage(this,value, direction);
	}
	else 
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
	}
}

//-----------------------------
// name: getKB
// set the keyboard container to target tag
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
u32 WidgetClass::getKB(
	u16 tagId
	)
{
	u16 widgetType = this->WidgetType;
	
	//find coresponding widget and set their animation tag
	if(widgetType == KBCONTAINER)
	{
		KeyboardContainerClass widget;
		return widget.getKB(this, tagId);
	}
	else 
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetTypeErr);
	}
}

//-----------------------------
// name: getWidgetIndex
// get the indexes of widget
//  
//   @param value
// addtional comments :
//   none
//-----------------------------
funcStatus WidgetClass::getWidgetIndex(
	DynamicPageClassPtr pagePtr,
	u16 *CanvasID,
	u16 *SubcanvasID,
	u16 *WidgetID
	)
{
	SubCanvasClassPtr scptr;
	int i = 0, widgetnum = 0, j = 0;
	u16 temp_sub = 0, temp_widgetid = 0;

	widgetnum = ((uint8_t *)this - (uint8_t *)&(pagePtr->pWidgetList[(pagePtr->pSubCanvasList[pagePtr->pCanvasList[this->ATTATCH_CANVAS].mStartAddrOfSubCanvas].StartAddrOfWidget)])) / sizeof(WidgetClass); // ��ǰ�ӻ����ӵ�һ���ؼ�����ǰ�ؼ��Ŀؼ�����

	for (i = 0; i < pagePtr->pCanvasList[this->ATTATCH_CANVAS].mNumOfSubCanvas; i++)
	{
		scptr = (SubCanvasClassPtr)(pagePtr->pSubCanvasList) + pagePtr->pCanvasList[this->ATTATCH_CANVAS].mStartAddrOfSubCanvas + i;  //��ǰ�ӻ�����ַ
		j = scptr->NumOfWidget;		// ��ǰ�ӻ����Ŀؼ�����

		if(widgetnum < j)
		{
			temp_sub = i;
			temp_widgetid = widgetnum;
			break;
		}
		else
		{
			widgetnum -= j;
		}
	}

	*CanvasID = this->ATTATCH_CANVAS;
	*SubcanvasID = temp_sub;
	*WidgetID = temp_widgetid;

	return AHMI_FUNC_SUCCESS;
}

#endif

#else
#include "stdafx.h"
#include "widgetClass.h"

WidgetClass::WidgetClass()
{
	int i = 0;
	for(i = 0; i < WIDGET_UNVAR;i++)
		WDG_u16_userVariables[i] = 0;

	this->MatrixZoomX = (1 << 9);
	this->MatrixZoomY = (1 << 9);

	this->TouchPropagation = 0;

	OnRealeaseAction = 0xffff;

	mWidgetMatrix.A = (1 << 20);
	mWidgetMatrix.B = 0;
	mWidgetMatrix.C = 0;
	mWidgetMatrix.D = 0;
	mWidgetMatrix.E = (1 << 20);
	mWidgetMatrix.F = 0;
	mWidgetMatrix.G = 0;
	mWidgetMatrix.H = 0;
	mWidgetMatrix.I = (1 << 13);
	mWidgetMatrix.Inverse_A = (1 << 20);
	mWidgetMatrix.Inverse_B = 0;
	mWidgetMatrix.Inverse_C = 0;
	mWidgetMatrix.Inverse_D = 0;
	mWidgetMatrix.Inverse_E = (1 << 20);
	mWidgetMatrix.Inverse_F = 0;
	mWidgetMatrix.Inverse_G = 0;
	mWidgetMatrix.Inverse_H = 0;
	mWidgetMatrix.Inverse_I = (1 << 13);
}

WidgetClass::WidgetClass( int zoomX, int zoomY, int rotateX, int rotateY, int rotateAngle,int projectX,int projectY, u16 touchPropagation)
{
	int i = 0;
	for(i = 0; i < WIDGET_UNVAR;i++)
		WDG_u16_userVariables[i] = 0;


	if(zoomX == 0)
		zoomX = (1 << 9);
	if(zoomY == 0)
		zoomY = (1 << 9);

	this->MatrixZoomX = zoomX;         
	this->MatrixZoomY = zoomY;        
	this->MatrixRotateCenterX = rotateX;
	this->MatrixRotateCenterY = rotateY;
	this->MatrixRotateAngle = rotateAngle;
	this->MatrixProjectionX = projectX;  
	this->MatrixProjectionY = projectY;

	if(touchPropagation == 1)
	{
		this->TouchPropagation = 1;
	}
	else
	{
		this->TouchPropagation = 0;
	}

	OnRealeaseAction = 0xffff;

	mWidgetMatrix.A = (1 << 20);
	mWidgetMatrix.B = 0;
	mWidgetMatrix.C = 0;
	mWidgetMatrix.D = 0;
	mWidgetMatrix.E = (1 << 20);
	mWidgetMatrix.F = 0;
	mWidgetMatrix.G = 0;
	mWidgetMatrix.H = 0;
	mWidgetMatrix.I = (1 << 13);
	mWidgetMatrix.Inverse_A = (1 << 20);
	mWidgetMatrix.Inverse_B = 0;
	mWidgetMatrix.Inverse_C = 0;
	mWidgetMatrix.Inverse_D = 0;
	mWidgetMatrix.Inverse_E = (1 << 20);
	mWidgetMatrix.Inverse_F = 0;
	mWidgetMatrix.Inverse_G = 0;
	mWidgetMatrix.Inverse_H = 0;
	mWidgetMatrix.Inverse_I = (1 << 13);
}

WidgetClass::WidgetClass( WidgetGenralInfo* piInfo)
{
	int i = 0;
	for(i = 0; i < WIDGET_UNVAR;i++)
		WDG_u16_userVariables[i] = 0;


	if(piInfo->zoomX == 0)
		piInfo->zoomX = (1 << 9);
	if(piInfo->zoomY == 0)
		piInfo->zoomY = (1 << 9);

	this->MatrixScaleCenterX = (u16)piInfo->zoomCenterX;         
	this->MatrixScaleCenterY = (u16)piInfo->zoomCenterY;   
	this->MatrixZoomX = (u16)piInfo->zoomX;         
	this->MatrixZoomY = (u16)piInfo->zoomY;        
	this->MatrixRotateCenterX = (u16)piInfo->rotateX;
	this->MatrixRotateCenterY = (u16)piInfo->rotateY;
	this->MatrixRotateAngle = (u16)piInfo->rotateAngle;
	this->MatrixProjectionX = (u16)piInfo->projectX;  
	this->MatrixProjectionY = (u16)piInfo->projectY;

	this->ADAPTIVE_SCALE_LEVEL = 0x200;

	if(piInfo->touchPropagation == 1)
	{
		this->TouchPropagation = 1;
	}
	else
	{
		this->TouchPropagation = 0;
	}

	OnRealeaseAction = 0xffff;

	mWidgetMatrix.A = (1 << 20);
	mWidgetMatrix.B = 0;
	mWidgetMatrix.C = 0;
	mWidgetMatrix.D = 0;
	mWidgetMatrix.E = (1 << 20);
	mWidgetMatrix.F = 0;
	mWidgetMatrix.G = 0;
	mWidgetMatrix.H = 0;
	mWidgetMatrix.I = (1 << 13);
	mWidgetMatrix.Inverse_A = (1 << 20);
	mWidgetMatrix.Inverse_B = 0;
	mWidgetMatrix.Inverse_C = 0;
	mWidgetMatrix.Inverse_D = 0;
	mWidgetMatrix.Inverse_E = (1 << 20);
	mWidgetMatrix.Inverse_F = 0;
	mWidgetMatrix.Inverse_G = 0;
	mWidgetMatrix.Inverse_H = 0;
	mWidgetMatrix.Inverse_I = (1 << 13);
}

#endif
