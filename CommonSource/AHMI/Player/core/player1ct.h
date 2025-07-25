/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : player1ct.h
*
* Description  : This file contains the common data type declarations of PLAR
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-09-23
****************************************************************************/
#ifndef PLAR_CT_H
#define PLAR_CT_H

typedef enum{
    enPLAR_OK = 0,                  /**/
    enPLAR_CreateQueueErr,          /* Create Queue error */
    enPLAR_CreateTaskErr,           /* Create Task error */
    enPLAR_InvalidPlay,             /* Play status not at Playing */
    enPLAR_ModuleUninted,           /* Module not inited */
	enPLAR_RefreshErr,           	/* Refresh error */
	enPLAR_CmdPageIDErr,           	/* Cmd PageID error */
	enPLAR_CmdSwPageErr,           	/* Cmd Switch Page error */
	enPLAR_CmdActionCodeErr,        /* Cmd action code error */
	enPLAR_CmdParaErr,           	/* Cmd Parameter error */
    
}PLAR_tenErrorType;


typedef enum{
    enCmdInputFromNull = 0,
    enCmdInputFromAction,
    enCmdInputFromTouch,
    enCmdInputFromTimer,
    enCmdInputFromShmem
    
}PLAR_tCmdInputFrom;


typedef enum{
    enCmdType_Null = 0,
    enCmdType_SetValue,
    enCmdType_SetString,
    enCmdType_MouseTouch,
    enCmdType_AnimationTimer,
    enCmdType_TriggerAnimation,
    enCmdType_StartPlay,
    enCmdType_StopPlay,
    enCmdType_SwitchPage,
    enCmdType_ActionExe,
    enCmdType_DrawControl,
	enCmdType_DrawVideo,
	enCmdType_RefreshGif,
	enCmdType_RefreshSnapshot,
    enCmdType_DrawVideoPlayer,
    
}PLAR_tenCmdType;



typedef enum{
    enPlayStsInit = 0,
    enPlayStsPlay,
    enPlayStsStop,
    enPlayStsInvalid,
    
}PLAR_tenPlaySts;


typedef enum{
    tenMouseTouchParaPressPoint = 0,
    tenMouseTouchParaReleasePoint,
    tenMouseTouchParaEvent,
    
}PLAR_tenMouseTouchPara;


typedef enum{
    enRefreshStsIdle = 0,
    enRefreshStsPlay,
    enRefreshStsLastOne,
}PLAR_tenRefreshSts;


/*
typedef union{
    PIDState mMousePID;     //touch
    SendValueType mTagCmd;  //tag, ShMem/SOftTimer/action
    uint32_t mAnimationID;  //animation
    uint32_t mActionAddr;   //action
}PLAR_tunCmdMsg, *PLAR_tunCmdMsgPtr;
*/

typedef struct{
    uint32_t u32CmdType;		            /* Command Type */
    uint32_t u32PageID;			            //page id，判定是否执行
    uint32_t u32Para[PLAR_PARA_SIZE];   
	uint8_t screenType;   //refresh screenType
}PLAR_tstAHMICmd, *PLAR_tstAHMICmdPtr;


typedef struct{
    uint8_t u8Inited;
    uint8_t u8PlayStatus;                   /* Player Status: init/play/stop */
    uint16_t u16CurrentPage;
    uint16_t u16ExpectedPage;
	uint16_t u16RefreshType;				/* Refresh Type: normal/matrix/NULL */
    uint16_t u16RefreshStatus;              /* Refresh Status: init/play/stop */
}PLAR_tstWorkBuffer, *PLAR_tstWorkBufferPtr;

#endif

