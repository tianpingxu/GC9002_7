/**
  ******************************************************************************
  * @file    adcshare.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-10-23
  * @brief   This file contains all the functions prototypes for the ADC
  *          firmware library.
  *******************************************************************************/
#ifndef __GC90x_ADC_SHARE_H__
#define __GC90x_ADC_SHARE_H__


#include "gc90xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ADCChannelDataSize	16

typedef struct adcChannelControlTag
{
	volatile uint16_t state;
	uint16_t index;
	uint16_t data[ADCChannelDataSize];
}adcChannelControl;

typedef struct adcManageTag
{
	uint32_t              ChannelControl; /*!< ADC channel control                      */
}adcManage;

#define ADC_UsedChannel0		(0x00001)
#define ADC_UsedChannel1		(0x00002)
#define ADC_UsedChannel2		(0x00004)
#define ADC_UsedChannel3		(0x00008)
#define ADC_UsedChannel4		(0x00010)
#define ADC_UsedChannel5		(0x00020)
#define ADC_UsedChannel6		(0x00040)
#define ADC_UsedChannel7		(0x00080)
#define ADC_UsedChannel8		(0x00100)
#define ADC_UsedChannel9		(0x00200)	//reserved
#define ADC_UsedChannel10		(0x00400)	//reserved
#define ADC_UsedChannel11		(0x00800)	//reserved
#define ADC_UsedChannel12		(0x01000)	//reserved
#define ADC_UsedChannel13		(0x02000)	//reserved
#define ADC_UsedChannel14		(0x04000)	//core 1.2V
#define ADC_UsedChannel15		(0x08000)	//1.8V
#define ADC_UsedChannelTS		(0x10000)	//Temperature Sensor channel

#define ADCStateNoUsed		0
#define ADCStateInitial		1
#define ADCStateStart		2	
#define ADCStateBusy		3
#define ADCStateDataReady	4

#define ADCChannelWoring	0
#define ADCChannelEnd		1

uint32_t ADCEnableChannel(uint32_t channel);
uint32_t ADCDisableChannel(uint32_t channel);
uint32_t ADCStartChannel(uint32_t channel);
uint32_t ADCStopChannel(uint32_t channel);
uint32_t ADCGetChannelState(uint32_t channel, uint32_t* pState);
uint32_t ADCChannelWaitDone(uint32_t channel);
uint32_t ADCGetChannelData(uint32_t channel, uint16_t* buffer, uint32_t len);
uint32_t ADCChannelManageInit(void);
uint32_t ADCChannelStartConvert(void);
uint32_t GetADCDataPrior(uint32_t channel, uint16_t *buf, uint8_t size);
void ADCPriorTakeSem( uint32_t intflag );
void ADCPriorGiveSem( uint32_t intflag );

#ifdef __cplusplus
}
#endif

#endif
