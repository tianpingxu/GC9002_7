////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by YuChunYing
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef TEXTURENUMBERCLASS__H
#define TEXTURENUMBERCLASS__H
#include "widgetClassInterface.h"

//------------------------------------------------------------
//|image  |0	1	2	3	4	5	6	7	8	9	.	+	-
//|slice  |0	1	2	3	4	5	6	7	8	9	10	11	12
//------------------------------------------------------------
typedef enum{
	ZERO=0,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	MPOINT,
	PLUSSIGN,
	MINUSSIGN,
	X,
	a,b,c,d,e,f,
	A,B,C,D,E,F,
	UNDERLINE,//underline for waiting value
	UNIT,//unit for texture number
	SPACE
}Number;

#ifdef AHMI_CORE
class ActionTriggerClass;


//***********************************
//layer number widget
//@see NumberClass
//the difference is that textureNumberClass reads the number from image, not ALPHA4
//WidgetAttr：
//14:sign mode：0 for unsigned number，range 4294967295~0; 1 for signed number，range 2147483647~-2147483648
//13:leading zeros mode, 0 not display leading, 1 for display
//12-9:how many fraction number to display, 
//8-5:total number for display
//4-0:widget type, 0x8
//------------------------------------------------------------
//|image  |0	1	2	3	4	5	6	7	8	9	.	+	-
//|slice  |0	1	2	3	4	5	6	7	8	9	10	11	12
//------------------------------------------------------------
//**********************************
class TextureNumberClass
{
public:
	TextureNumberClass();
	~TextureNumberClass();

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
        u8 u8_pageRefresh      
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

	//***********************************
	// number to texture for display
	//**********************************
	funcStatus updateNumberDisplay(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,			
		u32 *u32p_sourceShift,
		NewMatrixPtr pWidgetMatrix,
		ActionTriggerClassPtr ActionPtr,
		u8 u8_pageRefresh				
		);
};

#endif

#endif
