////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    definition used only by widget
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef WIDGETCLASS_CD__H
#define WIDGETCLASS_CD__H
#include "publicDefine.h"
#ifdef AHMI_CORE

//   ----------------------->x
//   |  offset  height
//   |     .-----------. 
//   |     |           | width
//   |     .___________.
//   y

#define WIDGET_GENERAL_NUM (44) //general number of widget attribute
#define WIDGET_UNVAR (WIDGET_GENERAL_NUM + 25)  //vriable attribute number

#define WidgetAttr							WDG_u16_userVariables[(u32)0]  //attribute,@see every widget
#define NumOfTex							WDG_u16_userVariables[1]      //total number of texture
#define StartNumofTex						WDG_u16_userVariables[2]  //the first pointer of texture in texture array
#define WidgetWidth							WDG_u16_userVariables[3]  //widget width
#define WidgetHeight						WDG_u16_userVariables[4]  //widget height
#define WidgetOffsetX						WDG_u16_userVariables[5] //left top point x of widget
#define WidgetOffsetY						WDG_u16_userVariables[6] //left top poiint y of widget
#define BindTagID							WDG_u16_userVariables[7] //the ID of tag bound to

//the start address of trigger action
#define EnterHighAlarmAction				WDG_u16_userVariables[8]
#define EnterLowAlarmAction					WDG_u16_userVariables[9]
#define LeaveHighAlarmAction				WDG_u16_userVariables[10]
#define LeaveLowAlarmAction					WDG_u16_userVariables[11]
#define TagChangeAction					    WDG_u16_userVariables[12]   //add by Mr.z to realize tag change
#define OnRealeaseAction					WDG_u16_userVariables[13]	
#define OnLongPressAction					WDG_u16_userVariables[14]   //long press action
#define OnPressAction						WDG_u16_userVariables[15]   //press action

//the attribute for animation
//#define TOTALFRAME_AND_NOWFRAME             WDG_u16_userVariables[14]   //[7:0] now frame [15:8] total frame. if total frame == 0, indicates that the widget contains no animaiton
#define NOW_DURATION						WDG_u16_userVariables[16]  //now duration
#define START_TAG_L                         WDG_u16_userVariables[17]   //start value, used for animation
#define START_TAG_H                         WDG_u16_userVariables[18]	//start value, used for animation
#define STOP_TAG_L                          WDG_u16_userVariables[19]   //stop value, used for animation
#define STOP_TAG_H                          WDG_u16_userVariables[20]   //stop value, used for animation
#define ATTATCH_CANVAS						WDG_u16_userVariables[21]   //the canvas that the widget attatch to
#define ANIMATION_DURATION					WDG_u16_userVariables[22]   //u16 animation duration
#define REFRESH_FLAG						WDG_u16_userVariables[23]   //u16 widget refresh flag
#define INITIALIZED						    WDG_u16_userVariables[24]   //u16 widget whether widget initialized

#define MatrixZoomX                         WDG_u16_userVariables[25]
#define MatrixZoomY                         WDG_u16_userVariables[26]
#define MatrixRotateCenterX                 WDG_u16_userVariables[27]
#define MatrixRotateCenterY                 WDG_u16_userVariables[28]

#define MatrixRotateAngle                   WDG_u16_userVariables[29]
#define MatrixProjectionX                   WDG_u16_userVariables[30]
#define MatrixProjectionY                   WDG_u16_userVariables[31]

#define TouchPropagation					WDG_u16_userVariables[32]
#define TouchPressPosX						WDG_u16_userVariables[33]
#define TouchPressPosY						WDG_u16_userVariables[34]

#define SourceBufferStartPos				WDG_u16_userVariables[35]	//[15] last texture long or short (1 for short; 0 for long) [14:0] widget sourcebuffer start position.
#define SourceBufferLength					WDG_u16_userVariables[36]	//[15:12] Does the 0 to 3 quadrant exist for FBBatchDraw  [12:0] widget sourcebuffer start position.

#define WidgetType							WDG_u16_userVariables[37]
#define ATTATCH_PAGE						WDG_u16_userVariables[38]   //the page that the widget attatch to

#define ADAPTIVE_SCALE_LEVEL				WDG_u16_userVariables[39]

#define MatrixScaleCenterX                  WDG_u16_userVariables[40]
#define MatrixScaleCenterY                  WDG_u16_userVariables[41]

//new action triggers 
#define OnClickReleaseAction				WDG_u16_userVariables[42]   //click and then release action
#define OnSwipeOutAction				    WDG_u16_userVariables[43]   //swipe out action

#define TouchDisable						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 24]

//for swipe responder
#define SwipeLeftAction						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 0]
#define SwipeRightAction					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1]
#define SwipeTopAction						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 2]
#define SwipeBottomAction					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 3]

#define SwipeDirection                      WDG_u16_userVariables[WIDGET_GENERAL_NUM + 4]
#define SwipeDistanceH                      WDG_u16_userVariables[WIDGET_GENERAL_NUM + 5]
#define SwipeDistanceV                      WDG_u16_userVariables[WIDGET_GENERAL_NUM + 6]

#define PressedPosX                         WDG_u16_userVariables[WIDGET_GENERAL_NUM + 7]  
#define PressedPosY                         WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]	

#define LastPressedPosX                     WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]  
#define LastPressedPosY                     WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]
#define WRActionTriggerFlag                 WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]	   //not used, but same as PressFlag, can't be used for other feature
#define SwipeTickH                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]
#define SwipeTickL                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]
#define SwipeStatus                         WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]     //0:pressed 1: dragging 2:keyboard ok press 3:keybaord ok long press


//the value of low/high alarm
#define LowAlarmValueL						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 0] 
#define LowAlarmValueH						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1] 
#define HighAlarmValueL						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 2]
#define HighAlarmValueH						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 3]
//the value of most/least
#define MinValueL							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 4] 
#define MinValueH							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 5]
#define MaxValueL							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 6]  
#define MaxValueH							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 7]

//used for switch
#define FLASHMODE							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8] 
#define FLASHDUR							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] 
//onOffTex
#define NumOfSlice							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]        //1 only on tex, 2 both off and on tex

//barchart
#define BarchartCount						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]
#define BarchartPadding						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]
#define BarchartYMin						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]
#define BarchartYBase						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]
#define BarchartYMax						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]
#define BarchartColor						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]
#define BarchartBarWidth					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]
#define BarchartBarHeight					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]

//piechart
#define PiechartCount						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]
#define PiechartOffset						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]

//gif
#define GifNowTime							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]
#define GifInterval							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]
#define GifNowIdx							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]
#define GifSliceNum							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]

//list
#define ListHighlightElemID                 WDG_u16_userVariables[WIDGET_GENERAL_NUM + 6]
#define ListHighlightOptions			    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 7]  //0: isEditing
#define ListShowNum							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]
#define ListItemCount						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]
#define ListSelectedTagID					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]
#define ListOffset							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 18]
#define ListArrange							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]
#define ListPadding                         WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]
#define ListStatus                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]     //0:pressed bit 1: dragging 2:keyboard ok press 3:keybaord ok long press
#define ListPressOffset                     WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]
#define ListTickH                           WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]
#define ListTickL                           WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]

//video player
#define VideoPlayerAutoPlay                 WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]
#define VideoPlayerLoopPlay                 WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]
#define VideoH264TextureSizeH               WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]
#define VideoH264TextureSizeL               WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]
#define VideoSoundTextureSizeH              WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]
#define VideoSoundTextureSizeL              WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]
#define VideoMaxFrameSizeH                  WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]
#define VideoMaxFrameSizeL                  WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]
#define VideoH264MaxPicBufWidth             WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]
#define VideoH264MaxPicBufHeight            WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]

//CombinationSwitchTexture
#define CS_FLASHMODEL						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8] 
#define CS_FLASHMODEH						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] 
#define CS_FLASHDUR							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10] 
#define AlignMode							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]        //the aign mode of Combination Switch 

//only used for bar
//the start and end color
#define StartColorValueGB					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8] 
#define EndColorValueBAndStartColorValueR	WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] 
#define EndColorValueRG						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]

//multi color bar 
// the value of three color that needs to be changed 
#define COLOR0GB                            WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]
#define COLOR1B_COLOR0R                     WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]
#define COLOR1RG							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]
#define COLOR2GB							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]
#define COLOR2R                             WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]
//the value of threshold to change color
//#define THRESHOLD0_L                        WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]
//#define THRESHOLD0_H                        WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]
//#define THRESHOLD1_L                        WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]
//#define THRESHOLD1_H                        WDG_u16_userVariables[WIDGET_GENERAL_NUM + 18]
#define THRESHOLD0							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]
#define THRESHOLD1							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 18]

//only used for actionbar
#define GlobalTriggerFlag					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 19]       //Global Trigger Flag: 0 for disable; 1 for enable

//used for button
#define OnEnteredAction                     WDG_u16_userVariables[13]                         //the keyboard entered button
#define KeyboardPreAndNextWidget            WDG_u16_userVariables[WIDGET_GENERAL_NUM + 0]     //{previous highlight��next highlight}
#define NumOfButtonAndCurHighLight          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1]     //{total number of button��current highlight ID of button}
#define ReleaseColorValueGB					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 2]     //press and release color value
#define PressValueBAndReleaseColorValueR	WDG_u16_userVariables[WIDGET_GENERAL_NUM + 3]     //press and release color value
#define PressColorValueRG					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 4]     //press and release color value
#define HoldModeONorOFF						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 5]     //hold mode on or off
#define HoldModeDuration					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 6]     //when hold mode on set duration for animation
#define OldTickForHoldL					    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 7]     //tick 
#define OldTickForHoldH					    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]     //tick
#define LongPressFlag                       WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]
#define HoldFirstFlag                       WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]
#define PressFlag                           WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]

//used for meter
#define MeterMinAngle						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]    //the min value
#define MeterMaxAngle						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] 	 //the max value
#define StartAngle							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]    //starting angle

//used for meter and circleSlider
#define RotationPointX						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 20]    //Rotation point x coordinates
#define RotationPointY						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 21]    //Rotation point y coordinates

//used for circleSlider
#define CircleSliderMinAngle				WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]    //the min value
#define CircleSliderMaxAngle				WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] 	 //the max value
#define SliderDeviation						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 22] 	 //sliderDeviation
#define TravelPrincipleMode					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 23] 	 //travelPrincipleMode: original return; Proximity mode
#define CircleSliderStartAngle				WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]    //starting angle

//scroll options
#define ScrollOptions					    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1]   //0-1:direction, 2: autoreverse 3:mode, 4-5:status
//scroll delay & duration
#define ScrollDelay					        WDG_u16_userVariables[WIDGET_GENERAL_NUM + 2]
#define ScrollDuration				        WDG_u16_userVariables[WIDGET_GENERAL_NUM + 3]
     
//rotation texture
#define DynamicTexMinAngle					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]   //min angle
#define DynamicTexMaxAngle					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] 	//max angle
                                            
//OSC, not used now      
#define CurPosI								WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8] //the ID of pointer
#define OSCColor							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] //color of line,R5G6B5
#define BLANK_X                             WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10] //X blank               
#define BLANK_Y                             WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11] //Y blank
	

	
//video not used now
#define FrameSpeed							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 0] //how many ms to display one frame
#define NowTime								WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1] //now is how many ms

//dynamic texture
#define TextureSource						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1] //0 for normal; 1 for copy&paste action

// only for rotatingImage
#define RotateInitValueL					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8] //low 16 value of the init value for rotatingImage
#define RotateInitValueH					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9] //high 16 value of the init value for rotatingImage

//used for multi-line number for animation
#define NumOfLine							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]        //8:11 the align method of number. @see RIGHTALIGN/CENTERALIGN/LEFTALIGN
                                                                                                 //12:15 the method when number exceeds the maxium OVERFLOW_DISPLAY/OVERFLOW_NOT_DISPLAY
                                                                                                 //[7:0] the animation type of number ��0 is none��1 is switch
#define PreviousTexturePtrFlag				WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]        //0 moving down, 1 moving up
#define CurValueL							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]        //the value of center line
#define CurValueH							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]        //the value of center line

//textureNumber waitingValue and enableWaiting
#define TextureNumberWaitingValue			WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]		//0:enableWaiting 1:31 waitingValue
#define TextureNumberWaitingValueH			WDG_u16_userVariables[34]							//waitingValue high 16bit

//progbar,dynamictex,meter,number,OSC
#define OldValueL							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]       //low 16 value of the previous value
#define OldValueH							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]       //high 16 value of the previous value
#define SPACINGX							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]       //the space of number, SpacingX                                                
#define ANIMATIONCURVALUE_L					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]       //current value of animation by Mr.z
#define ANIMATIONCURVALUE_H					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]       //current value of animation by Mr.z
#define HexControl                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]       // [3:0]whether to display in hex [7:4] whether to display 0x [11:8] thether to display up case
#define DOTOPTIONS                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 18]       //0:dotLayout
#define TimingFun							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 19]       //timingFun type
#define NumberWaitingValue					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 20]		 //0:enableWaiting 1:31 waitingValue
#define NumberWaitingValueH					WDG_u16_userVariables[34]							//waitingValue high 16bit
#define UnitStringLength					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 21]		 //the length of unit string
#define UnitStringL							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 22]		 //the string of unit low 16bit
#define UnitStringH							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 23]		 //the string of unit high 16bit
#define WaitStringLength						WDG_u16_userVariables[13]	//the length of wait string
#define WaitStringL							WDG_u16_userVariables[14]   //the string of wait low 16bit
#define WaitStringH							WDG_u16_userVariables[15]   //the string of wait high 16bit

//data image
#define DATAIMG_MODE					    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]       //mode
#define DATAIMG_REFRESH_MODE			    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]       //refresh mode
#define DATAIMG_DATA_ALIGN				    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]       //data align
#define DATAIMG_LINE_IDX                    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]       //line idx
#define DATAIMG_ID                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]       //id
    

//textInput
#define TextInputFontWidth					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]       //FontWidth no padding
#define TextInputFontHeight					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]       //FontHeight no padding
#define TextInputLineSpacing				WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]       //multiline line spacing
#define TextInputFontRangeID                WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]       //font range id

//not used now
#define IOX									WDG_u16_userVariables[WIDGET_GENERAL_NUM + 0]      //the picked IO 
#define PinMusk								WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1]      //valid Pin
#define OpenPin								WDG_u16_userVariables[WIDGET_GENERAL_NUM + 2]      //opened Pin

//chart
#define XCOUNT								WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]      //opened Pin
#define YCOUNT								WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]      //opened Pin
#define XPADDING							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]      //opened Pin
#define YPADDING							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]      //opened Pin
#define SHOWMODE							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]      //opened Pin
#define XMinValueL							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]
#define XMinValueH							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]
#define XMaxValueL							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 18]
#define XMaxValueH							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 19]
//for chart & QRCode
#define IDnumber							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]      //opened Pin

//for kbcontainer
#define SHOWPASSWD							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]      //show passwd
#define SHOWOPTIONS                         WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]      //show options 0 bit:show parsswd, 1：2 bit:mode

#define CHAR_LIMIT						    WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]      //kb point state, -1: no point, other: num after point

#define KBSTATE_SHIFT					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]      //kb num shift state

#define KBSTATE_CURVALUE_L					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 11]      //kb num state
#define KBSTATE_CURVALUE_H					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 12]      //kb num state

#define KBSTATEID                           WDG_u16_userVariables[WIDGET_GENERAL_NUM + 13]      //kb state id

#define KBINTCOUNT                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]      //kb integerCount
#define KBDECCOUNT                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 15]      //kb decimalCount
#define KBHINTTAGID                          WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]      //kb hintTagID

//selector
#define SELECTOROldTickForSpeedH				WDG_u16_userVariables[10]
#define SELECTOROldTickForSpeedL				WDG_u16_userVariables[11]
#define STATIC_ANIMATION_DURATION				WDG_u16_userVariables[15]
#define SwipeChangeDistance					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 0] 
#define SwipeFactor							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 1] 
#define SwipeMultiSpeed						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 2]
#define QuickSwipeSpeed						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 3]
#define SELECTORCURSPEED						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 14]
#define SELECTORCOUNT						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 4]      //selector count
#define SELECTORSHOWCOUNT					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 5]      //selector show count
#define SELECTOROFFSET                      WDG_u16_userVariables[WIDGET_GENERAL_NUM + 6]      //offset of selector item when moving
#define SELECTORPRESSOFFSET                 WDG_u16_userVariables[WIDGET_GENERAL_NUM + 7]      //offset when press selector
#define SELECTORSTATUS                      WDG_u16_userVariables[WIDGET_GENERAL_NUM + 8]      //selector status 0:pressed 1:animating 2:animate type, 3: dragging 4:isMultiSpeed
#define SELECTORALPHA                       WDG_u16_userVariables[WIDGET_GENERAL_NUM + 9]      //selector original unchosen alpha
#define SELECTOROPTIONS                     WDG_u16_userVariables[WIDGET_GENERAL_NUM + 10]     //selector options: 0:auotoHide, 1:cycle, 2:range, 3:focusflag, 4:selectorMode
#define INDEXOFSELECTEDINSHOW               WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]     //selector indexOfSelectedInShow:index of the selected item in showing items 
#define INDEXOFTOP							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 18]     //selector indexOfTop:index of the top/leftmost item
#define D_INDEXOFTOP						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 20]     //selector d_indexOfTop:distance of two positions when inputting a different tag_value
#define SELECTORSCALE						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 21]     //0:7 selected bg scale
#define SELECTORTickH                       WDG_u16_userVariables[WIDGET_GENERAL_NUM + 22]
#define SELECTORTickL                       WDG_u16_userVariables[WIDGET_GENERAL_NUM + 23]

//MenuItem
#define MENUSTATEID                         WDG_u16_userVariables[WIDGET_GENERAL_NUM + 16]     //menu state id
#define MENUMODE							WDG_u16_userVariables[WIDGET_GENERAL_NUM + 17]     //menu mode
#define MENUITEMVALUE						WDG_u16_userVariables[WIDGET_GENERAL_NUM + 18]     //menu item value
#define MENUITEMSTRINGID					WDG_u16_userVariables[WIDGET_GENERAL_NUM + 19]     //menu item string id
//type of widget

#define   DYNAMIC_TEX			1                 //@see dynamicTextureClass
#define   BUTTON_TEX			2                 //buton, @see ButtonClass
#define   METER_TEX				3                 //meter, @see MeterClass
#define   PROGBAR_TEX			4                 //progbar, @see ProgBarClass
#define   SLIDER_TEX			5                 //slider, @see SliderBarClass
#define   OSCSCP_TEX			6                 //OSC, not used now
#define   TEXTBOX_TEX			7                 //text, not used now
#define   NUMBOX_TEX			8                 //number box, @see NumberClass
#define   ANIME_TEX				9                 //change the texture for animation, not used now
#define   VIDEO_TEX				10                 //video/cell phone protection, @see VideoClass
#define   KNOB_TEX				11                 //knob, not used now
#define   CLOCK_TEX				12                 //clock, @see ClockClass
#define   ON_OFF_TEX			13                 //on off, @see OnOffClass
#define   TEXTURE_NUM			14                 //textureNumber, @see TextureNumberClass
#define   TEXTURE_TIME			15                 //texture timer, @see TextureTimerClass
#define	  TOUCH_TRACK			16					//touch track
#define	  ALPHA_SLIDE			17					//alpha slide
#define	  TEXT_INPUT			18					//text input
#define	  ALPHA_IMG				19					//alpha img
#define	  BUTTON_SWITCH			20					//button switch
#define   GALLERY				21                  //gallery
#define   CLOCKBOARD			22                  //clockboard
#define	  CHART					23					//chart
#define	  QRCode				24					//QR code
#define	  SELECTOR				25					//Selector
#define   KBCONTAINER			26                  //KeyboardContainer
#define   SWIPERESPONDER		27                 //swipe responder
#define   CombinationSwitch		28                 //CombinationSwitch
#define   MENUCONTAINER 		29                 //MenuContainer
#define   CIRCLESLIDER_TEX 		30                 //CircleSlider
#define   DATAIMAGE             31                 //DataImage
#define   BARCHART              32                 //BarChart
#define   PIECHART              33                 //PieChart
#define   GIF                   34				   //Gif
#define   LIST                  35                 //List
#define   VIDEO_PLAYER_TEX      36                 //Video Player
#define   THREE_DIMENSIONAL     37                 //3D Model
#define   SX_WIDGETTYPE_END     38                 //end of standard widget type list
#define   CUSTOMWIDGET          0x8000			//custom widget

#define  WidgetValueRefreshFlag			(0x01)
#define  WidgetMatrixRefreshFlag		(0x02)
#define  WidgetSourceBufferRefreshFlag	(0x04)
#define  WidgetBindCanvasRefreshFlag	(0x08)

//the marco for texture
//dynamicTex
#define  DYNAMIC_TYPE_BIT 0x00E0 //the bit for dynamic texture

//button
#define  SINGLEBUTTON (0) //single button. only one button 
#define  SWITCHBUTTON (1) //switch. one button. once press, it will always be pressed until next press.
#define  COMPLEXBUTTON (2) //complex button
#define  SINGLE_COLOR_BUTTON_BIT (0x800) //whether is color button, 11 bit of widget

//clock
#define CLOCK_HOUR_MINUTE_SECOND_MODE (0) //the widget displays hour:minute:second
#define CLOCK_HOUR_MINUTE_MODE        (1) //the widget displays hour:minute
#define CLOCK_GANG_YEAR_MODE          (2) //the widget displays year/month/day
#define CLOCK_MINUS_YEAR_MODE         (3) //the widget displays year-month-day
#define CLOCK_USA_YEAR_MODE           (4) //the widget displays month-day-year
#define CLOCK_EUROPE_YEAR_MODE        (5) //the widget displays day-month-year
//rotating dynamic texture
#define ROTATING_DYNAMIC_TEX 0x20  //the type of rotating
#define DIM_TEX              0x60  //the type of dim, not used now



//Progbar
#define  CHANGECOLOR (0x100) //change color
#define  CHANGECOLOR_MODE (0x200) //0 is gradually change��1 is sudden change
#define  BAR_IS_COLOR   (0x80)  //the bar is color
#define	 LINE_FILE_EN (0x40)  //the bar contains line file
#define  LINE_CHANGE_COLOR (0x0400) //the bar changes color
#define  PROBARDISPLAY		(0x0800)//whether to display
#define  THRESHOLD_NUM      (0x1000) //the number of threshold��0 represts one��1 represents two
#define  PROGBAR_DIR        (0x6000)  //direction, 0 is towards the right, 1 is towards the up, 2 is towards the left, 3 is towards the down

//the aign mode of number 
#define		RIGHTALIGN	0 //right
#define     CENTERALIGN	1 //center
#define		LEFTALIGN	2 //left

//the vertical aign mode of number 
#define		TOPALIGN	0 //top
#define     MIDDLEALIGN	1 //middle
#define		BOTTOMALIGN	2 //bottom

//the aign mode of Combination Switch 
#define		CombinationSwitch_LEFTALIGN		0 //left
#define     CombinationSwitch_RIGHTALIGN	1 //right
#define		CombinationSwitch_UPALIGN		2 //up
#define		CombinationSwitch_DOWNALIGN		3 //down

//excution after exceeds 0:display the max value, 1: not display
#define    OVERFLOW_DISPLAY     0
#define    OVERFLOW_NON_DISPLAY 1


//the type of rotation
#define SWITCH_TEXTURE (0) //centeral roatation
#define CENTRAL_ROTATE (1) //centeral roatation
#define TRANSLATION    (2) //translation
#define DIM            (3) //dim

//meter
#define  METER_SIMPLIFY 0x80 //the simplify mode of meter

#define WIDGET_TOUCH_DIFF	(5)

//chart max number
#define Chart_Num_MAX	10

//QRCode max number
#define QRCode_Num_MAX	10

//DataImage max number
#define DataImage_Num_Max 10

#ifndef PC_SIM_Gen
//video
#define VIDEO_TYPE_MASK     0x60
#define VIDEO_TYPE_CVBS     0x00
#define VIDEO_TYPE_DVP      0x20
#define VIDEO_TYPE_AHD      0x40
#define VIDEO_TYPE_RGB      0x60
#endif

#define  NUMOFNUMBER 32/4//most number to display

//test
#define MAX_WIDTH_AND_HEIGHT 1920
#define MAX_ANGLE            90
#define MAX_ZOOM			 60*512*512 //1.6.9

#endif

#endif
