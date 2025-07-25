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
#ifndef NUMBERCLASS__H
#define NUMBERCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
class ActionTriggerClass;


//***********************************
// widget for display number
// display 10 number most. 
// support display pointer. 
// support display minus
// support display leading zeros
// support animation: two lines of number, moving up or down
// the type of alpha is ALPHA4
// 0. 0 PUREMASK to clear alpha buffer
// 1-N. alpha4 for number
// N+1. PURECOLOR for display
//WidgetAttr±Í ∂£∫
//14:sign mode£∫0 for unsigned number£¨range 4294967295~0; 1 for signed number£¨range 2147483647~-2147483648
//13:leading zeros mode, 0 not display leading, 1 for display
//12-9:how many fraction number to display, 
//8-5:total number for display
//4-0:widget type, 0x8
//**********************************
class NumberClass
{
public:
	NumberClass();
	~NumberClass();

    //-----------------------------
    // name£∫ MeterClass::initWidget
    // compute the texture using the tag value
    //  
    //  @param   WidgetClassPtr p_wptr,   //pointer of the widget
    //  @param	 u32 tagValue             //the value of the tag
    // addtional comments :
    //   none
    //-----------------------------
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
    //initialize the widget
    //
	//@param WidgetClassPtr p_wptr Pointer of widget
	//@param u32 *u32p_sourceShift,   //pointer of sourcebuffer
	//@param u8_pageRefresh          //whether refresh the page. if refresh, the value of the widget also refresh.
	//@param u8 RefreshType ,        //the type of animation
	//@param TileBoxClassPtr pTileBox, //tile box @see TileBoxClass
	//@param u8 staticTextureEn          //whether to use the static buffer, not used now
    //**************************************
	funcStatus widgetCtrl(
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr, 
		 ActionTriggerClassPtr ActionPtr,
		 u8 u8_pageRefresh   
		 );

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
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        PIDPoint* pPressPoint,
        PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
        PIDPoint* pAbsoluteReleasePoint,
        u8 u8_pageRefresh      
        );

//-----------------------------
// name£∫ MeterClass::setATag
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

//-----------------------------
// name£∫ setColor
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

//***********************************
// number to texture for display
//**********************************
funcStatus updateNumberDisplay(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	u32 *u32p_sourceShift,
	NewMatrixPtr pWidgetMatrix,
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	);

};




#endif

#endif
