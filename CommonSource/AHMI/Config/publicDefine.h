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
//    this file only contains the definition that will used inlcuded by all core files.
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef PUBLICDEFINE__H
#define PUBLICDEFINE__H

#include "publicType.h"
#include "AHMICfgDefinition.h"

#define VectorDataSize 16*1024

#if defined(PC_SIM) || defined(PC_SIM_Gen)
#define EMBEDDED_GC9003				0
#define EMBEDDED_GC9002				1
#define EMBEDDED_GC9005				2
#define EMBEDDED_GC9002_NoHyper		3
#endif

#define DEFAULT_AUDIO_OUT_SAMPLERATE 11720
#define AUDIO_OUT_SMAPLERATE_5120 5120
#define AUDIO_OUT_SMAPLERATE_10200 10200
#define AUDIO_OUT_SMAPLERATE_11720 11720
#define AUDIO_OUT_SMAPLERATE_20500 20500
#define AUDIO_OUT_SMAPLERATE_41000 41000

#ifdef NDEBUG
#define AHMI_DIRECTORY_NAME ""
#define AHMI_CFG_FILE_NAME "ConfigData.acf" //the configuration file path
#define AHMI_TEX_EVEN_NAME "TextureEven.acf" //the texture data file path
#define AHMI_TEX_ODD_NAME "TextureOdd.acf"   //the texture data file path
#define AHMI_TEX_VIDEO_NAME "TextureVideo.acf"  //the video texture data to simulate video widget
#define AHMI_SOURCEFILE_NAME "ConfigData&Texture.acf"
#define AHMI_CMPSOURCEFILE_NAME "ConfigData&TextureCMP.acf"
#define AHMI_TEMPFILE_NAME "temp.acf"
#define AHMI_CMPTEMPFILE_NAME "temp_CMP.acf"
#define AHMI_BOOTLOADERFILE_NAME "Texture.acf"
#define AHMI_BOOTLOADERFILE_ADD_NAME "Texture_add.acf"
#define AHMI_PRODUCT_BIN_FILE_NAME "product.bin"
#define AHMI_CONV_RULE_BIN_FILE_NAME "Conv_rule.bin"
#define AHMI_SOUND_FILE_NAME   "Sound.acf"
#define AHMI_LOG_FILE_NAME   "log.csv"
#define AHMI_YTH_VERSION_FILE  "VersionInfomation\\YTH_Version.Info"
#define AHMI_ERR_MESSAGE_FILE  "ErrorInfo.bin"
#define AHMI_NUM_TAG_FILE  "NumberTagInfo.bin"
#define AHMI_STR_TAG_FILE  "StringTagInfo.bin"
#else

	#ifdef PC_SIM
	#define AHMI_CFG_FILE_NAME "..\\..\\..\\testData_0304\\ConfigData.acf"
	#define AHMI_TEX_EVEN_NAME "..\\..\\..\\testData_0304\\TextureEven.acf"
	#define AHMI_TEX_ODD_NAME "..\\..\\..\\testData_0304\\TextureOdd.acf"
	#define AHMI_TEX_VIDEO_NAME "..\\..\\..\\testData_0304\\TextureVideo.acf"
	#define AHMI_SOURCEFILE_NAME "..\\..\\..\\testData_0304\\ConfigData&Texture.acf"
	#define AHMI_CMPSOURCEFILE_NAME "..\\..\\..\\testData_0304\\ConfigData&TextureCMP.acf"
	#define AHMI_BOOTLOADERFILE_NAME "..\\..\\..\\testData_0304\\Texture.acf"
	#define AHMI_BOOTLOADERFILE_ADD_NAME "..\\..\\..\\testData_0304\\Texture_add.acf"
	#define AHMI_SOUND_FILE_NAME   "..\\..\\..\\testData_0304\\Sound.acf"
	#define AHMI_LOG_FILE_NAME   "..\\..\\..\\testData_0304\\log.csv"
	#define AHMI_YTH_VERSION_FILE  "VersionInfomation\\YTH_Version.Info"
	#define AHMI_ERR_MESSAGE_FILE  "..\\..\\..\\testData_0304\\ErrorInfo.bin"
	#define AHMI_NUM_TAG_FILE  "..\\..\\..\\testData_0304\\NumberTagInfo.bin"
	#define AHMI_STR_TAG_FILE  "..\\..\\..\\testData_0304\\StringTagInfo.bin"

	#endif

	#ifdef PC_SIM_Gen  
	//if not generated for implmentation, the file location needs to be outside the project
	#define AHMI_DIRECTORY_NAME "..\\..\\..\\..\\testData_0304"
	#define AHMI_CFG_FILE_NAME "..\\..\\..\\..\\testData_0304\\ConfigData.acf"
	#define AHMI_TEX_EVEN_NAME "..\\..\\..\\..\\testData_0304\\TextureEven.acf"
	#define AHMI_TEX_ODD_NAME "..\\..\\..\\..\\testData_0304\\TextureOdd.acf"
	#define AHMI_TEX_VIDEO_NAME "..\\..\\..\\..\\testData_0304\\TextureVideo.acf"
	#define AHMI_SOURCEFILE_NAME "..\\..\\..\\..\\testData_0304\\ConfigData&Texture.acf"
	#define AHMI_CMPSOURCEFILE_NAME "..\\..\\..\\..\\testData_0304\\ConfigData&TextureCMP.acf"
	#define AHMI_TEMPFILE_NAME "..\\..\\..\\..\\testData_0304\\temp.acf"
	#define AHMI_CMPTEMPFILE_NAME "..\\..\\..\\..\\testData_0304\\temp_CMP.acf"
	#define AHMI_BOOTLOADERFILE_NAME "..\\..\\..\\..\\testData_0304\\Texture.acf"
	#define AHMI_BOOTLOADERFILE_ADD_NAME "..\\..\\..\\..\\testData_0304\\Texture_add.acf"
	#define AHMI_PRODUCT_BIN_FILE_NAME "..\\..\\..\\..\\testData_0304\\product.bin"
	#define AHMI_CONV_RULE_BIN_FILE_NAME "..\\..\\..\\..\\testData_0304\\Conv_rule.bin"
	#define AHMI_SOUND_FILE_NAME   "..\\..\\..\\..\\testData_0304\\Sound.acf"
	#define AHMI_LOG_FILE_NAME   "..\\..\\..\\..\\testData_0304\\log.csv"
	#define AHMI_YTH_VERSION_FILE  "VersionInfomation\\YTH_Version.Info"
	#endif


#endif

typedef enum
{ 
	KeyBoard = 0 ,
	TouchCalibration ,
	ErrorManagement ,
	Debugging 
}SystemPage_List;

#if defined(CHIP_GC9005)
#define TILESIZE 16         //the size of TILE
#else
#define TILESIZE 32         //the size of TILE
#endif

#define WIDGET_TOUCH_CANCEL			2 //the function touch cancel for touch
#define AHMI_FUNC_FAILURE			0 //the function failure
#define AHMI_FUNC_SUCCESS			1 //the function success
#define AHMI_FUNC_MATRIX_ILLEGAL	3 //the function matrix value illegal

#define     SIM_DXT1      0 //DXT1 type of texture. 4x4 block compression method. 16*32bit color->64bit color.
#define     SIM_DXT3      1 //DXT3 type of texture. 4x4 block compression method. 16*32bit color->64bit color + 64bit alpha.
#define     SIM_ALPHA8    10 //8 bit alpha
#define     SIM_ALPHA1    2  //1 bit alpha
#define     SIM_ALPHA2    9   //2 bit alpha
#define     SIM_ALPHA4    11  //4 bit alpha
#define     SIM_PURECOLOR 3   //PURECOLOR. The triangle contains only one color
#define     SIM_RGB565    4   //image texture of format 565(5bit red, 6bit green, 5bit blue)
#define     SIM_RGBA8888  5   //image texture of format 8888(8bit red, 8bit green, 8bit blue)
#define     SIM_Y8        6   //reserved
#ifdef VERSION_2
#define     SIM_RGB565_RE 7
#endif
#define     SIM_PUREMASK  8
#define     SIM_RGBA1555  12
#define     SIM_RGBA4444  13
#define     SIM_DXT5      14
#define     SIM_ARGB8888_RE 15

#define     TilePixelSize  (TILESIZE*TILESIZE)
#define		ENCODING_ASCII 0
#define		ENCODING_UTF8 1
#define		ENCODING_GB2312	2

//the type for graphics format
#define     U64 unsigned long long 
#define 	Alpha 	u8
#define 	Mask 	u8
#define 	BitMask  u8
#define     Color16  u16

//wave filter types
#define WaveFilter_Damping_Linear 0
#define	WaveFilter_Damping_PT1 1
#define WaveFilter_Damping_PT2 2


//screen 
extern u16 gScreenWidth; //screen width
extern u16 gScreenHeight; //screen height

extern u16 gDisplayWidth; 
extern u16 gDisplayHeight;
//#ifdef PC_SIM
#define     TILE_NUM_X  ((gScreenWidth + TILESIZE-1)/TILESIZE)//x total number of tile
#define     TILE_NUM_Y  ((gScreenHeight + TILESIZE-1)/TILESIZE)//y total number of tile
//#endif
#ifdef EMBEDDED
#define SCREEN_WIDTH_CFG 800  //screen width  for 5 point scene
#define SCREEN_HEIGHT_CFG 480 //screen height for 5 point scene
#define TILE_NUM_X_CFG  ((SCREEN_WIDTH_CFG + TILESIZE-1)/TILESIZE)//x total number of tile
#define TILE_NUM_Y_CFG  ((SCREEN_HEIGHT_CFG + TILESIZE-1)/TILESIZE)//y total number of tile
#endif

//#define NORMALIZE_ENABLE

#define 	SCREEN_WIDTH  gScreenWidth//screen width
#define 	SCREEN_HEIGHT gScreenHeight//screen height

#define 	DISPLAY_WIDTH  gDisplayWidth  //screen width
#define 	DISPLAY_HEIGHT gDisplayHeight //screen height
#ifdef EMBEDDED
#define		STARTOFMEMORY	0x20000000
#define		ENDOFMEMORY		0x20010000      //total memory 64K
#endif

#define     SCREEN_SIZE ((TILE_NUM_X) * (TILE_NUM_Y) * TilePixelSize )
#define     MAX_SCREEN_SIZE (1920) * (1080) * 3

static  U64 Mul_count = 0;
#define MUL(a,b) (Mul_count++,(U64)((U64)(a)*(U64)(b))) 

//#define TRUE 1
//#define FALSE 0

#define  SRCBUFSIZE  4096
#ifdef PC_SIM
#define  TEXBUFSIZE  1024*1024*512
#define  VIDEO_BUF_SIZE 720*576*2
#define  Oscilloscope_BUF_SIZE 800*480/8
#define  QRcode_BUF_SIZE 250000
#define  DataImage_BUF_SIZE  800*480*4
#define  H264Pic_BUF_SIZE    800*480*4
#define  Snapshot_BUF_SIZE   1024*768*4
#define  PageCache_Size   1920*1080*4
#endif

#if defined(VERSION_2) && defined(EMBEDDED)
#define  QRcode_BUF_SIZE (4096)
#endif

#define CVBS_REMOVING_BLACK_BORDER 1 //whether to mask black border for cvbs video, 1 use, 0 not use

typedef u32 funcStatus;

#endif


#define STRINGMAXSIZE 32

