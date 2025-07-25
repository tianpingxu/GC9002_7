////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2019/7/26
// File Name:     SelectorClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 1.00 - File Created 2019/7/26 by Zzen1sS

// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef SELECTORCLASS__H
#define SELECTORCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
#ifdef __cplusplus
class ActionTriggerClass;

/**
* The class to display the selector widget, set the widget's tag and change the action
   
*/
class SelectorClass 
{
public:
	SelectorClass(void);
	~SelectorClass(void);

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
    //reponse for Tag.setValue
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
    //@param WidgetClassPtr p_wptr Pointer of widget
    //@param ActionPtr             Pointer of trigger
    //@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
    //**************************************
    funcStatus MouseTouchCtrl(
		DynamicPageClassPtr pagePtr,
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
		DynamicPageClassPtr pagePtr,
        WidgetClassPtr p_wptr,  
        ActionTriggerClassPtr ActionPtr,
        u8 u8_pageRefresh      
		);

		//-----------------------------
// name�� SelectorClass::updateIndex
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 ActionTriggerClassPtr ActionPtr    //Pointer of trigger
// addtional comments :
//   none
//-----------------------------
	 funcStatus updateIndex
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  
		 ActionTriggerClassPtr ActionPtr
		 );

	//-----------------------------
// name�� SelectorClass::renderTexture
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
// name�� SelectorClass::forceTextureUndisplay
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param   TextureClassPtr TexturePtr,   //pointer of texture
//  @param	 u16 singleLength             //the width/height of the single element
// addtional comments :
//   none
//-----------------------------
	 funcStatus forceTextureUndisplay(
		WidgetClassPtr p_wptr,
		TextureClassPtr TexturePtr,
		u16 singleLength
	 );

	 //-----------------------------
// name�� SelectorClass::renderAlpha
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 s32 value            //the value of the alpha
// addtional comments :
//   none
//-----------------------------

	funcStatus renderAlpha(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,  
		u8 value
	);

//-----------------------------
// name�� setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
funcStatus setATag
	(
	WidgetClassPtr p_wptr,  
	u16 ATagValue
	);

};



typedef SelectorClass* SelectorClassPtr;
typedef ActionTriggerClass* ActionTriggerClassPtr;
#endif
#endif

#endif
