/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-10     huyt         the first version
 */
/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal.h"
#ifdef HAL_HYPER_MODULE_ENABLED

static uint16_t             hyper_read_reg(HYPER_HandleTypeDef *hhyper, uint8_t die_id, uint8_t region_id, uint8_t reg_id);
static void                 hyper_write_reg(HYPER_HandleTypeDef *hhyper, uint8_t die_id, uint8_t region_id, uint8_t reg_id, uint16_t data);
static void                 hyper_set_capture(HYPER_HandleTypeDef *hhyper, uint8_t div, uint8_t delay, uint8_t edge);
static HAL_StatusTypeDef    hyper_adapted_capture(HYPER_HandleTypeDef *hhyper);



static uint16_t hyper_read_reg(HYPER_HandleTypeDef *hhyper, uint8_t die_id, uint8_t region_id, uint8_t reg_id)
{
    uint32_t tempreg = 0;
    ((union _HYPER_OP*)&tempreg)->bit.START     = HYPER_ENABLE;     // 启动总线
    ((union _HYPER_OP*)&tempreg)->bit.WR        = HYPER_DIR_READ;   // read
    ((union _HYPER_OP*)&tempreg)->bit.DIE       = die_id;           // HyperRAM die_id，对于GC9002，该项为0
    ((union _HYPER_OP*)&tempreg)->bit.REGION    = region_id;        // HyperRAM region_id, 对应于HyperRAM CA命令的31:24位
    ((union _HYPER_OP*)&tempreg)->bit.REG       = reg_id;           // HyperRAM reg_id, 对应于HyperRAM CA命令的7:0位
    hhyper->Instance->OP.reg_u32val = tempreg;
    while(hhyper->Instance->OP.bit.WIP);
    return (hhyper->Instance->RDATA & 0xFFFF);
}
static void hyper_write_reg(HYPER_HandleTypeDef *hhyper, uint8_t die_id, uint8_t region_id, uint8_t reg_id, uint16_t data)
{
    uint32_t tempreg = 0;
    hhyper->Instance->WDATA = data;
    ((union _HYPER_OP*)&tempreg)->bit.START     = HYPER_ENABLE;     // 启动总线
    ((union _HYPER_OP*)&tempreg)->bit.WR        = HYPER_DIR_WRITE;  // write
    ((union _HYPER_OP*)&tempreg)->bit.DIE       = die_id;           // HyperRAM die_id，对于GC9002，该项为0
    ((union _HYPER_OP*)&tempreg)->bit.REGION    = region_id;        // HyperRAM region_id, 对应于HyperRAM CA命令的31:24位
    ((union _HYPER_OP*)&tempreg)->bit.REG       = reg_id;           // HyperRAM reg_id, 对应于HyperRAM CA命令的7:0位
    hhyper->Instance->OP.reg_u32val = tempreg;
    while(hhyper->Instance->OP.bit.WIP);
}
static void hyper_set_capture(HYPER_HandleTypeDef *hhyper, uint8_t div, uint8_t delay, uint8_t edge)
{
    uint32_t tempreg = 0;
    ((union _HYPER_CAPTURE*)&tempreg)->bit.OP_CLK_DIV = div;
    ((union _HYPER_CAPTURE*)&tempreg)->bit.CDELAY = delay;
    ((union _HYPER_CAPTURE*)&tempreg)->bit.CEDGE = edge;
    hhyper->Instance->CAPTURE.reg_u32val = tempreg;
}
static HAL_StatusTypeDef hyper_adapted_capture(HYPER_HandleTypeDef *hhyper)
{
	uint8_t flag[32];
	int num = 0;
    /* 2. 通过读取ID Register 0标定capture延迟 */
    for(int i = 0; i <= 0xF; i++)
    {
        hyper_set_capture(hhyper, HYPER_DIV, i, HYPER_CAP_EDGE_RISING);
        if(hyper_read_reg(hhyper, HYPER_CA_DIE_ID, 0x00, 0x00) == HYPER_IDR0)
        {
			num++;
			flag[i*2] = 1;
            //return HAL_OK;
        }
		else {
			flag[i*2] = 0;
		}
        hyper_set_capture(hhyper, HYPER_DIV, i, HYPER_CAP_EDGE_FALLING);
        if(hyper_read_reg(hhyper, HYPER_CA_DIE_ID, 0x00, 0x00) == HYPER_IDR0)
        {
			num++;
			flag[i*2+1] = 1;
            //return HAL_OK;
        }
		else {
			flag[i*2+1] = 0;
		}
    }
	int temp = 0;
	for(int i = 0; i < sizeof(flag); i++)
	{
		if(flag[i] == 1)
		{
			if(temp == (num/2))
			{
				hyper_set_capture(hhyper, HYPER_DIV, i/2, (i%2) == 0 ? HYPER_CAP_EDGE_RISING : HYPER_CAP_EDGE_FALLING);
				return HAL_OK;
			}
			temp++;
		}
	}
    return HAL_ERROR;
}
/**
  * @brief  Hyper Check
  * @param  hhyper hyper handle
  * @retval HAL status
  */
static bool HYPER_Check(HYPER_HandleTypeDef *hhyper)
{    
    /* 2. 通过读取ID Register 0m，确认是否有封装hyper */
    for(int i = 0; i <= 0xF; i++)
    {
        hyper_set_capture(hhyper, HYPER_DIV, i, HYPER_CAP_EDGE_FALLING);
        if(hyper_read_reg(hhyper, HYPER_CA_DIE_ID, 0x00, 0x00) == HYPER_IDR0)
        {
            return true;
        }
        hyper_set_capture(hhyper, HYPER_DIV, i, HYPER_CAP_EDGE_RISING);
        if(hyper_read_reg(hhyper, HYPER_CA_DIE_ID, 0x00, 0x00) == HYPER_IDR0)
        {
            return true;
        }
    }
    return false;
}

/**
  * @brief  Hyper Init
  * @param  hhyper hyper handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HYPER_Init(HYPER_HandleTypeDef *hhyper)
{
    uint32_t tempreg = 0;
    assert_param(hhyper != NULL);
    assert_param(IS_HYPER_ALL_INSTANCE(hhyper->Instance));
    hhyper->State = HAL_HYPER_STATE_RESET;    
    hhyper->MapAHBAddrBase = HYPER_DATA_BASE;
    
    if(hhyper->Instance->QOSID == 3)
    {
        return HAL_OK;
    }
    //enable 1v8 power
    if(RCC->PMU_CTL.bit.vo3318_en == 0)
    {
        HAL_RCC_VO3318Enable();
    }
    // enable hyper clock
    tempreg = RCC->HYPER_CLK_PRES.reg_u32val;
    ((union _HYPER_CLK_PRES*)&tempreg)->bit.hyper_pres = hhyper->Pres;
    ((union _HYPER_CLK_PRES*)&tempreg)->bit.hyper_clken = 0x1;
    RCC->HYPER_CLK_PRES.reg_u32val = tempreg;

    /* 1. 配置HyperRAM的Configuration Register 0 */
    hyper_write_reg(hhyper, HYPER_CA_DIE_ID, 0x01, 0x00, 0x8f04);
    hhyper->HardPackEn = (HYPER_Check(hhyper) ? 1 : 0);
    hhyper->Capture = 0;
    if(hhyper->HardPackEn)
    {
        if(hyper_adapted_capture(hhyper) != HAL_OK)
        {
            return HAL_ERROR;
        }
    }
    hhyper->Capture = 1;
    hhyper->Instance->QOSID = 0x3;
    hhyper->State = HAL_HYPER_STATE_READY;
    return HAL_OK;
}
/**
  * @brief  Hyper read data
  * @param  hhyper hyper handle
  * @param  addr hyper sram addr
  * @param  data addr of data buff
  * @param  size size of data buff
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HYPER_Read(HYPER_HandleTypeDef *hhyper, uint32_t addr, uint32_t *data, uint32_t size)
{
    assert_param(hhyper != NULL);
    assert_param(IS_HYPER_ALL_INSTANCE(hhyper->Instance));
    assert_param(IS_ADDR_ALIGNED_4B(addr));
    assert_param(IS_ADDR_ALIGNED_4B(data));
    assert_param(hhyper->State == HAL_HYPER_STATE_READY);
    
    uint32_t *pread = (uint32_t *)(hhyper->MapAHBAddrBase+addr);
    size /= 4;
    if(hhyper->State == HAL_HYPER_STATE_READY)
    {
        for(uint32_t i = 0; i < size; i++)
        {
            data[i] = pread[i];
        }
    }
    return HAL_OK;
}
/**
  * @brief  Hyper write data
  * @param  hhyper hyper handle
  * @param  addr hyper sram addr
  * @param  data addr of data buff
  * @param  size size of data buff
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HYPER_Write(HYPER_HandleTypeDef *hhyper, uint32_t addr, uint32_t *data, uint32_t size)
{
    assert_param(hhyper != NULL);
    assert_param(IS_HYPER_ALL_INSTANCE(hhyper->Instance));
    assert_param(IS_ADDR_ALIGNED_4B(addr));
    assert_param(IS_ADDR_ALIGNED_4B(data));
    assert_param(hhyper->State == HAL_HYPER_STATE_READY);
    
    uint32_t *pwrite = (uint32_t *)(hhyper->MapAHBAddrBase+addr);
    size /= 4;
    if(hhyper->State == HAL_HYPER_STATE_READY)
    {
        for(uint32_t i = 0; i < size; i++)
        {
            pwrite[i] = data[i];
        }
    }
    return HAL_OK;
}



static inline uint32_t get(uint32_t addr) {
    volatile uint32_t * ptr = (volatile uint32_t *)(addr);
    return *ptr;
}

static inline void set(uint32_t addr, uint32_t value) {
    volatile uint32_t * ptr = (volatile uint32_t *)(addr);
    *ptr = value;
}
static void dma_memcpy(uint8_t ch, uint32_t src_addr, uint32_t dst_addr, uint32_t len) {
    // CONFIG START
    uint32_t src_dev_type = 1; // 0:hw 1:sw
    uint32_t dst_dev_type = 1; // 0:hw 1:sw
    uint32_t src_addr_incr_mode = 0; // 0:incr, 1:decr, 2:fix
    uint32_t dst_addr_incr_mode = 0;
    uint32_t src_hs = 0;
    uint32_t dst_hs = 0;
    // CONFIG END
    uint32_t src_burst_type = 0;
    uint32_t dst_burst_type = 0;
    uint32_t src_width = 0;
    uint32_t dst_width = 0;
    uint32_t length = 0;
    switch(len & 0x3) {
        case 0: src_width = 2; length = len >> 2; break;
        case 1: src_width = 0; length = len; break;
        case 2: src_width = 1; length = len >> 1; break;
        case 3: src_width = 0; length = len; break;
    }
    dst_width = src_width;

    set(0x50200398, 1); // enable dma
    set(0x50200000 + 0x58 * ch, src_addr); // src addr
    set(0x50200008 + 0x58 * ch, dst_addr); // dst addr
    set(0x50200040 + 0x58 * ch, (src_dev_type << 11) | (dst_dev_type << 10));
    set(0x50200044 + 0x58 * ch, 0x1c | (dst_hs << 11) | (src_hs << 7) ); // cfg
    set(0x50200018 + 0x58 * ch, 0x1 | (src_burst_type << 14) | (dst_burst_type << 11) | (src_addr_incr_mode << 9) | (dst_addr_incr_mode << 7) | (src_width << 4) | (dst_width<<1));
    set(0x5020001c + 0x58 * ch, length);
}

static void dma_start(uint8_t ch) {
    uint32_t regvalue;
    regvalue = get(0x502003a0);
    set(0x502003a0, regvalue | (0x101 << ch)); // enable channel
}

static void dma_wait_done(uint8_t ch) {
    uint32_t status;
    do {
      status = get(0x502003a0);
    } while((status & (0x1 << ch)) != 0);
}

static int dma_wait_done_us(uint8_t ch, uint32_t us) {
    uint32_t status;
    for(uint32_t i = 0; i < us; i++)
    {
        status = get(0x502003a0);
        if((status & (0x1 << ch)) != 0)
        {
            __NOP();
        }
        else {
            return 1;
        }
    }
    return 0;
}

HAL_StatusTypeDef HAL_HYPER_CheckStatus(void)
{
    uint32_t tempreg = 0;
	__attribute__ ((aligned(64))) uint8_t rx_data[64];
    
    //enable 1v8 power
    HAL_RCC_VO3318Enable();

    // enable hyper clock
    tempreg = RCC->HYPER_CLK_PRES.reg_u32val;
    ((union _HYPER_CLK_PRES*)&tempreg)->bit.hyper_pres = HYPER_Pres_3;
    ((union _HYPER_CLK_PRES*)&tempreg)->bit.hyper_clken = 0x1;
    RCC->HYPER_CLK_PRES.reg_u32val = tempreg;
    
    dma_memcpy(0, 0x20000000, (uint32_t)rx_data, sizeof(rx_data));
    dma_start(0);
    
    if(dma_wait_done_us(0, 20000) == 0)
    {// timeout
		return HAL_TIMEOUT;
    }
    return HAL_OK;
}
#endif /* HAL_RCC_MODULE_ENABLED */
