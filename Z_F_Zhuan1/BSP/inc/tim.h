#ifndef _TIM_H
#define _TIM_H


#include "stm32l0xx.h"
#include "stm32l031xx.h"

//void TIM2_Init(void);
void TIM21_ETR_Init(void);   //�������������
//void TIM22_CH2_Init(void);   //�������������
void TIM22_Init(void);            //��ʱ100ms
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif