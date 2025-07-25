#include "sample_def.h"
#include "csi_qspi.h"
#include "csi_spiflash.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_QSPI_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

#define W25Q64FV_READ_DEVICE_ID            0x9F
static csi_qspi_t     qspi_handle;
int main(void)
{
    csi_error_t ret;
    uint32_t device_id;

    ret = csi_qspi_init(&qspi_handle, 0);
    if (ret != CSI_OK)
    {
        while(1);
    }
    uint32_t freq = csi_qspi_frequence(&qspi_handle, CSI_QSPI_FREQ_MIN);
    if(freq != CSI_QSPI_FREQ_MIN)
    {
        while(1);
    }

    /* Read device id operations */
    command.instruction.value     = W25Q64FV_READ_DEVICE_ID;
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = true;
    command.address.value         = 0;
    command.address.size          = 0;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 0;
    
    if (csi_qspi_receive(&qspi_handle, &command, (uint8_t *)&device_id, 3, 0x0000FFFF) != 3)
    {
        while(1);
    }
    printf("device id: %x\n", device_id);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_QSPI_ */
#endif /* BSP_DEBUG */