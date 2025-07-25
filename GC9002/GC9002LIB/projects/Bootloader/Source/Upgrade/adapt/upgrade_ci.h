/**
  ******************************************************************************
  * @file    upgrade_ci.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    21-06-2021
  * @brief   The internal configuration head file of Upgrade module
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPGRADE_CI_H
#define __UPGRADE_CI_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "srv_timer_c1.h"
#include "em_c1.h"
#include "ParsePara.h"
#include "cfg_c1.h"

#ifndef OK
#define OK                                                  (0)
#endif 
#define UPG_USE_SD_CARD                                     (1)
#define UPG_USE_MSD                                         (1)
     
#if ((UPG_USE_SD_CARD != 1) && (UPG_USE_MSD != 1))
     #error At least one of UPG_USE_SD_CARD and UPG_USE_MSD must be equal to 1.
#endif

#define VERSION_LEN                                         (32)
	
#define SECOND                                              (1000)

/* related to Timer Service */
#define TIMER_WAIT_ERASE_DONE_INDEX                         SRV_TIMER_enTimerWaitEraseDone
/* wait erase done timer length: 60s */
#define TIMER_WAIT_ERASE_DONE_LEN                           (60 * SECOND)

#define TIMER_WAIT_WRITE_DONE_INDEX                         SRV_TIMER_enTimerWaitWriteDone
/* wait write done timer length: 100s */
#define TIMER_WAIT_WRITE_DONE_LEN                           (100 * SECOND)

#define TIMER_WAIT_VERIFY_DONE_INDEX                        SRV_TIMER_enTimerWaitVerifyDone
/* wait verify done timer length: 60s */
#define TIMER_WAIT_VERIFY_DONE_LEN                          (60 * SECOND)
	 
#define TIMER_WAIT_JUMP_DONE_INDEX                          SRV_TIMER_enTimerWaitJumpDone
/* wait Jump done timer length: 5000 ms */
#define TIMER_WAIT_JUMP_DONE_LEN                            (5000)


#define COMMON_BUFFER_ADDRESS                               (0x20180000)
#define COMMON_BUFFER_SIZE                                  (0x80000)


#define UPG_GET_MODE_TRY_TIMES                              (1)
#define UPG_GET_VERSION_TRY_TIMES                           (5)

/* Config detecting CAN & UART work try times */
#define UPG_CAN_UART_WORK_TRY_TIMES                         (CFG_Get_can_uart_wait_ms())

/* Config CPU1.bin default size: Max size(0x080F7000 - 0x08040000) */
#define CPU1_BIN_DEFAULT_SIZE                               (0xB7000)


#define ROM_CONFIG_BASE_ADDR      							((uint32_t)0x080F0000)

/* CPU0 related address configuration */
//#define CPU0_START_ADDRESS                                  (0x08020000)
//#define CPU0_VERSION_ADDRESS                                (0x08021000)
//#define CPU0_VERSION_OFFSET                                 (CPU0_VERSION_ADDRESS - CPU0_START_ADDRESS)



/* The max address of Texture Flash */
#define TEXTURE_FLASH_MAX_ADDR                              (0x08400000)

#define BOOTLOADER_SAVE_OFFSET                              (0x40000)
#define BOOTLOADER_SAVE_ADDR                                (TEXTURE_FLASH_MAX_ADDR - BOOTLOADER_SAVE_OFFSET)

#define BOOTLOADER_START_ADDR                               (0x08000000)

/* Max bootloader size need to subtrack head info size(16) */
#define BOOTLOADER_CODE_SIZE_MAX                            (0x40000 - 16)

/* CPU1 related address configuration */
#define CPU1_START_ADDRESS                                  (0x08040000)
#define CPU1_VERSION_ADDRESS                                (CPU1_START_ADDRESS)
#define CPU1_VERSION_OFFSET                                 (CPU1_VERSION_ADDRESS - CPU1_START_ADDRESS)
#define CPU1_CODE_SIZE_MAX                                 	(0xB7000)

#define CPU1_BOOT_VERSION_STORE_ADDR                        (0x08000100)
#define CPU1_BOOT_COMPILE_DATA_STORE_ADDR                   (0x08000120)
#define CPU1_BOOT_COMPILE_TIME_STORE_ADDR                   (0x08000140)

#define CPU1_APP_VERSION_STORE_ADDR                         (0x08040000)
#define CPU1_APP_COMPILE_DATA_STORE_ADDR                    (0x08040020)
#define CPU1_APP_COMPILE_TIME_STORE_ADDR                    (0x08040030)
#define CPU1_APP_COMPILE_ADDR_STORE_ADDR                    (0x08040040)


#define TEXTURE_SIZE_MAX                                 	(0x1F00000)
#define TEXTURE_START_ADDRESS                               (ROM_CONFIG_BASE_ADDR + 0x10000)
#define TEXTURE_FLASH_OFFSET                                (0x100000)

#define CONFIG_START_ADDRESS                                (ROM_CONFIG_BASE_ADDR + 0x8000)
/* upg_cfg.bin start burn address */
#define UPG_CFG_START_ADDRESS                               (ROM_CONFIG_BASE_ADDR + 0x7000)


#define BANK_WRITE_START_ADDR_STORETAG_PARA      			((uint32_t)ROM_CONFIG_BASE_ADDR + 0xD000)
#define BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA			((uint32_t)ROM_CONFIG_BASE_ADDR + 0xC000)

#define APP_FUNCTION_START_ADDR         					((uint32_t)0x08040060)
#define APP_FUNCTION_START_ADDR_HYPER         				((uint32_t)0x20000060)
/* Special start value in Flash/HyperRam */
#define APP_FLASH_START_VALUE                               ((uint32_t)0x18061197)
#define APP_JUMP_START_MASK                                 ((uint32_t)0xFF00001F)

#define APP_HYPER_START_VALUE                               ((uint32_t)0x000A1197)

#define START_ADDR_HYPER         				            ((uint32_t)0x20000000)


#define SIM_GEN_ALL_VERSION_STORE_ADDR      		        (TEXTURE_START_ADDRESS + 92)
#define IDE_VERSION_STORE_ADDR              		        (TEXTURE_START_ADDRESS + 124)
#define DEVICE_ID_STORE_ADDR                		        (TEXTURE_START_ADDRESS + 284)
#define SIM_GEN_ALL_VERSION_STROE_ADDR_FOR16      	        (TEXTURE_START_ADDRESS + 88)
#define IDE_VERSION_STORE_ADDR_FOR16              	        (TEXTURE_START_ADDRESS + 120)
#define DEVICE_ID_STORE_ADDR_FOR16                	        (TEXTURE_START_ADDRESS + 280)


/* The address must be in the area of No-init RAM */
#define CPU0_CAN_EXIST_FLAG                                 (0xA55AAA55)
#define CPU0_UART_EXIST_FLAG                                (0x5AA5AA55)
/****** Configure the Upgrade file information ********/
/* enUpgFileType,                   enUpgFlashType,             enVerifyInputType,      bFlagCheckVersion,  bFlagVerifyInputFile,   bFlagVerifyStoredFile,  bFlagVerifyUpdateProgress,   bFlagFixedRecvBufAddress,    bFlagUpgWithSelfUpg,   pu8UpgFlashStartAdress,                                  pu8RecvBufStartAddress,             u32RecvBufSize,          pu8FileName*/
#define UPG_PRECONFIG                                       \
   {UPG_enUpgFileBootloader,        UPG_enUpgFlashCpu1,         UPG_enVerifyInputInvalid,    UPG_FALSE,              UPG_FALSE,               UPG_FALSE,              UPG_FALSE,              UPG_FALSE,                UPG_TRUE,               (uint8_t*)(BOOTLOADER_SAVE_ADDR),                       (uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"bootloader.bin"},\
   {UPG_enUpgFileTexture,           UPG_enUpgFlashCpu1,         UPG_enVerifyInputXor,        UPG_FALSE,              UPG_TRUE,                UPG_TRUE,               UPG_FALSE,              UPG_FALSE,                UPG_FALSE,              (uint8_t*)(TEXTURE_START_ADDRESS),        				(uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"Texture.acf"},\
   {UPG_enUpgFileCpu1Bin,           UPG_enUpgFlashCpu1,         UPG_enVerifyInputCrc,        UPG_FALSE,              UPG_TRUE,                UPG_TRUE,               UPG_FALSE,              UPG_FALSE,                UPG_FALSE,              (uint8_t*)(CPU1_START_ADDRESS),             			(uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"cpu1.bin"},\
   {UPG_enUpgFileProduct,           UPG_enUpgFlashCpu1,         UPG_enVerifyInputInvalid,    UPG_FALSE,              UPG_FALSE,               UPG_FALSE,              UPG_FALSE,              UPG_FALSE,                UPG_TRUE,               (uint8_t*)(BANK_WRITE_START_ADDR_PRODUCT_PARA + 8),     (uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"product.bin"},\
   {UPG_enUpgFileCalibration,       UPG_enUpgFlashCpu1,         UPG_enVerifyInputInvalid,    UPG_FALSE,              UPG_FALSE,               UPG_FALSE,              UPG_FALSE,              UPG_FALSE,                UPG_FALSE,              (uint8_t*)(BANK_WRITE_START_ADDR_TOUCH_CALI),     		(uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"calibration.bin"},\
   {UPG_enUpgFileCpu0Cfg,           UPG_enUpgFlashCpu1,         UPG_enVerifyInputInvalid,    UPG_FALSE,              UPG_FALSE,               UPG_FALSE,              UPG_TRUE,               UPG_FALSE,                UPG_FALSE,              (uint8_t*)(CONFIG_START_ADDRESS),                       (uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"cpu0_config.bin"},\
   {UPG_enUpgFilefHWConfig,         UPG_enUpgFlashCpu1,         UPG_enVerifyInputInvalid,    UPG_FALSE,              UPG_FALSE,               UPG_FALSE,              UPG_FALSE,              UPG_FALSE,                UPG_FALSE,              (uint8_t*)(BANK_WRITE_START_ADDR_HWCONFIG + 8),         (uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"hwconfig.bin"},\
   {UPG_enUpgFileEraseStoreSpace,   UPG_enUpgFlashCpu1,         UPG_enVerifyInputInvalid,    UPG_FALSE,              UPG_FALSE,               UPG_FALSE,              UPG_FALSE,              UPG_FALSE,                UPG_FALSE,              (uint8_t*)(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA),  (uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"EraseStoreSpace.bin"},\
   {UPG_enUpgFileUpgCfg,            UPG_enUpgFlashCpu1,         UPG_enVerifyInputInvalid,    UPG_FALSE,              UPG_FALSE,               UPG_FALSE,              UPG_FALSE,              UPG_FALSE,                UPG_TRUE,               (uint8_t*)(UPG_CFG_START_ADDRESS),                      (uint8_t*)COMMON_BUFFER_ADDRESS,      COMMON_BUFFER_SIZE,       (uint8_t*)"upg_cfg.bin"}
/****** End of Configure the Upgrade file information ********/

#define UPG_MODULE_ID                                       (1)

#define SYN_ERR_LOG(ErrType, ErrLevel)                      EM_vLog(ErrType, ErrLevel, UPG_MODULE_ID, 0)
   
void UPG_Main_vInit(void);



#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_CI_H */




