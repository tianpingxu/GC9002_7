
#ifndef CHARTCLASS__H
#define CHARTCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
class ActionTriggerClass;

//******************************************

//******************************************
class ChartClass
{
public:
	ChartClass();
	~ChartClass();

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
    //The widget don't support touch action.
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
        u8 u8_pageRefresh      
        );
};

#endif

#endif
