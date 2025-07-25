/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-22     huyt         the first version
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_DEF_H__
#define __GC90XX_HAL_DEF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

 /**
 * @brief  HAL Status structures definition
 */
 typedef enum
 {
     HAL_OK       = 0x00U,
     HAL_ERROR    = 0x01U,
     HAL_BUSY     = 0x02U,
     HAL_TIMEOUT  = 0x03U
 } HAL_StatusTypeDef;

 /**
 * @brief  HAL Lock structures definition
 */
 typedef enum
 {
     HAL_UNLOCKED = 0x00U,
     HAL_LOCKED   = 0x01U
 } HAL_LockTypeDef;

#define __IO volatile

typedef unsigned char        u8;
typedef unsigned short       u16;
typedef unsigned int         u32;
typedef unsigned long long   u64;
typedef signed char          s8;
typedef signed short         s16;
typedef signed int           s32;
typedef signed long long     s64;

#define UNUSED(X) (void)X    /* To avoid gcc/g++ warnings */

#define HAL_MAX_DELAY        0xFFFFFFFFU

#define HAL_IS_BIT_SET(REG, BIT)         (((REG) & (BIT)) == (BIT))
#define HAL_IS_BIT_CLR(REG, BIT)         (((REG) & (BIT)) == 0U)

#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)               \
                        do{                                                        \
                              (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
                              (__DMA_HANDLE__).Parent = (__HANDLE__);              \
                          } while(0U)

#if (USE_RTOS == 1U)
/* Reserved for future use */
#error "USE_RTOS should be 0 in the current HAL release"
#else
#define __HAL_LOCK(__HANDLE__)                                             \
                                do{                                        \
                                    if((__HANDLE__)->Lock == HAL_LOCKED)   \
                                    {                                      \
                                       return HAL_BUSY;                    \
                                    }                                      \
                                    else                                   \
                                    {                                      \
                                       (__HANDLE__)->Lock = HAL_LOCKED;    \
                                    }                                      \
                                  }while (0U)

#define __HAL_UNLOCK(__HANDLE__)                                           \
                                do{                                        \
                                    (__HANDLE__)->Lock = HAL_UNLOCKED;     \
                                  }while (0U)
#endif /* USE_RTOS */

#define MDELAY(x)           UDELAY(1000*(x))
#define UDELAY(x)                       \
    do{                                 \
        for(int _i = 0; _i < (x); ++_i) \
        {                               \
            __NOP();                    \
            __NOP();                    \
            __NOP();                    \
            __NOP();                    \
            __NOP();                    \
            __NOP();                    \
        }                               \
    }while(0)

/** \brief Recommend that function should be inlined by the compiler. */
#ifndef   __INLINE
  #define __INLINE                inline
#endif

/** \brief Define a static function that should be always inlined by the compiler. */
#ifndef __ALWAYS_STATIC_INLINE
#define __ALWAYS_STATIC_INLINE    __attribute__((always_inline)) static inline
#endif

/** \brief Define a static function that may be inlined by the compiler. */
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE       static inline
#endif

/** \brief restrict pointer qualifier to enable additional optimizations. */
#ifndef   __WEAK
  #define __WEAK                __attribute__((weak))
#endif

#define DIV_ROUND_UP(n, d)    (((n) + (d) - 1) / (d))  //get round number that greater or equal n/d

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

#ifdef BIT
#undef BIT
#define BIT(x)                (0x1 << (x))
#else
#define BIT(x)                (0x1 << (x))
#endif

#include <gc90xx.h>

#ifdef __cplusplus
}
#endif

#endif /* __GC90XX_HAL_DEF_H__ */
