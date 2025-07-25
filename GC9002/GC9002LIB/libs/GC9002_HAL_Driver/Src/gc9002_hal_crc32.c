/**
  ******************************************************************************
  * @file    gc9005_hal_crc32.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2022/12/14
  * @brief   This file provides all the crc32 firmware functions.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal.h"
#ifdef HAL_CRC32_MODULE_ENABLED

static CRC32_Typedef *pCRC32 = CRC32;
/**
  * @brief  Set CRC32 calculation data and start.
  * @param  addr Pointer to data buffer to calculated. must be 64 bytes aligned
  * @param  len Len of data buffer to calculated, (bytes).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRC32_Begin(uint32_t *addr, uint32_t len)
{
    assert_param(addr != NULL);
    assert_param(IS_ADDR_ALIGNED(addr, WORD_ALIGN_64B_MASK));   //64 bytes aligned
    assert_param(IS_ADDR_ALIGNED(len, WORD_ALIGN_4B_MASK));   //64 bytes aligned
    
    while(pCRC32->IDLE == 0);
    pCRC32->AR = (uint32_t)addr;
    pCRC32->LR = len;
    pCRC32->START = 1;
    return HAL_OK;
}
/**
  * @brief  Get CRC32 calculation results.
  * @param  result Pointer to result buffer.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRC32_GetResult(uint32_t *result, uint32_t Timeout)
{
    assert_param(result != NULL);
    uint32_t tickstart = HAL_GetTick();
    
    while(pCRC32->IDLE == 0)
    {
        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY)
        {
            if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
            {
                return HAL_BUSY;
            }
        }
    }
    *result = pCRC32->DR;
    return HAL_OK;
}
#endif /* HAL_CRC32_MODULE_ENABLED */
