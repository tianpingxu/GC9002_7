/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     csi_irq.h
 * @brief    header File for IRQ Driver
 * @version  V1.0
 * @date     21. Dec 2018
 * @model    irq
 ******************************************************************************/
#ifndef __CSI_IRQ_H__
#define __CSI_IRQ_H__

#include <csi_irq_vectors.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief      Non Vector Handler Function Typedef
 */
typedef void(*NON_VECTOR_HANDLER)(void);

extern void Default_Handler(void);
/**
  \brief       enable irq.
  \param[in]   irq_num Number of IRQ.
  \return      None.
*/
__ALWAYS_STATIC_INLINE void csi_irq_enable(uint32_t irq_num)
{
    if (irq_num < IRQ_VECTORS_SIZE) 
    {
#ifdef CONFIG_SYSTEM_SECURE
        csi_vic_enable_sirq(irq_num);
#else
        csi_vic_enable_irq(irq_num);
#endif
    }
}

/**
  \brief       disable irq.
  \param[in]   irq_num Number of IRQ.
  \return      None.
*/
__ALWAYS_STATIC_INLINE void csi_irq_disable(uint32_t irq_num)
{
    if (irq_num < IRQ_VECTORS_SIZE) 
    {
#ifdef CONFIG_SYSTEM_SECURE
        csi_vic_disable_sirq(irq_num);
#else
        csi_vic_disable_irq(irq_num);
#endif
    }
}

/**
  \brief       register irq handler.
  \param[in]   irq_num Number of IRQ.
  \param[in]   irq_handler IRQ Handler.
  \return      None.
*/
__ALWAYS_STATIC_INLINE void csi_irq_attach(uint32_t irq_num, void (*irq_handler)(void))
{
    if (irq_num < IRQ_VECTORS_SIZE) 
    {
        csi_vic_set_vector(irq_num, (uint32_t)irq_handler);
    }
}

/**
  \brief       detach irq handler.
  \param[in]   irq_num Number of IRQ.
  \return      None.
*/
__ALWAYS_STATIC_INLINE void csi_irq_detach(uint32_t irq_num)
{
    if (irq_num < IRQ_VECTORS_SIZE) 
    {
        csi_vic_set_vector(irq_num, (uint32_t)csi_irq_get_default(irq_num));
    }
}
/**
  \brief       set irq priority.
  \param[in]   irq_num Number of IRQ.
  \param[in]   priority priority.
  \return      None.
*/
__ALWAYS_STATIC_INLINE void csi_irq_priority(uint32_t irq_num, uint32_t priority)
{
    if (irq_num < IRQ_VECTORS_SIZE) 
    {
        csi_vic_set_priority_irq(irq_num, priority);
    }
}
/**
  \brief       set irq priority.
  \param[in]   irq_num Number of IRQ.
  \param[in]   priority priority.
  \return      None.
*/
__STATIC_INLINE bool csi_irq_is_enabled(uint32_t irq_num)
{
    if (irq_num < IRQ_VECTORS_SIZE) 
    {
        return (csi_vic_get_enabled_irq(irq_num) != 0 ? true : false);
    }
    return false;
}

/**
 * \brief  Initialize a specific IRQ and register the handler
 * \details
 * This function set vector mode, trigger mode and polarity, interrupt level and priority,
 * assign handler for specific IRQn.
 * \param [in]  irq_num     Number of IRQ.
 * \param [in]  shv         \ref CLIC_NON_VECTOR_INTERRUPT means non-vector mode, and \ref CLIC_VECTOR_INTERRUPT is vector mode
 * \param [in]  trig_mode   see \ref ECLIC_TRIGGER_Type
 * \param [in]  lvl         interupt level
 * \param [in]  priority    interrupt priority
 * \param [in]  handler     interrupt handler, if NULL, handler will not be installed
 * \return       -1 means invalid input parameter. 0 means successful.
 * \remarks
 * - This function use to configure specific eclic interrupt and register its interrupt handler and enable its interrupt.
 * - If the vector table is placed in read-only section(FLASHXIP mode), handler could not be installed
 */
int32_t csi_clic_register_irq(uint32_t irq_num, uint8_t shv, CLIC_TRIGGER_Type trig_mode, uint8_t lvl, uint8_t priority, IRQ_Handler_Type handler);

#ifdef __cplusplus
}
#endif
#endif /* __CSI_IRQ_H__ */