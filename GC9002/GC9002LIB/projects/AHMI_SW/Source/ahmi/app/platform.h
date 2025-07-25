////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI Team
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2018/09/26 by Yu Chunying
// Additional Comments:
//    definition for debug
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef PLATFORM__H
#define PLATFORM__H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* ¹¦ÄÜºê
******************************************************************************/
/*
 *	Select platform
 */
//#define PC_SIM         					//for PC simulator
#define EMBEDDED     						//for Embedded
//#define PC_SIM_Gen         				//for simg generator

//the version of platform, choose one of below
//#define VERSION_1       //for v1.x.x/fpga
#define VERSION_2     //for v2.x.x/IC

#define CHIP_GC9002     					//for Embedded

//#define CHIP_GC9002_NoHyper     			//for Embedded(GC9002_NoHyper)
	
#if defined(PC_SIM) && defined(EMBEDDED) && defined(PC_SIM_Gen)
#error "Please select only one from PC_SIM ,EMBEDDED  or PC_SIM_Gen macro"
#elif !defined(PC_SIM) && !defined(EMBEDDED) && !defined(PC_SIM_Gen)
#error "Please select one from PC_SIM, EMBEDDED or PC_SIM_Gen macro"
#endif

#ifdef __cplusplus
}
#endif

#endif
