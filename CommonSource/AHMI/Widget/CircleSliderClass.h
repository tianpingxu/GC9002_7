////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2022/02/22
// File Name:     CircleSliderClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 1.00 - File Created 2022/02/22 by YuShanShan
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef CIRCLESLIDERCLASS__H
#define CIRCLESLIDERCLASS__H
#include "widgetClassInterface.h"


#ifdef AHMI_CORE
class ActionTriggerClass;


//**************************************
// the circleSlider widget
// simple mode or complex mode
// simple mode display two image:
// 0. background
// 1. slider

// complex mode display three images:
// 0. background
// 1. slider
// 2. progress
// progress needs 5 puremask
//
//         |
//     2   |   3
// ________|_________
//         |
//     1   |   0
//         |
//
//WidgetAttr��
//15:  to judge the old value
//14-8:reserved
//6:mode��0 simple mode��1 complex mode
//5:rotation direction��1 clockwise��0 anti-clockwise
//4-0:type of widget
//**************************************
class CircleSliderClass
{
public:
	CircleSliderClass();
	~CircleSliderClass();

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
	funcStatus initWidget(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,   
		u32 *u32p_sourceShift,  
		NewMatrixPtr pWidgetMatrix,
		u8 u8_pageRefresh,      
		u8 RefreshType ,       
		TileBoxClassPtr pTileBox, 
		u8 staticTextureEn          
		);

	//**************************************
    //config and add button widget
    //
	//@param WidgetClassPtr p_wptr Pointer of widget
	//@param ActionPtr             Pointer of trigger
	//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
    //**************************************
	funcStatus widgetCtrl(
		DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  
		 ActionTriggerClassPtr ActionPtr,
		 u8 u8_pageRefresh     
		 );


    //**************************************
    //reponse for Tag.setValue
    //
	//@param WidgetClassPtr p_wptr Pointer of widget
	//@param ActionPtr             Pointer of trigger
	//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
    //**************************************
    funcStatus SetValueCtrl(
		DynamicPageClassPtr pagePtr,
        WidgetClassPtr p_wptr,			  
        ActionTriggerClassPtr ActionPtr,  
        u8 u8_widgetRefresh				  
        );


	//**************************************
    //reponse for Mouse.Touch
    //The widget don't support touch action.
    //@param WidgetClassPtr p_wptr Pointer of widget
    //@param ActionPtr             Pointer of trigger
    //@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
    //**************************************
	funcStatus MouseTouchCtrl(
		DynamicPageClassPtr pageptr,
		WidgetClassPtr p_wptr,  
		ActionTriggerClassPtr ActionPtr,
        PIDPoint* pPressPoint,
        PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
		PIDPoint* pAbsoluteReleasePoint,
		u8 u8_widgetRefresh      
		);

	//**************************************
    //reponse for Keyboard.Touch
    //
	//@param WidgetClassPtr p_wptr Pointer of widget
	//@param ActionPtr             Pointer of trigger
	//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
    //**************************************
	funcStatus KeyboardTouchCtrl(
		 WidgetClassPtr p_wptr,  
		 ActionTriggerClassPtr ActionPtr,
		 u8 u8_pageRefresh      
		 );


//-----------------------------
// name�� CircleSliderClass::renderTextureAngle
// set the texture's angle
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
	funcStatus renderTextureAngle
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  //�ؼ�ָ��
		 s16 Angle
		 );

//-----------------------------
// name�� CircleSliderClass::setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
	funcStatus setATag(
		WidgetClassPtr p_wptr,   
		u16 value                
		);

	//---------------------------------------
	// name: CircleSliderClass::computeAngle
	// compute current angle
	// @param  s32 x,   // cur_x
	// @param  s32 y,   // cur_y
	// @param  u16 sliderWidth,   // sliderWidth
	// @param  u16 sliderHeight,   // sliderHeight
	// @param  s32 RotatePointX,   // RotatePointX
	// @param  s32 RotatePointY,   // RotatePointY
	// @param  s32 width,   // width of the widget
	// @param  s32 height,   // height of the widget
	//---------------------------------------

	s16 computeAngle(
		s32 x,   //1.11.4
		s32 y,   //1.11.4
		u16 sliderWidth,   //1.11.4
		u16 sliderHeight,   //1.11.4
		s32 RotatePointX,   //1.11.4
		s32 RotatePointY,   //1.11.4
		s32 width,   //1.11.4
		s32 height,   //1.11.4
		WidgetClassPtr p_wptr,
		s16 lastAngle   //1.11.4
		);

};

#endif

#endif
