#include "adcshare.h"
#include "csi_irq_vectors.h"
#include "csi_hal_core.h"
#include "csi_kernel.h"
#include "semphr.h"

#define ADCChannelMaxNumber		9
adcChannelControl channeldata[ADCChannelMaxNumber] = {0};
adcManage ADCManageBlock = {.ChannelControl = 0};

volatile static uint32_t ADCCurrentChannel = 0;
volatile static uint32_t ADCShareControl = 0;
volatile static uint32_t ADCShareWorkingFlag = 0;
volatile static uint32_t ADCSharePriorFlag = 0;
volatile static uint32_t ADCShareStartTriggerFlag = 0;
volatile static uint32_t ADCInitFlag = 0;
k_sem_handle_t ADCShareSemaphore = NULL;
ADC_HandleTypeDef hAdc;


/**
  * @name       ADCEnableChannel
  * @brief      Enable a ADC channel for user
  * @note       
  * @param      channel:id of the channel
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCEnableChannel(uint32_t channel)
{
	if(channel >= ADCChannelMaxNumber)
		return HAL_ERROR;
	
	if((ADCManageBlock.ChannelControl & (1 << channel)) != 0)
		return HAL_ERROR;
	
	ADCManageBlock.ChannelControl |= 1 << channel;
	channeldata[channel].state = ADCStateInitial;
	
	return HAL_OK;
}


/**
  * @name       ADCDisableChannel
  * @brief      Disable a ADC channel for user
  * @note       
  * @param      channel:id of the channel
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCDisableChannel(uint32_t channel)
{
	if(channel >= ADCChannelMaxNumber)
		return HAL_ERROR;
	
	channeldata[channel].state = ADCStateNoUsed;
	ADCManageBlock.ChannelControl &= (~(1 << channel));
	
	return HAL_OK;
}


/**
  * @name       ADCStartChannel
  * @brief      Start a ADC channel for user
  * @note       
  * @param      channel:id of the channel
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCStartChannel(uint32_t channel)
{
	if(channel >= ADCChannelMaxNumber)
		return HAL_ERROR;
	
	channeldata[channel].state = ADCStateStart;
	
	return HAL_OK;
}


/**
  * @name       ADCStopChannel
  * @brief      Stop a ADC channel for user
  * @note       
  * @param      channel:id of the channel
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCStopChannel(uint32_t channel)
{
	if(channel >= ADCChannelMaxNumber)
		return HAL_ERROR;
	
	channeldata[channel].state = ADCStateInitial;
	
	return HAL_OK;
}


/**
  * @name       ADCGetChannelState
  * @brief      Get a ADC channel's state for user
  * @note       
  * @param      channel:id of the channel
  * @param      pState: pointer for state
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCGetChannelState(uint32_t channel, uint32_t* pState)
{
	if(channel >= ADCChannelMaxNumber)
		return HAL_ERROR;
		
	*pState = channeldata[channel].state;
	
	return HAL_OK;
}


/**
  * @name       ADCChannelWaitDone
  * @brief      Wait a ADC channel convert done for user
  * @note       
  * @param      channel:id of the channel
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCChannelWaitDone(uint32_t channel)
{
	
	if(channel >= ADCChannelMaxNumber)
		return HAL_ERROR;
	
	while(channeldata[channel].state != ADCStateDataReady);
	
	return HAL_OK;
}

//uint32_t ADCGetChannelData(uint32_t channel, uint16_t* buffer, uint32_t len)
//{
//	uint32_t i = 0;
//	if((channel >= ADCChannelMaxNumber) || (buffer == NULL) || (len > ADCChannelDataSize))
//		return HAL_ERROR;
//	
//	while(channeldata[channel].state != ADCStateDataReady);
//	
//	for(i = 0; i < len; i++)
//	{
//		*(buffer + i) = channeldata[channel].data[i];
//	}
//	
//	channeldata[channel].state = ADCStateStart;
//	
//	return HAL_OK;
//}

/**
  * @name       ADCGetChannelData
  * @brief      Get a ADC channel data for user
  * @note       
  * @param      channel:id of the channel
  * @param      buffer: pointer for the data buffer
  * @param      len:    length of the data
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCGetChannelData(uint32_t channel, uint16_t* buffer, uint32_t len)
{
	uint32_t i = 0;
	if((channel >= ADCChannelMaxNumber) || (buffer == NULL) || ((len + channeldata[channel].index) > ADCChannelDataSize))
		return HAL_ERROR;
	
//	while(channeldata[ADCCurrentChannel].state != ADCStateDataReady);
	
	for(i = 0; i < len; i++)
	{
		*(buffer + i) = channeldata[channel].data[channeldata[channel].index];
		channeldata[channel].index++;
	}
	
	
//	channeldata[ADCCurrentChannel].state = ADCStateStart;
	
	return HAL_OK;
}

DMA_HandleTypeDef adc_hdma;


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
static void TEMPLATE_DMA_IRQHandler(void)
#else
ATTRIBUTE_ISR void DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_DMA_IRQHandler(hAdc.hdma);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

/**
  * @name       HAL_ADC_ConvertComplete
  * @brief      Callback function for ADC complete convert 
  * @note        
  * @param      hadc: pointer to ADC handle
  * @retval     none
  * @author     
  * @date       2023-11-06
  */
void HAL_ADC_ConvertComplete(ADC_HandleTypeDef *hadc)
{
//    for (int i = 0; i < TEST_ADC_BUFFER_SIZE; i++)
//    {
//        // printf("s_val[%d]: %d\n", i, s_val[i]);
//        printf("s_val[%d]: %d\n", i, HAL_ADC_ValueToCelsius(s_val[i]));
//        s_val[i] = 0;
//    }
//    printf("\n");
	channeldata[ADCCurrentChannel].state = ADCStateDataReady;
	ADCCurrentChannel++;
	
	ADCShareWorkingFlag = 0;
	
	if(ADCCurrentChannel >= ADCChannelMaxNumber)
	{
		ADCCurrentChannel = 0;
		ADCShareControl = 0;
		return;
	}
	else
	{
		ADCChannelManageInit();
	}
}

/**
  * @name       HAL_ADC_ErrorCallback
  * @brief      Callback function for ADC error 
  * @note        
  * @param      hadc: pointer to ADC handle
  * @retval     none
  * @author     
  * @date       2023-11-06
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
//    printf("ADC DMA Get Value error.\n");
	ADCShareWorkingFlag = 0;
	ADCChannelManageInit();
}


/**
  * @name       DMA_ADC_Init
  * @brief      Init DMA channel for ADC for system
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2023-11-06
  */
void DMA_ADC_Init(void)
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
  * @name       ADCShareInit
  * @brief      Init ADC and DMA
  * @note       
  * @param      none
  * @retval     None
  * @author     
  * @date       2023-11-17
  */
void ADCShareInit(void)
{
	memset(&hAdc, 0, sizeof(ADC_HandleTypeDef));
	hAdc.Instance = ADC;
		
	HAL_ADC_StructInit(&hAdc);
		
	hAdc.Init.ClockDiv = 0x2;
	hAdc.Init.Mode = ADC_CONVERT_CONTINUE_MODE;
	hAdc.Init.Channel = ADCCurrentChannel;
	hAdc.Init.FullSize = 0xF;
		

	HAL_ADC_Init(&hAdc);
	HAL_ADC_RegisterCallback(&hAdc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, HAL_ADC_ConvertComplete);
	HAL_ADC_RegisterCallback(&hAdc, HAL_ADC_ERROR_CB_ID, HAL_ADC_ErrorCallback);
	DMA_ADC_Init();
	
//	/* Create Semaphore for ADC. */
//	ADCShareSemaphore = csi_kernel_sem_new( 1, 1 ); 
//	if(ADCShareSemaphore == NULL)
//	{
////		ERROR_PRINT("Fail to create semaphore ADCShareSemaphore.\r\n");
//		return ;
//	}
}


/**
  * @name       ADCChannelManageInit
  * @brief      Manage channels for system
  * @note       
  * @param      none
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCChannelManageInit(void)
{
	
	uint32_t i = 0;
	
	if(ADCSharePriorFlag == 1)
	{
		ADCShareStartTriggerFlag = 1;
		return ADCChannelWoring;
	}
	
	for(i = ADCCurrentChannel; i < ADCChannelMaxNumber; i++)
	{
		if((channeldata[ADCCurrentChannel].state == ADCStateStart) || (channeldata[ADCCurrentChannel].state == ADCStateDataReady))
		{
			if(ADCInitFlag == 0)
			{
				ADCShareInit();
				
				ADCInitFlag = 1;
			}
			
			ADCShareWorkingFlag = 1;
			
			HAL_ADC_SetChannel(&hAdc, ADCCurrentChannel);
			
			HAL_ADC_Start_DMA(&hAdc, channeldata[ADCCurrentChannel].data, ADCChannelDataSize * sizeof(uint16_t));
		
			channeldata[ADCCurrentChannel].state = ADCStateBusy;
			channeldata[ADCCurrentChannel].index = 0;
		
//			for(i = 0; i < ADCChannelDataSize; i++)
//			{
//				HAL_ADC_Start(&hAdc);
//				__HAL_ADC_WAIT_DONE(&hAdc);
//			
//				HAL_ADC_GetValue(&hAdc, channeldata[ADCCurrentChannel].data + i, 1);
//			}
		
			ADCShareControl = 1;
			break;
		
		}
		
		ADCCurrentChannel++;
		
		if(ADCCurrentChannel >= ADCChannelMaxNumber)
		{
			ADCCurrentChannel = 0;
			ADCShareControl = 0;
		}
	}

	
	return ADCChannelWoring;
}


/**
  * @name       ADCChannelStartConvert
  * @brief      Start ADC convert for system
  * @note       
  * @param      none
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-06
  */
uint32_t ADCChannelStartConvert(void)
{
//	while(ADCChannelManageInit() == ADCChannelWoring);
//	ADCCurrentChannel = 0;
	
	if(ADCShareControl == 0)
	{
		return ADCChannelManageInit();
	}
	else 
	{
		return 0;
	}
}

/**
  * @name       GetADCDataPrior
  * @brief      Get a ADC channel data for user
  * @note       
  * @param      channel:id of the channel
  * @param      buffer: pointer for the data buffer
  * @param      len:    length of the data
  * @retval     bool:ERROE or OK
  * @author     
  * @date       2023-11-17
  */
uint32_t GetADCDataPrior(uint32_t channel, uint16_t *buf, uint8_t size)
{
	
	if(channel >= ADCChannelMaxNumber)
		return HAL_ERROR;
		
	if(ADCInitFlag == 0)
	{
		ADCShareInit();
	
		ADCInitFlag = 1;
	}
		
//	if(ADCShareSemaphore != NULL)
//	{
//		xSemaphoreTake(ADCShareSemaphore, portMAX_DELAY);
//	}
//	
	ADCSharePriorFlag = 1;
	
	while(ADCShareWorkingFlag == 1);
	
	
	
//	hAdc.Init.Channel = channel;
	HAL_ADC_SetChannel(&hAdc, channel);
	
	HAL_ADC_Start(&hAdc);
	__HAL_ADC_WAIT_DONE(&hAdc);
	
	HAL_ADC_GetValue(&hAdc, buf, size);
	
	ADCSharePriorFlag = 0;
	
	if(ADCShareStartTriggerFlag == 1)
	{
		if(ADCShareControl == 0)
		{
			ADCChannelManageInit();
		}
	}
	
//	if(ADCShareSemaphore != NULL)
//	{
//		xSemaphoreGive(ADCShareSemaphore);
//	}
	
	
	return HAL_OK;
}


/**
  * @name       ADCPriorTakeSem
  * @brief      Take the semaphore for ADC share
  * @note       
  * @param      intflag: 1: call it in intrrupt,0: call it not in intrrupt.
  * @retval     none
  * @author     
  * @date       2023-11-20
  */
void ADCPriorTakeSem( uint32_t intflag )
{
    
    static BaseType_t xHigherPriorityTaskWoken;
	
	if(ADCShareSemaphore == NULL)
	{
		/* Create Semaphore for ADC. */
		ADCShareSemaphore = csi_kernel_sem_new( 1, 1 ); 
		if(ADCShareSemaphore == NULL)
		{
//			ERROR_PRINT("Fail to create semaphore ADCShareSemaphore.\r\n");
			return ;
		}
	
	}

    if(ADCShareSemaphore != NULL) //if donnot calibration, needs to start video too
    {
		if(intflag == 1)
		{
			xSemaphoreTakeFromISR(ADCShareSemaphore,  &xHigherPriorityTaskWoken);
		}
		else 
		{
			xSemaphoreTake(ADCShareSemaphore, portMAX_DELAY);
		}
	}
}


/**
  * @name       ADCPriorGiveSem
  * @brief      Give the semaphore for ADC share
  * @note       
  * @param      intflag: 1: call it in intrrupt,0: call it not in intrrupt.
  * @retval     none
  * @author     
  * @date       2023-11-20
  */
void ADCPriorGiveSem( uint32_t intflag )
{
	static BaseType_t xHigherPriorityTaskWoken;

	if(ADCShareSemaphore != NULL) //if donnot calibration, needs to start video too
    {
        if(intflag == 1)
        {
            xSemaphoreGiveFromISR(ADCShareSemaphore, &xHigherPriorityTaskWoken);
        }
		else 
		{
			xSemaphoreGive(ADCShareSemaphore);
		}
    }
}

#if 0
void adcshareTest(void)
{
	uint32_t i = 0;
	uint16_t temp[ADCChannelDataSize] = {0};
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_16 | GPIO_PIN_17 | GPIO_PIN_18 | GPIO_PIN_19;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.SchmidtTrigger = GPIO_SCHMIDT_TRIGGER_DISABLE;
    GPIO_InitStruct.DriverStrength = GPIO_DRIVER_STRENGTH_0;
    GPIO_InitStruct.Alternate = GPIO_AF0;//general IO
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	
	ADCEnableChannel(ADC_CHANNEL_0);
	ADCEnableChannel(ADC_CHANNEL_1);
	ADCEnableChannel(ADC_CHANNEL_2);
	ADCEnableChannel(ADC_CHANNEL_3);
	ADCEnableChannel(ADC_CHANNEL_4);
	ADCEnableChannel(ADC_CHANNEL_5);
	ADCEnableChannel(ADC_CHANNEL_6);
	ADCEnableChannel(ADC_CHANNEL_7);
	ADCEnableChannel(ADC_CHANNEL_8);
	
	while(1)
	{
	
		for(i = 0; i < ADCChannelMaxNumber; i++)
		{
			ADCStartChannel(i);
		}
		
		for(i = 0; i < ADCChannelMaxNumber; i++)
		{
			ADCChannelWaitDone(i);
			ADCGetChannelData(i, temp, ADCChannelDataSize);
			ADCStopChannel(i);
		}
	}
	
	ADCDisableChannel(ADC_CHANNEL_0);
	ADCDisableChannel(ADC_CHANNEL_1);
	ADCDisableChannel(ADC_CHANNEL_2);
	ADCDisableChannel(ADC_CHANNEL_3);
	ADCDisableChannel(ADC_CHANNEL_4);
	ADCDisableChannel(ADC_CHANNEL_5);
	ADCDisableChannel(ADC_CHANNEL_6);
	ADCDisableChannel(ADC_CHANNEL_7);
	ADCDisableChannel(ADC_CHANNEL_8);
}

#endif

