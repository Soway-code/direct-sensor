/************************************************************************************************************************************************************************
** ��Ȩ��   2020-2030, ��������Ϊ�Ƽ���չ���޹�˾
** �ļ���:  led.c
** ����:    weijianx
** �汾:    V1.0.0
** ����:    20120-06-09
** ����:    
** ����:    led�Ƶ����� 
*************************************************************************************************************************************************************************
** �޸���:      No
** �汾:  		
** �޸�����:    No 
** ����:        No
*************************************************************************************************************************************************************************/


#include "led.h"


#define LED_PORT        GPIOA
#define LED_GREEN_PIN   GPIO_PIN_6      // ��� PA6
#define LED_RED_PIN     GPIO_PIN_5      //�̵�PA5

/**
* ����       : Led_Control()
* ��������   : 2020-06-09
* ����       : weijianx
* ����       : led�Ŀ���
* �������   : ��
* �������   : ��
* ���ؽ��   : ��
* ע���˵�� : 
* �޸�����   :
*/
void Led_Control(uint8_t color)  // 1 ��ɫ  0��ɫ
{       
	if(color)  //1 ��ɫ
	{
		HAL_GPIO_WritePin(LED_PORT,LED_RED_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_PORT,LED_GREEN_PIN,GPIO_PIN_SET);

		//    LED_PORT->BRR = LED_RED_PIN; 
		//    LED_PORT->BSRR = LED_GREEN_PIN;   
	}
	else      // 0  ��ɫ
	{     
		 HAL_GPIO_WritePin(LED_PORT,LED_GREEN_PIN,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(LED_PORT,LED_RED_PIN,GPIO_PIN_SET);
		//      LED_PORT->BRR = LED_GREEN_PIN; 
		//      LED_PORT->BSRR = LED_RED_PIN;  
	}
}


/**
* ����       : GPIO_Configuration()
* ��������   : 2020-06-09
* ����       : weijianx
* ����       : led��������
* �������   : ��
* �������   : ��
* ���ؽ��   : ��
* ע���˵�� : 
* �޸�����   :
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
