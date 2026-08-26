//
// Created by asus on 2025/3/29.
//

#include "bsp_adc.h"


uint16_t adc_value;
uint16_t adc_value_dma[10];
uint8_t adc_dma_i;
int32_t adc_value_dma_sum;
int16_t adc_value_dma_avg;

//连续模式初始化adc
void bsp_adc_init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start(&hadc1);
}

int16_t bsp_adc_read(void)
{
    HAL_ADC_PollForConversion(&hadc1,50);
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        adc_value = HAL_ADC_GetValue(&hadc1);
    return 2047 - adc_value;
}

void bsp_adc_dma_init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adc_value_dma, 10);
}

int16_t bsp_adc_dma_read(void)
{
    for(adc_dma_i = 0; adc_dma_i < 10; adc_dma_i++)
    {
        adc_value_dma_sum += adc_value_dma[adc_dma_i];
    }
    adc_value_dma_avg = adc_value_dma_sum / 10;
    adc_value_dma_sum = 0;
    return 2046 - adc_value_dma_avg;
}