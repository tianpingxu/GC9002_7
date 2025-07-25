#include "csi_ahmi_i2s.h"
#include "csi_hal_core.h"

#if defined(EMBEDDED) && defined(VERSION_2)
void I2STX_Handler(void);
uint16_t databuffer[32] = {0};
uint8_t currentmusicLOOP = 0;
uint32_t currentmusicSIZE = 0;
uint16_t* currentmusicSTART = 0;
uint16_t* currentmusicPos = 0;
extern uint8_t  audio_should_pause_till_cycle_end;
extern uint8_t  audio_should_stop_till_cycle_end;
extern s32  audio_play_num; // -1 no play_num
#endif

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
static I2S_HandleTypeDef hi2shandle = {.Instance = I2S};
static uint8_t sI2SInitFlag = 0;
#if defined(CHIP_GC9005)
static CODEC_HandleTypeDef haudio = {.Instance = CODEC};
#endif
#endif
void csi_ahmi_i2s_init(uint32_t SampleRate)
{	
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
    if(sI2SInitFlag != 0)
    {
        return ;
    }
	
	switch(SampleRate)
    {
    case I2S_AUDIOFREQ_8K:
    case I2S_AUDIOFREQ_11K:
    case I2S_AUDIOFREQ_12K:
    case I2S_AUDIOFREQ_16K:
    case I2S_AUDIOFREQ_22K:
    case I2S_AUDIOFREQ_24K:
    case I2S_AUDIOFREQ_32K:
    case I2S_AUDIOFREQ_44K:
    case I2S_AUDIOFREQ_48K:
    case I2S_AUDIOFREQ_96K:
    case I2S_AUDIOFREQ_192K:
    case I2S_AUDIOFREQ_256K:
        break;
    default:
        SampleRate = I2S_AUDIOFREQ_16K;//GC9002 system PLL is 480M, duty_h duty_l max 0xFF, can't support less than 16KHz
        break;
    }
	
	HAL_I2S_StructInit(&hi2shandle.Init);
	
#if defined(CHIP_GC9002)
	hi2shandle.Init.Mode = I2S_MODE_MASTER;
	hi2shandle.Init.DataFormat = I2S_WSS_CLOCK_CYCLES_16;
	hi2shandle.Init.TxResolution = I2S_WLEN_RESOLUTION_16_BIT;
#endif
	hi2shandle.Init.TxTriggerLevel = I2S_TRIGGER_LEVEL_2;
	hi2shandle.Init.AudioFreq = SampleRate;
    HAL_I2S_Init(&hi2shandle);
	
#if defined(CHIP_GC9002)
	__HAL_I2S_CHX_TX_ENABLE(&hi2shandle, hi2shandle.Init.Channel);
    __HAL_I2S_CHX_ENABLE_IT(&hi2shandle, hi2shandle.Init.Channel, I2S_IT_TXFEM);
#else
	HAL_CODEC_StructInit(&haudio);
    HAL_CODEC_Init(&haudio);
    HAL_CODEC_SetVolume(&haudio, 50);
#endif
	sI2SInitFlag = 1;
#else
	I2S_InitTypeDef I2S_InitStruct;
	if(SampleRate == 5120)
	{
		I2S_InitStruct.APBCLKPRE = APBtoI2S_PRE_128;
	}
	else if(SampleRate == 10200)
	{
		I2S_InitStruct.APBCLKPRE = APBtoI2S_PRE_64;
	}
	else if(SampleRate == 20500)
	{
		I2S_InitStruct.APBCLKPRE = APBtoI2S_PRE_32;
	}
	else if(SampleRate == 41000)
	{
		I2S_InitStruct.APBCLKPRE = APBtoI2S_PRE_16;
	}
	else
	{
		I2S_InitStruct.APBCLKPRE = APBtoI2S_PRE_128;
	}
	I2S_InitStruct.I2S_Mode = I2S_Mode_Tx;
	I2S_InitStruct.I2S_Resolution = I2S_Resolution_16b;
	I2S_InitStruct.I2S_WSS = I2S_WSS_16ClockCycles;
	I2S_InitStruct.I2S_TriggerLevel = I2S_TriggerLevel_2bit;
	
	I2S_Init(&I2S_InitStruct);
	I2S_ITConfig(I2S_FLAG_TXE, ENABLE);
#endif
}

void csi_ahmi_i2s_play(uint16_t *MusicStart, uint32_t MusicSize, uint8_t MusicLoop, uint16_t PlayNum)
{
	uint32_t i = 0;
	
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	csi_hal_irq_disable(I2S_IRQn);
	#else
	NVIC_DisableIRQ(I2STX_IRQn);
	#endif
	
	currentmusicSTART = MusicStart;
    currentmusicPos = MusicStart;
    currentmusicSIZE = MusicSize;
    currentmusicLOOP = MusicLoop;
	audio_should_pause_till_cycle_end = 0;
	audio_should_stop_till_cycle_end = 0;
	audio_play_num = PlayNum>0?PlayNum:-1;
	
    for(i = 0; i < (I2S_TX_FIFO_DEPTH - 1) * 2; i++)
    {
        databuffer[i] = *currentmusicPos;
		currentmusicPos++;
    }
	
	#if defined(CHIP_GC9002)
	csi_hal_irq_register(I2S_IRQn, (void *)I2S_IRQHandler);
	csi_hal_irq_enable(I2S_IRQn);
	#elif defined(CHIP_GC9005)
	csi_hal_irq_register(I2S_IRQn, NULL);
	csi_hal_irq_enable(I2S_IRQn);
	HAL_I2S_Transmit_IT(&hi2shandle, (uint8_t*)currentmusicSTART, currentmusicSIZE);
	#else
	NVIC_EnableIRQ(I2STX_IRQn); 
	#endif
}

void csi_ahmi_i2s_pause(void)
{
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
    csi_hal_irq_disable(I2S_IRQn);
	#else
	NVIC_DisableIRQ(I2STX_IRQn);
	#endif
}

void csi_ahmi_i2s_resume(void)
{
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
    csi_hal_irq_enable(I2S_IRQn);
	#else
	NVIC_EnableIRQ(I2STX_IRQn);
	#endif 
}

void csi_ahmi_i2s_stop(void)
{
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	sI2SInitFlag = 0;
	HAL_I2S_DeInit(&hi2shandle);
	csi_hal_irq_disable(I2S_IRQn);
	#else
	I2S_DeInit();
	NVIC_DisableIRQ(I2STX_IRQn);
	#endif
}

#if defined(EMBEDDED) && defined(VERSION_2)
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
void I2S_IRQHandler(void)
#else
#if defined(CHIP_GC9002)
__attribute__ ((interrupt ("machine"))) void I2S_IRQHandler(void)
#else
__attribute__((interrupt)) void I2S_IRQHandler(void)
#endif
#endif
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

#if defined(CHIP_GC9005)
    rt_interrupt_enter();
#endif

    int i = 0;
	__HAL_I2S_CHX_TX_CLEAR_OVRFLAG(&hi2shandle, hi2shandle.Init.Channel);
	
	if (((hi2shandle.Instance->Chx[hi2shandle.Init.Channel].ISR & I2S_FLAG_TXFE) == I2S_FLAG_TXFE) && (__HAL_I2S_CHX_GET_IT_SOURCE(&hi2shandle, hi2shandle.Init.Channel, I2S_IT_TXFEM) != RESET))
	{
		if(currentmusicSIZE > ((I2S_TX_FIFO_DEPTH - 1) * 2 * 2))
		{
			for(i = 0; i < (I2S_TX_FIFO_DEPTH - 1); i++)
			{
				hi2shandle.Instance->Chx[hi2shandle.Init.Channel].LTHR = databuffer[2*i];
				hi2shandle.Instance->Chx[hi2shandle.Init.Channel].RTHR = databuffer[2*i + 1];
			}
			
			for(i = 0; i < (I2S_TX_FIFO_DEPTH - 1) * 2; i++)
			{
				databuffer[i] = *currentmusicPos;
				currentmusicPos++;
			}	
		}
		
		if(((uint32_t)currentmusicPos - (uint32_t)currentmusicSTART) >= currentmusicSIZE)
		{
			if(((uint32_t)currentmusicPos - (uint32_t)currentmusicSTART - currentmusicSIZE) <= ((I2S_TX_FIFO_DEPTH - 1) * 2 * 2))
			{
				for(i = 0; i < ((((I2S_TX_FIFO_DEPTH - 1) * 2 * 2) - ((uint32_t)currentmusicPos - (uint32_t)currentmusicSTART - currentmusicSIZE)) / 4); i++)
				{
					hi2shandle.Instance->Chx[hi2shandle.Init.Channel].LTHR = databuffer[2*i];
					hi2shandle.Instance->Chx[hi2shandle.Init.Channel].RTHR = databuffer[2*i + 1];
				}
			}
			
			currentmusicPos = currentmusicSTART; 
				
			for(i = 0; i < (I2S_TX_FIFO_DEPTH - 1) * 2; i++)
			{
				databuffer[i] = *currentmusicPos;
				currentmusicPos++;
			}
			
			if(audio_should_pause_till_cycle_end)
			{
				audio_should_pause_till_cycle_end = 0;
				csi_hal_irq_disable(I2S_IRQn);
			}
			
			if(audio_should_stop_till_cycle_end)
			{
				audio_should_stop_till_cycle_end = 0;
				HAL_I2S_DeInit(&hi2shandle);
				csi_hal_irq_disable(I2S_IRQn);
			}
			
			if((currentmusicLOOP) && (audio_play_num == -1))
			{
				//return;
			    goto _end;
			}
			else if(audio_play_num > 0)
			{
				audio_play_num--;
				
				if(audio_play_num != 0)
				{
					//return;
				    goto _end;
				}
				else
				{
					csi_hal_irq_disable(I2S_IRQn);
				}
			}
			else
			{
				csi_hal_irq_disable(I2S_IRQn);
			}
		}
	}

_end:
#if defined(CHIP_GC9005)
    rt_interrupt_leave();
#endif
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
#else
void I2STX_HandlerISR(void)
{
    int i = 0;
	NVIC_ClearPendingIRQ(I2STX_IRQn);
	
	if(currentmusicSIZE > 60)
	{
		for(i = 0; i < 15; i++)
		{
			I2S_SendLeftData(databuffer[2*i]);
			I2S_SendRightData(databuffer[2*i + 1]);
		}
		
		for(i = 0; i < 30; i++)
		{
			databuffer[i] = *currentmusicPos;
			currentmusicPos++;
		}	
	}
	
	if(((uint32_t)currentmusicPos - (uint32_t)currentmusicSTART) >= currentmusicSIZE)
	{
		if(((uint32_t)currentmusicPos - (uint32_t)currentmusicSTART - currentmusicSIZE) <= 60)
		{
			for(i = 0; i < ((60 - ((uint32_t)currentmusicPos - (uint32_t)currentmusicSTART - currentmusicSIZE)) / 4); i++)
			{
				I2S_SendLeftData(databuffer[2*i]);
				I2S_SendRightData(databuffer[2*i + 1]);
			}
		}
		
		currentmusicPos = currentmusicSTART; 
			
		for(i = 0; i < 30; i++)
		{
			databuffer[i] = *currentmusicPos;
			currentmusicPos++;
		}
		
		if(audio_should_pause_till_cycle_end)
		{
			audio_should_pause_till_cycle_end = 0;
			NVIC_DisableIRQ(I2STX_IRQn);
		}
		
		if(audio_should_stop_till_cycle_end)
		{
			audio_should_stop_till_cycle_end = 0;
			I2S_DeInit();
			NVIC_DisableIRQ(I2STX_IRQn);
		}
		
		if((currentmusicLOOP) && (audio_play_num == -1))
		{
			return;
		}
		else if(audio_play_num > 0)
		{
			audio_play_num--;
			
			if(audio_play_num != 0)
			{
				return;
			}
			else
			{
				NVIC_DisableIRQ(I2STX_IRQn);
			}
		}
		else
		{
			NVIC_DisableIRQ(I2STX_IRQn);
		}
	}
}
#endif
#endif
