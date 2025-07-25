////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2019/01/08
// File Name:     GalleryClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2019/01/08 by lpj

// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef GALLERYCLASS__H
#define GALLERYCLASS__H
#include "widgetClassInterface.h"
#include "TextureClass.h"

#ifdef AHMI_CORE
#ifdef __cplusplus
class ActionTriggerClass;

/**
* The class to display the gallery widget, set the widget's tag and change the action
    //WidgetAttr��
    //15-11: reserved

    //10-7:  the number of picture. 1-15
 
    //4-0: type of widget
	//
*/
class GalleryClass 
{
public:
	GalleryClass(void);
	~GalleryClass(void);

    //**************************************
    //config and add gallery widget
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

	funcStatus renderTexture
	(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,   //控件指针
	s32 value,                //the value of tag
	s16 texwid                //the texture width
	);

	funcStatus ComputeShearAngle
	(
	DynamicPageClassPtr pagePtr,
	TextureClassPtr textureptr,   //控件指针
	s32 middlex,                //the middlex of widget
	s16 texwid                //the texture width
	);

	funcStatus SetValueCtrl
	(
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
};

typedef GalleryClass* GalleryClassPtr;
typedef ActionTriggerClass* ActionTriggerClassPtr;
#endif
#endif

#endif
