////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2021/07/08
// File Name:     em_c1.c
// Project Name:  CPU0_Bootloader_Redesign
// 
// Revision:
// Revision 1.0.0 - File Created 2021/07/08 by Zhu Jiahai
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "platform.h"
#include "AHMIInterface.h"
#include "FreeRTOS.h"
#include "task.h"

#ifdef PC_SIM
#include "PublicType.h"
#include "publicDefine.h"
#endif

#if defined(EMBEDDED) && defined(VERSION_2)
//#include "CPU1_CMSDK_CM0.h"
//#include "core_cm0.h"
#include "csi_hal_driver_flash.h"
#endif


#include "em_c1.h"

#ifndef CHIP_GC9002_NoHyper
const char * pau8ErrInfo[EM_enErrCount] = 
{
#define CFG(a, b)           b,
EM_ALL_ERROR_INFO_TABLE
#undef CFG
};

EM_tstErrItem astErrItem[EM_ERROR_MESSAGE_COUNT_MAX];
#endif

uint8_t error_write_flag = 0; // 1 for need write; 0 for don't need write
uint8_t error_write_pos = 0xFF;



/**
  * @name       EM_vInit
  * @brief      Initialization of Timer Service module
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-12
  * @date       2021-08-06     modify by Yu Ning
  */
void EM_vInit(void)
{	
	int i, full_flag = 1;
	#ifdef PC_SIM
	FILE *fp_temp;
	errno_t err;
	size_t count;

	memset(astErrItem, 0xFF, sizeof(EM_tstErrItem) * EM_ERROR_MESSAGE_COUNT_MAX);
	err = fopen_s(&fp_temp, AHMI_ERR_MESSAGE_FILE, "r");
	if (err == 0)
    {
		count = fread_s(astErrItem, sizeof(EM_tstErrItem) * EM_ERROR_MESSAGE_COUNT_MAX, sizeof(EM_tstErrItem) * EM_ERROR_MESSAGE_COUNT_MAX, 1, fp_temp);

		/* Close file. */
        fclose(fp_temp);
    }
    else
    {
        /* Open file "ErrorInfo.bin" fail */
    }
	#endif

	#ifndef CHIP_GC9002_NoHyper
	#if defined(EMBEDDED) && defined(VERSION_2)
	csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_ERRMESSAGE_PARA, sizeof(EM_tstErrItem) * EM_ERROR_MESSAGE_COUNT_MAX, (uint8_t *)astErrItem);
	#endif

	for(i = 0; i < EM_ERROR_MESSAGE_COUNT_MAX; i++)
	{
		if((EM_enErrLevelNoErr >= astErrItem[i].u16ErrLevel) || (EM_enErrLevelRecover < astErrItem[i].u16ErrLevel))
		{
			astErrItem[i].u16OccurCount = 0;
			full_flag = 0;
		}
	}

	if(full_flag)
	{
		error_write_pos = 0;
	}
	#endif
}




/**
  * @name       EM_vLog
  * @brief      Log function of Error Management Module
  * @note       If ErrType is invalid, it will do nothing 
  * @param      u16ErrType: error type
  * @param      u8ErrLevel: error level
  * @param      u8ModuleId: module ID
  * @param      u32ExtroInfo: extro information of the error
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-12
  * @date       2021-08-06     modify by Yu Ning
  */
void EM_vLog(uint16_t u16ErrType, uint16_t u16ErrLevel, uint16_t u16PageID, uint16_t u16CanvasID, uint16_t u16SubcanvasID, uint16_t u16WidgetID, uint16_t u16TagID, uint32_t u32TagValue)
{
	#ifndef CHIP_GC9002_NoHyper
	int i = 0;

    if(u16ErrType >= EM_enErrCount)
    {
        return;
    }
    
    taskENTER_CRITICAL();

	for(i = 0; i < EM_ERROR_MESSAGE_COUNT_MAX; i++)
	{
		if(astErrItem[i].u16ErrType == u16ErrType && 
			astErrItem[i].u16ErrLevel == u16ErrLevel && 
			astErrItem[i].u16PageID == u16PageID && 
			astErrItem[i].u16CanvasID == u16CanvasID && 
			astErrItem[i].u16SubcanvasID == u16SubcanvasID && 
			astErrItem[i].u16WidgetID == u16WidgetID && 
			astErrItem[i].u16TagID == u16TagID)
		{
			astErrItem[i].u32TagValue = u32TagValue;
			if(astErrItem[i].u16OccurCount < EM_ERROR_COUNT_MAX)
			{
				astErrItem[i].u16OccurCount++;
			}
			error_write_flag = 1;
			taskEXIT_CRITICAL();
			return;
		}

		if(error_write_pos == 0xFF)
		{
			if((EM_enErrLevelNoErr >= astErrItem[i].u16ErrLevel) || (EM_enErrLevelRecover < astErrItem[i].u16ErrLevel))
			{
				astErrItem[i].u16ErrType = u16ErrType;
				astErrItem[i].u16ErrLevel = u16ErrLevel;
				astErrItem[i].u16PageID = u16PageID;
				astErrItem[i].u16CanvasID = u16CanvasID;
				astErrItem[i].u16SubcanvasID = u16SubcanvasID;
				astErrItem[i].u16WidgetID = u16WidgetID;
				astErrItem[i].u16TagID = u16TagID;
				astErrItem[i].u32TagValue = u32TagValue;
				if(astErrItem[i].u16OccurCount < EM_ERROR_COUNT_MAX)
				{
					astErrItem[i].u16OccurCount++;
				}
				error_write_flag = 1;
				taskEXIT_CRITICAL();
				return;
			}
		}
	}

	if(error_write_pos == 0xFF)
	{
		error_write_pos = 0;
	}

	astErrItem[error_write_pos].u16ErrType = u16ErrType;
	astErrItem[error_write_pos].u16ErrLevel = u16ErrLevel;
	astErrItem[error_write_pos].u16PageID = u16PageID;
	astErrItem[error_write_pos].u16CanvasID = u16CanvasID;
	astErrItem[error_write_pos].u16SubcanvasID = u16SubcanvasID;
	astErrItem[error_write_pos].u16WidgetID = u16WidgetID;
	astErrItem[error_write_pos].u16TagID = u16TagID;
	astErrItem[error_write_pos].u32TagValue = u32TagValue;
	if(astErrItem[error_write_pos].u16OccurCount < EM_ERROR_COUNT_MAX)
	{
		astErrItem[error_write_pos].u16OccurCount++;
	}
	error_write_pos++;
	error_write_flag = 1;

	if(error_write_pos >= EM_ERROR_MESSAGE_COUNT_MAX)
	{
		error_write_pos = 0;
	}
	taskEXIT_CRITICAL();
	#endif
}




/**
  * @name       EV_pu8GetErrDescription
  * @brief      Get the error description of the specified error type
  * @note       
  * @param      u16ErrType: error type
  * @retval     it will return NULL while u16ErrType is invalid, else return the address of the error information
  * @author     Zhu Jiahai
  * @date       2021-07-13
  * @date       2021-08-06     modify by Yu Ning
  */
uint8_t* EM_pu8GetErrDescription(uint16_t u16ErrType)
{
	#ifndef CHIP_GC9002_NoHyper
    if(u16ErrType >= EM_enErrCount)
    {
        return NULL;
    }
    else
    {

        return (uint8_t*)pau8ErrInfo[u16ErrType];
    }
	#else
	return NULL;
	#endif
}




/**
  * @name       EM_bIfErrorExisted
  * @brief      Inquire if error exist
  * @note       
  * @param      none
  * @retval     TRUE: exist error, FALSE: no error
  * @author     Zhu Jiahai
  * @date       2021-07-13
  * @date       2021-08-06     modify by Yu Ning
  */
EM_tenErrBool EM_bIfErrorExisted(void)
{
    int32_t i;

    for(i = 0; i < EM_enErrCount; i++)
    {
        //if((EM_enErrLevelNoErr != astErrItem[i].u8ErrLevel) && (0 != astErrItem[i].u16OccurCount))
        {
            break;
        }
    }

    if(i < EM_enErrCount)
    {
        return TRUE_1;
    }
    else
    {
        return FALSE_0;
    }
}


#include "csi_kernel.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "semphr.h"
//extern k_sem_handle_t flashSemaphore;

/**
  * @name       EM_vPrintErrorInfoToFile
  * @brief      Print error information to File
  * @note       
  * @param      fp: File pointer
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-13
  * @date       2021-08-06     modify by Yu Ning
  */
void EM_vPrintErrorInfoToFile(void)
{
	#ifndef CHIP_GC9002_NoHyper
	#ifdef PC_SIM
	FILE *fp_errorlog;
    errno_t err;
    uint32_t i, err_num = 0;
	uint8_t* srcAddr = 0;

	/* Find the special file by name for version code. */
    err = fopen_s(&fp_errorlog, AHMI_ERR_MESSAGE_FILE, "wt+");
    if (err == 0)
    {
		if(error_write_pos == 0xFF)
		{
			for(i = 0; i < EM_ERROR_MESSAGE_COUNT_MAX; i++)
			{
			    if((EM_enErrLevelNoErr >= astErrItem[i].u16ErrLevel) || (EM_enErrLevelRecover < astErrItem[i].u16ErrLevel))
			    {
					err_num = i;
					break;
			    }
			}
		}
		else
		{
			err_num = EM_ERROR_MESSAGE_COUNT_MAX;
		}

		srcAddr = (uint8_t*)astErrItem;

		for(i = 0; i < err_num * sizeof(EM_tstErrItem); i++)
		{
			fputc(*srcAddr++,fp_errorlog);
		}

        /* Close file. */
        fclose(fp_errorlog);
		error_write_flag = 0;
    }
    else
    {
        /* Open file "ErrorInfo.bin" fail */
    }
    #endif

	#if defined(EMBEDDED) && defined(VERSION_2)
	uint32_t i, err_num = 0;
	uint8_t* srcAddr = 0;

	if(error_write_pos == 0xFF)
	{
		for(i = 0; i < EM_ERROR_MESSAGE_COUNT_MAX; i++)
		{
			if((EM_enErrLevelNoErr >= astErrItem[i].u16ErrLevel) || (EM_enErrLevelRecover < astErrItem[i].u16ErrLevel))
			{
				err_num = i;
				break;
			}
		}
	}
	else
	{
		err_num = EM_ERROR_MESSAGE_COUNT_MAX;
	}

	srcAddr = (uint8_t*)astErrItem;
	
//    if(flashSemaphore != NULL)
//    {
//        xSemaphoreTake(flashSemaphore, portMAX_DELAY);
//    }
	
	csi_hal_internal_flash_take(0);
	
	csi_hal_internal_flash_ClearWriteProtect();
	csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_ERRMESSAGE_PARA);
	csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_ERRMESSAGE_PARA, err_num * sizeof(EM_tstErrItem), (uint8_t *)srcAddr);
	csi_hal_internal_flash_SetWriteProtect();
	
//    if(flashSemaphore != NULL)
//    {
//        xSemaphoreGive(flashSemaphore);
//    }
	
	csi_hal_internal_flash_give(0);
	
	error_write_flag = 0;
	#endif
    return;
	#endif
}

//-----------------------------
// name： ErrorManagementTask
// 异常信息存储函数
//  
//    @param1 void* pvParameters     消息队列中的参数
// addtional comments :
//-----------------------------
extern void vTaskDelay( const TickType_t xTicksToDelay );
void ErrorManagementTask(void)
{
 	//for(;;)
 	//{
		if(error_write_flag)
		{
			EM_vPrintErrorInfoToFile();
		}
		//vTaskDelay( 5000 / portTICK_RATE_MS );
 	//}
}
