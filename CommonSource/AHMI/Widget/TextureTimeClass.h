////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by ÓÚ´ºÓª
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef TEXTURETIMECLASS__H
#define TEXTURETIMECLASS__H
#include "widgetClassInterface.h"

typedef enum{
	COLONSIGN =10,     //Ã°ºÅ
	SLAHSIGN,	 //Ð±¸Ü
	DASHSIGN,   //¶ÌºáÏß 
}TimeSign;

#ifdef AHMI_CORE
class ActionTriggerClass;


//***********************************
//texture timer widget
//@see TimerClass
// difference is that the number is read from image
//--------------------------------------------------------
//|image    |0	1	2	3	4	5	6	7	8	9	-	/	:
//|slice	|0	1	2	3	4	5	6	7	8	9	10	11	12
//---------------------------------------------------------
class TextureTimeClass
{
public:
	TextureTimeClass();
	~TextureTimeClass();

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
    //4-0: type of widget, TextureClock is TEXTURE_TIME
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
    
    //**************************************
    //reponse for Keyboard.Touch
    //widgetCtrl
    //15-11: reserved
    //11:  whether the button is color button
    //10-7: only used in complex button, the number of button. 1-15
    //6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
    //4-0: type of widget, TextureClock is TEXTURE_TIME
    //**************************************    
    funcStatus KeyboardTouchCtrl(
		DynamicPageClassPtr pagePtr,
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        u8 u8_pageRefresh      
        );

//-----------------------------
// name£º MeterClass::setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
	funcStatus setATag(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,   //¿Ø¼þÖ¸Õë
		u8 value
		);
};

#endif

#endif
