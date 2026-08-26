//
// Created by asus on 2025/3/29.
//

#ifndef C8T6_BSP_ADC_H
#define C8T6_BSP_ADC_H
#include "bsp_def.h"

void bsp_adc_init(void);
int16_t bsp_adc_read(void);
void bsp_adc_dma_init(void);
int16_t bsp_adc_dma_read(void);

#endif //C8T6_BSP_ADC_H
