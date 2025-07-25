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
//#include "CPU1_CMSDK_CM0.h"
//#include "core_cm0.h"
#include "DrawProgramBar.h"

#include "upgrade_c1.h"
#include "em_c1.h"

const uint8_t * const pau8ErrInfo[EM_enErrCount] = 
{
#define CFG(a, b)           (const uint8_t*)(b),
EM_ALL_ERROR_INFO_TABLE
#undef CFG
};

EM_tstErrItem astErrItem[EM_enErrCount];






/**
  * @name       EM_vInit
  * @brief      Initialization of Timer Service module
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-12
  */
void EM_vInit(void)
{
    memset(astErrItem, 0, sizeof(astErrItem));
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
  */
void EM_vLog(uint16_t u16ErrType, uint8_t u8ErrLevel, uint8_t u8ModuleId, uint32_t u32ExtroInfo)
{
	DrawErrorBackground(SCREEN_WIDTH, SCREEN_HEIGHT, u8ErrLevel, pau8ErrInfo[u16ErrType]);
	
    if(u16ErrType >= EM_enErrCount)
    {
        return;
    }

    if(EM_enErrLevelRecover == u8ErrLevel)
    {
        if(astErrItem[u16ErrType].u16OccurCount > 0)
        {
            astErrItem[u16ErrType].u16OccurCount = 0;
        }
        astErrItem[u16ErrType].u8ErrLevel = EM_enErrLevelNoErr;
    }
    else if(EM_enErrLevelNoErr != u8ErrLevel)
    {
        if(astErrItem[u16ErrType].u16OccurCount < EM_ERROR_COUNT_MAX)
        {
             astErrItem[u16ErrType].u16OccurCount++;
        }   
        astErrItem[u16ErrType].u8ErrLevel = u8ErrLevel;
    }

    
    astErrItem[u16ErrType].u8ModuleIndex = u8ModuleId;
    astErrItem[u16ErrType].u32ExtraInfo = u32ExtroInfo;
}




/**
  * @name       EV_pu8GetErrDescription
  * @brief      Get the error description of the specified error type
  * @note       
  * @param      u16ErrType: error type
  * @retval     it will return NULL while u16ErrType is invalid, else return the address of the error information
  * @author     Zhu Jiahai
  * @date       2021-07-13
  */
uint8_t* EM_pu8GetErrDescription(uint16_t u16ErrType)
{
    if(u16ErrType >= EM_enErrCount)
    {
        return NULL;
    }
    else
    {

        return (uint8_t*)pau8ErrInfo[u16ErrType];
    }
}




/**
  * @name       EM_bIfErrorExisted
  * @brief      Inquire if error exist
  * @note       
  * @param      none
  * @retval     TRUE: exist error, FALSE: no error
  * @author     Zhu Jiahai
  * @date       2021-07-13
  */
bool EM_bIfErrorExisted(void)
{
    int32_t i;

    for(i = 0; i < EM_enErrCount; i++)
    {
        if((EM_enErrLevelNoErr != astErrItem[i].u8ErrLevel) && (0 != astErrItem[i].u16OccurCount))
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



/**
  * @name       EM_vPrintErrorInfoToFile
  * @brief      Print error information to File
  * @note       
  * @param      fp: File pointer
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-13
  */
void EM_vPrintErrorInfoToFile(FIL* fp)
{
    FRESULT res;
    int32_t i;
    bool bWriteEable = FALSE;

    if(NULL == fp)
    {
        /* Input parameter error */
        return;
    }


    /* Find the special file by name for version code. */
    res = f_open(fp, "1:ErrorInfo.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if(res != FR_OK)
    {
        res = f_open(fp, "2:ErrorInfo.txt", FA_CREATE_ALWAYS | FA_WRITE);
        if(res == FR_OK)
        {
            bWriteEable = TRUE;
        }
            
    }
    else
    {
        bWriteEable = TRUE;
    }
    
    if (bWriteEable)
    {
        UINT bw;
        char tmp_buf[128];
        uint8_t* pu8ErrorDesc;


        for(i = 0; i < EM_enErrCount; i++)
        {
            if((EM_enErrLevelNoErr != astErrItem[i].u8ErrLevel) && (0 != astErrItem[i].u16OccurCount))
            {
                memset(tmp_buf, 0, sizeof(tmp_buf));
                pu8ErrorDesc = EM_pu8GetErrDescription((uint16_t)i);
                
                snprintf(tmp_buf, sizeof(tmp_buf), "Error Type:%4d, Error Level:%4d, Occur Count:%6d, Error Description: %s\r\n",
                 i, astErrItem[i].u8ErrLevel, astErrItem[i].u16OccurCount, pu8ErrorDesc);
                f_write(fp, tmp_buf, strlen(tmp_buf), &bw);
            }
        }


        /* Close file. */
        f_close(fp);
    }
    else
    {
        /* Open file "ErrorInfo.txt" fail */
    }
    
    return;
}


