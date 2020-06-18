/**
  ******************************************************************************
  * File Name          : RNG.c
  * Description        : This file provides code for the configuration
  *                      of the RNG instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rng.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RNG_HandleTypeDef hrng;


#define   FILTER_A   1

int filter_buf[FILTER_N] = {0};

int i ;


/* RNG init function */
void MX_RNG_Init(void)
{

	hrng.Instance = RNG;
	hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
	if (HAL_RNG_Init(&hrng) != HAL_OK)
	{
	Error_Handler();
	}
	
	filter_buf[FILTER_N - 2] = 300;
	
}

void HAL_RNG_MspInit(RNG_HandleTypeDef* rngHandle)
{

  if(rngHandle->Instance==RNG)
  {
  /* USER CODE BEGIN RNG_MspInit 0 */

  /* USER CODE END RNG_MspInit 0 */
    /* RNG clock enable */
    __HAL_RCC_RNG_CLK_ENABLE();
  /* USER CODE BEGIN RNG_MspInit 1 */

  /* USER CODE END RNG_MspInit 1 */
  }
}

void HAL_RNG_MspDeInit(RNG_HandleTypeDef* rngHandle)
{

  if(rngHandle->Instance==RNG)
  {
  /* USER CODE BEGIN RNG_MspDeInit 0 */

  /* USER CODE END RNG_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RNG_CLK_DISABLE();
  /* USER CODE BEGIN RNG_MspDeInit 1 */

  /* USER CODE END RNG_MspDeInit 1 */
  }
} 

/**
* @brief       得到随机数
* @param    	None
* @return       获取到的随机数
*/
uint32_t RNG_Get_RandomNum(void)
{
	uint32_t randomnum;
	
    HAL_RNG_GenerateRandomNumber(&hrng,&randomnum);
	return randomnum;
}

/**
* @brief       生成[min,max]范围的随机数
* @param        min: 最小值    max: 最大值
* @return       随机数
*/
int RNG_Get_RandomRange(int min,int max)
{ 
	uint32_t randomnum;
	
	HAL_RNG_GenerateRandomNumber(&hrng,&randomnum);
	return randomnum%(max-min+1)+min;
}


/**
* @brief   限幅平均滤波法
* @brief   在设定的幅度内，求平均值
* @retval  限幅平均值
*/
int Filter(void) 
{
	int filter_sum = 0;
	
	filter_buf[FILTER_N - 1] = RNG_Get_RandomRange(295, 305);

	if((filter_buf[FILTER_N - 1] - filter_buf[FILTER_N - 2] > FILTER_A) || (filter_buf[FILTER_N - 2] - filter_buf[FILTER_N - 1] > FILTER_A))
	{
		filter_buf[FILTER_N - 1] = filter_buf[FILTER_N - 2];
		
	}
	
	for(i = 0; i < FILTER_N - 1; i++) 
	{
		filter_buf[i] = filter_buf[i + 1];
		filter_sum += filter_buf[i];
	}
	
    
	return (int)(filter_sum / (FILTER_N - 1));

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
