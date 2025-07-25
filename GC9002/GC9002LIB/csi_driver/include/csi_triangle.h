/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_triangle.h
 * @brief    header File for triangle Driver
 * @version  V1.0
 * @date     2023/04/18
 * @model    triangle
 ******************************************************************************/
#ifndef __CSI_TRIANGLE_H__
#define __CSI_TRIANGLE_H__

#include <csi_common.h>
#include <csi_irq.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_TRIANGLE_ENABLED

/**
  * @brief  sin Fixed-point mode
  * @param  angle Angle value, Fixed-point integer: 1.7.24
  *         eg:sin(30),angle = 30*2^24(30<<24);
  *            sin(30.5),angle = 30.5*2^24
  * @retval result Fixed-point integer: 1.15.16, eg:sin(30), result=0.5*2^16
  */
int csi_sin_fixed(int64_t angle);
/**
  * @brief  cos Fixed-point mode
  * @param  angle Angle value, Fixed-point integer: 1.7.24
  *         eg:cos(30),angle = 30*2^24(30<<24);
  *            cos(30.5),angle = 30.5*2^24
  * @retval result Fixed-point integer: 1.15.16, 
  *         eg:cos(60), result=0.5*2^16
  */
int csi_cos_fixed(int64_t angle);
/**
  * @brief  arctan Fixed-point mode
  * @param  tan tangent value, Fixed-point integer: 1.15.16
  *         eg:arctan(1),val = 1*2^16(1<<16);
  *            arctan(1.5),val = 1.5*2^16
  * @retval result Fixed-point integer: 1.7.24, 
  *         eg:arctan(1), result=45*2^24
  */
int csi_arctan_fixed(int val);

#if(defined CONFIG_CSI_FLOAT_SOFT_ENABLE && CONFIG_CSI_FLOAT_SOFT_ENABLE)
/**
  * @brief  sin Float-point mode
  * @param  angle Arbitrary Angle value
  * @retval result
  */
float csi_sin(float angle);
/**
  * @brief  cos Float-point mode
  * @param  angle Arbitrary Angle value
  * @retval result
  */
float csi_cos(float angle);
/**
  * @brief  cos Float-point mode
  * @param  tan tan value
  * @retval result
  */
float csi_arctan(float arc);
#endif

#endif /* CONFIG_CSI_DRV_TRIANGLE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_TRIANGLE_H__ */