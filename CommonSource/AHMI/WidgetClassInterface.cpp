////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     WidgetClassInterface.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#ifdef AHMI_DEBUG
#include "stdio.h"
#include "trace.h"
#endif
#include "drawImmediately_cd.h"

#if defined(VERSION_2) && defined(EMBEDDED)
#include "string.h"
extern uint32_t  START_ADDR_OF_QRCode_FROM_GCHEAP; //the start address of QRCode ram
extern uint32_t  START_ADDR_OFFSET_FROM_GCHEAP;
extern uint32_t gNewSoureBufferSize;
#endif

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "SBBatchDraw.h"
#endif

#ifdef AHMI_CORE
extern DynamicPageClassPtr gPagePtr;
extern TagClassPtr			TagPtr;

uint32_t MoveNumbers = 0;
extern uint32_t datanumber[Chart_Num_MAX];

//#define VISUAL_SOURCE_BOX_EN

WidgetClassInterface::WidgetClassInterface(void)
{
}


WidgetClassInterface::~WidgetClassInterface(void)
{
}

//-----------------------------
// name�� drawTexture
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
u32 WidgetClassInterface::drawTexture(
		WidgetClassPtr p_wptr,    //widget pointer
		u32 *u32p_sourceShift,    //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		  //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox, //source box
		u8 staticTextureEn,        //Whether to draw to static storage space
		DynamicPageClassPtr pagePtr
		)
{
	funcStatus status = AHMI_FUNC_SUCCESS;
	int TextureCount;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 mSourceBufferStartPos;
	u32 mSourceBufferLength = 0;
	#endif
	TextureClassPtr  DrawingTex;
	DynamicPageClassPtr curPagePtr;
	CanvasClassPtr curCanvas;

	/*NewMatrix screenMatrix;
	NewMatrix screenAdjustedWidgetMatrix;

	screenMatrix.NewMatrix_init();
	screenMatrix.NewMatrixInv_init();
	screenAdjustedWidgetMatrix.NewMatrix_init();
	screenAdjustedWidgetMatrix.NewMatrixInv_init();

	switch (pagePtr->screenType)
	{
	case RefreshScreenMain:
		screenMatrix.TranslateToMatrix((-pagePtr->screenAreas.mainOffsetX)<<13, (-pagePtr->screenAreas.mainOffsetY)<<13);
		break;
	case RefreshScreenSub:
		screenMatrix.TranslateToMatrix((-pagePtr->screenAreas.subOffsetX)<<13, (-pagePtr->screenAreas.subOffsetY)<<13);
		break;
	default:

		break;
	}

	screenAdjustedWidgetMatrix.LeftMultiMatrix(pWidgetMatrix);
	screenAdjustedWidgetMatrix.LeftMultiMatrix(&screenMatrix);

	pWidgetMatrix = &screenAdjustedWidgetMatrix;*/
	

	if(NULL == p_wptr || NULL == u32p_sourceShift || NULL == pTileBox){
		ERROR_PRINT("ERROR: data is NULL Pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	curPagePtr = pagePtr;
	curCanvas = &pagePtr->pCanvasList[p_wptr->ATTATCH_CANVAS];
	DrawingTex = &curPagePtr->pBasicTextureList[p_wptr->StartNumofTex];
	
	#if (defined VERSION_2) && (defined EMBEDDED)  
	mSourceBufferStartPos = *u32p_sourceShift;
	p_wptr->SourceBufferStartPos = mSourceBufferStartPos;
	#endif
	
	for (TextureCount = 0; TextureCount < p_wptr->NumOfTex; TextureCount++)
	{
		DrawingTex = &curPagePtr->pBasicTextureList[p_wptr->StartNumofTex + TextureCount];
		
	//make it visible
	#ifdef VISUAL_SOURCE_BOX_EN
		TextureClass sourceBoxTexture;
		NewMatrix sourceBoxMatrix;
		sourceBoxMatrix.NewMatrix_init();
		sourceBoxTexture.TexAddr = 0xffff0000;
		sourceBoxTexture.OffsetX = textureTileBox.LeftBox * 32 * 16;
		sourceBoxTexture.OffsetY = textureTileBox.TopBox * 32 * 16;
		sourceBoxTexture.FocusedSlice = 0;
		sourceBoxTexture.RotateAngle = 0 * 16;
		sourceBoxTexture.ScalerX = 1 * 512;
		sourceBoxTexture.ScalerY = 1 * 512;
		sourceBoxTexture.ShearAngleX = 0 * 16;
		sourceBoxTexture.ShearAngleY = 0 * 16;
		sourceBoxTexture.SingleSliceSize = SCREEN_WIDTH * SCREEN_HEIGHT * 2;
		sourceBoxTexture.mTexAttr = NONMASKTEX | EFMATRIX | ADDRTEXTURE | PURECOLOR | DRAWING;
		sourceBoxTexture.TexWidth = (textureTileBox.RightBox - textureTileBox.LeftBox + 1) * 32;
		sourceBoxTexture.TexHeight = (textureTileBox.RightBox - textureTileBox.LeftBox + 1) * 32;
		sourceBoxTexture.TexLeftTileBox = textureTileBox.LeftBox;
		sourceBoxTexture.TexRightTileBox = textureTileBox.RightBox;
		sourceBoxTexture.TexTopTileBox = textureTileBox.TopBox;
		sourceBoxTexture.TexButtomTileBox = textureTileBox.ButtomBox;
		status = sourceBoxTexture.WriteNewSourceBuffer(u32p_sourceShift, &sourceBoxMatrix, &sourceBoxTexture,p_wptr);
	#endif
		if (DrawingTex == NULL)
		{
#ifdef AHMI_DEBUG
			ERROR_PRINT("In darwimmediately(),currenttexture + TextureCount is NULL.\r\n");
#endif
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
		}

		if((DrawingTex->mTexAttr & FORCE_UNDISPLAY) != 0)
		{
			continue; //TexAttr[11] is 1, do not display
		}

		//if((DrawingTex->mTexAttr & DRAWING) == 0)
		//{
		//	continue; //TexAttr[15]is 0,The texture does not need to be drawn.
		//}
		//Recompute source box
		//DrawingTex->copyBox(pTileBox);
		
		if(DrawingTex->TexLeftTileBox < 0)
			DrawingTex->TexLeftTileBox = 0;
		if(DrawingTex->TexTopTileBox < 0)
			DrawingTex->TexTopTileBox = 0;

		if((DrawingTex->TexWidth >= curCanvas->mwidth) && (DrawingTex->TexHeight >= curCanvas->mheight) && (DrawingTex->ScalerX == 0x200) && (DrawingTex->ScalerY == 0x200))
		{
			DrawingTex->useCanvasPoint = 1;
		}
		else
		{
			DrawingTex->useCanvasPoint = 0;
		}

		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex,p_wptr);
	}
	
	#if (defined VERSION_2) && (defined EMBEDDED)
    #if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	if(BatchDraw_u8GetBatchDrawType() == BatchDraw_SB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength = mSourceBufferLength;
		if((*u32p_sourceShift - SBBatchDraw_u32GetSBBatchDrawLastSBPos()) > SBBatchDraw_u32GetSBBatchDrawMaxSize())
		{
			SBBatchDraw_vStartSBBatchDraw(mSourceBufferStartPos);
		}
	}
	else if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength &= 0xF000;
		p_wptr->SourceBufferLength |= mSourceBufferLength;
		PointClass p0(0, 0);
		PointClass p1((p_wptr->WidgetWidth) << 4, 0);
		PointClass p2((p_wptr->WidgetWidth) << 4, (p_wptr->WidgetHeight) << 4);
		PointClass p3(0, (p_wptr->WidgetHeight) << 4);
		p0.rightMulMatrix(pWidgetMatrix);
		p1.rightMulMatrix(pWidgetMatrix);
		p2.rightMulMatrix(pWidgetMatrix);
		p3.rightMulMatrix(pWidgetMatrix);
		FBBatchDraw_vCopyWidgetSB(((p0.mPointX << 12) & 0xFFFF0000) | (p0.mPointY >> 4), 
								  ((p1.mPointX << 12) & 0xFFFF0000) | (p1.mPointY >> 4),
								  ((p2.mPointX << 12) & 0xFFFF0000) | (p2.mPointY >> 4), 
								  ((p3.mPointX << 12) & 0xFFFF0000) | (p3.mPointY >> 4), 
								  (p_wptr->SourceBufferStartPos & 0x7FFF), &(p_wptr->SourceBufferLength));
	}
    #else
    mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
    p_wptr->SourceBufferLength = mSourceBufferLength;
	#endif
	if(gNewSoureBufferSize == 64)
	{
		p_wptr->SourceBufferStartPos &= (~0x8000);
	}
	else if(gNewSoureBufferSize == 24)
	{
		p_wptr->SourceBufferStartPos |= 0x8000;
	}
	else
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}
	#endif
	
	return status;
}

u32 WidgetClassInterface::drawGallryTexture(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,    //widget pointer
		u32 *u32p_sourceShift,    //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		  //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox, //source box
		u8 staticTextureEn        //Whether to draw to static storage space
		)
{
	if(NULL == p_wptr || NULL == u32p_sourceShift || NULL == pTileBox){
		ERROR_PRINT("ERROR: data is NULL Pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	funcStatus status;
	TextureClassPtr  DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex];
	TileBoxClass     textureTileBox;
	TextureClass clearpuremasktexture;
	NewMatrix matrixTemp;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 mSourceBufferStartPos;
	u32 mSourceBufferLength = 0;
	#endif
	TagClassPtr bindTag = &TagPtr->getTag(p_wptr->BindTagID);
	s32 tag_value = bindTag->getValue();
	if (tag_value < 0)
	{
		tag_value = 0;
	}
	if (tag_value > p_wptr->NumOfTex-2)
	{
		tag_value = p_wptr->NumOfTex;
	}

	#if (defined VERSION_2) && (defined EMBEDDED)  
	mSourceBufferStartPos = *u32p_sourceShift;
	p_wptr->SourceBufferStartPos = mSourceBufferStartPos;
	#endif
	
	//clear puremask
	/* matrix initial */
	matrixTemp.NewMatrix_init();
	matrixTemp.NewMatrixInv_init();

	memset((void *)&clearpuremasktexture, 0, sizeof(TextureClass));
	clearpuremasktexture.mTexAttr = PUREMASK;
	clearpuremasktexture.ScalerX = 0x200;
	clearpuremasktexture.ScalerY = 0x200;
	clearpuremasktexture.TexHeight = SCREEN_HEIGHT;
	clearpuremasktexture.TexWidth = SCREEN_WIDTH;
	clearpuremasktexture.TexLeftTileBox = 0;
	clearpuremasktexture.TexTopTileBox = 0;
	clearpuremasktexture.TexButtomTileBox = (((SCREEN_HEIGHT + TILESIZE-1)/TILESIZE) - 1);
	clearpuremasktexture.TexRightTileBox = (((SCREEN_WIDTH + TILESIZE-1)/TILESIZE) - 1);
	clearpuremasktexture.WriteNewSourceBuffer(u32p_sourceShift,&matrixTemp,&clearpuremasktexture,NULL);

	//clear puremask
	//DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + p_wptr->NumOfTex - 2];
	//status = drawtexture_withoutorder(DrawingTex,p_wptr,u32p_sourceShift, pWidgetMatrix);
	//if(status == AHMI_FUNC_MATRIX_ILLEGAL)
	//{
	//	return status;
	//}

	//set puremask
	DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + p_wptr->NumOfTex - 1];
	status = drawtexture_withoutorder(DrawingTex,p_wptr,u32p_sourceShift, pWidgetMatrix);
	if(status == AHMI_FUNC_MATRIX_ILLEGAL)
	{
		return status;
	}

	int TextureCount;
	int middletexture = 0;
	for (TextureCount = 0; TextureCount < p_wptr->NumOfTex - 1; TextureCount++)
	{
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + TextureCount];
		#ifdef VERSION_2
		if (DrawingTex->ShearAngleZX < (-10<<4))
		{
			middletexture++;
		}
		#endif
	}
	
	for (TextureCount = 0; TextureCount < middletexture; TextureCount++)
	{
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + TextureCount];
		status = drawtexture_withoutorder(DrawingTex,p_wptr,u32p_sourceShift, pWidgetMatrix);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}
	}
	for (TextureCount = p_wptr->NumOfTex - 2; TextureCount > middletexture; TextureCount--)
	{
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + TextureCount];
		status = drawtexture_withoutorder(DrawingTex,p_wptr,u32p_sourceShift, pWidgetMatrix);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}
	}
	DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + middletexture];
	status = drawtexture_withoutorder(DrawingTex,p_wptr,u32p_sourceShift, pWidgetMatrix);
	if(status == AHMI_FUNC_MATRIX_ILLEGAL)
	{
		return status;
	}

	#if (defined VERSION_2) && (defined EMBEDDED)
    #if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	if(BatchDraw_u8GetBatchDrawType() == BatchDraw_SB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength = mSourceBufferLength;
		if((*u32p_sourceShift - SBBatchDraw_u32GetSBBatchDrawLastSBPos()) > SBBatchDraw_u32GetSBBatchDrawMaxSize())
		{
			SBBatchDraw_vStartSBBatchDraw(mSourceBufferStartPos);
		}
	}
	else if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength &= 0xF000;
		p_wptr->SourceBufferLength |= mSourceBufferLength;
		PointClass p0(0, 0);
		PointClass p1((p_wptr->WidgetWidth) << 4, 0);
		PointClass p2((p_wptr->WidgetWidth) << 4, (p_wptr->WidgetHeight) << 4);
		PointClass p3(0, (p_wptr->WidgetHeight) << 4);
		p0.rightMulMatrix(pWidgetMatrix);
		p1.rightMulMatrix(pWidgetMatrix);
		p2.rightMulMatrix(pWidgetMatrix);
		p3.rightMulMatrix(pWidgetMatrix);
		FBBatchDraw_vCopyWidgetSB(((p0.mPointX << 12) & 0xFFFF0000) | (p0.mPointY >> 4), 
								  ((p1.mPointX << 12) & 0xFFFF0000) | (p1.mPointY >> 4),
								  ((p2.mPointX << 12) & 0xFFFF0000) | (p2.mPointY >> 4), 
								  ((p3.mPointX << 12) & 0xFFFF0000) | (p3.mPointY >> 4), 
								  (p_wptr->SourceBufferStartPos & 0x7FFF), &(p_wptr->SourceBufferLength));
	}
    #else
    mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
    p_wptr->SourceBufferLength = mSourceBufferLength;
	#endif
	//get last texture data length for last texture flag
	if(gNewSoureBufferSize == 64)
	{
		p_wptr->SourceBufferStartPos &= (~0x8000);
	}
	else if(gNewSoureBufferSize == 24)
	{
		p_wptr->SourceBufferStartPos |= 0x8000;
	}
	else
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}
	#endif
	
	return AHMI_FUNC_SUCCESS;
}

#ifdef VERSION_2

#if defined(EMBEDDED) && defined(VERSION_2)
extern uint8_t* p_Oscilloscopetexture[Chart_Num_MAX];
#endif

#if defined(VERSION_2)
extern uint8_t ChangeModeFlag[Chart_Num_MAX];
#endif

u32 WidgetClassInterface::drawChartTexture(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,    //widget pointer
		u32 *u32p_sourceShift,    //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		  //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox, //source box
		u8 staticTextureEn        //Whether to draw to static storage space
		)
{
	#ifndef CHIP_GC9002_NoHyper
	if(NULL == p_wptr || NULL == u32p_sourceShift || NULL == pTileBox){
		ERROR_PRINT("ERROR: data is NULL Pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	funcStatus status;
	TextureClassPtr  DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex];
	uint16_t Widthtemp, Heighttemp, HeighttempAfterAlign;

	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 mSourceBufferStartPos;
	u32 mSourceBufferLength = 0;
	#endif
	
	if (DrawingTex == NULL)
	{
#ifdef AHMI_DEBUG
		ERROR_PRINT("In darwimmediately(),currenttexture + TextureCount is NULL.\r\n");
#endif
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	#if (defined VERSION_2) && (defined EMBEDDED)  
	mSourceBufferStartPos = *u32p_sourceShift;
	p_wptr->SourceBufferStartPos = mSourceBufferStartPos;
	#endif
	
	//clear alpha buffer texture
	status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	
	//background
	DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 1];
	status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	Heighttemp = DrawingTex->TexHeight;
	Widthtemp = DrawingTex->TexWidth;
	
	//for alpha1 texture
	if(MoveNumbers >= Widthtemp)
	{
		MoveNumbers = 0;
	}
	MoveNumbers++;
	
	//widget height supplement length (64bit align)
	if( (Heighttemp & 0x3f) != 0)
	{
		HeighttempAfterAlign = ((Heighttemp >> 6) << 6) + 64;
	}
	else
	{
		HeighttempAfterAlign = Heighttemp;
	}

	//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
	if(p_wptr->YPADDING == 0)
	{
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2];
		DrawingTex->TexHeight = Widthtemp;
		DrawingTex->TexWidth = HeighttempAfterAlign;
		#if defined(VERSION_2) && defined(EMBEDDED)
        DrawingTex->TexAddr = ((uint32_t)(p_Oscilloscopetexture[p_wptr->IDnumber]) + START_ADDR_OFFSET_FROM_GCHEAP);
		#else
		DrawingTex->TexAddr = (START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET);
		#endif
		DrawingTex->RotateAngle = (270 << 4);
		p_wptr->RotationPointX = (Heighttemp >> 1);
		p_wptr->RotationPointY = (Heighttemp >> 1);
		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	}
	else if(p_wptr->YPADDING == 1)
	{		
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2];
		DrawingTex->TexHeight = Widthtemp - datanumber[p_wptr->IDnumber];
		DrawingTex->TexWidth = HeighttempAfterAlign;
		#if defined(VERSION_2) && defined(EMBEDDED)
		DrawingTex->TexAddr = (((uint32_t)p_Oscilloscopetexture[p_wptr->IDnumber] + START_ADDR_OFFSET_FROM_GCHEAP) + (HeighttempAfterAlign * datanumber[p_wptr->IDnumber] >> 3));
		#else
		DrawingTex->TexAddr = ((START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET) + (HeighttempAfterAlign * datanumber[p_wptr->IDnumber] >> 3));
		#endif
		DrawingTex->RotateAngle = (270 << 4);
		p_wptr->RotationPointX = (Heighttemp >> 1);
		p_wptr->RotationPointY = (Heighttemp >> 1);
		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
		
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 3];
		DrawingTex->TexHeight = datanumber[p_wptr->IDnumber];
		DrawingTex->TexWidth = HeighttempAfterAlign;
		#if defined(VERSION_2) && defined(EMBEDDED)
		DrawingTex->TexAddr = ((uint32_t)p_Oscilloscopetexture[p_wptr->IDnumber] + START_ADDR_OFFSET_FROM_GCHEAP);
		#else
		DrawingTex->TexAddr = (START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET);
		#endif
		DrawingTex->RotateAngle = (270 << 4);
		p_wptr->RotationPointX = (Heighttemp >> 1);
		p_wptr->RotationPointY = (Heighttemp >> 1);
		DrawingTex->OffsetY += ((Widthtemp - datanumber[p_wptr->IDnumber]) << 4);
		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
		DrawingTex->OffsetY -= ((Widthtemp - datanumber[p_wptr->IDnumber]) << 4);
	}
	else if(p_wptr->YPADDING == 2)
	{
		if(ChangeModeFlag[p_wptr->IDnumber] == 1)
		{
			if(datanumber[p_wptr->IDnumber] > Widthtemp)
			{
				DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2];
				DrawingTex->TexHeight = (Widthtemp - (datanumber[p_wptr->IDnumber] - Widthtemp));
				DrawingTex->TexWidth = HeighttempAfterAlign;
				#if defined(VERSION_2) && defined(EMBEDDED)
				DrawingTex->TexAddr = (((uint32_t)p_Oscilloscopetexture[p_wptr->IDnumber] + START_ADDR_OFFSET_FROM_GCHEAP) + (HeighttempAfterAlign * (datanumber[p_wptr->IDnumber] - Widthtemp) >> 3));
				#else
				DrawingTex->TexAddr = ((START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET) + (HeighttempAfterAlign * datanumber[p_wptr->IDnumber] >> 3));
				#endif
				DrawingTex->RotateAngle = (270 << 4);
				p_wptr->RotationPointX = (Heighttemp >> 1);
				p_wptr->RotationPointY = (Heighttemp >> 1);
				status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
				
				DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 3];
				DrawingTex->TexHeight = (datanumber[p_wptr->IDnumber] - Widthtemp);
				DrawingTex->TexWidth = HeighttempAfterAlign;
				#if defined(VERSION_2) && defined(EMBEDDED)
				DrawingTex->TexAddr = ((uint32_t)p_Oscilloscopetexture[p_wptr->IDnumber] + START_ADDR_OFFSET_FROM_GCHEAP);
				#else
				DrawingTex->TexAddr = (START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET);
				#endif
				DrawingTex->RotateAngle = (270 << 4);
				p_wptr->RotationPointX = (Heighttemp >> 1);
				p_wptr->RotationPointY = (Heighttemp >> 1);
				DrawingTex->OffsetY += ((Widthtemp - (datanumber[p_wptr->IDnumber] - Widthtemp)) << 4);
				status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
				DrawingTex->OffsetY -= ((Widthtemp - (datanumber[p_wptr->IDnumber] - Widthtemp)) << 4);
			}
			else
			{
				DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2];
				DrawingTex->TexHeight = Widthtemp - datanumber[p_wptr->IDnumber];
				DrawingTex->TexWidth = HeighttempAfterAlign;
				#if defined(VERSION_2) && defined(EMBEDDED)
				DrawingTex->TexAddr = (((uint32_t)p_Oscilloscopetexture[p_wptr->IDnumber] + START_ADDR_OFFSET_FROM_GCHEAP) + (HeighttempAfterAlign * datanumber[p_wptr->IDnumber] >> 3));
				#else
				DrawingTex->TexAddr = ((START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET) + (HeighttempAfterAlign * datanumber[p_wptr->IDnumber] >> 3));
				#endif
				DrawingTex->RotateAngle = (270 << 4);
				p_wptr->RotationPointX = (Heighttemp >> 1);
				p_wptr->RotationPointY = (Heighttemp >> 1);
				status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
				
				DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 3];
				DrawingTex->TexHeight = datanumber[p_wptr->IDnumber];
				DrawingTex->TexWidth = HeighttempAfterAlign;
				#if defined(VERSION_2) && defined(EMBEDDED)
				DrawingTex->TexAddr = ((uint32_t)p_Oscilloscopetexture[p_wptr->IDnumber] + START_ADDR_OFFSET_FROM_GCHEAP);
				#else
				DrawingTex->TexAddr = (START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET);
				#endif
				DrawingTex->RotateAngle = (270 << 4);
				p_wptr->RotationPointX = (Heighttemp >> 1);
				p_wptr->RotationPointY = (Heighttemp >> 1);
				DrawingTex->OffsetY += ((Widthtemp - datanumber[p_wptr->IDnumber]) << 4);
				status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
				DrawingTex->OffsetY -= ((Widthtemp - datanumber[p_wptr->IDnumber]) << 4);
			}
		}
		else
		{
			DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2];
			DrawingTex->TexHeight = Widthtemp;
			DrawingTex->TexWidth = HeighttempAfterAlign;
			#if defined(VERSION_2) && defined(EMBEDDED)
			DrawingTex->TexAddr = ((uint32_t)(p_Oscilloscopetexture[p_wptr->IDnumber]) + START_ADDR_OFFSET_FROM_GCHEAP);
			#else
			DrawingTex->TexAddr = (START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET);
			#endif
			DrawingTex->RotateAngle = (270 << 4);
			p_wptr->RotationPointX = (Heighttemp >> 1);
			p_wptr->RotationPointY = (Heighttemp >> 1);
			status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
		}
	}
	else if(p_wptr->YPADDING == 3)
	{
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2];
		#if defined(VERSION_2) && defined(EMBEDDED)
        DrawingTex->TexAddr = ((uint32_t)(p_Oscilloscopetexture[p_wptr->IDnumber]) + START_ADDR_OFFSET_FROM_GCHEAP);
		#else
		DrawingTex->TexAddr = (START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET);
		#endif
		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	}

	//rgba8888 texture
	DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 4];
	status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);

	#if (defined VERSION_2) && (defined EMBEDDED)
    #if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	if(BatchDraw_u8GetBatchDrawType() == BatchDraw_SB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength = mSourceBufferLength;
		if((*u32p_sourceShift - SBBatchDraw_u32GetSBBatchDrawLastSBPos()) > SBBatchDraw_u32GetSBBatchDrawMaxSize())
		{
			SBBatchDraw_vStartSBBatchDraw(mSourceBufferStartPos);
		}
	}
	else if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength &= 0xF000;
		p_wptr->SourceBufferLength |= mSourceBufferLength;
		PointClass p0(0, 0);
		PointClass p1((p_wptr->WidgetWidth) << 4, 0);
		PointClass p2((p_wptr->WidgetWidth) << 4, (p_wptr->WidgetHeight) << 4);
		PointClass p3(0, (p_wptr->WidgetHeight) << 4);
		p0.rightMulMatrix(pWidgetMatrix);
		p1.rightMulMatrix(pWidgetMatrix);
		p2.rightMulMatrix(pWidgetMatrix);
		p3.rightMulMatrix(pWidgetMatrix);
		FBBatchDraw_vCopyWidgetSB(((p0.mPointX << 12) & 0xFFFF0000) | (p0.mPointY >> 4), 
								  ((p1.mPointX << 12) & 0xFFFF0000) | (p1.mPointY >> 4),
								  ((p2.mPointX << 12) & 0xFFFF0000) | (p2.mPointY >> 4), 
								  ((p3.mPointX << 12) & 0xFFFF0000) | (p3.mPointY >> 4), 
								  (p_wptr->SourceBufferStartPos & 0x7FFF), &(p_wptr->SourceBufferLength));
	}
    #else
    mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
    p_wptr->SourceBufferLength = mSourceBufferLength;
	#endif
	if(gNewSoureBufferSize == 64)
	{
		p_wptr->SourceBufferStartPos &= (~0x8000);
	}
	else if(gNewSoureBufferSize == 24)
	{
		p_wptr->SourceBufferStartPos |= 0x8000;
	}
	else
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}
	#endif
	
	return status;
	#else
	return AHMI_FUNC_SUCCESS;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
}

#if defined(EMBEDDED) && defined(VERSION_2)
extern uint8_t* p_QRtexture[QRCode_Num_MAX];
#endif
u32 WidgetClassInterface::drawQRcodeTexture(
		WidgetClassPtr p_wptr,    //widget pointer
		u32 *u32p_sourceShift,    //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		  //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox, //source box
		u8 staticTextureEn,        //Whether to draw to static storage space
		DynamicPageClassPtr pageptr
		)
{
	#ifndef CHIP_GC9002_NoHyper
	if(NULL == p_wptr || NULL == u32p_sourceShift || NULL == pTileBox){
		ERROR_PRINT("ERROR: data is NULL Pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	funcStatus status;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 mSourceBufferStartPos;
	u32 mSourceBufferLength = 0;
	#endif
	
	TextureClassPtr  DrawingTex = &pageptr->pBasicTextureList[p_wptr->StartNumofTex];

	if (DrawingTex == NULL)
	{
#ifdef AHMI_DEBUG
		ERROR_PRINT("In darwimmediately(),currenttexture + TextureCount is NULL.\r\n");
#endif
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	#if (defined VERSION_2) && (defined EMBEDDED)  
	mSourceBufferStartPos = *u32p_sourceShift;
	p_wptr->SourceBufferStartPos = mSourceBufferStartPos;
	#endif
	
	//for clear alpha buffer texture
	status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	
	//for white background
	DrawingTex = &pageptr->pBasicTextureList[p_wptr->StartNumofTex + 1];
	status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	
	#if defined(EMBEDDED) && defined(VERSION_2)
	//judge whether draw the QRCode
	if(p_QRtexture[p_wptr->IDnumber] == NULL)
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	else
	{
		START_ADDR_OF_QRCode_FROM_GCHEAP = (uint32_t)p_QRtexture[p_wptr->IDnumber];
	}
	#endif
	
	//for alpha1 texture
	DrawingTex = &pageptr->pBasicTextureList[p_wptr->StartNumofTex + 2];
	#if defined(VERSION_2) && defined(EMBEDDED)
	DrawingTex->TexAddr = (START_ADDR_OF_QRCode_FROM_GCHEAP + START_ADDR_OFFSET_FROM_GCHEAP);
	#else
	DrawingTex->TexAddr = (START_ADDR_OF_QRCode + START_ADDR_OFFSET + p_wptr->IDnumber * QRcode_BUF_SIZE);
	#endif
	status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	
	//for pure color texture
	DrawingTex = &pageptr->pBasicTextureList[p_wptr->StartNumofTex + 3];
	status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	
	#if (defined VERSION_2) && (defined EMBEDDED)
    #if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	if(BatchDraw_u8GetBatchDrawType() == BatchDraw_SB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength = mSourceBufferLength;
		if((*u32p_sourceShift - SBBatchDraw_u32GetSBBatchDrawLastSBPos()) > SBBatchDraw_u32GetSBBatchDrawMaxSize())
		{
			SBBatchDraw_vStartSBBatchDraw(mSourceBufferStartPos);
		}
	}
	else if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength &= 0xF000;
		p_wptr->SourceBufferLength |= mSourceBufferLength;
		PointClass p0(0, 0);
		PointClass p1((p_wptr->WidgetWidth) << 4, 0);
		PointClass p2((p_wptr->WidgetWidth) << 4, (p_wptr->WidgetHeight) << 4);
		PointClass p3(0, (p_wptr->WidgetHeight) << 4);
		p0.rightMulMatrix(pWidgetMatrix);
		p1.rightMulMatrix(pWidgetMatrix);
		p2.rightMulMatrix(pWidgetMatrix);
		p3.rightMulMatrix(pWidgetMatrix);
		FBBatchDraw_vCopyWidgetSB(((p0.mPointX << 12) & 0xFFFF0000) | (p0.mPointY >> 4), 
								  ((p1.mPointX << 12) & 0xFFFF0000) | (p1.mPointY >> 4),
								  ((p2.mPointX << 12) & 0xFFFF0000) | (p2.mPointY >> 4), 
								  ((p3.mPointX << 12) & 0xFFFF0000) | (p3.mPointY >> 4), 
								  (p_wptr->SourceBufferStartPos & 0x7FFF), &(p_wptr->SourceBufferLength));
	}
    #else
    mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
    p_wptr->SourceBufferLength = mSourceBufferLength;
	#endif
	if(gNewSoureBufferSize == 64)
	{
		p_wptr->SourceBufferStartPos &= (~0x8000);
	}
	else if(gNewSoureBufferSize == 24)
	{
		p_wptr->SourceBufferStartPos |= 0x8000;
	}
	else
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}
	#endif
	
	return status;
	#else
	return AHMI_FUNC_SUCCESS;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
}

#ifdef PC_SIM
extern u8       DataImageDataArray[SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max];
#endif
#if defined(EMBEDDED) && defined(VERSION_2)
extern uint8_t* p_DataImagetexture[DataImage_Num_Max];
#endif
funcStatus WidgetClassInterface::drawDataImageTexture(
	    DynamicPageClassPtr pageptr,
		WidgetClassPtr p_wptr,    //widget pointer
		u32 *u32p_sourceShift,    //sourcebuffer pointer
		NewMatrixPtr pWidgetMatrix,
		u8 RefreshType ,		  //Draw the animation type and change the bounding box of the control according to the animation type.
		TileBoxClassPtr pTileBox, //source box
		u8 staticTextureEn        //Whether to draw to static storage space
		
		)
{
	#ifndef CHIP_GC9002_NoHyper
	if(NULL == p_wptr || NULL == u32p_sourceShift || NULL == pTileBox){
		ERROR_PRINT("ERROR: data is NULL Pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	funcStatus status;
	#if defined(EMBEDDED) && defined(VERSION_2)
	u32 mSourceBufferStartPos;
	u32 mSourceBufferLength = 0;
	#endif
	
	TextureClassPtr  DrawingTex = &pageptr->pBasicTextureList[p_wptr->StartNumofTex];
	uint8_t refreshMode = p_wptr->DATAIMG_REFRESH_MODE; //0:push 1:cycle
	uint8_t dataAlign = p_wptr->DATAIMG_DATA_ALIGN;//0:column 1:row

	TextureClass PuremaskTex;
	NewMatrix 	matrixTemp;

	if (DrawingTex == NULL)
	{
#ifdef AHMI_DEBUG
			ERROR_PRINT("In darwimmediately(),currenttexture + TextureCount is NULL.\r\n");
#endif
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	#if defined(EMBEDDED) && defined(VERSION_2)
	mSourceBufferStartPos = *u32p_sourceShift;
	p_wptr->SourceBufferStartPos = mSourceBufferStartPos;
	#endif
	
	if(refreshMode == 0 &&  p_wptr->DATAIMG_LINE_IDX != 0)
	{
		//clear total screen alphabuffer to 0
		PuremaskTex.mTexAttr = DRAWING | PUREMASK;
		PuremaskTex.TexWidth = (SCREEN_WIDTH);
		PuremaskTex.TexHeight = (SCREEN_HEIGHT);
		PuremaskTex.FocusedSlice = 0;
		PuremaskTex.OffsetX = 0;
		PuremaskTex.OffsetY = 0;
		PuremaskTex.RotateAngle = (short)(0);
		PuremaskTex.ShearAngleX = (short)(0);
		PuremaskTex.ShearAngleY = (short)(0);
		PuremaskTex.ShearAngleZX = (short)(0);
		PuremaskTex.ShearAngleZY = (short)(0);
		PuremaskTex.ScalerX = (short)(1<<9);
		PuremaskTex.ScalerY = (short)(1<<9);
		PuremaskTex.useCanvasPoint = 0;
		PuremaskTex.SliceFlagsHeaderAddr = 0;
		PuremaskTex.TexLeftTileBox = 0;
		PuremaskTex.TexTopTileBox = 0;
		PuremaskTex.TexRightTileBox = ((PuremaskTex.TexWidth >> 4) + TILESIZE - 1) / TILESIZE - 1;
		PuremaskTex.TexButtomTileBox = ((PuremaskTex.TexHeight >> 4) + TILESIZE - 1) / TILESIZE - 1;
		PuremaskTex.TexAddr = 0;
		PuremaskTex.staticTexture = 0;
		matrixTemp.NewMatrix_init();
		matrixTemp.NewMatrixInv_init();
		PuremaskTex.WriteNewSourceBuffer(u32p_sourceShift, &matrixTemp, &PuremaskTex, NULL);

		//set widgetbox alphabuffer to 0xFF
		PuremaskTex.mTexAttr = DRAWING | PUREMASK;
		PuremaskTex.TexWidth = p_wptr->WidgetWidth;
		PuremaskTex.TexHeight = p_wptr->WidgetHeight;
		PuremaskTex.OffsetX = 0;
		PuremaskTex.OffsetY = 0;
		PuremaskTex.TexLeftTileBox = 0;
		PuremaskTex.TexTopTileBox = 0;
		PuremaskTex.TexRightTileBox = ((PuremaskTex.TexWidth >> 4) + TILESIZE - 1) / TILESIZE - 1;
		PuremaskTex.TexButtomTileBox = ((PuremaskTex.TexHeight >> 4) + TILESIZE - 1) / TILESIZE - 1;
		PuremaskTex.TexAddr = 0xFF000000;
		PuremaskTex.WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, &PuremaskTex, NULL);
	}

	#if defined(EMBEDDED) && defined(VERSION_2)
	//judge whether draw the QRCode
	if(p_DataImagetexture[p_wptr->DATAIMG_ID] == NULL)
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	else
	{
		START_ADDR_OF_QRCode_FROM_GCHEAP = (uint32_t)p_DataImagetexture[p_wptr->DATAIMG_ID];
	}
	DrawingTex->TexAddr = (START_ADDR_OF_QRCode_FROM_GCHEAP + START_ADDR_OFFSET_FROM_GCHEAP);
	#else
	DrawingTex->TexAddr = START_ADDR_OF_DataImage + START_ADDR_OFFSET + p_wptr->DATAIMG_ID * SIZE_OF_SingleDataImage_BUF;
	#endif
	
	if(refreshMode == 0 &&  p_wptr->DATAIMG_LINE_IDX != 0)
	{
		DrawingTex->mTexAttr |= MASKTEX;
		if(dataAlign == 0)
		{
			DrawingTex->OffsetX = ((p_wptr->WidgetWidth - p_wptr->DATAIMG_LINE_IDX) << 4);
		}
		else
		{
			DrawingTex->OffsetY = ((p_wptr->WidgetHeight - p_wptr->DATAIMG_LINE_IDX) << 4);
		}
		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);

		if(dataAlign == 0)
		{
			DrawingTex->OffsetX = ((-p_wptr->DATAIMG_LINE_IDX) << 4);
		}
		else
		{
			DrawingTex->OffsetY = ((-p_wptr->DATAIMG_LINE_IDX) << 4);
		}
		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	}
	else
	{
		DrawingTex->mTexAttr &= (~MASKTEX);
		status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex, p_wptr);
	}
	
	#if (defined VERSION_2) && (defined EMBEDDED)
    #if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	if(BatchDraw_u8GetBatchDrawType() == BatchDraw_SB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength = mSourceBufferLength;
		if((*u32p_sourceShift - SBBatchDraw_u32GetSBBatchDrawLastSBPos()) > SBBatchDraw_u32GetSBBatchDrawMaxSize())
		{
			SBBatchDraw_vStartSBBatchDraw(mSourceBufferStartPos);
		}
	}
	else if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
	{
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength &= 0xF000;
		p_wptr->SourceBufferLength |= mSourceBufferLength;
		PointClass p0(0, 0);
		PointClass p1((p_wptr->WidgetWidth) << 4, 0);
		PointClass p2((p_wptr->WidgetWidth) << 4, (p_wptr->WidgetHeight) << 4);
		PointClass p3(0, (p_wptr->WidgetHeight) << 4);
		p0.rightMulMatrix(pWidgetMatrix);
		p1.rightMulMatrix(pWidgetMatrix);
		p2.rightMulMatrix(pWidgetMatrix);
		p3.rightMulMatrix(pWidgetMatrix);
		FBBatchDraw_vCopyWidgetSB(((p0.mPointX << 12) & 0xFFFF0000) | (p0.mPointY >> 4), 
								  ((p1.mPointX << 12) & 0xFFFF0000) | (p1.mPointY >> 4),
								  ((p2.mPointX << 12) & 0xFFFF0000) | (p2.mPointY >> 4), 
								  ((p3.mPointX << 12) & 0xFFFF0000) | (p3.mPointY >> 4), 
								  (p_wptr->SourceBufferStartPos & 0x7FFF), &(p_wptr->SourceBufferLength));
	}
    #else
    mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
    p_wptr->SourceBufferLength = mSourceBufferLength;
	#endif
	if(gNewSoureBufferSize == 64)
	{
		p_wptr->SourceBufferStartPos &= (~0x8000);
	}
	else if(gNewSoureBufferSize == 24)
	{
		p_wptr->SourceBufferStartPos |= 0x8000;
	}
	else
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}
	#endif
	
	return status;
	#else
	return AHMI_FUNC_SUCCESS;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
}

#endif

u32 WidgetClassInterface::drawtexture_withoutorder(
		 TextureClassPtr DrawingTex,
		 WidgetClassPtr p_wptr,
		 u32 *u32p_sourceShift,
		 NewMatrixPtr pWidgetMatrix
		 )
{
	funcStatus status;
	#ifdef VISUAL_SOURCE_BOX_EN
		TextureClass sourceBoxTexture;
		NewMatrix sourceBoxMatrix;
		sourceBoxMatrix.NewMatrix_init();
		sourceBoxTexture.TexAddr = 0xffff0000;
		sourceBoxTexture.OffsetX = textureTileBox.LeftBox * 32 * 16;
		sourceBoxTexture.OffsetY = textureTileBox.TopBox * 32 * 16;
		sourceBoxTexture.FocusedSlice = 0;
		sourceBoxTexture.RotateAngle = 0 * 16;
		sourceBoxTexture.ScalerX = 1 * 512;
		sourceBoxTexture.ScalerY = 1 * 512;
		sourceBoxTexture.ShearAngleX = 0 * 16;
		sourceBoxTexture.ShearAngleY = 0 * 16;
		sourceBoxTexture.SingleSliceSize = SCREEN_WIDTH * SCREEN_HEIGHT * 2;
		sourceBoxTexture.mTexAttr = NONMASKTEX | EFMATRIX | ADDRTEXTURE | PURECOLOR | DRAWING;
		sourceBoxTexture.TexWidth = (textureTileBox.RightBox - textureTileBox.LeftBox + 1) * 32;
		sourceBoxTexture.TexHeight = (textureTileBox.RightBox - textureTileBox.LeftBox + 1) * 32;
		sourceBoxTexture.TexLeftTileBox = textureTileBox.LeftBox;
		sourceBoxTexture.TexRightTileBox = textureTileBox.RightBox;
		sourceBoxTexture.TexTopTileBox = textureTileBox.TopBox;
		sourceBoxTexture.TexButtomTileBox = textureTileBox.ButtomBox;
		status = sourceBoxTexture.WriteNewSourceBuffer(u32p_sourceShift, &sourceBoxMatrix, &sourceBoxTexture,p_wptr);
	#endif
		if (DrawingTex == NULL)
		{
#ifdef AHMI_DEBUG
			ERROR_PRINT("In darwimmediately(),currenttexture + TextureCount is NULL.\r\n");
#endif
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
		}

		if((DrawingTex->mTexAttr & FORCE_UNDISPLAY) != 0)
		{
			return AHMI_FUNC_SUCCESS; //TexAttr[11] is 1, do not display
		}

		if(DrawingTex->TexLeftTileBox < 0)
			DrawingTex->TexLeftTileBox = 0;
		if(DrawingTex->TexTopTileBox < 0)
			DrawingTex->TexTopTileBox = 0;
		//If the animation is forced to be animated, the bounding box of all controls is the entire screen.
		//if(DrawingTex->staticTexture == staticTextureEn)//Update static caching
			status = DrawingTex->WriteNewSourceBuffer(u32p_sourceShift, pWidgetMatrix, DrawingTex,p_wptr);
	
	return status;
}

#if (defined VERSION_2) && (defined EMBEDDED)   
#include <string.h>
//extern u8       sourceBuffer[SoureBufferSize];
extern uint32_t SBAddress;
u32 WidgetClassInterface::drawTexture_withCopySourceBuffer(
		WidgetClassPtr p_wptr,   //widget pointer
		u32 *u32p_sourceShift    //sourcebuffer pointer
		)
{
	u32 sourcebufferstart = 0;
	u8 *sourcebufferaddr = 0;
	u8 *lastSourceBufferaddr = 0;
    u8* srcAddr = 0;
    u8* dstAddr = 0;
    uint32_t SBLength = 0;
    uint32_t i = 0;
    
    if( NULL == u32p_sourceShift )
	{
		ERROR_PRINT("ERROR: data is NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	//set last texture data length for last texture flag
	if((p_wptr->SourceBufferLength & 0xFFF) != 0)
	{
		if(p_wptr->SourceBufferStartPos & 0x8000)
		{
			gNewSoureBufferSize = 24;
		}
		else
		{
			gNewSoureBufferSize = 64;
		}
	}
	
	//get lastsourcebuffer start address position for copy
    sourcebufferaddr = (u8 *)SBAddress;
	if(SBAddress == SOURCE_BUFFER_START_ADDR)
    {
        lastSourceBufferaddr = (uint8_t *)(SOURCE_BUFFER_START_ADDR + SOURCE_BUFFER_SIZE);
    }
    else
    {
        lastSourceBufferaddr = (uint8_t *)SOURCE_BUFFER_START_ADDR;
    }

	//get texture copy start address & copy
	sourcebufferstart = (p_wptr->SourceBufferStartPos & 0x7FFF);
//    memcpy(sourcebufferaddr + (*u32p_sourceShift), (uint8_t *)((uint32_t)lastSourceBufferaddr + sourcebufferstart), p_wptr->SourceBufferLength);
    
    srcAddr = (uint8_t *)((uint32_t)lastSourceBufferaddr + sourcebufferstart);
    dstAddr = sourcebufferaddr + (*u32p_sourceShift);
    SBLength = (p_wptr->SourceBufferLength & 0xFFF);
    for(i = 0; i < SBLength; i++)
    {
        *(dstAddr++) = *(srcAddr++);
    }
	
	
	//upgrade texture start address in sourcebuffer
	if(*u32p_sourceShift != (p_wptr->SourceBufferStartPos & 0x7FFF))
	{
		p_wptr->SourceBufferStartPos &= 0x8000;
		p_wptr->SourceBufferStartPos |= *u32p_sourceShift;
	}
	
	//upgrade u32p_sourceShift
	*u32p_sourceShift += (p_wptr->SourceBufferLength & 0xFFF);
	
    #if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	if(BatchDraw_u8GetBatchDrawType() == BatchDraw_SB)
	{
		if((*u32p_sourceShift - SBBatchDraw_u32GetSBBatchDrawLastSBPos()) > SBBatchDraw_u32GetSBBatchDrawMaxSize())
		{
			SBBatchDraw_vStartSBBatchDraw(p_wptr->SourceBufferStartPos & 0x7FFF);
		}
	}
	else if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
	{
		FBBatchDraw_vCopyWidgetSB(FBBatchDrawMagicNum, FBBatchDrawMagicNum, FBBatchDrawMagicNum, FBBatchDrawMagicNum, (p_wptr->SourceBufferStartPos & 0x7FFF), &(p_wptr->SourceBufferLength));
	}
	#endif
	
	//remove the last texture flag bit
	if((p_wptr->SourceBufferLength & 0xFFF) != 0)
	{
		*(sourcebufferaddr + *u32p_sourceShift - (gNewSoureBufferSize - 1)) &= ~(1 << 7);
	}
	
	return AHMI_FUNC_SUCCESS;
}
#endif
#endif
