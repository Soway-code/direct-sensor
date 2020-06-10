/************************************************************************************************************************************************************************
** ��Ȩ��   2020-2030, ��������Ϊ�Ƽ���չ���޹�˾
** �ļ���:  system.c
** ����:    weijianx
** �汾:    V1.0.0
** ����:    20120-06-09
** ����:    
** ����:    ϵͳ��ʼ��
*************************************************************************************************************************************************************************
** �޸���:      No
** �汾:  		
** �޸�����:    No 
** ����:        No
*************************************************************************************************************************************************************************/



#include "system.h"
#include "config.h"


IWDG_HandleTypeDef  IWDG_HandleStructure;

///*
//ϵͳĬ��ʱ��ΪMSI��ƵΪ2M���˴�����ʱ��ΪHSI16M(L031��ʱ�����Ϊ16M)
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __HAL_RCC_HSI_CONFIG(RCC_HSI_ON);
    __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_HSICALIBRATION_DEFAULT);
    
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;//RCC_SYSCLK_DIV4;                          
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}
//*/


//******************************************************************************
// ����         : User_Iwdg_Init()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ���Ź�����
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
void User_Iwdg_Init(void)
{
    IWDG_HandleStructure.Init.Prescaler = IWDG_PRESCALER_8;                     //LSI��32.768��8��Ƶλ4.096K
    IWDG_HandleStructure.Init.Reload = 0x0FA0;                                  //��װ��ֵΪ4000��Լ1s
    IWDG_HandleStructure.Init.Window = 0x0FA0;
    IWDG_HandleStructure.Instance = IWDG;
    HAL_IWDG_Init(&IWDG_HandleStructure);
}


//******************************************************************************
// ����         : User_Iwdg_Feed()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ι��
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
void User_Iwdg_Feed(void)
{
    HAL_IWDG_Refresh(&IWDG_HandleStructure);
}

/**
* ����       : system_Init()
* ��������   : 2020-06-09
* ����       : weijianx
* ����       : �豸��ʼ��
* �������   : ��
* �������   : ��
* ���ؽ��   : ��
* ע���˵�� : 
* �޸�����   :
*/
void system_Init(void)
{
	SystemClock_Config();                        //ʱ������ΪHSI 16M
	
	GPIO_Configuration();
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  	 //�رյδ�ʱ��
	
	Led_Control(1);  //�ϵ���ɫ
	ReadPara();                                  //��ȡEEPROM����
    Uart_Config_Init();                          //��������
    //TIM2_Init();                               //PA0���� �������
    
    TIM21_ETR_Init(); 							 //PA0���� �������
    TIM22_Init();                                //100ms��ʱ��
    
    User_Iwdg_Init();     

	Led_Control(1);  							 //�ϵ���ɫ

}




