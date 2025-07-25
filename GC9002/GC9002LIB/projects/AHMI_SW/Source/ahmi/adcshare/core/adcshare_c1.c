/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : adcshare_c1.c
*
* Description  : This file contains the implementation of ADC SHARE interface
*
* Environment  : MDK / PC Simulation
*
* Responsible  : 
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2023-12-18
****************************************************************************/


/* Foreign headerfiles */
#include <stdio.h>
#include "csi_irq_vectors.h"
#include "csi_hal_core.h"
#include "csi_kernel.h"
#include "semphr.h"
#include "com_c1.h"


/* Own headerfiles */
#include "adcshare_c1.h"

static ADCSHR_tstWorkBuf ADCSHR_stWorkBuf;
/* Make sure ADC DMA buffer localled in iRAM address space */
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".iramdata"))) 
#endif
ADCSHR_tstChannelControl ADCSHR_astChannelControl[ADCSHR_CHANNEL_MAX_NUMBER];
static DMA_HandleTypeDef adc_hdma;
static ADC_HandleTypeDef hAdc;
static k_sem_handle_t xSemaphoreAdcPrio = NULL;

#if COM_COMPILE_SWITCH
extern UART_HandleTypeDef handle_uart;
#endif 

void ADCSHR__vHardwareInit(void);
void ADCSHR_DMA_ADC__vInit(void);
void ADCSHR_HAL__vConvertComplete(ADC_HandleTypeDef *hadc);
void ADCSHR_HAL__vErrorCallback(ADC_HandleTypeDef *hadc);
void ADCSHR__vCycleSampleStart(void);
void ADCSHR__vAdcIoInit(uint8_t channel);


/**
  * @name       DMA_IRQHandler
  * @brief      DMA IRQHandler function for ADC
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2023-11-06
  */
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".flashdriver"))) 
#endif
static void DMA_IRQHandler(void)
#else
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".flashdriver"))) 
#endif 
ATTRIBUTE_ISR void DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    
#if COM_COMPILE_SWITCH

    if (__HAL_DMA_CHX_GET_IT_SOURCE(hAdc.hdma, hAdc.hdma->Init.mChannel, DMA_INT_Tfr)
        && __HAL_DMA_CHX_GET_IT_STATUS(hAdc.hdma, hAdc.hdma->Init.mChannel, DMA_INT_Tfr))
    {
        __HAL_DMA_CHX_CLEAR_IT(hAdc.hdma, hAdc.hdma->Init.mChannel, DMA_INT_ALL);
        
		/* Process Unlocked */
		__HAL_UNLOCK(hAdc.hdma);
		/* Change the DMA state */
		hAdc.hdma->State = HAL_DMA_STATE_READY;

        if(hAdc.ConvCpltCallback)
            hAdc.ConvCpltCallback(&hAdc);
    }

    if (__HAL_DMA_CHX_GET_IT_SOURCE(handle_uart.hdmarx, handle_uart.hdmarx->Init.mChannel, DMA_INT_Tfr)
        && __HAL_DMA_CHX_GET_IT_STATUS(handle_uart.hdmarx, handle_uart.hdmarx->Init.mChannel, DMA_INT_Tfr))
    {
        //__HAL_DMA_CHX_CLEAR_IT(handle_uart.hdmarx, handle_uart.hdmarx->Init.mChannel, DMA_INT_Tfr);
        COM_UART_DMA_RxCpltCallback(handle_uart.hdmarx);
    }

#else
    HAL_DMA_IRQHandler(hAdc.hdma);
#endif

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


/**
  * @name       ADCSHR_DMA_ADC__vInit
  * @brief      Init DMA channel for ADC
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2023-12-19
  */
void ADCSHR_DMA_ADC__vInit(void)
{
    adc_hdma.Instance = DMA;
    HAL_DMA_StructInit(&(adc_hdma.Init));
	__HAL_LINKDMA(&hAdc, hdma, adc_hdma);
    adc_hdma.Init.mChannel = DMA_CHANNEL1;
    adc_hdma.Init.mS_Address = ((uint32_t)&(hAdc.Instance->DOUT));/*!< Specifies Source Address.             */
    adc_hdma.Init.mD_Address = 0;/*!< Specifies Destination Address.        */
    adc_hdma.Init.mBlockSize = 0;
    adc_hdma.Init.mS_AutoReload_EN = DMA_DISABLE;
    adc_hdma.Init.mD_AutoReload_EN = DMA_DISABLE;
    adc_hdma.Init.mS_TransferWidth = DMA_TR_WIDTH_16bits;
    adc_hdma.Init.mD_TransferWidth = DMA_TR_WIDTH_16bits;//16
    adc_hdma.Init.mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    adc_hdma.Init.mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    adc_hdma.Init.mS_AddrInc = DMA_FIXED;
    adc_hdma.Init.mD_AddrInc = DMA_INCREMENTAL;
    adc_hdma.Init.mChannelPriority = DMA_MIN_CHANNEL_PRIORITY;
    adc_hdma.Init.mTransferType_FlowControl = DMA_TT_PER_TO_MEM_DMAC;
    adc_hdma.Init.mS_Peripheral = DMA_HS_ADC;
    adc_hdma.Init.mD_Peripheral = DMA_HS_MEMORY;
    adc_hdma.Init.mEnable = DMA_DISABLE;
    HAL_DMA_Init(&adc_hdma);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(DMA_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 0, DMA_IRQHandler);
#else
    csi_clic_register_irq(DMA_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 0, DMA_IRQHandler);
#endif
}


/**
  * @name       ADCSHR_HAL__vConvertComplete
  * @brief      Callback function for ADC complete convert 
  * @note        
  * @param      hadc: pointer to ADC handle
  * @retval     none
  * @author     
  * @date       2023-11-06
  */
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".flashdriver"), optimize(0))) 
#endif
void ADCSHR_HAL__vConvertComplete(ADC_HandleTypeDef *hadc)
{
	/* Adjust Index */
	if(ADCSHR_stWorkBuf.u8ChannelIndex < ADCSHR_CHANNEL_MAX_NUMBER)
	{
		ADCSHR_stWorkBuf.pstChannelControl[ADCSHR_stWorkBuf.u8ChannelIndex].enState = ADCSHR_enChannelState_Done;
	}
    
    ADCSHR_stWorkBuf.u8ChannelIndex++;
    
    if(ADCSHR_stWorkBuf.u8ChannelIndex < ADCSHR_CHANNEL_MAX_NUMBER)
    {   /* start another channel sample */
        ADCSHR__vCycleSampleStart();
    }
    else
    {
        ADCSHR_stWorkBuf.u8ChannelIndex = 0;
        ADCSHR_stWorkBuf.enCycleCurSM = ADCSHR_enCycleSM_Idle;    
    }
}


/**
  * @name       ADCSHR_HAL__vErrorCallback
  * @brief      Callback function for ADC error 
  * @note        
  * @param      hadc: pointer to ADC handle
  * @retval     none
  * @author     
  * @date       2023-11-06
  */
void ADCSHR_HAL__vErrorCallback(ADC_HandleTypeDef *hadc)
{
    if(ADCSHR_enCycleSM_Idle != ADCSHR_stWorkBuf.enCycleCurSM)
    {
        /* Stop Cycle Sample */
        HAL_ADC_Stop_DMA(&hAdc);
    }
    ADCSHR__vHardwareInit();
    ADCSHR_stWorkBuf.enCycleCurSM = ADCSHR_enCycleSM_Idle;
    ADCSHR_stWorkBuf.enPrioSM = ADCSHR_enPrioSM_Idle;   
}


/**
  * @name       ADCSHR__vHardwareInit
  * @brief      Hardware initialization function of ADCSHR
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2023-12-19
  */
void ADCSHR__vHardwareInit(void)
{
    memset(&hAdc, 0, sizeof(ADC_HandleTypeDef));
	hAdc.Instance = ADC;
		
	HAL_ADC_StructInit(&hAdc);
		
	hAdc.Init.ClockDiv = 0x2;
	hAdc.Init.Mode = ADC_CONVERT_CONTINUE_MODE;
	hAdc.Init.Channel = 0;
	hAdc.Init.FullSize = 0xF;
		

	HAL_ADC_Init(&hAdc);
	HAL_ADC_RegisterCallback(&hAdc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, ADCSHR_HAL__vConvertComplete);
	HAL_ADC_RegisterCallback(&hAdc, HAL_ADC_ERROR_CB_ID, ADCSHR_HAL__vErrorCallback);
	ADCSHR_DMA_ADC__vInit();
}


/**
  * @name       ADCSHR__vCycleSampleStart
  * @brief      Manage channels for system
  * @note       
  * @param      none
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".flashdriver"), optimize(0))) 
#endif
void ADCSHR__vCycleSampleStart(void)
{
	uint8_t index = ADCSHR_stWorkBuf.u8ChannelIndex;

    for(index = index; index < ADCSHR_CHANNEL_MAX_NUMBER; index++)
	{
		if((TRUE == ADCSHR_stWorkBuf.pstChannelControl[index].bRegistered))
		{
            if(ADCSHR_enChannelState_Busy != ADCSHR_stWorkBuf.pstChannelControl[index].enState)
            {

                HAL_ADC_SetChannel(&hAdc, index);
                HAL_ADC_Start_DMA(&hAdc, ADCSHR_stWorkBuf.pstChannelControl[index].u16Data, \
                                (ADCSHR_stWorkBuf.pstChannelControl[index].u8ContinueTimes <= DMA_CHANNEL_1_BLOCK_TS_MAX ? \
                                 ADCSHR_stWorkBuf.pstChannelControl[index].u8ContinueTimes : DMA_CHANNEL_1_BLOCK_TS_MAX) * sizeof(uint16_t));
                
                /* Change enState to ADCSHR_enChannelState_Busy indicate that the special channel is busy */
                ADCSHR_stWorkBuf.pstChannelControl[index].enState = ADCSHR_enChannelState_Busy;
                /* Change enCycleCurSM to ADCSHR_enCycleSM_Work indicate that Cycle ADC Sample is working */
                ADCSHR_stWorkBuf.enCycleCurSM = ADCSHR_enCycleSM_Work;
                break;
            }
            else
            {
                ADCSHR_stWorkBuf.pstChannelControl[index].u8BusyCount++;
                if(ADCSHR_stWorkBuf.pstChannelControl[index].u8BusyCount >= ADCSHR_CHANNEL_MAX_BUSY_COUNT)
                {
                    ADCSHR_stWorkBuf.pstChannelControl[index].u8BusyCount = 0;
                    ADCSHR_stWorkBuf.pstChannelControl[index].enState = ADCSHR_enChannelState_Idle;
                }
            }
		}
        
    }
	
	/* Prevent System interrupt: If be called in Task, ADC DMA interrupt is stopped in advance. */
	if(ADCSHR_CHANNEL_MAX_NUMBER <= index)
	{
		index = 0;
		ADCSHR_stWorkBuf.enCycleCurSM = ADCSHR_enCycleSM_Idle;
	}
	
	ADCSHR_stWorkBuf.u8ChannelIndex = index;
}




/**
  * @name       ADCSHR_vTrigCycleSample
  * @brief      
  * @note       This function be executed in ISR
  * @param      none
  * @retval     none
  * @author     
  * @date       2023-12-19
  */
void ADCSHR_vTrigCycleSample(void)
{
    /* Prevent uninitialized module start work */
    if(FALSE == ADCSHR_stWorkBuf.bInited)
    {
        return;
    }
    
    /* Check State Machine */
    if((ADCSHR_enCycleSM_Idle == ADCSHR_stWorkBuf.enCycleCurSM) && (ADCSHR_enPrioSM_Idle == ADCSHR_stWorkBuf.enPrioSM))
    {
        ADCSHR__vCycleSampleStart();
    }
    else
    {
        /* do nothing */
    }
}


/**
  * @name       ADCSHR__vAdcIoInit
  * @brief      Init GPIO for special ADC channel
  * @note       
  * @param      channel, indicate the special channel
  * @retval     none
  * @author     
  * @date       2023-12-17
  */
void ADCSHR__vAdcIoInit(uint8_t channel)
{
    uint32_t pin;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    switch(channel)
    {
        case ADC_CHANNEL_0:
            pin = GPIO_PIN_19;
            break;
        case ADC_CHANNEL_1:
            pin = GPIO_PIN_18;
            break;
        case ADC_CHANNEL_2:
            pin = GPIO_PIN_17;
            break;
        case ADC_CHANNEL_3:
            pin = GPIO_PIN_16;
            break;
        case ADC_CHANNEL_4:
            pin = GPIO_PIN_15;
            break;
        case ADC_CHANNEL_5:
            pin = GPIO_PIN_14;
            break;
        case ADC_CHANNEL_6:
            pin = GPIO_PIN_13;
            break;
        case ADC_CHANNEL_7:
            pin = GPIO_PIN_12;
            break;
		case ADC_CHANNEL_8:
			// ADC0_DI8 is dedicated to ADC and does not require pin mapping processing. 
			break;

        default:
            pin = GPIO_PIN_15;
            break;    
    }
    
    if (channel != ADC_CHANNEL_8)
	{
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.SchmidtTrigger = GPIO_SCHMIDT_TRIGGER_DISABLE;
		GPIO_InitStruct.DriverStrength = GPIO_DRIVER_STRENGTH_0;
		GPIO_InitStruct.Alternate = GPIO_AF0;//ADC IO
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}


/**
  * @name       ADCSHR_u32RegisterChannel
  * @brief      Register channel to participate cycle ADC sample
  * @note       
  * @param      channel, indicate the special channel
  * @param      continousSampleTimes, indicate the continous Sample times
  * @retval     ADCSHR_enErrState, no err will return ADCSHR_enErrState_OK
  * @author     
  * @date       2023-12-18
  */
uint32_t ADCSHR_u32RegisterChannel(uint8_t channel, uint8_t continousSampleTimes)
{
    ADCSHR_tenErrState errState;
    uint32_t irq;
    
    if((channel >= ADCSHR_CHANNEL_MAX_NUMBER) || (continousSampleTimes > ADCSHR_CHANNEL_DATA_SIZE))
    {
        errState = ADCSHR_enErrState_InvalidPara;
    }
    else
    {
        /* Init ADC GPIO */
        ADCSHR__vAdcIoInit(channel);
        /* Prevent System interrupt */
        irq = csi_irq_save();
        ADCSHR_stWorkBuf.pstChannelControl[channel].bRegistered = TRUE;
        ADCSHR_stWorkBuf.pstChannelControl[channel].u8ContinueTimes = continousSampleTimes;
        ADCSHR_stWorkBuf.pstChannelControl[channel].enState = ADCSHR_enChannelState_Idle;
        errState = ADCSHR_enErrState_OK;
        csi_irq_restore(irq);
    }
    
    return errState;
}


/**
  * @name       ADCSHR_u32UnRegisterChannel
  * @brief      Unregister channel to quit cycle ADC sample
  * @note       
  * @param      channel, indicate the special channel
  * @retval     ADCSHR_enErrState, no err will return ADCSHR_enErrState_OK
  * @author     
  * @date       2023-12-18
  */
uint32_t ADCSHR_u32UnRegisterChannel(uint8_t channel)
{
    ADCSHR_tenErrState errState;
    uint32_t irq;
    
    if(channel >= ADCSHR_CHANNEL_MAX_NUMBER)
    {
        errState = ADCSHR_enErrState_InvalidPara;
    }
    else
    {
        /* Prevent System interrupt */
        irq = csi_irq_save();
        ADCSHR_stWorkBuf.pstChannelControl[channel].bRegistered = FALSE;
        ADCSHR_stWorkBuf.pstChannelControl[channel].u8ContinueTimes = 0;
        ADCSHR_stWorkBuf.pstChannelControl[channel].enState = ADCSHR_enChannelState_Idle;
        errState = ADCSHR_enErrState_OK;
        csi_irq_restore(irq);
    }
    
    return errState;
}


/**
  * @name       ADCSHR_u32GetChannelDataImmediately
  * @brief      Get a ADC channel data for user immediately
  * @note       This function be called in Task, it will return in limited time
  * @param      channel:id of the channel
  * @param      buffer: pointer for the data buffer
  * @param      len:    length of the data
  * @retval     ADCSHR_enErrState, no err will return ADCSHR_enErrState_OK
  * @author     
  * @date       2023-12-19
  */
__attribute__((optimize(0))) uint32_t ADCSHR_u32GetChannelDataImmediately(uint32_t channel, uint16_t* buffer, uint32_t len)
{
    return ADCSHR_u32GetChannelDataWait(channel, buffer, len, ADCSHR_IMMEDIATELY_MAX_WAIT_TIME);
}


/**
  * @name       ADCSHR_u32GetChannelData
  * @brief      Get a ADC channel data for user in indicated time
  * @note       This function be called in Task, it can specify the timeout time
  * @param      channel:id of the channel
  * @param      buffer: pointer for the data buffer
  * @param      len:    length of the data
  * @param      timeout: specify the timeout, unit is ms
  * @retval     ADCSHR_enErrState, no err will return ADCSHR_enErrState_OK
  * @author     
  * @date       2023-12-19
  */
__attribute__((optimize(0))) uint32_t ADCSHR_u32GetChannelDataWait(uint32_t channel, uint16_t* buffer, uint32_t len, uint16_t timeoutMs)
{
    uint16_t t;
    uint32_t i;
    ADCSHR_tenErrState errState;
    uint32_t irq;
    
    if((channel >= ADCSHR_CHANNEL_MAX_NUMBER) || (len > ADCSHR_stWorkBuf.pstChannelControl[channel].u8ContinueTimes))
    {
        errState = ADCSHR_enErrState_InvalidPara;
    }
    else
    {
        for(t = 0; t < timeoutMs; t++)
        {
            if(ADCSHR_enChannelState_Done == ADCSHR_stWorkBuf.pstChannelControl[channel].enState)
            {
                /* Prevent System interrupt */
                irq = csi_irq_save();
                /* Copy data to user buffer */
                for(i = 0; i < len; i++)
                {
                    *buffer++ = ADCSHR_stWorkBuf.pstChannelControl[channel].u16Data[i];
                }
                csi_irq_restore(irq);
                break;
            }
            else
            {
                /* make the task suspend */
                vTaskDelay(1);
            }
        }

        if(t >= timeoutMs)
        {
            errState = ADCSHR_enErrState_Timeout;
        }
        else
        {
            errState = ADCSHR_enErrState_OK;
        }
    }

    return errState;
}


/**
  * @name       ADCSHR_u32PrioGetChannelData
  * @brief      Get a ADC channel data for user priorly
  * @note       
  * @param      channel:id of the channel
  * @param      buffer: pointer for the data buffer
  * @param      len:    length of the data
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-12-19
  */
uint32_t ADCSHR_u32PrioGetChannelData(uint32_t channel, uint16_t* buffer, uint32_t len)
{
    uint32_t irq;

    if((channel >= ADCSHR_CHANNEL_MAX_NUMBER) || (len > ADCSHR_CHANNEL_DATA_SIZE))
    {
        return ADCSHR_enErrState_InvalidPara;
    }

    if(csi_kernel_sem_wait(xSemaphoreAdcPrio, portMAX_DELAY))
    {
        return ADCSHR_enErrState_SemErr;
    }

    /* Enter Prio Adc Sample */
    if(ADCSHR_enCycleSM_Work == ADCSHR_stWorkBuf.enCycleCurSM)
    {
        /* Stop Cycle Sample */
        HAL_ADC_Stop_DMA(&hAdc);
    }
    ADCSHR_stWorkBuf.enCycleLastSM = ADCSHR_stWorkBuf.enCycleCurSM;
    ADCSHR_stWorkBuf.enCycleCurSM = ADCSHR_enCycleSM_Suspend;
    ADCSHR_stWorkBuf.enPrioSM = ADCSHR_enPrioSM_Work;
        
    /* Start Sample */
    /* Set ADC Full Size */
    hAdc.Init.FullSize = len;
    hAdc.Instance->FULLSIZE &= ~ADC_FULL_SIZE_Msk;
    hAdc.Instance->FULLSIZE |= hAdc.Init.FullSize;
    HAL_ADC_SetChannel(&hAdc, channel);
	
	HAL_ADC_Start(&hAdc);
	__HAL_ADC_WAIT_DONE(&hAdc);
	
	HAL_ADC_GetValue(&hAdc, buffer, len);

    /* Exit Prio Adc Sample */
    if(ADCSHR_enCycleSM_Work == ADCSHR_stWorkBuf.enCycleLastSM)
    {
        ADCSHR_stWorkBuf.enCycleCurSM = ADCSHR_enCycleSM_Work;
        ADCSHR__vCycleSampleStart();
    }
    else
    {
        ADCSHR_stWorkBuf.enCycleCurSM = ADCSHR_enCycleSM_Idle;
    }
    ADCSHR_stWorkBuf.enPrioSM = ADCSHR_enPrioSM_Idle;

    csi_kernel_sem_post(xSemaphoreAdcPrio);
    
    return ADCSHR_enErrState_OK;
}


/**
  * @name       ADCSHR_vInit
  * @brief      Initialization function of ADCSHR module
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2023-12-18
  */
void ADCSHR_vInit(void)
{
    memset(&ADCSHR_stWorkBuf, 0, sizeof(ADCSHR_tstWorkBuf));
    memset(&ADCSHR_astChannelControl, 0, sizeof(ADCSHR_astChannelControl));
    ADCSHR_stWorkBuf.pstChannelControl = ADCSHR_astChannelControl;
    
    ADCSHR__vHardwareInit();
    
    xSemaphoreAdcPrio = csi_kernel_sem_new(1, 1);
    if(NULL == xSemaphoreAdcPrio)
    {
        /* Log err */
        while(1);
    }
    ADCSHR_stWorkBuf.bInited = TRUE;
}



