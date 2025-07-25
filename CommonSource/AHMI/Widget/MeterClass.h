////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by 于春营
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef METERCLASS__H
#define METERCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
class ActionTriggerClass;


//**************************************
// the meter widget
// simple mode or complex mode
// simple mode display two image:
// 0. background
// 1. pointer
// if the pointer contains mask, then 5 additional mask is needed for pointer
// 0. background
// 1. 0 PUREMASK
// 2. PUREMASK for quadrant 0
// 3. PUREMASK for quadrant 1
// 4. PUREMASK for quadrant 2
// 5. PUREMASK for quadrant 3
// 6. pointer
//
//         |
//     2   |   3
// ________|_________
//         |
//     1   |   0
//         |
// complex mode display three images:
// 0. background
// 1. pointer
// 2. lightband
// light band needs 5 puremask, just like pointer mask above
// if pointer contains mask, 5 additional mask is needed for pointer
//
//WidgetAttr：
//15:  to judge the old value
//14-9:reserved
//8:whether has the mask for pointer
//6:rotation direction，1 clockwise，0 anti-clockwise
//5:mode，0 simple mode，1 complex mode
//4-0:type of widget
//**************************************
class MeterClass
{
public:
	MeterClass();
	~MeterClass();

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
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        PIDPoint* pPressPoint,
        PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
        PIDPoint* pAbsoluteReleasePoint,
        u8 widgetRefresh      
        );

//-----------------------------
// name： MeterClass::initWidget
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
	 funcStatus renderTexture
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  
		 s32 value
		 );

//-----------------------------
// name： MeterClass::renderTextureAngle
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
		 WidgetClassPtr p_wptr,  //控件指针
		 s16 Angle
		 );

//-----------------------------
// name： MeterClass::setATag
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
};

#endif

#endif
