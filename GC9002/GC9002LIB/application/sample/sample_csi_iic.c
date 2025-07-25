#include "sample_def.h"
#include "csi_pin.h"
#include "csi_iic.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_IIC_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

#define IIC_SLAVE_ADDR      0x50
#define IIC_IDX             0
static csi_iic_t master_iic;

#define Addr_Self  0x56
#define Addr_Dest  0x57
#define CSI_TEST_MODE  0//0-Master TX/RX   1-Slave TX/RX

static uint8_t write_data[16];
static uint8_t read_data[16];
static void sample_callback(csi_iic_t *iic, csi_iic_event_t event, void *arg)
{
    printf("irq:%d, event:%d\n", HANDLE_IRQ_NUM(iic), event);
}
    
int main(void)
{
    csi_error_t ret;
    uint32_t i;
    int32_t num;
    ret = csi_iic_init(&master_iic, IIC_IDX);
    if (ret != CSI_OK) {
        printf("csi_iic_initialize error\n"); 
        while(1);
    }
    /* config iic master mode */  
    ret = csi_iic_mode(&master_iic, IIC_MODE_MASTER);
    if (ret != CSI_OK) {
        printf("csi_iic_set_mode error\n");
        while(1);
    }
    /* config iic 7bit address mode */  
    ret = csi_iic_addr_mode(&master_iic, IIC_ADDRESS_7BIT);
    if (ret != CSI_OK) {
        printf("csi_iic_set_addr_mode error\n");
        while(1);
    }
    ret = csi_iic_own_addr(&master_iic, Addr_Self);
    if (ret != CSI_OK) {
        printf("csi_iic_own_addr error\n");
        while(1);
    }
    /* config iic standard speed*/  
    ret = csi_iic_speed(&master_iic, IIC_BUS_SPEED_FAST_PLUS);
    if (ret != CSI_OK) {
        printf("csi_iic_set_speed error\n");
        while(1);
    }
    ret = csi_iic_attach_callback(&master_iic, sample_callback, NULL);
    if (ret != CSI_OK) {
        printf("csi_iic_attach_callback error\n");
        while(1);
    }

    for (i = 0; i < sizeof(write_data); i++) { 
        write_data[i] = i;           ///< init write_data value
    }
#if(CSI_TEST_MODE == 0)
    num = csi_iic_master_send(&master_iic, Addr_Dest, write_data, sizeof(write_data), 100000);
    if (num != sizeof(write_data)) {
        printf("csi_iic_master_send error\n");
        while(1);
    }
    mdelay(100);
    num = csi_iic_master_receive(&master_iic, Addr_Dest, read_data, sizeof(read_data), 100000);
    if (num != sizeof(read_data)) {
        printf("csi_iic_master_receive error\n");
        while(1);
    }
    printf("I2C Master RX success, read_data={");
    for(i = 0; i < 16; i++)
    {
        printf("%x, ", read_data[i]);
    }
    printf("}\r\n");
#elif(CSI_TEST_MODE == 1)
    num = csi_iic_slave_receive(&master_iic, read_data, sizeof(read_data), 0xFFFFFFFF);
    if (num != sizeof(read_data)) {
        printf("csi_iic_master_receive error\n");
        while(1);
    }
    printf("I2C Slave RX success, read_data={");
    for(i = 0; i < 16; i++)
    {
        printf("%x, ", read_data[i]);
    }
    printf("}\r\n");
    num = csi_iic_slave_send(&master_iic, write_data, sizeof(write_data), 0xFFFFFFFF);
    if (num != sizeof(write_data)) {
        printf("csi_iic_master_send error\n");
        while(1);
    }
#endif
    csi_iic_uninit(&master_iic);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_IIC_ */
#endif /* BSP_DEBUG */