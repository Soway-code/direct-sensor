#include "adc.h"
#include "stm32l0xx.h"


//******************************************************************************
// ����         : Adc_Init()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ADC����
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
void Adc_Init(void)
{ 
    uint32_t Timeout =0xffff;
    
    ADC_PORT->MODER |= (3 << 2);                                                //PA1ģ������ģʽ
    
    __HAL_RCC_ADC1_CLK_ENABLE();                                                //��ADC1ʱ�� 
    
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
    
    ADC1->CR |= ADC_CR_ADCAL;                                                   //У׼
    
    while(HAL_IS_BIT_SET(ADC1->CR, ADC_CR_ADCAL) && Timeout > 0)                //�ȴ�У׼���
    {
        Timeout--;
    }
    
    ADC1->CR |= ADC_CR_ADEN;                                                    //ʹ��
    
    ADC1->CR |= ADC_CR_ADSTART;                                                 //��ʼת��
}


//******************************************************************************
// ����         : Get_Adc_Vol()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ��ȡ��ѹֵ
// �������     : ��
// �������     : ��
// ���ؽ��     : ��ѹֵ
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
uint16_t Get_Adc_Vol(void)
{
    uint16_t AD_Value;
        
    uint32_t AD_Vol;
        
    AD_Value =  ADC1->DR;
    
    AD_Vol = AD_Value * 33000 / 4095 ;                                      //����10000����+50������������
    
    return (uint16_t)(AD_Vol );/// 100);                                           //����С100��
}


