////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef WIDGETCLASS__H
#define WIDGETCLASS__H

#include "publicType.h"
#include "publicDefine.h"
#include "widgetClass_c1.h"
#include "widgetClass_cd.h"
#include "publicDefine.h"

#ifdef PC_SIM_Gen
#include <string>
using namespace std;
#endif

#ifdef AHMI_CORE

typedef enum
{
    EM_ErrRetWidget_NULLPointerErr = 1,
    EM_ErrRetWidget_WidgetSizeErr,
    EM_ErrRetWidget_TexturePtrOverrun,
	EM_ErrRetWidget_DrawTextureErr,
	EM_ErrRetWidget_WidgetIDOverrun,
    EM_ErrRetWidget_ActionTypeErr,
    EM_ErrRetWidget_InitWidgetErr,
	EM_ErrRetWidget_WidgetCtrlErr,
    EM_ErrRetWidget_SetValueErr,
	EM_ErrRetWidget_WidgetTypeErr,
	EM_ErrRetWidget_WidgetAnimationDurationErr,
	EM_ErrRetWidget_WidgetActionTriggerErr,
	EM_ErrRetWidget_CreateAnimationErr,
	EM_ErrRetWidget_CopySourcebufferErr
   
}EM_ErrRetWidget;

typedef struct WidgetGenralInfoTag
{
	int zoomCenterX; 
	int zoomCenterY; 
	int zoomX; 
	int zoomY; 
	int rotateX; 
	int rotateY; 
	int rotateAngle;
	int projectX;
	int projectY;
	u16 touchPropagation;
	u16 replaceMode;
}WidgetGenralInfo;

typedef struct TextScrollInfoTag
{
	u16 duration;
	u16 delay;
	u8 direction;
	u8 autoReverse;
	u8 timingFun;
	u8 enabled;
	u8 mode;
}TextScrollInfo;


class WidgetClass
{
public:
#ifdef PC_SIM_Gen
	string mName;
#endif
	u16 WDG_u16_userVariables[WIDGET_UNVAR];
	NewMatrix mWidgetMatrix;
	ROIClassPtr mROI;
	
	//functions
	WidgetClass(void);
	WidgetClass( int zoomX, int zoomY, int rotateX, int rotateY, int rotateAngle,int projectX,int projectY, u16 touchPropagation);
	WidgetClass( WidgetGenralInfo* piInfo);

#ifndef PC_SIM_Gen
	~WidgetClass(void);

	//-----------------------------
	// name: initWidgetValueForNotFocusedSub
	// set the init tag value for widget
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	u32 initWidgetValue(
		DynamicPageClassPtr pageptr
		);

	//-----------------------------
	// name�� drawWidget
	// draw widget
	//  
	//   @param1 TileBoxClassPtr pTileBox,
	//   @param2 u32 *sourceShift,
	//   @param3 u8  pageEnable ,
	//   @param4 matrixClassPtr pCanvasMatrix,
	//   @param5 u8 RefreshType,        //Draw the animation type and change the bounding box of the control according to the animation type.
	//   @param6 u8 staticTextureEn     //Whether to draw to static storage space
	// addtional comments :
	//   none
	//-----------------------------
	u32 drawWidget(
		TileBoxClassPtr pTileBox,
		u32 *sourceShift,
		u8  pageEnable,
		DynamicPageClassPtr pageptr,
		NewMatrixPtr pCanvasMatrix,
		u8 RefreshType,        //Draw the animation type and change the bounding box of the control according to the animation type.
		u8 staticTextureEn,     //Whether to draw to static storage space
		ROIClassPtr canvasROI = NULL,  //roi of canvas
		s16 widgetScale = 512
		);
		
	//-----------------------------
	// name�� initWidget
	// initial widget matrix
	//  
	//   @param1 none
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus initWidget(void);

	//-----------------------------
	// name�� loadWidgetWithAnimation
	// load widget with animation
	//  
	//   @param1 none
	// addtional comments :
	//  Create a list of control animations
	//	There is no need to pass the current element and the next element because there is no handover in the control.
	//-----------------------------
	u32 loadWidgetWithAnimation(
		DynamicPageClassPtr pagePtr	
	);

	//-----------------------------
	// name�� loadTextureWithAnimation
	// load texture with animation
	//  
	//   @param1 none
	// addtional comments :
	//	 Create texture animation drawing list
	//	 There is no need to pass the current element and the next element because there is no handover in the control.
	//-----------------------------
	u32 loadTextureWithAnimation(
		DynamicPageClassPtr pagePtr
	);

	//-----------------------------
	// name�� clearAnimation
	// clear widget animation
	//  
	//   @param1 none
	// addtional comments :
	//	 There is no need to pass the current element and the next element because there is no handover in the control.
	//-----------------------------
	u32 clearAnimation(
		
	);

	//-----------------------------
	// name�� calculateCurrentAnimation
	// set aTag with current time
	//  
	//   @param1 none
	// addtional comments :
	//	 Create texture animation drawing list
	//	 There is no need to pass the current element and the next element because there is no handover in the control.
	//-----------------------------
	funcStatus calculateCurrentAnimation(
		DynamicPageClassPtr pagePtr
	);


	funcStatus CalculateMatrix(NewMatrix* curCanvasMatrix, s16 widgetScale = 512);

	void SetWidgetMatrixRefreshFlag(void);

	//-----------------------------
	// name�� displayEn
	// Determine whether widget is displayed.
	//  
	//   @param1 u8 widgetID
	// addtional comments :
	//     none
	//-----------------------------
	u32 displayEn(
		DynamicPageClassPtr pagePtr,
		u8 widgetID
		);

	//-----------------------------
	// name�� setATag
	// set the tag of widget
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	u32 setATag(
		u16 value
		);


	//-----------------------------
	// name�� setColor
	// set the color of widget
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	u32 setColor(
		DynamicPageClassPtr pagePtr,
		u32 value
		);

	//-----------------------------
	// name�� setKB
	// set the keyboard container
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	u32 setKB(
		u32 value
		);

	//-----------------------------
	// name�� setKBHint
	// set the keyboard container
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	u32 setKBHint(
		u32 value
		);


	//-----------------------------
	// name�� setKBHintsPage
	// set the keyboard container
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	u32 setKBHintsPage(
		u32 value,
		u8 direction
		);

	//-----------------------------
	// name�� getKB
	// set the keyboard container to target tag
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	u32 getKB(
		u16 tagId
		);

	//-----------------------------
	// name: getWidgetIndex
	// get the indexes of widget
	//  
	//   @param value
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus getWidgetIndex(
		DynamicPageClassPtr pagePtr,
		u16 *CanvasID,
		u16 *SubcanvasID,
		u16 *WidgetID
		);

#endif
};

#ifdef PC_SIM_Gen
class WidgetClassCfg
{
public:
	//��?��?
	u16 WDG_u16_userVariables[WIDGET_UNVAR];
	NewMatrix mWidgetMatrix;
	ROIClassPtr mROI;
};
#endif
#endif

#endif

