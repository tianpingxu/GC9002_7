/**
  ******************************************************************************
  * @file    gc9002_hal_hyper.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the Hyper
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_HYPER_H__
#define __GC9002_HAL_HYPER_H__

#ifdef cplusplus
extern C{
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_HYPER_MODULE_ENABLED

#define HYPER_DIV                       0x01U// 分频系数，该项为1，不可修改

#define HYPER_CA_DIE_ID                 0x00U//HyperRAM die_id，对于GC9002，该项为0
#define HYPER_IDR0                      0x005FU// [15:7] Reserved, [6:4] Density <101b - 32 Mb>  [3:0] Manufacturer <1111b – Winbond>
#define HYPER_IDR1                      0x000FU// [15:4] Reserved, [3:0] Device Type <1111b – HyperRAM>

#define HYPER_READ_DUMMY                0x05U//HyperRAM Read Dummy，该项来自HyperRAM手册，不需要修改
#define HYPER_ADDITIONAL_READ_DUMMY     0x05U//HyperRAM addtional Read Dummy，该项与HyperRAM Read Dummy值相同，不需要修改

typedef enum
{
   HAL_HYPER_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
   HAL_HYPER_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
   HAL_HYPER_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
   HAL_HYPER_STATE_ERROR             = 0xE0U    /*!< Error                                     */

}HAL_HYPER_StateTypeDef;

enum
{
    HYPER_DISABLE = 0x00,
    HYPER_ENABLE = 0x01,
};
typedef enum
{
    HYPER_DIR_READ = 0x00,
    HYPER_DIR_WRITE = 0x01,
}hyper_wr_t;
typedef enum
{
    HYPER_CAP_EDGE_RISING = 0x00,
    HYPER_CAP_EDGE_FALLING = 0x01,
}hyper_Capture_edge_t;
typedef enum
{
    HYPER_Pres_0 = 0x00,
    HYPER_Pres_1 = 0x01,
    HYPER_Pres_2 = 0x02,
    HYPER_Pres_3 = 0x03,
}hyper_pres_t;

typedef struct
{
    HYPER_TypeDef                *Instance;         /*!< hyper registers base address                           */
    uint32_t                     MapAHBAddrBase;    /*!< hyper mapping AHB base address                         */
    uint32_t                     Pres : 2;          /*!< hyper Frequency division coefficient,@ref hyper_pres_t */
    uint32_t                     HardPackEn : 1;
    uint32_t                     Capture : 1;
    uint32_t                     : 28;
    __IO HAL_HYPER_StateTypeDef  State;             /*!< hyper communication state                              */
}HYPER_HandleTypeDef;


// only calling with hyper
HAL_StatusTypeDef HAL_HYPER_CheckStatus(void);
HAL_StatusTypeDef HAL_HYPER_Init(HYPER_HandleTypeDef *hhyper);
HAL_StatusTypeDef HAL_HYPER_Read(HYPER_HandleTypeDef *hhyper, uint32_t addr, uint32_t *data, uint32_t size);
HAL_StatusTypeDef HAL_HYPER_Write(HYPER_HandleTypeDef *hhyper, uint32_t addr, uint32_t *data, uint32_t size);



#endif /* HAL_HYPER_MODULE_ENABLED */
#ifdef cplusplus
}
#endif
#endif /* __GC9002_HAL_HYPER_H__ */
