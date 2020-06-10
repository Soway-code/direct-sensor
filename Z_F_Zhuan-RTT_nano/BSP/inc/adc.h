#ifndef _ADC_H
#define _ADC_H


#define ADC_PORT        GPIOA
#define ADC_PIN         GPIO_PIN_1

#include "stm32l0xx.h"

void Adc_Init(void);
uint16_t Get_Adc_Vol(void);

#endif


