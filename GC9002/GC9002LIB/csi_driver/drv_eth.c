/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-31     mazy         first version
 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "board.h"
#include "csi_irq.h"

#define LOG_ENABLE_I
#define LOG_ENABLE_W
#define LOG_ENABLE_E
#include "syslog.h"

#ifdef BSP_USING_ETH

#include "drv_eth.h"

/*
* Emac driver uses CubeMX tool to generate emac and phy's configuration,
* the configuration files can be found in CubeMX_Config folder.
*/

/* debug option */
//#define ETH_RX_DUMP
//#define ETH_TX_DUMP

#define MAX_ADDR_LEN 6

struct rt_gc90xx_eth
{
    /* interface address info, hw address */
    uint8_t  dev_addr[MAX_ADDR_LEN];
    /* ETH_Speed */
    uint32_t    ETH_Speed;
    /* ETH_Duplex_Mode */
    uint32_t    ETH_Mode;
};

static ETH_DMADescTypeDef *DMARxDscrTab = NULL, *DMATxDscrTab = NULL;
static uint8_t *Rx_Buff = NULL, *Tx_Buff = NULL;
static  ETH_HandleTypeDef EthHandle;
static struct rt_gc90xx_eth gc90xx_eth_device;

xSemaphoreHandle s_xSemaphore;


#if defined(ETH_RX_DUMP) || defined(ETH_TX_DUMP)
#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
static void dump_hex(const rt_uint8_t *ptr, rt_size_t buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;

    for (i = 0; i < buflen; i += 16)
    {
        printf("%08X: ", i);

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                printf("%02X ", buf[i + j]);
            else
                printf("   ");
        printf(" ");

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                printf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
        printf("\n");
    }
}
#endif

void Dump_ETH_Rx_Desc(void)
{
    uint32_t *pt = (uint32_t *)(DMARxDscrTab);

    if(NULL == pt) return;

    printf("\n\n");
    for(int i=0; i<ETH_RXBUFNB; i++)
    {
        printf("%02d %08X ", i, (u32)pt);
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X \n", *(pt++));
    }
}

void Dump_ETH_Tx_Desc(void)
{
    uint32_t *pt = (uint32_t *)(DMATxDscrTab);

    if(NULL == pt) return;

    printf("\n\n");
    for(int i=0; i<ETH_TXBUFNB; i++)
    {
        printf("%02d %08X ", i, (u32)pt);
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X ", *(pt++));
        printf("%08X \n", *(pt++));
    }
}

void Dump_ETH_DMA_Register(void)
{
    printf("\n=======================================\n");
    printf("DmaBusMode:%08X \n",      ETH->DMABMR);
    printf("DmaTxPollDemand:%08X \n", ETH->DMATPDR);
    printf("DmaRxPollDemand:%08X \n", ETH->DMARPDR);
    printf("DmaRxBaseAddr:%08X \n",   ETH->DMARDLAR);
    printf("DmaTxBaseAddr:%08X \n",   ETH->DMATDLAR);
    printf("DmaStatus:%08X \n",       ETH->DMASR);
    printf("DmaControl:%08X \n",      ETH->DMAOMR);
    printf("DmaInterrupt:%08X \n",    ETH->DMAIER);
    printf("DmaMissedFr:%08X \n",     ETH->DMAMFBOCR);
    printf("DmaTxCurrDesc:%08X \n",   ETH->DMACHTDR);
    printf("DmaRxCurrDesc:%08X \n",   ETH->DMACHRDR);
    printf("DmaTxCurrAddr:%08X \n",   ETH->DMACHTBAR);
    printf("DmaRxCurrAddr:%08X \n",   ETH->DMACHRBAR);
    printf("=======================================\n");
}

void Dump_ETH_MAC_Register(void)
{
    printf("\n=======================================\n");
    printf("GmacConfig:%08X \n",          ETH->MACCR);
    printf("GmacFrameFilter:%08X \n",     ETH->MACFFR);
    printf("GmacHashHigh:%08X \n",        ETH->MACHTHR);
    printf("GmacHashLow:%08X \n",         ETH->MACHTLR);
    printf("GmacGmiiAddr:%08X \n",        ETH->MACMIIAR);
    printf("GmacGmiiData:%08X \n",        ETH->MACMIIDR);
    printf("GmacFlowControl:%08X \n",     ETH->MACFCR);
    printf("GmacVersion:%08X \n",         ETH->MACVERR);
    printf("GmacPmtCtrlStatus:%08X \n",   ETH->MACPMTCSR);
    printf("GmacInterruptStatus:%08X \n", ETH->MACSR);
    printf("GmacInterruptMask:%08X \n",   ETH->MACIMR);
    printf("=======================================\n");
}

void Dump_ETH_PHY_Register(void)
{
    uint32_t val = 0;
    printf("\n=======================================\n");
    HAL_ETH_ReadPHYRegister(&EthHandle, 0x00, &val);
    printf("PHY BMCR:%04X \n",          val);
    val = 0;
    HAL_ETH_ReadPHYRegister(&EthHandle, 0x01, &val);
    printf("PHY BMSR:%04X \n",          val);
    val = 0;
    HAL_ETH_ReadPHYRegister(&EthHandle, 0x02, &val);
    printf("PHY ID1:%04X \n",          val);
    val = 0;
    HAL_ETH_ReadPHYRegister(&EthHandle, 0x1D, &val);
    printf("PHY INER:%04X \n",          val);
    val = 0;
    HAL_ETH_ReadPHYRegister(&EthHandle, 0x1E, &val);
    printf("PHY INSR:%04X \n",          val);
    val = 0;
    HAL_ETH_ReadPHYRegister(&EthHandle, 0x1F, &val);
    printf("PHY CR_SR:%04X \n",          val);
    printf("=======================================\n");
}

/* interrupt service routine */
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void GC_MAC_IRQHandler(void)
#else
static ATTRIBUTE_ISR void GC_MAC_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_ETH_IRQHandler(&EthHandle);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(s_xSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *heth)
{
    if (__HAL_ETH_DMA_GET_FLAG(heth, ETH_DMA_FLAG_RBU))
    {
        (heth->Instance)->DMARPDR = 0U;
    }
    if (__HAL_ETH_DMA_GET_FLAG(heth, ETH_DMA_FLAG_TBU))
    {
        (heth->Instance)->DMATPDR = 0U;
    }
    // LOG_E("eth err");
}

// extern void phy_reset(void);
/* EMAC initialization function */
static rt_err_t rt_gc90xx_eth_init(void)
{
    // ccm_FlushDCache();

    /* ETHERNET Configuration */
    EthHandle.Instance = ETH;
    EthHandle.Init.MACAddr = (uint8_t *)&gc90xx_eth_device.dev_addr[0];
    EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_DISABLE;
    EthHandle.Init.Speed = ETH_SPEED_100M;
    EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    EthHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
#ifdef RT_LWIP_USING_HW_CHECKSUM
    EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
#else
    EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_SOFTWARE;
#endif

    HAL_ETH_DeInit(&EthHandle);

    /* configure ethernet peripheral (GPIOs, clocks, MAC, DMA) */
    if (HAL_ETH_Init(&EthHandle) != HAL_OK)
    {
        LOG_E("eth hardware init failed");
    }
    else
    {
        // ccm_FlushInvalidateDCache();
        LOG_I("eth hardware init success");
    }

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, Tx_Buff, ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, Rx_Buff, ETH_RXBUFNB);

    // ccm_FlushDCache();

    /* Enable MAC and DMA transmission and reception */
    if (HAL_ETH_Start(&EthHandle) == HAL_OK)
    {
        LOG_D("emac hardware start");
    }
    else
    {
        LOG_E("emac hardware start faild");
        return -RT_ERROR;
    }

    /* ETH interrupt Init */
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(MAC_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, GC_MAC_IRQHandler);
#else
    csi_clic_register_irq(MAC_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, GC_MAC_IRQHandler);
#endif
    return RT_EOK;
}

/* ethernet device interface */
/* transmit data*/
int32_t low_level_output(uint8_t *buf, uint32_t bufLen)
{
    HAL_StatusTypeDef state;
    uint8_t *buffer = (uint8_t *)(EthHandle.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc;

    DmaTxDesc = EthHandle.TxDesc;

    // ccm_FlushInvalidateDCache();

    if(0 == DmaTxDesc->Buffer1Addr) return 0;

    memcpy(buffer, buf, bufLen);

#ifdef ETH_TX_DUMP
    dump_hex(buffer, p->tot_len);
#endif

    /* wait for unlocked */
    while (EthHandle.Lock == HAL_LOCKED);

    // ccm_FlushDCache();

    state = HAL_ETH_TransmitFrame(&EthHandle, bufLen);
    if (state != HAL_OK)
    {
        LOG_E("eth transmit frame faild: %d", state);
    }

    return 0;
}

/* receive data*/
int32_t low_level_input(uint8_t *buf)
{
    HAL_StatusTypeDef state;
    uint16_t len = 0;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;

    // ccm_FlushInvalidateDCache();

    /* Get received frame */
    state = HAL_ETH_GetReceivedFrame_IT(&EthHandle);
    if (state != HAL_OK)
    {
        LOG_D("receive frame faild state %08X", state);
        return 0;
    }

    /* Obtain the size of the packet and put it into the "len" variable. */
    len = EthHandle.RxFrameInfos.length;
    buffer = (uint8_t *)EthHandle.RxFrameInfos.buffer;

    memcpy(buf, buffer, len);

    /* Release descriptors to DMA */
    /* Point to first descriptor */
    dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
    /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
    dmarxdesc->Status |= ETH_DMARXDESC_OWN;

    /* Clear Segment_Count */
    EthHandle.RxFrameInfos.SegCount = 0;

    /* When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((EthHandle.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)
    {
        /* Clear RBUS ETHERNET DMA flag */
        EthHandle.Instance->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        EthHandle.Instance->DMARPDR = 1;
    }

    return len;
}

enum {
    PHY_LINK        = (1 << 0),
    PHY_100M        = (1 << 1),
    PHY_1000M       = (1 << 2),
    PHY_FULL_DUPLEX = (1 << 3),
};

static void phy_linkchange()
{
    static uint8_t phy_speed = 0;
    uint8_t phy_speed_new = 0;
    uint32_t status;

    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_BSR, (uint32_t *)&status);
    LOG_D("phy basic status reg is 0x%X", status);

    if (status & (PHY_AUTONEGO_COMPLETE | PHY_LINKED_STATUS))
    {
        uint32_t SR = 0;

        phy_speed_new |= PHY_LINK;

        HAL_ETH_ReadPHYRegister(&EthHandle, PHY_Status_REG, (uint32_t *)&SR);
        LOG_D("phy control status reg is 0x%X", SR);
    }

    if (phy_speed != phy_speed_new)
    {
        phy_speed = phy_speed_new;
        if (phy_speed & PHY_LINK)
        {
            LOG_I("link up");
            gc90xx_eth_device.ETH_Speed = EthHandle.Init.Speed;
            gc90xx_eth_device.ETH_Mode  = ETH_MODE_FULLDUPLEX;
        }
        else
        {
            LOG_I("link down");
        }
    }
}

#ifdef PHY_USING_INTERRUPT_MODE
static void eth_phy_isr(void *args)
{
    uint32_t status = 0;

    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_INTERRUPT_FLAG_REG, (uint32_t *)&status);
    LOG_D("phy interrupt status reg is 0x%X", status);

    phy_linkchange();
}
#endif /* PHY_USING_INTERRUPT_MODE */

void phy_monitor_thread_entry(void *parameter)
{
    uint8_t phy_addr = 0xFF;
    uint8_t detected_count = 0;

    while(phy_addr == 0xFF)
    {
        /* phy search */
        uint32_t i, temp;
        for (i = 0; i <= 0x1F; i++)
        {
            EthHandle.Init.PhyAddress = i;
            HAL_ETH_ReadPHYRegister(&EthHandle, PHY_ID1_REG, (uint32_t *)&temp);

            if (temp != 0xFFFF && temp != 0x00)
            {
                phy_addr = i;
                break;
            }
        }

        detected_count++;
        vTaskDelay(pdMS_TO_TICKS(100));

        if (detected_count > 10)
        {
            LOG_E("No PHY device was detected, please check hardware!");
        }
    }

    LOG_I("Found a phy, address:0x%02X", phy_addr);

    /* RESET PHY */
    LOG_D("RESET PHY!");
    HAL_ETH_WritePHYRegister(&EthHandle, PHY_BCR, PHY_RESET);
    vTaskDelay(pdMS_TO_TICKS(2000));
    HAL_ETH_WritePHYRegister(&EthHandle, PHY_BCR, PHY_FULLDUPLEX_100M | PHY_AUTONEGOTIATION);
    vTaskDelay(pdMS_TO_TICKS(500));

    // ccm_FlushDCache();

//     phy_linkchange();
// #ifdef PHY_USING_INTERRUPT_MODE
//     /* configuration intterrupt pin */
//     rt_pin_mode(PHY_INT_PIN, PIN_MODE_INPUT_PULLUP);
//     rt_pin_attach_irq(PHY_INT_PIN, PIN_IRQ_MODE_FALLING, eth_phy_isr, (void *)"callbackargs");
//     rt_pin_irq_enable(PHY_INT_PIN, PIN_IRQ_ENABLE);

//     /* enable phy interrupt */
//     HAL_ETH_WritePHYRegister(&EthHandle, PHY_INTERRUPT_MASK_REG, PHY_INT_MASK);
// #if defined(PHY_INTERRUPT_CTRL_REG)
//     HAL_ETH_WritePHYRegister(&EthHandle, PHY_INTERRUPT_CTRL_REG, PHY_INTERRUPT_EN);
// #endif
// #else /* PHY_USING_INTERRUPT_MODE */
//     TimerHandle_t Swtmr1_Handle = xTimerCreate((const char *)"phylnk", (TickType_t)100, pdTRUE, (void *)0, 
//                                                (TimerCallbackFunction_t)phy_linkchange);
//     if (Swtmr1_Handle != NULL)
//     {
//         xTimerStart(Swtmr1_Handle, 0);
//     }
// #endif /* PHY_USING_INTERRUPT_MODE */

    for(;;)
    {
        phy_linkchange();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* Register the EMAC device */
int Bsp_Eth_Init(void)
{
    int state = RT_EOK;

    /* Prepare receive and send buffers */
    Rx_Buff = (uint8_t *)rt_calloc(ETH_RXBUFNB, ETH_MAX_PACKET_SIZE);
    if (Rx_Buff == RT_NULL)
    {
        LOG_E("No memory");
        state = -RT_ENOMEM;
        goto __exit;
    }

    Tx_Buff = (uint8_t *)rt_calloc(ETH_TXBUFNB, ETH_MAX_PACKET_SIZE);
    if (Tx_Buff == RT_NULL)
    {
        LOG_E("No memory");
        state = -RT_ENOMEM;
        goto __exit;
    }

    DMARxDscrTab = (ETH_DMADescTypeDef *)rt_calloc(ETH_RXBUFNB, sizeof(ETH_DMADescTypeDef));
    if (DMARxDscrTab == RT_NULL)
    {
        LOG_E("No memory");
        state = -RT_ENOMEM;
        goto __exit;
    }

    DMATxDscrTab = (ETH_DMADescTypeDef *)rt_calloc(ETH_TXBUFNB, sizeof(ETH_DMADescTypeDef));
    if (DMATxDscrTab == RT_NULL)
    {
        LOG_E("No memory");
        state = -RT_ENOMEM;
        goto __exit;
    }

    gc90xx_eth_device.ETH_Speed = ETH_SPEED_100M;
    gc90xx_eth_device.ETH_Mode  = ETH_MODE_FULLDUPLEX;

    /* OUI 00-80-E1 STMICROELECTRONICS. */
    gc90xx_eth_device.dev_addr[0] = MAC_ADDR0;
    gc90xx_eth_device.dev_addr[1] = MAC_ADDR1;
    gc90xx_eth_device.dev_addr[2] = MAC_ADDR2;
    /* generate MAC addr from 96bit unique ID (only for test). */
    gc90xx_eth_device.dev_addr[3] = MAC_ADDR3;  //*(rt_uint8_t *)(UID_BASE + 4);
    gc90xx_eth_device.dev_addr[4] = MAC_ADDR4;  //*(rt_uint8_t *)(UID_BASE + 2);
    gc90xx_eth_device.dev_addr[5] = MAC_ADDR5;  //*(rt_uint8_t *)(UID_BASE + 0);

    int err = rt_gc90xx_eth_init();
    if (RT_EOK == err)
    {
        LOG_I("emac device init success");
    }
    else
    {
        LOG_E("emac device init faild: %d", state);
        state = -RT_ERROR;
        goto __exit;
    }

    /* start phy monitor */
    BaseType_t xReturn;
    xReturn = xTaskCreate(phy_monitor_thread_entry,
                           "phy",
                           512,
                           RT_NULL,
                           15,
                           RT_NULL);
    if(pdPASS == xReturn)
    {
        LOG_I("create phy monitor success");
    }
    else
    {
        state = -RT_ERROR;
    }
__exit:
    if (state != RT_EOK)
    {
        if (Rx_Buff)
        {
            rt_free(Rx_Buff);
        }

        if (Tx_Buff)
        {
            rt_free(Tx_Buff);
        }

        if (DMARxDscrTab)
        {
            rt_free(DMARxDscrTab);
        }

        if (DMATxDscrTab)
        {
            rt_free(DMATxDscrTab);
        }
    }

    return state;
}

#endif /* BSP_USING_ETH */


