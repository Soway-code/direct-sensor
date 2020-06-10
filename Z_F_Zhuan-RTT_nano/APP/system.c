/************************************************************************************************************************************************************************
** 版权：   2020-2030, 深圳市信为科技发展有限公司
** 文件名:  system.c
** 作者:    weijianx
** 版本:    V1.0.0
** 日期:    20120-06-09
** 描述:    
** 功能:    系统初始化
*************************************************************************************************************************************************************************
** 修改者:      No
** 版本:  		
** 修改内容:    No 
** 日期:        No
*************************************************************************************************************************************************************************/



#include "system.h"
#include "config.h"


IWDG_HandleTypeDef  IWDG_HandleStructure;

///*
//系统默认时钟为MSI分频为2M，此处配置时钟为HSI16M(L031的时钟最大为16M)
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
// 名称         : User_Iwdg_Init()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : 看门狗配置
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************
void User_Iwdg_Init(void)
{
    IWDG_HandleStructure.Init.Prescaler = IWDG_PRESCALER_8;                     //LSI的32.768的8分频位4.096K
    IWDG_HandleStructure.Init.Reload = 0x0FA0;                                  //重装载值为4000，约1s
    IWDG_HandleStructure.Init.Window = 0x0FA0;
    IWDG_HandleStructure.Instance = IWDG;
    HAL_IWDG_Init(&IWDG_HandleStructure);
}


//******************************************************************************
// 名称         : User_Iwdg_Feed()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : 喂狗
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************
void User_Iwdg_Feed(void)
{
    HAL_IWDG_Refresh(&IWDG_HandleStructure);
}

/**
* 名称       : system_Init()
* 创建日期   : 2020-06-09
* 作者       : weijianx
* 功能       : 设备初始化
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void system_Init(void)
{
	SystemClock_Config();                        //时钟配置为HSI 16M
	
	GPIO_Configuration();
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  	 //关闭滴答时钟
	
	Led_Control(1);  //上电绿色
	ReadPara();                                  //读取EEPROM参数
    Uart_Config_Init();                          //串口配置
    //TIM2_Init();                               //PA0读数 脉冲读数
    
    TIM21_ETR_Init(); 							 //PA0读数 脉冲读数
    TIM22_Init();                                //100ms定时器
    
    User_Iwdg_Init();     

	Led_Control(1);  							 //上电绿色

}




