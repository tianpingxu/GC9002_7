////////////////////////////////////////////////////////////////////////////////
// Company:      Graphichia Inc
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     drawImmediately_cd.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef DYNAMICPAGECFGCLASS__H
#define DYNAMICPAGECFGCLASS__H
#include "publicType.h"
#include "publicDefine.h"
#include "Easing.h"
#ifdef AHMI_CORE

typedef struct _ProjectScreenOptions
{
	u16 mainWidth;
	u16 mainHeight;
	u16 subWidth;
	u16 subHeight;
	u8 enable;
}ProjectScreenOptions;

typedef struct _PageScreenOptions
{
	s16 mainOffsetX;
	s16 mainOffsetY;
	s16 subOffsetX;
	s16 subOffsetY;
}PageScreenOptions;

/**
*  the class that is used in generator to pass the data of page
*/
class DynamicPageCfgClass
{
public:
	u32     StoreOffset            ;//the offset of page stored in flash
	u32		TotalSize			   ;//total size of page
	u32		CheckCode			   ;//check code of page
	u16     PageAttr               ;//attribute of the page
	u16     totalNumOfTexture	   ;//total number of texture
	u16     ActionInstructionsSize ;//total streangth of action
	u16     NumOfPageAction        ;//the number of action of page
	u16		NumOfCanvasAction	   ;//the number of action of canvas
	u16     NumOfWidget            ;//the number of widget
	u16     NumofTouchWidget       ;//the number of touchable widget
	u16     NumofGifWidget         ;//the number of gif widget
	u16		pageAnimationDuration  ;//the duration time of page animation
	u16		pageReleasingAnimationDuration  ;//the duration time of page animation
	u16		NumOfCanvas			   ;//the number of canvas
	u16     NumOfSubCanvas         ;//the number of subcanvas
	u8      AnimationType          ;//the type of animation
	u8      enlightedWidgetID      ;//the first highlight widget
	u8		mStartOfStack          ;//the start address of the stack for page ID
	u8		mNumOfPara             ;//the number of parameters one page need to pass
	u8		timingFun			   ;//easing
	u16     mNumOfAnimationKeyFrames;//the number of animation key frames
	u8      mNunOfAnimationActions ;//the number of user action adopted by usr
	u8      VideoType              ;//video interface type: 0 for no video, 1 for CVBS(bt656), 2 for DVP, 3 for AHD, 4 for RGB
	PageScreenOptions screenAreas;//page screen areas
	//??
#ifndef PC_SIM_Gen
	DynamicPageCfgClass(void);
	~DynamicPageCfgClass(void);
#endif
};

/**
*  the class that is used in generator to pass the data of AHMI information
*/
class  AHMIInfoClass
{
public:
	u32 Version;
	u16 ScreenWidth;
	u16 ScreenHeight;
	u16 ColorSpace;
	u16 TileSize;
};

/**
*  the class that is used in generator to pass the data of whole system
*/
class  ConfigInfoClass
{
public:
	u8  md5_value[32]; // md5 value to check
	u32 MagicNumber;//0x494d4841
	u32 TotalSize;//total size of configuration file
	u32 TotalTextureSize;//total size of texture, count in byte
	u32 DynamicPageSize; //total size of page, count in byte
	u16 NumofTimers;// total number of timers
	u16 NumofUartBuf;//total buffer size for uart
	u16 NumofWidget;//num of widget
	u16 NumOfWaveFilter;//num of wavefilters
	u16 NumOfWaveFilterStatus; //num of wavefilterstatus
	u16 NumofTags;//the number of tags
	u16 NumofTagUpdateQueue;//the number of tag that needs to be update
	u16 TagValueTotalSize;
	u8  NumofSound;           //the number of sound
	
	u16 NumofSysPage;//the number of system pages
	u16 StringSize;   //the size for all string
	u16 PageSwipeMode;//Page Swipe Mode: [0] loop(1 for loop; 0 for not loop); [1] swipe switch page(1 for switch multi page; 0 for switch single page);
	
	u16 CfgStringSize; //the total size of configuration string
	u16  NumofPages;    //the number of pages
	//u8  InterfaceType;//the service type of MODBUS,0 AHMI self type,1 Modbus Master��?2 Modbus Slave?��
	
	//u8  innerClockEn;   //inner clock enable
	s16 screenratio;    //the w:h of a pixel
	u16 encoding; //global encoding of project
	u32 audioSampleRate; //global audio sample rate
	u8 displayDirection;//display direction
	u16 serialBaudRate;//serial baud rate
	u16 NumofStrings;         // the number of strings added by SET_STR  CONCAT
	u16 NumOfHints; //the number of dict hints for chinese
	u8  YTHVersion[32]; //version of YTH
	u8  IDEVersion[32]; //version of IDE
	u8  CPU0Version[32]; //version of CPU0
	u8  CPU1Version[32]; //version of CPU1
	u8  DeviceID[32]; //ID of device
	u16 NumofArrayDatas;     // the number of array datas added by SET_ARR
	u16 NumOfFontRanges;    //the number of font ranges
	u8  NumOfKBStates; // num of kbStates for kbcontainer
	u8  NumOfMenuStates; // num of MenuStates for menucontainer
	AHMIInfoClass AHMIInfo; //information of AHMI
	ProjectScreenOptions screenOptions; //info of two screens
};

//Page Swipe Mode
#define PageSwipeMode_Normal        	(0x0)
#define PageSwipeMode_Loop          	(0x1)
#define PageSwipeMode_SwitchMultiPage	(0x2)

#endif

#endif

