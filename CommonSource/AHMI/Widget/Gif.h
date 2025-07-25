////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:  2023/4/19
// File Name:     Gif.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef GIFCLASS__H
#define GIFCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE

class GifClass:
	public WidgetClassInterface
{
public:
	GifClass();
	~GifClass();

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
		DynamicPageClassPtr pageptr,
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
		DynamicPageClassPtr pageptr,
		WidgetClassPtr p_wptr,  
		u8 u8_pageRefresh     
	);
         

	funcStatus SetValueCtrl(
		DynamicPageClassPtr pageptr,
		WidgetClassPtr p_wptr,  
		u8 u8_widgetRefresh      
	);
         
    

	
};
#endif

#endif
