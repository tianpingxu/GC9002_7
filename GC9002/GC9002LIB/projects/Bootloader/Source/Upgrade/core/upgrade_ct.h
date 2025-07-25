/**
  ******************************************************************************
  * @file    upgrade_ct.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    21-06-2021
  * @brief   The type definition head file of Upgrade module
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPGRADE_CT_H
#define __UPGRADE_CT_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "ff.h"

#define     MAX_UPGRADE_FILE_COUNT      32

#define UPG_TRUE            (1)
#define UPG_FALSE           (0)

typedef  void (*pFunction)(void);


typedef enum
{
    UPG_enComSdcard = 0,
    UPG_enComMSD,
    UPG_enComCan,
    UPG_enComUart,
    UPG_enComInvalid
        
}UPG_tenComMode;


typedef enum
{
    UPG_enUpgFlashCpu0Nor = 0,
    UPG_enUpgFlashCpu1,
    UPG_enUpgFlashTexture,
    UPG_enUpgFlashCpu1boot2,
    UPG_enUpgFlashInvalid,
        
}UPG_tenUpgFlashType;

/* Note: the configuration sequence in ci.h  must keep consistent with here*/
typedef enum
{
    UPG_enUpgFileCpu0Bin = 0,
    UPG_enUpgFileTexture,
    UPG_enUpgFileCpu1Bin,
    UPG_enUpgFileProduct,
    UPG_enUpgFileCalibration,
    UPG_enUpgFilefHWConfig,
	UPG_enUpgFileCpu0Cfg,
    UPG_enUpgFileEraseStoreSpace,
    UPG_enUpgFileBootloader,
    UPG_enUpgFileUpgCfg,
    UPG_enUpgFileCount
    
}UPG_tenUpgFileType;


/* Used for communication with CPU1 */
typedef enum
{
    UPG_enOpVerifyFile = 0,
    UPG_enOpErase,
    UPG_enOpWriteFile,
    UPG_enOpVerifyData,
    UPG_enOpEraseCount
    
}UPG_tenOpType;


/* Save the parameter of the special Flash */
typedef struct
{
    UPG_tenUpgFlashType             enUpgFlashType;
    uint16_t                        u16SectorSize;
    uint16_t                        u16BufferLen;
    
}UPG_tstUpgFlashConfig;


typedef enum
{
    UPG_enUpgStepInit = 0,
    UPG_enUpgStepCheckVersion,
    UPG_enUpgStepGetData,
    UPG_enUpgStepVerifyData,
    UPG_enUpgStepEraseFlash,
    UPG_enUpgStepWaitEraseDone,
    UPG_enUpgStepWriteFlash,
    UPG_enUpgStepWaitWriteDone,
    UPG_enUpgStepVerifyFlash,
	UPG_enUpgStepWaitVerifyDone,
    UPG_enUpgStepEnd,
    UPG_enUpgStepDelayJumpToApp,
    UPG_enUpgStepGetConfig,
    UPG_enUpgStepGetFirstUpdateFile,
    UPG_enUpgStepInvalid
   
}UPG_tenUpgStepType;


typedef enum
{
    UPG_enVerifyInputCrc = 0,
    UPG_enVerifyInputXor,
    UPG_enVerifyInputInvalid
    
}UPG_tenVerifyInputType;


typedef struct 
{	
    UPG_tenUpgFileType              enUpgFileType;                  /* Upgrade file type: CPU0.bin/CPU1.bin, ...   */
    UPG_tenUpgFlashType             enUpgFlashType;                 /* Upgrade flash type, it determine the Flash parameter */
    UPG_tenVerifyInputType          enVerifyInputType;              /* the verification type of input data : CRC / XOR */
	
    bool                            bFlagCheckVersion;              /* Flag to indicate if check version is needed */
    bool                            bFlagVerifyInputFile;           /* Flag to indicate if verifying input file is needed */
    bool                            bFlagVerifyStoredFile;          /* Flag to indicate if verifying stored file is needed */
    bool                            bFlagVerifyUpdateProgress;      /* Flag to indicate if update progress when verify */
    bool                            bFlagFixedRecvBufAddress;       /* Flag to indicate if Receive Buffer Address is fixed */
    bool                            bFlagUpgWithSelfUpg;            /* Flag to indicate if Upgrade with bootlader.bin */

    uint8_t*                        pu8UpgFlashStartAdress;         /* Upgrade file save start address (on Flash) */
    uint8_t*                        pu8RecvBufStartAddress;         /* start address of Receive buffer */
    uint32_t                        u32RecvBufSize;                 /* Receive buffer Size */
    uint8_t*                        pu8FileName;                    /* Address point to file name */ 
}UPG_tstPrebuildConfig;


typedef struct 
{	
    bool                            bFlagBufEnough;                 /* Flag indicate the buffer for receive data is big enough */
    bool                            bFlagReadFinished;              /* Flag indicate the data all read to buffer */
    bool                            bFlagFlashing;                  /* Flag indicate the Flash is operating */
    bool                            bFlagCheckVerValid;             /* Flag indicate check version is valid: send check version command to CPU1 already */
    
    UPG_tenUpgStepType              enUpgStep;                      /* current Upgrade step */
	uint8_t*                        pu8RecvBufStartAddress;         /* start address of Receive buffer */
    uint8_t*                        pu8RecvBufCurAddress;           /* current address of Receive buffer */
    uint8_t*                        pu8FlashWrCurAddress;           
    uint32_t                        u32RecvBufTotalSize;            /* Total buffer size */
    uint8_t*                        pu8RecvConvertBufStartAddress;  /* current address of Receive convert buffer */           
    uint32_t                        u32RecvConvertBufTotalSize;     /* Total convert buffer size */
    uint32_t                        u32FileSize;                    /* file size */
    uint32_t                        u32FileReadSize;                /* the size of already read data from file */
    uint32_t                        u32FlashWroteSize;              /* Already wrote size */
    uint32_t                        u32TmpWrLeftSize;               /* Temporary wrote Flash left size */
    uint32_t                        u32TmpWrSize;                   /* Temporary wrote Flash size */
}UPG_tstRuntimeConfig;


typedef enum
{
    UPG_enMainSM_Init = 0,
    UPG_enMainSM_Upgrade,
    UPG_enMainSM_JumpToApp,
    UPG_enMainSM_Error,
    UPG_enMainSM_Invalid
	
}UPG_tenMainSM;


typedef enum
{
    UPG_enSubSM_GetComMode = 0,
    UPG_enSubSM_Process_Active,
	UPG_enSubSM_Process_Passive,
    UPG_enSubSM_Invalid
	
}UPG_tenSubSM;


typedef enum
{
    UPG_enSubSMPassive_CanUart_FirstIn = 0,
    UPG_enSubSMPassive_CanUart_Run,
	UPG_enSubSMPassive_CanUart_Quit,
    UPG_enSubSMPassive_Invalid
	
}UPG_tenSubSMPassive;




typedef struct 
{	
    bool                            bFlagCpu1PreUpgrade;            /* Flag to indicate if File System operation is needed: Com mode is SD-Card */
    bool                            bFlagCpu1Upgrading;             /* Flag to indicate if File System operation is needed: Com mode is SD-Card */
    bool                            bFlagFfOperation;               /* Flag to indicate if File System operation is needed: Com mode is SD-Card */
    bool                            bExistUpgComCAN;              	/* Flag to indicate if Com mode CAN is enable */
    bool                            bExistUpgComUART;           	/* Flag to indicate if Com mode UART is enable */
    bool                            bExistUpgComUSB;          		/* Flag to indicate if Com mode USB is enable */
    bool                            bExistUpgComSD;      			/* Flag to indicate if Com mode SD is enable */
    bool                            bFlagCanWork;                   /* Flag to indicate if CAN work(shakehand finished) */
	bool                            bFlagUartWork;                  /* Flag to indicate if UART work(shakehand finished) */
    bool                            bFlagCanInit;                   /* Flag to indicate if CAN Initialized */
    bool                            bFlagUartInit;                  /* Flag to indicate if UART Initialized */
    bool                            bFlagBootloaderExist;           /* Flag to indicate if bootloader.bin exist */
    
    uint8_t                         u8ErrState;
    uint8_t                         u8FileIndex;                    /* */
    uint8_t                         u8FileCount;
    uint16_t                        u16GetModeTryTimes;             /* the times of trying to get Comm mode */
    uint16_t                        u16GetVersionTryTimes;
    uint32_t                        u32BootloaderAdaptedFlashAddr;  /* Bootloader self-Upgrade Flash address: depend on Flash size */
    UPG_tenMainSM                   enMainSM;                       /* Main state machine  */
    UPG_tenSubSM                    enSubSM;                        /* Sub state machine */
    UPG_tenSubSMPassive             enSubSMPassive;                 /* Passive sub state machine */
    UPG_tenComMode                  enComMode;
	const UPG_tstPrebuildConfig*    pstPbConfig;                    /* Point to Prebuild config */
    UPG_tstRuntimeConfig            stRtConfig;                     /* Runtime config of Upgrade */
    FIL*                            pFil;
    FATFS*                          pFatFs;
    DIR*                            pDir;
}UPG_tstWorkBuf;


typedef struct 
{	
    bool                            bFlagCpu1Runned;  
	bool                            bFlagCpu1Waiting;
    bool                            bFlagCpu1PreUpgrade;
    bool                            bFlagCpu1Upgrading;
    bool                            bFlagCpu1Finished;
    bool                            bFlagCpu0Waiting;
    bool                            bFlagCpu0PreUpgrade;
    bool                            bFlagCpu0Upgrading;
    bool                            bFlagCpu1UpgradeEnd;
    bool                            bFlagCpu1QuitWaiting;
}UPG_tstHeartbeat, *UPG_tstHeartbeatPtr;

	 

#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_CT_H */




