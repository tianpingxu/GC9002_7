#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "QRcodeClass.h"
#include "drawImmediately_cd.h"
#include "writeTexture.h"

#ifdef AHMI_DEBUG
#include "trace.h"
#include "stdio.h"
#endif

#ifdef AHMI_CORE

extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16					WorkingPageID;
extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

QRcodeClass::QRcodeClass()
{

}
QRcodeClass::~QRcodeClass()
{

}

#ifdef VERSION_2
funcStatus QRcodeClass::initWidget(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,   
	u32 *u32p_sourceShift,  
	NewMatrixPtr pWidgetMatrix,
	u8 u8_pageRefresh,       
	u8 RefreshType ,         
	TileBoxClassPtr pTileBox,
	u8 staticTextureEn       
	)
{
	#ifndef CHIP_GC9002_NoHyper
	TagClassPtr bindTag;
	ActionTriggerClass tagtrigger;
	WidgetClassInterface myWidgetClassInterface;
	TextureClassPtr TexturePtr;
	char *QRSource;
	u8 i;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 Op_sourceShift = 0;
	funcStatus status;
	#endif

	if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
	{
		//find the bind tag
		bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		//set the value of widget according to tag
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		QRSource = (char*)bindTag->getStringValue();

		//TODO
		//QRSource may not have \0 as end

		writeQRCode_alpha1(p_wptr->IDnumber, p_wptr->WidgetWidth, QRSource);
		
		p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	//pixel_ratio
	TexturePtr = &(pageptr->pBasicTextureList[p_wptr->StartNumofTex]);
	for (i = 0; i < 4; i++)
	{
		TexturePtr[i].mTexAttr |= TEXTURE_CENTRAL_ROTATE;
		TexturePtr[i].mTexAttr |= USING_PIXEL_RATIO;
		//TexturePtr[i].renewTextureSourceBox(NULL,NULL,NULL);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawQRcodeTexture(p_wptr,u32p_sourceShift, pWidgetMatrix, RefreshType,pTileBox,staticTextureEn,pageptr) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawQRcodeTexture(p_wptr,u32p_sourceShift, pWidgetMatrix, RefreshType,pTileBox,staticTextureEn,pageptr);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			*u32p_sourceShift = Op_sourceShift;
			if(myWidgetClassInterface.drawTexture_withCopySourceBuffer(p_wptr, u32p_sourceShift) != AHMI_FUNC_SUCCESS)
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
		}
		else if(status != AHMI_FUNC_SUCCESS)
		{
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		}

		p_wptr->REFRESH_FLAG &= (~WidgetSourceBufferRefreshFlag);
	}
	else
	{
		if(myWidgetClassInterface.drawTexture_withCopySourceBuffer(p_wptr, u32p_sourceShift) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}
	#endif
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
	return AHMI_FUNC_SUCCESS;
}

funcStatus QRcodeClass::widgetCtrl(
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	return AHMI_FUNC_SUCCESS;
}

funcStatus QRcodeClass::SetValueCtrl(
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	#ifndef CHIP_GC9002_NoHyper
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//**************************************
funcStatus QRcodeClass::MouseTouchCtrl(
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

#endif

#endif
