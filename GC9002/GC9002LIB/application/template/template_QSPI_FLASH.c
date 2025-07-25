/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     main.c
 * @brief    hello world
 * @version  V1.0
 * @date     17. Jan 2018
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "csi_motor.h"
#include "core_rv32.h"
#include "template_def.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_QSPI_FLASH_

#define Flash_ADDR              0x0U
static QSPI_Flash_HandleTypeDef hqspi_flash;
#define TEST_BUFF_SIZE          128//256
uint8_t *buff_ram = (uint8_t *)0x2001000;
uint8_t *tx_buff = (uint8_t *)0x2001000;
uint8_t *rx_buff = (uint8_t *)0x2003000;

#define TEST_MODE_WRITE_READ        0
#define TEST_MODE_ERASE             1
#define TEST_MODE_CHECK             TEST_MODE_ERASE

int ProgramPage (unsigned int adr, unsigned int size, unsigned int *buf)
{
    unsigned int val = 0;
    size = (size + 3) & ~3;
    while(size)
    {
        *(unsigned int *)(adr) = *((unsigned int *)buf);
        adr += 4;
        buf+=1;
        size -= 4;
        val+=4;
        if(val == 0xc0)
        {
            val+=4;
        }
    }
    return 0;
}

int main(void)
{
    HAL_QSPI_FlashStatusTypeDef status;
    hqspi_flash.Instance = CQSPI;
	HAL_QSPI_Flash_StructInit(&hqspi_flash);

    while(1)
    {
        status = HAL_QSPI_Flash_Init(&hqspi_flash);
        if(status != HAL_Flash_OK)
        {
            printf("HAL_QSPI_Flash_Init ERROR.\n");
            while(1);
        }    
#if(TEST_MODE_CHECK == TEST_MODE_WRITE_READ)
        memset(tx_buff, 0xC3, TEST_BUFF_SIZE);
        status = HAL_QSPI_Flash_Block_Erase(&hqspi_flash, Flash_ADDR);
        if(status != HAL_Flash_OK)
        {
            printf("HAL_QSPI_Flash_Block_Erase ERROR.\n");
            while(1);
        }
        for(int i = 0; i < TEST_BUFF_SIZE; i++)
        {
            tx_buff[i] = i;
        }
        status = HAL_QSPI_Flash_Write(&hqspi_flash, Flash_ADDR, tx_buff, TEST_BUFF_SIZE);
        if(status != HAL_Flash_OK)
        {
            printf("HAL_QSPI_Flash_Write ERROR.\n");
            while(1);
        }
        memset(tx_buff, 0x00, TEST_BUFF_SIZE);
        status = HAL_QSPI_Flash_Read(&hqspi_flash, Flash_ADDR, tx_buff, TEST_BUFF_SIZE);
        if(status != HAL_Flash_OK)
        {
            printf("HAL_QSPI_Flash_Read ERROR.\n");
            while(1);
        }
#elif(TEST_MODE_CHECK == TEST_MODE_ERASE)

        HAL_QSPI_Flash_Sector_Erase(&hqspi_flash, 0);
        for(int k = 0; k < TEST_BUFF_SIZE; k++)
        {
            tx_buff[k] = k;
        }
        HAL_QSPI_Flash_Write(&hqspi_flash, 0, tx_buff, TEST_BUFF_SIZE);
        memset(rx_buff, 0x00, TEST_BUFF_SIZE);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0, rx_buff, TEST_BUFF_SIZE);
        /* Data compare */
        if(memcmp(tx_buff, rx_buff, TEST_BUFF_SIZE) != 0)
        {
            printf("memcmp error\n");
            while(1);
        }
        while(1);
        

//#define TEST_PAGE
//#define TEST_SECTOR
//#define TEST_BLOCK
//#define TEST_CHIP
#ifdef TEST_PAGE
        while(1)
        {
            HAL_QSPI_Flash_Sector_Erase(&hqspi_flash, 0);
            for(int k = 0; k < TEST_BUFF_SIZE; k++)
            {
                tx_buff[k] = k;
            }
            HAL_QSPI_Flash_Write(&hqspi_flash, 0, tx_buff, TEST_BUFF_SIZE);
//            ProgramPage(0x8000000, TEST_BUFF_SIZE, (unsigned int*)tx_buff);
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, 0, tx_buff, TEST_BUFF_SIZE);
        }
        HAL_QSPI_Flash_Sector_Erase(&hqspi_flash, 0);
        for(int i = 0; i < hqspi_flash.Flash.Size.pages_one_sector; i++)
        {
            for(int k = 0; k < TEST_BUFF_SIZE; k++)
            {
                tx_buff[k] = k;
            }
            HAL_QSPI_Flash_Write(&hqspi_flash, i*hqspi_flash.Flash.Size.page_size, tx_buff, TEST_BUFF_SIZE);
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, i*hqspi_flash.Flash.Size.page_size, tx_buff, TEST_BUFF_SIZE);
            for(int j = 0; j < TEST_BUFF_SIZE; j++)
            {
                printf("%x ", tx_buff[j]);
                if(j%16 == 15)
                    printf("\n");
            }
            printf("\r\n");
        }
        memset(buff_ram, 0x00, hqspi_flash.Flash.Size.sector_size);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0, buff_ram, hqspi_flash.Flash.Size.sector_size);
#endif
#ifdef TEST_SECTOR
        HAL_QSPI_Flash_Erase(&hqspi_flash, 0, hqspi_flash.Flash.Size.sector_size*3);

        for(int k = 0; k < TEST_BUFF_SIZE; k++)
        {
            tx_buff[k] = k;
        }
//        memset(tx_buff, 0xC3, TEST_BUFF_SIZE);
        HAL_QSPI_Flash_Write(&hqspi_flash, 0, tx_buff, TEST_BUFF_SIZE);
        HAL_QSPI_Flash_Write(&hqspi_flash, 0x1000, tx_buff, TEST_BUFF_SIZE);
        HAL_QSPI_Flash_Write(&hqspi_flash, 0x2000, tx_buff, TEST_BUFF_SIZE);
        
        memset((uint8_t *)0x02001000, 0, 512);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0x1000, (uint8_t *)0x02001000, 512);
        memset((uint8_t *)0x02001000, 0, 512);
        uint8_t tempbuff[128];
        HAL_QSPI_Flash_Read(&hqspi_flash, 0x1000, tempbuff, sizeof(tempbuff));
        
        memset(tx_buff, 0x00, TEST_BUFF_SIZE+4);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0, tx_buff, TEST_BUFF_SIZE);
        memset(tx_buff, 0x00, TEST_BUFF_SIZE+4);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0x1000, tx_buff, TEST_BUFF_SIZE+1);
        memset(tx_buff, 0x00, TEST_BUFF_SIZE+4);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0x2000, tx_buff, TEST_BUFF_SIZE+2);
        memset(tx_buff, 0x00, TEST_BUFF_SIZE+4);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0x2000, tx_buff, TEST_BUFF_SIZE+3);
        HAL_QSPI_Flash_Erase(&hqspi_flash, 0xff, hqspi_flash.Flash.Size.sector_size*2);
        while(1);
        memset(tx_buff, 0x00, TEST_BUFF_SIZE);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0, tx_buff, TEST_BUFF_SIZE);
        memset(tx_buff, 0x00, TEST_BUFF_SIZE);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0x1000, tx_buff, TEST_BUFF_SIZE);
        memset(tx_buff, 0x00, TEST_BUFF_SIZE);
        HAL_QSPI_Flash_Read(&hqspi_flash, 0x2000, tx_buff, TEST_BUFF_SIZE);
        
        // Sector Erase
        for(int i = 0; i < hqspi_flash.Flash.Size.sectors_one_block; i++)
        {
            HAL_QSPI_Flash_Sector_Erase(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size);
            memset(tx_buff, 0xC3, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Write(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size, tx_buff, TEST_BUFF_SIZE);
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size, tx_buff, TEST_BUFF_SIZE);
            for(int j = 0; j < TEST_BUFF_SIZE; j++)
            {
                printf("%x ", tx_buff[j]);
                if(j%16 == 15)
                    printf("\n");
            }
            printf("\r\n");
//            HAL_QSPI_Flash_Sector_Erase(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size);
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size, tx_buff, TEST_BUFF_SIZE);
            for(int j = 0; j < TEST_BUFF_SIZE; j++)
            {
                printf("%x ", tx_buff[j]);
                if(j%16 == 15)
                    printf("\n");
            }
            printf("\r\n");
        }
#endif
        HAL_Delay(10000000);
#ifdef TEST_BLOCK
        // Block Erase
        for(int i = 0; i < hqspi_flash.Flash.Size.blocks; i++)
        {
            memset(tx_buff, 0xC3, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Write(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size*hqspi_flash.Flash.Size.sectors_one_block, tx_buff, TEST_BUFF_SIZE);
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size*hqspi_flash.Flash.Size.sectors_one_block, tx_buff, TEST_BUFF_SIZE);
            for(int j = 0; j < TEST_BUFF_SIZE; j++)
            {
                printf("%x ", tx_buff[j]);
                if(j%16 == 15)
                    printf("\n");
            }
            printf("\r\n");
            HAL_Delay(10000000);
            
            HAL_QSPI_Flash_Block_Erase(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size*hqspi_flash.Flash.Size.sectors_one_block);
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size*hqspi_flash.Flash.Size.sectors_one_block, tx_buff, TEST_BUFF_SIZE);
            for(int j = 0; j < TEST_BUFF_SIZE; j++)
            {
                printf("%x ", tx_buff[j]);
                if(j%16 == 15)
                    printf("\n");
            }
            printf("\r\n");
        }
#endif
        HAL_Delay(10000000);
#ifdef TEST_CHIP
        // Chip Erase
        HAL_QSPI_Flash_Chip_Erase(&hqspi_flash);
        for(int i = 0; i < hqspi_flash.Flash.Size.blocks; i++)
        {
            memset(tx_buff, 0xC3, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Write(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size*hqspi_flash.Flash.Size.sectors_one_block, tx_buff, TEST_BUFF_SIZE);
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size*hqspi_flash.Flash.Size.sectors_one_block, tx_buff, TEST_BUFF_SIZE);
            for(int j = 0; j < TEST_BUFF_SIZE; j++)
            {
                printf("%x ", tx_buff[j]);
                if(j%16 == 15)
                    printf("\n");
            }
            printf("\r\n");
            HAL_Delay(10000000);
        }
        HAL_Delay(10000000);
        HAL_QSPI_Flash_Chip_Erase(&hqspi_flash);
        for(int i = 0; i < hqspi_flash.Flash.Size.blocks; i++)
        {
            memset(tx_buff, 0x00, TEST_BUFF_SIZE);
            HAL_QSPI_Flash_Read(&hqspi_flash, i*hqspi_flash.Flash.Size.sector_size*hqspi_flash.Flash.Size.sectors_one_block, tx_buff, TEST_BUFF_SIZE);
            for(int j = 0; j < TEST_BUFF_SIZE; j++)
            {
                printf("%x ", tx_buff[j]);
                if(j%16 == 15)
                    printf("\n");
            }
            printf("\r\n");
        }
#endif
#endif
        while(1);
    }
    return 0;
}

#endif
#endif