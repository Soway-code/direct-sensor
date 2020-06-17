#include "tim.h"
#include "type.h"

extern uint8_t SendLen;
extern uint8_t SendBuf[100];
TIM_HandleTypeDef    TimHandle;

TIM_HandleTypeDef TIM22_Handler;      //��ʱ��TIM22��� 

BitAction  PulseFlag = Bit_RESET;                                         //��ʼװ�������־
uint16_t Pulse100msCntBuf[3] = {0};                                     //ÿ100ms�������������
extern UserTypeDef UserPara;


//******************************************************************************
// ����         : TIM2_Init()    PA0����
// ��������     : 2018-06-08
// ����         : MMX
// ����         : TIM2��ETR����
// �������     : ��  
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
/*
void TIM2_Init(void)
{
    uint32_t tmpcr1 = 0U;
    uint32_t tmpsmcr = 0U;
    
    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);                                   //GPIOAʱ�ӿ���
    SET_BIT(RCC->APB1ENR, (RCC_APB1ENR_TIM2EN));                                //TIM2ʱ�ӿ���
    GPIOA->MODER &= 0xFFFFFFFC;                                                 //PA0���ù���                                
    GPIOA->MODER |= 0x00000002;      
    GPIOA->OTYPER &= ~(0x1);                                                    //�������
    GPIOA->OSPEEDR |= 0x3;
    GPIOA->PUPDR &= 0xFFFFFFFC;                                                 //��Ҫ����Ϊ��������  �����Ƶ��׼ 
    GPIOA->PUPDR |= 0x00000000;     // 00: No pull-up, pull-down     01: Pull-up     10: Pull-down
    GPIOA->AFR[0] &= 0xFFFFFFF0;                                                //AF5 TIM2_ETR
    GPIOA->AFR[0] |= 0x00000005;                                                //AFR[0]��ӦPIN0~7,AFR[1]��ӦPIN8~15
    
    tmpcr1 = TIM2->CR1;                                                         //TIM2 Base Init
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= TIM_COUNTERMODE_UP;
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= 0x00;
    TIM2->CR1 = tmpcr1;
    TIM2->ARR = 0xFFFF;
    TIM2->PSC = 0x00;
    TIM2->EGR = TIM_EGR_UG;    
    
    tmpsmcr = TIM2->SMCR;                                                       //TIM2 ETR����
    tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);
    tmpsmcr |= (uint32_t)(TIM_ETRPRESCALER_DIV1 | (TIM_ETRPOLARITY_NONINVERTED | (0x0F << 8)));
    TIM2->SMCR = tmpsmcr;
    TIM2->SMCR |= TIM_SMCR_ECE;                                                 //ʹ���ⲿʱ��ģʽ2������Ҫ������
    
    
   // __HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
    TIM2->CNT = 0;                                                              //����ֵ��0
    TIM2->CR1|= TIM_CR1_CEN;                                                    //ʹ��TIM2
}
*/

//******************************************************************************
// ����         : TIM21_ETR_Init()   PA1���á�
// ��������     : 2020-04-03
// ����         : �����
// ����         : TIM21�ļ���������
// �������     : ��  
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
// ******************************************************************************
//
void TIM21_ETR_Init(void)   //�������������
{
    uint32_t tmpcr1 = 0U;
    uint32_t tmpsmcr = 0U;

    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);                                   //GPIOAʱ�ӿ���
    SET_BIT(RCC->APB2ENR, (RCC_APB2ENR_TIM21EN));                                //TIM21ʱ�ӿ���
   /*
    GPIOA->MODER &= 0xFFCFFFFF;                                                 //PA10���ù���                                
    GPIOA->MODER |= 0x00200000;//10: Alternate function mode      
    GPIOA->OTYPER &= ~(1<<10);                                                    //�������
    GPIOA->OSPEEDR |= (3<<20);            //���� 
    GPIOA->PUPDR &= 0xFFCFFFFF;                                                 //���� 
    GPIOA->PUPDR |= 0x00200000; 
    GPIOA->AFR[1] &= 0xFFFFFFF0;                                                //AF5 TIM22_CH2
    GPIOA->AFR[1] |= 0x00000005;                                                //AFR[0]��ӦPIN0~7,AFR[1]��ӦPIN8~15
    */  
   
    GPIOA->MODER &= 0xFFFFFFF3;                                                 //PA1���ù���                                
    GPIOA->MODER |= 0x00000008;      
    GPIOA->OTYPER &= ~(0x1<<1);                                                    //�������
    GPIOA->OSPEEDR |= (0x3<<2);
    GPIOA->PUPDR &= 0xFFFFFFF3;  //��Ҫ����Ϊ��������  �����Ƶ��׼                                             
    GPIOA->PUPDR |= 0x00000000; // 00: No pull-up, pull-down     01: Pull-up     10: Pull-down
    GPIOA->AFR[0] &= 0xFFFFFF0F;                                                //AF5 TIM21_ETR
    GPIOA->AFR[0] |= 0x00000050;                                                //AFR[0]��ӦPIN0~7,AFR[1]��ӦPIN8~15
    
    tmpcr1 = TIM21->CR1;                                                         //TIM21 Base Init
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= TIM_COUNTERMODE_UP;
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= 0x00;
    TIM21->CR1 = tmpcr1;
    TIM21->ARR = 0xFFFF;
    TIM21->PSC = 0x00;
    TIM21->EGR = TIM_EGR_UG;    
    
    tmpsmcr = TIM21->SMCR;                                                       //TIM21 ETR����
    tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);
    tmpsmcr |= (uint32_t)(TIM_ETRPRESCALER_DIV1 | (TIM_ETRPOLARITY_NONINVERTED | (0x0F << 8)));
    TIM21->SMCR = tmpsmcr;
    TIM21->SMCR |= TIM_SMCR_ECE;                                                 //ʹ���ⲿʱ��ģʽ2������Ҫ������
    
    
    TIM21->CNT = 0;                                                              //����ֵ��0
    TIM21->CR1|= TIM_CR1_CEN;                                                    //ʹ��TIM21
}


void TIM22_Init(void)            //��ʱ1s
{
    TIM22_Handler.Instance = TIM22;
    TIM22_Handler.Init.Prescaler = 1599;
    TIM22_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM22_Handler.Init.Period = 9999;
    TIM22_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM22_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM22_Handler); //ʹ�ܶ�ʱ��22�Ͷ�ʱ��22�����жϣ�TIM_IT_UPDATE   
    
}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM22)
    {
        __HAL_RCC_TIM22_CLK_ENABLE();            //ʹ��TIM22ʱ��
        HAL_NVIC_SetPriority(TIM22_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�3
        HAL_NVIC_EnableIRQ(TIM22_IRQn);          //����ITM22�ж�   
    }
}



//��ʱ��3�жϷ�����
void TIM22_IRQHandler(void)
{  
    HAL_TIM_IRQHandler(&TIM22_Handler);
}

uint16_t tim_cnt = 0;   //��ʱ���ۼ�
uint16_t tim_1min_cnt = 0;   //1minʱ���ۼ�

uint8_t Time_1s_flag = 0;
uint8_t Time_5s_flag = 0;
uint8_t Time_1min_flag = 0;

extern uint16_t Current_pulse;  //��ǰ������
 

//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{     
       uint16_t read_pulse = 0;  //��ǰ������
      
       //TIM22->SR = 0xFE;
      if(htim==(&TIM22_Handler))
      {
            tim_cnt++;//
 
            tim_1min_cnt += 1;   //��һ��Ϊ1s

            read_pulse = TIM21->CNT;    //��ȡ��ǰ��������
            TIM21->CNT = 0;      
           
         
          UserPara.TotalPulse += read_pulse;  //��������        
            
          if(read_pulse)
          {            
            PulseFlag = Bit_SET;  //������   ת����
            Time_1min_flag = 0;
          }
            
          Time_1min_flag++;
          
          if((Time_1min_flag > 59) && (read_pulse == 0))
          {
            PulseFlag = Bit_RESET; //������  ͣת
          }
          
          if(!((tim_cnt+1)%1))  //1s
          {
            Time_1s_flag = 1;
     
          }
          if(!((tim_cnt +1)%5))//5s��ʱ
          {
              Time_5s_flag = 1;
          }
          
          if(!((tim_cnt +1)%20))//20s��ʱ
          {
              UserPara.RotateSpeed = (UserPara.TotalPulse - Current_pulse) * 3 ;  //������ת�ٶ�  1s������*60  HZ/min  ��λ��תÿ��
              Current_pulse = UserPara.TotalPulse;              
          }
      }
}
 
