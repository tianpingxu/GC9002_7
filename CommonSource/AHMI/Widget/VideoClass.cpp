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
// Revision:
// Revision 1.6.2 - Modify 2019/07/16 by YuChunYing
// Additional Comments:
//      整合奇偶帧融合，去除四路视频输入，只保留单路
//   
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "VideoClass.h"
#include "drawImmediately_cd.h"
#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi_bt656.h"
#endif

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;

extern k_msgq_handle_t   RefreshQueueWithoutDoubleBuffer;
extern  u8 video_cur_w0;           
extern  u8 video_cur_r0 ;   
extern  u8 video_next_r0;

#define CAMERA_RESOLUTION_WIDTH         720
#define CAMERA_RESOLUTION_HEIGTH        480

#ifdef EMBEDDED
#ifdef VERSION_2
uint16_t CameraWidth = 0;//CAMERA_RESOLUTION_WIDTH;
uint16_t CameraHeight = 0;//CAMERA_RESOLUTION_HEIGTH;

extern u8 normalvideoframe;

#if defined(CHIP_GC9002)
#define  RAMOFFSET_FOR_GPU          0
#elif defined(CHIP_GC9005)
#define  RAMOFFSET_FOR_GPU          0x60000000
#else
#define  RAMOFFSET_FOR_GPU          0x80000000
#endif
#endif
#endif

#define CUT_PIXEL_X 00 //how many pixels to cut in x
#define CUT_PIXEL_Y 00 //how many pixels to cut in y
#define MOVE_PIXEL_Y 0 //how many pixels to move in y


//-----------------------------
// name： MeterClass
// 构造函数
//  
//
// addtional comments :
//   none
//-----------------------------
VideoClass::VideoClass()
{

}

//-----------------------------
// name： ~MeterClass
// 析构函数
//  
//
// addtional comments :
//   none
//-----------------------------
VideoClass::~VideoClass()
{

}

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
funcStatus VideoClass::initWidget(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,   
	u32 *u32p_sourceShift,
	NewMatrixPtr pWidgetMatrix,
	u8 u8_pageRefresh,       
	u8 RefreshType ,         
	TileBoxClassPtr pTileBox,
	u8 staticTextureEn       
	)
{
	WidgetClassInterface myWidgetClassInterface;
	ActionTriggerClass tagtrigger;
//	TagClassPtr bindTag;

	if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	
	//bindTag = &TagPtr[p_wptr->BindTagID];
	tagtrigger.mTagPtr = NULL;
	tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
	if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,1) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
    
	#if defined(EMBEDDED) && defined(VERSION_2)
	if(normalvideoframe >= VIDEO_FIRST_FRAME_NUMBER)
	#endif
	{
		if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
//  @param u8 RefreshType             //the type of animation
// addtional comments :
//   none
//-----------------------------
funcStatus VideoClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,              
	u8 u8_pageRefresh				
	)
{
	TextureClassPtr videoTexturePtr;// = &(gPagePtr[WorkingPageID].pBasicTextureList[p_wptr->StartNumofTex]);
	u8 videoType = (p_wptr->WidgetAttr & VIDEO_TYPE_MASK);
	u16 hdmi_width;
	u16 hdmi_height;
//	uint32_t value;
	u8 video_next_r_temp = 0;

	if(NULL == p_wptr)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	
	if(videoType == VIDEO_TYPE_DVP || videoType == VIDEO_TYPE_RGB)
	{
		//get the video texture
		videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
		
#ifdef EMBEDDED 
		//read the video width and height from spi
		//readVideoWidthAndHeight(&hdmi_width, &hdmi_height);
		//hdmi_width = 1280;
		//hdmi_height = 480;

		//now is fixed
		#ifdef EMBEDDED
		BT656_GetParam(&CameraHeight, &CameraWidth);
        #endif
	
        videoTexturePtr->TexWidth = CameraWidth;
        videoTexturePtr->TexHeight = CameraHeight;
        videoTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / videoTexturePtr->TexWidth;  //the scaler is widget size/ texture size
        videoTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / videoTexturePtr->TexHeight; //the scaler is widget size/ texture size
		
        #if defined(CHIP_GC9002)
        if(video_cur_w0 == 1)
            video_next_r0 = 0;
        else
            video_next_r0 = 1;
        #elif defined(CHIP_GC9005)
        #if (VIDEO_BUFFER_NUM == 3)
        if(video_cur_w0 == (video_cur_r0 + 2) % 3)
            video_next_r0 = (video_cur_r0 + 1) % 3;
        else
            video_next_r0 = video_cur_r0;
        #elif (VIDEO_BUFFER_NUM == 2)
        if(video_cur_w0 == 1)
            video_next_r0 = 0;
        else
            video_next_r0 = 1;
        #endif
        #else
        #endif
			
		video_next_r_temp = video_next_r0;

		videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer);

#endif
#ifdef PC_SIM
	    //now is fixed
		hdmi_width = 720;
		hdmi_height = 576;
		videoTexturePtr->TexWidth = hdmi_width;
	    videoTexturePtr->TexHeight = hdmi_height;
		videoTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / videoTexturePtr->TexWidth;  //the scaler is widget size/ texture size
	    videoTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / videoTexturePtr->TexHeight; //the scaler is widget size/ texture size
        videoTexturePtr->TexAddr = START_ADDR_OF_RAM;
#endif
	}
	else if(videoType == VIDEO_TYPE_AHD)
	{
		//get the video texture
		videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
		
#ifdef EMBEDDED 

		BT656_GetParam(&CameraHeight, &CameraWidth);
		
        videoTexturePtr->TexWidth = CameraWidth;
        videoTexturePtr->TexHeight = CameraHeight;
        videoTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / videoTexturePtr->TexWidth;  //the scaler is widget size/ texture size
        videoTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / videoTexturePtr->TexHeight; //the scaler is widget size/ texture size
		
		#if defined(CHIP_GC9002)
		if(video_cur_w0 == 1)
			video_next_r0 = 0;
		else
			video_next_r0 = 1;
		#elif defined(CHIP_GC9005)
        #if (VIDEO_BUFFER_NUM == 3)
        if(video_cur_w0 == (video_cur_r0 + 2) % 3)
            video_next_r0 = (video_cur_r0 + 1) % 3;
        else
            video_next_r0 = video_cur_r0;
        #elif (VIDEO_BUFFER_NUM == 2)
        if(video_cur_w0 == 1)
            video_next_r0 = 0;
        else
            video_next_r0 = 1;
        #endif
		#else
		#endif
		video_next_r_temp = video_next_r0;
		videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer);

#endif
#ifdef PC_SIM
	    //now is fixed
		hdmi_width = 720;
		hdmi_height = 576;
		videoTexturePtr->TexWidth = hdmi_width;
	    videoTexturePtr->TexHeight = hdmi_height;
		videoTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / videoTexturePtr->TexWidth;  //the scaler is widget size/ texture size
	    videoTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / videoTexturePtr->TexHeight; //the scaler is widget size/ texture size
        videoTexturePtr->TexAddr = START_ADDR_OF_RAM;
#endif
	}
	else if(videoType == VIDEO_TYPE_CVBS)
	{
		//get the video texture 
		// 0 clear PUREMASK
		// 1 PUREMASK for overscan
		// 2 the video texture
		// 3 PUREMASK for overscan
		// 4 the odd video texture
		// now the default overscan is 10% for around.
		// if want to change the overscan value, refer to  <video overscan mannual>

		if (CVBS_REMOVING_BLACK_BORDER == 1)
		{
			videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex+2]);
		}else
		{
			videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
		}

		#ifdef EMBEDDED
		
		//read the width and height
        #ifdef VERSION_2
            #if defined(VIDEO_EVEN) || defined(VIDEO_ODD)
			BT656_GetParam(&CameraHeight, &CameraWidth);
            #endif
            //read the width and height
            #ifdef VIDEO_ODD_And_EVEN
		    hdmi_width = CameraWidth;
		    hdmi_height = CameraHeight/2;
            #else 
	    	hdmi_width = CameraWidth;
		    hdmi_height = CameraHeight;
            #endif
        #endif
		
		if(hdmi_width == 0 || hdmi_height == 0)
		{
            #ifdef VERSION_2
            hdmi_width = CAMERA_RESOLUTION_WIDTH;
			hdmi_height = CAMERA_RESOLUTION_HEIGTH/2;
            #endif
		}

		videoTexturePtr->TexWidth = hdmi_width;
		videoTexturePtr->TexHeight = hdmi_height;
		
		videoTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / hdmi_width + 1;  //the scaler is widget size/ texture size
		videoTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / hdmi_height + 1; //the scaler is widget size/ texture size

		#if defined(CHIP_GC9002)
		if(video_cur_w0 == 1)
			video_next_r0 = 0;
		else
			video_next_r0 = 1;
		#elif defined(CHIP_GC9005)
        #if (VIDEO_BUFFER_NUM == 3)
        if(video_cur_w0 == (video_cur_r0 + 2) % 3)
            video_next_r0 = (video_cur_r0 + 1) % 3;
        else
            video_next_r0 = video_cur_r0;
        #elif (VIDEO_BUFFER_NUM == 2)
        if(video_cur_w0 == 1)
            video_next_r0 = 0;
        else
            video_next_r0 = 1;
        #endif
		#endif
		video_next_r_temp = video_next_r0;
		
        #ifdef VIDEO_ODD_EVEN_ALL
		videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer) - hdmi_width * MOVE_PIXEL_Y;
        #endif //VIDEO_ODD_EVEN_ALL
        
		#ifdef VIDEO_ODD_And_EVEN
        videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer) - 2 * hdmi_width * MOVE_PIXEL_Y;
        #endif

		#ifdef VIDEO_ODD
        videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Odd) + video_next_r_temp * (SizeOfCVBSBuffer) - 2 * hdmi_width * MOVE_PIXEL_Y;
        #endif

		#ifdef VIDEO_EVEN
        videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer) - 2 * hdmi_width * MOVE_PIXEL_Y;
        #endif

		#endif
#ifdef PC_SIM
		hdmi_width = 720;
		hdmi_height = 576;
		videoTexturePtr->TexWidth = hdmi_width;
		videoTexturePtr->TexHeight = hdmi_height;
        videoTexturePtr->TexAddr = (START_ADDR_OF_RAM);
#endif
#ifdef EMBEDDED
		videoTexturePtr->ScalerX =(((p_wptr->WidgetWidth + CUT_PIXEL_X) * 512 / hdmi_width) + 1);
		videoTexturePtr->ScalerY = (((p_wptr->WidgetHeight + CUT_PIXEL_Y) * 512 / hdmi_height) + 1);
#endif
#ifdef PC_SIM
		videoTexturePtr->ScalerX = (((p_wptr->WidgetWidth + CUT_PIXEL_X) * 512 / hdmi_width) + 1);
		videoTexturePtr->ScalerY = (((p_wptr->WidgetHeight + CUT_PIXEL_Y) * 512 / hdmi_height) + 1);
#endif
		//videoTexturePtr->OffsetX = (p_wptr->WidgetOffsetX << 4) - (CUT_PIXEL_X << 4) / 2;
		//videoTexturePtr->OffsetY = (p_wptr->WidgetOffsetY << 4) - (CUT_PIXEL_Y << 4) / 2;
		videoTexturePtr->OffsetX = (0) - (CUT_PIXEL_X << 4) / 2;
		videoTexturePtr->OffsetY = (0) - (CUT_PIXEL_Y << 4) / 2;

		//compute the source box
		//videoTexturePtr->renewTextureSourceBox(NULL,NULL,NULL);
		//videoTexturePtr->computeTextureBoxbyWidget(NULL,NULL,p_wptr,NULL);

		if (CVBS_REMOVING_BLACK_BORDER == 1)
		{
		videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 4]);

		#ifdef EMBEDDED

		videoTexturePtr->TexWidth = hdmi_width;
		videoTexturePtr->TexHeight = hdmi_height;
		
		videoTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / hdmi_width + 1;  //the scaler is widget size/ texture size
		videoTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / hdmi_height + 1; //the scaler is widget size/ texture size
		
		#ifdef VIDEO_ODD_EVEN_ALL
		videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer) - 2 * hdmi_width * MOVE_PIXEL_Y;
        #endif

		#ifdef VIDEO_ODD_And_EVEN
        videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Odd) + video_next_r_temp * (SizeOfCVBSBuffer) - 2 * hdmi_width * MOVE_PIXEL_Y;
        #endif

		#ifdef VIDEO_ODD
        videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Odd) + video_next_r_temp * (SizeOfCVBSBuffer) - 2 * hdmi_width * MOVE_PIXEL_Y;
        #endif

		#ifdef VIDEO_EVEN
        videoTexturePtr->TexAddr = (RAMOFFSET_FOR_GPU) + (StartAddrOfCVBS0Even) + video_next_r_temp * (SizeOfCVBSBuffer) - 2 * hdmi_width * MOVE_PIXEL_Y;
        #endif

		#endif
#ifdef PC_SIM
		hdmi_width = 720;
		hdmi_height = 576;
		videoTexturePtr->TexWidth = hdmi_width;
		videoTexturePtr->TexHeight = hdmi_height;
        videoTexturePtr->TexAddr = (START_ADDR_OF_RAM);

#endif
#ifdef EMBEDDED
		videoTexturePtr->ScalerX =(((p_wptr->WidgetWidth + CUT_PIXEL_X) * 512 / hdmi_width) + 1);
		videoTexturePtr->ScalerY =(((p_wptr->WidgetHeight + CUT_PIXEL_Y) * 512 / hdmi_height) + 1);
#endif
#ifdef PC_SIM
		videoTexturePtr->ScalerX = (((p_wptr->WidgetWidth + CUT_PIXEL_X) * 512 / hdmi_width) + 1);
		videoTexturePtr->ScalerY = (((p_wptr->WidgetHeight + CUT_PIXEL_Y) * 512 / hdmi_height) + 1);
#endif
		//videoTexturePtr->OffsetX = (p_wptr->WidgetOffsetX << 4) - (CUT_PIXEL_X << 4) / 2;
		videoTexturePtr->OffsetX = (0) - (CUT_PIXEL_X << 4) / 2;
		
		#ifdef VIDEO_EVEN_FIRST
		/*videoTexturePtr->OffsetY = (p_wptr->WidgetOffsetY << 4) - (CUT_PIXEL_Y << 4) / 2 - (1 << 4) * videoTexturePtr->ScalerY / 512 / 2;*/
		videoTexturePtr->OffsetY = (0) - (CUT_PIXEL_Y << 4) / 2 - (1 << 4) * videoTexturePtr->ScalerY / 512 / 2;
		#endif
		#ifdef VIDEO_ODD_FIRST
		/*videoTexturePtr->OffsetY = (p_wptr->WidgetOffsetY << 4) - (CUT_PIXEL_Y << 4) / 2 + (1 << 4) * videoTexturePtr->ScalerY / 512 / 2;*/
		videoTexturePtr->OffsetY = (0) - (CUT_PIXEL_Y << 4) / 2 + (1 << 4) * videoTexturePtr->ScalerY / 512 / 2;
		#endif
		#if !defined(VIDEO_EVEN_FIRST) && !defined(VIDEO_ODD_FIRST)
		/*videoTexturePtr->OffsetY = (p_wptr->WidgetOffsetY << 4) - (CUT_PIXEL_Y << 4) / 2;*/
		videoTexturePtr->OffsetY = (0) - (CUT_PIXEL_Y << 4) / 2;
		#endif
		
		//compute the source box
		//videoTexturePtr->computeTextureBoxbyWidget(NULL,NULL,p_wptr,NULL);

		//when VIDEO_EVEN or VIDEO_ODD, 3 4 not display
		#if defined(VIDEO_EVEN) || defined(VIDEO_ODD) || defined(VIDEO_ODD_EVEN_ALL)
		videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 3]);
		videoTexturePtr->TexAddr = 0;
		videoTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
		
		videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 4]);
		videoTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
		#endif

		//modify the puremask
		videoTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
#ifdef EMBEDDED
		videoTexturePtr->TexWidth = p_wptr->WidgetWidth + CUT_PIXEL_X;
		videoTexturePtr->TexHeight = p_wptr->WidgetHeight + CUT_PIXEL_Y;
#endif
#ifdef PC_SIM
		videoTexturePtr->TexWidth = p_wptr->WidgetWidth + CUT_PIXEL_X;
		videoTexturePtr->TexHeight = p_wptr->WidgetHeight + CUT_PIXEL_Y;
#endif
		//compute the source box
		//videoTexturePtr->computeTextureBoxbyWidget(NULL,NULL,p_wptr,NULL);
		}
	}

	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Tag.setValue
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus VideoClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
    
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//**************************************
funcStatus VideoClass::MouseTouchCtrl(
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    PIDPoint* pPressPoint,
    PIDPoint* pReleasePoint,
    PIDPoint* pAbsolutePressPoint,
    PIDPoint* pAbsoluteReleasePoint,
    u8 u8_pageRefresh      
    )
{
    
    if(p_wptr->TouchPropagation & TouchPropagationSet)
	{   
        /* Touch propagation. */
        return WIDGET_TOUCH_CANCEL;
	}
    
    /* Don,t propagation. */
    return AHMI_FUNC_SUCCESS;
}
     

//**************************************
//reponse for Keyboard.Touch
//
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus VideoClass::KeyboardTouchCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_pageRefresh      
    )
{
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
    
	return AHMI_FUNC_SUCCESS;
}

#endif
