/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-14     huyt         V1.0.1
 */
#include "gc90xx_hal.h"
#include <soft_libc.h>

#ifdef HAL_PMU_MODULE_ENABLED
static uint16_t adc_get_value(uint16_t ch)
{
    volatile uint32_t *adc = (volatile uint32_t *)0x40107000;
    uint16_t retval = 0;
	
	*(adc + 1) &= ~(1 << 8);  //adc_on disable
	*(adc + 2) &= ~0x0000000F;
	*(adc + 2) |= ch;         //adjust core ldo by adc channel_14
	*(adc + 1) |= (1 << 8);   //adc_on enable
    
    (*(adc + 8)) &= ~0x1;
    *adc |= 0x01;  //adc start
    while((*(adc + 8) & 0x1) == 0);  //wait adc done
    //*(adc + 7) |= 0x1;  //adc stop only for continue mode
    retval = *(adc + 9) & 0x00000FFF;
    return retval;
}

HAL_StatusTypeDef HAL_PMU_VTR12_Calibrate(uint16_t threshold)
{
    uint32_t tempreg;
    uint16_t adc_get_array[TEST_ADC_ARRAY_COUNT];
	volatile uint32_t *pmu_ctl = (volatile uint32_t *)0x4020003C;
	volatile uint32_t *adc = (volatile uint32_t *)0x40107000;
	const uint16_t targetAdcVal = 1489;  //1489=(4096/3.3)*1.2
	uint16_t val[4] = {0, 0, 0, 0};
	uint16_t defaultConvertVal = 0;
	
    for (uint32_t i = 0; i < TEST_ADC_ARRAY_COUNT; i++)
    {
        adc_get_array[i] = adc_get_value(14);//adjust core ldo by adc channel_14
    }
    bubble_sort_u16(adc_get_array, TEST_ADC_ARRAY_COUNT);
    
    defaultConvertVal = adc_get_array[TEST_ADC_ARRAY_COUNT/2];
	if(defaultConvertVal - targetAdcVal < threshold)
	{
		return HAL_OK;
	}

    //factor值越大则反馈值也越大
	//adjust core ldo parameter
	for(uint32_t i = 0; i < 4; i++)
	{
		uint32_t factor = (defaultConvertVal > targetAdcVal) ? (3-i) : (i+4);
        //set vtr12 parameter
        tempreg = RCC->PMU_CTL.reg_u32val;
        ((union _PMU_CTL *)(&tempreg))->bit.vtr12 = factor;
        RCC->PMU_CTL.reg_u32val = tempreg;
		
		for(uint32_t j = 0; j < 1000; j++) 
        {
            __NOP();
        }
        for(uint32_t j = 0; j < TEST_ADC_ARRAY_COUNT; j++)
        {
            adc_get_array[j] = adc_get_value(14);//adjust core ldo by adc channel_14
        }
        bubble_sort_u16(adc_get_array, TEST_ADC_ARRAY_COUNT);
		val[i] = (adc_get_array[TEST_ADC_ARRAY_COUNT/2] > targetAdcVal) ? 
                 (adc_get_array[TEST_ADC_ARRAY_COUNT/2] - targetAdcVal) : 
                 (targetAdcVal - adc_get_array[TEST_ADC_ARRAY_COUNT/2]);
		if(val[i] < threshold)
		{
		    return HAL_OK;
		}
	}
	
	//if all is not in threshold, then find the closest
	uint16_t min = 0xFFFF;
    uint8_t  minLdo = 0xFF;
    for(uint32_t i = 0; i < 4; i++)
    {
        if(val[i] < min)
        {
            min = val[i];
            minLdo = (defaultConvertVal > targetAdcVal) ? (3-i) : (i+4);
        }
    }
    //set vtr12 parameter
    tempreg = RCC->PMU_CTL.reg_u32val;
    ((union _PMU_CTL *)(&tempreg))->bit.vtr12 = minLdo;
    RCC->PMU_CTL.reg_u32val = tempreg;
    return HAL_ERROR;	
}

HAL_StatusTypeDef HAL_PMU_VTR3318_Calibrate(uint16_t threshold)
{
    HAL_StatusTypeDef ret = HAL_OK;
    uint16_t adc_get_array[TEST_ADC_ARRAY_COUNT];
    uint32_t tempreg;
    volatile uint32_t *pmu_ctl = (volatile uint32_t *)0x4020003C;
    volatile uint32_t *adc = (volatile uint32_t *)0x40107000;
    const uint32_t targetAdcVal = 2234;  //2234=(4096/3.3)*1.8
    uint8_t vo3318_en;

    HAL_RCC_VO3318Enable();

    for (uint32_t i = 0; i < TEST_ADC_ARRAY_COUNT; i++)
    {
        adc_get_array[i] = adc_get_value(15);//adjust 3318 ldo by adc channel_15
    }

    //factor值越大则反馈值也越大
    //adjust 3318 ldo parameter
    uint32_t maxFactor = 8;
    uint32_t minFactor = 0;
    uint32_t midFactor = (maxFactor + minFactor) >> 1;
    do {
        //set 3318 parameter
        tempreg = RCC->PMU_CTL.reg_u32val;
        ((union _PMU_CTL *)(&tempreg))->bit.vtr3318 = midFactor;
        RCC->PMU_CTL.reg_u32val = tempreg;
        
        for(uint32_t j = 0; j < 1000; j++)
        {
            __NOP();
        }
        for(uint32_t j = 0; j < TEST_ADC_ARRAY_COUNT; j++)
        {
            adc_get_array[j] = adc_get_value(15);//adjust 3318 ldo by adc channel_15
        }
        bubble_sort_u16(adc_get_array, TEST_ADC_ARRAY_COUNT);
        if(adc_get_array[TEST_ADC_ARRAY_COUNT/2] > targetAdcVal)
        {
            maxFactor = midFactor;
            midFactor = (maxFactor + minFactor) >> 1;
        }
        else
        {
            minFactor = midFactor;
            midFactor = (maxFactor + minFactor) >> 1;
        }
    } while((midFactor - minFactor) > 1);
    
    uint32_t diff[3] = {0, 0, 0};
    uint32_t tmpFactor[3] = {minFactor , midFactor, maxFactor};
    uint32_t minDiff = 0xFFFFFFFF;
    uint32_t targetFactor = 0;
    for(uint32_t i = 0; i < 3; i++)
    {
        //set 3318 parameter
        tempreg = RCC->PMU_CTL.reg_u32val;
        ((union _PMU_CTL *)(&tempreg))->bit.vtr3318 = tmpFactor[i];
        RCC->PMU_CTL.reg_u32val = tempreg;
        
        for(uint32_t j = 0; j < 1000; j++)
        {
            __NOP();
        }        
        for(uint32_t j = 0; j < TEST_ADC_ARRAY_COUNT; j++)
        {
            adc_get_array[j] = adc_get_value(15);//adjust 3318 ldo by adc channel_15
        }
        bubble_sort_u16(adc_get_array, TEST_ADC_ARRAY_COUNT);
        diff[i] = (adc_get_array[TEST_ADC_ARRAY_COUNT/2] > targetAdcVal) ? 
                  (adc_get_array[TEST_ADC_ARRAY_COUNT/2] - targetAdcVal) : 
                  (targetAdcVal - adc_get_array[TEST_ADC_ARRAY_COUNT/2]);
        if(diff[i] < minDiff)
        {
            minDiff = diff[i];
            targetFactor = tmpFactor[i];
        }
    }
    //set 3318 parameter
    tempreg = RCC->PMU_CTL.reg_u32val;
    ((union _PMU_CTL *)(&tempreg))->bit.vtr3318 = targetFactor;
    RCC->PMU_CTL.reg_u32val = tempreg;
    if(minDiff > threshold) {
        ret = HAL_ERROR;
    }
    return ret;
}
#endif /* HAL_PMU_MODULE_ENABLED */