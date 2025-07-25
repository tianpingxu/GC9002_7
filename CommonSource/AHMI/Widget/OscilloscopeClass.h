////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by  Yu Chunying
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef OSCCLASS__H
#define OSCCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
class ActionTriggerClass;

class OSCClass
{
public:
	OSCClass();
	~OSCClass();

    //初始化
	 funcStatus initWidget(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,   //控件指针
		u32 *u32p_sourceShift,   //sourcebuffer指针
		NewMatrixPtr pWidgetMatrix,
		u8 u8_pageRefresh,       //页面刷新
		u8 RefreshType ,       //绘制的动画类型，根据动画类型改变绘制控件的包围盒
		TileBoxClassPtr pTileBox, //包围盒
		u8 staticTextureEn          //是否绘制到静态存储空间
		);

	//控件控制函数
	funcStatus widgetCtrl(
		DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  //控件指针
		 ActionTriggerClassPtr ActionPtr,
		 u8 u8_pageRefresh      //页面刷新
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
};

#endif


#endif
