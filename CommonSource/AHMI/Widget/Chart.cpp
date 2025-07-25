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
//   add comments by zuz 2018/10/29
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "Chart.h"
#include "drawImmediately_cd.h"
#include "myMathClass.h"
#include "WriteTexture.h"

#ifdef AHMI_CORE
//extern ActionTriggerClass		gWidgetTagTrigger;
//extern ActionTriggerClass		gMouseTagTrigger;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16  WorkingPageID;

//extern QueueHandle_t		ActionInstructionQueue;
uint32_t datanumber[Chart_Num_MAX] = {0};

extern Point DataBuffer[800];
extern uint32_t pointnumber;

extern Point ControlPoint[3];//1.27.4
extern Point DataPoint[Chart_Num_MAX][3];
extern Point WorkPoint[3];

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

#if defined(VERSION_2)
uint8_t ChangeModeFlag[Chart_Num_MAX] = {0};
extern uint32_t olddata[Chart_Num_MAX];
#endif
#ifdef PC_SIM
extern uint8_t p_OscilloscopeInitFlag[Chart_Num_MAX];
#endif
#if defined(EMBEDDED) && defined(VERSION_2)
extern uint8_t* p_Oscilloscopetexture[Chart_Num_MAX];
#endif

//-----------------------------
// name�� TouchTrackClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
ChartClass::ChartClass()
{

}

//-----------------------------
// name�� ~TouchTrackClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
ChartClass::~ChartClass()
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
funcStatus ChartClass::initWidget(
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
	#ifndef CHIP_GC9002_NoHyper
	TagClassPtr bindTag;
	ActionTriggerClass tagtrigger;
	WidgetClassInterface myWidgetClassInterface;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 Op_sourceShift = 0;
	funcStatus status;
	#endif

	if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	
	#ifdef PC_SIM
	if(p_OscilloscopeInitFlag[p_wptr->IDnumber] == 0)
	#endif
	#if defined(EMBEDDED) && defined(VERSION_2)
	if(p_Oscilloscopetexture[p_wptr->IDnumber] == 0)
	#endif
	{
		//widget height supplement length (64bit align)
		uint32_t heighttemp = p_wptr->WidgetHeight;
		if( (heighttemp & 0x3f) != 0)
		{
			heighttemp = ((heighttemp >> 6) << 6) + 64;
		}
				
		//Initialize Oscilloscope Texture RAM
		initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, heighttemp);
	}
	
//	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
//	{
//		//find the bind tag
//		bindTag = &TagPtr->getTag(p_wptr->BindTagID);
//		//set the value of widget according to tag
//		tagtrigger.mTagPtr = bindTag;
//		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
//		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
//			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
//        
//        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
//	}
	
	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	#ifdef VERSION_2
	if(myWidgetClassInterface.drawChartTexture(pagePtr, p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn) != AHMI_FUNC_SUCCESS)
	#endif	
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawChartTexture(pagePtr, p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn);
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

//-----------------------------
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 ActionPtr                //the value of the tag
//  @param	 u8_pageRefresh           //refresh type
// addtional comments :
//   none
//-----------------------------
funcStatus ChartClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,		
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh			
	)
{	
#if 0
	#ifndef CHIP_GC9002_NoHyper
	s32 XMinValue, XMaxValue, YMinValue, YMaxValue, XtotalValue, YtotalValue;
	u32 minValue, maxValue, oldValue, curValue, totalValue, displayHeight = 0;
	s16 old_x, old_y, cur_x, cur_y;
	u16 xCount,xPadding,heighttemp;
	u8 showMode;
	u8 linewidth;
	Point FinalPoint;
	int i;

	//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
	if(p_wptr->YPADDING == 3)
	{
		#ifdef PC_SIM
		if(p_OscilloscopeInitFlag[p_wptr->IDnumber] == 0)
		#endif
		#if defined(EMBEDDED) && defined(VERSION_2)
		if(p_Oscilloscopetexture[p_wptr->IDnumber] == 0)
		#endif
		{
			p_wptr->OldValueL = 0x7FFF;
			p_wptr->OldValueH = 0x7FFF;

			/********* Initialize Oscilloscope Texture RAM ************/
			initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, p_wptr->WidgetHeight);
		}

		if(p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag)
		{
			XMinValue = p_wptr->XMinValueL | (p_wptr->XMinValueH << 16);
			XMaxValue = p_wptr->XMaxValueL | (p_wptr->XMaxValueH << 16);
			YMinValue = p_wptr->MinValueL | (p_wptr->MinValueH << 16);
			YMaxValue = p_wptr->MaxValueL | (p_wptr->MaxValueH << 16);
			XtotalValue = XMaxValue - XMinValue;
			YtotalValue = YMaxValue - YMinValue;

			if((p_wptr->OldValueL != 0x7FFF) && (p_wptr->OldValueH != 0x7FFF) && (ActionPtr->mTagPtr->getValue() != 0x7FFF7FFF))
			{
				oldValue = (u32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL );
				curValue = ActionPtr->mTagPtr->getValue();
			}
			else
			{
				curValue = ActionPtr->mTagPtr->getValue();
				p_wptr->OldValueL = (u16)curValue;
				p_wptr->OldValueH = (u16)(curValue >> 16); 
				return AHMI_FUNC_SUCCESS;
			}
			
			old_x = ((oldValue & 0xFFFF0000) >> 16);
			old_y = (oldValue & 0xFFFF);
			cur_x = ((curValue & 0xFFFF0000) >> 16);
			cur_y = (curValue & 0xFFFF);

			if(old_x < XMinValue || old_x >= XMaxValue || old_y < YMinValue || old_y >= YMaxValue ||
			   cur_x < XMinValue || cur_x >= XMaxValue || cur_y < YMinValue || cur_y >= YMaxValue ||
			   ((old_x == cur_x) && (old_y == cur_y)))
			{
				return AHMI_FUNC_SUCCESS;
			}

			old_x = (old_x - XMinValue) * p_wptr->WidgetWidth / XtotalValue;
			old_y = (old_y - YMinValue) * p_wptr->WidgetHeight / YtotalValue;
			cur_x = (cur_x - XMinValue) * p_wptr->WidgetWidth / XtotalValue;
			cur_y = (cur_y - YMinValue) * p_wptr->WidgetHeight / YtotalValue;

			old_y = p_wptr->WidgetHeight - old_y;
			cur_y = p_wptr->WidgetHeight - cur_y;

			linewidth = p_wptr->SHOWMODE >> 1;
			Bresenham(p_wptr->IDnumber,old_x,old_y,cur_x,cur_y,p_wptr->WidgetWidth,p_wptr->WidgetHeight,linewidth);

			p_wptr->OldValueL = (u16)curValue;
			p_wptr->OldValueH = (u16)(curValue >> 16); 
		}
	}
	else
	{
		//for embedded gpu read right ram data
		#if defined(EMBEDDED) && defined(VERSION_2)
		for(i = 0; i < 0x80000; i++);
		#endif
		
		minValue = p_wptr->MinValueL | (p_wptr->MinValueH << 16);
		maxValue = p_wptr->MaxValueL | (p_wptr->MaxValueH << 16);
		totalValue = maxValue - minValue;
		xCount = p_wptr->XCOUNT;
		xPadding = p_wptr->WidgetWidth / (xCount - 1);
		showMode = p_wptr->SHOWMODE & 0x1;
		linewidth = p_wptr->SHOWMODE >> 1;
		
		//if mValue < min, mValue assignment min
		if(ActionPtr->mTagPtr->getValue() <= minValue)
		{
			displayHeight = 1;
		}
		else if(ActionPtr->mTagPtr->getValue() >= maxValue)
		{
			displayHeight = p_wptr->WidgetHeight;
		}
		else
		{
			displayHeight = (ActionPtr->mTagPtr->getValue() - minValue) * p_wptr->WidgetHeight / totalValue;
		}
		
		if(p_wptr->YCOUNT)//linear interpolation
		{
			#ifdef PC_SIM
			if(p_OscilloscopeInitFlag[p_wptr->IDnumber] == 0)
			#endif
			#if defined(EMBEDDED) && defined(VERSION_2)
			if(p_Oscilloscopetexture[p_wptr->IDnumber] == 0)
			#endif
			{
				DataPoint[p_wptr->IDnumber][0].X = 0;
				DataPoint[p_wptr->IDnumber][0].Y = 0;
				DataPoint[p_wptr->IDnumber][1].X = 0;
				DataPoint[p_wptr->IDnumber][1].Y = displayHeight;
				
				olddata[p_wptr->IDnumber] = displayHeight;

		           //widget height supplement length (64bit align)
				heighttemp = p_wptr->WidgetHeight;
				if( (heighttemp & 0x3f) != 0)
				{
					heighttemp = ((heighttemp >> 6) << 6) + 64;
				}
		           
				/********* Initialize Oscilloscope Texture RAM ************/
				initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, heighttemp);
			}

			if(p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag)
			{
				DataPoint[p_wptr->IDnumber][0] = DataPoint[p_wptr->IDnumber][1];
				DataPoint[p_wptr->IDnumber][1].X = DataPoint[p_wptr->IDnumber][0].X + xPadding;
				if(displayHeight > p_wptr->WidgetHeight)
				{
					DataPoint[p_wptr->IDnumber][1].Y = p_wptr->WidgetHeight - 1;
				}
				else
				{
					DataPoint[p_wptr->IDnumber][1].Y = displayHeight;
				}

				for(int i=0;i<xPadding+1;i++)
				{
				    for (int j = 0; j < 2; j++)
				    {
				        WorkPoint[j] = DataPoint[p_wptr->IDnumber][j];
				    }
					int16_t t = (i << 13)/(xPadding+1);//t0 1.2.13
					FinalPoint = GetFinalPoint_s16(t,2);

					//widget height supplement length (64bit align)
					heighttemp = p_wptr->WidgetHeight;
					if( (heighttemp & 0x3f) != 0)
					{
						heighttemp = ((heighttemp >> 6) << 6) + 64;
					}

					if(FinalPoint.X >= p_wptr->WidgetWidth)
					{
						//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
						if(p_wptr->YPADDING == 2)
						{
							ChangeModeFlag[p_wptr->IDnumber] = 1;
						}
						FinalPoint.X -= p_wptr->WidgetWidth;
					}
					if(FinalPoint.Y > p_wptr->WidgetHeight)
					{
						updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, p_wptr->WidgetHeight, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
						datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
					}
					else
					{
						updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, FinalPoint.Y, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
						datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
					}
				}

				if(DataPoint[p_wptr->IDnumber][1].X > p_wptr->WidgetWidth)
				{
					DataPoint[p_wptr->IDnumber][1].X -= p_wptr->WidgetWidth;
				}
			}
		}
		else//Bessel curve interpolation
		{
			#ifdef PC_SIM
			if(p_OscilloscopeInitFlag[p_wptr->IDnumber] == 0)
			#endif
			#if defined(EMBEDDED) && defined(VERSION_2)
			if(p_Oscilloscopetexture[p_wptr->IDnumber] == 0)
			#endif
			{
				/********* Initialize data points and control points ************/
				for(i = 0; i < 3; i++)
				{
					DataPoint[p_wptr->IDnumber][i].X = 0;
					DataPoint[p_wptr->IDnumber][i].Y = 0;
					ControlPoint[i].X = 0;
					ControlPoint[i].Y = 0;
				}
				
				DataPoint[p_wptr->IDnumber][1].Y = displayHeight;
				DataPoint[p_wptr->IDnumber][2].Y = displayHeight;
				olddata[p_wptr->IDnumber] = displayHeight;

				//widget height supplement length (64bit align)
				heighttemp = p_wptr->WidgetHeight;
				if( (heighttemp & 0x3f) != 0)
				{
					heighttemp = ((heighttemp >> 6) << 6) + 64;
				}
		           
				/********* Initialize Oscilloscope Texture RAM ************/
				initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, heighttemp);
			}

			if(p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag)
			{
				/********* Update data points and control points based on Tag values ************/
				DataPoint[p_wptr->IDnumber][0] = DataPoint[p_wptr->IDnumber][1];
				DataPoint[p_wptr->IDnumber][1] = DataPoint[p_wptr->IDnumber][2];
				DataPoint[p_wptr->IDnumber][2].X = DataPoint[p_wptr->IDnumber][1].X + xPadding;
				if(displayHeight > p_wptr->WidgetHeight)
				{
					DataPoint[p_wptr->IDnumber][2].Y = p_wptr->WidgetHeight - 1;
				}
				else
				{
					DataPoint[p_wptr->IDnumber][2].Y = displayHeight;
				}

				/********* Getting the midpoint of the data point ************/
				ControlPoint[0].X = (DataPoint[p_wptr->IDnumber][0].X + DataPoint[p_wptr->IDnumber][1].X) >> 1;
				ControlPoint[0].Y = (DataPoint[p_wptr->IDnumber][0].Y + DataPoint[p_wptr->IDnumber][1].Y) >> 1;
				ControlPoint[1].X = DataPoint[p_wptr->IDnumber][1].X;
				ControlPoint[1].Y = DataPoint[p_wptr->IDnumber][1].Y;
				ControlPoint[2].X = (DataPoint[p_wptr->IDnumber][1].X + DataPoint[p_wptr->IDnumber][2].X) >> 1;
				ControlPoint[2].Y = (DataPoint[p_wptr->IDnumber][1].Y + DataPoint[p_wptr->IDnumber][2].Y) >> 1;

				ControlPoint[0].X = ControlPoint[0].X << 4;
				ControlPoint[0].Y =	ControlPoint[0].Y << 4;
				ControlPoint[1].X =	ControlPoint[1].X << 4;
				ControlPoint[1].Y =	ControlPoint[1].Y << 4;
				ControlPoint[2].X =	ControlPoint[2].X << 4;
				ControlPoint[2].Y =	ControlPoint[2].Y << 4;

				/********* Midpoint optimization of second-order Bessel curve ************/
				GetAllPoint(ControlPoint[0], ControlPoint[1], ControlPoint[2]); //all output points in DataBuffer[] arry


				/********* Initial two points draw straight lines ************/
				if((ControlPoint[0].X == ControlPoint[1].X) && (pointnumber == 0))
				{
					//Fill all pixels less than mValue in "0" column when the data points are dense
					if(xPadding < 4)
					{
						showMode = 0x1;
					}
					
					WorkPoint[0] = DataPoint[p_wptr->IDnumber][1];
					if(DataPoint[p_wptr->IDnumber][1].Y == DataPoint[p_wptr->IDnumber][2].Y)
					{
						WorkPoint[1].X = DataPoint[p_wptr->IDnumber][2].X / 2;
						WorkPoint[1].Y = DataPoint[p_wptr->IDnumber][2].Y;
					}
					else
					{
						WorkPoint[1].X = DataPoint[p_wptr->IDnumber][2].X / 2;
						WorkPoint[1].Y = DataPoint[p_wptr->IDnumber][2].Y / 2;
					}

					for(int i=0;i<xPadding+1;i++)
					{
						int16_t t = (i << 13)/(xPadding+1);//t0 1.2.13
						FinalPoint = GetFinalPoint_s16(t,2);

						//widget height supplement length (64bit align)
						heighttemp = p_wptr->WidgetHeight;
						if( (heighttemp & 0x3f) != 0)
						{
							heighttemp = ((heighttemp >> 6) << 6) + 64;
						}

						if(FinalPoint.Y > p_wptr->WidgetHeight)
						{
							updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, p_wptr->WidgetHeight, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
							datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
						}
						else
						{
							updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, FinalPoint.Y, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
							datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
						}
					}
				}
				/********* Initial two points draw straight lines end *********/

				/********* X-axis overflow treatment ************/
				if(ControlPoint[2].X > (p_wptr->WidgetWidth << 4))
				{
					DataPoint[p_wptr->IDnumber][1].X -= p_wptr->WidgetWidth;
					DataPoint[p_wptr->IDnumber][2].X -= p_wptr->WidgetWidth;
					ControlPoint[2].X -= (p_wptr->WidgetWidth << 4);
				}

				/********* Data points are sorted from small to large ************/
				for(uint32_t i = 0;i < pointnumber; i++)
				{
					for(uint32_t j = i + 1;j < pointnumber; j++)
					{
						if(DataBuffer[i].X > DataBuffer[j].X)
						{
							Point TempPoint = DataBuffer[j];
							DataBuffer[j] = DataBuffer[i];
							DataBuffer[i] = TempPoint;
						}
					}
				}

				if(pointnumber != 0)
				{
					DataBuffer[pointnumber++] = ControlPoint[2];
				}

				//Rounding Handle
				HandleAllPoint();

				/********* X-axis overflow treatment ************/
				for(uint32_t i = 0;i < pointnumber; i++)
				{
					//widget height supplement length (64bit align)
					heighttemp = p_wptr->WidgetHeight;
					if( (heighttemp & 0x3f) != 0)
					{
						heighttemp = ((heighttemp >> 6) << 6) + 64;
					}

					/********* X-axis overflow treatment ************/
					if(DataBuffer[i].X >= p_wptr->WidgetWidth)
					{
						//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
						if(p_wptr->YPADDING == 2)
						{
							ChangeModeFlag[p_wptr->IDnumber] = 1;
						}
						DataBuffer[i].X -= p_wptr->WidgetWidth;
					}
					updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, DataBuffer[i].Y, DataBuffer[i].X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
					datanumber[p_wptr->IDnumber] = DataBuffer[i].X + linewidth;
				}

				pointnumber = 0;
			}
		}
	}
	#endif //endof #ifndef CHIP_GC9002_NoHyper
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
funcStatus ChartClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	#ifndef CHIP_GC9002_NoHyper
	//u32 actionAddr;
	//s32 oldValue=0;
	//s32 value;
	//s32 lowAlarmValue ;
	//s32 highAlarmValue;
	//u16 oldValueinit=0;

	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	//oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	//lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL );
	//highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL);
	//value = (s32)(ActionPtr->mTagPtr->mValue);

	//if(u8_widgetRefresh == 0 && ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
	//{
	//	if(oldValueinit)
	//	{
	//		oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
	//		if(p_wptr->EnterLowAlarmAction && oldValue > lowAlarmValue && value <= lowAlarmValue)
	//		{
	//			//enter low
	//			if(p_wptr->EnterLowAlarmAction <= pagePtr->mActionInstructionsSize)
	//			{
	//				actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->EnterLowAlarmAction);
	//				xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
	//			}		
	//		}
	//		else if(p_wptr->LeaveLowAlarmAction && oldValue <= lowAlarmValue && value > lowAlarmValue)
	//		{
	//			//leave low
	//			if(p_wptr->LeaveLowAlarmAction <= pagePtr->mActionInstructionsSize)
	//			{
	//				actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->LeaveLowAlarmAction);
	//				xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
	//			}		
	//		}
	//		if(p_wptr->EnterHighAlarmAction && oldValue < highAlarmValue && value >= highAlarmValue)
	//		{
	//			//enter high
	//			if(p_wptr->EnterHighAlarmAction <= pagePtr->mActionInstructionsSize)
	//			{
	//				actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->EnterHighAlarmAction);
	//				xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
	//			}
	//		}
	//		else if(p_wptr->LeaveHighAlarmAction && oldValue >= highAlarmValue && value < highAlarmValue)
	//		{
	//			//leave high
	//			if(p_wptr->LeaveHighAlarmAction <= pagePtr->mActionInstructionsSize)
	//			{
	//				actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->LeaveHighAlarmAction);
	//				xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
	//			}	
	//		}
	//		//tagchange  by Mr.z
	//		if(oldValue != value){
	//			if(p_wptr->TagChangeAction && p_wptr->TagChangeAction<= pagePtr->mActionInstructionsSize)
	//			{
	//				actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->TagChangeAction);
	//				xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pagePtr->mActionInstructionsSize))
	//		{
	//			actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->TagChangeAction);
	//			xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
	//		}
	//	}
	//}

	s32 XMinValue, XMaxValue, YMinValue, YMaxValue, XtotalValue, YtotalValue;
	u32 minValue, maxValue, oldValue, curValue, totalValue, displayHeight = 0;
	s16 old_x, old_y, cur_x, cur_y;
	u16 xCount,xPadding,heighttemp;
	u8 showMode;
	u8 linewidth;
	Point FinalPoint;
	int i;

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;

	//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
	if(p_wptr->YPADDING == 3)
	{
		#ifdef PC_SIM
		if(p_OscilloscopeInitFlag[p_wptr->IDnumber] == 0)
		#endif
		#if defined(EMBEDDED) && defined(VERSION_2)
		if(p_Oscilloscopetexture[p_wptr->IDnumber] == 0)
		#endif
		{
			p_wptr->OldValueL = 0x7FFF;
			p_wptr->OldValueH = 0x7FFF;

			/********* Initialize Oscilloscope Texture RAM ************/
			initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, p_wptr->WidgetHeight);
		}

		if(p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag)
		{
			XMinValue = p_wptr->XMinValueL | (p_wptr->XMinValueH << 16);
			XMaxValue = p_wptr->XMaxValueL | (p_wptr->XMaxValueH << 16);
			YMinValue = p_wptr->MinValueL | (p_wptr->MinValueH << 16);
			YMaxValue = p_wptr->MaxValueL | (p_wptr->MaxValueH << 16);
			XtotalValue = XMaxValue - XMinValue;
			YtotalValue = YMaxValue - YMinValue;

			if((p_wptr->OldValueL != 0x7FFF) && (p_wptr->OldValueH != 0x7FFF) && (ActionPtr->mTagPtr->getValue() != 0x7FFF7FFF))
			{
				oldValue = (u32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL );
				curValue = ActionPtr->mTagPtr->getValue();
			}
			else
			{
				curValue = ActionPtr->mTagPtr->getValue();
				p_wptr->OldValueL = (u16)curValue;
				p_wptr->OldValueH = (u16)(curValue >> 16); 
				return AHMI_FUNC_SUCCESS;
			}
			
			old_x = ((oldValue & 0xFFFF0000) >> 16);
			old_y = (oldValue & 0xFFFF);
			cur_x = ((curValue & 0xFFFF0000) >> 16);
			cur_y = (curValue & 0xFFFF);

			if(old_x < XMinValue || old_x >= XMaxValue || old_y < YMinValue || old_y >= YMaxValue ||
			   cur_x < XMinValue || cur_x >= XMaxValue || cur_y < YMinValue || cur_y >= YMaxValue ||
			   ((old_x == cur_x) && (old_y == cur_y)))
			{
				return AHMI_FUNC_SUCCESS;
			}

			old_x = (old_x - XMinValue) * p_wptr->WidgetWidth / XtotalValue;
			old_y = (old_y - YMinValue) * p_wptr->WidgetHeight / YtotalValue;
			cur_x = (cur_x - XMinValue) * p_wptr->WidgetWidth / XtotalValue;
			cur_y = (cur_y - YMinValue) * p_wptr->WidgetHeight / YtotalValue;

			old_y = p_wptr->WidgetHeight - old_y;
			cur_y = p_wptr->WidgetHeight - cur_y;

			linewidth = p_wptr->SHOWMODE >> 1;
			Bresenham(p_wptr->IDnumber,old_x,old_y,cur_x,cur_y,p_wptr->WidgetWidth,p_wptr->WidgetHeight,linewidth);

			p_wptr->OldValueL = (u16)curValue;
			p_wptr->OldValueH = (u16)(curValue >> 16); 
		}
	}
	else
	{
		//for embedded gpu read right ram data
		#if defined(EMBEDDED) && defined(VERSION_2)
		for(i = 0; i < 0x80000; i++);
		#endif
		
		minValue = p_wptr->MinValueL | (p_wptr->MinValueH << 16);
		maxValue = p_wptr->MaxValueL | (p_wptr->MaxValueH << 16);
		totalValue = maxValue - minValue;
		xCount = p_wptr->XCOUNT;
		xPadding = p_wptr->WidgetWidth / (xCount - 1);
		showMode = p_wptr->SHOWMODE & 0x1;
		linewidth = p_wptr->SHOWMODE >> 1;
		
		//if mValue < min, mValue assignment min
		if(ActionPtr->mTagPtr->getValue() <= minValue)
		{
			displayHeight = 1;
		}
		else if(ActionPtr->mTagPtr->getValue() >= maxValue)
		{
			displayHeight = p_wptr->WidgetHeight;
		}
		else
		{
			displayHeight = (ActionPtr->mTagPtr->getValue() - minValue) * p_wptr->WidgetHeight / totalValue;
		}
		
		if(p_wptr->YCOUNT)//linear interpolation
		{
			#ifdef PC_SIM
			if(p_OscilloscopeInitFlag[p_wptr->IDnumber] == 0)
			#endif
			#if defined(EMBEDDED) && defined(VERSION_2)
			if(p_Oscilloscopetexture[p_wptr->IDnumber] == 0)
			#endif
			{
				DataPoint[p_wptr->IDnumber][0].X = 0;
				DataPoint[p_wptr->IDnumber][0].Y = 0;
				DataPoint[p_wptr->IDnumber][1].X = 0;
				DataPoint[p_wptr->IDnumber][1].Y = displayHeight;
				
				olddata[p_wptr->IDnumber] = displayHeight;

				//widget height supplement length (64bit align)
				heighttemp = p_wptr->WidgetHeight;
				if( (heighttemp & 0x3f) != 0)
				{
					heighttemp = ((heighttemp >> 6) << 6) + 64;
				}
		           
				/********* Initialize Oscilloscope Texture RAM ************/
				initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, heighttemp);
			}

			if(p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag)
			{
				DataPoint[p_wptr->IDnumber][0] = DataPoint[p_wptr->IDnumber][1];
				DataPoint[p_wptr->IDnumber][1].X = DataPoint[p_wptr->IDnumber][0].X + xPadding;
				if(displayHeight > p_wptr->WidgetHeight)
				{
					DataPoint[p_wptr->IDnumber][1].Y = p_wptr->WidgetHeight - 1;
				}
				else
				{
					DataPoint[p_wptr->IDnumber][1].Y = displayHeight;
				}

				for(int i=0;i<xPadding+1;i++)
				{
				    for (int j = 0; j < 2; j++)
				    {
				        WorkPoint[j] = DataPoint[p_wptr->IDnumber][j];
				    }
					int16_t t = (i << 13)/(xPadding+1);//t0 1.2.13
					FinalPoint = GetFinalPoint_s16(t,2);

					//widget height supplement length (64bit align)
					heighttemp = p_wptr->WidgetHeight;
					if( (heighttemp & 0x3f) != 0)
					{
						heighttemp = ((heighttemp >> 6) << 6) + 64;
					}

					if(FinalPoint.X >= p_wptr->WidgetWidth)
					{
						//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
						if(p_wptr->YPADDING == 2)
						{
							ChangeModeFlag[p_wptr->IDnumber] = 1;
						}
						FinalPoint.X -= p_wptr->WidgetWidth;
					}
					if(FinalPoint.Y > p_wptr->WidgetHeight)
					{
						updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, p_wptr->WidgetHeight, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
						datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
					}
					else
					{
						updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, FinalPoint.Y, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
						datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
					}
				}

				if(DataPoint[p_wptr->IDnumber][1].X > p_wptr->WidgetWidth)
				{
					DataPoint[p_wptr->IDnumber][1].X -= p_wptr->WidgetWidth;
				}
			}
		}
		else//Bessel curve interpolation
		{
			#ifdef PC_SIM
			if(p_OscilloscopeInitFlag[p_wptr->IDnumber] == 0)
			#endif
			#if defined(EMBEDDED) && defined(VERSION_2)
			if(p_Oscilloscopetexture[p_wptr->IDnumber] == 0)
			#endif
			{
				/********* Initialize data points and control points ************/
				for(i = 0; i < 3; i++)
				{
					DataPoint[p_wptr->IDnumber][i].X = 0;
					DataPoint[p_wptr->IDnumber][i].Y = 0;
					ControlPoint[i].X = 0;
					ControlPoint[i].Y = 0;
				}
				
				DataPoint[p_wptr->IDnumber][1].Y = displayHeight;
				DataPoint[p_wptr->IDnumber][2].Y = displayHeight;
				olddata[p_wptr->IDnumber] = displayHeight;

				//widget height supplement length (64bit align)
				heighttemp = p_wptr->WidgetHeight;
				if( (heighttemp & 0x3f) != 0)
				{
					heighttemp = ((heighttemp >> 6) << 6) + 64;
				}
		           
				/********* Initialize Oscilloscope Texture RAM ************/
				initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, heighttemp);
			}

			if(p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag)
			{
				/********* Update data points and control points based on Tag values ************/
				DataPoint[p_wptr->IDnumber][0] = DataPoint[p_wptr->IDnumber][1];
				DataPoint[p_wptr->IDnumber][1] = DataPoint[p_wptr->IDnumber][2];
				DataPoint[p_wptr->IDnumber][2].X = DataPoint[p_wptr->IDnumber][1].X + xPadding;
				if(displayHeight > p_wptr->WidgetHeight)
				{
					DataPoint[p_wptr->IDnumber][2].Y = p_wptr->WidgetHeight - 1;
				}
				else
				{
					DataPoint[p_wptr->IDnumber][2].Y = displayHeight;
				}

				/********* Getting the midpoint of the data point ************/
				ControlPoint[0].X = (DataPoint[p_wptr->IDnumber][0].X + DataPoint[p_wptr->IDnumber][1].X) >> 1;
				ControlPoint[0].Y = (DataPoint[p_wptr->IDnumber][0].Y + DataPoint[p_wptr->IDnumber][1].Y) >> 1;
				ControlPoint[1].X = DataPoint[p_wptr->IDnumber][1].X;
				ControlPoint[1].Y = DataPoint[p_wptr->IDnumber][1].Y;
				ControlPoint[2].X = (DataPoint[p_wptr->IDnumber][1].X + DataPoint[p_wptr->IDnumber][2].X) >> 1;
				ControlPoint[2].Y = (DataPoint[p_wptr->IDnumber][1].Y + DataPoint[p_wptr->IDnumber][2].Y) >> 1;

				ControlPoint[0].X = ControlPoint[0].X << 4;
				ControlPoint[0].Y =	ControlPoint[0].Y << 4;
				ControlPoint[1].X =	ControlPoint[1].X << 4;
				ControlPoint[1].Y =	ControlPoint[1].Y << 4;
				ControlPoint[2].X =	ControlPoint[2].X << 4;
				ControlPoint[2].Y =	ControlPoint[2].Y << 4;

				/********* Midpoint optimization of second-order Bessel curve ************/
				GetAllPoint(ControlPoint[0], ControlPoint[1], ControlPoint[2]); //all output points in DataBuffer[] arry


				/********* Initial two points draw straight lines ************/
				if((ControlPoint[0].X == ControlPoint[1].X) && (pointnumber == 0))
				{
					//Fill all pixels less than mValue in "0" column when the data points are dense
					if(xPadding < 4)
					{
						showMode = 0x1;
					}
					
					WorkPoint[0] = DataPoint[p_wptr->IDnumber][1];
					if(DataPoint[p_wptr->IDnumber][1].Y == DataPoint[p_wptr->IDnumber][2].Y)
					{
						WorkPoint[1].X = DataPoint[p_wptr->IDnumber][2].X / 2;
						WorkPoint[1].Y = DataPoint[p_wptr->IDnumber][2].Y;
					}
					else
					{
						WorkPoint[1].X = DataPoint[p_wptr->IDnumber][2].X / 2;
						WorkPoint[1].Y = DataPoint[p_wptr->IDnumber][2].Y / 2;
					}

					for(int i=0;i<xPadding+1;i++)
					{
						int16_t t = (i << 13)/(xPadding+1);//t0 1.2.13
						FinalPoint = GetFinalPoint_s16(t,2);

						//widget height supplement length (64bit align)
						heighttemp = p_wptr->WidgetHeight;
						if( (heighttemp & 0x3f) != 0)
						{
							heighttemp = ((heighttemp >> 6) << 6) + 64;
						}

						if(FinalPoint.Y > p_wptr->WidgetHeight)
						{
							updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, p_wptr->WidgetHeight, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
							datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
						}
						else
						{
							updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, FinalPoint.Y, FinalPoint.X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
							datanumber[p_wptr->IDnumber] = FinalPoint.X + linewidth;
						}
					}
				}
				/********* Initial two points draw straight lines end *********/

				/********* X-axis overflow treatment ************/
				if(ControlPoint[2].X > (p_wptr->WidgetWidth << 4))
				{
					DataPoint[p_wptr->IDnumber][1].X -= p_wptr->WidgetWidth;
					DataPoint[p_wptr->IDnumber][2].X -= p_wptr->WidgetWidth;
					ControlPoint[2].X -= (p_wptr->WidgetWidth << 4);
				}

				/********* Data points are sorted from small to large ************/
				for(uint32_t i = 0;i < pointnumber; i++)
				{
					for(uint32_t j = i + 1;j < pointnumber; j++)
					{
						if(DataBuffer[i].X > DataBuffer[j].X)
						{
							Point TempPoint = DataBuffer[j];
							DataBuffer[j] = DataBuffer[i];
							DataBuffer[i] = TempPoint;
						}
					}
				}

				if(pointnumber != 0)
				{
					DataBuffer[pointnumber++] = ControlPoint[2];
				}

				//Rounding Handle
				HandleAllPoint();

				/********* X-axis overflow treatment ************/
				for(uint32_t i = 0;i < pointnumber; i++)
				{
					//widget height supplement length (64bit align)
					heighttemp = p_wptr->WidgetHeight;
					if( (heighttemp & 0x3f) != 0)
					{
						heighttemp = ((heighttemp >> 6) << 6) + 64;
					}

					/********* X-axis overflow treatment ************/
					if(DataBuffer[i].X >= p_wptr->WidgetWidth)
					{
						//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
						if(p_wptr->YPADDING == 2)
						{
							ChangeModeFlag[p_wptr->IDnumber] = 1;
						}
						DataBuffer[i].X -= p_wptr->WidgetWidth;
					}
					updataOscilloscopeTexture_alpha1(p_wptr->IDnumber, DataBuffer[i].Y, DataBuffer[i].X, heighttemp, p_wptr->WidgetWidth, showMode,linewidth);
					datanumber[p_wptr->IDnumber] = DataBuffer[i].X + linewidth;
				}

				pointnumber = 0;
			}
		}
	}

	p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//**************************************
funcStatus ChartClass::MouseTouchCtrl(
    DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	PIDPoint* pPressPoint,
	PIDPoint* pReleasePoint,
	PIDPoint* pAbsolutePressPoint,
	PIDPoint* pAbsoluteReleasePoint,
	u8 u8_widgetRefresh       
    )
{
	#ifndef CHIP_GC9002_NoHyper
	s32 XMinValue, XMaxValue, YMinValue, YMaxValue, XtotalValue, YtotalValue;
	s32 tag_value = 0;
	s32 cur_x;
	s32 cur_y;

	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	//yPadding means display mode: 0->loop, 1->left advance, 2->right advance, 3->Dot and line
	//if(p_wptr->YPADDING != 3)
	{
		return WIDGET_TOUCH_CANCEL;
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
		ERROR_PRINT("ERROR: when drawing touch track widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

    if( ActionPtr->mInputType == ACTION_MOUSE_PRESS )
	{
		p_wptr->PressFlag = 1;

		cur_x  = ((s16)(pPressPoint->x));
		cur_y  = ((s16)(pPressPoint->y));
		XMinValue = p_wptr->XMinValueL | (p_wptr->XMinValueH << 16);
		XMaxValue = p_wptr->XMaxValueL | (p_wptr->XMaxValueH << 16);
		YMinValue = p_wptr->MinValueL | (p_wptr->MinValueH << 16);
		YMaxValue = p_wptr->MaxValueL | (p_wptr->MaxValueH << 16);
		XtotalValue = XMaxValue - XMinValue;
		YtotalValue = YMaxValue - YMinValue;

		if (cur_x<0)
		{
			cur_x = 0;
		}else if (cur_x>p_wptr->WidgetWidth)
		{
			cur_x = p_wptr->WidgetWidth;
		}

		if (cur_y<0)
		{
			cur_y = 0;
		}else if (cur_y>p_wptr->WidgetHeight)
		{
			cur_y = p_wptr->WidgetHeight;
		}

		cur_x = cur_x * XtotalValue / p_wptr->WidgetWidth + XMinValue;
		cur_y = p_wptr->WidgetHeight - cur_y;
		cur_y = cur_y * YtotalValue / p_wptr->WidgetHeight + YMinValue;

		if(cur_x >= 0 && cur_y >= 0)
		{
			tag_value = ((cur_y) + ((cur_x) << 16)) ; //caculate value
		}
		else if(cur_x < 0 && cur_y >= 0)
		{
			tag_value = ((cur_y) | ((cur_x) << 16)) ; //caculate value
		}
		else if(cur_x >= 0 && cur_y < 0)
		{
			tag_value = ((cur_y & 0x0000FFFF) | ((cur_x) << 16)) ; //caculate value
		}
		else if(cur_x < 0 && cur_y < 0)
		{
			tag_value = ((cur_y & 0x0000FFFF) | ((cur_x) << 16)) ; //caculate value
		}

		TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);

		//p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	}
	else if(ActionPtr->mInputType == ACTION_MOUSE_HOLDING)
	{
		if(p_wptr->PressFlag)
		{
			cur_x  = ((s16)(pReleasePoint->x));
			cur_y  = ((s16)(pReleasePoint->y));
			XMinValue = p_wptr->XMinValueL | (p_wptr->XMinValueH << 16);
			XMaxValue = p_wptr->XMaxValueL | (p_wptr->XMaxValueH << 16);
			YMinValue = p_wptr->MinValueL | (p_wptr->MinValueH << 16);
			YMaxValue = p_wptr->MaxValueL | (p_wptr->MaxValueH << 16);
			XtotalValue = XMaxValue - XMinValue;
			YtotalValue = YMaxValue - YMinValue;

			if (cur_x<0)
			{
				cur_x = 0;
			}else if (cur_x>p_wptr->WidgetWidth)
			{
				cur_x = p_wptr->WidgetWidth;
			}

			if (cur_y<0)
			{
				cur_y = 0;
			}else if (cur_y>p_wptr->WidgetHeight)
			{
				cur_y = p_wptr->WidgetHeight;
			}

			cur_x = cur_x * XtotalValue / p_wptr->WidgetWidth + XMinValue;
			cur_y = p_wptr->WidgetHeight - cur_y;
			cur_y = cur_y * YtotalValue / p_wptr->WidgetHeight + YMinValue;

			if(cur_x >= 0 && cur_y >= 0)
			{
				tag_value = ((cur_y) + ((cur_x) << 16)) ; //caculate value
			}
			else if(cur_x < 0 && cur_y >= 0)
			{
				tag_value = ((cur_y) | ((cur_x) << 16)) ; //caculate value
			}
			else if(cur_x >= 0 && cur_y < 0)
			{
				tag_value = ((cur_y & 0x0000FFFF) | ((cur_x) << 16)) ; //caculate value
			}
			else if(cur_x < 0 && cur_y < 0)
			{
				tag_value = ((cur_y & 0x0000FFFF) | ((cur_x) << 16)) ; //caculate value
			}

			TagPtr[p_wptr->BindTagID].setRawValue(tag_value);

			//p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
			p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
		}
	}
    else if(ActionPtr->mInputType == ACTION_MOUSE_RELEASE)
	{
		p_wptr->PressFlag = 0;

		TagPtr[p_wptr->BindTagID].setRawValue(0x7FFF7FFF);
	}
    if(p_wptr->TouchPropagation & TouchPropagationSet)
	{   
        /* Touch propagation. */
        return WIDGET_TOUCH_CANCEL;
	}
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
    /* Don,t propagation. */
    return AHMI_FUNC_SUCCESS;
}

#endif
