////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:  2023/6/14
// File Name:     Gif.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef CUSTOMCLASS__H
#define CUSTOMCLASS__H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE

enum CUSTOM_OPCODE {
	CUSTOM_END,
	CUSTOM_GOTO,
	CUSTOM_RET,
	CUSTOM_SET,
	CUSTOM_INC,
	CUSTOM_DEC,
	CUSTOM_MUL,
	CUSTOM_DIV,
	CUSTOM_MOD,
	CUSTOM_AND,
	CUSTOM_OR,
	CUSTOM_XOR,
	CUSTOM_NOT,
	CUSTOM_SL,
	CUSTOM_SR,
	CUSTOM_EQU,
	CUSTOM_NEQ,
	CUSTOM_MORE,
	CUSTOM_LESS,
	CUSTOM_JUMP,
	CUSTOM_BLANK
};

enum texAttr {
	FocusedSlice,
	offsetX,
	offsetY,
	rotateRadius,
	shearX,
	shearY,
	projectX,
	projectY,
	scaleX,
	scaleY
};

enum Snum {
	Imm = 0,
	Tag,
	TexAttr,
	WidgtAttr
};

typedef struct
{
  uint16_t Opcode;
  uint16_t Dest_LOW;
  uint16_t Dest_HIGH;
  uint16_t Source1_LOW;
  uint16_t Source1_HIGH;
  uint16_t Source2_LOW;
  uint16_t Source2_HIGH;
} CustomActionFormat;

#define  CustomActionFormat_Length		14


#define		DESTMASK							0xC000
#define		DEST_Tag							0x4000
#define		DEST_TexAttr						0x8000
#define		DEST_WidgtAttr					0xC000

#define		SOURCE1MASK						0x3000
#define		SOURCE1_Imm						0x0000
#define		SOURCE1_Tag						0x1000
#define		SOURCE1_TexAttr					0x2000
#define		SOURCE1_WidgtAttr				0x3000

#define		SOURCE2MASK						0x0C00
#define		SOURCE2_Imm						0x0000
#define		SOURCE2_Tag						0x0400
#define		SOURCE2_TexAttr					0x0800
#define		SOURCE2_WidgtAttr				0x0C00


class CustomClass:
	public WidgetClassInterface
{
public:
	CustomClass();
	~CustomClass();

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
		ActionTriggerClassPtr ActionPtr,
		u8 u8_widgetRefresh      
	);
         
    

	
};
#endif

#endif
