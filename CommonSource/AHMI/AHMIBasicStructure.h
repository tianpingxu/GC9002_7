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
//    the original structure in C
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef AHMIBASICSTRUCTURE__H
#define AHMIBASICSTRUCTURE__H
#include "publicType.h"
#include "publicDefine.h"


#ifdef AHMI_CORE
typedef struct WidgetLinkData
{
	u16 LinkWidgetPageID;//the page widget is linked to
	u16 LinkWidgetID;    //the id of widget
} *WidgetLinkDataPtr;

typedef struct CanvasLinkData
{
	u16 LinkCanvasPageID; //the page id canvas linked to
	u16 LinkCanvasID;	//CanvasId
}*CanvasLinkDataPtr;

typedef struct PageLinkData
{
	u16 LinkDynamicPageID;//Page ID
	u16 LinkActionIDInDynamicPage;//ID of Page Action array
} *PageLinkDataPtr;

//*************************ActionTriggerClass*************************************
typedef struct PID
{
u16 x;
u16 y;
u8 press;
u8 screenType;
}PIDState;

typedef struct PIDPoint
{
u16 x;
u16 y;
}PIDPoint;

typedef PIDState* PIDPtr;
typedef PIDPoint* PIDPointPtr;

//******************ModbusTask***************************************
typedef struct ModBusRefresh
{
	u16 type;//tag type£¬0 is number£¬1 the address of string
	u32 value;
	u8 tagRW;         //tag read/write from modbus¡£0 is read£¬1 is write by Arthas 20151210
	u16 regID;		 //tag binding reg by Arthas 20151207
}ModBusRefreshMsg; // tag upadate queue structure by Arthas 20151210

typedef struct ModBusRW
{
	u16 regID;
	u16 numofReg;        
	u32 value;	
	u8 opCode;
}ModBusMasterRWMsg; // ModBus read/write queue structure

struct globalArgs_t
{
	unsigned int ScreenWidth; //-w(W) option width of screen
	unsigned int ScreenHeigth;//-h(H) option hieght of screen
	char* FileName;  //-f(F) option filename input
};

#endif

#endif

