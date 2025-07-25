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
//    definition for debug
// Revision:
// Revision 1.6.2 - Modify 2019/07/16 by Yu ChunYing
// Additional Comments:
//   添加视频奇偶帧融合所需控制宏，增加嵌入式软件子版本号。
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef AHMI_CFG_DEFINITION
#define AHMI_CFG_DEFINITION

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"
#if defined(CHIP_GC9005)
#include "HUDDraw.h"
#endif

#ifdef PC_SIM

//#define AHMI_DEBUG            //to open debug information
#define AHMI_CORE               //core code
#define SXGL_LIB
#define USE_TOUCHSCREEN         //whether to open touch screen
//#define TEST_VECTOR_GEN      //generate the test date for FPGA


#define KEYBOARD_DEBUG         //open the debug for keyboard
//#define TEST_LOG_GEN         //test log of uv
//#define TILE_BUFFER_LOG_GEN  //test log of tile buffer
//#define PARTIAL_DOUBLE_BUFFER //partial double buffer
#define WHOLE_TRIBLE_BUFFER     //whole tribble buffer
#define STATIC_BUFFER_EN        //static buffer on
//#define VISUAL_SOURCE_BOX_EN //make the tile box visual


//#define USE_MODBUS_MASTER					0   //use Jbus
//#define USE_MODBUS_SLAVE                  1

//#define PAGE_CACHE_ENABLE		//enable page cache for page animation
#endif

#ifdef EMBEDDED
//#define DEBUG          					//定义了DEBUG则会打开所有的调试信息
#define AHMI_CORE      						//core code
//#define USE_TOUCHSCREEN					 //whether to use touch screen
//#define KEYBOARD_DEBUG  					//open the debug for keyboard
#define USING_XILINX						//using Xilinx FPGA
//#define USING_LATTICE						//using Lattice FPGA
#define KEYBOARD_DEBUG  					//open the debug for keyboard
//#define PARTIAL_DOUBLE_BUFFER 			//partial double buffer, unused
#define WHOLE_TRIBLE_BUFFER  				//whole tribble buffer
//#define PARTIAL_TRIBLE_BUFFER 			//partial double buffer
#define STATIC_BUFFER_EN     				//open the static buffer
//#define DDR_TEST							//to test DDR
//#define FLASH_TEST						//to test flash in bypass mode
//#define SOURCE_BOX_COMBINE  				//open source box combile strategy

//#define VIDEO_ODD_EVEN_ALL               //every even and odd frame has the whole resolution
//#define VIDEO_ODD_And_EVEN					//VIDEO CVBS use ODD and EVEN Deinterlacing
//#define VIDEO_ODD                         //VIDEO CVBS use ODD Deinterlacing
#define VIDEO_EVEN                        //VIDEO CVBS use EVEN Deinterlacing

//when choose VIDEO_ODD_And_EVEN, choose one of below according to the specific situation
#define VIDEO_EVEN_FIRST				//VIDEO CVBS EVEN first
//#define VIDEO_ODD_FIRST					//VIDEO CVBS ODD first

//#define USE_MODBUS_MASTER					1   //use Jbus
//#define USE_MODBUS_SLAVE                  1
//#define WATCH_DOG_CPU1                      /*  Open/Close WatchDog for CPU1 */

#ifndef CHIP_GC9002_NoHyper
#ifndef HUDDraw_ENABLE
#define SXGL_LIB
#endif
#define VIDEO_EN
//#define PAGE_CACHE_ENABLE		//enable page cache for page animation
#endif

#define VIDEO_FIRST_FRAME_NUMBER			6	//The first VIDEO_FIRST_FRAME_NUMBER frames of video display turn off the triple buffer

//#define LowDelayRefresh
//#define HighDelayRefresh
#endif	/* end of defined EMBEDDED */

#ifdef PAGE_CACHE_ENABLE	
#define PAGE_CACHE_NUM	2
#else
#define PAGE_CACHE_NUM	0
#endif

//all version, refresh strategy
//#define SOURCE_BOX_COMBINE  //open source box combile strategy

/******************************************************************************
* macro
******************************************************************************/
/*
 *	AHMI version, the base line of embedded
 *	AHMI_CPU1_VERSION needs to be the same with generation software, otherwise the 5 point scene will be displayed
 */
#define AHMI_CPU1_VERSION 			0x5A010a13

#define CPU0VERSION					"CPU0VERSION"
#define DEVICEID					"CPU0DEVICEID"

#ifdef PC_SIM_Gen         					//only for PC generator
#define AHMI_CORE      						//core code
#define SXGL_LIB
#endif

#ifdef __cplusplus
}
#endif

#endif //end of header
