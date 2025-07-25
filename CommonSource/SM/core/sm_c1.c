////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2021/09/06
// File Name:     sm_c1.c
// 
// Revision:
// Revision 1.0.0 - File Created 2021/09/06 by Yu Ning
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "platform.h"
#include "AHMIInterface.h"
#include "AHMICfgDefinition.h"

#ifdef PC_SIM
#include "PublicType.h"
#include "publicDefine.h"
#endif

#if defined(EMBEDDED) && defined(VERSION_2)
//#include "CPU1_CMSDK_CM0.h"
//#include "core_cm0.h"
#include "csi_hal_driver_flash.h"
#endif


#include "sm_c1.h"

#ifndef CHIP_GC9002_NoHyper
SM_tstStoreItem astStoreItem[MaxStoreTagNum];
uint32_t StoreStringTagArry[MaxStoreStringTagWordSize] = {0};

#include "csi_kernel.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "semphr.h"
//extern k_sem_handle_t flashSemaphore;
#endif



/**
  * @name       SM_vInit
  * @brief      Initialization of Store Management
  * @note        
  * @param      none
  * @retval     none
  * @author     Yu Ning
  * @date       2021-09-06
  */
void SM_vInit(void)
{
    #ifndef CHIP_GC9002_NoHyper
    #if defined(EMBEDDED) && defined(VERSION_2)
    uint32_t StoreTagValueFlag = AHMI_CPU1_VERSION;
    uint32_t flagReadFromFlash = 0;

    csi_hal_internal_flash_read(BANK_WRITE_AND_ADDR_STORETAG_PARA,4,(uint8_t *)&flagReadFromFlash);

    /* Erase store number tag space (0x0807d000~0x0807dfff) if need */
    //if(*(uint32_t *)BANK_WRITE_AND_ADDR_STORETAG_PARA != StoreTagValueFlag)
    if(flagReadFromFlash != StoreTagValueFlag)
    {
        csi_hal_internal_flash_ClearWriteProtect();
        csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORETAG_PARA);
        csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORETAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
        csi_hal_internal_flash_SetWriteProtect();
    }

    csi_hal_internal_flash_read(BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA,4,(uint8_t *)&flagReadFromFlash);

    /* Erase store string tag space (0x0807c000~0x0807cfff) if need */
    //if(*(uint32_t *)BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA != StoreTagValueFlag)
    if(flagReadFromFlash != StoreTagValueFlag)
    {
        csi_hal_internal_flash_ClearWriteProtect();
        csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA);
        csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
        csi_hal_internal_flash_SetWriteProtect();
    }
    #endif

    memset(astStoreItem, 0xFF, sizeof(SM_tstStoreItem) * MaxStoreTagNum);
    SM_vUpgrade(StoreNumberTag);

    memset(StoreStringTagArry, 0xFF, sizeof(uint32_t) * MaxStoreStringTagWordSize);
    SM_vUpgrade(StoreStringTag);
    #endif
}

/**
  * @name       SM_vClear
  * @brief      Clear tag data to file(PC_SIM) or Flash(EMBEDDED & VERSION_2)
  * @note        
  * @param      TagType: tag's type(StoreNumberTag or StoreStringTag)
  * @retval     none
  * @author     Yu Ning
  * @date       2021-09-27
  */
void SM_vClear(uint8_t TagType)
{
	#ifndef CHIP_GC9002_NoHyper
	#ifdef PC_SIM
	FILE *fp_storedata;
	errno_t err;
	
	if(TagType == StoreNumberTag)
	{
		/* Find the special file by name for version code. */
		err = fopen_s(&fp_storedata, AHMI_NUM_TAG_FILE, "w+");
		
		/* Close file. */
		fclose(fp_storedata);
	}
	else if(TagType == StoreStringTag)
	{
		/* Find the special file by name for version code. */
		err = fopen_s(&fp_storedata, AHMI_STR_TAG_FILE, "w+");
		
		/* Close file. */
		fclose(fp_storedata);
	}
	#endif
	
	#if defined(EMBEDDED) && defined(VERSION_2)
	uint32_t StoreTagValueFlag = AHMI_CPU1_VERSION;
	
//    if(flashSemaphore != NULL)
//    {
//        xSemaphoreTake(flashSemaphore, portMAX_DELAY);
//    }
	
	csi_hal_internal_flash_take(0);
	
	if(TagType == StoreNumberTag)
	{
		csi_hal_internal_flash_ClearWriteProtect();
		csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORETAG_PARA);
		csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORETAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
		csi_hal_internal_flash_SetWriteProtect();
	}
	else if(TagType == StoreStringTag)
	{
		csi_hal_internal_flash_ClearWriteProtect();
		csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA);
		csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
		csi_hal_internal_flash_SetWriteProtect();
	}
	
//    if(flashSemaphore != NULL)
//    {
//       xSemaphoreGive(flashSemaphore);
//    }

	csi_hal_internal_flash_give(0);

	#endif
	#endif
}

/**
  * @name       SM_vUpgrade
  * @brief      Upgrade tag data from file(PC_SIM) or Flash(EMBEDDED & VERSION_2)
  * @note        
  * @param      TagType: tag's type(StoreNumberTag or StoreStringTag)
  * @retval     none
  * @author     Yu Ning
  * @date       2021-09-06
  */
void SM_vUpgrade(uint8_t TagType)
{
	#ifndef CHIP_GC9002_NoHyper
	SM_tstStoreItem temp_StoreItem = {0xFFFFFFFF,0xFFFFFFFF};
	uint32_t i = 0, j = 0, k = 0;
	uint32_t temp = 0xFFFFFFFF;
	uint16_t currentStringTagLength = 0;
	uint16_t currentStringRegID = 0;
	uint16_t currentStringPos = 0;
	uint16_t currentStringTagLengthInBuf = 0;
	uint16_t currentStringRegIDInBuf = 0;
	uint16_t currentStringPosInBuf = 0;

	#ifdef PC_SIM
	FILE *fp_temp;
	errno_t err;
	u32 count = 0;

	if(TagType == StoreNumberTag)
	{
		err = fopen_s(&fp_temp, AHMI_NUM_TAG_FILE, "rb+");
		if (err == 0)
		{
			for(i = 0; i < MaxStoreTagNum; i++)
			{
				count = fread_s(&temp_StoreItem, sizeof(SM_tstStoreItem), sizeof(SM_tstStoreItem), 1, fp_temp);

				if(temp_StoreItem.u32RegID != 0xFFFFFFFF)
				{
					if((temp_StoreItem.u32RegID & 0xFFFF0000) == 0)
					{
						for(j = 0; j < MaxStoreTagNum; j++)
						{
							if(astStoreItem[j].u32RegID == temp_StoreItem.u32RegID)
							{
								astStoreItem[j].u32TagValue = temp_StoreItem.u32TagValue;
								break;
							}
							else if(astStoreItem[j].u32RegID == 0xFFFFFFFF)
							{
								astStoreItem[j].u32RegID = temp_StoreItem.u32RegID;
								astStoreItem[j].u32TagValue = temp_StoreItem.u32TagValue;
								break;
							}
						}
					}
					else
					{
						/* Close file. */
						fclose(fp_temp);
						err = 1;

						/* Find the special file by name for version code. */
						err = fopen_s(&fp_temp, AHMI_NUM_TAG_FILE, "w+");

						break;
					}
				}
				else
				{
					break;
				}
			}
			/* Close file. */
		    fclose(fp_temp);
		}
		else
		{
		    /* Open file "NumberTagInfo.bin" fail */
		}
	}
	else if(TagType == StoreStringTag)
	{
		err = fopen_s(&fp_temp, AHMI_STR_TAG_FILE, "rb+");
		if (err == 0)
		{
			for(i = 0; i < MaxStoreStringTagWordSize;)
			{
				fseek(fp_temp, sizeof(u32)*i, SEEK_SET);
				count = fread_s(&temp, sizeof(u32), sizeof(u32), 1, fp_temp);

				if(count != 0)
				{
					currentStringRegID = (temp & 0xFFFF);
					currentStringTagLength = ((temp & 0xFFFF0000) >> 16);
					currentStringPos = (i + 1);

					if((currentStringTagLength != 0xFFFF) && ((currentStringTagLength == 0) || (currentStringTagLength > 480) || (currentStringTagLength % 4 != 0)))
					{
						/* Close file. */
						fclose(fp_temp);
						err = 1;

						/* Find the special file by name for version code. */
						err = fopen_s(&fp_temp, AHMI_STR_TAG_FILE, "w+");

						break;
					}

					for(j = 0; j < MaxStoreStringTagWordSize;)
					{
						temp = StoreStringTagArry[j];

						currentStringRegIDInBuf = (temp & 0xFFFF);
						currentStringTagLengthInBuf = ((temp & 0xFFFF0000) >> 16);
						currentStringPosInBuf = (j + 1);

						if((currentStringRegIDInBuf == currentStringRegID) && (currentStringTagLengthInBuf == currentStringTagLength))
						{
							for(k = 0; k < currentStringTagLength/sizeof(u32); k++)
							{
								count = fread_s(&temp, sizeof(u32), sizeof(u32), 1, fp_temp);
								if(count != 0)
								{
									StoreStringTagArry[currentStringPosInBuf + k] = temp;
								}
							}
							break;
						}
						else if((currentStringRegIDInBuf == 0xFFFF) && (currentStringTagLengthInBuf == 0xFFFF))
						{
							StoreStringTagArry[currentStringPosInBuf - 1] = (((u32)currentStringTagLength << 16) | currentStringRegID);
							for(k = 0; k < currentStringTagLength/sizeof(u32); k++)
							{
								count = fread_s(&temp, sizeof(u32), sizeof(u32), 1, fp_temp);
								if(count != 0)
								{
									StoreStringTagArry[currentStringPosInBuf + k] = temp;
								}
							}
							break;
						}
						j = (j + 1 + currentStringTagLengthInBuf/4);
					}

					i = (i + 1 + currentStringTagLength/4);
				}
				else
				{
					currentStringRegID = 0xFFFF;
				}

				if(currentStringRegID == 0xFFFF)
				{
					break;
				}
			}
			/* Close file. */
			fclose(fp_temp);
		}
	}
	#endif

	#if defined(EMBEDDED) && defined(VERSION_2)
	uint32_t StoreTagValueFlag = AHMI_CPU1_VERSION;
	
	if(TagType == StoreNumberTag)
	{
		for(i = 0; i < MaxStoreTagNum; i++)
		{
			csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORETAG_PARA + i * sizeof(SM_tstStoreItem), sizeof(SM_tstStoreItem), (uint8_t *)&temp_StoreItem);

			if(temp_StoreItem.u32RegID != 0xFFFFFFFF)
			{
				if((temp_StoreItem.u32RegID & 0xFFFF0000) == 0)
				{
					for(j = 0; j < MaxStoreTagNum; j++)
					{
						if(astStoreItem[j].u32RegID == temp_StoreItem.u32RegID)
						{
							astStoreItem[j].u32TagValue = temp_StoreItem.u32TagValue;
							break;
						}
						else if(astStoreItem[j].u32RegID == 0xFFFFFFFF)
						{
							astStoreItem[j].u32RegID = temp_StoreItem.u32RegID;
							astStoreItem[j].u32TagValue = temp_StoreItem.u32TagValue;
							break;
						}
					}
				}
				else
				{
					/* data invalid, need erase flash */
					csi_hal_internal_flash_ClearWriteProtect();
					csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORETAG_PARA);
					csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORETAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
					csi_hal_internal_flash_SetWriteProtect();
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else if(TagType == StoreStringTag)
	{
		for(i = 0; i < MaxStoreStringTagWordSize;)
		{
			csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + i * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);

			currentStringRegID = (temp & 0xFFFF);
			currentStringTagLength = ((temp & 0xFFFF0000) >> 16);
			currentStringPos = (i + 1);

			if((currentStringTagLength != 0xFFFF) && ((currentStringTagLength == 0) || (currentStringTagLength > 480) || (currentStringTagLength % 4 != 0)))
			{
				/* data invalid, need erase flash */
				csi_hal_internal_flash_ClearWriteProtect();
				csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA);
				csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
				csi_hal_internal_flash_SetWriteProtect();
				break;
			}

			if((currentStringRegID != 0xFFFF) && (currentStringTagLength != 0xFFFF))
			{
				for(j = 0; j < MaxStoreStringTagWordSize;)
				{
					temp = StoreStringTagArry[j];

					currentStringRegIDInBuf = (temp & 0xFFFF);
					currentStringTagLengthInBuf = ((temp & 0xFFFF0000) >> 16);
					currentStringPosInBuf = (j + 1);

					if((currentStringRegIDInBuf == currentStringRegID) && (currentStringTagLengthInBuf == currentStringTagLength))
					{
						for(k = 0; k < currentStringTagLength/sizeof(uint32_t); k++)
						{
							csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + (currentStringPos + k) * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);
							StoreStringTagArry[currentStringPosInBuf + k] = temp;
						}
						break;
					}
					else if((currentStringRegIDInBuf == 0xFFFF) && (currentStringTagLengthInBuf == 0xFFFF))
					{
						StoreStringTagArry[currentStringPosInBuf - 1] = (((uint32_t)currentStringTagLength << 16) | currentStringRegID);
						for(k = 0; k < currentStringTagLength/sizeof(uint32_t); k++)
						{
							csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + (currentStringPos + k) * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);
							StoreStringTagArry[currentStringPosInBuf + k] = temp;
						}
						break;
					}
					j = (j + 1 + currentStringTagLengthInBuf/4);
				}
				i = (i + 1 + currentStringTagLength/4);
			}
			else
			{
				break;
			}
		}
	}
	#endif
	#endif
}

/**
  * @name       SM_vStoreNumberTag
  * @brief      Store number tag data to file(PC_SIM) or Flash(EMBEDDED & VERSION_2)
  * @note        
  * @param      u32RegID: tag's reg ID
  * @param      u32TagValue: tag Value
  * @retval     none
  * @author     Yu Ning
  * @date       2021-09-06
  */
void SM_vStoreNumberTag(uint32_t u32RegID, uint32_t u32TagValue)
{
	#ifndef CHIP_GC9002_NoHyper
	SM_tstStoreItem temp_StoreItem = {0xFFFFFFFF,0xFFFFFFFF};
	uint32_t i;
	
	#ifdef PC_SIM
	FILE *fp_storedata;
    errno_t err;
    uint32_t file_size, j, count = 0;
	uint8_t* srcAddr = 0;

	/* Find the special file by name for version code. */
	err = fopen_s(&fp_storedata, AHMI_NUM_TAG_FILE, "ab+");
	if (err == 0)
	{
		/* get store number tag data size. */
		fseek(fp_storedata,0,SEEK_END);
		file_size = ftell(fp_storedata);

		if(file_size / 8 < MaxStoreTagNum)
		{
			/* write store number tag data to file. */
			temp_StoreItem.u32RegID = u32RegID;
			temp_StoreItem.u32TagValue = u32TagValue;

			srcAddr = (uint8_t *)&temp_StoreItem;
			for(i = 0; i < 8; i++)
			{
				fputc(*srcAddr++, fp_storedata);
			}
		}
		else
		{
			/* read all store number tag data to astStoreItem. */
			fseek(fp_storedata,0,SEEK_SET);
			for(i = 0; i < MaxStoreTagNum; i++)
			{
				count = fread_s(&temp_StoreItem, sizeof(SM_tstStoreItem), sizeof(SM_tstStoreItem), 1, fp_storedata);

				if(temp_StoreItem.u32RegID != 0xFFFFFFFF)
				{
					for(j = 0; j < MaxStoreTagNum; j++)
					{
						if(astStoreItem[j].u32RegID == temp_StoreItem.u32RegID)
						{
							astStoreItem[j].u32TagValue = temp_StoreItem.u32TagValue;
							break;
						}
						else if(astStoreItem[j].u32RegID == 0xFFFFFFFF)
						{
							astStoreItem[j].u32RegID = temp_StoreItem.u32RegID;
							astStoreItem[j].u32TagValue = temp_StoreItem.u32TagValue;
							break;
						}
					}
				}
			}
			/* Close file. */
			fclose(fp_storedata);
			err = 1;

			/* Find the special file by name for version code. */
			err = fopen_s(&fp_storedata, AHMI_NUM_TAG_FILE, "w+");

			if(err == 0)
			{
				/* put u32TagID number tag data to astStoreItem. */
				temp_StoreItem.u32RegID = u32RegID;
				temp_StoreItem.u32TagValue = u32TagValue;
				for(i = 0; i < MaxStoreTagNum; i++)
				{
					if(astStoreItem[i].u32RegID == 0xFFFFFFFF)
					{
						astStoreItem[i].u32RegID = temp_StoreItem.u32RegID;
						astStoreItem[i].u32TagValue = temp_StoreItem.u32TagValue;
						break;
					}
					else
					{
						/* There is no space for store tag value */
					}
				}

				/* write astStoreItem to file. */
				fseek(fp_storedata,0,SEEK_SET);
				srcAddr = (uint8_t *)&astStoreItem;
				for(j = 0; j < (8 * (i + 1)); j++)
				{
					fputc(*srcAddr++, fp_storedata);
				}
			}
		}

	    /* Close file. */
	    fclose(fp_storedata);
	}
	else
	{
	    /* Open file "NumberTagInfo.bin" fail */
	}
    #endif

	#if defined(EMBEDDED) && defined(VERSION_2)
	uint32_t StoreTagValueFlag = AHMI_CPU1_VERSION;
	uint8_t findflag = 0;
	
	/* get store number tag data size. */
	for(i = 0; i < MaxStoreTagNum; i++)
	{
//	    if(flashSemaphore != NULL)
//	    {
//	        xSemaphoreTake(flashSemaphore, portMAX_DELAY);
//	    }

		csi_hal_internal_flash_take(0);

	    csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORETAG_PARA + i * sizeof(SM_tstStoreItem), sizeof(SM_tstStoreItem), (uint8_t *)&temp_StoreItem);
		
//	    if(flashSemaphore != NULL)
//	    {
//	       xSemaphoreGive(flashSemaphore);
//	    }
		
		csi_hal_internal_flash_give(0);

		if(temp_StoreItem.u32RegID == 0xFFFFFFFF)
			break;
	}

	if(i < MaxStoreTagNum)
	{
		/* write store number tag data to flash. */
		temp_StoreItem.u32RegID = u32RegID;
		temp_StoreItem.u32TagValue = u32TagValue;
		
//		if(flashSemaphore != NULL)
//		{
//		    xSemaphoreTake(flashSemaphore, portMAX_DELAY);
//		}
		
		csi_hal_internal_flash_take(0);

		csi_hal_internal_flash_ClearWriteProtect();
		csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_STORETAG_PARA + i * sizeof(SM_tstStoreItem), sizeof(SM_tstStoreItem), (uint8_t *)&temp_StoreItem);
		csi_hal_internal_flash_SetWriteProtect();

//		if(flashSemaphore != NULL)
//		{
//		    xSemaphoreGive(flashSemaphore);
//		}
		
		csi_hal_internal_flash_give(0);
	}
	else
	{
		csi_hal_internal_flash_take(0);
		
		/* read all store number tag data to astStoreItem. */
		SM_vUpgrade(StoreNumberTag);
		
		csi_hal_internal_flash_give(0);

		/* put u32TagID number tag data to astStoreItem. */
		temp_StoreItem.u32RegID = u32RegID;
		temp_StoreItem.u32TagValue = u32TagValue;

		for(i = 0; i < MaxStoreTagNum; i++)
		{
			if(astStoreItem[i].u32RegID == temp_StoreItem.u32RegID)
			{
				astStoreItem[i].u32TagValue = temp_StoreItem.u32TagValue;
				findflag = 1;
			}
			else if(astStoreItem[i].u32RegID == 0xFFFFFFFF)
			{
				if(findflag)
				{
					i--;
					break;
				}
				
				astStoreItem[i].u32RegID = temp_StoreItem.u32RegID;
				astStoreItem[i].u32TagValue = temp_StoreItem.u32TagValue;
				findflag = 1;
				break;
			}
			else
			{
				/* There is no space for store tag value */
			}
		}

		/* write astStoreItem to flash. */
		if(findflag && (i < MaxStoreTagNum))
		{
//		    if(flashSemaphore != NULL)
//		    {
//		       xSemaphoreTake(flashSemaphore, portMAX_DELAY);
//		    }
			
			csi_hal_internal_flash_take(0);

		    csi_hal_internal_flash_ClearWriteProtect();
		    csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORETAG_PARA);
		    csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_STORETAG_PARA, (i + 1) * sizeof(SM_tstStoreItem), (uint8_t *)astStoreItem);
		    csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORETAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
		    csi_hal_internal_flash_SetWriteProtect();
			
//		    if(flashSemaphore != NULL)
//		    {
//		        xSemaphoreGive(flashSemaphore);
//		    }
			
			csi_hal_internal_flash_give(0);
		}
		else
		{
			/* There is no space for store tag value */
		}
	}
	#endif
    return;
	#endif
}

/**
  * @name       SM_vStoreStringTag
  * @brief      Store string tag data to file(PC_SIM) or Flash(EMBEDDED & VERSION_2)
  * @note        
  * @param      u32RegID: tag's reg ID
  * @param      u32StringLength: String length
  * @param      pData: String pointer
  * @retval     none
  * @author     Yu Ning
  * @date       2021-09-06
  */
void SM_vStoreStringTag(uint32_t u32RegID, uint32_t u32StringLength, uint8_t *pData)
{
	#ifndef CHIP_GC9002_NoHyper
	uint32_t temp = 0xFFFFFFFF;
	uint16_t currentStringTagLength = 0;
	uint16_t currentStringRegID = 0;
	uint16_t currentStringPos = 0;
	uint16_t currentStringTagLengthInBuf = 0;
	uint16_t currentStringRegIDInBuf = 0;
	uint16_t currentStringPosInBuf = 0;
	uint32_t i, j, k;

	#ifdef PC_SIM
	FILE *fp_storedata;
    errno_t err;
    uint32_t file_size, count = 0;
	u8* srcAddr = 0;
	
	/* Find the special file by name for version code. */
	err = fopen_s(&fp_storedata, AHMI_STR_TAG_FILE, "ab+");
	if (err == 0)
	{
		fseek(fp_storedata,0,SEEK_END);
		file_size = ftell(fp_storedata);

		if((file_size + u32StringLength + sizeof(uint32_t)) < sizeof(uint32_t) * MaxStoreStringTagWordSize)
		{
			/* write string tag data to file */
			srcAddr = (uint8_t *)&u32RegID;
			for(i = 0; i < 2; i++)
			{
				fputc(*srcAddr++, fp_storedata);
			}
			srcAddr = (uint8_t *)&u32StringLength;
			for(i = 0; i < 2; i++)
			{
				fputc(*srcAddr++, fp_storedata);
			}
			for(i = 0; i < u32StringLength; i++)
			{
				fputc(*pData++, fp_storedata);
			}
		}
		else
		{
			/* read all string tag data from file to StoreStringTagArry */
			fseek(fp_storedata, 0, SEEK_SET);
			for(i = 0; i < MaxStoreStringTagWordSize;)
			{
				fseek(fp_storedata, sizeof(uint32_t)*i, SEEK_SET);
				count = fread_s(&temp, sizeof(uint32_t), sizeof(uint32_t), 1, fp_storedata);

				if(count != 0)
				{
					currentStringRegID = (temp & 0xFFFF);
					currentStringTagLength = ((temp & 0xFFFF0000) >> 16);
					currentStringPos = (i + 1);

					for(j = 0; j < MaxStoreStringTagWordSize;)
					{
						temp = StoreStringTagArry[j];

						currentStringRegIDInBuf = (temp & 0xFFFF);
						currentStringTagLengthInBuf = ((temp & 0xFFFF0000) >> 16);
						currentStringPosInBuf = (j + 1);

						if((currentStringRegIDInBuf == currentStringRegID) && (currentStringTagLengthInBuf == currentStringTagLength))
						{
							for(k = 0; k < currentStringTagLength/sizeof(uint32_t); k++)
							{
								count = fread_s(&temp, sizeof(uint32_t), sizeof(uint32_t), 1, fp_storedata);
								if(count != 0)
								{
									StoreStringTagArry[currentStringPosInBuf + k] = temp;
								}
							}
							break;
						}
						else if((currentStringRegIDInBuf == 0xFFFF) && (currentStringTagLengthInBuf == 0xFFFF))
						{
							StoreStringTagArry[currentStringPosInBuf - 1] = (((u32)currentStringTagLength << 16) | currentStringRegID);
							for(k = 0; k < currentStringTagLength/sizeof(uint32_t); k++)
							{
								count = fread_s(&temp, sizeof(uint32_t), sizeof(uint32_t), 1, fp_storedata);
								if(count != 0)
								{
									StoreStringTagArry[currentStringPosInBuf + k] = temp;
								}
							}
							break;
						}
						j = (j + 1 + currentStringTagLengthInBuf/4);
					}

					i = (i + 1 + currentStringTagLength/4);
				}
				else
				{
					currentStringRegID = 0xFFFF;
				}

				if(currentStringRegID == 0xFFFF)
				{
					break;
				}
			}

			/* put u32TagID into StoreStringTagArry */
			for(j = 0; j < MaxStoreStringTagWordSize;)
			{
				temp = StoreStringTagArry[j];

				currentStringRegIDInBuf = (temp & 0xFFFF);
				currentStringTagLengthInBuf = ((temp & 0xFFFF0000) >> 16);
				currentStringPosInBuf = (j + 1);

				if((currentStringRegIDInBuf == u32RegID) && (currentStringTagLengthInBuf == u32StringLength))
				{
					for(k = 0; k < u32StringLength/sizeof(uint32_t); k++)
					{
						StoreStringTagArry[currentStringPosInBuf + k] = *((uint32_t*)(pData + sizeof(uint32_t) * k));
					}
					break;
				}
				else if((currentStringRegIDInBuf == 0xFFFF) && (currentStringTagLengthInBuf == 0xFFFF) && (j * sizeof(u32) + u32StringLength + sizeof(u32)) < sizeof(u32) * MaxStoreStringTagWordSize)
				{
					StoreStringTagArry[currentStringPosInBuf - 1] = ((u32StringLength << 16) | u32RegID);
					for(k = 0; k < u32StringLength/sizeof(uint32_t); k++)
					{
						StoreStringTagArry[currentStringPosInBuf + k] = *((uint32_t*)(pData + sizeof(uint32_t) * k));
					}
					break;
				}
				j = (j + 1 + currentStringTagLengthInBuf/4);
			}

			/* Close file. */
			fclose(fp_storedata);
			err = 1;

			/* write StoreStringTagArry to file */
			/* Find the special file by name for version code. */
			err = fopen_s(&fp_storedata, AHMI_STR_TAG_FILE, "w+");

			if(err == 0)
			{
				fseek(fp_storedata,0,SEEK_SET);
				srcAddr = (uint8_t *)&StoreStringTagArry;

				while((*(uint32_t *)srcAddr != 0xFFFFFFFF) && ((srcAddr - (uint8_t *)&StoreStringTagArry) < sizeof(uint32_t) * MaxStoreStringTagWordSize))
				{
					fputc(*srcAddr++, fp_storedata);
				}
			}
		}

	    /* Close file. */
	    fclose(fp_storedata);
	}
	else
	{
	    /* Open file "NumberTagInfo.bin" fail */
	}
    #endif

	#if defined(EMBEDDED) && defined(VERSION_2)
	uint32_t StoreTagValueFlag = AHMI_CPU1_VERSION;
	
//    if(flashSemaphore != NULL)
//    {
//        xSemaphoreTake(flashSemaphore, portMAX_DELAY);
//    }
	
	csi_hal_internal_flash_take(0);
	
	for(i = 0; i < MaxStoreStringTagWordSize; i++)
	{
		csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + i * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);

		if(temp == 0xFFFFFFFF)
			break;
	}

	if((i * sizeof(uint32_t) + u32StringLength + sizeof(uint32_t)) < sizeof(uint32_t) * MaxStoreStringTagWordSize)
	{
		/* write string tag data to flash */
		csi_hal_internal_flash_ClearWriteProtect();

		temp = ((u32StringLength << 16) | u32RegID);
		csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + i * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);

		csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + (i + 1) * sizeof(uint32_t), u32StringLength, pData);

		csi_hal_internal_flash_SetWriteProtect();
	}
	else
	{
		/* read all string tag data from flash to StoreStringTagArry */
		for(i = 0; i < MaxStoreStringTagWordSize;)
		{
			csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + i * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);

			currentStringRegID = (temp & 0xFFFF);
			currentStringTagLength = ((temp & 0xFFFF0000) >> 16);
			currentStringPos = (i + 1);

			if((currentStringRegID != 0xFFFF) && (currentStringTagLength != 0xFFFF))
			{
				for(j = 0; j < MaxStoreStringTagWordSize;)
				{
					temp = StoreStringTagArry[j];

					currentStringRegIDInBuf = (temp & 0xFFFF);
					currentStringTagLengthInBuf = ((temp & 0xFFFF0000) >> 16);
					currentStringPosInBuf = (j + 1);

					if((currentStringRegIDInBuf == currentStringRegID) && (currentStringTagLengthInBuf == currentStringTagLength))
					{
						for(k = 0; k < currentStringTagLength/sizeof(uint32_t); k++)
						{
							csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + currentStringPos * sizeof(uint32_t) + k * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);
							StoreStringTagArry[currentStringPosInBuf + k] = temp;
						}
						break;
					}
					else if((currentStringRegIDInBuf == 0xFFFF) && (currentStringTagLengthInBuf == 0xFFFF))
					{
						StoreStringTagArry[currentStringPosInBuf - 1] = (((uint32_t)currentStringTagLength << 16) | currentStringRegID);
						for(k = 0; k < currentStringTagLength/sizeof(uint32_t); k++)
						{
							csi_hal_internal_flash_read(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA + currentStringPos * sizeof(uint32_t) + k * sizeof(uint32_t), sizeof(uint32_t), (uint8_t *)&temp);
							StoreStringTagArry[currentStringPosInBuf + k] = temp;
						}
						break;
					}
					j = (j + 1 + currentStringTagLengthInBuf/4);
				}

				i = (i + 1 + currentStringTagLength/4);
			}
			else
			{
				break;
			}
		}

		/* put u32TagID into StoreStringTagArry */
		for(j = 0; j < MaxStoreStringTagWordSize;)
		{
			temp = StoreStringTagArry[j];

			currentStringRegIDInBuf = (temp & 0xFFFF);
			currentStringTagLengthInBuf = ((temp & 0xFFFF0000) >> 16);
			currentStringPosInBuf = (j + 1);

			if((currentStringRegIDInBuf == u32RegID) && (currentStringTagLengthInBuf == u32StringLength))
			{
				for(k = 0; k < u32StringLength/sizeof(uint32_t); k++)
				{
					StoreStringTagArry[currentStringPosInBuf + k] = *((uint32_t*)(pData + sizeof(uint32_t) * k));
				}
				break;
			}
			else if((currentStringRegIDInBuf == 0xFFFF) && (currentStringTagLengthInBuf == 0xFFFF) && (j * sizeof(uint32_t) + u32StringLength + sizeof(uint32_t)) < sizeof(uint32_t) * MaxStoreStringTagWordSize)
			{
				StoreStringTagArry[currentStringPosInBuf - 1] = ((u32StringLength << 16) | u32RegID);
				for(k = 0; k < u32StringLength/sizeof(uint32_t); k++)
				{
					StoreStringTagArry[currentStringPosInBuf + k] = *((uint32_t*)(pData + sizeof(uint32_t) * k));
				}
				break;
			}
			j = (j + 1 + currentStringTagLengthInBuf/4);
		}

		/* write StoreStringTagArry to flash */
		for(i = 0; i < MaxStoreStringTagWordSize; i++)
		{
			if(StoreStringTagArry[i] == 0xFFFFFFFF)
				break;
		}
		if(i < MaxStoreStringTagWordSize)
		{
			csi_hal_internal_flash_ClearWriteProtect();
			csi_hal_interal_flash_sector_erase(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA);
	
			csi_hal_internal_flash_write(BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA, i * sizeof(uint32_t), (uint8_t *)StoreStringTagArry);
	
			csi_hal_internal_flash_write(BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA, 4, (uint8_t *)&StoreTagValueFlag);
			csi_hal_internal_flash_SetWriteProtect();
		}
	}
	
//    if(flashSemaphore != NULL)
//    {
//        xSemaphoreGive(flashSemaphore);
//    }
	
	csi_hal_internal_flash_give(0);

	#endif
	#endif
}
