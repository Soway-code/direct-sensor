/************************************************************************************************************************************************************************
** ��Ȩ��   2018-2028, ��������Ϊ�Ƽ���չ���޹�˾
** �ļ���:  main.c
** ����:    MMX
** �汾:    V1.0.0
** ����:    2018-06-08
** ����:    
** ����:    �����ڲ�ͬ״̬(ͣ�� ���� ����)�µ�ѹ��ͬ���жϳ���������״̬     
*************************************************************************************************************************************************************************
** �޸���:      No
** �汾:  		
** �޸�����:    No 
** ����:        No
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


//BitAction StartFillBufFlag = Bit_RESET;                                         //��ʼװ�������־
uint16_t ADBUF[AD_BUF_MAX];                                     //�ɼ�ADֵ��������  10����
float  LiquidHeight = 0;
uint16_t LiquidUnit = 0;

extern uint8_t StartFlag;
extern uint8_t StartCountFlag;
extern BitAction  PulseFlag;
IWDG_HandleTypeDef  IWDG_HandleStructure;


extern UserTypeDef UserPara;
 
u8 LFilCnt = 0;
u8 time_tick = 10;  //Ĭ���˲���ʽ  Ϊƽ���˲�

   // uint32_t systemClock;
    //uint32_t HclkFre;
    //uint32_t Pclk1Fre;
    //uint32_t Pclk2Fre;
extern uint8_t Time_1s_flag;
extern uint8_t Time_5s_flag;
extern uint16_t Pulse100msCntBuf[3];
extern uint16_t tim_cnt;   //��ʱ1S ʱ���ۼ�

extern uint16_t Current_PositiveTime ;  //��ǰ��ת������
extern uint16_t Current_NegativeTime ;  //��ǰ��ת������
extern uint16_t tim_1min_cnt; //1minʱ���ۼ�


#define LED_PORT        GPIOA
#define LED_GREEN_PIN   GPIO_PIN_6      // ��� PA6
#define LED_RED_PIN     GPIO_PIN_5      //�̵�PA5


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

//GPIO����
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

 uint8_t zhengfanzhuan; 
//******************************************************************************
// ����         : main()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : main����
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
void main(void)
{ 
    //uint8_t i;   
    uint8_t Z_F_Zhuan;     
    uint8_t uTemp[4];
    
    SystemClock_Config();                                                       //ʱ������ΪHSI 16M
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  //�رյδ�ʱ��
    GPIO_Configuration();
     
    Led_Control(1);  //�ϵ���ɫ
    // systemClock = HAL_RCC_GetSysClockFreq();
    //HclkFre = HAL_RCC_GetHCLKFreq();
    //Pclk1Fre = HAL_RCC_GetPCLK1Freq();
    //Pclk2Fre = HAL_RCC_GetPCLK2Freq();
  
    ReadPara();                                                                 //��ȡEEPROM����
    Uart_Config_Init();                                                         //��������
    //TIM2_Init();                                //PA0���� �������
    
    TIM21_ETR_Init(); //PA0���� �������
    TIM22_Init();                                //100ms��ʱ��
    
    User_Iwdg_Init();                                                           //���Ź�����
    Led_Control(1);  //�ϵ���ɫ
    
    while(1)
    {         
       
        User_Iwdg_Feed();                                                   //ι��
        
        MBASC_Function();                                                   //MODBUS����
     
        
    
       if(Time_5s_flag)  //10s ʱ�䵽 ���� ����  
       {
         
          
         Time_5s_flag = 0;
                        
         long32Array(UserPara.TotalPulse, uTemp);                               // ����  ��������     ��λ��HZ   
         Eeprom_WriteNBytes(PULSE_TOTAL_BASE, uTemp, 4);
         
         long32Array(UserPara.PositiveTimeBase, uTemp);                         // ����  ��תʱ��   ��λ������
         Eeprom_WriteNBytes(POSITIVE_ROTATE_TIME_BASE, uTemp, 4);  
         
         long32Array(UserPara.NegativeTimeBase, uTemp);                         // ����  ��תʱ��   ��λ������
         Eeprom_WriteNBytes(NEGATIVE_ROTATE_TIME_BASE, uTemp, 4);  
         
         if(UserPara.DirSta != Stall )//��ת���߷�תʱ      20200410 ����
         {
            UserPara.WorkTime = UserPara.PositiveTimeBase + UserPara.NegativeTimeBase + (UserPara.Duration + 30)/60;    //������ʱ�� ��λ����
         }
         else                  //ͣתʱ
         {
            UserPara.WorkTime = UserPara.PositiveTimeBase + UserPara.NegativeTimeBase ;    //������ʱ�� ��λ����
         
         }
         
         
         UserPara.WorkTime = UserPara.WorkTime/6;    // ����  �ۼ�����ʱ��   ��λת��  ����--> 0.1h
         long32Array(UserPara.WorkTime, uTemp);
         Eeprom_WriteNBytes(WORK_TIME_BASE, uTemp, 4);                         
         
         
         if(UserPara.DirSta==1)// ��ת
         {
             UserPara.Duration = UserPara.PositiveTimeBase - Current_PositiveTime; //  ��ת�������ʱ��    
         }
         else if(UserPara.RotateSta==2)// ��ת
        { 
            UserPara.Duration = UserPara.NegativeTimeBase - Current_NegativeTime;//  ��ת�������ʱ��   
         }
         else
         {
            UserPara.Duration = (tim_1min_cnt + 30) / 60;//  ͣת     ����ͣתʱ��
         }
//         long32Array(UserPara.Duration, uTemp);       // ����  ��ǰ״̬����ʱ��            ��λ 100ms = 0.1s
//         Eeprom_WriteNBytes(DURATION_BASE, uTemp, 4); 
       }
        
       if(Time_1s_flag)    //1s �ж�һ�� ���µ�ǰ״̬
       {     
          Time_1s_flag = 0;
         
          Z_F_Zhuan = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
          if(Z_F_Zhuan == 0) //��ת    //  �͵�ƽ ��ת   �޸�����ת������Э��һ��  20200527        
          {
              Led_Control(0);   //���
          }
          else
          {
              Led_Control(1);  //��ת  �̵�
          }
//          UserPara.RotateSpeed  = 0;
//          for(i = 0; i<3;i++ )
//          {
//            UserPara.RotateSpeed +=Pulse100msCntBuf[i];                       
//          }
//          UserPara.RotateSpeed *= 20;  //������ת�ٶ�  1s������*10 =10��* 6 = 1����  ��λ��תÿ��
       
            if(PulseFlag)  //������  ��ת��
            {           
                            
                //Z_F_Zhuan = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
                
                if(Z_F_Zhuan == 0) //��ת
                {
                    if(UserPara.DirSta == Reversal) //��һ��״̬��  ��ת
                    {
                        Current_PositiveTime = UserPara.PositiveTimeBase;
                        UserPara.NegativeTimeBase += (tim_1min_cnt + 30)/60;          //���㷴תʱ��  +30��˼�ǳ����������1����
                        tim_1min_cnt = 0;  //��0��ǰ״̬��ʱ
                        tim_cnt = 0;                    }
                    if(UserPara.RotateSta == STA_STOP)   //��һ��״̬��  ͣת
                    {
                        Current_PositiveTime = UserPara.PositiveTimeBase;
                        tim_1min_cnt = 0;  //��0��ǰ״̬��ʱ
                          tim_cnt = 0;
                    }
                                            
                    UserPara.DirSta = Foreward;   // ==1  //��ת����  ��ת
                             
                    if(!((tim_cnt +1)%60))//60s��  ����һ����תʱ��
                    {
                        UserPara.PositiveTimeBase += 1;        //60s ����һ����תʱ��  
                        tim_1min_cnt = 0;
                    }
                }
                else               //��ת
                {
                    if(UserPara.DirSta == Foreward) //��һ��״̬��  ��ת
                    {
                        Current_NegativeTime = UserPara.NegativeTimeBase;
                        UserPara.PositiveTimeBase += (tim_1min_cnt + 30)/60;          //������תʱ��
                        tim_1min_cnt = 0;  //��0��ǰ״̬��ʱ
                        tim_cnt = 0;
                    }
                    if(UserPara.RotateSta == STA_STOP)   //��һ��״̬��  ͣת
                    {
                        Current_NegativeTime = UserPara.NegativeTimeBase;
                        tim_1min_cnt = 0;  //��0��ǰ״̬��ʱ
                          tim_cnt = 0;
                    }
                                            
                    UserPara.DirSta = Reversal;   // ==0  //��ת����  ��ת
                    
                     if(!((tim_cnt +1)%60))//60s��  ����һ�η�תʱ��
                    {
                        UserPara.NegativeTimeBase += 1;          //60s  ����һ�η�תʱ��  
                        tim_1min_cnt = 0;
                     }                   
                }
                
                UserPara.RotateSta = STA_WORK;                      //��ת״̬   ת����
                
            }
            else     //������  ֹͣ
            {
                    if(UserPara.DirSta == Foreward)                      //��һ��״̬��  ��ת
                    {
                        UserPara.PositiveTimeBase += (tim_1min_cnt + 30)/60;             //������תʱ��  
                        tim_1min_cnt = 0;                                       //��0��ǰ״̬��ʱ
                    }           
                     if(UserPara.DirSta == Reversal)                //��һ��״̬��  ��ת
                    {
                        UserPara.NegativeTimeBase += (tim_1min_cnt + 30)/60;              //���㷴תʱ��
                        tim_1min_cnt = 0;                                      //��0��ǰ״̬��ʱ
                    }

                                  
                UserPara.DirSta = Stall; // ==0  //��ת���� 
                UserPara.RotateSta = STA_STOP;     //��ת״̬   ͣת
                
            }  

       }                              //end  if(Time_1s_flag)    //1s �ж�һ�� ���µ�ǰ״̬
    }                                   //end while 
}                                       //end main



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
