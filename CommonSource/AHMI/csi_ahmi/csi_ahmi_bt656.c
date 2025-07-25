#include "csi_ahmi_bt656.h"
#if defined(CHIP_GC9002)
#include "texture_flash.h"
#elif defined(CHIP_GC9005)
#include "gc90x_ahmi.h"
#include "gc9005_flash.h"
#else
#endif

#ifdef VIDEO_EN//enable video module
void csi_ahmi_bt656_init(uint8_t videotype)
{
	BT656_Configurate(videotype);
}

void csi_ahmi_bt656_enable(void)
{
	BT656_Start();
}

void csi_ahmi_bt656_disable(void)
{
	BT656_Disable();
}

void csi_ahmi_bt656_getParam(uint16_t* Height, uint16_t* Width)
{
	BT656_GetParam(Height, Width);
}

extern  u8 video_cur_r0 ; 
extern  u8 video_cur_w0 ;
extern  u8 video_next_r0;
extern uint32_t SizeOfCVBSBuffer;           //the size of cvbs buffer, used to change cvbs
extern uint32_t StartAddrOfCVBS0Even;       //the start address of cvbs
extern uint32_t StartAddrOfCVBS0Odd;        //the start address of cvbs
void csi_ahmi_bt656_switchbuffer(uint8_t* flag_frame, uint8_t videotype)
{
    if(videotype == Video_CVBS)
    {
        #if (defined VIDEO_ODD_EVEN_ALL)
            *flag_frame = SAMPLE_BOTH_FIELD;
        #endif

        #if (defined VIDEO_ODD_And_EVEN)
        if(*flag_frame == SAMPLE_EVEN_FIELD)
            *flag_frame = SAMPLE_ODD_FIELD;
        else
            *flag_frame = SAMPLE_EVEN_FIELD;
        #endif

        #ifdef VIDEO_EVEN
            *flag_frame = SAMPLE_EVEN_FIELD;
        #endif

        #ifdef VIDEO_ODD
            *flag_frame = SAMPLE_ODD_FIELD;
        #endif

        #ifdef VERSION_2
        BT656_SetSampleMode(*flag_frame);
        #endif
    }

	if((*flag_frame == 1) || //next frame is even, then we need to change the address
	   (videotype != Video_CVBS))
	{
		#if defined(CHIP_GC9002)
	    if(video_cur_w0 > 1)
	    {
	        video_cur_w0 = 1;
	    }
		if(video_cur_w0 == 1)
			video_cur_w0 = 0;
		else
			video_cur_w0 = 1;
		#elif defined(CHIP_GC9005)
        #if (VIDEO_BUFFER_NUM == 3)
		if(video_cur_w0 == (video_cur_r0 + 1) % 3)
			video_cur_w0 = (video_cur_w0 + 1) % 3;
        #elif (VIDEO_BUFFER_NUM == 2)
	    if(video_cur_w0 > 1)
	    {
	        video_cur_w0 = 1;
	    }
		if(video_cur_w0 == 1)
            video_cur_w0 = 0;
        else
            video_cur_w0 = 1;
        #endif
		#endif
		
		#ifdef VIDEO_ODD_EVEN_ALL //when even odd all, even and odd frame needs to be in the same address as even frame
		BT656_SetBaseAddress((StartAddrOfCVBS0Even) + video_cur_w0 * (SizeOfCVBSBuffer));
		#else
		BT656_SetBaseAddress((StartAddrOfCVBS0Even) + video_cur_w0 * (SizeOfCVBSBuffer));
		#endif
	}
	else
	{
		#if defined(CHIP_GC9002)
	    if(video_cur_w0 > 1)
	    {
	        video_cur_w0 = 1;
	    }
		if(video_cur_w0 == 1)
			video_cur_w0 = 0;
		else
			video_cur_w0 = 1;
		#elif defined(CHIP_GC9005)
        #if (VIDEO_BUFFER_NUM == 3)
		#if (defined VIDEO_ODD_EVEN_ALL)  || (defined VIDEO_ODD)
		if(video_cur_w0 == (video_cur_r0 + 1) % 3)
			video_cur_w0 = (video_cur_w0 + 1) % 3;
		#endif
        #elif (VIDEO_BUFFER_NUM == 2)
	    if(video_cur_w0 > 1)
	    {
	        video_cur_w0 = 1;
	    }
        if(video_cur_w0 == 1)
            video_cur_w0 = 0;
        else
            video_cur_w0 = 1;
        #endif
		#endif

		#ifdef VIDEO_ODD_EVEN_ALL //when even odd all, even and odd frame needs to be in the same address as even frame
		BT656_SetBaseAddress((StartAddrOfCVBS0Even) + video_cur_w0 * (SizeOfCVBSBuffer));
		#else
		BT656_SetBaseAddress((StartAddrOfCVBS0Odd) + video_cur_w0 * (SizeOfCVBSBuffer));
		#endif
	}
}

extern uint8_t drawVideoFlag;
extern void ExtiTrigCvbsHandler( void );
void Video_Cap_HandlerISRCB(void)
//#if defined(CHIP_GC9002)
//#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
//#else
//__attribute__ ((interrupt ("machine"))) 
//#endif
//#endif
//void Video_Cap_HandlerISR(void)
{
	if(BT656_GetITStatus(BT656_IT_WFFIFO))
	{
		BT656_ResetTIStatus(BT656_IT_WFFIFO);
	}
	else if(BT656_GetITStatus(BT656_IT_FOSE))
	{
		BT656_ResetTIStatus(BT656_IT_FOSE);
		
		#if defined(CHIP_GC9003)
		DRAM_CACHE->DramCacheFlush = 1;
		#endif
	}
	else if(BT656_GetITStatus(BT656_IT_C0DD))
	{
		BT656_ResetTIStatus(BT656_IT_C0DD);
		
		#ifdef VIDEO_Vertical_Synchronization_ENABLE
		extern u8 normalvideoframe;
		if(drawVideoFlag == BT656_WaitCaptureDone)
		{
			drawVideoFlag = BT656_CaptureDone;
		}
		else if((drawVideoFlag == BT656_AHMIDone) || (normalvideoframe < VIDEO_FIRST_FRAME_NUMBER))
		{
			ExtiTrigCvbsHandler();
			drawVideoFlag = BT656_WaitCaptureDone;
		}
		#else
		ExtiTrigCvbsHandler();
		#endif
	}
}
#endif
