////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     TextureClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    add comments by zuz 2018/10/30
////////////////////////////////////////////////////////////////////////////////

#ifndef TEXTURECLASS__H
#define TEXTURECLASS__H

#include "publicType.h"
#include "publicDefine.h"
#include "TextureClass_c1.h"
#include "textureClass_cd.h"
#include "publicDefine.h"
#ifndef PC_SIM_Gen
#include "AHMIDriverParameters.h"
#endif

#define CopyPaste_Number_MAX	64

#ifdef AHMI_CORE

/////////////////////////////////
//
//    TextureClass
//    texture attribute:
//    15   : Whether to draw, only for software
//    14   : using widget box 
//    13   : animationMoving // Whether to move in texture animation is used for software only.
//    12   : using_phsical_pixel_ratio // Fixed distortion caused by screen pixels that are not squares, 1 indicates that it needs to be corrected, only for software, by zuz 20180802
//    11   : reserved, by zuz 20180802
//    10-9 : maskType mask &|~
//    8    : addrType //address type
//    7-4  : TexType  //texture type
//    3	   : mask     //whether use mask
//    2-1  : matrix   //matrix type
//    0    : end      //the last texture
//
//////////////////////////////////////

class PointClass;

typedef enum
{
    EM_ErrRetTexture_NULLPointerErr = 1,
	EM_ErrRetTexture_CoordinateOverrun,
	EM_ErrRetTexture_TileBoxOverrun,
	EM_ErrRetTexture_CoordinateNumErr
   
}EM_ErrRetTexture;

class TextureClass
{
public:
	//variables
	u32 SingleSliceSize;	   //single slice size. the texture is multi-slice.
	u32 TexAddr;               //Texture address list first address
	u32 SliceFlagsHeaderAddr;  //slice flags header addr, if 0, no color slice
	u32 mTexAttr;    //texture attribute
	u16 TexWidth;    //texture width, integer
	u16 TexHeight;	 //texture height£¬integer
	u16 FocusedSlice;//Texture slices currently pointing
	//s16 CenterRotationOffsetX;	 //1.11.4 Texture translation vector X for center rotation
	//s16 CenterRotationOffsetY;	 //1.11.4 Texture translation vector Y for center rotation
	s16 OffsetX;	 //1.11.4 Texture translation vector X
	s16 OffsetY;	 //1.11.4 Texture translation vector Y
	s16 RotateAngle; //1.11.4 Texture rotation angle, degree representation
	s16 ShearAngleX; //1.11.4 The wrong angle of the texture in the X direction, tan0 indicates that there is no wrong cut, and tan90 means that it is cut into straight lines.
	s16 ShearAngleY; //1.11.4 The wrong angle of texture in Y direction
	#ifdef VERSION_2
	s16 ShearAngleZX;//1.11.4
	s16 ShearAngleZY;//1.11.4
	#endif
	s16 ScalerX;     //1.6.9 Texture scale vector X
	s16 ScalerY;     //1.6.9 Texture scale vector Y
	//A tile box for texture. The tile box here refers to the maximum range of texture that may be covered, not time-varying.
	s8  TexLeftTileBox      ;  //tile box
	s8  TexTopTileBox     	;  //tile box
	s8  TexRightTileBox   	;  //tile box
	s8  TexButtomTileBox  	;  //tile box
	u8  staticTexture;        //whether add this texture to static storage space
	u8  useCanvasPoint;       //1 for yes, 0 for no

#ifndef PC_SIM_Gen
	//functions
	TextureClass(void);
	~TextureClass(void);
	
	/*for write sourcebuffer api ... add by yn 2018.10.17 */

	//-----------------------------
	// function name£º  computeTextureMatrixAndFinalMatrix
	// compute texture matrix and final matrix
	// param list£º
	//	@param1 NewMatrix* M2£¬			//texture matrix
	//	@param1 WidgetClassPtr p_wptr£¬	//widgetclass pointer
	//	@param1 NewMatrix* Mw£¬			//All matrix operations except texture matrix.
	// Remarks(Revision between versions):
	//  compute the texture's matrix
	//	compute the final matrix M_p^(-1)*M_C^(-1)*M_SC^(-1)*M_W^(-1) M_T^(-1)
	//    added by yn 20181017
	//-----------------------------
	funcStatus computeTextureMatrixAndFinalMatrix(NewMatrix* M2, WidgetClassPtr p_wptr,NewMatrix* Mw);

	//-----------------------------
	// function name£º  computeTextureBoxAndPoint
	// compute texture tile box and point
	// param list£º
	//	@param1 NewMatrix* M2£¬			//texture matrix
	//	@param1 PointClass* p0£¬		//left top point
	//	@param1 PointClass* p1£¬		//right top point
	//	@param1 PointClass* p2£¬		//right bottom point
	//	@param1 PointClass* p3£¬		//left bottom point
	// Remarks(Revision between versions):
	//	compute the texture's box and point
	//    added by yn 20181017
	//-----------------------------
	u32 computeTextureBoxAndPoint(NewMatrix* M2, PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3);
	funcStatus computeTextureBoxAndPointByROI(PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3);

	//-----------------------------
	// function name£º  tailoringTexturePoint
	// tailoring texture point by boundary point
	// param list£º
	//	@param1 PointClass* p0£¬			//left top point
	//	@param1 PointClass* p1£¬			//right top point
	//	@param1 PointClass* p2£¬			//right bottom point
	//	@param1 PointClass* p3£¬			//left bottom point
	//	@param1 PointClass* Boundary_p0£¬	//left boundary top point
	//	@param1 PointClass* Boundary_p1£¬	//right boundary top point
	//	@param1 PointClass* Boundary_p2£¬	//right boundary bottom point
	//	@param1 PointClass* Boundary_p3£¬	//left boundary bottom point
	// Remarks(Revision between versions):
	//	tailoring texture point by boundary point
	//    added by yn 20200902
	//-----------------------------
	funcStatus tailoringTexturePoint(PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3,PointClass* Boundary_p0, PointClass* Boundary_p1, PointClass* Boundary_p2, PointClass* Boundary_p3, PointClass* pPoint, uint32_t* PointNum);

	//-----------------------------
	// function name£º  WriteSourceBuffer
	// write data to sourcebuffer structure, and move the data from the sourcebuffer structure to the SourceBuffer array.
	// param list£º
	//	@param1 u32 *psourceshift£¬			//sourcebuffer offset
	//	@param1 NewMatrix* Mw£¬				//All matrix operations except texture matrix.
	//	@param1 TextureClass* textureInfo£¬	//textureclass  pointer
	//	@param1 WidgetClassPtr p_wptr£¬		//widgetclass pointer
	// Remarks(Revision between versions):
	//    added by yn 20181017
	//-----------------------------
	funcStatus WriteNewSourceBuffer(
		u32 *psourceshift, 
		NewMatrix* Mw,  //matrix of widget
		TextureClass* textureInfo,
		WidgetClassPtr p_wptr
		);

	//-----------------------------
	// function name£º  MoveNewSourceBufferToSourcebufferArray
	// Move the data from the sourcebuffer structure to the SourceBuffer array.
	// param list£º
	//	@param1 u32 *psourceshift£¬					  //sourcebuffer offset
	//	@param1 NewSourceBufferPtr psourcebufferaddr, //NewSourceBuffer structure pointer
	// Remarks(Revision between versions):
	//    added by yn 20181019
	//-----------------------------
	u32 MoveNewSourceBufferToSourcebufferArray(u32 *psourceshift, NewSourceBufferPtr psourcebufferaddr);

#endif
};

void AddTextureFromCopyPasteAction(u32 *psourceshift, u16 widgetID);

void AddTextureFromSnapshot(u32 *psourceshift, WidgetClassPtr p_wptr);

void ChangeTextureBox(u32 sourceshift, u8 startX, u8 startY, u8 stopX, u8 stopY);

typedef TextureClass* TextureClassPtr;

#endif

#endif


