#include "adc.h"
#include "stm32l0xx.h"


//******************************************************************************
// 名称         : Adc_Init()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : ADC配置
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************
void Adc_Init(void)
{ 
    uint32_t Timeout =0xffff;
    
    ADC_PORT->MODER |= (3 << 2);                                                //PA1模拟输入模式
    
    __HAL_RCC_ADC1_CLK_ENABLE();                                                //打开ADC1时钟 
    
    ADC1->CFGR2 &= ~(ADC_CFGR2_CKMODE); 
    
    ADC1->CFGR2 |= ADC_CLOCK_SYNC_PCLK_DIV4;
    
    ADC1->CFGR1 &= ~( ADC_CFGR1_RES); 

    ADC->CCR &= (uint32_t)~ADC_CCR_LFMEN;

    if (HAL_IS_BIT_CLR(ADC1->CR, ADC_CR_ADVREGEN))
    {
        ADC1->CR |= ADC_CR_ADVREGEN;
    }
    
    ADC1->CFGR1 &= ~(ADC_CFGR1_ALIGN | ADC_CFGR1_SCANDIR | ADC_CFGR1_EXTSEL | ADC_CFGR1_EXTEN | ADC_CFGR1_CONT |
                     ADC_CFGR1_DMACFG| ADC_CFGR1_OVRMOD  | ADC_CFGR1_AUTDLY | ADC_CFGR1_AUTOFF| ADC_CFGR1_DISCEN);
    
    ADC1->CFGR1 |= (ADC_DATAALIGN_RIGHT | (1 << 13) | ADC_OVR_DATA_OVERWRITTEN );
    
    if(HAL_IS_BIT_SET(ADC1->CFGR2, ADC_CFGR2_OVSE))
    {
        ADC1->CFGR2 &= ~ADC_CFGR2_OVSE;
    }  
    ADC1->SMPR &= (uint32_t)(~ADC_SMPR_SMPR);
    
    ADC1->SMPR |= ADC_SAMPLETIME_160CYCLES_5;
    
    ADC1->CHSELR |= (uint32_t)(ADC_CHANNEL_1 & ADC_CHANNEL_MASK);
    
    ADC->CCR |= ADC_CCR_VREFEN; 
    
    ADC1->CR |= ADC_CR_ADCAL;                                                   //校准
    
    while(HAL_IS_BIT_SET(ADC1->CR, ADC_CR_ADCAL) && Timeout > 0)                //等待校准完成
    {
        Timeout--;
    }
    
    ADC1->CR |= ADC_CR_ADEN;                                                    //使能
    
    ADC1->CR |= ADC_CR_ADSTART;                                                 //开始转换
}


//******************************************************************************
// 名称         : Get_Adc_Vol()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : 获取电压值
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 电压值
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************
uint16_t Get_Adc_Vol(void)
{
    uint16_t AD_Value;
        
    uint32_t AD_Vol;
        
    AD_Value =  ADC1->DR;
    
    AD_Vol = AD_Value * 33000 / 4095 ;                                      //扩大10000倍，+50进行四舍五入
    
    return (uint16_t)(AD_Vol );/// 100);                                           //再缩小100倍
}


