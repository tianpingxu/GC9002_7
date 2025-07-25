/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_crc.h
 * @brief    header File for CRC Driver
 * @version  V1.0
 * @date     2023/02/24
 * @model    crc
 ******************************************************************************/
#ifndef __CSI_CRC_H__
#define __CSI_CRC_H__

#include <csi_common.h>
#include <csi_irq.h>
#include <soft_libc.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_CRC_ENABLED

uint8_t csi_crc7_be(uint8_t crc, uint8_t* data, uint32_t size);
uint8_t csi_crc8(uint8_t crc, uint8_t* data, uint32_t size);
uint8_t csi_crc8_maxim(uint8_t crc, uint8_t* data, uint32_t size);
uint16_t csi_crc16(uint16_t crc, uint8_t* data, uint32_t size);
uint16_t csi_crc16_ccitt(uint16_t crc, uint8_t* data, uint32_t size);
uint16_t csi_crc16_dnp(uint16_t crc, uint8_t* data, uint32_t size);
uint16_t csi_crc16_itu(uint16_t crc, uint8_t* data, uint32_t size);
uint32_t csi_crc32_le(uint32_t crc, uint8_t* data, uint32_t size);
uint32_t csi_crc32_be(uint32_t crc, uint8_t* data, uint32_t size);


#endif /* CONFIG_CSI_DRV_CRC_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_CRC_H__ */