////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by YuChunYing
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef ALPHASLIDECLASS__H
#define ALPHASLIDECLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE


//*****************************
//dynamic texture 
//only one layer of texture to be displayed, but can be multi-slice
//   
//   ...
//   slice 2
//   slice 1 <- focused slice (to be displayed)
//   slice 0
// the tag value means the focused slice
//WidgetAttr��
//15-8 : reserved
//5-7  : the mode��0��switch between different texture 1��rotate
//4-0  : the type of the widget
//*****************************
class AlphaSlideClass:
	public WidgetClassInterface
{
public:
	AlphaSlideClass();
	~AlphaSlideClass();

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
		u8 RefreshType ,       
		u8 u8_animationEnable,   
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
    //widgetCtrl
    //15-11: reserved
    //11:  whether the button is color button
    //10-7: only used in complex button, the number of button. 1-15
    //6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
    //4-0: type of widget, button is BUTTON_TEX
    //**************************************
    funcStatus SetValueCtrl(
		DynamicPageClassPtr pagePtr,
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        u8 u8_widgetRefresh      
        );
        
    //**************************************
    //reponse for Mouse.Touch
    //
    //**************************************
    funcStatus MouseTouchCtrl(
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        PIDPoint* pPressPoint,
        PIDPoint* pReleasePoint,
        PIDPoint* pAbsolutePressPoint,
        PIDPoint* pAbsoluteReleasePoint,
        u8 u8_pageRefresh      
        );
         
    //**************************************
    //reponse for Keyboard.Touch
    //
    //@param WidgetClassPtr p_wptr Pointer of widget
    //@param ActionPtr             Pointer of trigger
    //@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
    //**************************************
    funcStatus KeyboardTouchCtrl(
		DynamicPageClassPtr pagePtr,
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        u8 u8_pageRefresh      
        );

	//-----------------------------
	// name�� setColor
	// set the purecolor color
	//  
	//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
	//  @param	 u8 r             //the value of the red color
	//  @param	 u8 g             //the value of the red color
	//  @param	 u8 b             //the value of the red color
	//  @param	 u8 a             //the value of the red color
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus setColor(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr, 
		u8 r,
		u8 g,
		u8 b,
		u8 a
	);

	funcStatus setATag
	(
		WidgetClassPtr p_wptr,  
		u16 ATagValue
	);
};

#endif

#endif
