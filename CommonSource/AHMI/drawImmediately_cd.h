////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     drawImmediately_cd.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
// Revision:
// Revision 1.6.2 - Modify 2019/07/16 by YuChunYing
// Additional Comments:
//   
//    
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef DRAWIMMEDIATELY_CD__H
#define DRAWIMMEDIATELY_CD__H
#include "publicDefine.h"
#ifdef AHMI_CORE


#ifdef VERSION_2
#define SDARM_SIZE_BYTE_32M
//#define SDARM_SIZE_BYTE_16M
#endif

//*****************************drawimmediately**********************************
#define  Stm32_interface_Addr  stm32info
#define  Tile_start_x 0
#define  Tile_start_y 1
#define  Tile_end_x   2
#define  Tile_end_y   3
#define  addr_ahmi_0  4 //part of ahmi addr 
#define  addr_ahmi_1  5 //part of ahmi addr 
#define  addr_ahmi_2  6 //part of ahmi addr 
#define  addr_ahmi_3  7 //part of ahmi addr 
#define  addr_disp_0  8 //part of disp addr 
#define  addr_disp_1  9 //part of disp addr 
#define  addr_disp_2  10 //part of disp addr 
#define  addr_disp_3  11 //part of disp addr 
#define  addr_video_0  12 //part of video addr 
#define  addr_video_1  13 //part of video addr 
#define  addr_video_2  14 //part of video addr 
#define  addr_video_3  15 //part of video addr 
#define  clk_disp_mul_0 16 //display mul
#define  clk_disp_mul_1 17 //display mul
#define  clk_disp_div_0 18 //display div
#define  clk_disp_div_1 19 //display div
#define  hpixels_0      20 //horizontal live pixels        
#define  hpixels_1      21 //horizontal live pixels  
#define  vlines_0       22 //vertical live lines        
#define  vlines_1       23 //vertical live lines 
#define  hsyncpw_0      24 //HSYNC pulse width
#define  hsyncpw_1      25 //HSYNC pulse width
#define  vsyncpw_0      26 //VSYNC pulse width
#define  vsyncpw_1      27 //VSYNC pulse width
#define  hfnprch_0      28 //herizontal front portch
#define  hfnprch_1      29 //herizontal front portch
#define  vfnprch_0      30 //vertical front portch
#define  vfnprch_1      31 //vertical front portch
#define  hbkprch_0      32 //herizontal back portch
#define  hbkprch_1      33 //herizontal back portch
#define  vbkprch_0      34 //vertical back portch
#define  vbkprch_1      35 //vertical back portch
#define  logical_addr_0 36 //logical address of nand flash
#define  logical_addr_1 37 //logical address of nand flash
#define  physical_addr_0 38 //physical address of nand flash
#define  physical_addr_1 39 //physical address of nand flash
#define  BBMctrl_0       40 //bbm control of nand flash
#define  BBMctrl_1       41 //bbm control of nand flash
#define  screen_rotate_0 42 //screen rotate degree
#define  screen_rotate_1 43 //screen rotate degree
#define  video_interrupt_enable_0   50 //part of video interrupt 
#define  video_interrupt_enable_1   51 //part of video interrupt 
#define  addr_video0_0              52 //part of video0 addr 
#define  addr_video0_1              53 //part of video0 addr 
#define  addr_video0_2              54 //part of video0 addr 
#define  addr_video0_3              55 //part of video0 addr 
#define  addr_video1_0              56 //part of video1 addr 
#define  addr_video1_1              57 //part of video1 addr 
#define  addr_video1_2              58 //part of video1 addr 
#define  addr_video1_3              59 //part of video1 addr 
#define  addr_video2_0              60 //part of video2 addr 
#define  addr_video2_1              61 //part of video2 addr 
#define  addr_video2_2              62 //part of video2 addr 
#define  addr_video2_3              63 //part of video2 addr 
#define  addr_video3_0              64 //part of video3 addr 
#define  addr_video3_1              65 //part of video3 addr 
#define  addr_video3_2              66 //part of video3 addr 
#define  addr_video3_3              67 //part of video3 addr
#define  addr_video_max_0           68 //part of video0 max 
#define  addr_video_max_1           69 //part of video1 max 
#define  addr_video_max_2           70 //part of video2 max 
#define  addr_video_max_3           71 //part of video3 max 

extern uint32_t SizeOfDisplayBuffer;        // the size of display buffer, used to change buffer
extern uint32_t StartAddrOfDisplayBuffer;   // the start address of display buffer
extern uint32_t StartAddrOfSubDisplayBuffer;   // the start address of sub display buffer
extern uint32_t SizeOfCVBSBuffer;           // the size of cvbs buffer, used to change cvbs
extern uint32_t StartAddrOfCVBS0Even;       //the start address of cvbs
extern uint32_t StartAddrOfCVBS0Odd;        //the start address of cvbs
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
extern uint32_t StartAddrOfSourcebuffer;
#endif
#ifdef VERSION_2

#ifdef SDARM_SIZE_BYTE_32M
#define  SIZE_OF_DISPLAY_BUFFER     0x300000 // the size of display buffer, used to change buffer
#define  START_ADDR_OF_DISPLAY      0x200000 // the start address of display buufer

#define  SIZE_OF_CVBS_BUFFER        0x100000 // the size of cvbs buffer, used to change cvbs
#define  START_ADDR_OF_CVBS0_EVEN   0xD00000 //the start address of cvbs
#define  START_ADDR_OF_CVBS0_ODD    0xD80000 //the start address of cvbs

#define FRAME_BUFFER_START_ADDR        FRAME_BASE_ADDR
#endif

#ifdef SDARM_SIZE_BYTE_16M
	#ifdef VIDEO_EN
		#define  SIZE_OF_DISPLAY_BUFFER         0x130000// the size of display buffer, used to change buffer
        #define  START_ADDR_OF_DISPLAY          0x200000 // the start address of display buufer
		#define  SIZE_OF_CVBS_BUFFER            0xD00000 // the size of cvbs buffer, used to change cvbs
		#define  START_ADDR_OF_CVBS0_EVEN     	0x590000 //the start address of cvbs
		#define  START_ADDR_OF_CVBS0_ODD      	0x5F8000 //the start address of cvbs
	#else
		#define  SIZE_OF_DISPLAY_BUFFER         0x200000// the size of display buffer, used to change buffer
		#define  SIZE_OF_CVBS_BUFFER            0 // the size of cvbs buffer, used to change cvbs
	#endif
	
#define FRAME_BUFFER_START_ADDR        FRAME_BASE_ADDR
#endif

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#define  START_ADDR_OF_RAM          0x20000000 //the start address of hyper ram
#else
#define  START_ADDR_OF_RAM          0x80000000 //the start address of ram
#endif

#endif


//0x8000_0000 - 0x800C_A800  video buf(VIDEO_BUF_SIZE=720*576*2=0xCA800)
//0x800C_A800 - 0x8032_CDA0  QRCode buf(QRcode_BUF_SIZE * QRCode_Num_MAX=2500000=0x2625A0)
//0x8032_CDA0 - 0x803A_20A0  Oscilloscope buf(SIZE_OF_SingleOscilloscope_BUF * Chart_Num_MAX=480000=0x75300)
//0x803A_20A0 - 0x8124_80A0  DataImage buf(SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max=15360000=0xEA6000)
//0x8124_80A0 - 0x820E_E0A0  H264Pic buf(SIZE_OF_SingleH264Pic_BUF * 1 =1536000=0x177000)
//0x813B_F0A0 - 0x816B_F0A0  Snapshot buf(Snapshot_BUF_SIZE * 1 =3145728=0x300000)
//0x816B_F0A0 - 0x8269_10A0  Page Cahce buf(PageCache_Size * 2 =82944000=0xFD2000)
//0x8269_10A0 - 0x8366_30A0  Page Sub Cache buf(PageCache_Size * 2 =82944000=0xFD2000)
#ifdef PC_SIM
#define  START_ADDR_OFFSET					0x0		  //the offset address different between PC_SIM and EMBEDDED
#define  START_ADDR_OF_QRCode				0x800ca800 //the start address of QRCode ram
#define  SIZE_OF_SingleQRCode_BUF			QRcode_BUF_SIZE //the size of single QRCode ram buffer, has QRCode_Num_MAX(10)
#define  START_ADDR_OF_Oscilloscope			0x8032cda0 //the start address of Oscilloscope ram
#define  SIZE_OF_SingleOscilloscope_BUF		Oscilloscope_BUF_SIZE //the size of single Oscilloscope ram buffer, has Chart_Num_MAX(10)
#define  START_ADDR_OF_DataImage            0x803a20a0 //the start address of data image buf
#define  SIZE_OF_SingleDataImage_BUF        DataImage_BUF_SIZE //the size of single data image buffer(rgba), has DataImage_Num_Max(10)
#define  START_ADDR_OF_H264Pic              0x812480a0 //the start address of h264 picture buf
#define  SIZE_OF_SingleH264Pic_BUF          H264Pic_BUF_SIZE //the size of single h264 picture buffer(sfbc), has 1
#define  START_ADDR_OF_Snapshot             0x813bf0a0 //the start address of snapshot picture buf
#define  SIZE_OF_Snapshot_BUF				Snapshot_BUF_SIZE //the size of single snapshot picture buffer, has 1
#define  START_ADDR_OF_PAGE_CACHE           0x816bf0a0 //the start address of page cache buf
#define  SIZE_OF_PAGE_CACHE				    PageCache_Size //the size of single page cache buffer, has 2
#define  START_ADDR_OF_PAGE_SUB_CACHE       0x826910a0 //the start address of subpage cache buf
#define  SIZE_OF_PAGE_SUB_CACHE				PageCache_Size //the size of single subpage cache buffer, has 2
#endif

#define NUM_OF_STM32_REGS 42

#ifndef CHIP_GC9002
#define  SoureBufferAddr     sourceBuffer
#endif
#define  NewSoureBufferSize  gNewSoureBufferSize //88bytes

#define  COLOR_BITS    16

#ifdef PC_SIM
#define  SoureBufferSize  0x10000
#endif

/*
 *  source buffer
 *	
 */
#ifdef VERSION_2
    #define SoureBufferSize                 SOURCE_BUFFER_SIZE
	#if defined(CHIP_GC9002)
	#define SOURCE_BUFFER_START_ADDR        ((uint32_t)StartAddrOfSourcebuffer)
	#define SOURCE_BUFFER_SIZE              (0x4000)
    #elif defined(CHIP_GC9005)
    #define SOURCE_BUFFER_START_ADDR        ((uint32_t)StartAddrOfSourcebuffer)
	#define SOURCE_BUFFER_SIZE              (0x10000)
    extern uint8_t sourceBuffer[SoureBufferSize];
	#else
    #define SOURCE_BUFFER_ADDR_OFFSET       ((uint32_t)0x40100000)
    #define SOURCE_BUFFER_START_ADDR        ((uint32_t)sourceBuffer + SOURCE_BUFFER_ADDR_OFFSET)
	#define SOURCE_BUFFER_SIZE              (0x4000)
	#endif
#endif

#if defined(PC_SIM) || defined(PC_SIM_Gen)
#define  TEXTYPEMASK          0xf0

#define  DXT1                 0x00
#define  DXT3                 0x10

#define  ARGB3454             0xd0  //20160613
#define  DXT5                 0xe0  
 
#define  STCRGB888            0x00
#define  STCRGBA888           0x10
#define  ALPHA8               0xa0
#define  ALPHA1               0x20
#define  ALPHA2               0x90  //20160613
#define  ALPHA4               0xb0  //reserved
#define  PURECOLOR            0x30
#define  RGB565               0x40
#define  RGBA8888             0x50
#define  Y8                   0x60
#define  RGB565_RE            0x70
#define  PUREMASK             0x80
#define  ARGB1555             0xc0  //20160613
#define  ARGB4444             0xd0  //20160613
#define  SFBC                 0xe0
#define  RGBA8888_RE          0xf0  
#elif defined(CHIP_GC9002) || defined(CHIP_GC9005)
#define  TEXTYPEMASK          0xf0
#define  STCRGB888            0x00
#define  STCRGBA888           0x10
#define  ALPHA8               0xa0
#define  ALPHA1               0x20
#define  ALPHA2               0x90  //20160613
#define  ALPHA4               0xb0  //reserved
#define  PURECOLOR            0x30
#define  RGB565               0x40
#define  RGBA8888             0x50
#define  Y8                   0x60
#define  RGB565_RE            0x70
#define  PUREMASK             0x80
#define  ARGB1555             0xc0  //20160613
#define  ARGB4444             0xd0  //20160613
#define  SFBC                 0xe0
#define  RGBA8888_RE          0xf0  
#else
#define  TEXTYPEMASK          0xf0
#define  DXT1                 0x00
#define  DXT3                 0x10
#define  ALPHA8               0xa0
#define  ALPHA1               0x20
#define  ALPHA2               0x90  //20160613
#define  ALPHA4               0xb0  //reserved
#define  PURECOLOR            0x30
#define  RGB565               0x40
#define  RGBA8888             0x50
#define  Y8                   0x60
#define  RGB565_RE            0x70
#define  PUREMASK             0x80
#define  ARGB1555             0xc0  //20160613
#define  ARGB3454             0xd0  //20160613
#define  DXT5                 0xe0  
#define  RGBA8888_RE          0xf0  
#endif
					          
#define  MASKBIT              0x8
#define  MASKTEX              0x8
#define  NONMASKTEX           0x0
					          

#define  EFMATRIX             0x0
#define  ABEFMATRIX           0x2
#define  ADEFMATRIX           0x4
#define  ABCDEFMATRIX         0x6
#define  ABCDEFGHMATRIX		  0x80000000 


#define  NONADDRTEXTURE         0
#define  ADDRTEXTURE          0x100

#define  MASKTYPE_OVERLAP		0
#define  MASKTYPE_AND			0x200
#define  MASKTYPE_OR			0x400
#define  MASKTYPE_NOT			0x600
#define  FORCE_UNDISPLAY		0x800
#define	 TEXTURE_CENTRAL_ROTATE 0x2000
#define	 TEXTUREPOINT_USINGWIDGET 0x4000

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#define  FOREGROUND_NOT			0x10000
#define  BACKGROUND_NOT			0x20000
#define  BACKGROUND_FILTER		0x40000
#endif

#define		DRAWING                 0x8000

#define     ANIMATION_MOVING_BIT    0x4000
#define     ANIMATION_MOVING        0x4000
#define     ANIMATION_NO_MOVING     0     

#define  USING_PIXEL_RATIO    0x1000      

#define  ENDFLAG              0x1
#define  ENDFLAG_VERSION2     0x8000



//*****************************drawimmediately**********************************

#endif

#endif
