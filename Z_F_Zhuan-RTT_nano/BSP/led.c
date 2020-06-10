/************************************************************************************************************************************************************************
** 版权：   2020-2030, 深圳市信为科技发展有限公司
** 文件名:  led.c
** 作者:    weijianx
** 版本:    V1.0.0
** 日期:    20120-06-09
** 描述:    
** 功能:    led灯的配置 
*************************************************************************************************************************************************************************
** 修改者:      No
** 版本:  		
** 修改内容:    No 
** 日期:        No
*************************************************************************************************************************************************************************/


#include "led.h"


#define LED_PORT        GPIOA
#define LED_GREEN_PIN   GPIO_PIN_6      // 红灯 PA6
#define LED_RED_PIN     GPIO_PIN_5      //绿灯PA5

/**
* 名称       : Led_Control()
* 创建日期   : 2020-06-09
* 作者       : weijianx
* 功能       : led的控制
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Led_Control(uint8_t color)  // 1 绿色  0红色
{       
	if(color)  //1 绿色
	{
		HAL_GPIO_WritePin(LED_PORT,LED_RED_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_PORT,LED_GREEN_PIN,GPIO_PIN_SET);

		//    LED_PORT->BRR = LED_RED_PIN; 
		//    LED_PORT->BSRR = LED_GREEN_PIN;   
	}
	else      // 0  红色
	{     
		 HAL_GPIO_WritePin(LED_PORT,LED_GREEN_PIN,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(LED_PORT,LED_RED_PIN,GPIO_PIN_SET);
		//      LED_PORT->BRR = LED_GREEN_PIN; 
		//      LED_PORT->BSRR = LED_RED_PIN;  
	}
}


/**
* 名称       : GPIO_Configuration()
* 创建日期   : 2020-06-09
* 作者       : weijianx
* 功能       : led引脚配置
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : PA9 */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pin : PA5 PA6 */
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      
}
