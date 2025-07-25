/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "usbh_core.h"
#include "usbh_msc.h"
#include "gc9002_hal_usb.h"
#include "SPI_SDCard.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

extern SD_CardInfo SDCardInfo;
extern SPI_TypeDef * UPG_SdSpi;

//extern __s32       USB_SCSI_Open(__u16 drive_no);

//extern __s32       USB_SCSI_Read_Write(__u16 drive_no, 
//                                       __u32 sector, 
//                                        void *buffer, 
//                                       __u16 count, 
//                                       __s32 reading);                                   
                                        
static size_t musb_read(uint32_t pos, const uint8_t *buffer, uint32_t size)
{
    uint32_t readCnt = 0;
    const uint32_t readCntMax = 10;
    struct usbh_msc *msc_class = NULL;

    msc_class = msc_dev_probe(100);
    if(msc_class == NULL)
    {
        return 0;
    }

    int ret = 0;
    while(readCnt < readCntMax)
    {
        ret = usbh_msc_scsi_read10(msc_class, pos, buffer, size);
        if (ret < 0)
        {
            printf("scsi_read10 error, ret:%d \n", ret);
            readCnt++;
        }
        else
        {
            break;
        }
    }

    if(ret < 0)
    {
        return ret;
    }

    return size;
}

static size_t musb_write(uint32_t pos, const uint8_t *buffer, uint32_t size)
{
    uint32_t writeCnt = 0;
    const uint32_t writeCntMax = 10;
    struct usbh_msc *msc_class = NULL;
    
    msc_class = msc_dev_probe(100);
    if(msc_class == NULL)
    {
        return 0;
    }

    int ret = 0;
    while(writeCnt < writeCntMax)
    {
        ret = usbh_msc_scsi_write10(msc_class, pos, buffer, (size / msc_class->blocksize) + 1);
        if (ret < 0)
        {
            printf("scsi_write10 error, ret:%d \n", ret);
            writeCnt++;
        }
        else
        {
            break;
        }
    }

    if(ret < 0)
    {
        return ret;
    }

    return size;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case DEV_RAM :
		return RES_ERROR;

	case DEV_MMC :
		stat = SDCardSPI_Init(UPG_SdSpi);
		if (stat != SD_OK)
		{
			return STA_NOINIT;
		}
		else
		{
			return RES_OK;
		}
	case DEV_USB :
        return RES_OK;
	}
	
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
//	DRESULT res;
//	int result;
	unsigned int rcv;

	if(count == 0)
	{
		return RES_PARERR;
	}
	
	switch (pdrv) {
	case DEV_RAM :	
		return RES_ERROR;

	case DEV_MMC :
		if(count > 1)
		{
			rcv = SDCardSPI_ReadMultiBlockAPI(sector * SDCardInfo.CardBlockSize, buff, SDCardInfo.CardBlockSize, count);
		}
		else
		{
			rcv = SDCardSPI_ReadSingleBlockAPI(sector * SDCardInfo.CardBlockSize, buff, SDCardInfo.CardBlockSize);
		}
	
		if(rcv == SD_OK)
		{
			return RES_OK;
		}
		else
		{
			return RES_ERROR;
		}
		
	case DEV_USB :
        rcv = musb_read(sector, (const uint8_t *)buff, count);
		if(rcv == count)//sector size: MSD_DRIVE_SECTOR_SIZE(0x0200)
		{
			return RES_OK;
		}
		else
		{
			return RES_ERROR;
		}
	}
	
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	unsigned int rcv;

    if(count == 0)
	{
        return RES_PARERR;
	}
	
	switch (pdrv) 
	{
	case DEV_RAM :	
		return RES_ERROR;

	case DEV_MMC :
		if(count > 1)
		{
			rcv = SDCardSPI_WriteMultiBlockAPI(sector * SDCardInfo.CardBlockSize, buff, SDCardInfo.CardBlockSize, count);
		}
		else
		{
			rcv = SDCardSPI_WriteSingleBlockAPI(sector * SDCardInfo.CardBlockSize, buff, SDCardInfo.CardBlockSize);
		}
	
		if(rcv == SD_OK)
		{
			return RES_OK;
		}
		else
		{
			return RES_ERROR;
		}
		
	case DEV_USB :
        rcv = musb_write(sector, (const uint8_t *)buff, count);
		if(rcv == count)//sector size: MSD_DRIVE_SECTOR_SIZE(0x0200)
		{
			return RES_OK;
		}
		else
		{
			return RES_ERROR;
		}
	}
	
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}

