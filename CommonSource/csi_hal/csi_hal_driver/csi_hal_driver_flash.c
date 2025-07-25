

#include "csi_hal_driver_flash.h"

#if defined(CHIP_GC9005)
//#include "texture_flash.h"
#include "gc90x_flash.h"
extern uint32_t TextureFlashSize;
#else
//uint32_t TextureFlashSize;
#endif

#if defined(CHIP_GC9002)

#define OK 0
#endif

#if defined(CHIP_GC9002)
#define __FLASH_DRIVER 		__attribute__((used, section(".flashdriver")))
#elif defined(CHIP_GC9005)
#define  __FLASH_DRIVER
#else
#define  __FLASH_DRIVER
#endif

#if defined(CHIP_GC9002)
__FLASH_DRIVER void Flash_Delay_calib()
{
    uint32_t i;
    uint32_t tmp;
    uint32_t testID;
    uint32_t old_reg00;
    int max=-1,min=-1;
    uint32_t FlashID;
    uint8_t pass[32];
    
    old_reg00 = CQSPI->QSPI_REG.CR;
    
    // config flash clk divider to 32x
    CQSPI->QSPI_REG.CR = CQSPI->QSPI_REG.CR | 0x780000;
    
    // config flash read delay to 0
    CQSPI->QSPI_REG.RDCAPR = 1;
    
    CQSPI->QSPI_REG.FCCR = 0x9FB00001;
    while(((CQSPI->QSPI_REG.FCCR)&0x2)||((CQSPI->QSPI_REG.FCCR)==0)||((CQSPI->QSPI_REG.CR & 0x80000000)==0));
    
    FlashID = CQSPI->QSPI_REG.FCRLDR;
    
    //restore reg00
    CQSPI->QSPI_REG.CR = old_reg00;
    
    for(i=0;i<32;i++)
    {
        tmp = ((i >> 1) << 1) + (((uint32_t)(i + 1) <<31 ) >> 26) + 1;
        
        CQSPI->QSPI_REG.RDCAPR = tmp;
        
        CQSPI->QSPI_REG.FCCR = 0x9FB00001;
        while(((CQSPI->QSPI_REG.FCCR)&0x2)||((CQSPI->QSPI_REG.FCCR)==0)||((CQSPI->QSPI_REG.CR & 0x80000000)==0));
        
        testID = CQSPI->QSPI_REG.FCRLDR;

        if(testID == FlashID) pass[i] = 1;
        else pass[i]=0; 
        
        if((i==0) && (pass[0]==1)) min = 0;
        else if((i!=0) && (pass[i-1]==0) &&(pass[i]==1)) min = i;
        
        if((i==31) && (pass[31]==1)) max = 31;
        else if(i != 0 && (pass[i-1]==1) &&(pass[i]==0)) max = i - 1;
        
    }
    
    if((max == -1) || (max == -1)) return;
    
    i = (max+min)/2;
    
    
    tmp = ((i >> 1) << 1) + (((uint32_t)(i + 1) <<31 ) >> 26) + 1;
    
    CQSPI->QSPI_REG.RDCAPR = tmp;
}

__FLASH_DRIVER void gc9002_hal_flash_init(void)
{
    //int32_t FlashID = 0;

    while((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000){};

	//CQSPI PRESCALER /2
	*(uint32_t *)(0x40200014) = 1;

    //CQSPI->QSPI_REG.FCCR = 0x9FB00001;
    //while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;

    //FlashID = CQSPI->QSPI_REG.FCRLDR;

    //FlashID = FlashID & 0x00FFFFFF;

	//CQSPI->QSPI_REG.CR = 0x1B881;
	//CQSPI controller baud rate divisor /2
    CQSPI->QSPI_REG.CR &= 0xFF87FFFF;
    //CQSPI->QSPI_REG.CR |= 0x00080000;

    //CQSPI->QSPI_REG.RDCAPR &= ~(0x0000001E);
    //CQSPI->QSPI_REG.RDCAPR |= (0x00 << 1);

//  CQSPI->QSPI_REG.DRIR = 0x001220eb;  //XM 64Mb



//    /* Enter 4 byte address mode. */
//    if(FlashID == 0x1940C8)
//    {
//        //TextureFlashSize = 0x2000000;
//
//        CQSPI->QSPI_REG.DRIR = 0x041220eb;  //GD25Q257
//
//        CQSPI->QSPI_REG.DSCR &= 0xFFFFFFF0;
//        CQSPI->QSPI_REG.DSCR |= 0x03;
//
//        CQSPI->QSPI_REG.FCCR = 0xB7000001;
//        while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;
//    }


//    else if(FlashID == 0x17405E)
//    {
//        //TextureFlashSize = 0x800000;
//
//        CQSPI->QSPI_REG.DRIR = 0x041220eb;  //ZBIT 64 Mb QSPI
////      CQSPI->QSPI_REG.DRIR = 0x021220eb;  //ZBIT 64 Mb QPI
//    }

//    else if(FlashID == 0x174020)
//    {
//        //TextureFlashSize = 0x800000;
//
//        CQSPI->QSPI_REG.DRIR = 0x041220eb;  //XM 64Mb QSPI
////      CQSPI->QSPI_REG.DRIR = 0x001220eb;  //XM 64Mb QPI
//    }
//	else if(FlashID == 0x184020)
    {
        //TextureFlashSize = 0x1000000;

        CQSPI->QSPI_REG.DRIR = 0x041220eb;  //XM 128Mb QSPI
//      CQSPI->QSPI_REG.DRIR = 0x001220eb;  //XM 128Mb QPI
    }
	
	Flash_Delay_calib();
	
    /*Set QE.*/
    CQSPI->QSPI_REG.FCCR = 0x50000001;
    while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;
    CQSPI->QSPI_REG.FCWLDR = 0x02;
    CQSPI->QSPI_REG.FCCR = 0x31008001;
    while((CQSPI->QSPI_REG.FCCR &0x02) != 0);


//  if((FlashID == 0x17405E) || (FlashID == 0x174020))
//  {
//      /* Exter QPI mode. */
//      CQSPI->QSPI_REG.FCCR = 0x38000001;
//      while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;
//
//      /* Config Instruction Type. */
//      CQSPI->QSPI_REG.DRIR |= 0x200;   //for qpi
//  }
}

__FLASH_DRIVER void gc9002_hal_internal_flash_SetWriteProtect(void)
{
    CQSPI->QSPI_REG.LWPR = 0x08000000;
    CQSPI->QSPI_REG.UWPR = 0x10000000;
    CQSPI->QSPI_REG.WPR = 1;
}


__FLASH_DRIVER void gc9002_hal_internal_flash_ClearWriteProtect(void)
{
    CQSPI->QSPI_REG.WPR = 0;
}

__FLASH_DRIVER uint32_t gc9002_hal_internal_flash_read(uint32_t addr, uint32_t len, uint8_t* buffer)
{
    uint32_t i;

    while((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000);

    for(i = 0; i < len; i++)
    {
        *(unsigned char *)((uint32_t)buffer + i) = *((unsigned char *)(addr + i));
    }

    while((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000);

//  len = (len + 3) & ~3;
//  while (len) {
//      *(unsigned int *)(buffer) = *((unsigned int *) addr);
//      addr += 4;                                           // Go to next Word
//      buffer += 1;
//      len  -= 4;
//  }

    return OK;
}

__FLASH_DRIVER void gc9002_hal_internal_flash_write(uint32_t addr, uint32_t len, uint8_t* buffer)
{
    uint32_t i = 0;

    uint32_t val_mstatus = __get_MSTATUS();

    if(val_mstatus & 0x8)
    {
        __disable_irq();
    }

    while((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000);

	CQSPI->QSPI_REG.CR &= 0xFF87FFFF;
    CQSPI->QSPI_REG.CR |= 0x00080000;
	
    for(i = 0; i < len; i++)
    {
        *(unsigned char *)((uint32_t)addr + i) = *((unsigned char *)(buffer + i));
    }

    while((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000);

//  len = (len + 3) & ~3;                                   // Adjust size for Words
//  while (len) {
//      *(unsigned int *)(addr) = *((unsigned int *)buffer);
//      addr += 4;                                           // Go to next Word
//      buffer += 1;
//      len  -= 4;
//
//  }

    csi_icache_invalid();

	CQSPI->QSPI_REG.CR &= 0xFF87FFFF;
    //CQSPI->QSPI_REG.CR |= 0x00080000;
    if(val_mstatus & 0x8)
    {
        __enable_irq();
    }
}

__FLASH_DRIVER void gc9002_hal_interal_flash_sector_erase(uint32_t addr)
{
//  csi_spiflash_erase(&spiflash, addr, 4096);

    uint32_t FlashID = 0;

    uint32_t val_mstatus = __get_MSTATUS();

    if(val_mstatus & 0x8)
    {
        __disable_irq();
    }
	
	CQSPI->QSPI_REG.CR &= 0xFF87FFFF;
    CQSPI->QSPI_REG.CR |= 0x00080000;
	
    CQSPI->QSPI_REG.FCCR = 0x9FB00001;
    while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;

    FlashID = CQSPI->QSPI_REG.FCRLDR;

    CQSPI->QSPI_REG.FCCR = 0x06000001; //opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
    while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;

    CQSPI->QSPI_REG.FCAR = addr;       //set erase sector/block address
    if((FlashID & 0xFF0000) > 0x180000)
    {
        CQSPI->QSPI_REG.FCCR = 0x200B0001; //opcode 0x20 4bytes addr for sector erase
    }
    else
    {
        CQSPI->QSPI_REG.FCCR = 0x200A0001; //opcode 0x20 3bytes addr for sector erase
    }
    while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;

    do
    {
        CQSPI->QSPI_REG.FCCR = 0x05800001;
        while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;
    }while(((CQSPI->QSPI_REG.FCRLDR) & 0x01)||((CQSPI->QSPI_REG.FCRLDR)==0));

    csi_icache_invalid();

	CQSPI->QSPI_REG.CR &= 0xFF87FFFF;
    //CQSPI->QSPI_REG.CR |= 0x00080000;

    if(val_mstatus & 0x8)
    {
        __enable_irq();
    }
}

__FLASH_DRIVER void gc9002_hal_internal_flash_block_erase(uint32_t addr)
{
    uint32_t FlashID = 0;

    uint32_t val_mstatus = __get_MSTATUS();

    if(val_mstatus & 0x8)
    {
        __disable_irq();
    }

	CQSPI->QSPI_REG.CR &= 0xFF87FFFF;
    CQSPI->QSPI_REG.CR |= 0x00080000;

    CQSPI->QSPI_REG.FCCR = 0x9FB00001;
    while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;

    FlashID = CQSPI->QSPI_REG.FCRLDR;

    CQSPI->QSPI_REG.FCCR = 0x06000001; //opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
    while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;

    CQSPI->QSPI_REG.FCAR = addr;       //set erase sector/block address
    if((FlashID & 0xFF0000) > 0x180000)
    {
        CQSPI->QSPI_REG.FCCR = 0xD80B0001; //opcode 0xD8 4bytes addr for block erase
    }
    else
    {
        CQSPI->QSPI_REG.FCCR = 0xD80A0001; //opcode 0xD8 3bytes addr for block erase
    }
     while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;

    do
    {
        CQSPI->QSPI_REG.FCCR = 0x05800001;
        while((CQSPI->QSPI_REG.FCCR & 0x2) || (((CQSPI->QSPI_REG.CR & 0x80000000) == 0x00000000))) ;
    }while(((CQSPI->QSPI_REG.FCRLDR) & 0x01)||((CQSPI->QSPI_REG.FCRLDR)==0));

    csi_icache_invalid();

	CQSPI->QSPI_REG.CR &= 0xFF87FFFF;
    //CQSPI->QSPI_REG.CR |= 0x00080000;

    if(val_mstatus & 0x8)
    {
        __enable_irq();
    }
}
#endif
#include "csi_kernel.h"
#include "FreeRTOS.h"
#include "semphr.h"

k_sem_handle_t flashSemaphore;

void csi_hal_internal_flash_take(uint32_t intflag)
{
	static BaseType_t xHigherPriorityTaskWoken;
	
	if(flashSemaphore == NULL)
	{
		/* Create Semaphore for FLASH. */
		flashSemaphore = csi_kernel_sem_new( 1, 1 ); 
		if(flashSemaphore == NULL)
		{
//			ERROR_PRINT("Fail to create semaphore flashSemaphore.\r\n");
			return ;
		}
	
	}

    if(flashSemaphore != NULL) //if donnot calibration, needs to start video too
    {
		if(intflag == 1)
		{
			xSemaphoreTakeFromISR(flashSemaphore,  &xHigherPriorityTaskWoken);
		}
		else 
		{
			xSemaphoreTake(flashSemaphore, portMAX_DELAY);
		}
	}
}

void csi_hal_internal_flash_give(uint32_t intflag)
{
	static BaseType_t xHigherPriorityTaskWoken;

	if(flashSemaphore != NULL) //if donnot calibration, needs to start video too
    {
        if(intflag == 1)
        {
            xSemaphoreGiveFromISR(flashSemaphore, &xHigherPriorityTaskWoken);
        }
		else 
		{
			xSemaphoreGive(flashSemaphore);
		}
    }
}

__FLASH_DRIVER void csi_hal_internal_flash_init(void)
{
	#if defined(CHIP_GC9002)
	gc9002_hal_flash_init();
    #elif defined(CHIP_GC9005)
	init_texture_flash();
	set_flash_parameters();
	ext_flash_gzip_self_adaption();
	#else
	
	#endif
}

__FLASH_DRIVER void csi_hal_internal_flash_SetWriteProtect(void)
{
	#if defined(CHIP_GC9002)
	gc9002_hal_internal_flash_SetWriteProtect();
    #elif defined(CHIP_GC9005)
	ext_flash_setWriteProtect();
	#else
	INTERNAL_FLASH_SetWriteProtect();
	#endif
}

__FLASH_DRIVER void csi_hal_internal_flash_ClearWriteProtect(void)
{
	#if defined(CHIP_GC9002)
	gc9002_hal_internal_flash_ClearWriteProtect();
    #elif defined(CHIP_GC9005)
	ext_flash_clrWriteProtect();
	#else
	INTERNAL_FLASH_ClrWriteProtect();
	#endif
}

__FLASH_DRIVER uint32_t csi_hal_internal_flash_read(uint32_t addr, uint32_t len, uint8_t* buffer)
{
    #if defined(CHIP_GC9002)
    return gc9002_hal_internal_flash_read(addr, len, buffer);
    #elif defined(CHIP_GC9005)
	ext_flash_read(addr & 0x0FFFFFFF, len, buffer);
	#else
	return INTERNAL_FLASH_ReadData(addr, len, (unsigned char *)buffer);
	#endif
}

__FLASH_DRIVER void csi_hal_internal_flash_write(uint32_t addr, uint32_t len, uint8_t* buffer)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_internal_flash_write(addr, len, buffer);
    #elif defined(CHIP_GC9005)
    inter_flash_write(addr & 0x0FFFFFFF, len, buffer);
	#else
	INTERNAL_FLASH_WriteData(addr, len, (unsigned char *)buffer);
	#endif
}

__FLASH_DRIVER void csi_hal_interal_flash_sector_erase(uint32_t addr)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_interal_flash_sector_erase(addr);
    #elif defined(CHIP_GC9005)
    ext_flash_sector_erase(addr & 0x0FFFFFFF); // nand flash do not support sector erase.
	#else
	INTERNAL_FLASH_SectorErasePage(addr);
	#endif
}

__FLASH_DRIVER void csi_hal_internal_flash_block_erase(uint32_t addr)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_internal_flash_block_erase(addr);
    #elif defined(CHIP_GC9005)
	ext_flash_erase(addr & 0x0FFFFFFF);
    #else

	#endif
}

__FLASH_DRIVER void csi_hal_extern_flash_init(void)
{
	#if defined(CHIP_GC9002)
    #elif defined(CHIP_GC9005)

	#else

	#endif
}

__FLASH_DRIVER void csi_hal_extern_flash_SetWriteProtect(void)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_internal_flash_SetWriteProtect();
    #elif defined(CHIP_GC9005)
	ext_flash_setWriteProtect();
    #else
//	ext_flash_setWriteProtect();
	#endif
}

__FLASH_DRIVER void csi_hal_extern_flash_ClearWriteProtect(void)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_internal_flash_ClearWriteProtect();
    #elif defined(CHIP_GC9005)
	ext_flash_clrWriteProtect();
    #else
//	INTERNAL_FLASH_ClrWriteProtect();
	#endif
}

__FLASH_DRIVER uint32_t csi_hal_extern_flash_read(uint32_t addr, uint32_t len, uint8_t* buffer)
{
    #if defined(CHIP_GC9002)
    return gc9002_hal_internal_flash_read(addr, len, buffer);
    #elif defined(CHIP_GC9005)
	ext_flash_read(addr & 0x0FFFFFFF, len, buffer);
    #else
	ext_flash_read(addr, len, (unsigned char *)buffer);
	#endif
}

__FLASH_DRIVER void csi_hal_extern_flash_write(uint32_t addr, uint32_t len, uint8_t* buffer)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_internal_flash_write(addr, len, buffer);
    #elif defined(CHIP_GC9005)
	ext_flash_write(addr & 0x0FFFFFFF, len, buffer);
    #else
	ext_flash_write(addr, len, (unsigned char *)buffer);
	#endif
}

__FLASH_DRIVER void csi_hal_extern_flash_sector_erase(uint32_t addr)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_interal_flash_sector_erase(addr);
    #elif defined(CHIP_GC9005)
	ext_flash_sector_erase(addr); // nand flash do not support sector erase.
    #else
	ext_flash_erase(addr);
	#endif
}

__FLASH_DRIVER void csi_hal_extern_flash_block_erase(uint32_t addr)
{
    #if defined(CHIP_GC9002)
    gc9002_hal_internal_flash_block_erase(addr);
    #elif defined(CHIP_GC9005)
	ext_flash_erase(addr);
    #else

	#endif
}


