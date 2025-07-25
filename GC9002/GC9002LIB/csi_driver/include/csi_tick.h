/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_tick.h
 * @brief    header File for TICK Driver
 * @version  V1.0
 * @date     2023/02/23
 * @model    tick
 ******************************************************************************/
#ifndef __CSI_TICK_H__
#define __CSI_TICK_H__

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_TICK_ENABLED

csi_error_t csi_tick_init(void);
void        csi_tick_init_in_irq(void);
void        csi_tick_uninit(void);
uint64_t    csi_tick_get(void);
uint64_t    csi_tick_get_s(void);
uint64_t    csi_tick_get_ms(void);
uint64_t    csi_tick_get_us(void);
void        csi_tick_increase(void);


#endif /* CONFIG_CSI_DRV_TICK_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_TICK_H__ */