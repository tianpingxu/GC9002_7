////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2022/02/22
// File Name:     CircleSliderClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 1.00 - File Created 2022/02/22 by YuShanShan
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////
#include "CircleSliderClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
#include "myMathClass.h"

#define MAX_ANGLE_CIRCLESLIDER 360
#define MOVE_THRESHOLD (10 << 4) //0.12.4

#ifdef AHMI_CORE
extern ActionTriggerClass		gWidgetTagTrigger;
extern ActionTriggerClass		gMouseTagTrigger;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16  WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern Easing gEasing;

extern u8 force_cancel_subcanvas_page_mousetouch;
#define PI 3.14159

//#if defined(EMBEDDED) && defined(VERSION_2)
//extern void EnterSafeSectionForTag(void);
//extern void ExitSafeSectionForTag(void);
//#endif

//-----------------------------
// name： CircleSliderClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
CircleSliderClass::CircleSliderClass()
{

}

//-----------------------------
// name： ~CircleSliderClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
CircleSliderClass::~CircleSliderClass()
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
funcStatus CircleSliderClass::initWidget(
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
	TagClassPtr bindTag;
	ActionTriggerClass tagtrigger;
	WidgetClassInterface myWidgetClassInterface;
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
		if(widgetCtrl(pagePtr,p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn,pagePtr) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr);
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
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// reponse for Tag.setValue
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus CircleSliderClass::widgetCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{	
	s32 value;
	s32	maxValue;
	s32	minValue;	
	s16 Angle;
	s32 ValueStart;
	s32 ValueStop;
	int travelPrincipleMode = p_wptr->TravelPrincipleMode;

	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	//judge parameters
	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT || 
		p_wptr->WidgetWidth == 0 ||  
		p_wptr->WidgetHeight == 0)
	{
		ERROR_PRINT("ERROR: when drawing circleSlider widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	if (    p_wptr->CircleSliderMinAngle > MAX_ANGLE_CIRCLESLIDER ||
		 (s16)p_wptr->CircleSliderMinAngle < -MAX_ANGLE_CIRCLESLIDER ||
		      p_wptr->CircleSliderMaxAngle > MAX_ANGLE_CIRCLESLIDER ||
		 (s16)p_wptr->CircleSliderMaxAngle < -MAX_ANGLE_CIRCLESLIDER ||
		(p_wptr->CircleSliderMaxAngle < p_wptr->CircleSliderMinAngle))
	{		
		ERROR_PRINT("ERROR: when drawing CircleSlider widght, the Angle exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	value = (s32)(ActionPtr->mTagPtr->getValue());

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	if(maxValue == minValue){
		ERROR_PRINT("ERROR: can't be divided by zero");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}

	if(value > maxValue)
		value = maxValue;
	else if(value < minValue)
		value = minValue;

	Angle = 16* (value - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4

	//render the texture 
	if( p_wptr->ANIMATION_DURATION == 0 || p_wptr->HoldFirstFlag == 1) //no animation
	{
		if(travelPrincipleMode){
			ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));
			Angle = 16* (ValueStop - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4
		}
		renderTextureAngle(
			 pageptr,
			 p_wptr,  
			 Angle
		);
	}
	else if(u8_pageRefresh)	   //animation of page refresh
	{
		//when page refresh,use the value of animation as next value, not according to current value by Mr.z
		//p_wptr->ANIMATIONCURVALUE_L = (u16)( (u32)value );
		//p_wptr->ANIMATIONCURVALUE_H = (u16)(( (u32)value ) >> 16);
		p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		p_wptr->STOP_TAG_L = (u16)( (u32)value );
		p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
		renderTextureAngle(
			 pageptr,
			 p_wptr,  
			 Angle
		);   
	}
	else // first frame
	{
		/*if (p_wptr->NOW_DURATION > 0){
			p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
			p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		}else{
			p_wptr->START_TAG_L = p_wptr->STOP_TAG_L;
			p_wptr->START_TAG_H = p_wptr->STOP_TAG_H;
		}*/

		if (travelPrincipleMode)
		{
			p_wptr->START_TAG_L = 0;
			p_wptr->START_TAG_H = 0;
			////当p_wptr->STOP_TAG_L被赋了差值后，不能再将其改为value,否则将导致动画错误
			//if (!p_wptr->STOP_TAG_L)
			//{
			//	p_wptr->STOP_TAG_L = (u16)( (u32)value );
			//}
			//if (!p_wptr->STOP_TAG_H)
			//{
			//	p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
			//}
		}else
		{
			p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
			p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;

			p_wptr->STOP_TAG_L = (u16)( (u32)value );
			p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
		}	 
		

		/*char text[20];
		sprintf_s(text,"start %d ,stop %d",p_wptr->START_TAG_L,p_wptr->STOP_TAG_L);
		ERROR_PRINT(text);
*/
	}

	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));

	if( (p_wptr->ANIMATION_DURATION != 0 && p_wptr->HoldFirstFlag != 1) && (ValueStart != ValueStop) ) //create animation
	{
		p_wptr->loadTextureWithAnimation(pageptr);
	}
	

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name： MeterClass::renderTextureAngle
// set the texture's angle
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus CircleSliderClass::renderTextureAngle
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,   
		 s16 Angle
		 )
{
	//attr value
	u16 rotatingDir;
	u8 circleSliderMode;
	//rotatingDir
	rotatingDir  = (p_wptr->WidgetAttr & 0x20) ?1:0  ; //rotatingDir, 1 clockwise，0 anti-clockwise
	//mode
	if (p_wptr->NumOfTex==9)
	{
		circleSliderMode = 1;
	}else{
		circleSliderMode = 0;
	}

	u8 SliderPtr;
	u8 SliderAlphaPtr;
//	s16 initAngleForSliderAlpha = 45 * 16;
	s16 initAngleForSlider =  45 * 16;
	s16 startAngleForSlider;
	s16 startAngleForSliderAlpha;
	s16 ShearAngleX;				//12.4
	s16 ShearAngleY;
	s16 pureMaskAdjustAngle;     //puremask adjust angle
	s32	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	s32	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	

	int travelPrincipleMode = p_wptr->TravelPrincipleMode;

	s16 RotateAngle;			//12.4
	u16 ScaleX,ScaleY;
	u8 flag; //if flag == 0, the address need to be 0 in order not to display the meter light

//	myMathClass mymath;
	TextureClassPtr TexturePtr;
	TileBoxClass circleSliderBox;
	u8 index = 0;

	Angle += ((s16)(p_wptr->CircleSliderMinAngle)) << 4;//12.4 relative to min angle

	//start address of texture
	TexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
	
	//0 simple mode 1 complex mode
	if(circleSliderMode == 1){//complex mode
		SliderPtr = 8;
		SliderAlphaPtr = 6;
		//initAngleForSliderAlpha = mymath.GetAngleFromTan((long long)(((long long)TexturePtr[SliderAlphaPtr].TexWidth << 20) / TexturePtr[SliderAlphaPtr].TexHeight));
	}else{
		SliderPtr = 1;
	}
	
	

	if (travelPrincipleMode)
	{
		s16 lastAngle;
		s16 curAngle;
		s16 newAngle;
		s32 ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16)); //此时，变换后的ValueStop其实是value差值
		s16 walkingAngle = 16* (ValueStop - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4
		walkingAngle += ((s16)(p_wptr->CircleSliderMinAngle)) << 4;//12.4 relative to min angle


		s32 ValueNow = (s32)((p_wptr->OldValueL) +  ( (p_wptr->OldValueH) << 16)); //OldValueL其实是现在的value

		curAngle = 16* (ValueNow - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4
		curAngle += ((s16)(p_wptr->CircleSliderMinAngle)) << 4;//12.4 relative to min angle

		if(rotatingDir)
		{
			if (curAngle>=walkingAngle)
			{
				lastAngle = curAngle - walkingAngle;
			}else
			{
				lastAngle = curAngle - walkingAngle + 360*16;
			}
			
			startAngleForSlider = (s16)(p_wptr->CircleSliderStartAngle) + lastAngle + initAngleForSlider; //start angle		// StartAngle = offsetValue 起始角度
			startAngleForSliderAlpha = (s16)(p_wptr->CircleSliderStartAngle);   //
			pureMaskAdjustAngle = (s16)(p_wptr->CircleSliderStartAngle) + lastAngle + (((s16)(p_wptr->CircleSliderMinAngle)) << 4); //12.4
		}
		else 
		{
			if (curAngle+walkingAngle<=360*16)
			{
				lastAngle = curAngle + walkingAngle;
			}else
			{
				lastAngle = curAngle + walkingAngle - 360*16;
			}
			startAngleForSlider = ((s16)(p_wptr->CircleSliderStartAngle)) + lastAngle + initAngleForSlider;
			startAngleForSliderAlpha = (s16)(p_wptr->CircleSliderStartAngle);
			pureMaskAdjustAngle = -(s16)(p_wptr->CircleSliderStartAngle) - lastAngle + (((s16)(p_wptr->CircleSliderMinAngle)) << 4); //12.4
		}
		
	}else
	{
		if(rotatingDir)
		{
			startAngleForSlider = (s16)(p_wptr->CircleSliderStartAngle) + initAngleForSlider; //start angle		// StartAngle = offsetValue 起始角度
			startAngleForSliderAlpha = (s16)(p_wptr->CircleSliderStartAngle);   //
		}
		else 
		{
			startAngleForSlider = -(s16)(p_wptr->CircleSliderStartAngle) + initAngleForSlider;
			startAngleForSliderAlpha = -(s16)(p_wptr->CircleSliderStartAngle);
		}
		pureMaskAdjustAngle = (s16)(p_wptr->CircleSliderStartAngle) + (((s16)(p_wptr->CircleSliderMinAngle)) << 4); //12.4
	}

	
	if(rotatingDir)//clockwise
		{
			if(circleSliderMode == 1)//complex mode
			{
				TexturePtr[SliderAlphaPtr].RotateAngle = Angle + startAngleForSliderAlpha;//add the start angle,//12.4
				TexturePtr[SliderAlphaPtr].mTexAttr |= ABCDEFMATRIX;
				TexturePtr[SliderAlphaPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802
			}

			TexturePtr[SliderPtr].RotateAngle = Angle + startAngleForSlider;//add the start angle,//12.4
			TexturePtr[SliderPtr].mTexAttr |= ABCDEFMATRIX;
			TexturePtr[SliderPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802
			Angle = TexturePtr[SliderPtr].RotateAngle - ((s16)(p_wptr->CircleSliderMinAngle) << 4) - startAngleForSlider;// + (45*16);//////point angle relative to start angle
		}
	else //anti-clockwise
		{
			if(circleSliderMode == 1)//complex mode
			{
				TexturePtr[SliderAlphaPtr].RotateAngle = -Angle + startAngleForSliderAlpha;//add the start angle,//12.4
				TexturePtr[SliderAlphaPtr].mTexAttr |= ABCDEFMATRIX;
				TexturePtr[SliderAlphaPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802
			}

			TexturePtr[SliderPtr].RotateAngle = -Angle + startAngleForSlider;//add the start angle,//12.4
			TexturePtr[SliderPtr].mTexAttr |= ABCDEFMATRIX;
			TexturePtr[SliderPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802
			Angle = TexturePtr[SliderPtr].RotateAngle + ((s16)(p_wptr->CircleSliderMinAngle) << 4) - startAngleForSlider;// - (45*16);//////point angle relative to start angle

		}
	
	//recompute box
	//TexturePtr[SliderPtr].renewTextureSourceBox(NULL,NULL,NULL);
	//TexturePtr[PointerPtr].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
	   
	u8 noBackMode = 0;  //none background  edit by lx
	if(p_wptr->NumOfTex == 8 || p_wptr->NumOfTex == 1){
		// none background
		noBackMode = 1;
	}
	if(circleSliderMode == 1)//complex mode
	{
		if (noBackMode==1){
			//TexturePtr[0].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
		}else {
			//TexturePtr[1].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
		}
	
		if(rotatingDir)//clock wise
		{
			//     |
			//   2 | 3
			//  ---------
			//   1 | 4
			//     |
			for(SliderPtr = 1  ;SliderPtr<5;SliderPtr++)
			{
				if(Angle>= 16 * 90 * (SliderPtr)) //when pointer exceeds the quadrant,this quadrant shows triangle
				{
					ShearAngleX = 0; //donnot shear
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * (SliderPtr );//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				else if(Angle<= 16 * 90 * ((SliderPtr ) - 1 ))//when pointer donnot go to the quadrant, this quadrant donnot show
				{
					ShearAngleX = 0 ;//no shear
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * (SliderPtr);//rotate to current quadrant
					ScaleX = 0xf;//zoom small so that donnot show
					ScaleY = 0xf;
					flag = 0;
				}
				else //in the quadrant
				{
					ShearAngleX = 90*16*(SliderPtr ) - Angle;
					if(ShearAngleX >= 89*16)
						ShearAngleX = 89*16;
					if(ShearAngleX <= -89*16)
						ShearAngleX = -89*16;
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * ((SliderPtr ));//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
			
				if(noBackMode==1){
					index = (SliderPtr - 1)%4 + 1;
				}else{
					index = (SliderPtr - 1)%4 + 2;
				}
				TexturePtr[index ].RotateAngle = RotateAngle + pureMaskAdjustAngle;				
				TexturePtr[index].ShearAngleX = ShearAngleX;
				TexturePtr[index].ShearAngleY = ShearAngleY;
				TexturePtr[index].ScalerX = ScaleX;
				TexturePtr[index].ScalerY = ScaleY;
				if(flag == 0) TexturePtr[index].TexAddr = 0;
				else TexturePtr[index].TexAddr = 0xff000000;
				//TexturePtr[index].renewTextureSourceBox(NULL,NULL,NULL);
			}
		}
		else //anti-clockwise
		{
			//     |
			//   3 | 2
			//  ---------
			//   4 | 1
			//     |
			for(SliderPtr = (1 );SliderPtr<(5 );SliderPtr++)
			{
				if( (s16)Angle <= (s16)(- 16 * 90 * (SliderPtr )) ) //when pointer exceeds the quadrant,this quadrant shows triangle
				{
					ShearAngleX = 0;//donnot shear
					ShearAngleY = 0;//donnot shear
					RotateAngle = - 16 * 90 * (SliderPtr - 1);//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				else if( (s16)Angle>= (s16)(- 16 * 90 * (SliderPtr - 1 ) ))//when pointer donnot go to the quadrant, this quadrant donnot show
				{
					ShearAngleX = 0;//donnot shear
					ShearAngleY = 0;
					RotateAngle = -16 * 90 * (SliderPtr - 1 );//rotate to current quadrant
					ScaleX = 0xf;//zoom small so that donnot show
					ScaleY = 0xf;
					flag = 0;
				}
				else //in the quadrant
				{
					ShearAngleX = 0;
					ShearAngleY = Angle + SliderPtr * 90 * 16;
					if(ShearAngleY >= 89*16)
						ShearAngleY = 89*16;
					if(ShearAngleY <= -89*16)
						ShearAngleY = -89*16;
					RotateAngle = -16 * 90 * (SliderPtr - 1) + ShearAngleX;//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}

				u8 index = 0;
				if(noBackMode==1){
					index = (SliderPtr - 1)%4 + 1;
				}else{
					index = (SliderPtr - 1)%4 + 2;
				}

				TexturePtr[index].RotateAngle = RotateAngle - pureMaskAdjustAngle;
				TexturePtr[index].ShearAngleX = ShearAngleX;
				TexturePtr[index].ShearAngleY = ShearAngleY;
				TexturePtr[index].ScalerX = ScaleX;
				TexturePtr[index].ScalerY = ScaleY;
				if(flag == 0) TexturePtr[index].TexAddr = 0;
				else TexturePtr[index].TexAddr = 0xff000000;
				//TexturePtr[index].renewTextureSourceBox(NULL,NULL,NULL);
			}
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
funcStatus CircleSliderClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			  
	ActionTriggerClassPtr ActionPtr,  
	u8 u8_widgetRefresh				  
	)
{
	DynamicPageClassPtr pCurrentPage;
	s32 value;
	
	u32 actionAddr;
	u16 oldValueinit = 0;
	s32 oldValue = 0;	
	s32 lowAlarmValue ;
	s32 highAlarmValue;
	s32	maxValue;
	s32	minValue;
	
	int travelPrincipleMode = p_wptr->TravelPrincipleMode;
	
	oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);

	if( NULL == p_wptr || NULL == ActionPtr || NULL == ActionPtr->mTagPtr){
		ERROR_PRINT("ERROR: widgetPtr or ActionPtr is NULL");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	if( NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: when drawing CircleSlider widget, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	//set the attr value
	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL );
	highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL);
	value = (s32)(ActionPtr->mTagPtr->getValue());

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	if((value < minValue) || (value > maxValue)){
		ERROR_PRINT("ERROR: CircleSlider widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
	}

	//judge parameters
	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT || 
		p_wptr->WidgetWidth == 0 ||  
		p_wptr->WidgetHeight == 0)
	{
		ERROR_PRINT("ERROR: when drawing CircleSlider widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	if (     p_wptr->CircleSliderMinAngle > MAX_ANGLE_CIRCLESLIDER ||
		 (s16)p_wptr->CircleSliderMinAngle < -MAX_ANGLE_CIRCLESLIDER ||
		      p_wptr->CircleSliderMaxAngle > MAX_ANGLE_CIRCLESLIDER ||
		 (s16)p_wptr->CircleSliderMaxAngle < -MAX_ANGLE_CIRCLESLIDER ||
		(p_wptr->CircleSliderMaxAngle < p_wptr->CircleSliderMinAngle))
	{		
		ERROR_PRINT("ERROR: when drawing CircleSlider widget, the Angle exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
    
    if(u8_widgetRefresh == 0)
    {
        if((p_wptr->ANIMATION_DURATION) != 0) //contain animation and not holding
        {
			//calculate real start value
			p_wptr->calculateCurrentAnimation(pagePtr);			

			if (travelPrincipleMode)
			{
				u16 rotatingDir  = (p_wptr->WidgetAttr & 0x20) ?1:0  ; //rotatingDir, 1 clockwise，0 anti-clockwise
				
				s32 ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));
				s32 oldValue = (s32)((p_wptr->OldValueL) +  ( (p_wptr->OldValueH) << 16)); 
				s32 realValueStop = (s32)((p_wptr->ANIMATIONCURVALUE_L) + ( (p_wptr->ANIMATIONCURVALUE_H) << 16));

				s32 distanceBetStop = ValueStop - realValueStop;
				if (distanceBetStop==0 || p_wptr->HoldFirstFlag == 1)
				{
					p_wptr->START_TAG_L = p_wptr->OldValueL;
					p_wptr->START_TAG_H = p_wptr->OldValueH;
				}else
				{
					if (rotatingDir) //顺
					{
						realValueStop = oldValue - distanceBetStop;
						if (realValueStop < 0)
						{
							realValueStop += maxValue;
						}
					}else  //逆
					{
						realValueStop = oldValue + distanceBetStop;
						if (realValueStop>=maxValue)
						{
							realValueStop -= maxValue;
						}
					}
				
					p_wptr->START_TAG_L = (u16)realValueStop;
					p_wptr->START_TAG_H = (u16)(realValueStop>>16);
				}
				


				//p_wptr->ANIMATIONCURVALUE_L = p_wptr->OldValueL;
				//p_wptr->ANIMATIONCURVALUE_H = p_wptr->OldValueH;
				//p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
				//p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
				//p_wptr->START_TAG_L = p_wptr->OldValueL;
				//p_wptr->START_TAG_H = p_wptr->OldValueH;

				s32 ValueStart;
				s32 newValue;
				s16 startAngle;
				s16 stopAngle;
				

				ValueStart = (s32)((p_wptr->START_TAG_L) + ( (p_wptr->START_TAG_H) << 16));
				startAngle = 16* (ValueStart - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4
				startAngle += ((s16)(p_wptr->CircleSliderMinAngle)) << 4;

				stopAngle = 16* (value - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4
				stopAngle += ((s16)(p_wptr->CircleSliderMinAngle)) << 4;

				if (stopAngle>=startAngle)
				{
					if (stopAngle - startAngle <= 180*16)	//顺
					{
						rotatingDir = 1;						
						stopAngle = stopAngle - startAngle;
					}
					else	//逆
					{
						rotatingDir = 0;
						stopAngle = 360*16 - (stopAngle - startAngle);
					}
				}
				else
				{
					if (startAngle - stopAngle <= 180*16)	//逆
					{
						rotatingDir = 0;
						stopAngle = startAngle - stopAngle;
					}
					else	//顺
					{
						rotatingDir = 1;
						stopAngle = stopAngle - startAngle + 360*16;
					}
				}
				startAngle = 0;
				p_wptr->START_TAG_L = 0;
				p_wptr->START_TAG_H = 0;
				//p_wptr->ANIMATIONCURVALUE_L = 0;
				//p_wptr->ANIMATIONCURVALUE_H = 0;
				newValue = ((stopAngle - (s16)(p_wptr->CircleSliderMinAngle)) * (maxValue - minValue) / ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) )) / 16 + minValue;
				p_wptr->STOP_TAG_L = (u16)( (u32)newValue );
				p_wptr->STOP_TAG_H = (u16)(( (u32)newValue ) >> 16);
				if(rotatingDir){
					p_wptr->WidgetAttr |= 0x20; 
				}
				else
				{
					p_wptr->WidgetAttr &= ~0x20; 
				}
			}else
			{
				p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
				p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
				p_wptr->STOP_TAG_L = (u16)( (u32)value );
				p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);

				p_wptr->loadTextureWithAnimation(pagePtr);
			}			
		}
		else
		{
			if (travelPrincipleMode)
			{
				p_wptr->START_TAG_L = p_wptr->OldValueL;
				p_wptr->START_TAG_H = p_wptr->OldValueH;	

				s32 ValueStart;
				s32 newValue;
				s16 startAngle;
				s16 stopAngle;
				u16 rotatingDir  = (p_wptr->WidgetAttr & 0x20) ?1:0  ; //rotatingDir, 1 clockwise，0 anti-clockwise

				ValueStart = (s32)((p_wptr->START_TAG_L) + ( (p_wptr->START_TAG_H) << 16));
				startAngle = 16* (ValueStart - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4
				startAngle += ((s16)(p_wptr->CircleSliderMinAngle)) << 4;

				stopAngle = 16* (value - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4
				stopAngle += ((s16)(p_wptr->CircleSliderMinAngle)) << 4;

				if (stopAngle>=startAngle)
				{
					if (stopAngle - startAngle <= 180*16)	//顺
					{
						rotatingDir = 1;						
						stopAngle = stopAngle - startAngle;
					}
					else	//逆
					{
						rotatingDir = 0;
						stopAngle = 360*16 - (stopAngle - startAngle);
					}
				}
				else
				{
					if (startAngle - stopAngle <= 180*16)	//逆
					{
						rotatingDir = 0;
						stopAngle = startAngle - stopAngle;
					}
					else	//顺
					{
						rotatingDir = 1;
						stopAngle = stopAngle - startAngle + 360*16;
					}
				}
				startAngle = 0;
				p_wptr->START_TAG_L = 0;
				p_wptr->START_TAG_H = 0;
				newValue = ((stopAngle - (s16)(p_wptr->CircleSliderMinAngle)) * (maxValue - minValue) / ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) )) / 16 + minValue;
				p_wptr->STOP_TAG_L = (u16)( (u32)newValue );
				p_wptr->STOP_TAG_H = (u16)(( (u32)newValue ) >> 16);
				if(rotatingDir){
					p_wptr->WidgetAttr |= 0x20; 
				}
				else
				{
					p_wptr->WidgetAttr &= ~0x20; 
				}
			}
		}
    }

	pCurrentPage = pagePtr;
	if(u8_widgetRefresh == 0)
	{

		if(oldValueinit)
		{			
			if(oldValue != value){
				if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
			if(p_wptr->EnterLowAlarmAction && oldValue > lowAlarmValue && value <= lowAlarmValue)
			{
				//enter low
				if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}		
			}
			else if(p_wptr->LeaveLowAlarmAction && oldValue <= lowAlarmValue && value >lowAlarmValue)
			{
				//leave low
				if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}		
			}
			if(p_wptr->EnterHighAlarmAction && oldValue < highAlarmValue && value >= highAlarmValue)
			{
				//enter high
				if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);		
				}
			}
			else if(p_wptr->LeaveHighAlarmAction && oldValue >= highAlarmValue && value < highAlarmValue)
			{
				//leave high
				if(p_wptr->LeaveHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveHighAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}	
			}
		}
        else
        {
   			//if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
			//{
			//	actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
			//	xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
			//}
            
            //if(p_wptr->EnterLowAlarmAction && value < lowAlarmValue)
            //{
            //    //enter low
            //    if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
            //    {
            //        actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
            //        xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
            //    }		
            //}
            //
            //if(p_wptr->EnterHighAlarmAction && value > highAlarmValue)
            //{
            //    //enter high
            //    if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
            //    {
            //        actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
            //        xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
            //    }
            //}
        }

		//old value refresh
		p_wptr->WidgetAttr |= 0x8000;
		p_wptr->OldValueL = (u16)value;
		p_wptr->OldValueH = (u16)(value >> 16); 
	}
    
    if(oldValueinit)
	{
		if(oldValue != value)
		{
			p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
			p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
		}
	}
	else
	{
		p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	}
    
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//Do not support touch propagation.
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus CircleSliderClass::MouseTouchCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
    PIDPoint* pPressPoint,
    PIDPoint* pReleasePoint,
	PIDPoint* pAbsolutePressPoint,
    PIDPoint* pAbsoluteReleasePoint,
	u8 u8_widgetRefresh				
	)
{	
	u8 SliderPtr;
//	u8 SliderAlphaPtr;
	
	s16 Angle;
	s16 lastAngle;
	s16 newValue;

	s32 tag_value= 0;
	s32 cur_x;
	s32 cur_y;
	s32	maxValue;
	s32	minValue;
	s16	maxAngle = p_wptr->CircleSliderMaxAngle;
	s16	minAngle = p_wptr->CircleSliderMinAngle;
	s32 width = p_wptr->WidgetWidth;
	s32 height = p_wptr->WidgetHeight;
	s32 RotatePointX = p_wptr->RotationPointX << 4;
	s32 RotatePointY = p_wptr->RotationPointY << 4;
	u16 sliderWidth;
	u16 sliderHeight;
	int travelPrincipleMode = p_wptr->TravelPrincipleMode;
	
	u8 actionType;
	TextureClassPtr texturePtr;
	/*CanvasClassPtr curCanvas = &pageptr->pCanvasList[p_wptr->ATTATCH_CANVAS];
	SubCanvasClassPtr curSubCan = &(pageptr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);*/
	u32 actionAddr;

	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	actionType = ActionPtr->mInputType;

	//start address of texture
	texturePtr = &(pageptr->pBasicTextureList[p_wptr->StartNumofTex]);

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	if(maxValue == minValue){
		ERROR_PRINT("ERROR: can't be divided by zero");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetActionTriggerErr);
	}

	u8 noBackMode = 0;  //none background  edit by lx
	if(p_wptr->NumOfTex == 8 || p_wptr->NumOfTex == 1){
		// none background
		noBackMode = 1;
	}
	//mode
	if (p_wptr->NumOfTex==9)
	{
		SliderPtr = 8;
	}else{
		SliderPtr = 1;
	}

	sliderWidth = texturePtr[SliderPtr].TexWidth << 4;
	sliderHeight = texturePtr[SliderPtr].TexHeight << 4;
	if(noBackMode){
	
	}else{
		width = texturePtr[0].TexWidth << 4;
		height = texturePtr[0].TexHeight << 4;
	}
	
#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

	if(actionType == ACTION_MOUSE_HOLDING)
	{
		//if(((pAbsoluteReleasePoint->x - (s16)p_wptr->WidgetOffsetX - curCanvas->moffsetX - curSubCan->contentOffsetX) <= WIDGET_TOUCH_DIFF) || ((pAbsoluteReleasePoint->x - (s16)p_wptr->WidgetOffsetX - curCanvas->moffsetX - curSubCan->contentOffsetX) >= (p_wptr->WidgetWidth - WIDGET_TOUCH_DIFF)) ||
		//   ((pAbsoluteReleasePoint->y - (s16)p_wptr->WidgetOffsetY - curCanvas->moffsetY - curSubCan->contentOffsetY) <= WIDGET_TOUCH_DIFF) || ((pAbsoluteReleasePoint->y - (s16)p_wptr->WidgetOffsetY - curCanvas->moffsetY - curSubCan->contentOffsetY) >= (p_wptr->WidgetHeight - WIDGET_TOUCH_DIFF)))
		if(((pReleasePoint->x) <= WIDGET_TOUCH_DIFF) || ((pReleasePoint->x) >= (p_wptr->WidgetWidth - WIDGET_TOUCH_DIFF)) ||
			((pReleasePoint->y) <= WIDGET_TOUCH_DIFF) || ((pReleasePoint->y) >= (p_wptr->WidgetHeight - WIDGET_TOUCH_DIFF)))
		{
			actionType = ACTION_MOUSE_CANCLE;
		}
	}

	newValue  = (s32)(ActionPtr->mTagPtr->getValue());
	lastAngle = 16* (newValue - minValue) * ( maxAngle - minAngle ) /(maxValue - minValue) +  (s16)(p_wptr->CircleSliderStartAngle);//12.4;
	Angle = lastAngle;

	if( actionType == ACTION_MOUSE_PRESS )
	{
		p_wptr->PressFlag = 1;
//		cur_x  = ((s16)(pPressPoint->x) << 4);
//		cur_y  = ((s16)(pPressPoint->y) << 4);
//		s32 pressScale = (RotatePointX-cur_x)*(RotatePointX-cur_x) + (RotatePointY-cur_y)*(RotatePointY-cur_y);  // 鼠标点击的点到控件中心点的距离
//		s32 minScale = ((p_wptr->SliderDeviation)*16-sliderWidth)*((p_wptr->SliderDeviation)*16-sliderHeight);
//		s32 maxScale = ((p_wptr->SliderDeviation)*16+sliderWidth)*((p_wptr->SliderDeviation)*16+sliderHeight);
//
//		if (pressScale>= minScale && pressScale <= maxScale){
//			Angle = computeAngle(cur_x,cur_y,sliderWidth,sliderHeight,RotatePointX,RotatePointY,width,height,p_wptr,lastAngle);
//
//			if (!travelPrincipleMode)
//			{
//				if(lastAngle>=maxAngle*16 + (s16)(p_wptr->CircleSliderStartAngle)-10*16 && Angle <=minAngle*16 + (s16)(p_wptr->CircleSliderStartAngle)+10*16){   // 防止在滑到终点时，不小心点到刚开始的位置
//					Angle = lastAngle;
//				}
//				if(lastAngle<=minAngle*16 + (s16)(p_wptr->CircleSliderStartAngle) + 10*16 && Angle >=maxAngle*16 + (s16)(p_wptr->CircleSliderStartAngle) - 10*16){  // 防止在开始滑动滑块时，不小心点到快结束的位置
//					Angle = lastAngle;
//				}
//
//			}
//			
//
//			tag_value = (Angle -  (s16)(p_wptr->CircleSliderStartAngle))*(maxValue-minValue)/(maxAngle*16-minAngle*16) + minValue;
//
//			if(p_wptr->BindTagID != 0)
//			{
//				TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
//			}
//		
//			if(p_wptr->BindTagID != 0)
//			{
//				p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
//			}
//			p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//
//			if(p_wptr->TouchPropagation & TouchPropagationSet)
//			{
//				return WIDGET_TOUCH_CANCEL;
//			}
//			else
//			{
//				return AHMI_FUNC_SUCCESS;
//			}
//		} else{
//			
//			return WIDGET_TOUCH_CANCEL;
//		}
		
		//mymath.CORDIC_32(Angle, &cosDegree, &sinDegree);
		//long long median1,median2;
		//median1 = RotatePointX-sliderWidth + (p_wptr->SliderDeviation)*16;
		/*median1 = (RotatePointX * sinDegree)>>20;
		median1 = RotatePointX - median1;
		texturePtr[SliderPtr].OffsetX = median1;*/
		//texturePtr[SliderPtr].OffsetX = cur_x;

		//median2 = RotatePointY-sliderHeight + (p_wptr->SliderDeviation)*16;

		/*median2 = (RotatePointY * cosDegree)>>20;
		median2 = RotatePointY + median2;
		texturePtr[SliderPtr].OffsetY =median2;*/
		//texturePtr[SliderPtr].OffsetY = cur_y;
	}
	else if(actionType == ACTION_MOUSE_HOLDING)
	{
		if(p_wptr->PressFlag == 1)
		{
			p_wptr->HoldFirstFlag = 1;
			cur_x  = ((s16)(pReleasePoint->x)) << 4;
			cur_y  = ((s16)(pReleasePoint->y)) << 4;
			
			if (cur_x - (((s16)(pPressPoint->x)) << 4) < MOVE_THRESHOLD && cur_x - (((s16)(pPressPoint->x)) << 4) > -MOVE_THRESHOLD && cur_y - (((s16)(pPressPoint->y)) << 4) < MOVE_THRESHOLD && cur_y - (((s16)(pPressPoint->y)) << 4) > -MOVE_THRESHOLD)
			{
				
			}
			else
			{
				s32 pressScale = (RotatePointX-cur_x)*(RotatePointX-cur_x) + (RotatePointY-cur_y)*(RotatePointY-cur_y);  // 鼠标点击的点到控件中心点的距离
				s32 minScale = ((p_wptr->SliderDeviation)*16-sliderWidth)*((p_wptr->SliderDeviation)*16-sliderHeight);
				s32 maxScale = ((p_wptr->SliderDeviation)*16+sliderWidth)*((p_wptr->SliderDeviation)*16+sliderHeight);

				if (pressScale>= minScale && pressScale <= maxScale){
					Angle = computeAngle(cur_x,cur_y,sliderWidth,sliderHeight,RotatePointX,RotatePointY,width,height,p_wptr,lastAngle);
					if (travelPrincipleMode)
					{
						if((Angle >= lastAngle + 20*16 && Angle < lastAngle + 180*16) || (Angle <= lastAngle - 20*16 && Angle > lastAngle - 180*16)){    // 防止滑动太快
							Angle = lastAngle;
						}
					}else
					{
						if(Angle >= lastAngle + 20*16 || Angle <= lastAngle-20*16){    // 防止滑动太快,以及让其在最小角度时不能向反方向滑动
							Angle = lastAngle;
						}
						if(Angle == maxAngle*16 + (s16)(p_wptr->CircleSliderStartAngle)){     // 在最大角度时不能继续正方向滑动
							Angle = maxAngle*16 + (s16)(p_wptr->CircleSliderStartAngle);
						}
					}
					
					tag_value = (Angle -  (s16)(p_wptr->CircleSliderStartAngle))*(maxValue-minValue)/(maxAngle*16-minAngle*16) + minValue;
				
					if(p_wptr->BindTagID != 0)
					{
						TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
					}
				
					if(p_wptr->BindTagID != 0)
					{
						p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
					}
					p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

					force_cancel_subcanvas_page_mousetouch = 1;
				} else {
					force_cancel_subcanvas_page_mousetouch = 1;
					return WIDGET_TOUCH_CANCEL;
				}
			}
		}
	}
	else if(actionType == ACTION_MOUSE_RELEASE) //mouse up
	{
		if(p_wptr->PressFlag == 1)
		{
			p_wptr->PressFlag = 0;
			p_wptr->HoldFirstFlag = 0;

			//send release action
			if(p_wptr->OnRealeaseAction <= pageptr->mActionInstructionsSize)
			{
				actionAddr = (u32)(pageptr->pActionInstructions + p_wptr->OnRealeaseAction);
                csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
			
			cur_x  = ((s16)(pReleasePoint->x) << 4);
			cur_y  = ((s16)(pReleasePoint->y) << 4);
			
			if (cur_x - (((s16)(pPressPoint->x)) << 4) < MOVE_THRESHOLD && cur_x - (((s16)(pPressPoint->x)) << 4) > -MOVE_THRESHOLD && cur_y - (((s16)(pPressPoint->y)) << 4) < MOVE_THRESHOLD && cur_y - (((s16)(pPressPoint->y)) << 4) > -MOVE_THRESHOLD)
			{
				s32 pressScale = (RotatePointX-cur_x)*(RotatePointX-cur_x) + (RotatePointY-cur_y)*(RotatePointY-cur_y);  // 鼠标点击的点到控件中心点的距离
				s32 minScale = ((p_wptr->SliderDeviation)*16-sliderWidth)*((p_wptr->SliderDeviation)*16-sliderHeight);
				s32 maxScale = ((p_wptr->SliderDeviation)*16+sliderWidth)*((p_wptr->SliderDeviation)*16+sliderHeight);

				if (pressScale>= minScale && pressScale <= maxScale){
					Angle = computeAngle(cur_x,cur_y,sliderWidth,sliderHeight,RotatePointX,RotatePointY,width,height,p_wptr,lastAngle);

					if (!travelPrincipleMode)
					{
						if(lastAngle>=maxAngle*16 + (s16)(p_wptr->CircleSliderStartAngle)-10*16 && Angle <=minAngle*16 + (s16)(p_wptr->CircleSliderStartAngle)+10*16){   // 防止在滑到终点时，不小心点到刚开始的位置
							Angle = lastAngle;
						}
						if(lastAngle<=minAngle*16 + (s16)(p_wptr->CircleSliderStartAngle) + 10*16 && Angle >=maxAngle*16 + (s16)(p_wptr->CircleSliderStartAngle) - 10*16){  // 防止在开始滑动滑块时，不小心点到快结束的位置
							Angle = lastAngle;
						}

					}
					
					tag_value = (Angle -  (s16)(p_wptr->CircleSliderStartAngle))*(maxValue-minValue)/(maxAngle*16-minAngle*16) + minValue;

					if(p_wptr->BindTagID != 0)
					{
						TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
					}
				
					if(p_wptr->BindTagID != 0)
					{
						p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
					}
					p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

					if(p_wptr->TouchPropagation & TouchPropagationSet)
					{
						return WIDGET_TOUCH_CANCEL;
					}
					else
					{
						return AHMI_FUNC_SUCCESS;
					}
				} else{
					
					return WIDGET_TOUCH_CANCEL;
				}
			}
			else
			{

			}
		}
	}
	else if(actionType == ACTION_MOUSE_CANCLE)
	{
		// do nothing
	}

	//if(p_wptr->BindTagID != 0)
	//{
	//	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	//}
	//p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	if(p_wptr->TouchPropagation & TouchPropagationSet)
	{
		return WIDGET_TOUCH_CANCEL;
	}
	else
	{
		return AHMI_FUNC_SUCCESS;
	}
}

s16 CircleSliderClass::computeAngle(
	s32 x,  //1.11.4
	s32 y,	//1.11.4
	u16 sliderWidth, 	//1.11.4
	u16 sliderHeight,	//1.11.4
	s32 RotatePointX,	//1.11.4
	s32 RotatePointY,	//1.11.4
	s32 width,	//1.11.4
	s32 height,	//1.11.4
	WidgetClassPtr p_wptr,
	s16 lastAngle   //1.11.4
	){
	s16 Angle;
	myMathClass mymath;
	u16 rotatingDir;
	//rotatingDir
	rotatingDir  = (p_wptr->WidgetAttr & 0x20) ?1:0  ; //rotatingDir, 1 clockwise，0 anti-clockwise
	int travelPrincipleMode = p_wptr->TravelPrincipleMode;

	if (x >= -sliderWidth && x < RotatePointX) {
		if ( y > RotatePointY && y <= height+sliderHeight ) {
			Angle = mymath.GetAngleFromTan((long long)(((long long)(RotatePointX - x) << 20) / (y - RotatePointY)));
                    
		} else if ( y >= -sliderHeight && y <= RotatePointY ) {
			Angle = mymath.GetAngleFromTan((long long)(((long long)(RotatePointY - y) << 20) / (RotatePointX - x)));

			Angle += 90*16;
		}
	} else if (x >= RotatePointX && x <= width+sliderWidth) {
		if ( y >= -sliderHeight && y < RotatePointY ) {
			Angle = mymath.GetAngleFromTan((long long)(((long long)(x - RotatePointX) << 20) / (RotatePointY - y)));

			Angle += 180*16;
		} else if ( y > RotatePointY && y <= height+sliderHeight ){
			Angle = mymath.GetAngleFromTan((long long)(((long long)(x - RotatePointX) << 20) / (y - RotatePointY)));

			Angle = 360*16 - Angle;
		}else if(y == RotatePointY){
			Angle = 270*16;
		}

	} else {
		// Angle = lastArc
	}

	if (!travelPrincipleMode)
	{
		if(!rotatingDir){
			Angle = 360*16 - Angle;
		}
	}	

	if(Angle >= ((s16)(p_wptr->CircleSliderStartAngle))){
        Angle = Angle - ((s16)(p_wptr->CircleSliderStartAngle));
    } else{
        Angle = Angle + (p_wptr->CircleSliderMaxAngle)*16 - ((s16)(p_wptr->CircleSliderStartAngle));
    } 
	Angle += ((s16)(p_wptr->CircleSliderStartAngle));

	return Angle;
}
	
//**************************************
//reponse for Keyboard.Touch
//
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus CircleSliderClass::KeyboardTouchCtrl(
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{	
	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT || 
		p_wptr->WidgetWidth == 0 ||  
		p_wptr->WidgetHeight == 0)
	{
		ERROR_PRINT("ERROR: when drawing slider widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
    
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}

funcStatus CircleSliderClass::setATag(
	WidgetClassPtr p_wptr,    //widget pointer
	u16 value                 //value of animation tag
	)
{
	s32 ValueStart;
	s32 ValueStop;
	s32 maxValue;
	s32 minValue;
	s32 curValue;
	s16 Angle;
//	s16 startAngle;
//	s16 stopAngle;
	u16 transitionParamIn; //portion of animation progress 0.10
	u16 transitionParamOut; //portion of animation progress after varying function 0.10
#ifndef WHOLE_TRIBLE_BUFFER
	RefreshMsg refreshMsg;
#endif

	u16 totalDuration = p_wptr->ANIMATION_DURATION;
	if(totalDuration == 0 ) //no animation
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetAnimationDurationErr);

//	taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set meter animation tag %d", value);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= value;
	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	//char text[100];
	//sprintf(text,"loop: start: %d - stop: %d", ValueStart, ValueStop);
	//ERROR_PRINT(text);

	transitionParamIn = (value) * 1024 / totalDuration;//0.10
	
	gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);

	curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
	

	Angle = 16* (curValue - minValue) * ( (s16)(p_wptr->CircleSliderMaxAngle) - (s16)(p_wptr->CircleSliderMinAngle) ) /(maxValue - minValue);//12.4

	//Angle = 16 * (value) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) * (ValueStop - ValueStart) /(totalDuration * (maxValue - minValue)) + 16 * (ValueStart) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) / (maxValue - minValue);//12.4
	
	//record current animation by Mr.z
	p_wptr->ANIMATIONCURVALUE_L = (u16)curValue;
	p_wptr->ANIMATIONCURVALUE_H = (u16)(curValue >> 16);

	/*sprintf(text,"loop: animation start: %d - stop: %d ..", p_wptr->ANIMATIONCURVALUE_L, p_wptr->ANIMATIONCURVALUE_H);
	ERROR_PRINT(text);*/

	renderTextureAngle(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, Angle);

	if(value >= totalDuration)
		p_wptr->NOW_DURATION &= 0x0000; // set now frame to 0

	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}

#endif
