/**
  ******************************************************************************
  * @file    upgrade_c1.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    21-06-2021
  * @brief   The head file of Upgrade module interface.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPGRADE_C1_H
#define __UPGRADE_C1_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#include "upgrade_ct.h"
#include "upgrade_ci.h"
#include "upgrade_ce.h"
#include "upgrade_sm.h"
#include "upgrade_passive.h"
#include "upgrade_subsm_passive.h"
#include "upgrade_uart_platform.h"
#include "upgrade_btld.h"


//#include "upgrade_it.h"
//#include "upgrade_hb.h"
//#include "upgrade_syn.h"
#define UPG_FAILURE 						(0)
#define UPG_SUCCESS 						(1)
#define UPG_TIMEOUT 						(2)

#define TRUE_1                          	(1)
#define FALSE_0                         	(0)

#define UPG_OK                              (0)
#define UPG_ERR                             (1)

#define TextureBufferMaxSize            (2048)
#define TextureNorFlashBlockSize        (0x10000)

#define CodeFlashBaseAddr       		(0x08000000)
#define CodeFlashBaseAddrMask     		(0xFF000000)
//#define OffsetAddrOfCodeFlash1      		(0x20000)
#define OffsetAddrOfDisplayPara      		(0xF000)
#define OffsetAddrOfHWConfigMask      		(0xE000)
#define OffsetAddrOfCalibrationConf			(0xB000)

//#define StartAddrOfCodeFlash0Mask       (0xFFF80000) 
//#define StartAddrOfCodeFlash1Mask       (0xFFF80000)
#define StartAddrOfCodeFlash2Mask       (0x0000F000)
//#define StartAddrOfTextureFlashMask     (0xF0000000)
//#define StartAddrOfDisplayParaMask      (0xF0000000)
//#define StartAddrOfHWConfigMask         (0xF0000000)
//#define StartAddrOfEraseStoreSpaceMask  (0xF0000000)

uint32_t UPG_u32GetMaxTextureSize(void);
int32_t UPG_i32GetUpgFileName(uint8_t fileIndex, uint8_t** ppFileName);
uint8_t UPG_u8GetCurUpgFileIndex(void);
int32_t UPG_i32VerifyCPU0CodeByShareMemory(uint8_t* buffer, uint32_t size, uint8_t filetype, uint8_t optype);
int32_t UPG_i32VerifyCPUCodeFromBuffer(FIL* myfsrc, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype);   
int32_t UPG_i32VerifyTextureACF_Every2K(FIL* myfsrc);
int32_t UPG_i32VerifyTextureACF(FIL* myfsrc, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype);
int32_t UPG_i32VerifyTextureData(uint32_t startaddr, uint32_t filesize);
int32_t UPG_i32EraseCpu0Flash(uint32_t address, uint32_t size);
int32_t UPG_i32EraseCpu1Flash(uint32_t address, uint32_t size);     
int32_t UPG_i32WriteCpu1FlashFromBuffer(FIL* myfsrc, uint32_t FlashStartAddress, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype);
int32_t UPG_i32WriteTextureFlashFromBuffer(FIL* myfsrc, uint32_t FlashStartAddress, uint8_t* buffer, uint32_t bufsize, uint32_t filesize, uint8_t filetype, uint8_t optype);
int32_t UPG_i32WriteTextureFlashFromFile(FIL* myfsrc, uint32_t address);
int32_t UPG_i32SendDataByShareMemory(uint32_t address, uint32_t size, uint8_t* buffer);
int32_t UPG_i32GetVersionOnFlash(uint8_t u8Index, char** ppcVersion);
int32_t UPG_i32PrintVersionToFile(UPG_tstWorkBuf *pstWorkBuf);
bool UPG_bIfCpu1HasCode(void);
void UPG_vCbReadVersion(uint32_t* address);
uint32_t UPG_u32EraseTextureFlash(uint32_t address, uint32_t size);
uint32_t UPG_u32VerifyTextureData(uint32_t TextureSize);
uint32_t UPG_u32WriteTextureFlash(uint32_t address, uint8_t* pbuffer, uint32_t size);
uint32_t UPG_u32ReadTextureFlash(uint32_t address, uint8_t* pbuffer, uint32_t size);

uint32_t UPG_u32GetCpu1BinSize(void);
void UPG_vKeepCpu1BinSize(uint32_t size);

void UPG_vInit(void);
void UPG_vMain(void);

	 

#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_C1_H */




