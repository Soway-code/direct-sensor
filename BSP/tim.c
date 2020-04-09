#include "tim.h"
#include "type.h"

extern uint8_t SendLen;
extern uint8_t SendBuf[100];
TIM_HandleTypeDef    TimHandle;

TIM_HandleTypeDef TIM22_Handler;      //定时器TIM22句柄 

BitAction  PulseFlag = Bit_RESET;                                         //开始装填数组标志
uint16_t Pulse100msCntBuf[3] = 0;                                     //每100ms秒脉冲个数数组
extern UserTypeDef UserPara;


/*//******************************************************************************
// 名称         : TIM2_Init()    PA0可用
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : TIM2的ETR配置
// 输入参数     : 无  
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************

void TIM2_Init(void)
{
    uint32_t tmpcr1 = 0U;
    uint32_t tmpsmcr = 0U;
    
    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);                                   //GPIOA时钟开启
    SET_BIT(RCC->APB1ENR, (RCC_APB1ENR_TIM2EN));                                //TIM2时钟开启
    GPIOA->MODER &= 0xFFFFFFFC;                                                 //PA0复用功能                                
    GPIOA->MODER |= 0x00000002;      
    GPIOA->OTYPER &= ~(0x1);                                                    //推挽输出
    GPIOA->OSPEEDR |= 0x3;
    GPIOA->PUPDR &= 0xFFFFFFFC;                                                 //需要配置为浮空输入  否则低频不准 
    GPIOA->PUPDR |= 0x00000000;     // 00: No pull-up, pull-down     01: Pull-up     10: Pull-down
    GPIOA->AFR[0] &= 0xFFFFFFF0;                                                //AF5 TIM2_ETR
    GPIOA->AFR[0] |= 0x00000005;                                                //AFR[0]对应PIN0~7,AFR[1]对应PIN8~15
    
    tmpcr1 = TIM2->CR1;                                                         //TIM2 Base Init
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= TIM_COUNTERMODE_UP;
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= 0x00;
    TIM2->CR1 = tmpcr1;
    TIM2->ARR = 0xFFFF;
    TIM2->PSC = 0x00;
    TIM2->EGR = TIM_EGR_UG;    
    
    tmpsmcr = TIM2->SMCR;                                                       //TIM2 ETR配置
    tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);
    tmpsmcr |= (uint32_t)(TIM_ETRPRESCALER_DIV1 | (TIM_ETRPOLARITY_NONINVERTED | (0x0F << 8)));
    TIM2->SMCR = tmpsmcr;
    TIM2->SMCR |= TIM_SMCR_ECE;                                                 //使能外部时钟模式2（必须要开启）
    
    
   // __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
    TIM2->CNT = 0;                                                              //计数值置0
    TIM2->CR1|= TIM_CR1_CEN;                                                    //使能TIM2
}
*/
//******************************************************************************
// 名称         : TIM21_ETR_Init()   PA1可用。
// 创建日期     : 2020-04-03
// 作者         : 李红良
// 功能         : TIM21的计数器配置
// 输入参数     : 无  
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
// ******************************************************************************
//
void TIM21_ETR_Init(void)   //脉冲计数器配置
{
    uint32_t tmpcr1 = 0U;
    uint32_t tmpsmcr = 0U;

    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);                                   //GPIOA时钟开启
    SET_BIT(RCC->APB2ENR, (RCC_APB2ENR_TIM21EN));                                //TIM21时钟开启
   /*
    GPIOA->MODER &= 0xFFCFFFFF;                                                 //PA10复用功能                                
    GPIOA->MODER |= 0x00200000;//10: Alternate function mode      
    GPIOA->OTYPER &= ~(1<<10);                                                    //推挽输出
    GPIOA->OSPEEDR |= (3<<20);            //高速 
    GPIOA->PUPDR &= 0xFFCFFFFF;                                                 //下拉 
    GPIOA->PUPDR |= 0x00200000; 
    GPIOA->AFR[1] &= 0xFFFFFFF0;                                                //AF5 TIM22_CH2
    GPIOA->AFR[1] |= 0x00000005;                                                //AFR[0]对应PIN0~7,AFR[1]对应PIN8~15
    */  
   
    GPIOA->MODER &= 0xFFFFFFF3;                                                 //PA1复用功能                                
    GPIOA->MODER |= 0x00000008;      
    GPIOA->OTYPER &= ~(0x1<<1);                                                    //推挽输出
    GPIOA->OSPEEDR |= (0x3<<2);
    GPIOA->PUPDR &= 0xFFFFFFF3;  //需要配置为浮空输入  否则低频不准                                             
    GPIOA->PUPDR |= 0x00000000; // 00: No pull-up, pull-down     01: Pull-up     10: Pull-down
    GPIOA->AFR[0] &= 0xFFFFFF0F;                                                //AF5 TIM21_ETR
    GPIOA->AFR[0] |= 0x00000050;                                                //AFR[0]对应PIN0~7,AFR[1]对应PIN8~15
    
    tmpcr1 = TIM21->CR1;                                                         //TIM21 Base Init
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= TIM_COUNTERMODE_UP;
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= 0x00;
    TIM21->CR1 = tmpcr1;
    TIM21->ARR = 0xFFFF;
    TIM21->PSC = 0x00;
    TIM21->EGR = TIM_EGR_UG;    
    
    tmpsmcr = TIM21->SMCR;                                                       //TIM21 ETR配置
    tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);
    tmpsmcr |= (uint32_t)(TIM_ETRPRESCALER_DIV1 | (TIM_ETRPOLARITY_NONINVERTED | (0x0F << 8)));
    TIM21->SMCR = tmpsmcr;
    TIM21->SMCR |= TIM_SMCR_ECE;                                                 //使能外部时钟模式2（必须要开启）
    
    
    TIM21->CNT = 0;                                                              //计数值置0
    TIM21->CR1|= TIM_CR1_CEN;                                                    //使能TIM21
}


void TIM22_Init(void)            //定时1s
{
    TIM22_Handler.Instance = TIM22;
    TIM22_Handler.Init.Prescaler = 1599;
    TIM22_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM22_Handler.Init.Period = 9999;
    TIM22_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM22_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM22_Handler); //使能定时器22和定时器22更新中断：TIM_IT_UPDATE   
    
}


//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM22)
    {
        __HAL_RCC_TIM22_CLK_ENABLE();            //使能TIM22时钟
        HAL_NVIC_SetPriority(TIM22_IRQn,1,3);    //设置中断优先级，抢占优先级2，子优先级3
        HAL_NVIC_EnableIRQ(TIM22_IRQn);          //开启ITM22中断   
    }
}



//定时器3中断服务函数
void TIM22_IRQHandler(void)
{  
    HAL_TIM_IRQHandler(&TIM22_Handler);
}

uint16_t tim_cnt = 0;   //时间累加
uint8_t Time_1s_flag = 0;
uint8_t Time_5s_flag = 0;
//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{     
       uint16_t read_pulse = 0;  //当前脉冲数
      
       //TIM22->SR = 0xFE;
      if(htim==(&TIM22_Handler))
      {
            tim_cnt++;//
 
//          if(UserPara.DirSta)  
//          {
            UserPara.Duration += 1;   //加一次为0.1s
//          }
          read_pulse = TIM21->CNT;    //读取当前脉冲数据
          TIM21->CNT = 0;
          
          
         UserPara.RotateSpeed = read_pulse * 60;  //计算旋转速度  1s脉冲数*60  HZ/min  单位：转每分
          UserPara.TotalPulse += read_pulse;  //计总脉冲
          
//          memcpy((uint8_t*)Pulse100msCntBuf, (uint8_t*)Pulse100msCntBuf + 2, 4);  //fifo
//          *(Pulse100msCntBuf + 2) = read_pulse;
         
          
          if(read_pulse)
          {
            PulseFlag = Bit_SET;  //有脉冲
          }
          else
          {
            PulseFlag = Bit_RESET; //无脉冲
          }
             
          
          if(!((tim_cnt+1)%1))  //1s
          {
            Time_1s_flag = 1;
     
          }
          if(!((tim_cnt +1)%5))//5s计时
          {
              Time_5s_flag = 1;
          }
      }
      
}
 
