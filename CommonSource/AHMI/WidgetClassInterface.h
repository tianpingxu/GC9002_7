////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     WidgetClassInterface.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    definition used only by widget
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef WIDGETCLASSINTERFACE__H
#define WIDGETCLASSINTERFACE__H

#include "publicType.h"
#include "publicDefine.h"
#include "AHMIBasicStructure.h"
#include "widgetClassInterface_c1.h"
#include "TextureClass.h"
#include "publicInclude.h"
#ifdef AHMI_CORE
class WidgetClassInterface
{
public:
	//functions
	WidgetClassInterface(void);
	~WidgetClassInterface(void);

	//-----------------------------
	// name£º drawTexture
	// draw texture
	//  
	//   @param1 WidgetClassPtr p_wptr,    //widget pointer
	//   @param2 u32 *u32p_sourceShift,    //sourcebuffer pointer
	//   @param3 u8 RefreshType ,		   //Draw the animation type and change the bounding box of the control according to the animation type.
	//   @param4 TileBoxClassPtr pTileBox, //source box
	//   @param5 u8 staticTextureEn        //Whether to draw to static storage space
	// addtional comments :
	//   none
	//-----------------------------
	u32 drawTexture(
		WidgetClassPtr p_wptr,   //widget pointer
		u32 *u32p_sourceShift,   //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		 //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox,//source box
		u8 staticTextureEn,       //Whether to draw to static storage space
		DynamicPageClassPtr pagePtr
		);

	u32 drawGallryTexture(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,   //widget pointer
		u32 *u32p_sourceShift,   //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		 //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox,//source box
		u8 staticTextureEn       //Whether to draw to static storage space
		);

#ifdef VERSION_2
	u32 drawChartTexture(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,   //widget pointer
		u32 *u32p_sourceShift,   //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		 //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox,//source box
		u8 staticTextureEn       //Whether to draw to static storage space
		);
#endif

#ifdef VERSION_2
	u32 drawQRcodeTexture(
		WidgetClassPtr p_wptr,   //widget pointer
		u32 *u32p_sourceShift,   //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		 //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox,//source box
		u8 staticTextureEn,       //Whether to draw to static storage space
		DynamicPageClassPtr pageptr
		);

	funcStatus drawDataImageTexture(
		DynamicPageClassPtr pageptr,
		WidgetClassPtr p_wptr,   //widget pointer
		u32 *u32p_sourceShift,   //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		 //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox,//source box
		u8 staticTextureEn      //Whether to draw to static storage space
		
		);
#endif
		
	u32 drawtexture_withoutorder(
		 TextureClassPtr DrawingTex,
		 WidgetClassPtr p_wptr,
		 u32 *u32p_sourceShift,
		 NewMatrixPtr pWidgetMatrix
		);

	u32 drawTexture_withCopySourceBuffer(
		WidgetClassPtr p_wptr,   //widget pointer
		u32 *u32p_sourceShift    //sourcebuffer pointer
		);
};
#endif

#endif
