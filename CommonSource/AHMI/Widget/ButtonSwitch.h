//
#ifndef BUTTONSWITCHCLASS__H
#define BUTTONSWITCHCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
class ActionTriggerClass;

//******************************************
//slide
// user can drag it to change the tag
//needs three textures
//0. background
//1. the slide bar
//WidgetAttr£º
//15-5:reserved
// 5direction 0herizontal 1vertical
//4-0:widget type.0x5
//******************************************
class ButtonSwitchClass
{
public:
	ButtonSwitchClass();
	~ButtonSwitchClass();


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


	 //draw widget
	 funcStatus renderTexture
		(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,  
		s32 value
		);

//-----------------------------
// name£º ButtonSwitch::setATag
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
