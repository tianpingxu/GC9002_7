////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2024/5/20
// File Name:     VideoPlayer.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2024/5/27 by mazy
// Additional Comments:
// Revision:
// Revision
// Additional Comments:
//
//
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "VideoPlayer.h"
#include "drawImmediately_cd.h"
#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi.h"
#include "h264decode.h"
#endif
#if defined (PC_SIM)
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}
#endif

#if defined (CHIP_GC9005) || defined (PC_SIM)

#ifdef AHMI_CORE
extern TagClassPtr          TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ConfigInfoClass ConfigData;

extern  k_msgq_handle_t   RefreshQueueWithoutDoubleBuffer;


#define  RAMOFFSET_FOR_GPU          0x60000000

#if defined (PC_SIM)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
typedef struct {
	AVFormatContext *format_ctx_;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	int video_index;
	AVPacket *packet;
	AVFrame *pframe;
	AVFrame *pframeRGB;
	SwsContext *pSwsCtx;

	uint32_t h264DataStartPos;
	uint32_t h264DataSize;
	uint32_t soundStartPos;
	uint32_t soundSize;

	uint32_t lastPlayPicAHMITickMs;

	uint32_t h264DecodeSuccessLen;
	uint8_t* dataRamPtr;
	uint32_t dataRamLen;
} H264DecodeSt;
H264DecodeSt *gH264DecodeSt = NULL;
uint8_t* gPicBufPtr = NULL;
uint32_t *gVideoPlayWidgetClassPtr = NULL;
volatile uint32_t gCurrentH264DecodePageId = 0;
extern u8 H264PicBuffer[SIZE_OF_SingleH264Pic_BUF];
extern "C" void ExtiTrigH264PlayHandler(uint32_t curAHMITick);
#endif

VideoPlayerClass::VideoPlayerClass()
{

}

VideoPlayerClass::~VideoPlayerClass()
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
funcStatus VideoPlayerClass::initWidget(
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

	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
    {
        //bindTag = &TagPtr[p_wptr->BindTagID];
        tagtrigger.mTagPtr = NULL;
        tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
#if defined(CHIP_GC9005)
        uint32_t pageId = p_wptr->ATTATCH_PAGE;
        if((NULL != gH264BaseInfoPtr) && (NULL != gH264BaseInfoPtr[pageId].dataRamPtr) && u8_pageRefresh)
        {
            H264_DecodeInit(pageId);
        }
#endif
        if(widgetCtrl(pagePtr, p_wptr, &tagtrigger, 1) != AHMI_FUNC_SUCCESS)
            return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);

        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
    }
    
    if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
        return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);

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
funcStatus VideoPlayerClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh
	)
{
	TextureClassPtr videoPlayerTexturePtr;
	DynamicPageClassPtr pCurrentPage;

	if(NULL == p_wptr)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

    pCurrentPage = pagePtr;
    uint32_t pageId = p_wptr->ATTATCH_PAGE;

#if defined(CHIP_GC9005)
    if(u8_pageRefresh)
    {
        videoPlayerTexturePtr = &pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex];

        if(NULL == gH264BaseInfoPtr)
        {
            uint32_t size = (ConfigData.NumofPages - ConfigData.NumofSysPage) * sizeof(H264BaseInfoSt);
            gH264BaseInfoPtr = (H264BaseInfoSt*)csi_kernel_malloc(size, NULL);
            if(NULL == gH264BaseInfoPtr)
            {
                ERROR_PRINT("ERROR: malloc failed");
                return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
            }
            memset(gH264BaseInfoPtr, 0, size);
        }

        if(NULL == gPicBufPtr_PP)
        {
            uint32_t width  = ((p_wptr->VideoH264MaxPicBufWidth  >> 4) + 1) << 4;  //must be n*16
            uint32_t height = ((p_wptr->VideoH264MaxPicBufHeight >> 4) + 1) << 4; //must be n*16
            gVideoPlayerPicMaxWidth = width;
            gVideoPlayerPicMaxHeight = height;
            gPicBufPtr_PP = (uint8_t *)csi_kernel_malloc(width*height*H264_DECODE_PIC_BUF_NUM+16, NULL);
            if(NULL == gPicBufPtr_PP)
            {
                ERROR_PRINT("ERROR: malloc failed");
                return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
            }

            uint32_t addrOffset = (uint32_t)gPicBufPtr_PP % 16;
            gPicBufPtr_PP += 16 - addrOffset;
        }

        if(NULL == gH264BaseInfoPtr[pageId].dataRamPtr)
        {
            uint32_t h264TexSize = ((uint32_t)(p_wptr->VideoH264TextureSizeH) << 16) + p_wptr->VideoH264TextureSizeL;
            uint32_t soundTexSize = ((uint32_t)(p_wptr->VideoSoundTextureSizeH) << 16) + p_wptr->VideoSoundTextureSizeL;
            uint32_t h264MaxFrameSize = ((uint32_t)(p_wptr->VideoMaxFrameSizeH) << 16) + p_wptr->VideoMaxFrameSizeL;
            gH264BaseInfoPtr[pageId].dataOriginPos = videoPlayerTexturePtr->TexAddr;
            gH264BaseInfoPtr[pageId].dataOriginSize = h264TexSize;
            gH264BaseInfoPtr[pageId].soundOriginPos = videoPlayerTexturePtr->TexAddr + h264TexSize;
            gH264BaseInfoPtr[pageId].soundOriginSize = soundTexSize;

            gH264BaseInfoPtr[pageId].dataRamPtr = (uint8_t *)csi_kernel_malloc(h264MaxFrameSize, NULL);
            if(NULL == gH264BaseInfoPtr[pageId].dataRamPtr)
            {
                ERROR_PRINT("ERROR: malloc failed");
                return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
            }
            gH264BaseInfoPtr[pageId].dataRamLen = h264MaxFrameSize;
            gH264BaseInfoPtr[pageId].isLoopPlay = p_wptr->VideoPlayerLoopPlay;
            gH264BaseInfoPtr[pageId].widgetClassPtr = (uint32_t*)p_wptr;
            gH264BaseInfoPtr[pageId].configAudioSampleRate = ConfigData.audioSampleRate;

            H264_DecodeInit(pageId);
            videoPlayerTexturePtr->TexWidth = gH264BaseInfoPtr[pageId].decInfo.picWidth;
            videoPlayerTexturePtr->TexHeight = gH264BaseInfoPtr[pageId].decInfo.picHeight;
            videoPlayerTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / videoPlayerTexturePtr->TexWidth;  //the scaler is widget size/ texture size
            videoPlayerTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / videoPlayerTexturePtr->TexHeight; //the scaler is widget size/ texture size
            videoPlayerTexturePtr->mTexAttr &= 0xFFFFFF0F;
            videoPlayerTexturePtr->mTexAttr |= PURECOLOR;
            videoPlayerTexturePtr->TexAddr = 0xFF000000;
        }
        else
        {
            if(p_wptr->VideoPlayerAutoPlay)
            {
                uint32_t textureAddr = Get_AvaliableDecodedPic(pageId);
                if(textureAddr > 0)
                {
                    videoPlayerTexturePtr->mTexAttr &= 0xFFFFFF0F;
                    videoPlayerTexturePtr->mTexAttr |= SFBC;
                    videoPlayerTexturePtr->TexAddr = (textureAddr - START_ADDR_OF_RAM) | 0x80000000;
                }
                else
                {
                    videoPlayerTexturePtr->mTexAttr &= 0xFFFFFF0F;
                    videoPlayerTexturePtr->mTexAttr |= PURECOLOR;
                    videoPlayerTexturePtr->TexAddr = 0xFF000000;
                }
            }
        }
    }

#elif defined (PC_SIM)
	if(u8_pageRefresh)
	{
		videoPlayerTexturePtr = &pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex];

		if(NULL == gH264DecodeSt)
        {
            uint32_t size = (ConfigData.NumofPages - ConfigData.NumofSysPage) * sizeof(H264DecodeSt);
            gH264DecodeSt = (H264DecodeSt*)csi_kernel_malloc(size, NULL);
            if(NULL == gH264DecodeSt)
            {
                ERROR_PRINT("ERROR: malloc failed");
                return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
            }
            memset(gH264DecodeSt, 0, size);
        }

		if(NULL == gPicBufPtr)
        {
            uint32_t width  = ((gScreenWidth >> 4) + 1) << 4;  //must be n*16
            uint32_t height = ((gScreenHeight >> 4) + 1) << 4; //must be n*16
            gPicBufPtr = (uint8_t *)csi_kernel_malloc(width*height*2+16, NULL);
            if(NULL == gPicBufPtr)
            {
                ERROR_PRINT("ERROR: malloc failed");
                return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
            }

            uint32_t addrOffset = (uint32_t)gPicBufPtr % 16;
            gPicBufPtr += 16 - addrOffset;
        }

		if(NULL == gH264DecodeSt[pageId].format_ctx_)
		{
			uint32_t h264TexSize = ((uint32_t)(p_wptr->VideoH264TextureSizeH) << 16) + p_wptr->VideoH264TextureSizeL;
            uint32_t soundTexSize = ((uint32_t)(p_wptr->VideoSoundTextureSizeH) << 16) + p_wptr->VideoSoundTextureSizeL;
            uint32_t h264MaxFrameSize = ((uint32_t)(p_wptr->VideoMaxFrameSizeH) << 16) + p_wptr->VideoMaxFrameSizeL;

			gH264DecodeSt[pageId].h264DataSize = h264TexSize;
			gH264DecodeSt[pageId].soundSize = soundTexSize;

			gH264DecodeSt[pageId].h264DataStartPos = videoPlayerTexturePtr->TexAddr;
			gH264DecodeSt[pageId].soundStartPos = videoPlayerTexturePtr->TexAddr + h264TexSize;;
			
			gH264DecodeSt[pageId].dataRamPtr = (uint8_t *)csi_kernel_malloc(h264MaxFrameSize, NULL);
            if(NULL == gH264DecodeSt[pageId].dataRamPtr)
            {
                ERROR_PRINT("ERROR: malloc failed");
                return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
            }
            gH264DecodeSt[pageId].dataRamLen = h264MaxFrameSize;

			extern FILE *fp;
			long int pos = ftell(fp);
			fseek(fp, gH264DecodeSt[pageId].h264DataStartPos, SEEK_SET);

			char tmpH264File[100] = {'\0'};
			//sprintf(tmpH264File, "..\\..\\..\\testData_0304\\tmp_%d.h264", pageId);
			sprintf(tmpH264File, "tmp_%d.h264", pageId);
			remove(tmpH264File);
			FILE* h264Fp = fopen(tmpH264File, "wb+");
			for(int i=0; i<(gH264DecodeSt[pageId].h264DataSize/h264MaxFrameSize+1); i++)
			{
				if((gH264DecodeSt[pageId].h264DataSize - i*h264MaxFrameSize) < h264MaxFrameSize)
				{
					fread(gH264DecodeSt[pageId].dataRamPtr, 1, (gH264DecodeSt[pageId].h264DataSize - i*h264MaxFrameSize), fp);
					fwrite(gH264DecodeSt[pageId].dataRamPtr, (gH264DecodeSt[pageId].h264DataSize - i*h264MaxFrameSize), 1, h264Fp);
				}
				else
				{
					fread(gH264DecodeSt[pageId].dataRamPtr, 1, h264MaxFrameSize, fp);
					fwrite(gH264DecodeSt[pageId].dataRamPtr, h264MaxFrameSize, 1, h264Fp);
				}
			}
			fclose(h264Fp);

			uint32_t copyLen = MIN(gH264DecodeSt[pageId].h264DataSize, gH264DecodeSt[pageId].dataRamLen);
			fseek(fp, gH264DecodeSt[pageId].h264DataStartPos, SEEK_SET);
			fread(gH264DecodeSt[pageId].dataRamPtr, 1, copyLen, fp);
			if(gH264DecodeSt[pageId].h264DataSize > gH264DecodeSt[pageId].dataRamLen)
			{
				uint32_t preStartCodePos = 0;
				for(uint32_t i=0; i<(copyLen-4); i++)
				{
					uint32_t *pt = (uint32_t *)(gH264DecodeSt[pageId].dataRamPtr + copyLen - 5 - i);
					if((*pt & 0xFFFFFF00) == 0x01000000)
					{
						uint8_t type = *((uint8_t *)(pt) + 4) & 0x1F;
						if((preStartCodePos > 0) && ((type == 0x05) || (type == 0x01)))
						{
							copyLen = preStartCodePos;
							break;
						}
						preStartCodePos = copyLen - 5 - i;
					}
				}
			}

			avformat_network_init();
			gH264DecodeSt[pageId].format_ctx_ = avformat_alloc_context();
			avformat_open_input(&(gH264DecodeSt[pageId].format_ctx_), tmpH264File, nullptr, nullptr);
			gH264DecodeSt[pageId].video_index = avformat_find_stream_info(gH264DecodeSt[pageId].format_ctx_, NULL);
			int id = gH264DecodeSt[pageId].video_index;
			if(id < 0)
			{
				avformat_close_input(&(gH264DecodeSt[pageId].format_ctx_));
				gH264DecodeSt[pageId].format_ctx_ = NULL;
				ERROR_PRINT("texture is decode error");
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
			}

			if (gH264DecodeSt[pageId].format_ctx_->streams[id]->codec->codec_type != AVMEDIA_TYPE_VIDEO)
			{
				avformat_close_input(&(gH264DecodeSt[pageId].format_ctx_));
				gH264DecodeSt[pageId].format_ctx_ = NULL;
				ERROR_PRINT("texture is not video");
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
			}

			gH264DecodeSt[pageId].pCodecCtx = gH264DecodeSt[pageId].format_ctx_->streams[id]->codec;
			gH264DecodeSt[pageId].pCodec = avcodec_find_decoder(gH264DecodeSt[pageId].pCodecCtx->codec_id);
			avcodec_open2(gH264DecodeSt[pageId].pCodecCtx, gH264DecodeSt[pageId].pCodec, NULL);

			gH264DecodeSt[pageId].pframe = av_frame_alloc();
			gH264DecodeSt[pageId].pframeRGB = av_frame_alloc();
			avpicture_fill((AVPicture *)(gH264DecodeSt[pageId].pframeRGB), gPicBufPtr, AV_PIX_FMT_RGBA, gH264DecodeSt[pageId].pCodecCtx->width, gH264DecodeSt[pageId].pCodecCtx->height);
			gH264DecodeSt[pageId].pSwsCtx = sws_alloc_context();
			gH264DecodeSt[pageId].pSwsCtx = sws_getContext(gH264DecodeSt[pageId].pCodecCtx->width, gH264DecodeSt[pageId].pCodecCtx->height, gH264DecodeSt[pageId].pCodecCtx->pix_fmt,
					gH264DecodeSt[pageId].pCodecCtx->width, gH264DecodeSt[pageId].pCodecCtx->height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);

			gH264DecodeSt[pageId].packet = av_packet_alloc();
		}

		gVideoPlayWidgetClassPtr = (uint32_t*)p_wptr;
		gCurrentH264DecodePageId = pageId;
	}

	if(p_wptr->VideoPlayerAutoPlay)
	{
		av_init_packet(gH264DecodeSt[pageId].packet);
		int got_frame = -1;
		if( av_read_frame(gH264DecodeSt[pageId].format_ctx_, gH264DecodeSt[pageId].packet) >= 0 ) {
			if( gH264DecodeSt[pageId].packet->stream_index == gH264DecodeSt[pageId].video_index ) {
				int ret = avcodec_decode_video2(gH264DecodeSt[pageId].pCodecCtx, gH264DecodeSt[pageId].pframe, &got_frame, gH264DecodeSt[pageId].packet);
				if(ret < 0) {
					av_packet_free(&gH264DecodeSt[pageId].packet);
					sws_freeContext(gH264DecodeSt[pageId].pSwsCtx);
					av_frame_free(&(gH264DecodeSt[pageId].pframe));
					av_frame_free(&(gH264DecodeSt[pageId].pframeRGB));
					avformat_close_input(&(gH264DecodeSt[pageId].format_ctx_));
					gH264DecodeSt[pageId].format_ctx_ = NULL;
					ERROR_PRINT("avcodec_decode_video2 error");
					return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
				}
				gH264DecodeSt[pageId].h264DecodeSuccessLen += ret;
				if (got_frame > 0){
					sws_scale(gH264DecodeSt[pageId].pSwsCtx,(const uint8_t* const*)gH264DecodeSt[pageId].pframe->data, 
						gH264DecodeSt[pageId].pframe->linesize, 0, gH264DecodeSt[pageId].pframe->height, 
						gH264DecodeSt[pageId].pframeRGB->data, gH264DecodeSt[pageId].pframeRGB->linesize);
				}
			}
			av_packet_unref(gH264DecodeSt[pageId].packet);
	
			ExtiTrigH264PlayHandler(0);
		}
		else
		{
			if(p_wptr->VideoPlayerLoopPlay)
			{
				int ret = av_seek_frame(gH264DecodeSt[pageId].format_ctx_, gH264DecodeSt[pageId].video_index, 0, AVSEEK_FLAG_BYTE);
				if(ret < 0)
				{
					ERROR_PRINT("av_seek_frame error");
					return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
				}
				avcodec_flush_buffers(gH264DecodeSt[pageId].pCodecCtx);
			}
		}
	}
	videoPlayerTexturePtr->mTexAttr &= 0xFFFFFF0F;
	videoPlayerTexturePtr->mTexAttr |= RGBA8888;
	uint32_t texAddr = START_ADDR_OF_H264Pic + START_ADDR_OFFSET;
	memcpy(&H264PicBuffer[0], gPicBufPtr, gH264DecodeSt[pageId].pCodecCtx->width*gH264DecodeSt[pageId].pCodecCtx->height*3);
	videoPlayerTexturePtr->TexWidth = gH264DecodeSt[pageId].pCodecCtx->width;
	videoPlayerTexturePtr->TexHeight = gH264DecodeSt[pageId].pCodecCtx->height;
	videoPlayerTexturePtr->ScalerX = p_wptr->WidgetWidth * 512 / videoPlayerTexturePtr->TexWidth;  //the scaler is widget size/ texture size
	videoPlayerTexturePtr->ScalerY = p_wptr->WidgetHeight * 512 / videoPlayerTexturePtr->TexHeight; //the scaler is widget size/ texture size
	videoPlayerTexturePtr->TexAddr = texAddr;

	//av_packet_free(&packet[pageId]);
	//sws_freeContext(pSwsCtx[pageId]);
	//av_frame_free(&(pframe[pageId]));
	//av_frame_free(&(pframeRGB[pageId]));
	//avformat_close_input(&(format_ctx_[pageId]));

#else
        ERROR_PRINT("device not support");
        return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
#endif

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
funcStatus VideoPlayerClass::SetValueCtrl(
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
funcStatus VideoPlayerClass::MouseTouchCtrl(
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
funcStatus VideoPlayerClass::KeyboardTouchCtrl(
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

#endif  //defined (CHIP_GC9005) || defined (PC_SIM)
