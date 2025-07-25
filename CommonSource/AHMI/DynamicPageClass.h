////////////////////////////////////////////////////////////////////////////////
// Company:		Graphichina Inc
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

#ifndef DYNAMICPAGECLASS__H
#define DYNAMICPAGECLASS__H

#include "publicType.h"
#include "publicDefine.h"
#include "refreshQueueHandler.h"
#include "DynamicPageCfgClass.h"
#include "DynamicPageClass_c1.h"
#include "Easing.h"

#ifdef AHMI_CORE

typedef enum
{
    EM_ErrRetPage_NULLPointerErr = 1,
    EM_ErrRetPage_DataOverrun,
	EM_ErrRetPage_SpaceOverrun,
	EM_ErrRetPage_SendPageAnimationErr,
	EM_ErrRetPage_PageAnimationTypeErr
   
}EM_ErrRetPage;

class DynamicPageClass
{
public:
	//variables
	u32 mStoreOffset;//The page is located in the offset of the storage file
	u32 mTotalSize;  //Total size of DynamicPage dynamic data
	u32 mCheckCode;	 //check code for page
	u16 mPageAttr;  //Page attribute¡ê?reserved 
	u16 mTotalNumOfTexture;//The total texture number (including background texture) of this page
	u16 mNumOfWidget;//The current page contains widget quantities.
	u16 mActionInstructionsSize;//Total length of instruction queue
	u16 mNumOfPageAction;//Number of PageAction
	u16 mNumOfCanvasAction;//Number of canvasAction
	u16 mNumofTouchWidget;//Number of touch widget
	u16 mNumofGifWidget;//Number of gif widget
	u16 mNumOfCanvas;//Number of canvas in the current page
	u16 mNumOfSubCanvas;//Number of subcanvas in the current page
	u8  mAnimationType   ;//animation type
	//u8  mtotoalFrame     ;//Number of total frame 
	u16	 pageAnimationDuration;//Page animation duration
	u8 timingFun; //easing
	u16  mATag;            //animation tag
	u8  curEnlightedWidgetID; //current enlighted widgetID
	u8  curEnlightedWidgetLighted; //whether current enlighted widgetID remain high
	u8	mStartOfStack;    //The position of the passed parameters in the stack when changing pages.
	u8	mNumOfPara;			//Number of parameters passed when changing pages
	u16 mNumOfAnimationKeyFrames; //Number of animation key frames
	u8  mNumOfAnimationActions; //Number of user defined animation
	u8  VideoType;           //video interface type: 0 for no video, 1 for CVBS(bt656), 2 for DVP, 3 for AHD, 4 for RGB
	PageScreenOptions screenAreas; //screen areas of page
	
	WidgetClassPtr pWidgetList;      //WidgetList is a pointer to an array, where the address of WidgetList is placed.	
	CanvasClassPtr pCanvasList;      //CanvasList is a pointer to an array, where the address of CanvasList is placed.
	SubCanvasClassPtr pSubCanvasList;//SubCanvasList is a pointer to an array, where the address of SubCanvasList is placed.
	AnimationKeyFramePtr pAnimationKeyFrameList;  //animation key frame list
	AnimationActionClassPtr pAnimationActionList; //Animated instruction array
	u8* pActionInstructions;         //Instruction queue
	u16* pActionStartADDR;            //Address subscript of PageActions
	u16* pCanvasActionStartADDR;      //Canvas instruction start address
	touchWidgetRange *pTouchWidgetList; //The range of controls that can be touched.
	u16* pGifWidgetList; //gif widget list
	TextureClassPtr pBackgroundTexture;//Background texture information   BasicTexture* BackgroundTexture;	
	TextureClassPtr pBasicTextureList;//Texture structure	 BasicTexture** BasicTextureList;
	TextureClassPtr pMouseTexture; //Mouse texture information	
	WidgetClassPtr  pMouseWidget; //Mouse control
	
	NewMatrix     mPageMatrix ; //Page matrix
	NewMatrix     mSubPageMatrix;//sub page matrix

	/*u8 pressFlag;
	u16 pressedPosX;
	u16 pressedPosY;*/
	RefreshScreenTypes screenType;
	u8 shouldShow;
	s16 animationStartOffset;
	u16	pageReleasingAnimationDuration;//Page releasing animation duration
	
	//functions
	DynamicPageClass(void);
	~DynamicPageClass(void);

	//-----------------------------
	// name  InitPage
	// initialize page
	//
	// @param1 void*   BassAddress            , //Base address for storing dynamic data
	// @param2 u32     StoreOffset            , //Data offset
	// @param3 u16     PageAttr               , //Page attribute
	// @param3 u16     NumOfWidget            , //Number of widget
	// @param3 u8 	   NumOfCanvas			  , //Number of Canvas
	// @param3 u8      NumOfSubCanvas         , //Number of subCanvas
	// @param3 u16     ActionInstructionsSize , //Total instruction length
	// @param3 u16     TotalNumOfTexture	  , //Numbe of Texture
	// @param3 u16     NumofTouchWidget       , //Numbe of touch widget
	// @param3 u16     NumOfPageAction        , //Numbe of page action
	// @param3 u16	   NumOfCanvasAction	  ,	//Numbe of CanvasAction
	// @param3 u8      animationType          , //Switch animation type
	// @param3 u8      curEnlightedWidgetID   , //Current enlighted widget ID
	// @param3 u16     animationDuration      , //Animated frame number
	// @param3 u8      numOfAnimationActions    //Number of user defined animations
	// additional comments
	//   none
	//-----------------------------
	funcStatus InitPage(
		void*          BassAddress            , //Base address for storing dynamic data
		u32            StoreOffset            , //Data offset
		u32			   TotalSize			  , //total size
		u32			   CheckCode			  , //check code
		u16            PageAttr               , //Page attribute
		u16            NumOfWidget            , //Number of widget
		u16 			   NumOfCanvas			  , //Number of Canvas
		u16            NumOfSubCanvas         , //Number of subCanvas
		u16            ActionInstructionsSize , //Total instruction length
		u16            TotalNumOfTexture	  , //Numbe of Texture
		u16            NumofTouchWidget       , //Numbe of touch widget
		u16            NumofGifWidget         , //Numbe of gif widget
		u16            NumOfPageAction        , //Numbe of page action
		u16		       NumOfCanvasAction	  ,	//Numbe of CanvasAction
		u8             animationType          , //Switch animation type
		u8             curEnlightedWidgetID   , //Current enlighted widget ID
		u16            animationDuration      , //Animated frame number
		u8	           timingFun              ,	//easing
		u16            numOfAnimationKeyFrames,
		u8             numOfAnimationActions  , //Number of user defined animations
		u8			   mVideoType,
		PageScreenOptions _screenAreas
	);

	funcStatus loadLastOnePageData(u16 page, u8 systemInitial);
	funcStatus loadLastTwoPageData(u16 page, u8 systemInitial);

	//-----------------------------
	// name  loadPage
	// load page
	//  
	//   @param1 u8 page : page 
	//   @param2 u8 systemInitial : Represents the most initial page of the system for highlighting controls.
	// addtional comments :
	//   none
	//-----------------------------
	u32 loadPage(u16 page, u8 systemInitial = 0);

	//-----------------------------
	// name  loadPageWithAnimation
	// load page with animation
	//  
	//     @param1 DynamicPageClassPtr pCurPage  , //current page
	//     @param2 DynamicPageClassPtr pNextPage , //next page
	//     @param3 u8 curFrame                   , //current frame
	//     @param4 u8 totalFrame                 , //total frame
	//     @param5 u8 animationType                //animation type
	// addtional comments :
	//   none
	//-----------------------------
	u32 loadPageWithAnimation(
		u8 curPageID                  , //current page
		u8 nextPageID                 , //next page
        u8 curFrame                   , //current frame
		u8 totalFrame                 , //total frame
		u8 animationType                //animation type
		);

	//-----------------------------
	// name  drawPage
	// draw page
	//  
	//     @param1 TileBoxClassPtr tileBox, //source box
	//     @param2 u32 *sourceShift       , //sourcebuffer pointer
	//     @param3 u8  pageEnable         , //whether refresh total page
	//     @param4 u8 RefreshType         , //Animation is being drawn, and the bounding box of the control is cancelled.
	//     @param5 u8 staticTextureEn       //Whether to draw to static storage space
	// addtional comments :
	//   none
	//-----------------------------
	u32 drawPage(
		TileBoxClassPtr tileBox,//source box
		u32 *sourceShift       ,//sourcebuffer pointer
		u8 pageEnable          ,//whether refresh total page
		u8 RefreshType         ,//Animation is being drawn, and the bounding box of the control is cancelled.
		u8 staticTextureEn      //Whether to draw to static storage space
		);

	//-----------------------------
	// name  drawPageWithAnimation
	// draw page with animation
	//  
	//     @param1 DynamicPageClass* pCurPage    ,//Current page pointer
	//     @param2 TileBoxClassPtr tileBox       ,//source box
	//     @param3 u32* pSourceShift             ,//sourcebuffer offset
	//     @param4 u8  staticTextureEn            //Whether to draw to static storage space
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus drawPageWithAnimation(
		DynamicPageClass* pCurPage  , //Current page pointer
		TileBoxClassPtr tileBox     , //source box
		u32* pSourceShift           , //sourcebuffer offset
		u8  staticTextureEn			  //Whether to draw to static storage space
		);

	//-----------------------------
	// name  refreshWidgetBox
	// refresh widget box
	//  
	//     @param1 WidgetClassPtr pWidget,
	//     @param2 CanvasClassPtr pCanvas,
	//     @param3 TextureClassPtr pTexture,
	//     @param4 u8 textureEnable,
	//     @param5 u8 widgetEnable,
	//     @param6 u8 canvasEnable,
	//     @param7 u8 doubleBufferEnable,
	//     @param8 TileBoxClassPtr pTileBox
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus refreshWidgetBox(
	    WidgetClassPtr pWidget,
		CanvasClassPtr pCanvas,
		TextureClassPtr pTexture,
		u8 textureEnable,
		u8 widgetEnable,
		u8 canvasEnable,
		u8 doubleBufferEnable,
		TileBoxClassPtr pTileBox
	);

	//-----------------------------
	// name  RefreshOSCWidgetBox
	// refresh OSC widget box
	//  
	//     @param1 WidgetClassPtr pWidget,
	//     @param2 TileBoxClassPtr pTileBox
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus RefreshOSCWidgetBox(
		WidgetClassPtr pWidget,
		TileBoxClassPtr pTileBox
	);

	//-----------------------------
	// name  GetWidgetList
	// get widget list
	//  
	//     @param1 u8 Value
	// addtional comments :
	//   none
	//-----------------------------
	WidgetClassPtr GetWidgetList(u8 Value);

	//-----------------------------
	// name  GetBackgroundTexture
	// get background texture
	//  
	//     @param1 none
	// addtional comments :
	//   none
	//-----------------------------
	TextureClassPtr GetBackgroundTexture();

	//-----------------------------
	// name  initHighlightWidget
	// initialize highlight widget
	//  
	//     @param1 u8 systemInitial
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus initHighlightWidget(u8 systemInitial);

	//-----------------------------
	// name  setATag
	// Animation tag changed
	//  
	//     @param1 u8 value  Changed value
	// addtional comments :
	//   none
	//-----------------------------
	u32 setATag(u16 value);

	void LoadNextPageAfterReleasingAnimation(void);

	void LoadNextPageAfterMultiCardReleasingAnimation(s32 x, s32 y);
	
	u32 mouseTouchCtrl(
		ActionTriggerClassPtr ActionPtr,
		PIDPoint* pPressPoint,
		PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
		PIDPoint* pAbsoluteReleasePoint,
		u8 u8_pageRefresh      
	);

	void updatePageListScale(s16 offsetforAlign, s32 offset, u8 direction, u8 isReset = 0);
	
	void updatePageListOffsetForMultiCard(s16 offset, u8 direction, u8 isReset = 0, u8 offsetMode = 0);

	void updatePageListOffset(s16 offset, u8 direction, u8 isReset = 0);

	void updatePageListOffsetAbsolute(s16 offset, u8 direction, u8 isReset = 0);

	//-----------------------------
	// name  inoutTransitioin
	// Computation of slowly varying functions
	// if(x < 0.5) y = 4*x^3
	// else        y = 4(x-1)^3 + 1
	//  
	//     @param1 s16 in  0.10 
	//     @param2 s16 out 0.10
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus inoutTransitioin(
		u16 in,   //1.11.4
		u16 &out  //1.11.4
		);

	//-----------------------------
	// name  writeBackGround
	// Background for writing static caching
	//  
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus writeBackGround(
		u32 *psourceshift, 
		NewMatrixPtr addtionalMatrix
		);

	//-----------------------------
	// name  clearPage
	// Write a solid black background.
	//  
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus clearPage(
		u32 *psourceshift
		);
        
    //-----------------------------
	// name  UpdateCanvas
	// Update focused subCanvas.
	//  
    
	// addtional comments :
	//   none
	//-----------------------------
	u32 UpdateCanvas(void);

};

typedef DynamicPageClass* DynamicPageClassPtr;

#define  NullRefreshFlag	        (0x00)
#define  WidgetNormalRefreshFlag	(0x01)
#define  PageNormalRefreshFlag		(0x02)
#define  PageMatrixRefreshFlag		(0x04)
#define  RefreshFlagMusk            (0xFFFF)

#endif

#endif

