////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//

// File Name:     QRcodeClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// 20190507 by yn
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef QRCODECLASS__H
#define QRCODECLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE

class ActionTriggerClass;


//**************************************
//class for time and data widget
//the number is transfered by tag.
//the display mode for CLOCK_HOUR_MINUTE_SECOND_MODE is hour:minite:second,specific by tag[23:16]:tag[15:8]:tag[7:0]
//the diplay mode for CLOCK_HOUR_MINUTE_MODE is hour:minite, specific by tag[15:8]:tag[7:0]
//the display mode for CLOCK_GANG_YEAR_MODE is year/month/day,specified by tag[31:8]/tag[15:8]/tag[7:0]
//the display mode for CLOCK_MINUS_YEAR_MODE is year-month-day,specified by tag[31:8]-tag[15:8]-tag[7:0]
//WidgetAttr��
//15-7:reserved
//6-5 :mode,could be CLOCK_HOUR_MINUTE_SECOND_MODE / CLOCK_HOUR_MINUTE_MODE/CLOCK_GANG_YEAR_MODE/ CLOCK_MINUS_YEAR_MODE @see definition of the param
//4-0 :the type of widget
//**************************************


class QRcodeClass
{
public:
	QRcodeClass();
	~QRcodeClass();


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
		DynamicPageClassPtr pageptr,
		WidgetClassPtr p_wptr,  
		u32 *u32p_sourceShift,  
		NewMatrixPtr pWidgetMatrix,
		u8 u8_pageRefresh,     
		u8 RefreshType ,      
		TileBoxClassPtr pTileBox, 
		u8 staticTextureEn         
		);

	//**************************************
    
    //
	//@param WidgetClassPtr p_wptr Pointer of widget
	//@param ActionPtr             Pointer of trigger
	//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
    //**************************************
	funcStatus widgetCtrl(
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
    //4-0: type of widget, closk is CLOCK_TEX
    //**************************************
    
    funcStatus SetValueCtrl(
        WidgetClassPtr p_wptr,			
        ActionTriggerClassPtr ActionPtr,
        u8 u8_pageRefresh				
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

};
#endif

#endif

