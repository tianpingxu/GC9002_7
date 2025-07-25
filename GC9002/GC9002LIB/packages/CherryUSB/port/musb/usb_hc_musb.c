/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * 发送接收共享同一个FIFO
 * ep0 FIFO大小是64字节
 * ep1-ep3 FIFO大小是128字节
 */
#include "usbh_core.h"
#include "usbh_hub.h"
#include "usb_musb_reg.h"
#include "gc90xx.h"
#include "csi_config.h"
#include "csi_irq.h"



#define HWREG(x)    (*((volatile uint32_t *)(x)))
#define HWREGH(x)   (*((volatile uint16_t *)(x)))
#define HWREGB(x)   (*((volatile uint8_t *)(x)))

#if CONFIG_USBHOST_PIPE_NUM != 4
#error musb host ip only supports 4 pipe num
#endif

#define MUSB_FADDR_OFFSET 0x00
#define MUSB_POWER_OFFSET 0x01
#define MUSB_TXIS1_OFFSET 0x02  //tx interrupt status (0..7)
#define MUSB_TXIS2_OFFSET 0x03  //tx interrupt status (8..15)
#define MUSB_RXIS1_OFFSET 0x04  //rx interrupt status (1..7)
#define MUSB_RXIS2_OFFSET 0x05  //rx interrupt status (8..15)
#define MUSB_IS_OFFSET    0x06  //interrupt status
#define MUSB_TXIE1_OFFSET 0x07  //tx interrupt enable
#define MUSB_TXIE2_OFFSET 0x08  //tx interrupt enable
#define MUSB_RXIE1_OFFSET 0x09  //rx interrupt enable
#define MUSB_RXIE2_OFFSET 0x0A  //rx interrupt enable
#define MUSB_IE_OFFSET    0x0B  //interrupt enable

#define MUSB_EPIDX_OFFSET  0x0E
#define MUSB_DEVCTL_OFFSET 0x0F

#define MUSB_IND_TXMAP_OFFSET      0x10
#define MUSB_IND_CSR0L_OFFSET      0x11
#define MUSB_IND_CSR0H_OFFSET      0x12
#define MUSB_IND_COUNT0_OFFSET     0x16
#define MUSB_IND_TXCSRL_OFFSET     0x11
#define MUSB_IND_TXCSRH_OFFSET     0x12
#define MUSB_IND_RXMAP_OFFSET      0x13
#define MUSB_IND_RXCSRL_OFFSET     0x14
#define MUSB_IND_RXCSRH_OFFSET     0x15
#define MUSB_IND_RXCOUNT1_OFFSET   0x16
#define MUSB_IND_RXCOUNT2_OFFSET   0x17
#define MUSB_IND_TXTYPE_OFFSET     0x18
#define MUSB_IND_TXINTERVAL_OFFSET 0x19
#define MUSB_IND_RXTYPE_OFFSET     0x1A
#define MUSB_IND_RXINTERVAL_OFFSET 0x1B

#define MUSB_FIFO_OFFSET           0x20
#define USB_FIFO_BASE(ep_idx)      (USB_BASE + MUSB_FIFO_OFFSET + 4*ep_idx)

#ifndef CONIFG_USB_MUSB_PIPE_NUM
#define CONIFG_USB_MUSB_PIPE_NUM   5
#endif

typedef enum {
    USB_EP0_STATE_SETUP = 0x0, /**< SETUP DATA */
    USB_EP0_STATE_IN_DATA,     /**< IN DATA */
    USB_EP0_STATE_IN_STATUS,   /**< IN status*/
    USB_EP0_STATE_OUT_DATA,    /**< OUT DATA */
    USB_EP0_STATE_OUT_STATUS,  /**< OUT status */
} ep0_state_t;

struct musb_pipe {
    uint8_t dev_addr;
    uint8_t ep_addr;
    uint8_t ep_type;
    uint8_t ep_interval;
    uint8_t speed;
    uint16_t ep_mps;
    bool inuse;
    uint32_t xfrd;
    volatile bool waiter;
    usb_osal_sem_t waitsem;
    struct usbh_hubport *hport;
    struct usbh_urb *urb;
};

struct musb_hcd {
    volatile bool port_csc;
    volatile bool port_pec;
    volatile bool port_pe;
    struct musb_pipe pipe_pool[CONFIG_USBHOST_PIPE_NUM][2]; /* Support Bidirectional ep */
} g_musb_hcd;

static volatile uint8_t usb_ep0_state = USB_EP0_STATE_SETUP;

#ifdef CONFIG_USB_USE_DMA
static DMA_HandleTypeDef usb_rx_hdma =
{
    .Instance = DMA,
};
static DMA_HandleTypeDef usb_tx_hdma =
{
    .Instance = DMA,
};

void USB_DMA_Config(void)
{
    HAL_DMA_StructInit(&(usb_rx_hdma.Init));
    usb_rx_hdma.Init.mChannel = CONFIG_USB_RX_DMA_CHANNEL;
    usb_rx_hdma.Init.mS_Address = 0;/*!< Specifies Destination Address.        */
    usb_rx_hdma.Init.mD_Address = 0;/*!< Specifies Source Address.             */
    usb_rx_hdma.Init.mBlockSize = 0;/*!<[1 ~ 1024]                             */
    usb_rx_hdma.Init.mS_AutoReload_EN = DMA_DISABLE;
    usb_rx_hdma.Init.mD_AutoReload_EN = DMA_DISABLE;
    usb_rx_hdma.Init.mS_TransferWidth = DMA_TR_WIDTH_8bits;
    usb_rx_hdma.Init.mD_TransferWidth = DMA_TR_WIDTH_8bits;
    usb_rx_hdma.Init.mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    usb_rx_hdma.Init.mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    usb_rx_hdma.Init.mS_AddrInc = DMA_FIXED;
    usb_rx_hdma.Init.mD_AddrInc = DMA_INCREMENTAL;
    usb_rx_hdma.Init.mChannelPriority = DMA_MAX_CHANNEL_PRIORITY;
    usb_rx_hdma.Init.mTransferType_FlowControl = DMA_TT_MEM_TO_MEM_DMAC;  //ÎÞÐèÎÕÊÖ
    usb_rx_hdma.Init.mS_Peripheral = DMA_HS_OTG_RX0;
    usb_rx_hdma.Init.mD_Peripheral = DMA_HS_MEMORY;
    usb_rx_hdma.Init.mEnable = DMA_DISABLE;

    HAL_DMA_StructInit(&(usb_tx_hdma.Init));
    usb_tx_hdma.Init.mChannel = CONFIG_USB_TX_DMA_CHANNEL;
    usb_tx_hdma.Init.mS_Address = 0;/*!< Specifies Source Address.             */
    usb_tx_hdma.Init.mD_Address = 0;/*!< Specifies Destination Address.        */
    usb_tx_hdma.Init.mBlockSize = 0;/*!<[1 ~ 1024]                             */
    usb_rx_hdma.Init.mS_AutoReload_EN = DMA_DISABLE;
    usb_rx_hdma.Init.mD_AutoReload_EN = DMA_DISABLE;
    usb_tx_hdma.Init.mS_TransferWidth = DMA_TR_WIDTH_8bits;
    usb_tx_hdma.Init.mD_TransferWidth = DMA_TR_WIDTH_8bits;
    usb_tx_hdma.Init.mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    usb_tx_hdma.Init.mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    usb_tx_hdma.Init.mS_AddrInc = DMA_INCREMENTAL;
    usb_tx_hdma.Init.mD_AddrInc = DMA_FIXED;
    usb_tx_hdma.Init.mChannelPriority = DMA_MAX_CHANNEL_PRIORITY;
    usb_tx_hdma.Init.mTransferType_FlowControl = DMA_TT_MEM_TO_PER_DMAC;
    usb_tx_hdma.Init.mS_Peripheral = DMA_HS_MEMORY;
    usb_tx_hdma.Init.mD_Peripheral = DMA_HS_OTG_TX0;
    usb_tx_hdma.Init.mEnable = DMA_DISABLE;
}

void Start_USB_RX_DMA(uint32_t srcAddr, uint32_t destAddr, uint32_t size, uint8_t ep_idx)
{
    usb_rx_hdma.Init.mS_Peripheral = DMA_HS_OTG_RX0 + ep_idx - 1;
    HAL_DMA_Init(&usb_rx_hdma);
    HAL_DMA_Start(&usb_rx_hdma, srcAddr, 0x02001000, size);
}

void Start_USB_TX_DMA(uint32_t srcAddr, uint32_t destAddr, uint32_t size, uint8_t ep_idx)
{
    usb_tx_hdma.Init.mD_Peripheral = DMA_HS_OTG_TX0 + ep_idx - 1;
    HAL_DMA_Init(&usb_tx_hdma);
    HAL_DMA_Start(&usb_tx_hdma, srcAddr, destAddr, size);
}
#endif

/* get current active ep */
static uint8_t musb_get_active_ep(void)
{
    return HWREGB(USB_BASE + MUSB_EPIDX_OFFSET);
}

/* set the active ep */
static void musb_set_active_ep(uint8_t ep_index)
{
    HWREGB(USB_BASE + MUSB_EPIDX_OFFSET) = ep_index;
}

static void musb_fifo_flush(uint8_t ep)
{
    uint8_t ep_idx = ep & 0x7f;
    if (ep_idx == 0) {
        if ((HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) & (USB_CSRL0_RXRDY | USB_CSRL0_TXRDY)) != 0)
            HWREGB(USB_BASE + MUSB_IND_CSR0H_OFFSET) |= USB_CSRH0_FLUSH;
    } else {
        if (ep & 0x80) {
            if (HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) & USB_TXCSRL1_TXRDY)
                HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) |= USB_TXCSRL1_FLUSH;
        } else {
            if (HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) & USB_RXCSRL1_RXRDY)
                HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) |= USB_RXCSRL1_FLUSH;
        }
    }
}

static void musb_write_packet(uint8_t ep_idx, uint8_t *buffer, uint16_t len)
{
    uint32_t *buf32;
    uint8_t *buf8;
    uint32_t count32;
    uint32_t count8;
    int i;
	
#ifdef CONFIG_USB_USE_DMA
    if(ep_idx != 0)
    {	
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) = USB_TXCSRH1_MODE | USB_TXCSRH1_DMAEN;  //tx and rx use the same fifo
        memcpy(0x02000000, buffer, len);
        Start_USB_TX_DMA(0x02000000, USB_FIFO_BASE(ep_idx), len, ep_idx);
        while(__HAL_DMA_CHANNEL_IS_ENABLE(&usb_tx_hdma));
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) &= ~USB_TXCSRH1_DMAEN;
        return ;
    }
#endif

    if ((uint32_t)buffer & 0x03) {
        buf8 = buffer;
        for (i = 0; i < len; i++) {
            HWREGB(USB_FIFO_BASE(ep_idx)) = *buf8++;
        }
    } else {
        count32 = len >> 2;
        count8 = len & 0x03;

        buf32 = (uint32_t *)buffer;

        while (count32--) {
            HWREG(USB_FIFO_BASE(ep_idx)) = *buf32++;
        }

        buf8 = (uint8_t *)buf32;

        while (count8--) {
            HWREGB(USB_FIFO_BASE(ep_idx)) = *buf8++;
        }
    }
}

static void musb_read_packet(uint8_t ep_idx, uint8_t *buffer, uint16_t len)
{
    uint32_t *buf32;
    uint8_t *buf8;
    uint32_t count32;
    uint32_t count8;
    int i;
	
#ifdef CONFIG_USB_USE_DMA
    if(ep_idx != 0)
    {
        HWREGB(USB_BASE + MUSB_IND_RXCSRH_OFFSET) = USB_RXCSRH1_DMAEN;
        Start_USB_RX_DMA(USB_FIFO_BASE(ep_idx), (uint32_t)buffer, len, ep_idx);
        while(__HAL_DMA_CHANNEL_IS_ENABLE(&usb_rx_hdma));
		memcpy(buffer, 0x02001000, len);
        HWREGB(USB_BASE + MUSB_IND_RXCSRH_OFFSET) &= ~USB_RXCSRH1_DMAEN;
        return ;
    }
#endif

    if ((uint32_t)buffer & 0x03) {
        buf8 = buffer;
        for (i = 0; i < len; i++) {
            *buf8++ = HWREGB(USB_FIFO_BASE(ep_idx));
        }
    } else {
        count32 = len >> 2;
        count8 = len & 0x03;

        buf32 = (uint32_t *)buffer;

        while (count32--) {
            *buf32++ = HWREG(USB_FIFO_BASE(ep_idx));
        }

        buf8 = (uint8_t *)buf32;

        while (count8--) {
            *buf8++ = HWREGB(USB_FIFO_BASE(ep_idx));
        }
    }
}

void musb_control_pipe_init(struct musb_pipe *pipe, struct usb_setup_packet *setup, uint8_t *buffer, uint32_t buflen)
{
    uint8_t old_ep_index;

    old_ep_index = musb_get_active_ep();
    musb_set_active_ep(0);
    musb_write_packet(0, (uint8_t *)setup, 8);
    HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = USB_CSRL0_TXRDY | USB_CSRL0_SETUP;
    musb_set_active_ep(old_ep_index);
}

void musb_bulk_pipe_init(struct musb_pipe *pipe, uint8_t *buffer, uint32_t buflen)
{
    uint8_t ep_idx;
    uint8_t old_ep_index;

    ep_idx = pipe->ep_addr & 0x7F;
    old_ep_index = musb_get_active_ep();
    musb_set_active_ep(ep_idx);

    HWREGB(USB_BASE + MUSB_FADDR_OFFSET) = pipe->dev_addr & 0x7F;
    if (pipe->ep_addr & 0x80) {
        HWREGB(USB_BASE + MUSB_IND_RXTYPE_OFFSET) = ep_idx | USB_RXTYPE1_PROTO_BULK;
        HWREGB(USB_BASE + MUSB_IND_RXINTERVAL_OFFSET) = 0;
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) &= ~USB_TXCSRH1_MODE;
        HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) = USB_RXCSRL1_REQPKT;
        HWREGB(USB_BASE + MUSB_IND_RXMAP_OFFSET) = 16;
    } else {
        HWREGB(USB_BASE + MUSB_IND_TXTYPE_OFFSET) = ep_idx | USB_TXTYPE1_PROTO_BULK;
        HWREGB(USB_BASE + MUSB_IND_TXINTERVAL_OFFSET) = 0;
        HWREGB(USB_BASE + MUSB_IND_TXMAP_OFFSET) = 16;  //16*8=128, 如果没有使用autoset则不用关心此寄存器数值

        if (buflen > pipe->ep_mps) {
            buflen = pipe->ep_mps;
        }

        musb_write_packet(ep_idx, buffer, buflen);
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) &= ~USB_TXCSRH1_MODE;
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) |= USB_TXCSRH1_MODE;
        HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) = USB_TXCSRL1_TXRDY;
    }
    musb_set_active_ep(old_ep_index);
}

void musb_intr_pipe_init(struct musb_pipe *pipe, uint8_t *buffer, uint32_t buflen)
{
    uint8_t ep_idx;
    uint8_t old_ep_index;

    ep_idx = pipe->ep_addr & 0x7f;
    old_ep_index = musb_get_active_ep();
    musb_set_active_ep(ep_idx);

    if (pipe->ep_addr & 0x80) {
        HWREGB(USB_BASE + MUSB_IND_RXTYPE_OFFSET) = ep_idx | USB_RXTYPE1_PROTO_INT;
        HWREGB(USB_BASE + MUSB_IND_RXINTERVAL_OFFSET) = pipe->ep_interval;
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) &= ~USB_TXCSRH1_MODE;
        HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) = USB_RXCSRL1_REQPKT;
    } else {
        HWREGB(USB_BASE + MUSB_IND_TXTYPE_OFFSET) = ep_idx | USB_TXTYPE1_PROTO_INT;
        HWREGB(USB_BASE + MUSB_IND_TXINTERVAL_OFFSET) = pipe->ep_interval;

        if (buflen > pipe->ep_mps) {
            buflen = pipe->ep_mps;
        }

        musb_write_packet(ep_idx, buffer, buflen);
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) &= ~USB_TXCSRH1_MODE;
        HWREGB(USB_BASE + MUSB_IND_TXCSRH_OFFSET) |= USB_TXCSRH1_MODE;
        HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) = USB_TXCSRL1_TXRDY;
    }
    musb_set_active_ep(old_ep_index);
}

static int usbh_reset_port(const uint8_t port)
{
    g_musb_hcd.port_pe = 0;
    HWREGB(USB_BASE + MUSB_POWER_OFFSET) |= USB_POWER_RESET;
    usb_osal_msleep(20);
    HWREGB(USB_BASE + MUSB_POWER_OFFSET) &= ~(USB_POWER_RESET);
    usb_osal_msleep(20);
    g_musb_hcd.port_pe = 1;
    return 0;
}

static uint8_t usbh_get_port_speed(const uint8_t port)
{
    uint8_t speed;

    if (HWREGB(USB_BASE + MUSB_POWER_OFFSET) & USB_POWER_HSMODE)
        speed = USB_SPEED_HIGH;
    else if (HWREGB(USB_BASE + MUSB_DEVCTL_OFFSET) & USB_DEVCTL_FSDEV)
        speed = USB_SPEED_FULL;
    else if (HWREGB(USB_BASE + MUSB_DEVCTL_OFFSET) & USB_DEVCTL_LSDEV)
        speed = USB_SPEED_LOW;

    return speed;
}

__WEAK void usb_hc_low_level_init(void)
{
}

int usb_hc_init(void)
{
    uint8_t regval;
    uint32_t fifo_offset = 0;

    memset(&g_musb_hcd, 0, sizeof(struct musb_hcd));

    for (uint8_t i = 0; i < CONFIG_USBHOST_PIPE_NUM; i++) {
        g_musb_hcd.pipe_pool[i][0].waitsem = usb_osal_sem_create(0);
        g_musb_hcd.pipe_pool[i][1].waitsem = usb_osal_sem_create(0);
    }

#ifdef CONFIG_USB_USE_DMA
    USB_DMA_Config();
#endif

    usb_hc_low_level_init();

    musb_set_active_ep(0);
    HWREGB(USB_BASE + MUSB_IND_TXINTERVAL_OFFSET) = 0;

    /* Enable USB interrupts */
    regval = USB_IE_RESET | USB_IE_CONN | USB_IE_DISCON |
             USB_IE_RESUME | USB_IE_SUSPND |
             USB_IE_BABBLE | USB_IE_SESREQ | USB_IE_VBUSERR;

    HWREGB(USB_BASE + MUSB_IE_OFFSET) = regval;
    HWREGB(USB_BASE + MUSB_TXIE1_OFFSET) = USB_TXIE_EP0;
    HWREGB(USB_BASE + MUSB_TXIE2_OFFSET) = 0;
    HWREGB(USB_BASE + MUSB_RXIE1_OFFSET) = 0;
    HWREGB(USB_BASE + MUSB_RXIE2_OFFSET) = 0;

    //HWREGB(USB_BASE + MUSB_POWER_OFFSET) |= USB_POWER_HSENAB;

    HWREGB(USB_BASE + MUSB_DEVCTL_OFFSET) |= USB_DEVCTL_SESSION;

#ifdef CONFIG_USB_MUSB_SUNXI
    musb_set_active_ep(0);
    HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = USB_CSRL0_TXRDY;
#endif

    return 0;
}

int usbh_roothub_control(struct usb_setup_packet *setup, uint8_t *buf)
{
    __IO uint32_t hprt0;
    uint8_t nports;
    uint8_t port;
    uint32_t status;

    nports = CONFIG_USBHOST_MAX_RHPORTS;
    port = setup->wIndex;
    if (setup->bmRequestType & USB_REQUEST_RECIPIENT_DEVICE) {
        switch (setup->bRequest) {
            case HUB_REQUEST_CLEAR_FEATURE:
                switch (setup->wValue) {
                    case HUB_FEATURE_HUB_C_LOCALPOWER:
                        break;
                    case HUB_FEATURE_HUB_C_OVERCURRENT:
                        break;
                    default:
                        return -EPIPE;
                }
                break;
            case HUB_REQUEST_SET_FEATURE:
                switch (setup->wValue) {
                    case HUB_FEATURE_HUB_C_LOCALPOWER:
                        break;
                    case HUB_FEATURE_HUB_C_OVERCURRENT:
                        break;
                    default:
                        return -EPIPE;
                }
                break;
            case HUB_REQUEST_GET_DESCRIPTOR:
                break;
            case HUB_REQUEST_GET_STATUS:
                memset(buf, 0, 4);
                break;
            default:
                break;
        }
    } else if (setup->bmRequestType & USB_REQUEST_RECIPIENT_OTHER) {
        switch (setup->bRequest) {
            case HUB_REQUEST_CLEAR_FEATURE:
                if (!port || port > nports) {
                    return -EPIPE;
                }

                switch (setup->wValue) {
                    case HUB_PORT_FEATURE_ENABLE:
                        break;
                    case HUB_PORT_FEATURE_SUSPEND:
                    case HUB_PORT_FEATURE_C_SUSPEND:
                        break;
                    case HUB_PORT_FEATURE_POWER:
                        break;
                    case HUB_PORT_FEATURE_C_CONNECTION:
                        g_musb_hcd.port_csc = 0;
                        break;
                    case HUB_PORT_FEATURE_C_ENABLE:
                        g_musb_hcd.port_pec = 0;
                        break;
                    case HUB_PORT_FEATURE_C_OVER_CURREN:
                        break;
                    case HUB_PORT_FEATURE_C_RESET:
                        break;
                    default:
                        return -EPIPE;
                }
                break;
            case HUB_REQUEST_SET_FEATURE:
                if (!port || port > nports) {
                    return -EPIPE;
                }

                switch (setup->wValue) {
                    case HUB_PORT_FEATURE_SUSPEND:
                        break;
                    case HUB_PORT_FEATURE_POWER:
                        break;
                    case HUB_PORT_FEATURE_RESET:
                        usbh_reset_port(port);
                        break;

                    default:
                        return -EPIPE;
                }
                break;
            case HUB_REQUEST_GET_STATUS:
                if (!port || port > nports) {
                    return -EPIPE;
                }

                status = 0;
                if (g_musb_hcd.port_csc) {
                    status |= (1 << HUB_PORT_FEATURE_C_CONNECTION);
                }
                if (g_musb_hcd.port_pec) {
                    status |= (1 << HUB_PORT_FEATURE_C_ENABLE);
                }

                if (g_musb_hcd.port_pe) {
                    status |= (1 << HUB_PORT_FEATURE_CONNECTION);
                    status |= (1 << HUB_PORT_FEATURE_ENABLE);
                    if (usbh_get_port_speed(port) == USB_SPEED_LOW) {
                        status |= (1 << HUB_PORT_FEATURE_LOWSPEED);
                    } else if (usbh_get_port_speed(port) == USB_SPEED_HIGH) {
                        status |= (1 << HUB_PORT_FEATURE_HIGHSPEED);
                    }
                }

                memcpy(buf, &status, 4);
                break;
            default:
                break;
        }
    }
    return 0;
}

int usbh_ep0_pipe_reconfigure(usbh_pipe_t pipe, uint8_t dev_addr, uint8_t ep_mps, uint8_t speed)
{
    struct musb_pipe *ppipe = (struct musb_pipe *)pipe;

    ppipe->dev_addr = dev_addr;
    ppipe->ep_mps = ep_mps;

    if (speed == USB_SPEED_HIGH) {
        ppipe->speed = USB_TYPE0_SPEED_HIGH;
    } else if (speed == USB_SPEED_FULL) {
        ppipe->speed = USB_TYPE0_SPEED_FULL;
    } else if (speed == USB_SPEED_LOW) {
        ppipe->speed = USB_TYPE0_SPEED_LOW;
    }

    return 0;
}

int usbh_pipe_alloc(usbh_pipe_t *pipe, const struct usbh_endpoint_cfg *ep_cfg)
{
    struct musb_pipe *ppipe;
    uint8_t old_ep_index;
    uint8_t ep_idx;
    usb_osal_sem_t waitsem;

    ep_idx = ep_cfg->ep_addr & 0x7f;

    if (ep_idx > CONIFG_USB_MUSB_PIPE_NUM) {
        return -ENOMEM;
    }

    old_ep_index = musb_get_active_ep();
    musb_set_active_ep(ep_idx);

    if (ep_cfg->ep_addr & 0x80) {
        ppipe = &g_musb_hcd.pipe_pool[ep_idx][1];
    } else {
        ppipe = &g_musb_hcd.pipe_pool[ep_idx][0];
    }

    /* store variables */
    waitsem = ppipe->waitsem;

    memset(ppipe, 0, sizeof(struct musb_pipe));

    ppipe->ep_addr = ep_cfg->ep_addr;
    ppipe->ep_type = ep_cfg->ep_type;
    ppipe->ep_mps = ep_cfg->ep_mps;
    ppipe->ep_interval = ep_cfg->ep_interval;
    ppipe->speed = ep_cfg->hport->speed;
    ppipe->dev_addr = ep_cfg->hport->dev_addr;
    ppipe->hport = ep_cfg->hport;

    if (ep_cfg->ep_type == USB_ENDPOINT_TYPE_CONTROL) {
    } else {
        if (ppipe->speed == USB_SPEED_HIGH) {
            ppipe->speed = USB_TXTYPE1_SPEED_HIGH;
        } else if (ppipe->speed == USB_SPEED_FULL) {
            ppipe->speed = USB_TXTYPE1_SPEED_FULL;
        } else if (ppipe->speed == USB_SPEED_LOW) {
            ppipe->speed = USB_TXTYPE1_SPEED_LOW;
        }

        if (ppipe->ep_addr & 0x80) {
            HWREGB(USB_BASE + MUSB_RXIE1_OFFSET) |= (1 << ep_idx);
        } else {
            HWREGB(USB_BASE + MUSB_TXIE1_OFFSET) |= (1 << ep_idx);
        }
    }
    /* restore variable */
    ppipe->inuse = true;
    ppipe->waitsem = waitsem;

    musb_set_active_ep(old_ep_index);
    *pipe = (usbh_pipe_t)ppipe;
    return 0;
}

int usbh_pipe_free(usbh_pipe_t pipe)
{
    return 0;
}

int usbh_submit_urb(struct usbh_urb *urb)
{
    struct musb_pipe *pipe;
    size_t flags;
    int ret = 0;

    if (!urb) {
        return -EINVAL;
    }

    pipe = urb->pipe;

    if (!pipe) {
        return -EINVAL;
    }

    flags = usb_osal_enter_critical_section();

    if (!pipe->hport->connected) {
        usb_osal_leave_critical_section(flags);
        return -ENODEV;
    }

    if (pipe->urb) {
        usb_osal_leave_critical_section(flags);
        return -EBUSY;
    }

    pipe->waiter = false;
    pipe->xfrd = 0;
    pipe->urb = urb;
    urb->errorcode = -EBUSY;
    urb->actual_length = 0;

    if (urb->timeout > 0) {
        pipe->waiter = true;
    }
    
    usb_osal_leave_critical_section(flags);

    switch (pipe->ep_type) {
        case USB_ENDPOINT_TYPE_CONTROL:
            usb_ep0_state = USB_EP0_STATE_SETUP;
            musb_control_pipe_init(pipe, urb->setup, urb->transfer_buffer, urb->transfer_buffer_length);
            if(urb->setup->bRequest == USB_REQUEST_SET_ADDRESS)
            {
                usb_osal_msleep(10);
                HWREGB(USB_BASE + MUSB_FADDR_OFFSET) = urb->setup->wValue & 0x7F;
            }
            break;
        case USB_ENDPOINT_TYPE_BULK:
            musb_bulk_pipe_init(pipe, urb->transfer_buffer, urb->transfer_buffer_length);
            break;
        case USB_ENDPOINT_TYPE_INTERRUPT:
            musb_intr_pipe_init(pipe, urb->transfer_buffer, urb->transfer_buffer_length);
            break;
        case USB_ENDPOINT_TYPE_ISOCHRONOUS:
            break;
        default:
            break;
    }   
    
    if (urb->timeout > 0) {
        /* wait until timeout or sem give */
        ret = usb_osal_sem_take(pipe->waitsem, urb->timeout);
        if (ret < 0) {
            goto errout_timeout;
        }

        ret = urb->errorcode;
    }
    return ret;
errout_timeout:
    pipe->waiter = false;
    usbh_kill_urb(urb);
    return ret;
}

int usbh_kill_urb(struct usbh_urb *urb)
{
    return 0;
}

static inline void musb_pipe_waitup(struct musb_pipe *pipe)
{
    struct usbh_urb *urb;

    urb = pipe->urb;
    pipe->urb = NULL;

    if(urb == NULL) {
        return ;
    }

    if (pipe->waiter) {
        pipe->waiter = false;
        usb_osal_sem_give(pipe->waitsem);
    }

    if (urb->complete) {
        if (urb->errorcode < 0) {
            urb->complete(urb->arg, urb->errorcode);
        } else {
            urb->complete(urb->arg, urb->actual_length);
        }
    }
}

void handle_ep0(void)
{
    uint8_t ep0_status;
    struct musb_pipe *pipe;
    struct usbh_urb *urb;
    uint32_t size;

    pipe = (struct musb_pipe *)&g_musb_hcd.pipe_pool[0][0];
    urb = pipe->urb;

    musb_set_active_ep(0);
    ep0_status = HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET);
    if (ep0_status & USB_CSRL0_STALLED) {
        HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) &= ~USB_CSRL0_STALLED;
        usb_ep0_state = USB_EP0_STATE_SETUP;
        urb->errorcode = -EPERM;
        musb_pipe_waitup(pipe);
        return;
    }
    if (ep0_status & USB_CSRL0_ERROR) {
        HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) &= ~USB_CSRL0_ERROR;
        musb_fifo_flush(0);
        usb_ep0_state = USB_EP0_STATE_SETUP;
        urb->errorcode = -EIO;
        musb_pipe_waitup(pipe);
        return;
    }
    if (ep0_status & USB_CSRL0_STALL) {
        HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) &= ~USB_CSRL0_STALL;
        usb_ep0_state = USB_EP0_STATE_SETUP;
        urb->errorcode = -EPERM;
        musb_pipe_waitup(pipe);
        return;
    }

    switch (usb_ep0_state) {
        case USB_EP0_STATE_SETUP:
            urb->actual_length += 8;
            if (urb->transfer_buffer_length) {
                if (urb->setup->bmRequestType & 0x80) {
                    usb_ep0_state = USB_EP0_STATE_IN_DATA;
                    HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = USB_CSRL0_REQPKT;
                } else {
                    usb_ep0_state = USB_EP0_STATE_OUT_DATA;
                    size = urb->transfer_buffer_length;
                    if (size > pipe->ep_mps) {
                        size = pipe->ep_mps;
                    }

                    musb_write_packet(0, urb->transfer_buffer, size);
                    HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = USB_CSRL0_TXRDY;

                    urb->transfer_buffer += size;
                    urb->transfer_buffer_length -= size;
                    urb->actual_length += size;
                }
            } else {
                usb_ep0_state = USB_EP0_STATE_IN_STATUS;
                HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = (USB_CSRL0_REQPKT | USB_CSRL0_STATUS);
            }
            break;
        case USB_EP0_STATE_IN_DATA:
            if (ep0_status & USB_CSRL0_RXRDY) {
                size = urb->transfer_buffer_length;
                if (size > pipe->ep_mps) {
                    size = pipe->ep_mps;
                }

                //ep0 use count0 and count0 use D0-D6
                size = MIN(size, (0x7F & HWREGB(USB_BASE + MUSB_IND_COUNT0_OFFSET)));
                musb_read_packet(0, urb->transfer_buffer, size);
                HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) &= ~USB_CSRL0_RXRDY;
                urb->transfer_buffer += size;
                urb->transfer_buffer_length -= size;
                urb->actual_length += size;

                if ((size < pipe->ep_mps) || (urb->transfer_buffer_length == 0)) {
                    usb_ep0_state = USB_EP0_STATE_OUT_STATUS;
                    HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = (USB_CSRL0_TXRDY | USB_CSRL0_STATUS);
                } else {
                    HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = USB_CSRL0_REQPKT;
                }
            }
            break;
        case USB_EP0_STATE_OUT_DATA:
            if (urb->transfer_buffer_length > 0) {
                size = urb->transfer_buffer_length;
                if (size > pipe->ep_mps) {
                    size = pipe->ep_mps;
                }

                musb_write_packet(0, urb->transfer_buffer, size);
                HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = USB_CSRL0_TXRDY;

                urb->transfer_buffer += size;
                urb->transfer_buffer_length -= size;
                urb->actual_length += size;
            } else {
                usb_ep0_state = USB_EP0_STATE_IN_STATUS;
                HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) = (USB_CSRL0_REQPKT | USB_CSRL0_STATUS);
            }
            break;
        case USB_EP0_STATE_OUT_STATUS:
            urb->errorcode = 0;
            musb_pipe_waitup(pipe);
            break;
        case USB_EP0_STATE_IN_STATUS:
            if (ep0_status & (USB_CSRL0_RXRDY | USB_CSRL0_STATUS)) {
                HWREGB(USB_BASE + MUSB_IND_CSR0L_OFFSET) &= ~(USB_CSRL0_RXRDY | USB_CSRL0_STATUS);
                urb->errorcode = 0;
                musb_pipe_waitup(pipe);
            }
            break;
    }
}

extern uint32_t g_u32USBConnFlag;

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
void OTG_IRQHandler(void)
#else
__attribute__ ((interrupt ("machine"))) void OTG_IRQHandler(void)
#endif
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

    uint32_t is;
    uint32_t txis = 0, txie = 0;
    uint32_t rxis = 0, rxie = 0;
    uint8_t ep_csrl_status;
    // uint8_t ep_csrh_status;
    struct musb_pipe *pipe;
    struct usbh_urb *urb;
    uint8_t ep_idx;
    uint8_t old_ep_idx;
    uint32_t count;
    uint8_t jump_pipe_wait = 0;

    is = HWREGB(USB_BASE + MUSB_IS_OFFSET);
    txis = HWREGB(USB_BASE + MUSB_TXIS2_OFFSET);
    txis <<= 8;
    txis |= HWREGB(USB_BASE + MUSB_TXIS1_OFFSET);
    rxis = HWREGB(USB_BASE + MUSB_RXIS2_OFFSET);
    rxis <<= 8;
    rxis |= HWREGB(USB_BASE + MUSB_RXIS1_OFFSET);

    HWREGB(USB_BASE + MUSB_IS_OFFSET) = is;

    old_ep_idx = musb_get_active_ep();

    g_u32USBConnFlag++;

    if (is & USB_IS_CONN) {
        g_musb_hcd.port_csc = 1;
        g_musb_hcd.port_pec = 1;
        g_musb_hcd.port_pe = 1;
        usbh_roothub_thread_wakeup(1);
    }

    if (is & USB_IS_DISCON) {
        g_musb_hcd.port_csc = 1;
        g_musb_hcd.port_pec = 1;
        g_musb_hcd.port_pe = 0;
        for (uint8_t index = 0; index < CONFIG_USBHOST_PIPE_NUM; index++) {
            for (uint8_t j = 0; j < 2; j++) {
                struct musb_pipe *pipe = &g_musb_hcd.pipe_pool[index][j];
                struct usbh_urb *urb = pipe->urb;
                if (pipe->waiter) {
                    pipe->waiter = false;
                    urb->errorcode = -ESHUTDOWN;
                    usb_osal_sem_give(pipe->waitsem);
                }
            }
        }
        usbh_roothub_thread_wakeup(1);
    }

    if (is & USB_IS_SOF) {
    }

    if (is & USB_IS_RESUME) {
    }

    if (is & USB_IS_SUSPEND) {
    }

    if (is & USB_IS_VBUSERR) {
    }

    if (is & USB_IS_SESREQ) {
    }

    if (is & USB_IS_BABBLE) {
    }

    txie = HWREGB(USB_BASE + MUSB_TXIE2_OFFSET);
    txie <<= 8;
    txie |= HWREGB(USB_BASE + MUSB_TXIE1_OFFSET);
    txis &= txie;
    /* Handle EP0 interrupt */
    if (txis & USB_TXIE_EP0) {
        txis &= ~USB_TXIE_EP0;
        HWREGB(USB_BASE + MUSB_TXIS1_OFFSET) = USB_TXIE_EP0;
        handle_ep0();
    }

    for (ep_idx = 1; ep_idx < CONIFG_USB_MUSB_PIPE_NUM; ep_idx++) {
        if (txis & (1 << ep_idx)) {
            if(ep_idx >= 8) {
                HWREGB(USB_BASE + MUSB_TXIS2_OFFSET) = (1 << (ep_idx-8));
            } else {
                HWREGB(USB_BASE + MUSB_TXIS1_OFFSET) = (1 << ep_idx);
            }

            pipe = &g_musb_hcd.pipe_pool[ep_idx][0];
            urb = pipe->urb;
            musb_set_active_ep(ep_idx);

            ep_csrl_status = HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET);

            if (ep_csrl_status & USB_TXCSRL1_ERROR) {
                HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) &= ~USB_TXCSRL1_ERROR;
                urb->errorcode = -EIO;
                goto pipe_wait;
            } else if (ep_csrl_status & USB_TXCSRL1_NAKTO) {
                HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) &= ~USB_TXCSRL1_NAKTO;
                urb->errorcode = -EBUSY;
                goto pipe_wait;
            } else if (ep_csrl_status & USB_TXCSRL1_STALL) {
                HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) &= ~USB_TXCSRL1_STALL;
                urb->errorcode = -EPERM;
                goto pipe_wait;
            } else {
                uint32_t size = urb->transfer_buffer_length;

                if (size > pipe->ep_mps) {
                    size = pipe->ep_mps;
                }

                urb->transfer_buffer += size;
                urb->transfer_buffer_length -= size;
                urb->actual_length += size;

                if (urb->transfer_buffer_length == 0) {
                    urb->errorcode = 0;
                    goto pipe_wait;
                } else {
                    musb_write_packet(ep_idx, urb->transfer_buffer, size);
                    HWREGB(USB_BASE + MUSB_IND_TXCSRL_OFFSET) = USB_TXCSRL1_TXRDY;
                }
            }
        }
    }

    rxie = HWREGB(USB_BASE + MUSB_RXIE2_OFFSET);
    rxie <<= 8;
    rxie |= HWREGB(USB_BASE + MUSB_RXIE1_OFFSET);
    rxis &= rxie;
    for (ep_idx = 1; ep_idx < CONIFG_USB_MUSB_PIPE_NUM; ep_idx++) {
        if (rxis & (1 << ep_idx)) {
            if(ep_idx >= 8) {
                HWREGB(USB_BASE + MUSB_RXIS2_OFFSET) = (1 << (ep_idx-8)); // clear isr flag
            } else {
                HWREGB(USB_BASE + MUSB_RXIS1_OFFSET) = (1 << ep_idx);
            }

            pipe = &g_musb_hcd.pipe_pool[ep_idx][1];
            urb = pipe->urb;
            musb_set_active_ep(ep_idx);

            ep_csrl_status = HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET);
            //ep_csrh_status = HWREGB(USB_BASE + MUSB_IND_RXCSRH_OFFSET); // todo:for iso transfer

            if (ep_csrl_status & USB_RXCSRL1_ERROR) {
                HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) &= ~USB_RXCSRL1_ERROR;
                urb->errorcode = -EIO;
                goto pipe_wait;
            } else if (ep_csrl_status & USB_RXCSRL1_NAKTO) {
                HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) &= ~USB_RXCSRL1_NAKTO;
                urb->errorcode = -EBUSY;
                goto pipe_wait;
            } else if (ep_csrl_status & USB_RXCSRL1_STALL) {
                HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) &= ~USB_RXCSRL1_STALL;
                urb->errorcode = -EPERM;
                goto pipe_wait;
            } else if (ep_csrl_status & USB_RXCSRL1_RXRDY) {
                uint32_t size = urb->transfer_buffer_length;
                if (size > pipe->ep_mps) {
                    size = pipe->ep_mps;
                }
                count = HWREGB(USB_BASE + MUSB_IND_RXCOUNT2_OFFSET) & 0x07;  //only D0-D2 valid
                count <<= 8;
                count |= HWREGB(USB_BASE + MUSB_IND_RXCOUNT1_OFFSET);
                size = MIN(size, count);

                musb_read_packet(ep_idx, urb->transfer_buffer, size);

                HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) &= ~USB_RXCSRL1_RXRDY;

                urb->transfer_buffer += size;
                urb->transfer_buffer_length -= size;
                urb->actual_length += size;

                if ((size < pipe->ep_mps) || (urb->transfer_buffer_length == 0)) {
                    urb->errorcode = 0;
                    goto pipe_wait;
                } else {
                    HWREGB(USB_BASE + MUSB_IND_RXCSRL_OFFSET) = USB_RXCSRL1_REQPKT;
                }
            }
        }
    }
    
    musb_set_active_ep(old_ep_idx);
    jump_pipe_wait = 1;


    
pipe_wait:
    if(jump_pipe_wait == 0)
    {
        musb_set_active_ep(old_ep_idx);
        musb_pipe_waitup(pipe);
    }

#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


