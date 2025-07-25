/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-26     huyt         V1.0.1
 */
#include <gc90xx_hal.h>

#ifdef HAL_CANFD_MODULE_ENABLED


#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
static void CANFD_InitCallbacksToDefault(CANFD_HandleTypeDef *hcanfd);
#endif /* USE_HAL_CANFD_REGISTER_CALLBACKS */

static void CANFD_EnterResetMode(CANFD_HandleTypeDef *hcanfd);
static void CANFD_QuitResetMode(CANFD_HandleTypeDef *hcanfd);

static void CANFD_SetBaudrateParamSlow(CANFD_HandleTypeDef *hcanfd);
static void CANFD_SetBaudrateParamFast(CANFD_HandleTypeDef *hcanfd);

static void CANFD_SetBitTimingSlow(CANFD_HandleTypeDef *hcanfd, uint8_t seg_1, uint8_t seg_2, uint8_t SJW);
static void CANFD_SetPrescalerSlow(CANFD_HandleTypeDef *hcanfd, uint8_t prescaler);
static void CANFD_SetBitTimingFast(CANFD_HandleTypeDef *hcanfd, uint8_t seg_1, uint8_t seg_2, uint8_t SJW);
static void CANFD_SetPrescalerFast(CANFD_HandleTypeDef *hcanfd, uint8_t prescaler);

static void CANFD_TDC_config(CANFD_HandleTypeDef *hcanfd, uint8_t TDC_Enable, uint8_t SSP_offset);



#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
/**
  * @brief  Initialize the callbacks to their default values.
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval none
  */
static void CANFD_InitCallbacksToDefault(CANFD_HandleTypeDef *hcanfd)
{
    hcanfd->AbortCallback = HAL_CANFD_AbortCallback;
    hcanfd->ErrorCallback = HAL_CANFD_ErrorCallback;
    hcanfd->TxSecCpltCallback = HAL_CANFD_TxSecClptCallback;
    hcanfd->TxPriCpltCallback = HAL_CANFD_TxPriClptCallback;
    hcanfd->RxBuffAlmostFullCallback = HAL_CANFD_RxBuffAlmostFullCallback;
    hcanfd->RxBuffFullCallback = HAL_CANFD_RxBuffFullCallback;
    hcanfd->RxBuffOverrunCallback = HAL_CANFD_RxBuffOverrunCallback;
    hcanfd->RxMsgCallback = HAL_CANFD_RxMsgCallback;
    hcanfd->PassiveErrorCallback = HAL_CANFD_PassiveErrorCallback;
    hcanfd->ArbitrationLostCallback = HAL_CANFD_ArbitrationLostCallback;
    hcanfd->BusErrorCallback = HAL_CANFD_BusErrorCallback;
}
#endif /* USE_HAL_CANFD_REGISTER_CALLBACKS */

static HAL_StatusTypeDef CANFD_CheckMsgStruct(CANFD_MsgTypeDef* Message)
{
	if(Message->DLC > CANFD_DLC_64)
	{
	    return HAL_ERROR;
	}
	if((Message->BRS != CANFD_BRS_Disable) && (Message->BRS != CANFD_BRS_Enable))
	{
        return HAL_ERROR;
	}
	if((Message->FDF != CANFD_Frame_CAN20) && (Message->FDF != CANFD_Frame_CANFD))
	{
	    return HAL_ERROR;
	}
	if((Message->RTR != CANFD_RTR_DATA) && (Message->RTR != CANFD_RTR_REMOTE))
	{
	    return HAL_ERROR;
	}
    if(!IS_CANFD_STD_ID(Message->StdId) || !IS_CANFD_EXT_ID(Message->ExtId))
	{
	    return HAL_ERROR;
	}
	return HAL_OK;
}
/**
  * @brief  CANFD TDC config
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  TDC_Enable: TDC enable select.
  * @param  SSP_offset: Secondary Sample Point OFFset
  * @retval None.
  */
static void CANFD_TDC_config(CANFD_HandleTypeDef *hcanfd, uint8_t TDC_Enable, uint8_t SSP_offset)
{
	if(TDC_Enable == ENABLE)
	{// TDC[8:15]
	    hcanfd->Instance->TECNT_RECNT_TDC_EALCAP.value &= ~CANFD_SSPOFF;
	    hcanfd->Instance->TECNT_RECNT_TDC_EALCAP.value |= (SSP_offset << 8);
	    hcanfd->Instance->TECNT_RECNT_TDC_EALCAP.value |= CANFD_TDCEN;
	}
	else
	{
	    hcanfd->Instance->TECNT_RECNT_TDC_EALCAP.value &= ~CANFD_TDCEN;
	}
}
/**
  * @brief  CANFD automatic resend configuration
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  TDC_Enable: TDC enable select.
  * @param  SSP_offset: Secondary Sample Point OFFset
  * @retval None.
  */
HAL_StatusTypeDef HAL_CANFD_AutoReTxCtrl(CANFD_HandleTypeDef *hcanfd, CANFD_TBSEL_TypeDef tb, bool enable)
{
    /* Check CANFD handle */
    if (hcanfd == NULL)
    {
        return HAL_ERROR;
    }
    if(tb == CANFD_TBSEL_PTB)
    {
        MODIFY_REG(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TPSS, (enable ? 0U : CANFD_TPSS));
    }
    else if(tb == CANFD_TBSEL_STB)
    {
        MODIFY_REG(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TSSS, (enable ? 0U : CANFD_TSSS));
    }
    /* Return function status */
    return HAL_OK;
}
/**
  * @brief  Transmit a Primary Message.
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  pTxMsg pointer to a TX Message.
  * @param  Timeout Time of wait.
  * @retval None
  */
HAL_StatusTypeDef HAL_CANFD_TxPrimaryMessage(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pTxMsg, uint32_t Timeout)
{
    uint32_t tickstart;
    /* Check CANFD handle */
    if (hcanfd == NULL || pTxMsg == NULL)
    {
        return HAL_ERROR;
    }
    /* Get tick */
    tickstart = HAL_GetTick();
    HAL_CANFD_StateTypeDef state = hcanfd->State;
    if ((state == HAL_CANFD_STATE_READY) ||
        (state == HAL_CANFD_STATE_LISTENING))
    {
        __HAL_LOCK(hcanfd);
        if(Timeout == 0)
        {
            if((hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_TSSTAT) == CANFD_TSSTAT)
            {
                /* Update error code */
                hcanfd->ErrorCode |= HAL_CANFD_ERROR_TIMEOUT;
                __HAL_UNLOCK(hcanfd);
                return HAL_ERROR;
            }
        } else {
            // wait PTB empty
            while((hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_TSSTAT) == CANFD_TSSTAT)
            {
                /* Check for the Timeout */
                if ((HAL_GetTick() - tickstart) > Timeout)
                {
                    /* Update error code */
                    hcanfd->ErrorCode |= HAL_CANFD_ERROR_TIMEOUT;
                    __HAL_UNLOCK(hcanfd);
                    return HAL_ERROR;
                }
            }
        }
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TBSEL);
        if(CANFD_CheckMsgStruct(pTxMsg) != HAL_OK)
        {
            return HAL_ERROR;
        }
        union CANFD_TX_FrameIdentifier frame_identifier_field;
        frame_identifier_field.std_bits.TTSEN = (pTxMsg->Timestamp != 0 ? 1 : 0);
        if(pTxMsg->IDE == CANFD_ID_STD)//standard frame
        {
            frame_identifier_field.std_bits.ID10_0 = pTxMsg->StdId;
        }
        else if(pTxMsg->IDE == CANFD_ID_EXT)//Extend frame
        {
            frame_identifier_field.ext_bits.ID28_0 = pTxMsg->ExtId;
        }
        hcanfd->Instance->TxBuff.FrameIdentifier.reg_u32val = frame_identifier_field.reg_u32val;
        union CANFD_TX_FrameCtrlField frame_ctrl_field;
        frame_ctrl_field.bits.DLC = pTxMsg->DLC;
        frame_ctrl_field.bits.BRS = pTxMsg->BRS;
        frame_ctrl_field.bits.FDF = pTxMsg->FDF;
        frame_ctrl_field.bits.RTR = pTxMsg->RTR;
        frame_ctrl_field.bits.IDE = pTxMsg->IDE;
        hcanfd->Instance->TxBuff.FrameCtrlField.reg_u32val = frame_ctrl_field.reg_u32val;
        //write data items number may be optimize with DLC
        if(pTxMsg->FDF == CANFD_Frame_CAN20)
        {
            for(int i = 0; i < 8; i += 4)
            {
                hcanfd->Instance->TxBuff.DATA[i/4] = ReadUint32_LittleEndian(&pTxMsg->Data[i]);
            }
        }
        else if(pTxMsg->FDF == CANFD_Frame_CANFD)
        {
            for(int i = 0; i < 64; i += 4)
            {
                hcanfd->Instance->TxBuff.DATA[i/4] = ReadUint32_LittleEndian(&pTxMsg->Data[i]);
            }
        }
        /* Request transmission */
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TPE); //start transmit from PTB
        /* Update error code */
        hcanfd->ErrorCode = HAL_CANFD_ERROR_NONE;
        __HAL_UNLOCK(hcanfd);
        /* Return function status */
        return HAL_OK;
    }
    else
    {
        /* Update error code */
        hcanfd->ErrorCode |= HAL_CANFD_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    }
}
/**
  * @brief  Add a TX Message.
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  pTxMsg pointer to a TX Message.
  * @retval None
  */
HAL_StatusTypeDef HAL_CANFD_AddTxMessage(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pTxMsg)
{
    /* Check CANFD handle */
    if (hcanfd == NULL || pTxMsg == NULL)
    {
        return HAL_ERROR;
    }
    HAL_CANFD_StateTypeDef state = hcanfd->State;
    uint32_t tsr = READ_REG(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value);
    if ((state == HAL_CANFD_STATE_READY) ||
        (state == HAL_CANFD_STATE_LISTENING))
    {
        if((tsr & CANFD_TSSTAT) == CANFD_TSSTAT)
        {//STB is full
            /* Update error code */
            hcanfd->ErrorCode |= HAL_CANFD_ERROR_TB_FULL;
            return HAL_ERROR;
        }
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TBSEL);
        if(CANFD_CheckMsgStruct(pTxMsg) != HAL_OK)
        {
            return HAL_ERROR;
        }
        union CANFD_TX_FrameIdentifier frame_identifier_field;
        frame_identifier_field.std_bits.TTSEN = (pTxMsg->Timestamp != 0 ? 1 : 0);
        if(pTxMsg->IDE == CANFD_ID_STD)//standard frame
        {
            frame_identifier_field.std_bits.ID10_0 = pTxMsg->StdId;
        }
        else if(pTxMsg->IDE == CANFD_ID_EXT)//Extend frame
        {
            frame_identifier_field.ext_bits.ID28_0 = pTxMsg->ExtId;
        }
        hcanfd->Instance->TxBuff.FrameIdentifier.reg_u32val = frame_identifier_field.reg_u32val;
        union CANFD_TX_FrameCtrlField frame_ctrl_field;
        frame_ctrl_field.bits.DLC = pTxMsg->DLC;
        frame_ctrl_field.bits.BRS = pTxMsg->BRS;
        frame_ctrl_field.bits.FDF = pTxMsg->FDF;
        frame_ctrl_field.bits.RTR = pTxMsg->RTR;
        frame_ctrl_field.bits.IDE = pTxMsg->IDE;
        hcanfd->Instance->TxBuff.FrameCtrlField.reg_u32val = frame_ctrl_field.reg_u32val;
        //write data items number may be optimize with DLC
        if(pTxMsg->FDF == CANFD_Frame_CAN20)
        {
            for(int i = 0; i < 8; i+=4)
            {
                hcanfd->Instance->TxBuff.DATA[i/4] = ReadUint32_LittleEndian(&pTxMsg->Data[i]);
            }
        }
        else if(pTxMsg->FDF == CANFD_Frame_CANFD)
        {
            for(int i = 0; i < 64; i+=4)
            {
                hcanfd->Instance->TxBuff.DATA[i/4] = ReadUint32_LittleEndian(&pTxMsg->Data[i]);
            }
        }
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TSNEXT);//notify this STB slot filled, TBUF point to next slot
        /* Request transmission */
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TSONE);//start transmit from STB
        return HAL_OK;
    }
    return HAL_ERROR;
}
/**
  * @brief  Abort Primary TX Message Request.
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval None
  */
HAL_StatusTypeDef HAL_CANFD_AbortTxPrimaryRequest(CANFD_HandleTypeDef *hcanfd)
{
    /* Check CANFD handle */
    if (hcanfd == NULL)
    {
        return HAL_ERROR;
    }
    HAL_CANFD_StateTypeDef state = hcanfd->State;
    if ((state == HAL_CANFD_STATE_READY) ||
        (state == HAL_CANFD_STATE_LISTENING))
    {
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TPA);//start transmit from PTB
        while(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_TPE);
        return HAL_OK;
    }
    else
    {
        /* Update error code */
        hcanfd->ErrorCode |= HAL_CANFD_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    }
}
/**
  * @brief  Abort Secondary TX Message Request.
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval None
  */
HAL_StatusTypeDef HAL_CANFD_AbortTxSecondaryRequest(CANFD_HandleTypeDef *hcanfd)
{
    /* Check CANFD handle */
    if (hcanfd == NULL)
    {
        return HAL_ERROR;
    }
    HAL_CANFD_StateTypeDef state = hcanfd->State;
    if ((state == HAL_CANFD_STATE_READY) ||
        (state == HAL_CANFD_STATE_LISTENING))
    {
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TSA);//start transmit from STB
        while(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_TSONE);
        while(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_TSALL);
        return HAL_OK;
    }
    else
    {
        /* Update error code */
        hcanfd->ErrorCode |= HAL_CANFD_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    }
}
/**
  * @brief  Get an CANFD frame from the Rx FIFO zone into the message RAM.
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  pRxMsg pointer to a Rx Message.
  * @param  Timeout Time of wait.
  * @retval None
  */
HAL_StatusTypeDef HAL_CANFD_GetRxMessage(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pRxMsg, uint32_t Timeout)
{
    uint32_t tickstart;
    /* Check CANFD handle */
    if (hcanfd == NULL || pRxMsg == NULL)
    {
        return HAL_ERROR;
    }
    /* Get tick */
    tickstart = HAL_GetTick();
    HAL_CANFD_StateTypeDef state = hcanfd->State;
    if ((state == HAL_CANFD_STATE_READY) ||
        (state == HAL_CANFD_STATE_LISTENING))
    {
        while((hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_RSTAT) == RESET)
        {
            /* Check for the Timeout */
            if (Timeout != HAL_MAX_DELAY)
            {
                /* Check for the Timeout */
                if ((HAL_GetTick() - tickstart) > Timeout)
                {
                    /* Update error code */
                    hcanfd->ErrorCode |= HAL_CANFD_ERROR_TIMEOUT;
                    hcanfd->State = state;
                    return HAL_ERROR;
                }
            }
        }
        union CANFD_RX_FrameCtrlField rx_frame_ctrl_field;
        union CANFD_RX_FrameIdentifier rx_frame_ident;
        rx_frame_ctrl_field.reg_u32val = hcanfd->Instance->RxBuff.FrameCtrlField.reg_u32val;
        rx_frame_ident.reg_u32val = hcanfd->Instance->RxBuff.FrameIdentifier.reg_u32val;
        pRxMsg->IDE = rx_frame_ctrl_field.bits.IDE;
        if(pRxMsg->IDE == CANFD_ID_STD)
        {
            pRxMsg->StdId = rx_frame_ident.std_bits.ID10_0;
        }
        else if(pRxMsg->IDE == CANFD_ID_EXT)
        {
            pRxMsg->ExtId = rx_frame_ident.ext_bits.ID28_0;
        }
        pRxMsg->DLC = rx_frame_ctrl_field.bits.DLC;
        pRxMsg->BRS = rx_frame_ctrl_field.bits.BRS;
        pRxMsg->FDF = rx_frame_ctrl_field.bits.FDF;
        pRxMsg->RTR = rx_frame_ctrl_field.bits.RTR;
        if(pRxMsg->FDF == CANFD_Frame_CAN20)
        {
            for(int i = 0; i < 8; i += 4)
            {
                WriteUint32_LittleEndian(&pRxMsg->Data[i], hcanfd->Instance->RxBuff.DATA[i/4]);
            }
        }
        else if(pRxMsg->FDF == CANFD_Frame_CANFD)
        {
            for(int i = 0; i < 64; i += 4)
            {
                WriteUint32_LittleEndian(&pRxMsg->Data[i], hcanfd->Instance->RxBuff.DATA[i/4]);
            }
        }
        pRxMsg->Timestamp = hcanfd->Instance->RxBuff.RTS_MSB;
        pRxMsg->Timestamp <<= 32;
        pRxMsg->Timestamp |= hcanfd->Instance->RxBuff.RTS_LSB;
        //notify RBUF have been read
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_RREL);
        return HAL_OK;
    }
    return HAL_ERROR;
}
/**
  * @brief  Get an CANFD frame from the Rx FIFO zone into the message RAM.
  *         Only use in interrupt handle
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  pRxMsg pointer to a Rx Message.
  * @retval None
  */
HAL_StatusTypeDef HAL_CANFD_GetRxMessage_IT(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pRxMsg)
{
    /* Check CANFD handle */
    if (hcanfd == NULL || pRxMsg == NULL)
    {
        return HAL_ERROR;
    }
//    while((hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_RSTAT) == RESET);
    union CANFD_RX_FrameCtrlField rx_frame_ctrl_field;
    union CANFD_RX_FrameIdentifier rx_frame_ident;
    rx_frame_ctrl_field.reg_u32val = hcanfd->Instance->RxBuff.FrameCtrlField.reg_u32val;
    rx_frame_ident.reg_u32val = hcanfd->Instance->RxBuff.FrameIdentifier.reg_u32val;
    pRxMsg->IDE = rx_frame_ctrl_field.bits.IDE;
    if(pRxMsg->IDE == CANFD_ID_STD)
    {
        pRxMsg->StdId = rx_frame_ident.std_bits.ID10_0;
    }
    else if(pRxMsg->IDE == CANFD_ID_EXT)
    {
        pRxMsg->ExtId = rx_frame_ident.ext_bits.ID28_0;
    }
    pRxMsg->DLC = rx_frame_ctrl_field.bits.DLC;
    pRxMsg->BRS = rx_frame_ctrl_field.bits.BRS;
    pRxMsg->FDF = rx_frame_ctrl_field.bits.FDF;
    pRxMsg->RTR = rx_frame_ctrl_field.bits.RTR;
    if(pRxMsg->FDF == CANFD_Frame_CAN20)
    {
        for(int i = 0; i < 8; i += 4)
        {
            WriteUint32_LittleEndian(&pRxMsg->Data[i], hcanfd->Instance->RxBuff.DATA[i/4]);
        }
    }
    else if(pRxMsg->FDF == CANFD_Frame_CANFD)
    {
        for(int i = 0; i < 64; i += 4)
        {
            WriteUint32_LittleEndian(&pRxMsg->Data[i], hcanfd->Instance->RxBuff.DATA[i/4]);
        }
    }
    pRxMsg->Timestamp = hcanfd->Instance->RxBuff.RTS_MSB;
    pRxMsg->Timestamp <<= 32;
    pRxMsg->Timestamp |= hcanfd->Instance->RxBuff.RTS_LSB;
    //notify RBUF have been read
    SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_RREL);
    return HAL_OK;
}
/**
  * @brief  CANFD Interrupt Enable
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  interrupt specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg CANFD_IT_AIF    //Abort Interrupt.
  *            @arg CANFD_IT_EIE    //Error Interrupt.
  *            @arg CANFD_IT_TSIE   //Transmission Secondary Interrupt.
  *            @arg CANFD_IT_TPIE   //Transmission Primary Interrupt.
  *            @arg CANFD_IT_RAFIE  //RB Almost Full Interrupt.
  *            @arg CANFD_IT_RFIE   //RB Full Interrupt.
  *            @arg CANFD_IT_ROIE   //RB Overrun Interrupt.
  *            @arg CANFD_IT_RIE    //Receive Interrupt.
  *            @arg CANFD_IT_BEIE   //Bus Error Interrupt.
  *            @arg CANFD_IT_ALIE   //Arbitration Lost Interrupt.
  *            @arg CANFD_IT_EPIE   //Error Passive Interrupt.
  *            @arg CANFD_IT_TTIE   //Time Trigger Interrupt.
  *            @arg CANFD_IT_TEIF   //Trigger Error Interrupt.
  *            @arg CANFD_IT_WTIE   //Watch Trigger Interrupt.
  * @retval None
  */
void HAL_CANFD_IT_Enable(CANFD_HandleTypeDef *hcanfd, uint32_t interrupt)
{
    if(hcanfd == NULL)
    {
        return;
    }
    if(CANFD_IT_ALL & interrupt)
    {
        SET_BIT(hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE, (CANFD_IT_ALL & interrupt));
    }
    if(CANFD_IT_OTHER & interrupt)
    {
        SET_BIT(hcanfd->Instance->TTCFG_TBSLOT_VER1_VER0, (CANFD_IT_OTHER & interrupt));
    }
}
/**
  * @brief  CANFD Interrupt Disable
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  interrupt specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg CANFD_IT_AIF    //Abort Interrupt.
  *            @arg CANFD_IT_EIE    //Error Interrupt.
  *            @arg CANFD_IT_TSIE   //Transmission Secondary Interrupt.
  *            @arg CANFD_IT_TPIE   //Transmission Primary Interrupt.
  *            @arg CANFD_IT_RAFIE  //RB Almost Full Interrupt.
  *            @arg CANFD_IT_RFIE   //RB Full Interrupt.
  *            @arg CANFD_IT_ROIE   //RB Overrun Interrupt.
  *            @arg CANFD_IT_RIE    //Receive Interrupt.
  *            @arg CANFD_IT_BEIE   //Bus Error Interrupt.
  *            @arg CANFD_IT_ALIE   //Arbitration Lost Interrupt.
  *            @arg CANFD_IT_EPIE   //Error Passive Interrupt.
  *            @arg CANFD_IT_TTIE   //Time Trigger Interrupt.
  *            @arg CANFD_IT_TEIF   //Trigger Error Interrupt.
  *            @arg CANFD_IT_WTIE   //Watch Trigger Interrupt.
  * @retval None
  */
void HAL_CANFD_IT_Disable(CANFD_HandleTypeDef *hcanfd, uint32_t interrupt)
{
    if(hcanfd == NULL)
    {
        return;
    }
    if(CANFD_IT_ALL & interrupt)
    {
        CLEAR_BIT(hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE, (CANFD_IT_ALL & interrupt));
    }
    if(CANFD_IT_OTHER & interrupt)
    {
        CLEAR_BIT(hcanfd->Instance->TTCFG_TBSLOT_VER1_VER0, (CANFD_IT_OTHER & interrupt));
    }
}
/**
  * @brief  Get CANFD Interrupt Flag
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  interrupt specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg CANFD_FLAG_AIF    //Abort Interrupt.
  *            @arg CANFD_FLAG_EIF    //Error Interrupt.
  *            @arg CANFD_FLAG_TSIF   //Transmission Secondary Interrupt.
  *            @arg CANFD_FLAG_TPIF   //Transmission Primary Interrupt.
  *            @arg CANFD_FLAG_RAFIF  //RB Almost Full Interrupt.
  *            @arg CANFD_FLAG_RFIF   //RB Full Interrupt.
  *            @arg CANFD_FLAG_ROIF   //RB Overrun Interrupt.
  *            @arg CANFD_FLAG_RIF    //Receive Interrupt.
  *            @arg CANFD_FLAG_BEIF   //Bus Error Interrupt.
  *            @arg CANFD_FLAG_ALIF   //Arbitration Lost Interrupt.
  *            @arg CANFD_FLAG_EPIF   //Error Passive Interrupt.
  *            @arg CANFD_FLAG_TTIF   //Time Trigger Interrupt.
  *            @arg CANFD_FLAG_TEIF   //Trigger Error Interrupt.
  *            @arg CANFD_FLAG_WTIF   //Watch Trigger Interrupt.
  * @retval The bit flag
  */
uint32_t HAL_CANFD_GetITFlag(CANFD_HandleTypeDef *hcanfd, uint32_t isrflag)
{
    uint32_t tempisr = 0;
    tempisr = (hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE & (CANFD_ISR0_FLAG_MASK & isrflag));
    tempisr |= (hcanfd->Instance->TTCFG_TBSLOT_VER1_VER0 & (CANFD_ISR1_FLAG_MASK & isrflag));
    return tempisr;
}
/**
  * @brief  Clear CANFD Interrupt Flag
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  interrupt specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg CANFD_FLAG_AIF    //Abort Interrupt.
  *            @arg CANFD_FLAG_EIF    //Error Interrupt.
  *            @arg CANFD_FLAG_TSIF   //Transmission Secondary Interrupt.
  *            @arg CANFD_FLAG_TPIF   //Transmission Primary Interrupt.
  *            @arg CANFD_FLAG_RAFIF  //RB Almost Full Interrupt.
  *            @arg CANFD_FLAG_RFIF   //RB Full Interrupt.
  *            @arg CANFD_FLAG_ROIF   //RB Overrun Interrupt.
  *            @arg CANFD_FLAG_RIF    //Receive Interrupt.
  *            @arg CANFD_FLAG_BEIF   //Bus Error Interrupt.
  *            @arg CANFD_FLAG_ALIF   //Arbitration Lost Interrupt.
  *            @arg CANFD_FLAG_EPIF   //Error Passive Interrupt.
  *            @arg CANFD_FLAG_TTIF   //Time Trigger Interrupt.
  *            @arg CANFD_FLAG_TEIF   //Trigger Error Interrupt.
  *            @arg CANFD_FLAG_WTIF   //Watch Trigger Interrupt.
  * @retval The bit flag
  */
void HAL_CANFD_ClearITFlag(CANFD_HandleTypeDef *hcanfd, uint32_t isrflag)
{
    if(isrflag & CANFD_ISR0_FLAG_MASK)
    {
        SET_BIT(hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE, (isrflag & CANFD_ISR0_FLAG_MASK));
    }
    if(isrflag & CANFD_ISR1_FLAG_MASK)
    {
        SET_BIT(hcanfd->Instance->TTCFG_TBSLOT_VER1_VER0, (isrflag & CANFD_ISR1_FLAG_MASK));
    }
}
/**
  * @brief  Handles CANFD interrupt request
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval None
  */
void HAL_CANFD_IRQHandler(CANFD_HandleTypeDef *hcanfd)
{
    uint32_t tempreg = 0;
    uint32_t int_flag = HAL_CANFD_GetITFlag(hcanfd, CANFD_FLAG_ALL);
    HAL_CANFD_ClearITFlag(hcanfd, int_flag);

    if(int_flag & CANFD_FLAG_AIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->AbortCallback(hcanfd);
#else
        HAL_CANFD_AbortCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_AIF);
    }
    if(int_flag & CANFD_FLAG_EIF)
    {
        tempreg = hcanfd->Instance->TECNT_RECNT_TDC_EALCAP.value;
        hcanfd->ErrorCode = ((union _CANFD_RegB0 *)&tempreg)->bits.EALCAP >> 5;
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->ErrorCallback(hcanfd);
#else
        HAL_CANFD_ErrorCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_EIF);
    }
    if(int_flag & CANFD_FLAG_TSIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->TxSecCpltCallback(hcanfd);
#else
        HAL_CANFD_TxSecClptCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_TSIF);
    }
    if(int_flag & CANFD_FLAG_TPIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->TxPriCpltCallback(hcanfd);
#else
        HAL_CANFD_TxPriClptCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_TPIF);
    }
    if(int_flag & CANFD_FLAG_RAFIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->RxBuffAlmostFullCallback(hcanfd);
#else
        HAL_CANFD_RxBuffAlmostFullCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_RAFIF);
    }
    if(int_flag & CANFD_FLAG_RFIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->RxBuffFullCallback(hcanfd);
#else
        HAL_CANFD_RxBuffFullCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_RFIF);
    }
    if(int_flag & CANFD_FLAG_ROIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->RxBuffOverrunCallback(hcanfd);
#else
        HAL_CANFD_RxBuffOverrunCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_ROIF);
    }
    if(int_flag & CANFD_FLAG_RIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->RxMsgCallback(hcanfd);
#else
        HAL_CANFD_RxMsgCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_RIF);
    }
    if(int_flag & CANFD_FLAG_EPIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->PassiveErrorCallback(hcanfd);
#else
        HAL_CANFD_PassiveErrorCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_EPIF);
    }
    if(int_flag & CANFD_FLAG_ALIF)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->ArbitrationLostCallback(hcanfd);
#else
        HAL_CANFD_ArbitrationLostCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_ALIF);
    }
    if(int_flag & CANFD_FLAG_BEIF)
    {
        tempreg = hcanfd->Instance->TECNT_RECNT_TDC_EALCAP.value;
        hcanfd->ErrorCode = ((union _CANFD_RegB0 *)&tempreg)->bits.EALCAP >> 5;
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        hcanfd->BusErrorCallback(hcanfd);
#else
        HAL_CANFD_BusErrorCallback(hcanfd);
#endif
        HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_BEIF);
    }
//    tempreg = READ_REG(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value);
//    if(tempreg & CANFD_TSSTAT)//STB is not empty
//    {
//        /* Request transmission */
//        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TSONE);//start transmit from STB
//    }
}
__WEAK void HAL_CANFD_RxMsgCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_RxBuffOverrunCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_RxBuffFullCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_RxBuffAlmostFullCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_TxPriClptCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_TxSecClptCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_ErrorCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_AbortCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_PassiveErrorCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_ArbitrationLostCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
__WEAK void HAL_CANFD_BusErrorCallback(CANFD_HandleTypeDef *hcanfd)
{
    UNUSED(hcanfd);
}
/**
  * @brief  Configures the CANFD reception filter according to the specified
  *         parameters in the CAN_FilterInitStruct.
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  sFilterConfig pointer to a CANFD_FilterTypeDef structure that
  *         contains the filter configuration information.
  * @retval None
  */
HAL_StatusTypeDef HAL_CANFD_ConfigFilter(CANFD_HandleTypeDef *hcanfd, CANFD_FilterTypeDef *sFilterConfig)
{
    assert_param(hcanfd != NULL);
    assert_param(IS_CANFD_ALL_INSTANCE(hcanfd->Instance));
    assert_param(sFilterConfig != NULL);
    assert_param(IS_CANFD_FILTER_MODE(sFilterConfig->FilterMode));
    assert_param(IS_CANFD_FILTER_ID(sFilterConfig->FilterId));
    assert_param(IS_CANFD_FILTER_ID(sFilterConfig->FilterMaskId));
    assert_param(IS_CANFD_FILTER_ACRAMR(sFilterConfig->FilterBank));
    
    if(sFilterConfig->FilterBank == CANFD_ACRAMR_NOCARE)
    {
        for (int i = 0; i < CANFD_ACRAMR_NOCARE; ++i) {
            if((hcanfd->Instance->ACFEN10_TIMECFG_ACFCTRL & (0x1 << (16+i))) == 0)
            {
                sFilterConfig->FilterBank = i;
                break;
            }
        }
        if(sFilterConfig->FilterBank == CANFD_ACRAMR_NOCARE)
        {
            return HAL_ERROR;
        }
    }
    //enable filter number filter
    SET_BIT(hcanfd->Instance->ACFEN10_TIMECFG_ACFCTRL, ((0x1<<16) << sFilterConfig->FilterBank));
    //set ACFADR
    MODIFY_REG(hcanfd->Instance->ACFEN10_TIMECFG_ACFCTRL, CANFD_ACFADR, sFilterConfig->FilterBank);
    //Registers ACF_x point to acceptance code
    CLEAR_BIT(hcanfd->Instance->ACFEN10_TIMECFG_ACFCTRL, CANFD_SELMASK);
    CANFD_EnterResetMode(hcanfd);
    //Set ACFR
    hcanfd->Instance->ACFx = sFilterConfig->FilterId;
    CANFD_QuitResetMode(hcanfd);


    if(sFilterConfig->FilterMode == CANFD_Filter_STD_EXT)//accept both standard and extend frame
    {
        CLEAR_BIT(sFilterConfig->FilterMaskId, CANFD_AIDEE);
    }
    else
    {
        SET_BIT(sFilterConfig->FilterMaskId, CANFD_AIDEE);
        if(sFilterConfig->FilterMode == CANFD_Filter_STD)
        {//accept only stadand frame
            CLEAR_BIT(sFilterConfig->FilterMaskId, CANFD_AIDE);
        }
        else
        {//accept only extend frame
            SET_BIT(sFilterConfig->FilterMaskId, CANFD_AIDE);
        }
    }
    //Registers ACF_x point to acceptance code
    SET_BIT(hcanfd->Instance->ACFEN10_TIMECFG_ACFCTRL, CANFD_SELMASK);
    CANFD_EnterResetMode(hcanfd);
    //Set AMFR
    hcanfd->Instance->AMFx = sFilterConfig->FilterMaskId;
    CANFD_QuitResetMode(hcanfd);
    return HAL_OK;
}
HAL_StatusTypeDef HAL_CANFD_DisableFilter(CANFD_HandleTypeDef *hcanfd, CANFD_FilterTypeDef *sFilterConfig)
{
    assert_param(hcanfd != NULL);
    assert_param(IS_CANFD_ALL_INSTANCE(hcanfd->Instance));
    assert_param(sFilterConfig != NULL);
    assert_param(IS_CANFD_FILTER_ACRAMR(sFilterConfig->FilterBank));

    //disable filter number filter
    CLEAR_BIT(hcanfd->Instance->ACFEN10_TIMECFG_ACFCTRL, ((0x1<<16) << sFilterConfig->FilterBank));
    return HAL_OK;
}
/**
  * @brief  Fills each CANFD_InitStruct member with its default value.
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval status
  */
HAL_StatusTypeDef HAL_CANFD_StructInit(CANFD_HandleTypeDef *hcanfd)
{
    assert_param(hcanfd != NULL);
    /*set system clock prescaler to get f_clock 40MHz*/
    hcanfd->Init.Mode = CANFD_Mode_Normal;
    hcanfd->Init.ISO_En = CANFD_DISABLE;
    hcanfd->Init.Prescaler = CANFD_PRESCALER_2;
    hcanfd->Init.Baudrate_slow = CANFD_BAUDRATE_250kbps;
    hcanfd->Init.Baudrate_fast = CANFD_BAUDRATE_2mbps;
    return HAL_OK;
}

/**
  * @brief  Initializes the CANFD peripheral registers to their default reset values.
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval status.
  */
HAL_StatusTypeDef HAL_CANFD_Init(CANFD_HandleTypeDef *hcanfd)
{
    assert_param(hcanfd != NULL);
    assert_param(IS_CANFD_ALL_INSTANCE(hcanfd->Instance));
    assert_param(IS_CANFD_MODE(hcanfd->Init.Mode));
    assert_param(IS_CANFD_EN(hcanfd->Init.ISO_En));
    assert_param(IS_CANFD_PRESCALER(hcanfd->Init.Prescaler));
    assert_param(IS_CANFD_BAUDRATE_SLOW(hcanfd->Init.Baudrate_slow));
    assert_param(IS_CANFD_BAUDRATE_HIGH(hcanfd->Init.Baudrate_fast));

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_CHECK_PERI_LOCK((uint32_t)(hcanfd->Instance));
#endif
    if (hcanfd->State == HAL_CANFD_STATE_RESET)
    {
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
        CANFD_InitCallbacksToDefault(hcanfd);
        if (hcanfd->MspInitCallback == NULL)
        {
            hcanfd->MspInitCallback = HAL_CANFD_MspInit;
        }
        /* Init the low level hardware: CLOCK */
        hcanfd->MspInitCallback(hcanfd);
#else
        /* Init the low level hardware: CLOCK */
        HAL_CANFD_MspInit(hcanfd);
#endif
    }
    HAL_CANFD_SetBaudrateAuto(hcanfd, hcanfd->Init.Baudrate_slow, hcanfd->Init.Baudrate_fast);

	CANFD_EnterResetMode(hcanfd);
	if(hcanfd->Init.ISO_En == CANFD_ENABLE)
	{
	    /*ISO format or non-ISO format, only writeable if RESET=1.*/
	    __HAL_CANFD_ISO_MODE_ENABLE(hcanfd);
	}
	else
	{
	    __HAL_CANFD_ISO_MODE_DISABLE(hcanfd);
    }
    CANFD_QuitResetMode(hcanfd);

	/*disable Interrupt*/
	HAL_CANFD_IT_Disable(hcanfd, CANFD_IT_ALL);
	if(hcanfd->Init.Mode & CANFD_Mode_LoopbackInternal)
	{
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBMI);
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBME | CANFD_SACK);
	}
	else if(hcanfd->Init.Mode & CANFD_Mode_LoopbackExternal)
	{
	    SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBME | CANFD_SACK);
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBMI);
	}
	else
	{
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBMI | CANFD_LBME | CANFD_SACK);
    }
	if(hcanfd->Init.Mode & CANFD_Mode_Listen_Only)
	{
	    SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LOM);
	}
	else
	{
	    CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LOM);
    }

	CLEAR_BIT(hcanfd->Instance->ACFEN10_TIMECFG_ACFCTRL, (0xFF<<16));
//    SET_BIT(hcanfd->Instance->TTCFG_TBSLOT_VER1_VER0, CANFD_TTEN);
//    SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TTTBM);
    CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TSMODE);
    /* Initialize the error code */
	hcanfd->ErrorCode = HAL_CANFD_ERROR_NONE;
    /* Initialize the CANFD state */
    hcanfd->State = HAL_CANFD_STATE_READY;

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_SET_PERI_LOCK((uint32_t)(hcanfd->Instance), PERIPHERAL_INITED);
#endif
    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Deinitializes the CANFD peripheral registers to their default reset values.
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval status.
  */
HAL_StatusTypeDef HAL_CANFD_DeInit(CANFD_HandleTypeDef *hcanfd)
{
    assert_param(hcanfd != NULL);
    assert_param(IS_CANFD_ALL_INSTANCE(hcanfd->Instance));
    
    CANFD_EnterResetMode(hcanfd);
    HAL_CANFD_MspDeInit(hcanfd);
    /* Return function status */
    return HAL_OK;
}
HAL_StatusTypeDef HAL_CANFD_SetBaudrateAuto(CANFD_HandleTypeDef *hcanfd, CANFD_Baudrate_Enum slow, CANFD_Baudrate_Enum fast)
{
    hcanfd->Init.Baudrate_slow = slow;
    hcanfd->Init.Baudrate_fast = fast;
    CANFD_SetBaudrateParamSlow(hcanfd);
    CANFD_SetBaudrateParamFast(hcanfd);
    return HAL_CANFD_SetBaudrate(hcanfd, &(hcanfd->Init.baudrate));
}
/**
  * @brief  Set CANFD Baudrate.
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  baudrate param of baudrate.
  * @note   use for 5K/10K/20K/50K/100K/125K
  *         TQ = div/f_clock
  *         Baudrate = f_clock/div/(tSeg_1 + tSeg_2) 
  *         tSeg_1 [2…65] TQ                     CAN 2.0 bit rate (slow)
  *                [2…65] TQ                     CAN FD nominal bit rate (slow)
  *                [2…17] TQ                     CAN FD data bit rate (fast)
  *         tSeg_2 [1…8]  TQ tSeg_1 >= tSeg_2+2  CAN 2.0 bit rate (slow)
  *                [1…32] TQ tSeg_1 >= tSeg_2+2  CAN FD nominal bit rate (slow)
  *                [1…8]  TQ tSeg_1 >= tSeg_2+1  CAN FD data bit rate (fast)
  *         tSJW   [1…16] TQ tSeg_2 >= tSJW      CAN 2.0 bit rate (slow)
  *                [1…16] TQ tSeg_2 >= tSJW      CAN FD nominal bit rate (slow)
  *                [1…8]  TQ tSeg_2 >= tSJW      CAN FD data bit rate (fast)
  *         it is suggested to set tSeg_1 equal to SSPOFF.
  *         ISO 11898-1:2015 requires to use only F_PRESC(div_fast)=0 or 1 if that TDC is used. 
  *         With this requirement CANCTRL is capable of automatically determining a transmitter delay of up to 3 bit times of the fast data bit rate.
  * @retval status.
  */
HAL_StatusTypeDef HAL_CANFD_SetBaudrate(CANFD_HandleTypeDef *hcanfd, const CANFD_BaudrateTypeDef *baudrate)
{
    assert_param(hcanfd != NULL);
    assert_param(baudrate != NULL);
    assert_param(baudrate->tSeg_1_slow >= 2);
    assert_param(baudrate->tSeg_2_slow >= 1);
    assert_param(baudrate->tSeg_1_fast >= 2);
    assert_param(baudrate->tSeg_2_fast >= 1);
    assert_param(baudrate->tSJW_slow >= 1);
    assert_param(baudrate->tSJW_fast >= 1);
    assert_param(baudrate->div_slow >= 1);
    assert_param(baudrate->div_fast >= 1);
    
	CANFD_EnterResetMode(hcanfd);
    CANFD_SetBitTimingSlow(hcanfd, baudrate->tSeg_1_slow-2, baudrate->tSeg_2_slow-1, baudrate->tSJW_slow-1);
    CANFD_SetPrescalerSlow(hcanfd, baudrate->div_slow-1);  

    CANFD_SetBitTimingFast(hcanfd, baudrate->tSeg_1_fast-2, baudrate->tSeg_2_fast-1, baudrate->tSJW_fast-1);
    CANFD_SetPrescalerFast(hcanfd, baudrate->div_fast-1);
    CANFD_TDC_config(hcanfd, baudrate->ssp_offset != 0 ? ENABLE : DISABLE, baudrate->ssp_offset);
    CANFD_QuitResetMode(hcanfd);
    return HAL_OK;
}
/**
  * @brief  Initializes the CANFD MSP.
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval None
  */
__WEAK void HAL_CANFD_MspInit(CANFD_HandleTypeDef *hcanfd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcanfd);
}
/**
  * @brief  DeInitializes the CANFD MSP.
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval None
  */
__WEAK void HAL_CANFD_MspDeInit(CANFD_HandleTypeDef *hcanfd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcanfd);
}
/**
  * @brief  CANFD set slow mode timing params by a giving baudrate
  * @param  hcanfd: where x can be CANFD0 or CANFD1 to select the CANFD peripheral.
  * @param  Baudrate:slow mode baudrate
  * @retval None.
  */
static void CANFD_SetBaudrateParamSlow(CANFD_HandleTypeDef *hcanfd)
{
    uint32_t baudrate = 0;
    uint32_t temp = 0;
    uint32_t f_clock = 0;
    uint8_t tSeg_1 = 0;
    uint8_t tSeg_2 = 0;
    uint8_t tSJW = 0;
    uint8_t div = 0;
    uint8_t ssp_offset = 0;
    f_clock = HAL_RCC_GetCANFDCLKFreq(hcanfd->Init.Prescaler);
    assert_param(IS_CANFD_CLK(f_clock));
    /* Baudrate = f_clock/div/(tSeg_1 + tSeg_2) 
     * TQ = div/f_clock
     * tSeg_1 [2…65] TQ                     CAN 2.0 bit rate (slow)
     *        [2…65] TQ                     CAN FD nominal bit rate (slow)
     *        [2…17] TQ                     CAN FD data bit rate (fast)
     * tSeg_2 [1…8]  TQ tSeg_1 >= tSeg_2+2  CAN 2.0 bit rate (slow)
     *        [1…32] TQ tSeg_1 >= tSeg_2+2  CAN FD nominal bit rate (slow)
     *        [1…8]  TQ tSeg_1 >= tSeg_2+1  CAN FD data bit rate (fast)
     * tSJW   [1…16] TQ tSeg_2 >= tSJW      CAN 2.0 bit rate (slow)
     *        [1…16] TQ tSeg_2 >= tSJW      CAN FD nominal bit rate (slow)
     *        [1…8]  TQ tSeg_2 >= tSJW      CAN FD data bit rate (fast)
     *        it is suggested to set tSeg_1 equal to SSPOFF.
     *        ISO 11898-1:2015 requires to use only F_PRESC(div_fast)=0 or 1 if that TDC is used. 
     *        With this requirement CANCTRL is capable of automatically determining a transmitter delay of up to 3 bit times of the fast data bit rate.
     * */
    
    if(f_clock == 20000000)
    {
        div = 1;
        switch(hcanfd->Init.Baudrate_slow)
        {
        case CANFD_BAUDRATE_125kbps:
            div = 2;
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_250kbps:
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_500kbps:
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            break;
        case CANFD_BAUDRATE_833kbps:
            tSeg_1 = 19;
            tSeg_2 = 5;
            tSJW = 5;
            break;
        case CANFD_BAUDRATE_1mbps:
            tSeg_1 = 16;
            tSeg_2 = 4;
            tSJW = 4;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    else if(f_clock == 40000000)
    {
        switch(hcanfd->Init.Baudrate_slow)
        {
        case CANFD_BAUDRATE_125kbps:
            div = 4;
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_250kbps:
            div = 2;
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_500kbps:
            div = 1;
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_833kbps:
            div = 2;
            tSeg_1 = 19;
            tSeg_2 = 5;
            tSJW = 5;
            break;
        case CANFD_BAUDRATE_1mbps:
            div = 1;
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    else if(f_clock == 48000000)
    {
        switch(hcanfd->Init.Baudrate_slow)
        {
        case CANFD_BAUDRATE_125kbps:
            div = 6;
            tSeg_1 = 48;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_250kbps:
            div = 3;
            tSeg_1 = 48;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_500kbps:
            div = 3;
            tSeg_1 = 24;
            tSeg_2 = 8;
            tSJW = 8;
            break;
        case CANFD_BAUDRATE_833kbps:
            div = 2;
            tSeg_1 = 22;
            tSeg_2 = 6;
            tSJW = 6;
            break;
        case CANFD_BAUDRATE_1mbps:
            div = 1;
            tSeg_1 = 36;
            tSeg_2 = 12;
            tSJW = 12;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    else if(f_clock == 80000000)
    {
        switch(hcanfd->Init.Baudrate_slow)
        {
        case CANFD_BAUDRATE_125kbps:
            div = 8;
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_250kbps:
            div = 4;
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_500kbps:
            div = 2;
            tSeg_1 = 64;
            tSeg_2 = 16;
            tSJW = 16;
            break;
        case CANFD_BAUDRATE_833kbps:
            div = 4;
            tSeg_1 = 19;
            tSeg_2 = 5;
            tSJW = 5;
            break;
        case CANFD_BAUDRATE_1mbps:
            div = 2;
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    hcanfd->Init.baudrate.tSeg_1_slow = tSeg_1;
    hcanfd->Init.baudrate.tSeg_2_slow = tSeg_2;
    hcanfd->Init.baudrate.tSJW_slow = tSJW;
    hcanfd->Init.baudrate.div_slow = div;
}

/**
  * @brief  CANFD set fast mode baudrate
  * @param  hcanfd CANFD handle.
  * @retval None.
  */
static void CANFD_SetBaudrateParamFast(CANFD_HandleTypeDef *hcanfd)
{
    uint32_t pll = 0;
    uint32_t baudrate = 0;
    uint32_t f_clock = 0;
    uint8_t tSeg_1 = 0;
    uint8_t tSeg_2 = 0;
    uint8_t tSJW = 0;
    uint8_t div = 0;
    uint8_t ssp_offset = 0;
    f_clock = HAL_RCC_GetCANFDCLKFreq(hcanfd->Init.Prescaler);
    assert_param(IS_CANFD_CLK(f_clock));
    /* Baudrate = f_clock/div/(tSeg_1 + tSeg_2) 
     * TQ = div/f_clock
     * tSeg_1 [2…65] TQ                     CAN 2.0 bit rate (slow)
     *        [2…65] TQ                     CAN FD nominal bit rate (slow)
     *        [2…17] TQ                     CAN FD data bit rate (fast)
     * tSeg_2 [1…8]  TQ tSeg_1 >= tSeg_2+2  CAN 2.0 bit rate (slow)
     *        [1…32] TQ tSeg_1 >= tSeg_2+2  CAN FD nominal bit rate (slow)
     *        [1…8]  TQ tSeg_1 >= tSeg_2+1  CAN FD data bit rate (fast)
     * tSJW   [1…16] TQ tSeg_2 >= tSJW      CAN 2.0 bit rate (slow)
     *        [1…16] TQ tSeg_2 >= tSJW      CAN FD nominal bit rate (slow)
     *        [1…8]  TQ tSeg_2 >= tSJW      CAN FD data bit rate (fast)
     *        it is suggested to set tSeg_1 equal to SSPOFF.
     *        ISO 11898-1:2015 requires to use only F_PRESC(div_fast)=0 or 1 if that TDC is used. 
     *        With this requirement CANCTRL is capable of automatically determining a transmitter delay of up to 3 bit times of the fast data bit rate.
     * */
    if(f_clock == 20000000)
    {
        div = 1;
        switch(hcanfd->Init.Baudrate_fast)
        {
        case CANFD_BAUDRATE_500kbps:
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_833kbps:
            tSeg_1 = 19;
            tSeg_2 = 5;
            tSJW = 5;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_1mbps:
            tSeg_1 = 16;
            tSeg_2 = 4;
            tSJW = 4;
            ssp_offset = 16;
            break;
        case CANFD_BAUDRATE_1538kbps:
            tSeg_1 = 10;
            tSeg_2 = 3;
            tSJW = 3;
            ssp_offset = 10;
            break;
        case CANFD_BAUDRATE_2mbps:
            tSeg_1 = 8;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 8;
            break;
        case CANFD_BAUDRATE_4mbps:
            tSeg_1 = 4;
            tSeg_2 = 1;
            tSJW = 1;
            ssp_offset = 4;
            break;
        case CANFD_BAUDRATE_5mbps:
            tSeg_1 = 3;
            tSeg_2 = 1;
            tSJW = 1;
            ssp_offset = 3;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    else if(f_clock == 40000000)
    {
        switch(hcanfd->Init.Baudrate_fast)
        {
        case CANFD_BAUDRATE_500kbps:
            div = 2;
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_833kbps:
            div = 2;
            tSeg_1 = 19;
            tSeg_2 = 5;
            tSJW = 5;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_1mbps:
            div = 1;
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            ssp_offset = 32;
            break;
        case CANFD_BAUDRATE_1538kbps:
            div = 1;
            tSeg_1 = 20;
            tSeg_2 = 6;
            tSJW = 6;
            ssp_offset = 20;
            break;
        case CANFD_BAUDRATE_2mbps:
            div = 1;
            tSeg_1 = 16;
            tSeg_2 = 4;
            tSJW = 4;
            ssp_offset = 16;
            break;
        case CANFD_BAUDRATE_3077kbps:
            div = 1;
            tSeg_1 = 10;
            tSeg_2 = 3;
            tSJW = 3;
            ssp_offset = 10;
            break;
        case CANFD_BAUDRATE_4mbps:
            div = 1;
            tSeg_1 = 8;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 8;
            break;
        case CANFD_BAUDRATE_5mbps:
            div = 1;
            tSeg_1 = 6;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 6;
            break;
        case CANFD_BAUDRATE_6667kbps:
            div = 1;
            tSeg_1 = 5;
            tSeg_2 = 1;
            tSJW = 1;
            ssp_offset = 5;
            break;
        case CANFD_BAUDRATE_8mbps:
            div = 1;
            tSeg_1 = 4;
            tSeg_2 = 1;
            tSJW = 1;
            ssp_offset = 4;
            break;
        case CANFD_BAUDRATE_10mbps:
            div = 1;
            tSeg_1 = 3;
            tSeg_2 = 1;
            tSJW = 1;
            ssp_offset = 3;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    else if(f_clock == 48000000)
    {
        switch(hcanfd->Init.Baudrate_fast)
        {
        case CANFD_BAUDRATE_500kbps:
            div = 2;
            tSeg_1 = 38;
            tSeg_2 = 10;
            tSJW = 10;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_833kbps:
            div = 2;
            tSeg_1 = 22;
            tSeg_2 = 7;
            tSJW = 7;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_1mbps:
            div = 1;
            tSeg_1 = 38;
            tSeg_2 = 10;
            tSJW = 10;
            ssp_offset = 38;
            break;
        case CANFD_BAUDRATE_1538kbps:
            div = 1;
            tSeg_1 = 23;
            tSeg_2 = 8;
            tSJW = 8;
            ssp_offset = 23;
            break;
        case CANFD_BAUDRATE_2mbps:
            div = 1;
            tSeg_1 = 19;
            tSeg_2 = 5;
            tSJW = 5;
            ssp_offset = 19;
            break;
        case CANFD_BAUDRATE_3077kbps:
            div = 1;
            tSeg_1 = 12;
            tSeg_2 = 4;
            tSJW = 4;
            ssp_offset = 12;
            break;
        case CANFD_BAUDRATE_4mbps:
            div = 1;
            tSeg_1 = 9;
            tSeg_2 = 3;
            tSJW = 3;
            ssp_offset = 9;
            break;
        case CANFD_BAUDRATE_5mbps:
            div = 1;
            tSeg_1 = 8;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 8;
            break;
        case CANFD_BAUDRATE_6667kbps:
            div = 1;
            tSeg_1 = 5;
            tSeg_2 = 1;
            tSJW = 1;
            ssp_offset = 5;
            break;
        case CANFD_BAUDRATE_8mbps:
            div = 1;
            tSeg_1 = 5;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 5;
            break;
        case CANFD_BAUDRATE_10mbps:
            div = 1;
            tSeg_1 = 3;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 3;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    else if(f_clock == 80000000)
    {
        switch(hcanfd->Init.Baudrate_fast)
        {
        case CANFD_BAUDRATE_500kbps:
            div = 4;
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_833kbps:
            div = 4;
            tSeg_1 = 19;
            tSeg_2 = 5;
            tSJW = 5;
            ssp_offset = 0;
            break;
        case CANFD_BAUDRATE_1mbps:
            div = 2;
            tSeg_1 = 32;
            tSeg_2 = 8;
            tSJW = 8;
            ssp_offset = 64;
            break;
        case CANFD_BAUDRATE_1538kbps:
            div = 2;
            tSeg_1 = 20;
            tSeg_2 = 6;
            tSJW = 6;
            ssp_offset = 40;
            break;
        case CANFD_BAUDRATE_2mbps:
            div = 2;
            tSeg_1 = 16;
            tSeg_2 = 4;
            tSJW = 4;
            ssp_offset = 32;
            break;
        case CANFD_BAUDRATE_3077kbps:
            div = 2;
            tSeg_1 = 10;
            tSeg_2 = 3;
            tSJW = 3;
            ssp_offset = 20;
            break;
        case CANFD_BAUDRATE_4mbps:
            div = 1;
            tSeg_1 = 16;
            tSeg_2 = 4;
            tSJW = 4;
            ssp_offset = 16;
            break;
        case CANFD_BAUDRATE_5mbps:
            div = 1;
            tSeg_1 = 12;
            tSeg_2 = 4;
            tSJW = 4;
            ssp_offset = 12;
            break;
        case CANFD_BAUDRATE_6667kbps:
            div = 1;
            tSeg_1 = 10;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 10;
            break;
        case CANFD_BAUDRATE_8mbps:
            div = 1;
            tSeg_1 = 8;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 8;
            break;
        case CANFD_BAUDRATE_10mbps:
            div = 1;
            tSeg_1 = 6;
            tSeg_2 = 2;
            tSJW = 2;
            ssp_offset = 6;
            break;
        default:
            assert_param(0);
            break;
        }
    }
    hcanfd->Init.baudrate.tSeg_1_fast = tSeg_1;
    hcanfd->Init.baudrate.tSeg_2_fast = tSeg_2;
    hcanfd->Init.baudrate.tSJW_fast = tSJW;
    hcanfd->Init.baudrate.div_fast = div;
}

static volatile uint32_t s_CFG_STAT;
/**
  * @brief  enter CANFD Reset Mode
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval None.
  */
static void CANFD_EnterResetMode(CANFD_HandleTypeDef *hcanfd)
{
    //Save these bit value in s_CFG_STAT, Wait until quit reset mode them write to RCTRL_TCTRL_TCMD_CFGSTAT.value;
    s_CFG_STAT = hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value;
    SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_RESET);
    while((hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_RESET)!=CANFD_RESET);
}
/**
  * @brief  quit CANFD Reset Mode
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @retval None.
  */
static void CANFD_QuitResetMode(CANFD_HandleTypeDef *hcanfd)
{
    CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_RESET);
    while((hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_RESET) == CANFD_RESET);
    //write the saved value to RCTRL_TCTRL_TCMD_CFGSTAT.value;
    CLEAR_BIT(s_CFG_STAT, CANFD_RESET);
    hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value |= s_CFG_STAT;
}
/**
  * @brief  set CANFD Bit Timing Register for slow speed
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  seg_1:  The sample point will be set to tSeg_1 = (seg_1 + 2)*TQ after start of bit time.
  * @param  seg_2:  Time tSeg_2 = (seg_2 + 1)*TQ after the sample point.
  * @param  SJW:  The Synchronization Jump Width tSJW = (SJW+1)*TQ is the maximum time for 
  * @param  SJW:  shortening or lengthening the Bit Time for resynchronization, where TQ is a time quanta.
  * @retval none
  */
static void CANFD_SetBitTimingSlow(CANFD_HandleTypeDef *hcanfd, uint8_t seg_1, uint8_t seg_2, uint8_t SJW)
{
    if(seg_2 <= 127 && SJW <= 127)
    {
        uint32_t tmpreg = hcanfd->Instance->S_PRESC_SJW_Seg2_Seg1 & 0xff000000;
        tmpreg |= seg_1;
        tmpreg |= (seg_2 << 8);
        tmpreg |= (SJW << 16);
        hcanfd->Instance->S_PRESC_SJW_Seg2_Seg1 = tmpreg;
    }
}
/**
  * @brief  set CANFD Bit Timing Register for fast speed
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  seg_1:  The sample point will be set to tSeg_1 = (seg_1 + 2)*TQ after start of bit time.
  * @param  seg_2:  Time tSeg_2 = (seg_2 + 1)*TQ after the sample point.
  * @param  SJW:  The Synchronization Jump Width tSJW = (SJW+1)*TQ is the maximum time for 
  * @param  SJW:  shortening or lengthening the Bit Time for resynchronization, where TQ is a time quanta.
  * @retval none
  */
static void CANFD_SetBitTimingFast(CANFD_HandleTypeDef *hcanfd, uint8_t seg_1, uint8_t seg_2, uint8_t SJW)
{
    if(seg_2 <= 127 && SJW <= 127)
    {
        uint32_t tmpreg = hcanfd->Instance->F_PRESC_SJW_Seg2_Seg1 & 0xff000000;
        tmpreg |= seg_1;
        tmpreg |= (seg_2 << 8);
        tmpreg |= (SJW << 16);
        hcanfd->Instance->F_PRESC_SJW_Seg2_Seg1 = tmpreg;
    }
}
/**
  * @brief  set CANFD Prescaler Register for slow speed 
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  prescaler:  [0~0xFF] results in divider values 1 to 256.
  * @retval none
  */
static void CANFD_SetPrescalerSlow(CANFD_HandleTypeDef *hcanfd, uint8_t prescaler)
{
    uint32_t tmpreg = hcanfd->Instance->S_PRESC_SJW_Seg2_Seg1 & 0x00FFFFFF;
    tmpreg |= (prescaler << 24);
    hcanfd->Instance->S_PRESC_SJW_Seg2_Seg1 = tmpreg;
}
/**
  * @brief  set CANFD Prescaler Register for fast speed 
  * @param  hcanfd pointer to a CANFD_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  prescaler:  results in divider values 1 to 256.
  * @retval none
  */
static void CANFD_SetPrescalerFast(CANFD_HandleTypeDef *hcanfd, uint8_t prescaler)
{
    uint32_t tmpreg = hcanfd->Instance->F_PRESC_SJW_Seg2_Seg1 & 0x00FFFFFF;
    tmpreg |= (prescaler << 24);
    hcanfd->Instance->F_PRESC_SJW_Seg2_Seg1 = tmpreg;
}

#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_CANFD_RegisterCallback(CANFD_HandleTypeDef *hcanfd, HAL_CANFD_CallbackIDTypeDef CallbackID, pCANFD_CallbackTypeDef pCallback)
{
    assert_param(hcanfd != NULL);
    HAL_StatusTypeDef status = HAL_OK;

    if(pCallback == NULL)
    {
        /* Update the error code */
        hcanfd->ErrorCode |= HAL_CANFD_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }
    if(hcanfd->State == HAL_CANFD_STATE_READY)
    {
        switch (CallbackID)
        {
        case HAL_CANFD_ABORT_CB_ID:
            hcanfd->AbortCallback = pCallback;
        break;
        case HAL_CANFD_ERROR_CB_ID:
            hcanfd->ErrorCallback = pCallback;
        break;
        case HAL_CANFD_TX_SEC_COMPLETE_CB_ID:
            hcanfd->TxSecCpltCallback = pCallback;
        break;
        case HAL_CANFD_TX_PRI_COMPLETE_CB_ID:
            hcanfd->TxPriCpltCallback = pCallback;
        break;
        case HAL_CANFD_RX_BUFF_ALMOST_FULL_CB_ID:
            hcanfd->RxBuffAlmostFullCallback = pCallback;
        break;
        case HAL_CANFD_RX_BUFF_FULL_CB_ID:
            hcanfd->RxBuffFullCallback = pCallback;
        break;
        case HAL_CANFD_RX_OVERRUN_CB_ID:
            hcanfd->RxBuffOverrunCallback = pCallback;
        break;
        case HAL_CANFD_RX_MSG_CB_ID:
            hcanfd->RxMsgCallback = pCallback;
        break;
        case HAL_CANFD_PASSIVE_ERROR_CB_ID:
            hcanfd->PassiveErrorCallback = pCallback;
        break;
        case HAL_CANFD_ARB_LOST_CB_ID:
            hcanfd->ArbitrationLostCallback = pCallback;
        break;
        case HAL_CANFD_BUS_ERROR_CB_ID:
            hcanfd->BusErrorCallback = pCallback;
        break;
        default:
            /* Update the error code */
            hcanfd->ErrorCode |= HAL_CANFD_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else if (hcanfd->State == HAL_CANFD_STATE_RESET)
    {
        switch (CallbackID)
        {
            case HAL_CANFD_MSPINIT_CB_ID :
                hcanfd->MspInitCallback = pCallback;
            break;
            case HAL_CANFD_MSPDEINIT_CB_ID :
                hcanfd->MspDeInitCallback = pCallback;
            break;
            default :
                /* Update the error code */
                hcanfd->ErrorCode |= HAL_CANFD_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Update the error code */
        hcanfd->ErrorCode |= HAL_CANFD_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hcanfd);
    return status;
}
HAL_StatusTypeDef HAL_CANFD_UnRegisterCallback(CANFD_HandleTypeDef *hcanfd, HAL_CANFD_CallbackIDTypeDef CallbackID)
{
    assert_param(hcanfd != NULL);
    HAL_StatusTypeDef status = HAL_OK;
    
    /* Process locked */
    __HAL_LOCK(hcanfd);
    if(hcanfd->State == HAL_CANFD_STATE_READY)
    {
        switch (CallbackID)
        {
        case HAL_CANFD_ABORT_CB_ID:
            hcanfd->AbortCallback = HAL_CANFD_AbortCallback;
        break;
        case HAL_CANFD_ERROR_CB_ID:
            hcanfd->ErrorCallback = HAL_CANFD_ErrorCallback;
        break;
        case HAL_CANFD_TX_SEC_COMPLETE_CB_ID:
            hcanfd->TxSecCpltCallback = HAL_CANFD_TxSecClptCallback;
        break;
        case HAL_CANFD_TX_PRI_COMPLETE_CB_ID:
            hcanfd->TxPriCpltCallback = HAL_CANFD_TxPriClptCallback;
        break;
        case HAL_CANFD_RX_BUFF_ALMOST_FULL_CB_ID:
            hcanfd->RxBuffAlmostFullCallback = HAL_CANFD_RxBuffAlmostFullCallback;
        break;
        case HAL_CANFD_RX_BUFF_FULL_CB_ID:
            hcanfd->RxBuffFullCallback = HAL_CANFD_RxBuffFullCallback;
        break;
        case HAL_CANFD_RX_OVERRUN_CB_ID:
            hcanfd->RxBuffOverrunCallback = HAL_CANFD_RxBuffOverrunCallback;
        break;
        case HAL_CANFD_RX_MSG_CB_ID:
            hcanfd->RxMsgCallback = HAL_CANFD_RxMsgCallback;
        break;
        case HAL_CANFD_PASSIVE_ERROR_CB_ID:
            hcanfd->PassiveErrorCallback = HAL_CANFD_PassiveErrorCallback;
        break;
        case HAL_CANFD_ARB_LOST_CB_ID:
            hcanfd->ArbitrationLostCallback = HAL_CANFD_ArbitrationLostCallback;
        break;
        case HAL_CANFD_BUS_ERROR_CB_ID:
            hcanfd->BusErrorCallback = HAL_CANFD_BusErrorCallback;
        break;
        default: 
            /* Update the error code */
            hcanfd->ErrorCode |= HAL_CANFD_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else if (hcanfd->State == HAL_CANFD_STATE_RESET)
    {
        switch (CallbackID)
        {
            case HAL_CANFD_MSPINIT_CB_ID :
                hcanfd->MspInitCallback = HAL_CANFD_MspInit;
            break;
            case HAL_CANFD_MSPDEINIT_CB_ID :
                hcanfd->MspDeInitCallback = HAL_CANFD_MspDeInit;
            break;
            default :
                /* Update the error code */
                hcanfd->ErrorCode |= HAL_CANFD_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Update the error code */
        hcanfd->ErrorCode |= HAL_CANFD_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hcanfd);
    return status;
}
#endif /* USE_HAL_CANFD_REGISTER_CALLBACKS */

/**
  * @brief  set CANFD Error Warning Limit. 
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param EWL: (Error Warning Limit= (EWL+1)*8.)
  * @retval none
  */
void HAL_CANFD_SetErrorWarningLimit(CANFD_HandleTypeDef *hcanfd, uint8_t EWL)
{
    if(EWL <= 15)
    {
        uint32_t tmp = (hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE & CANFD_EWL) | (EWL << 24);
        hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE = tmp;
    }
}
/**                                                         
  * @brief  set CANFD receive buffer Almost Full Warning Limit
  * @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CANFD.
  * @param  AFWL:  receive buffer Almost Full Warning Limit
  * @retval none
  */
void HAL_CANFD_SetAlmostFullWarningLimit(CANFD_HandleTypeDef *hcanfd, uint8_t AFWL)
{
    if(AFWL <= 15)
    {
        uint32_t tmp = (hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE & CANFD_AFWL) | (AFWL << 28);
        hcanfd->Instance->LIMIT_ERRINT_RTIF_RTIE = tmp;
    }
}

void HAL_CANFD_SetMode(CANFD_HandleTypeDef *hcanfd, CANFD_Mode_Enum mode)
{
    hcanfd->Init.Mode = mode;
    switch(mode)
    {
    case CANFD_Mode_Normal:
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBMI | CANFD_LBME | CANFD_SACK);
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LOM);
        break;
    case CANFD_Mode_Listen_Only:
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LOM);
        break;
    case CANFD_Mode_LoopbackInternal:
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBMI);
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBME | CANFD_SACK);
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LOM);
        break;
    case CANFD_Mode_LOOPBACKANLISTEN:
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBMI);
        CLEAR_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LBME | CANFD_SACK);
        SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_LOM);
        break;
    default: break;
    }
}
void HAL_CANFD_SetISOMode(CANFD_HandleTypeDef *hcanfd, uint16_t en)
{
    CANFD_EnterResetMode(hcanfd);
    if(en == CANFD_ENABLE)
    {
        /*ISO format or non-ISO format, only writeable if RESET=1.*/
        __HAL_CANFD_ISO_MODE_ENABLE(hcanfd);
    }
    else
    {
        __HAL_CANFD_ISO_MODE_DISABLE(hcanfd);
    }
    CANFD_QuitResetMode(hcanfd);
}
#endif /* HAL_CANFD_MODULE_ENABLED */
