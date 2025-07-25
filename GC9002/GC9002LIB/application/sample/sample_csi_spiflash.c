#include "sample_def.h"
#include "csi_spiflash.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_SPIFLASH_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

/* example parameters */
#define EXAMPLE_FLASH_START_ADDRESS        0x100000
#define EXAMPLE_FLASH_WRITE_SIZE           64

static csi_spiflash_t spiflash;
static csi_spiflash_info_t flash_info;
QSPI_Flash_HandleTypeDef qspi = {.Instance = CQSPI};
int main(void)
{
    csi_error_t ret = CSI_OK;
    
    __attribute__((aligned(4))) uint8_t tx_data[EXAMPLE_FLASH_WRITE_SIZE];
    __attribute__((aligned(4))) uint8_t rx_data[EXAMPLE_FLASH_WRITE_SIZE];
    uint32_t address, size;
    __disable_excp_irq();
    printf("csi qspi flash test.\n");
    /* Initialize SPIFLASH based on spi controler */
    ret = csi_spiflash_qspi_init(&spiflash, 0, NULL);
    CHECK_RETURN(ret);
    printf("csi_spiflash_qspi_init success.\n");

    /* Get SPIFLASH information */
    ret = csi_spiflash_get_flash_info(&spiflash, &flash_info);
    CHECK_RETURN(ret);
    printf("csi_spiflash_get_flash_info success.\n");
    printf("capacity:0x%08X\nsector_size:0x%04X\n", flash_info.capacity, flash_info.sector_size);
    printf("erase addr:0x%08X.\n", flash_info.capacity-flash_info.sector_size);
    printf("erase size:0x%08X.\n", EXAMPLE_FLASH_WRITE_SIZE);
    /* Chip erase */
     ret = csi_spiflash_erase(&spiflash, flash_info.capacity-flash_info.sector_size, EXAMPLE_FLASH_WRITE_SIZE);
    CHECK_RETURN(ret);
    printf("csi_spiflash_erase success.\n");
    /* Chip erase */
//    ret = csi_spiflash_chip_erase(&spiflash);
//    CHECK_RETURN(ret);
    
    /* Prefill tx_data buffer */
    int num = 0;
    for(int i = 0; i < EXAMPLE_FLASH_WRITE_SIZE; i++)
    {
        tx_data[i] = num+1;
        num++;
        if(num >= 0xFF)
        {
            num = 0;
        }
    }
    /* Write data to flash */
    address = flash_info.capacity-flash_info.sector_size;
    size    = EXAMPLE_FLASH_WRITE_SIZE;
    if(csi_spiflash_program(&spiflash, address, tx_data, size) != size)
    {
        while(1);
    }
    printf("csi_spiflash_program success.\n");
    mdelay(500);  
    /* Read data from SPIFLASH */
    address = flash_info.capacity-flash_info.sector_size;
    size    = EXAMPLE_FLASH_WRITE_SIZE;
    if(csi_spiflash_read(&spiflash, address, rx_data, size) != size)
    {
        while(1);
    }
    printf("csi_spiflash_read success.\n");
    
    /* Data compare */
    ret = memcmp(tx_data, rx_data, size);
    CHECK_RETURN(ret);
    printf("csi flash test success.\n");
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_SPIFLASH_ */
#endif /* BSP_DEBUG */