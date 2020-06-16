/************************************************************************************************************************************************************************
** 版权：   2018-2028, 深圳市信为科技发展有限公司
** 文件名:  main.c
** 作者:    MMX
** 版本:    V1.0.0
** 日期:    2018-06-08
** 描述:    
** 功能:    利用在不同状态(停车 怠速 工作)下电压不同来判断车辆的运行状态     
*************************************************************************************************************************************************************************
** 修改者:      No
** 版本:  		
** 修改内容:    No 
** 日期:        No
*************************************************************************************************************************************************************************/

#include "eeprom.h"
#include "usart.h"
//#include "type.h"
#include "modbus_asc.h"
#include "main.h"
#include "para.h"
#include "tim.h"
#include "string.h"
//#include "adc.h"


//BitAction StartFillBufFlag = Bit_RESET;                                         //开始装填数组标志
uint16_t ADBUF[AD_BUF_MAX];                                     //采集AD值缓冲数组  10个数
float  LiquidHeight = 0;
uint16_t LiquidUnit = 0;

extern uint8_t StartFlag;
extern uint8_t StartCountFlag;
extern BitAction  PulseFlag;
IWDG_HandleTypeDef  IWDG_HandleStructure;


extern UserTypeDef UserPara;
 
u8 LFilCnt = 0;
u8 time_tick = 10;  //默认滤波方式  为平稳滤波

   // uint32_t systemClock;
    //uint32_t HclkFre;
    //uint32_t Pclk1Fre;
    //uint32_t Pclk2Fre;
extern uint8_t Time_1s_flag;
extern uint8_t Time_5s_flag;
extern uint16_t Pulse100msCntBuf[3];
extern uint16_t tim_cnt;   //定时1S 时间累加

extern uint16_t Current_PositiveTime ;  //当前正转脉冲数
extern uint16_t Current_NegativeTime ;  //当前反转脉冲数
extern uint16_t tim_1min_cnt; //1min时间累加


#define LED_PORT        GPIOA
#define LED_GREEN_PIN   GPIO_PIN_6      // 红灯 PA6
#define LED_RED_PIN     GPIO_PIN_5      //绿灯PA5


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

//GPIO配置
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

 uint8_t zhengfanzhuan; 
//******************************************************************************
// 名称         : main()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : main函数
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************
void main(void)
{ 
    //uint8_t i;   
    uint8_t Z_F_Zhuan;     
    uint8_t uTemp[4];
    
    SystemClock_Config();                                                       //时钟配置为HSI 16M
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  //关闭滴答时钟
    GPIO_Configuration();
     
    Led_Control(1);  //上电绿色
    // systemClock = HAL_RCC_GetSysClockFreq();
    //HclkFre = HAL_RCC_GetHCLKFreq();
    //Pclk1Fre = HAL_RCC_GetPCLK1Freq();
    //Pclk2Fre = HAL_RCC_GetPCLK2Freq();
  
    ReadPara();                                                                 //读取EEPROM参数
    Uart_Config_Init();                                                         //串口配置
    //TIM2_Init();                                //PA0读数 脉冲读数
    
    TIM21_ETR_Init(); //PA0读数 脉冲读数
    TIM22_Init();                                //100ms定时器
    
    User_Iwdg_Init();                                                           //看门狗配置
    Led_Control(1);  //上电绿色
    
    while(1)
    {         
       
        User_Iwdg_Feed();                                                   //喂狗
        
        MBASC_Function();                                                   //MODBUS处理
     
        
    
       if(Time_5s_flag)  //10s 时间到 更新 数据  
       {
         
          
         Time_5s_flag = 0;
                        
         long32Array(UserPara.TotalPulse, uTemp);                               // 更新  总脉冲数     单位：HZ   
         Eeprom_WriteNBytes(PULSE_TOTAL_BASE, uTemp, 4);
         
         long32Array(UserPara.PositiveTimeBase, uTemp);                         // 更新  正转时间   单位：分钟
         Eeprom_WriteNBytes(POSITIVE_ROTATE_TIME_BASE, uTemp, 4);  
         
         long32Array(UserPara.NegativeTimeBase, uTemp);                         // 更新  反转时间   单位：分钟
         Eeprom_WriteNBytes(NEGATIVE_ROTATE_TIME_BASE, uTemp, 4);  
         
         if(UserPara.DirSta != Stall )//正转或者反转时      20200410 增加
         {
            UserPara.WorkTime = UserPara.PositiveTimeBase + UserPara.NegativeTimeBase + (UserPara.Duration + 30)/60;    //计算总时间 单位分钟
         }
         else                  //停转时
         {
            UserPara.WorkTime = UserPara.PositiveTimeBase + UserPara.NegativeTimeBase ;    //计算总时间 单位分钟
         
         }
         
         
         UserPara.WorkTime = UserPara.WorkTime/6;    // 更新  累计运行时间   单位转换  分钟--> 0.1h
         long32Array(UserPara.WorkTime, uTemp);
         Eeprom_WriteNBytes(WORK_TIME_BASE, uTemp, 4);                         
         
         
         if(UserPara.DirSta==1)// 正转
         {
             UserPara.Duration = UserPara.PositiveTimeBase - Current_PositiveTime; //  旋转方向持续时间    
         }
         else if(UserPara.RotateSta==2)// 反转
        { 
            UserPara.Duration = UserPara.NegativeTimeBase - Current_NegativeTime;//  旋转方向持续时间   
         }
         else
         {
            UserPara.Duration = (tim_1min_cnt + 30) / 60;//  停转     不计停转时间
         }
//         long32Array(UserPara.Duration, uTemp);       // 更新  当前状态运行时间            单位 100ms = 0.1s
//         Eeprom_WriteNBytes(DURATION_BASE, uTemp, 4); 
       }
        
       if(Time_1s_flag)    //1s 判断一次 更新当前状态
       {     
          Time_1s_flag = 0;
         
          Z_F_Zhuan = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
          if(Z_F_Zhuan == 0) //正转    //  低电平 正转   修改正反转方向，与协议一致  20200527        
          {
              Led_Control(0);   //红灯
          }
          else
          {
              Led_Control(1);  //反转  绿灯
          }
//          UserPara.RotateSpeed  = 0;
//          for(i = 0; i<3;i++ )
//          {
//            UserPara.RotateSpeed +=Pulse100msCntBuf[i];                       
//          }
//          UserPara.RotateSpeed *= 20;  //计算旋转速度  1s脉冲数*10 =10秒* 6 = 1分钟  单位：转每分
       
            if(PulseFlag)  //有脉冲  在转动
            {           
                            
                //Z_F_Zhuan = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
                
                if(Z_F_Zhuan == 0) //正转
                {
                    if(UserPara.DirSta == Reversal) //上一次状态是  反转
                    {
                        Current_PositiveTime = UserPara.PositiveTimeBase;
                        UserPara.NegativeTimeBase += (tim_1min_cnt + 30)/60;          //计算反转时间  +30意思是超过半分钟算1分钟
                        tim_1min_cnt = 0;  //清0当前状态计时
                        tim_cnt = 0;                    }
                    if(UserPara.RotateSta == STA_STOP)   //上一次状态是  停转
                    {
                        Current_PositiveTime = UserPara.PositiveTimeBase;
                        tim_1min_cnt = 0;  //清0当前状态计时
                          tim_cnt = 0;
                    }
                                            
                    UserPara.DirSta = Foreward;   // ==1  //旋转方向  正转
                             
                    if(!((tim_cnt +1)%60))//60s到  更新一次正转时间
                    {
                        UserPara.PositiveTimeBase += 1;        //60s 更新一次正转时间  
                        tim_1min_cnt = 0;
                    }
                }
                else               //反转
                {
                    if(UserPara.DirSta == Foreward) //上一次状态是  正转
                    {
                        Current_NegativeTime = UserPara.NegativeTimeBase;
                        UserPara.PositiveTimeBase += (tim_1min_cnt + 30)/60;          //计算正转时间
                        tim_1min_cnt = 0;  //清0当前状态计时
                        tim_cnt = 0;
                    }
                    if(UserPara.RotateSta == STA_STOP)   //上一次状态是  停转
                    {
                        Current_NegativeTime = UserPara.NegativeTimeBase;
                        tim_1min_cnt = 0;  //清0当前状态计时
                          tim_cnt = 0;
                    }
                                            
                    UserPara.DirSta = Reversal;   // ==0  //旋转方向  反转
                    
                     if(!((tim_cnt +1)%60))//60s到  更新一次反转时间
                    {
                        UserPara.NegativeTimeBase += 1;          //60s  更新一次反转时间  
                        tim_1min_cnt = 0;
                     }                   
                }
                
                UserPara.RotateSta = STA_WORK;                      //旋转状态   转动中
                
            }
            else     //无脉冲  停止
            {
                    if(UserPara.DirSta == Foreward)                      //上一次状态是  正转
                    {
                        UserPara.PositiveTimeBase += (tim_1min_cnt + 30)/60;             //计算正转时间  
                        tim_1min_cnt = 0;                                       //清0当前状态计时
                    }           
                     if(UserPara.DirSta == Reversal)                //上一次状态是  反转
                    {
                        UserPara.NegativeTimeBase += (tim_1min_cnt + 30)/60;              //计算反转时间
                        tim_1min_cnt = 0;                                      //清0当前状态计时
                    }

                                  
                UserPara.DirSta = Stall; // ==0  //旋转方向 
                UserPara.RotateSta = STA_STOP;     //旋转状态   停转
                
            }  

       }                              //end  if(Time_1s_flag)    //1s 判断一次 更新当前状态
    }                                   //end while 
}                                       //end main



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
