////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2023/6/14
// File Name:     Custom.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "Custom.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
extern u16  WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

//-----------------------------
// name: CustomClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
CustomClass::CustomClass()
{

}

//-----------------------------
// name: ~CustomClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
CustomClass::~CustomClass()
{

}

funcStatus CustomClass::initWidget(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,   
	u32 *u32p_sourceShift,  
	DynamicPageClassPtr pageptr,
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
		//set the widget value
		if(widgetCtrl(pageptr,p_wptr,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
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


funcStatus CustomClass::widgetCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,			
	u8 u8_pageRefresh				
	)
{
	TextureClassPtr texturePtr;
	DynamicPageClassPtr pCurrentPage;
	
	if((NULL == p_wptr)|| NULL == gPagePtr->pBasicTextureList){
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
		ERROR_PRINT("ERROR: when drawing gif widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
    
	return AHMI_FUNC_SUCCESS;
}

u8 *pSetValueCtrl;
u8	mStatusRegisterForCustom;  //the result of comparsion

void GetSource1Source2(DynamicPageClassPtr pagePtr, WidgetClassPtr p_wptr, CustomActionFormat* pCustomActionFormat, int32_t* pS1, int32_t* pS2, uint32_t tagvalue)
{
	switch(pCustomActionFormat->Opcode & SOURCE1MASK)
	{
		case SOURCE1_Imm:
			*pS1 = (pCustomActionFormat->Source1_LOW | (pCustomActionFormat->Source1_HIGH << 16));
		break;

		case SOURCE1_Tag:
			*pS1 = tagvalue;
		break;

		case SOURCE1_TexAttr:
			if(pCustomActionFormat->Source1_LOW == FocusedSlice)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].FocusedSlice;
			}
			else if(pCustomActionFormat->Source1_LOW == offsetX)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].OffsetX >> 4;
			}
			else if(pCustomActionFormat->Source1_LOW == offsetY)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].OffsetY >> 4;
			}
			else if(pCustomActionFormat->Source1_LOW == rotateRadius)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].RotateAngle >> 4;
			}
			else if(pCustomActionFormat->Source1_LOW == shearX)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].ShearAngleX >> 4;
			}
			else if(pCustomActionFormat->Source1_LOW == shearY)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].ShearAngleY >> 4;
			}
			else if(pCustomActionFormat->Source1_LOW == projectX)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].ShearAngleZX >> 4;
			}
			else if(pCustomActionFormat->Source1_LOW == projectY)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].ShearAngleZY >> 4;
			}
			else if(pCustomActionFormat->Source1_LOW == scaleX)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].ScalerX >> 9;
			}
			else if(pCustomActionFormat->Source1_LOW == scaleY)
			{
				*pS1 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source1_HIGH].ScalerY >> 9;
			}
		break;

		case SOURCE1_WidgtAttr:
			*pS1 = (int16_t)p_wptr->WDG_u16_userVariables[WIDGET_GENERAL_NUM + (pCustomActionFormat->Source1_LOW | (pCustomActionFormat->Source1_HIGH << 16))];
		break;

		default:

		break;
	}

	switch(pCustomActionFormat->Opcode & SOURCE2MASK)
	{
		case SOURCE2_Imm:
			*pS2 = (pCustomActionFormat->Source2_LOW | (pCustomActionFormat->Source2_HIGH << 16));
		break;

		case SOURCE2_Tag:
			*pS2 = tagvalue;
		break;

		case SOURCE2_TexAttr:
			if(pCustomActionFormat->Source2_LOW == FocusedSlice)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].FocusedSlice;
			}
			else if(pCustomActionFormat->Source2_LOW == offsetX)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].OffsetX >> 4;
			}
			else if(pCustomActionFormat->Source2_LOW == offsetY)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].OffsetY >> 4;
			}
			else if(pCustomActionFormat->Source2_LOW == rotateRadius)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].RotateAngle >> 4;
			}
			else if(pCustomActionFormat->Source2_LOW == shearX)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].ShearAngleX >> 4;
			}
			else if(pCustomActionFormat->Source2_LOW == shearY)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].ShearAngleY >> 4;
			}
			else if(pCustomActionFormat->Source2_LOW == projectX)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].ShearAngleZX >> 4;
			}
			else if(pCustomActionFormat->Source2_LOW == projectY)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].ShearAngleZY >> 4;
			}
			else if(pCustomActionFormat->Source2_LOW == scaleX)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].ScalerX >> 9;
			}
			else if(pCustomActionFormat->Source2_LOW == scaleY)
			{
				*pS2 = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pCustomActionFormat->Source2_HIGH].ScalerY >> 9;
			}
		break;

		case SOURCE2_WidgtAttr:
			*pS2 = (int16_t)p_wptr->WDG_u16_userVariables[WIDGET_GENERAL_NUM + (pCustomActionFormat->Source2_LOW | (pCustomActionFormat->Source2_HIGH << 16))];
		break;

		default:

		break;
	}
}

int SetValueCtrlDecoder(DynamicPageClassPtr pagePtr, WidgetClassPtr p_wptr, uint32_t tagvalue)
{
	TagClassPtr targetTag;
	u32 u32_imm;
	u8 u8_CdRun;
	u8 u8_CdRCd;
	u16 widgetID;
    u8 retflag = 0; //whether need stop action processor
	u8 retPageID = 0; //return Page ID

	u8_CdRun =  mStatusRegisterForCustom;//ban the conditional running
	                             //use it for conditional branch, if it is true, jump the next code
	u8_CdRCd =  1 ;
	mStatusRegisterForCustom = 0;
	
	switch((*(u16 *)pSetValueCtrl) & 0x3FF)
	{
		case(CUSTOM_BLANK):
		{
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_END):
		{
			void End();
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_JUMP):
		{
			CustomActionFormat *JUMP_CustomActionFormat;
			JUMP_CustomActionFormat = (CustomActionFormat *)pSetValueCtrl;

			if(!u8_CdRun || (u8_CdRun && mStatusRegisterForCustom == u8_CdRCd ))  
			{
				pSetValueCtrl = pSetValueCtrl + (JUMP_CustomActionFormat->Dest_LOW | (JUMP_CustomActionFormat->Dest_HIGH << 16));
			}
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_INC):
		{
			CustomActionFormat *INC_CustomActionFormat;
			int32_t s1,s2; 
			INC_CustomActionFormat = (CustomActionFormat *)pSetValueCtrl;

			if(!u8_CdRun || (u8_CdRun && mStatusRegisterForCustom == u8_CdRCd ))
			{
				GetSource1Source2(pagePtr, p_wptr, INC_CustomActionFormat, &s1, &s2, tagvalue);

				if((INC_CustomActionFormat->Opcode & DESTMASK) == DEST_Tag)
				{
					//todo
				}
				else if((INC_CustomActionFormat->Opcode & DESTMASK) == DEST_WidgtAttr)
				{
					p_wptr->WDG_u16_userVariables[WIDGET_GENERAL_NUM + (INC_CustomActionFormat->Dest_LOW | (INC_CustomActionFormat->Dest_HIGH << 16))] = s1 + s2;
				}
				else if((INC_CustomActionFormat->Opcode & DESTMASK) == DEST_TexAttr)
				{
					if(INC_CustomActionFormat->Dest_HIGH == FocusedSlice)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].FocusedSlice = s1 + s2;
					}
					else if(INC_CustomActionFormat->Dest_HIGH == offsetX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].OffsetX = ((s1 + s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == offsetY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].OffsetY = ((s1 + s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == rotateRadius)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].RotateAngle = ((s1 + s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == shearX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleX = ((s1 + s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == shearY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleY = ((s1 + s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == projectX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleZX = ((s1 + s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == projectY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleZY = ((s1 + s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == scaleX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ScalerX = ((s1 + s2));
					}
					else if(INC_CustomActionFormat->Dest_HIGH == scaleY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ScalerY = ((s1 + s2));
					}
				}
			}
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_DEC):
		{
			CustomActionFormat *INC_CustomActionFormat;
			int32_t s1,s2; 
			INC_CustomActionFormat = (CustomActionFormat *)pSetValueCtrl;

			if(!u8_CdRun || (u8_CdRun && mStatusRegisterForCustom == u8_CdRCd ))
			{
				GetSource1Source2(pagePtr, p_wptr, INC_CustomActionFormat, &s1, &s2, tagvalue);

				if((INC_CustomActionFormat->Opcode & DESTMASK) == DEST_Tag)
				{
					//todo
				}
				else if((INC_CustomActionFormat->Opcode & DESTMASK) == DEST_WidgtAttr)
				{
					p_wptr->WDG_u16_userVariables[WIDGET_GENERAL_NUM + (INC_CustomActionFormat->Dest_LOW | (INC_CustomActionFormat->Dest_HIGH << 16))] = s1 + s2;
				}
				else if((INC_CustomActionFormat->Opcode & DESTMASK) == DEST_TexAttr)
				{
					if(INC_CustomActionFormat->Dest_HIGH == FocusedSlice)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].FocusedSlice = s1 - s2;
					}
					else if(INC_CustomActionFormat->Dest_HIGH == offsetX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].OffsetX = ((s1 - s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == offsetY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].OffsetY = ((s1 - s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == rotateRadius)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].RotateAngle = ((s1 - s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == shearX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleX = ((s1 - s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == shearY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleY = ((s1 - s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == projectX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleZX = ((s1 - s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == projectY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ShearAngleZY = ((s1 - s2) << 4);
					}
					else if(INC_CustomActionFormat->Dest_HIGH == scaleX)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ScalerX = ((s1 - s2));
					}
					else if(INC_CustomActionFormat->Dest_HIGH == scaleY)
					{
						pagePtr->pBasicTextureList[p_wptr->StartNumofTex + INC_CustomActionFormat->Dest_LOW].ScalerY = ((s1 - s2));
					}
				}
			}
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_MORE):
		{
			CustomActionFormat *MORE_CustomActionFormat;
			uint32_t value, compareValue;

			MORE_CustomActionFormat = (CustomActionFormat *)pSetValueCtrl;

			if((MORE_CustomActionFormat->Opcode & DESTMASK) == DEST_Tag)
			{
				value = tagvalue;
			}
			else if((MORE_CustomActionFormat->Opcode & DESTMASK) == DEST_WidgtAttr)
			{
				value = p_wptr->WDG_u16_userVariables[WIDGET_GENERAL_NUM + (MORE_CustomActionFormat->Dest_LOW | (MORE_CustomActionFormat->Dest_HIGH << 16))];
			}
			else if((MORE_CustomActionFormat->Opcode & DESTMASK) == DEST_TexAttr)
			{
				if(MORE_CustomActionFormat->Dest_HIGH == FocusedSlice)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].FocusedSlice;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == offsetX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].OffsetX >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == offsetY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].OffsetY >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == rotateRadius)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].RotateAngle >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == shearX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleX >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == shearY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleY >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == projectX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleZX >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == projectY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleZY >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == scaleX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ScalerX;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == scaleY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ScalerY;
				}
			}

			if((MORE_CustomActionFormat->Opcode & SOURCE1MASK) == SOURCE1_Imm)
			{
				compareValue = (MORE_CustomActionFormat->Source1_LOW | (MORE_CustomActionFormat->Source1_HIGH << 16));
			}
			else if((MORE_CustomActionFormat->Opcode & SOURCE1MASK) == SOURCE1_Tag)
			{
				compareValue = tagvalue;
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegisterForCustom == u8_CdRCd ))  
			{
				if(value > compareValue)
					mStatusRegisterForCustom = 1;
				else mStatusRegisterForCustom = 0;
			}
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_LESS):
		{
			CustomActionFormat *MORE_CustomActionFormat;
			uint32_t value, compareValue;

			MORE_CustomActionFormat = (CustomActionFormat *)pSetValueCtrl;

			if((MORE_CustomActionFormat->Opcode & DESTMASK) == DEST_Tag)
			{
				value = tagvalue;
			}
			else if((MORE_CustomActionFormat->Opcode & DESTMASK) == DEST_WidgtAttr)
			{
				value = p_wptr->WDG_u16_userVariables[WIDGET_GENERAL_NUM + (MORE_CustomActionFormat->Dest_LOW | (MORE_CustomActionFormat->Dest_HIGH << 16))];
			}
			else if((MORE_CustomActionFormat->Opcode & DESTMASK) == DEST_TexAttr)
			{
				if(MORE_CustomActionFormat->Dest_HIGH == FocusedSlice)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].FocusedSlice;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == offsetX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].OffsetX >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == offsetY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].OffsetY >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == rotateRadius)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].RotateAngle >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == shearX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleX >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == shearY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleY >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == projectX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleZX >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == projectY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ShearAngleZY >> 4;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == scaleX)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ScalerX;
				}
				else if(MORE_CustomActionFormat->Dest_HIGH == scaleY)
				{
					value = pagePtr->pBasicTextureList[p_wptr->StartNumofTex + MORE_CustomActionFormat->Dest_LOW].ScalerY;
				}
			}

			if((MORE_CustomActionFormat->Opcode & SOURCE1MASK) == SOURCE1_Imm)
			{
				compareValue = (MORE_CustomActionFormat->Source1_LOW | (MORE_CustomActionFormat->Source1_HIGH << 16));
			}
			else if((MORE_CustomActionFormat->Opcode & SOURCE1MASK) == SOURCE1_Tag)
			{
				compareValue = tagvalue;
			}

			if(!u8_CdRun || (u8_CdRun && mStatusRegisterForCustom == u8_CdRCd ))  
			{
				if(value < compareValue)
					mStatusRegisterForCustom = 1;
				else mStatusRegisterForCustom = 0;
			}
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_MUL):
		{
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_DIV):
		{
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_EQU):
		{
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		case(CUSTOM_NEQ):
		{
			pSetValueCtrl = pSetValueCtrl + CustomActionFormat_Length - 1;
			break;
		}
		default:
			break;
	}

	return 0;
}
//**************************************
//reponse for Tag.setValue
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget
//**************************************
funcStatus CustomClass::SetValueCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,
	ActionTriggerClassPtr ActionPtr,
	u8 u8_widgetRefresh      
	)
{
    u16 value, err;
	
	if((NULL == p_wptr) || NULL == gPagePtr->pBasicTextureList){
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
		ERROR_PRINT("ERROR: when drawing gif widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	value = ActionPtr->mTagPtr->getValue();

	pSetValueCtrl = (pageptr->pActionInstructions + p_wptr->OnLongPressAction);
	while(pSetValueCtrl < (pageptr->pActionInstructions + pageptr->mActionInstructionsSize) && (*(u16 *)pSetValueCtrl != 0))
 	{
 		err = SetValueCtrlDecoder(pageptr, p_wptr, value);
		if(err == AHMI_FUNC_SUCCESS) return AHMI_FUNC_SUCCESS;
 		pSetValueCtrl++;
 	}

	//refresh this widget
	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	return AHMI_FUNC_SUCCESS;
}
#endif
