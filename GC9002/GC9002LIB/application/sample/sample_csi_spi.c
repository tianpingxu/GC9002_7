#include "sample_def.h"
#include "csi_pin.h"
#include "csi_spi.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_SPI_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

csi_spi_t spi_master, spi_slave;

static void slave_spi_event_callback(struct _csi_spi *spi, csi_spi_event_t event, void *arg){
    switch(event)
    {
        case SPI_EVENT_TRANSMIT_COMPLETE:
            printf("SPI_EVENT_TRANSMIT_COMPLETE\n");
        break;
        case SPI_EVENT_RECEIVE_COMPLETE:
            printf("SPI_EVENT_RECEIVE_COMPLETE\n");
        break;
        case SPI_EVENT_TRANSMIT_RECEIVE_COMPLETE:
            printf("SPI_EVENT_TRANSMIT_RECEIVE_COMPLETE\n");
        break;
        case SPI_EVENT_ERROR:
            printf("SPI_EVENT_ERROR\n");
        break;
        default: break;
    }
}

#define EXAMPLE_PIN_SPI_SCK         PB13
#define EXAMPLE_PIN_SPI_CS          PB12
#define EXAMPLE_PIN_SPI_MOSI        PB11
#define EXAMPLE_PIN_SPI_MISO        PB10

#define EXAMPLE_PIN_SPI_SCK_FUNC    PB13_SPI0_SCK
#define EXAMPLE_PIN_SPI_CS_FUNC     PB12_SPI0_NSS
#define EXAMPLE_PIN_SPI_MOSI_FUNC   PB11_SPI0_MOSI
#define EXAMPLE_PIN_SPI_MISO_FUNC   PB10_SPI0_MISO
void example_pin_spi_init(void)
{
    csi_error_t ret;
    /* SPI0 GPIO Configuration
     * SPI0_SCK 		PB13	-AG25	    VIDEO_D3
     * SPI0_NSS 		PB12	-K30		EX2_IO6
     * SPI0_MOSI		PB11	-L27		EX2_IO5
     * SPI0_MISO		PB10	-L23 		EX2_IO4
     * */
    ret = csi_pin_set_mux(EXAMPLE_PIN_SPI_MISO,       EXAMPLE_PIN_SPI_MISO_FUNC);
    CHECK_RETURN(ret);
    ret = csi_pin_set_mux(EXAMPLE_PIN_SPI_MOSI,       EXAMPLE_PIN_SPI_MOSI_FUNC);
    CHECK_RETURN(ret);
    ret = csi_pin_set_mux(EXAMPLE_PIN_SPI_CS,         EXAMPLE_PIN_SPI_CS_FUNC);
    CHECK_RETURN(ret);
    ret = csi_pin_set_mux(EXAMPLE_PIN_SPI_SCK,        EXAMPLE_PIN_SPI_SCK_FUNC);
    CHECK_RETURN(ret);
}
#define EXAMPLE_SPI_IDX     0
int main(void)
{
    csi_error_t ret;
    uint8_t  tx_data[16];
//    uint8_t  rx_data[16];
//    uint32_t timestart;

//    example_pin_spi_init();

    /* Initialize master SPI */
    ret = csi_spi_init(&spi_master, EXAMPLE_SPI_IDX);
    CHECK_RETURN(ret);

    /* Set master SPI as master mode */
    ret = csi_spi_mode(&spi_master, SPI_MASTER);
    CHECK_RETURN(ret);

    ret = csi_spi_cp_format(&spi_master, SPI_FORMAT_CPOL0_CPHA0);
    CHECK_RETURN(ret);

    ret = csi_spi_frame_len(&spi_master, SPI_FRAME_LEN_8);
    CHECK_RETURN(ret);

    ret = csi_spi_baud(&spi_master, 100000);
    if(ret == 0){
        while(1);
    }

    for(int i = 0; i < sizeof(tx_data); i++)
    {
        tx_data[i] = i;
    }
    csi_spi_attach_callback(&spi_master, slave_spi_event_callback, NULL);
    /* Master SPI send data */
    if(csi_spi_send(&spi_master, tx_data, sizeof(tx_data), 0xFFFFFFFF) != sizeof(tx_data))
    {
        printf("csi_spi_send error\n");
        while(1);
    }
    ret = csi_spi_send_async(&spi_master, tx_data, sizeof(tx_data));
    printf("send: %s\n", "hello word");
    if (ret != sizeof(tx_data)) {
        while(1);
    }
    
    /* Uninit SPI */
    csi_spi_uninit(&spi_master);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_SPI_ */
#endif /* BSP_DEBUG */