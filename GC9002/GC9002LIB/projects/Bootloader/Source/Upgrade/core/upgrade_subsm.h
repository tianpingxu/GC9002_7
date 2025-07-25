/**
  ******************************************************************************
  * @file    upgrade_subsm.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    23-06-2021
  * @brief   The head file of Upgrade SUB State Machine.
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPGRADE_SUBSM_H
#define __UPGRADE_SUBSM_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#include "upgrade_ct.h"
#include "upgrade_ci.h"
//#include "upgrade_it.h"

#define CodeFlashSectorSize             (4096)
#define BUFFERMAXSIZE                   (256)

/* Synchronization with CFG, if interface changed, only need to redefine macro in .h file */
#define SYN_GET_SD_SPI_CHANEL                                 (CFG_pCfg2SpiChannel(CFG_Get_sd_spi_channel()))


typedef enum EraseState_Type
{
    ERASE_IDLE    = 0,      //There is no erase operaton.
    ERASE_ERASING,          //Erasing a flash.
    ERASE_DONE,             //Erase done.
    ERASE_ERROR             //Address error.
}EraseState;

typedef enum WriteState_Type
{
    WRITE_IDLE    = 0,      //There is no write operaton.
    WRITE_WRITING,          //writing a flash.
    WRITE_DONE,             //write done.
    WRITE_ERROR             //Address error.
}WriteState;

typedef enum ReadState_Type
{
    READ_IDLE    = 0,     	//There is no read operaton.
    READ_READING,         	//reading a flash.
    READ_DONE,            	//read done.
    READ_ERROR            	//Address error.
}ReadState;

typedef enum VerifyState_Type
{
    VERIFY_IDLE    = 0,     	//There is no read operaton.
    VERIFY_DOING,         	//reading a flash.
    VERIFY_DONE,            	//read done.
    VERIFY_ERROR            	//Address error.
}VerifyState;

void UPG_SUBSM__vGetComMode(UPG_tstWorkBuf *pstWorkBuf);
void UPG_SUBSM__vProcess(UPG_tstWorkBuf *pstWorkBuf);







	 

#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_SUBSM_H */




