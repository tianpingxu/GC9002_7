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
#ifndef PROGBARCLASS__H
#define PROGBARCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
class ActionTriggerClass;


//******************************************
//progbar
//needs 3/4/5 texture
//0：background
//1：puremask for bar to decide how many to display
//2: bar texture
//3-4: (optianal) pointer texture and pointer mask
//WidgetAttr：
//15:whether the old value has been initialized
//14-10:reserved
//9:whether to display?
//8:whether the pointer needs to change color, 1 change, 0 not change
//7:progbar mode ,0 oridinal,1 changing color
//6:pointer, 1 has, 0 no
//5:direction of the bar,1 vertical,0 herizontal
//4-0:widget type 0x4
//******************************************
class ProgBarClass
{
public:
	ProgBarClass();
	~ProgBarClass();


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

	//-----------------------------
    // compute the texture using the tag value
    //  
    //  @param   WidgetClassPtr p_wptr,   //pointer of the widget
    //  @param	 u32 tagValue             //the value of the tag
	//  @param u8 RefreshType             //the type of animation
    // addtional comments :
    //   none
    //-----------------------------
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
    //4-0: type of widget
    //**************************************
    funcStatus SetValueCtrl(
		DynamicPageClassPtr pagePtr,
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        u8 u8_widgetRefresh,
		u8 u8_actionEnable = 0
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
			WidgetClassPtr p_wptr,   //控件指针
			s32 value                //the value of tag
		 );


//-----------------------------
// name： setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
	funcStatus setATag
	(
		WidgetClassPtr p_wptr,   //控件指针
		u16 ATagValue
	);
};

#endif

#endif
