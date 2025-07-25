/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-18     huyt         V1.0.1
 */
#include <gc90xx_hal.h>

#ifdef HAL_CAN_MODULE_ENABLED

/** @defgroup CAN_Private_Functions
  * @{
  */
static void CAN_SetBandrate(CAN_HandleTypeDef *hcan, CAN_BaudrateTypeDef *timing);
static void CAN_SetFilter(CAN_HandleTypeDef *hcan, CAN_ACRAMR_Enum acramrnum, uint8_t BCAN_ACR0, uint8_t BCAN_ACR1, uint8_t BCAN_ACR2, uint8_t BCAN_ACR3, uint8_t BCAN_AMR0, uint8_t BCAN_AMR1, uint8_t BCAN_AMR2, uint8_t BCAN_AMR3);

static void CAN_EnableInterrupt(CAN_HandleTypeDef *hcan, uint8_t ITtype);

static uint8_t SET_ACR(CAN_HandleTypeDef *hcan, CAN_ACRAMR_Enum acramrnum, uint8_t BCAN_ACR0,uint8_t BCAN_ACR1,uint8_t BCAN_ACR2,uint8_t BCAN_ACR3);
static uint8_t SET_AMR(CAN_HandleTypeDef *hcan, CAN_ACRAMR_Enum acramrnum, uint8_t BCAN_AMR0,uint8_t BCAN_AMR1,uint8_t BCAN_AMR2,uint8_t BCAN_AMR3);

static void CAN_SetFilterByFrame(CAN_HandleTypeDef *hcan, CAN_ACRAMR_Enum acramrnum, uint8_t FilterMode, uint32_t FrameType, uint32_t CAN_ID0, uint32_t mask, uint8_t CAN_DATA0, uint8_t CAN_DATA1, uint32_t CAN_ID1);


/**
  * @brief  Deinitializes the CAN peripheral registers to their default reset values.
  * @param  CANx: where x can be CAN2 or CAN3 to select the CAN peripheral.
  * @retval None.
  */
HAL_StatusTypeDef HAL_CAN_DeInit(CAN_HandleTypeDef *hcan)
{
    /* Check the parameters */
    assert_param(hcan != NULL);
    assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
    SET_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Reset Mode Selected
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    hcan->MspDeInitCallback(hcan);
#else
    HAL_CAN_MspDeInit(hcan);
#endif
    return HAL_OK;
}

/**
  * @brief  Initializes the CAN peripheral registers to their default reset values.
  * @param  CANx: where x can be CAN2 or CAN3 to select the CAN peripheral.
  * @param  CAN_InitStruct: pointer to a CAN_InitTypeDef structure.
  * @retval None.
  */
HAL_StatusTypeDef HAL_CAN_Init(CAN_HandleTypeDef *hcan)
{
    /* Check the parameters */
    assert_param(hcan != NULL);
    assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
    assert_param(IS_CAN_MODE(hcan->Init.Mode));
    assert_param(IS_CAN_BAUDRATE(hcan->Init.BaudRate));
    uint8_t tempreg = 0;

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_CHECK_PERI_LOCK((uint32_t)(hcan->Instance));
#endif
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    if (hcan->State == HAL_CAN_STATE_RESET)
    {
        hcan->ArbLostCallback = HAL_CAN_ArbLostCallback;
        hcan->PassiveCallback = HAL_CAN_PassiveCallback;
        hcan->WakeUpCallback = HAL_CAN_WakeUpCallback;
        hcan->DataOverrunCallback = HAL_CAN_DataOverrunCallback;
        hcan->ErrorCallback = HAL_CAN_ErrorCallback;
        hcan->BusErrorCallback = HAL_CAN_BusErrorCallback;
        hcan->TxCompleteCallback = HAL_CAN_TxCompleteCallback;
        hcan->RxMsgCallback = HAL_CAN_RxMsgCallback;
        hcan->MspDeInitCallback = HAL_CAN_MspDeInit;
        if (hcan->MspInitCallback == NULL)
        {
            hcan->MspInitCallback = HAL_CAN_MspInit; /* Legacy weak MspInit */
        }
        /* Init the low level hardware: CLOCK, NVIC */
        hcan->MspInitCallback(hcan);
    }
#else
    if (hcan->State == HAL_CAN_STATE_RESET)
    {
        /* Init the low level hardware: CLOCK, NVIC */
        HAL_CAN_MspInit(hcan);
    }
#endif /* (USE_HAL_CAN_REGISTER_CALLBACKS) */
    SET_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Reset Mode Selected
    MODIFY_REG(hcan->Instance->MOD.reg, CAN_MODE_Listen_Only | CAN_MODE_LOOPBACK, hcan->Init.Mode);
    hcan->Instance->CDR.reg = CAN_OutClock_OFF;
    hcan->Instance->CDR.reg = 0x0B;
    hcan->Instance->OCR.reg = CAN_OUTPUT_MODE_CLOCK;//CAN_OUTPUT_MODE_NORMAL;
    tempreg = hcan->Instance->ISR.reg;//Clear interrupt
    hcan->Instance->IER.reg = 0x0U;
    CLEAR_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Normal operation

    HAL_CAN_SetBandrateAuto(hcan, hcan->Init.BaudRate);
    /* Initialize the error code */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;
    /* Initialize the CAN state */
    hcan->State = HAL_CAN_STATE_READY;

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_SET_PERI_LOCK((uint32_t)(hcan->Instance), PERIPHERAL_INITED);
#endif
    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Fills each CAN_InitStruct member with its default value.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
HAL_StatusTypeDef HAL_CAN_StructInit(CAN_HandleTypeDef *hcan)
{
    /* CAN_InitStruct members default value */
    hcan->Init.Mode = CAN_MODE_NORMAL;
    hcan->Init.BaudRate = CAN_BaudRate_250k;
    return HAL_OK;
}

__WEAK void HAL_CAN_ArbLostCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
__WEAK void HAL_CAN_PassiveCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
__WEAK void HAL_CAN_WakeUpCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
__WEAK void HAL_CAN_DataOverrunCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
__WEAK void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
__WEAK void HAL_CAN_BusErrorCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
__WEAK void HAL_CAN_TxCompleteCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
__WEAK void HAL_CAN_RxMsgCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
}
/**
  * @brief  Initializes the CAN MSP.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__WEAK void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes the CAN MSP.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
__WEAK void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_MspDeInit could be implemented in the user file
   */
}

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
/**
  * @brief  Register a CAN CallBack.
  *         To be used instead of the weak predefined callback
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for CAN module
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_RegisterCallback(CAN_HandleTypeDef *hcan, HAL_CAN_CallbackIDTypeDef CallbackID, void (* pCallback)(CAN_HandleTypeDef *_hcan))
{
    HAL_StatusTypeDef status = HAL_OK;
    if (pCallback == NULL)
    {
        /* Update the error code */
        hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }
    if (hcan->State == HAL_CAN_STATE_READY)
    {
        switch (CallbackID)
        {
        case HAL_CAN_ARB_LOST_CB_ID :
            hcan->ArbLostCallback = pCallback;
            break;
        case HAL_CAN_PASSIVE_CB_ID :
            hcan->PassiveCallback = pCallback;
            break;
        case HAL_CAN_WAKEUP_CB_ID :
            hcan->WakeUpCallback = pCallback;
            break;
        case HAL_CAN_DATA_OVERRUN_CB_ID :
            hcan->DataOverrunCallback = pCallback;
            break;
        case HAL_CAN_ERROR_CB_ID :
            hcan->ErrorCallback = pCallback;
            break;
        case HAL_CAN_BUS_ERROR_CB_ID :
            hcan->BusErrorCallback = pCallback;
            break;
        case HAL_CAN_TX_COMPLETE_CB_ID :
            hcan->TxCompleteCallback = pCallback;
            break;
        case HAL_CAN_RX_COMPLETE_CB_ID :
            hcan->RxMsgCallback = pCallback;
            break;
        case HAL_CAN_MSPINIT_CB_ID :
            hcan->MspInitCallback = pCallback;
            break;
        case HAL_CAN_MSPDEINIT_CB_ID :
            hcan->MspDeInitCallback = pCallback;
            break;
        default :
            /* Update the error code */
            hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else if (hcan->State == HAL_CAN_STATE_RESET)
    {
        switch (CallbackID)
        {
        case HAL_CAN_MSPINIT_CB_ID :
            hcan->MspInitCallback = pCallback;
            break;
        case HAL_CAN_MSPDEINIT_CB_ID :
            hcan->MspDeInitCallback = pCallback;
            break;
        default :
            /* Update the error code */
            hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Update the error code */
        hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    return status;
}

/**
  * @brief  Unregister a CAN CallBack.
  *         CAN callabck is redirected to the weak predefined callback
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for CAN module
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CAN_UnRegisterCallback(CAN_HandleTypeDef *hcan, HAL_CAN_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;
    if (hcan->State == HAL_CAN_STATE_READY)
    {
        switch (CallbackID)
        {
        case HAL_CAN_ARB_LOST_CB_ID :
            hcan->ArbLostCallback = HAL_CAN_ArbLostCallback;
            break;
        case HAL_CAN_PASSIVE_CB_ID :
            hcan->PassiveCallback = HAL_CAN_PassiveCallback;
            break;
        case HAL_CAN_WAKEUP_CB_ID :
            hcan->WakeUpCallback = HAL_CAN_WakeUpCallback;
            break;
        case HAL_CAN_DATA_OVERRUN_CB_ID :
            hcan->DataOverrunCallback = HAL_CAN_DataOverrunCallback;
            break;
        case HAL_CAN_ERROR_CB_ID :
            hcan->ErrorCallback = HAL_CAN_ErrorCallback;
            break;
        case HAL_CAN_BUS_ERROR_CB_ID :
            hcan->BusErrorCallback = HAL_CAN_BusErrorCallback;
            break;
        case HAL_CAN_TX_COMPLETE_CB_ID :
            hcan->TxCompleteCallback = HAL_CAN_TxCompleteCallback;
            break;
        case HAL_CAN_RX_COMPLETE_CB_ID :
            hcan->RxMsgCallback = HAL_CAN_RxMsgCallback;
            break;
        case HAL_CAN_MSPINIT_CB_ID :
            hcan->MspInitCallback = HAL_CAN_MspInit;
            break;
        case HAL_CAN_MSPDEINIT_CB_ID :
            hcan->MspDeInitCallback = HAL_CAN_MspDeInit;
            break;
        default :
            /* Update the error code */
            hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else if (hcan->State == HAL_CAN_STATE_RESET)
    {
        switch (CallbackID)
        {
        case HAL_CAN_MSPINIT_CB_ID :
            hcan->MspInitCallback = HAL_CAN_MspInit;
            break;
        case HAL_CAN_MSPDEINIT_CB_ID :
            hcan->MspDeInitCallback = HAL_CAN_MspDeInit;
            break;
        default :
            /* Update the error code */
            hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Update the error code */
        hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    return status;
}
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */

/**
  * @brief  Configures the CAN reception filter according to the specified
  *         parameters in the CAN_FilterInitStruct.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  sFilterConfig pointer to a CAN_FilterTypeDef structure that
  *         contains the filter configuration information.
  * @retval None
  */
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef *hcan, CAN_FilterTypeDef *sFilterConfig)
{
    CAN_FilterRegister_TypeDef *filter_reg;
    if(!hcan || !sFilterConfig)
    {
        return HAL_ERROR;
    }
    HAL_CAN_StateTypeDef state = hcan->State;
    /* Check the parameters */
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterIdHigh));
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterIdLow));
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterMaskIdHigh));
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterMaskIdLow));
    assert_param(IS_CAN_FILTER_MODE(sFilterConfig->FilterMode));
    assert_param(IS_CAN_FILTER_BANK_DUAL(sFilterConfig->FilterBank));

    if ((state == HAL_CAN_STATE_RESET) ||
        (state == HAL_CAN_STATE_READY) ||
        (state == HAL_CAN_STATE_LISTENING))
    {
        SET_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Reset Mode Selected
        // set MOD
        MODIFY_REG(hcan->Instance->MOD.reg, CAN_SingleFilter_Mode, sFilterConfig->FilterMode);
        if(sFilterConfig->FilterBank == CAN_ACRAMR_1)
        {
            hcan->Instance->EX_REG_EN.reg = 0x00U;
            filter_reg = &hcan->Instance->FilterRegister0;
        }
        else if(sFilterConfig->FilterBank >= CAN_ACRAMR_2 && sFilterConfig->FilterBank <= CAN_ACRAMR_15)
        {
            hcan->Instance->EX_REG_EN.reg = 0x00U;
            filter_reg = &hcan->Instance->FilterRegisterX[sFilterConfig->FilterBank-CAN_ACRAMR_2];
        }
        else if(sFilterConfig->FilterBank >= CAN_ACRAMR_16 && sFilterConfig->FilterBank <= CAN_ACRAMR_30)
        {
            hcan->Instance->EX_REG_EN.reg = 0xFFU;
            filter_reg = &hcan->Instance->FilterRegisterX_Ext[sFilterConfig->FilterBank-CAN_ACRAMR_16];
        }
        if(sFilterConfig->FilterMode == CAN_SingleFilter_Mode)
        {
            filter_reg->ACR0.reg = ((sFilterConfig->FilterIdHigh >> 8U) & 0xFFU);
            filter_reg->ACR1.reg = (sFilterConfig->FilterIdHigh & 0xFFU);
            filter_reg->ACR2.reg = ((sFilterConfig->FilterIdLow >> 8U) & 0xFFU);
            filter_reg->ACR3.reg = (sFilterConfig->FilterIdLow & 0xFFU);

            filter_reg->AMR0.reg = ((sFilterConfig->FilterMaskIdHigh >> 8U) & 0xFFU);
            filter_reg->AMR1.reg = (sFilterConfig->FilterMaskIdHigh & 0xFFU);
            filter_reg->AMR2.reg = ((sFilterConfig->FilterMaskIdLow >> 8U) & 0xFFU);
            filter_reg->AMR3.reg = (sFilterConfig->FilterMaskIdLow & 0xFFU);
        }
        else if(sFilterConfig->FilterMode == CAN_DualFilter_Mode)
        {
            filter_reg->ACR0.reg = ((sFilterConfig->FilterIdHigh >> 8U) & 0xFFU);
            filter_reg->ACR1.reg = (sFilterConfig->FilterIdHigh & 0xFFU);
            filter_reg->ACR2.reg = ((sFilterConfig->FilterIdHigh >> 8U) & 0xFFU);
            filter_reg->ACR3.reg = (sFilterConfig->FilterIdHigh & 0xFFU);

            filter_reg->AMR0.reg = ((sFilterConfig->FilterMaskIdHigh >> 8U) & 0xFFU);
            filter_reg->AMR1.reg = (sFilterConfig->FilterMaskIdHigh & 0xFFU);
            filter_reg->AMR2.reg = ((sFilterConfig->FilterMaskIdLow >> 8U) & 0xFFU);
            filter_reg->AMR3.reg = (sFilterConfig->FilterMaskIdLow & 0xFFU);
        }
        CLEAR_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Normal operation
        hcan->State = HAL_CAN_STATE_READY;
        /* Return function status */
        return HAL_OK;
    }
    else
    {
        /* Update error code */
        hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    }
}

/**
  * @brief  Transmit a Message.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  pTxMsg pointer to a TX Message.
  * @param  Timeout Time of wait.
  * @retval None
  */
HAL_StatusTypeDef HAL_CAN_TxMessage(CAN_HandleTypeDef *hcan, const CAN_MsgTypeDef *pTxMsg, uint32_t Timeout)
{
    uint64_t tickstart;
    /* Check CAN handle */
    if (hcan == NULL)
    {
        return HAL_ERROR;
    }
    /* Get tick */
    tickstart = HAL_GetTick();
    HAL_CAN_StateTypeDef state = hcan->State;
    /* Check the parameters */
    assert_param(IS_CAN_IDTYPE(pTxMsg->IDE));
    assert_param(IS_CAN_RTR(pTxMsg->RTR));
    assert_param(IS_CAN_DLC(pTxMsg->DLC));
    if (pTxMsg->IDE == CAN_ID_STD)
    {
        assert_param(IS_CAN_STDID(pTxMsg->StdId));
    }
    else
    {
        assert_param(IS_CAN_EXTID(pTxMsg->ExtId));
    }
    if ((state == HAL_CAN_STATE_READY) ||
        (state == HAL_CAN_STATE_LISTENING))
    {
        __HAL_LOCK(hcan);
        while(!__HAL_CAN_GET_STATUS(hcan, CAN_STATUS_TX_BUFF_RELEASE))
        {
            /* Check for the Timeout */
            if (Timeout != HAL_MAX_DELAY && (HAL_GetTick() - tickstart) > Timeout)
            {
                /* Update error code */
                hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;
                __HAL_UNLOCK(hcan);
                return HAL_ERROR;
            }
        }
        hcan->pTxMsg = (CAN_MsgTypeDef *)pTxMsg;
        if(pTxMsg->IDE == CAN_ID_STD)
        {
            union
            {
                __IO CAN_REG val;
                struct{
                    uint32_t DLC        : 4;
                    uint32_t X          : 2;
                    uint32_t RTR        : 1;
                    uint32_t IDE        : 1;
                    uint32_t Reserved   : 24;
                }bits;
            }frame_ctrl_field;
            frame_ctrl_field.bits.DLC = pTxMsg->DLC;
            frame_ctrl_field.bits.X = CAN_FRAME_BIT_X;
            frame_ctrl_field.bits.RTR = pTxMsg->RTR;
            frame_ctrl_field.bits.IDE = pTxMsg->IDE;
            hcan->Instance->TxBuffStd_WO.FrameCtrlField.val.reg = frame_ctrl_field.val.reg;
            hcan->Instance->TxBuffStd_WO.FrameIdentifier1.bits.ID10_3 = ((pTxMsg->StdId >> 3) & 0xFF);
            hcan->Instance->TxBuffStd_WO.FrameIdentifier2.bits.ID2_0 = (pTxMsg->StdId & 0x07);
            for(int i = 0; i < pTxMsg->DLC && i < 8; i++)
            {
                hcan->Instance->TxBuffStd_WO.DATA[i].reg = pTxMsg->Data[i];
            }
        }
        else if(pTxMsg->IDE == CAN_ID_EXT)
        {
            union
            {
                __IO CAN_REG val;
                struct{
                    uint32_t DLC        : 4;
                    uint32_t X          : 2;
                    uint32_t RTR        : 1;
                    uint32_t IDE         : 1;
                    uint32_t Reserved   : 24;
                }bits;
            }frame_ctrl_field;
            frame_ctrl_field.bits.DLC = pTxMsg->DLC;
            frame_ctrl_field.bits.X = CAN_FRAME_BIT_X;
            frame_ctrl_field.bits.RTR = pTxMsg->RTR;
            frame_ctrl_field.bits.IDE = pTxMsg->IDE;
            hcan->Instance->TxBuffExt_WO.FrameCtrlField.val.reg = frame_ctrl_field.val.reg;
            hcan->Instance->TxBuffExt_WO.FrameIdentifier1.bits.ID28_21 = ((pTxMsg->ExtId >> 21) & 0xFF);
            hcan->Instance->TxBuffExt_WO.FrameIdentifier2.bits.ID20_13 = ((pTxMsg->ExtId >> 13) & 0xFF);
            hcan->Instance->TxBuffExt_WO.FrameIdentifier3.bits.ID12_5  = ((pTxMsg->ExtId >> 5) & 0xFF);
            hcan->Instance->TxBuffExt_WO.FrameIdentifier4.bits.ID4_0   = ( pTxMsg->ExtId & 0x1F);
            for(int i = 0; i < pTxMsg->DLC && i < 8; i++)
            {
                hcan->Instance->TxBuffExt_WO.DATA[i].reg = pTxMsg->Data[i];
            }
        }
        if(hcan->Instance->MOD.reg & CAN_MODE_LOOPBACK)
        {
            hcan->Instance->CMR.reg = CAN_CMD_SRR;
        }
        else
        {
            hcan->Instance->CMR.reg = CAN_CMD_TR;
        }
//        while(!__HAL_CAN_GET_STATUS(hcan, CAN_STATUS_TX_BUFF_RELEASE))
//        {
//            /* Check for the Timeout */
//            if (Timeout != HAL_MAX_DELAY && (HAL_GetTick() - tickstart) > Timeout)
//            {
//                /* Update error code */
//                hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;
//                __HAL_UNLOCK(hcan);
//                return HAL_ERROR;
//            }
//        }
//        if(!__HAL_CAN_GET_STATUS(hcan, CAN_STATUS_TX_COMPLATE))
//        {
//            /* Update error code */
//            hcan->ErrorCode |= HAL_CAN_ERROR_TX_ALST;
//            __HAL_UNLOCK(hcan);
//            return HAL_ERROR;
//        }
        /* Update error code */
        hcan->ErrorCode = HAL_CAN_ERROR_NONE;
        __HAL_UNLOCK(hcan);
        return HAL_OK;
    }
    return HAL_ERROR;
}

/**
  * @brief  Get an CANFD frame from the Rx FIFO zone into the message RAM.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  pRxMsg pointer to a Rx Message.
  * @param  Timeout Time of wait.
  * @retval None
  */
HAL_StatusTypeDef HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, CAN_MsgTypeDef *pRxMsg, uint32_t Timeout)
{
    uint64_t tickstart;
    /* Check CAN handle */
    if (hcan == NULL)
    {
        return HAL_ERROR;
    }
    /* Get tick */
    tickstart = HAL_GetTick();
    HAL_CAN_StateTypeDef state = hcan->State;
    if ((state == HAL_CAN_STATE_READY) ||
        (state == HAL_CAN_STATE_LISTENING))
    {
        while(!__HAL_CAN_GET_STATUS(hcan, CAN_STATUS_RX_BUFF_FULL))
        {
            /* Check for the Timeout */
            if (Timeout != HAL_MAX_DELAY)
            {
                /* Check for the Timeout */
                if (Timeout != HAL_MAX_DELAY && (HAL_GetTick() - tickstart) > Timeout)
                {
                    /* Update error code */
                    hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;
                    __HAL_UNLOCK(hcan);
                    return HAL_ERROR;
                }
            }
        }
        pRxMsg->IDE = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.IDE;
        if(pRxMsg->IDE == CAN_ID_STD)
        {
            pRxMsg->StdId = (((hcan->Instance->RxBuffStd_RO.FrameIdentifier1.bits.ID10_3 << 3) | \
                               hcan->Instance->RxBuffStd_RO.FrameIdentifier2.bits.ID2_0) & 0x7FF);
            pRxMsg->RTR = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.RTR;
            pRxMsg->DLC = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.DLC;
            for(int i = 0; i < pRxMsg->DLC && i < 8; i++)
            {
                pRxMsg->Data[i] = hcan->Instance->RxBuffStd_RO.DATA[i].reg;
            }
        }
        else if(pRxMsg->IDE == CAN_ID_EXT)
        {
            pRxMsg->ExtId = (((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier1.bits.ID28_21 & 0xFF) << 21) | \
                             ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier2.bits.ID20_13 & 0xFF) << 13) | \
                             ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier3.bits.ID12_5 & 0xFF) << 5) | \
                             ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier4.bits.ID4_0 & 0xFF)));
            pRxMsg->RTR = hcan->Instance->RxBuffExt_RO.FrameCtrlField.bits.RTR;
            pRxMsg->DLC = hcan->Instance->RxBuffExt_RO.FrameCtrlField.bits.DLC;
            for(int i = 0; i < pRxMsg->DLC && i < 8; i++)
            {
                pRxMsg->Data[i] = hcan->Instance->RxBuffExt_RO.DATA[i].reg;
            }
        }
        hcan->Instance->CMR.reg = CAN_CMD_RRB;
        return HAL_OK;
    }
    return HAL_ERROR;
}

/**
  * @brief  Receive a Message, use for Interrupt Handle.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  pRxMsg pointer to a Rx Message.
  * @param  Timeout Time of wait.
  * @retval status
  */
HAL_StatusTypeDef HAL_CAN_GetRxMessage_IT(CAN_HandleTypeDef *hcan, CAN_MsgTypeDef *pRxMsg)
{
    /* Check CAN handle */
    if (hcan == NULL)
    {
        return HAL_ERROR;
    }
    if (hcan->State != HAL_CAN_STATE_ERROR)
    {
        hcan->pRxMsg = pRxMsg;
        hcan->pRxMsg->IDE = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.IDE;
        if(hcan->pRxMsg->IDE == CAN_ID_STD)
        {
            hcan->pRxMsg->StdId = (((hcan->Instance->RxBuffStd_RO.FrameIdentifier1.bits.ID10_3 << 3) | \
                                     hcan->Instance->RxBuffStd_RO.FrameIdentifier2.bits.ID2_0) & 0x7FF);
            hcan->pRxMsg->RTR = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.RTR;
            hcan->pRxMsg->DLC = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.DLC;
            for(int i = 0; i < hcan->pRxMsg->DLC && i < 8; i++)
            {
                hcan->pRxMsg->Data[i] = hcan->Instance->RxBuffStd_RO.DATA[i].reg;
            }
        }
        else if(hcan->pRxMsg->IDE == CAN_ID_EXT)
        {
            hcan->pRxMsg->ExtId = (((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier1.bits.ID28_21 & 0xFF) << 21) | \
                                 ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier2.bits.ID20_13 & 0xFF) << 13) | \
                                 ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier3.bits.ID12_5 & 0xFF) << 5) | \
                                 ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier4.bits.ID4_0 & 0xFF)));
            hcan->pRxMsg->RTR = hcan->Instance->RxBuffExt_RO.FrameCtrlField.bits.RTR;
            hcan->pRxMsg->DLC = hcan->Instance->RxBuffExt_RO.FrameCtrlField.bits.DLC;
            for(int i = 0; i < hcan->pRxMsg->DLC && i < 8; i++)
            {
                hcan->pRxMsg->Data[i] = hcan->Instance->RxBuffExt_RO.DATA[i].reg;
            }
        }
        hcan->Instance->CMR.reg = CAN_CMD_RRB;
        return HAL_OK;
    }
    return HAL_ERROR;
}

static void CAN_RX_IT_Handle(CAN_HandleTypeDef *hcan)
{
    if(hcan->pRxMsg == NULL)
    {
        return;
    }
    hcan->pRxMsg->IDE = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.IDE;
    if(hcan->pRxMsg->IDE == CAN_ID_STD)
    {
        hcan->pRxMsg->StdId = (((hcan->Instance->RxBuffStd_RO.FrameIdentifier1.bits.ID10_3 << 3) | \
                                 hcan->Instance->RxBuffStd_RO.FrameIdentifier2.bits.ID2_0) & 0x7FF);
        hcan->pRxMsg->RTR = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.RTR;
        hcan->pRxMsg->DLC = hcan->Instance->RxBuffStd_RO.FrameCtrlField.bits.DLC;
        for(int i = 0; i < hcan->pRxMsg->DLC && i < 8; i++)
        {
            hcan->pRxMsg->Data[i] = hcan->Instance->RxBuffStd_RO.DATA[i].reg;
        }
    }
    else if(hcan->pRxMsg->IDE == CAN_ID_EXT)
    {
        hcan->pRxMsg->ExtId = (((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier1.bits.ID28_21 & 0xFF) << 21) | \
                             ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier2.bits.ID20_13 & 0xFF) << 13) | \
                             ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier3.bits.ID12_5 & 0xFF) << 5) | \
                             ((uint32_t)(hcan->Instance->RxBuffExt_RO.FrameIdentifier4.bits.ID4_0 & 0xFF)));
        hcan->pRxMsg->RTR = hcan->Instance->RxBuffExt_RO.FrameCtrlField.bits.RTR;
        hcan->pRxMsg->DLC = hcan->Instance->RxBuffExt_RO.FrameCtrlField.bits.DLC;
        for(int i = 0; i < hcan->pRxMsg->DLC && i < 8; i++)
        {
            hcan->pRxMsg->Data[i] = hcan->Instance->RxBuffExt_RO.DATA[i].reg;
        }
    }
}
/**
  * @brief  Handles CAN interrupt request
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_IRQHandler(CAN_HandleTypeDef *hcan)
{
    uint32_t errorcode = HAL_CAN_ERROR_NONE;
    //After the register has been read by the CPU, all except the Receive Interrupt bit are reset
    hcan->int_flag = hcan->Instance->ISR.reg;
    if(hcan->int_flag & CAN_IT_RX)
    {
//        CAN_RX_IT_Handle(hcan);
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->RxMsgCallback)
        {
            hcan->RxMsgCallback(hcan);
        }
#else
        HAL_CAN_RxMsgCallback(hcan);
#endif
        hcan->Instance->CMR.reg = CAN_CMD_RRB;
    }
    if(hcan->int_flag & CAN_IT_TX)
    {
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->TxCompleteCallback)
        {
            hcan->TxCompleteCallback(hcan);
        }
#else
        HAL_CAN_TxCompleteCallback(hcan);
#endif
    }
    if(hcan->int_flag & CAN_IT_ERROR_WARNING)
    {
        hcan->ErrorCode = hcan->Instance->ECC.reg;
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->ErrorCallback)
        {
            hcan->ErrorCallback(hcan);
        }
#else
        HAL_CAN_ErrorCallback(hcan);
#endif
    }
    if(hcan->int_flag & CAN_IT_DATA_OVERRUN)
    {
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->DataOverrunCallback)
        {
            hcan->DataOverrunCallback(hcan);
        }
#else
        HAL_CAN_DataOverrunCallback(hcan);
#endif
    }
    if(hcan->int_flag & CAN_IT_WAKE_UP)
    {
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->WakeUpCallback)
        {
            hcan->WakeUpCallback(hcan);
        }
#else
        HAL_CAN_WakeUpCallback(hcan);
#endif
    }
    if(hcan->int_flag & CAN_IT_PASSIVE_ERROR)
    {
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->PassiveCallback)
        {
            hcan->PassiveCallback(hcan);
        }
#else
        HAL_CAN_PassiveCallback(hcan);
#endif
    }
    if(hcan->int_flag & CAN_IT_ARB_LOST)
    {
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->ArbLostCallback)
        {
            hcan->ArbLostCallback(hcan);
        }
#else
        HAL_CAN_ArbLostCallback(hcan);
#endif
    }
    if(hcan->int_flag & CAN_IT_BUS_ERROR)
    {
        hcan->ErrorCode = hcan->Instance->ECC.reg;
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        if(hcan->BusErrorCallback)
        {
            hcan->BusErrorCallback(hcan);
        }
#else
        HAL_CAN_BusErrorCallback(hcan);
#endif
    }
}
/**
  * @brief  Setting CAN baud rate
                        only write in reset mode.
  * @param  CANx: where x can be CAN2 or CAN3 to select the CAN peripheral.
    * @param  baudrate: baud rate to be set,
                    where can be CAN_BaudRate_10k ~ CAN_BaudRate_800k.
  * @retval 1 for true / 0 for false.
  */
HAL_StatusTypeDef HAL_CAN_SetBandrate(CAN_HandleTypeDef *hcan, const CAN_BaudrateTypeDef *baudrate)
{
    /* Check the parameters */
    assert_param(hcan != NULL);
    assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
    SET_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Reset Mode Selected
    hcan->Instance->CDR.reg = CAN_OutClock_OFF;
    CAN_SetBandrate(hcan, (CAN_BaudrateTypeDef *)baudrate);
    hcan->Instance->CDR.reg = hcan->Init.timing.div;
    hcan->Instance->OCR.reg = CAN_OUTPUT_MODE_CLOCK;//CAN_OUTPUT_MODE_NORMAL;
    CLEAR_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Normal operation
    return HAL_OK;
}
HAL_StatusTypeDef HAL_CAN_SetBandrateAuto(CAN_HandleTypeDef *hcan, CAN_Baudrate_Enum baudrate)
{
    /* Check the parameters */
    assert_param(hcan != NULL);
    assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
    // baudRate = CAN_CLK/(2*(1 + BRP)*( 1 + (TSEG1 + 1) + (TSEG2 + 1)))
    CAN_BaudrateTypeDef *timing = &hcan->Init.timing;
    hcan->Init.BaudRate = baudrate;
    switch (hcan->Init.BaudRate)
    {
        case CAN_BaudRate_10k:{
            timing->brp = 59;
            timing->tSJW = 1;
            timing->tSeg_1 = 15;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_20k:{
            timing->brp = 59;
            timing->tSJW = 1;
            timing->tSeg_1 = 15;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_40k:{
            timing->brp = 29;
            timing->tSJW = 1;
            timing->tSeg_1 = 15;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_50k:{
            timing->brp = 23;
            timing->tSJW = 1;
            timing->tSeg_1 = 15;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
            }break;
        case CAN_BaudRate_80k:{
            timing->brp = 14;
            timing->tSJW = 1;
            timing->tSeg_1 = 15;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_100k:{
            timing->brp = 11;
            timing->tSJW = 1;
            timing->tSeg_1 = 15;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_125k:{
            timing->brp = 15;
            timing->tSJW = 1;
            timing->tSeg_1 = 8;
            timing->tSeg_2 = 1;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_200k:{
            timing->brp = 5;
            timing->tSJW = 1;
            timing->tSeg_1 = 15;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_250k:{
            timing->brp = 7;
            timing->tSJW = 1;
            timing->tSeg_1 = 8;
            timing->tSeg_2 = 1;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_400k:{
            timing->brp = 4;
            timing->tSJW = 1;
            timing->tSeg_1 = 8;
            timing->tSeg_2 = 1;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_500k:{
            timing->brp = 3;
            timing->tSJW = 1;
            timing->tSeg_1 = 8;
            timing->tSeg_2 = 1;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_800k:{
            timing->brp = 1;
            timing->tSJW = 1;
            timing->tSeg_1 = 10;
            timing->tSeg_2 = 2;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        case CAN_BaudRate_1000k:{
            timing->brp = 1;
            timing->tSJW = 1;
            timing->tSeg_1 = 8;
            timing->tSeg_2 = 1;
            timing->div = CAN_OutClock_PRE_8;
            timing->SAM = 0;
        }break;
        default : return HAL_ERROR;
    }
    return HAL_CAN_SetBandrate(hcan, timing);
}

static void CAN_SetBandrate(CAN_HandleTypeDef *hcan, CAN_BaudrateTypeDef *timing)
{
    // baudRate = CAN_CLK/(2*(1 + BRP)*( 1 + (TSEG1 + 1) + (TSEG2 + 1)))
    hcan->Init.timing.brp = timing->brp;
    hcan->Init.timing.tSeg_1 = timing->tSeg_1;
    hcan->Init.timing.tSeg_2 = timing->tSeg_2;
    hcan->Init.timing.tSJW = timing->tSJW;
    hcan->Init.timing.div = timing->div;
    hcan->Init.timing.SAM = timing->SAM;
    hcan->Instance->BTR0.reg = ((hcan->Init.timing.tSJW << 6) | hcan->Init.timing.brp);
    hcan->Instance->BTR1.reg = ((hcan->Init.timing.SAM << 7) | (hcan->Init.timing.tSeg_2 << 4) | hcan->Init.timing.tSeg_1);
}

void HAL_CAN_SetMode(CAN_HandleTypeDef *hcan, uint32_t mode)
{
    hcan->Init.Mode = mode;
    SET_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Reset Mode Selected
    MODIFY_REG(hcan->Instance->MOD.reg, CAN_MODE_Listen_Only | CAN_MODE_LOOPBACK, mode);
    CLEAR_BIT(hcan->Instance->MOD.reg, CAN_MODE_Reset);// Normal operation
}
#endif /* HAL_CAN_MODULE_ENABLED */
