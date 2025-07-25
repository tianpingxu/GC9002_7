/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/**************************************************************************//**
 * @file     soc.h
 * @brief    CSI Core Peripheral Access Layer Header File for
 *           CSKYSOC Device Series
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#ifndef _SOC_H_
#define _SOC_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <system.h>
#include <core_rv32.h>
#include <csi_core.h>
#include <csi_rv32_gcc.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IHS_VALUE
#define  IHS_VALUE    (0)
#endif

#ifndef EHS_VALUE
#define  EHS_VALUE    (144000000)
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _SOC_H_ */
