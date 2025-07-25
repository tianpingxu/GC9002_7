/**
  ******************************************************************************
  * @file    em_ci.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    12-07-2021
  * @brief   The internal configuration head file of Error Management module.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EM_CI_H
#define __EM_CI_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

/* All errors in different modules need to be configured here , combined by enum and string */
#define EM_ALL_ERROR_INFO_TABLE \
    /* Err Type Enum,                       Error Information */\
    CFG(EM_enErrInParaInvalid,              "Input Parameter invalid")\
	CFG(EM_enErrTextureFlashInitFail,       "Init Texture Flash fail")\
	CFG(EM_enErrCPUSizeOverrun,         	"cpu1.bin size overrun")\
	CFG(EM_enErrTextureSizeOverrun,         "Texture.acf size overrun")\
    CFG(EM_enErrVerifyCodeCrcFail,          "Verify Code Crc fail")\
    CFG(EM_enErrVerifyTextureFail,          "Verify Texture fail")\
    CFG(EM_enErrUpgFileSizeInvalid,         "Upgrade File size invalid")\
    CFG(EM_enErrWrCPU0FlashFail,            "Write CPU0 Flash fail")\
    CFG(EM_enErrGetVerFromCPU1Fail,         "Get Version from CPU1 fail")\
	CFG(EM_enErrGetVerFromTextureFail,      "Get Version from Texture fail")\
    CFG(EM_enErrConfigParaInvalid,          "Config Parameter invalid")\
    CFG(EM_enErrReadVerFromFileFail,        "Read Version from file fail")\
    CFG(EM_enErrReadDataFromFileFail,       "Read data from file fail")\
    CFG(EM_enErrEraseFlashBySMFail,         "Erase Flash by ShareMem fail")\
	CFG(EM_enErrEraseCPU0FlashFail,         "Erase CPU0 Flash fail")\
    CFG(EM_enErrEraseCPU1FlashFail,         "Erase CPU1 Flash fail")\
	CFG(EM_enErrEraseTextureFlashFail,      "Erase Texture Flash fail")\
    CFG(EM_enErrEraseFlashTimeout,          "Erase Flash timeout")\
    CFG(EM_enErrSendDatabySMFail,           "Send data by Share Memory fail")\
    CFG(EM_enErrWriteFlashTimeout,          "Write Flash timeout")\
    CFG(EM_enErrVerifyFlashFail,            "Verify Flash fail")\
	CFG(EM_enErrVerifyFlashTimeout,         "Verify Flash timeout")\
    CFG(EM_enErrNoComForUpgrade,            "No Com(interface) for upgrading")\
    CFG(EM_enErrWriteTextureFlashFail,      "Write texture flash fail")\
	CFG(EM_enErrWriteCPU1FlashFail,         "Write CPU1 flash fail")\
    CFG(EM_enErrConvertFail,                "Check Dwin files format")\
    CFG(EM_enErrReadDirFail,                "Read directory fail")\
    CFG(EM_enErrOutOfRange,                 "Writing address overrun")\
    CFG(EM_enErrSameVersion,                "same version")\
    CFG(EM_enErrOpenVersionTxt,             "open version txt error")\
    CFG(EM_enErrNoAppCode,                  "dont have CPU1 in flash")\
    CFG(EM_enErrUnvalidFile,                "unvalid file name")\
    CFG(EM_enCreateEventGroupFail,          "Create event group Fail")\
    CFG(EM_enCreateTaskFail,                "Create task Fail")\
    CFG(EM_enNotInit,                       "Module has't initialized")\
	CFG(EM_enErrWriteFlashFail,             "Write Data to Flash fail")\
    CFG(EM_enErrInAddrInvalid,              "Input Address invalid")\
    CFG(EM_enErrCanInitFail,                "Can Init Fail")\
    CFG(EM_enErrUartInitFail,               "Uart Init Fail")\
    CFG(EM_enErrUpgradeSameVersion,         "Upgrade Same Version")

/* configured end */

#define EM_ERROR_COUNT_MAX                  (65535)

#ifdef __cplusplus
}
#endif

#endif /* __EM_CI_H */




