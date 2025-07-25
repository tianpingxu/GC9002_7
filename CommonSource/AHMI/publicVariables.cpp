////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     PublicVariables.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Additional Comments:
//    include all the global variables
// Revision:
// Revision 1.6.2 - Modify 2019/07/16 by YuChunYing
// Additional Comments:
//      ��·��Ƶ����ʹ�ñ���ֻ����һ·��ȥ��������·��
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "AHMIDriver.h"
#include "StringClass.h"
#include "semphr.h"
#include "DelayArg.h"
#include "SoundVector.h"
#include "SdlPlayer.h"
#include "drawimmediately_cd.h"

#ifdef AHMI_CORE

extern uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];

ConfigInfoClass ConfigData;    // the config information from generator
u16             WorkingPageID; // the ID of current page
DynamicPageClassPtr  gPagePtr; // pointer of page
TimerClassPtr   TimerPtr;      // pointer of timer
WaveFilter*		gWaveFilterPtr; //pointer of wave filters
WaveFilterStatus* gWaveFilterStatusPtr; //pointer of wave filter status
WaveFilterTickArg* gWaveFilterTickArgPtr;
DelayArg*		gDelayArgPtr;
TagClassPtr     TagPtr;        // pointer of tag
StringClassPtr  StringListPtr;     // pointer of stringList
//FontRangeClass* FontRangeListPtr; //pointers of font ranges
//EncodingRange* gRangesPtr;  //pointer of fontRange ranges

TagUpdateClassPtr  TagUpdatePtr; // pointer of tagUpdateClass
SoundClass *   gSoundPtr;
KBState*       gKBState;      //pointer of kbstates
Hinter         gHinter;	      //pointer of hinter
HintClass*     gHintClass;    //pointer of hintClass
MenuState*     gMenuState;    //pointer of menustates
//interval
IntervalManager gIntervalManager; //global interval manager
//animation
AnimationClass  gAnimationClass;  //global parameter of animation class

//easing
Easing	gEasing; // global easing class
ArrayDataClassPtr ArrayDataListPtr; //pointer of ArrayData List
u8*               gArrayDataPtr; //start pointer of ArrayData data

u8*					gStringPtr;			// start pointer of strings
StringClassPtr		gStringTagStringPtr; // pointer of stringtag strings
u8*                 gTagValueSpacePtr;
EncodingClass*		gEncoder;//encoding strings		

MyPlayer            gSDLplayer;
u8*					TagStringPtr;      // pointer of string tag
u8*					TagStringListPtr;  // pointer of string tag list
u8*                 TagChangedListPtr; // record what tags have been changed, storing the tagID

u32*				gBackgroundTextureAddrPtr;// start pointer of Background Texture address arry

#ifdef PC_SIM
fstream logFile; //log file
#endif

#ifdef PC_SIM
extern "C" extern k_msgq_handle_t   MouseQueue; //the queue of mouse touch
#endif
k_msgq_handle_t   keyboardQueue;      // keyboard queue
k_msgq_handle_t   RefreshQueue;       // queue to receive refresh data
k_msgq_handle_t   RefreshQueueWithoutDoubleBuffer; // queue to receive immediate refresh data
k_msgq_handle_t   ActionInstructionQueue;  // queue to receive antion instructions
k_msgq_handle_t   ModbusMasterRWTaskQueue; // queue to receive modbus command
k_msgq_handle_t   AnimationTaskQueue; ///////////////////animation task queue, stores the animation data

//all elements' information is loaded by dynamic page. The storage address is specified by PageSpace pointer
// when page changes, the PageSpace needs to be reloaded. 
// then all imformation of canvas/subcanvas/widget/texture will also be reloaded.
void*           PageSpace; 

s32             windowPressX;
s32             windowPressY;
s32             windowHoldX;
s32             windowHoldY;
s32             windowSpeedX;
s32             windowSpeedY;
u8              windowPressed = 0;

//handler of corresponding task
k_task_handle_t ModBusMasterRWTaskHandle;  
k_task_handle_t ModBusSlaveTaskHandle;
k_task_handle_t	ModBusMasterTaskHandle;
k_task_handle_t TouchTaskHandle;
k_task_handle_t InterruptTaskHandle;
k_task_handle_t CalibrationTaskHande;
#if defined(CHIP_GC9005) || defined(PC_SIM)
k_task_handle_t H264DecodeTaskHandle;
#endif
k_task_handle_t	ActionProcessorTaskHandle;
k_task_handle_t TimerProcessTaskHandle;
k_task_handle_t AnimationTimerProcessTaskHandle;
k_task_handle_t AnimationTaskHandle;
k_task_handle_t Cyclic10msTaskHandle;
#if defined(EMBEDDED) && defined(VERSION_2)
k_task_handle_t SetValueTaskHandle;
#endif

#if (defined WHOLE_TRIBLE_BUFFER) || (defined PARTIAL_TRIBLE_BUFFER)
k_task_handle_t screenUpdateTaskHandle;
#endif

//interrupt
//k_sem_handle_t interuptSemaphore_cvbs;
#if defined(CHIP_GC9005) || defined(PC_SIM)
k_sem_handle_t startPlayH264Semaphore;
#endif
//k_sem_handle_t interuptSemaphore_animation; //interrupt for animation done
//k_sem_handle_t interuptSemaphore_pagerefresh;   //interrupt for refresh page
//k_sem_handle_t interuptSemaphore_touch; 	   //interrupt for touch

u8 animationDuration = 0;  //during animation
//u8 gPageNeedRefresh;       //after the page changes, all widgets need to set the value

#ifdef	EMBEDDED
#ifdef __SRAM_SUPPORT__
static uint8_t mal_buffer[4096]; //buffer for USB
const uint8_t*  MAL_Buffer = mal_buffer;
#else
const uint8_t*  MAL_Buffer = ucHeap + 1024;
#endif	/*__SRAM_SUPPORT__*/

#endif

//source buffer is used to store the information of one frame that GPU needs to render
//refer to doc <AHMI hardware design> for 1.0 and <AHMI.docx> for 2.0
#ifdef PC_SIM
u8       sourceBuffer[20480]; //for AHMI to test
//NewSourceBuffer mNewSourceBuffer[1]; //the maxium of source buffer
#endif
#ifdef EMBEDDED
#ifdef VERSION_2
#ifndef CHIP_GC9002
u8 sourceBuffer[SoureBufferSize]__attribute__((section(".sbdata")));
#endif
//NewSourceBuffer mNewSourceBuffer[1]; //the maxium of source buffer
#endif
#endif

#ifdef PC_SIM
u8       TextureBuffer[TEXBUFSIZE];          //global buffer to store texture data
u8       VideoTextureBuffer[VIDEO_BUF_SIZE]; //the buffer to be displayed
u8       OscilloscopeArry[SIZE_OF_SingleOscilloscope_BUF * Chart_Num_MAX] = {0};//the buffer to be displayed
u8       QRCodeArry[QRcode_BUF_SIZE * QRCode_Num_MAX] = {0}; //the buffer to be displayed
u8       DataImageDataArray[SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max] = {0}; //buffer of data images
u8       H264PicBuffer[SIZE_OF_SingleH264Pic_BUF] = {0};  //buffer of h264 picture
u8       SnapshotBuffer[SIZE_OF_Snapshot_BUF] = {0}; //buffer of snapshot


u8  GlobalPageCacheBuffer[ PageCache_Size *2] = {0};  //global page cache bufffer for AHMI to write one frame data
u8  GlobalPageCacheBufferSub[ PageCache_Size *2] = {0};  //global page cache bufffer for AHMI to write one sub frame data

u8  TempFrameBuffer[ MAX_SCREEN_SIZE] = {0};  //global page cache bufffer for AHMI to write one frame data

u8  GlobalFrameBuffer[ MAX_SCREEN_SIZE *2];  //global bufffer for AHMI to write one frame data
u8  GlobalFrameBufferSub[ MAX_SCREEN_SIZE *2];  //global bufffer for AHMI to write one sub frame data
u8  GlobalFrameBuffer2[ MAX_SCREEN_SIZE *2]; //used for double buffer
u8  GlobalBackgroundBuffer[ MAX_SCREEN_SIZE *2 ]; //used for background texture
#endif
u32  GlobalPageCacheUsed[2] = {0}; 
u32  GlobalPageCacheSubUsed[2] = {0};
uint32_t *pPageCacheAddr = 0;
uint32_t *pSubPageCacheAddr = 0;

//used for param check 
u32 startOfDynamicPage; //start address of the page
u32 endOfDynamicPage;   //end   address of the page

//Configdata offset
u32 StartOfConfigdata = 0; //start address of the configdata

//screen
u16 gScreenWidth = 800;
u16 gScreenHeight = 480;

ProjectScreenOptions gScreenOptions;
PageScreenOptions currentPageScreenAreas;

u16 gDisplayWidth = 800;
u16 gDisplayHeight = 480;

char* cfgString;  // cfg information contains the information of product
//screen ratio
s16 screenratio = 512; //1.6.9

//video
u8 addr_cvbs0 = 0;
u8 video_cur_w0 = 2;
u8 video_cur_r0 = 0;
u8 video_next_w0 = 0;
u8 video_next_r0 = 1;

uint32_t SizeOfDisplayBuffer = 0;        // the size of display buffer, used to change buffer
uint32_t StartAddrOfDisplayBuffer = 0;   // the start address of display buffer
uint32_t StartAddrOfSubDisplayBuffer = 0;   // the start address of sub display buffer
#if defined(CHIP_GC9002)
uint32_t SizeOfCVBSBuffer = 0x70000;           // the size of cvbs buffer, used to change cvbs
#else
uint32_t SizeOfCVBSBuffer = 0xD0000;           // the size of cvbs buffer, used to change cvbs
#endif
uint32_t StartAddrOfCVBS0Even = 0;       //the start address of cvbs
uint32_t StartAddrOfCVBS0Odd = 0;        //the start address of cvbs
uint32_t SizeOfSoundBuffer = 0;        // the size of sound buffer
uint32_t StartAddrOfSoundBuffer = 0;   // the start address of sound buufer

#if defined(EMBEDDED) && defined(VERSION_2)
uint16_t* SoundSpace = 0;
#if defined(CHIP_GC9002)
uint32_t StartAddrOfSourcebuffer = 0x0200c000;
#elif defined(CHIP_GC9005)
uint32_t StartAddrOfSourcebuffer = (uint32_t)sourceBuffer;
#endif
#endif
#endif

