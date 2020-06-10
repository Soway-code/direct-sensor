#include <stdarg.h>
#include <string.h>
#include "type.h"
#include "para.h"
#include "usart.h"
#include "modbus_ascii.h"

#define LPUART1_ENABLE_IT(__HANDLE__, __INTERRUPT__) (((((uint8_t)(__INTERRUPT__)) >> 5U) == 1U)? ((__HANDLE__)->CR1 |= (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((((uint8_t)(__INTERRUPT__)) >> 5U) == 2U)? ((__HANDLE__)->CR2 |= (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((__HANDLE__)->CR3 |= (1U << ((__INTERRUPT__) & UART_IT_MASK))))
#define LPUART1_DISABLE_IT(__HANDLE__, __INTERRUPT__) (((((uint8_t)(__INTERRUPT__)) >> 5U) == 1U)? ((__HANDLE__)->CR1 &= ~ (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((((uint8_t)(__INTERRUPT__)) >> 5U) == 2U)? ((__HANDLE__)->CR2 &= ~ (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((__HANDLE__)->CR3 &= ~ (1U << ((__INTERRUPT__) & UART_IT_MASK))))

uint8_t u8SendNum = 0;                  //��ǰ�������ݸ���
uint8_t u8SendIndex = 0;                //�������ݻ���ǰ׺
uint8_t UART1_RXBuffLen = 0;            //�������ݻ��泤��


BitAction UartRecvFrameOK = Bit_RESET;  //����������ɱ�־
uint8_t UART1_RXBuff[USART_MAX_DATALEN];//�������ݻ���
uint8_t UART1_TXBuff[USART_MAX_DATALEN];//�������ݻ���

UART_HandleTypeDef  LPUART1_HandleStruct;        //���ڲ���

extern UserTypeDef UserPara;
///*
//******************************************************************************
// ����               : Usart_Gpio_Init()
// ��������           : 2017-11-27
// ����               : ÷����
// ����               : ͨ�ô����շ���������
// �������           : ��
// �������           : ��
// ���ؽ��           : ��
// ע���˵��         : ��
// �޸�����           :
//******************************************************************************
void Usart_Gpio_Init(void)
{
    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);   //GPIOA��ʱ��
    EN_485_PORT->MODER &= 0xFFFFFCFF;            
    EN_485_PORT->MODER |= 0x00000100;           //PA4ͨ�����ģʽ
    EN_485_PORT->OTYPER &= ~(1<<4);             //�������
    EN_485_PORT->OSPEEDR |= (3<<8);             //����  
    EN_485_PORT->BRR = EN_485_PIN;              //����״̬
    
    LPUSART1_PORT->MODER &= 0xFFFFFF0F;            
    LPUSART1_PORT->MODER |= 0x000000A0;          //PA2 PA3���ù���ģʽ
    LPUSART1_PORT->OTYPER &= ~(3<<2);            //�������
    //LPUSART1_PORT->AFR[0] &= 0xF0FFFFFF;         //AF6�����ֲ�      
    //LPUSART1_PORT->AFR[0] |= 0x06000000;
    LPUSART1_PORT->AFR[0] &= 0xFFFF00FF;
    LPUSART1_PORT->AFR[0] |= 0x00006600;
}


//******************************************************************************
// ����               : Usart_Config_Init()
// ��������           : 2017-11-27
// ����               : ÷����
// ����               : ͨ�ô��ڲ�����ʼ��
// �������           : UARTx_ConfigTypeDef(����ͨ�Ų���)
// �������           : ��
// ���ؽ��           : ��
// ע���˵��         : ��
// �޸�����           :
//******************************************************************************
void Uart_Config_Init(void)
{  
    Usart_Gpio_Init(); 
      
    SET_BIT(RCC->APB1ENR, (RCC_APB1ENR_LPUART1EN));                             //��LPUSART1ʱ�� 
    NVIC->IP[_IP_IDX(LPUART1_IRQn)] = (NVIC->IP[_IP_IDX(LPUART1_IRQn)] & ~(0xFF << _BIT_SHIFT(LPUART1_IRQn))) |
       (((1 << (8 - __NVIC_PRIO_BITS)) & 0xFF) << _BIT_SHIFT(LPUART1_IRQn));    //���������ȼ�Ϊ1
    NVIC->ISER[0] = (1 << ((uint32_t)(LPUART1_IRQn) & 0x1F));                   //ʹ�����ж�
    
    LPUART1->CR1 &=  ~USART_CR1_UE;                                             //��ʧ�ܴ���
    
    //MODIFY_REG(LPUART1->CR1, ((0x10001U << 12) | (1 << 10) | (1 << 9) | (1 << 3) | (1 << 2) | (1 << 15)), 
   // UART_WORDLENGTH_8B | UART_PARITY_NONE | UART_MODE_TX_RX);               //��������λ��У��λ������ģʽ
   //// MODIFY_REG(LPUART1->CR2, USART_CR2_STOP, UART_STOPBITS_1);                  //����ֹͣλ
   // MODIFY_REG(LPUART1->CR3, (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT), UART_HWCONTROL_NONE);
   // LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 9600));          //���ò����ʣ�ϵͳʱ��2M��������9600��
    
    switch(UserPara.Parity)
    {   
    case 03:  //Ĭ����У��
        MODIFY_REG(LPUART1->CR1, ((0x10001U << 12) | (1 << 10) | (1 << 9) | (1 << 3) | (1 << 2) | (1 << 15)), 
              UART_WORDLENGTH_8B | UART_PARITY_NONE | UART_MODE_TX_RX);               //��������λ��У��λ������ģʽ
        break;
    case 02:
        MODIFY_REG(LPUART1->CR1, ((0x10001U << 12) | (1 << 10) | (1 << 9) | (1 << 3) | (1 << 2) | (1 << 15)), 
              UART_WORDLENGTH_8B | UART_PARITY_EVEN | UART_MODE_TX_RX);               //��������λ��У��λ������ģʽ
        break;
    case 01:
        MODIFY_REG(LPUART1->CR1, ((0x10001U << 12) | (1 << 10) | (1 << 9) | (1 << 3) | (1 << 2) | (1 << 15)), 
              UART_WORDLENGTH_8B | UART_PARITY_ODD | UART_MODE_TX_RX);               //��������λ��У��λ������ģʽ
        break;
    default:
        MODIFY_REG(LPUART1->CR1, ((0x10001U << 12) | (1 << 10) | (1 << 9) | (1 << 3) | (1 << 2) | (1 << 15)), 
              UART_WORDLENGTH_8B | UART_PARITY_NONE | UART_MODE_TX_RX);               //��������λ��У��λ������ģʽ
        break;
    }
   
    
    MODIFY_REG(LPUART1->CR2, USART_CR2_STOP, UART_STOPBITS_1);                  //����ֹͣλ
    
    MODIFY_REG(LPUART1->CR3, (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT), UART_HWCONTROL_NONE);
    
    
     switch(UserPara.Baudrate)
    {   
    case 01:  
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 2400));          //���ò����ʣ�ϵͳʱ��2M��������2400��
        break;
    case 02:
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 4800));          
        break;
    case 03:
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 9600));         
        break;
    case 04:  
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 19200));          
        break;
    case 05:
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 38400));          
        break;
    case 06:
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 57600));          
        break;
    case 07:
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 115200));         
        break;
      default://Ĭ��9600
        LPUART1->BRR = (uint32_t)(UART_DIV_LPUART(SystemCoreClock, 9600));          //���ò����ʣ�ϵͳʱ��2M��������9600��
        break;
    }
           
    CLEAR_BIT(LPUART1->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
    CLEAR_BIT(LPUART1->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));
    LPUART1_ENABLE_IT(LPUART1, UART_IT_RXNE);                                   //ʹ�ܽ����ж�
    LPUART1->CR1 |=  USART_CR1_UE;                                              //ʹ�ܴ���
}



//******************************************************************************
// ����               : UART1_RecvData()
// ��������           : 2017-11-27
// ����               : ÷����
// ����               : UART1�������ݺ���
// �������           : uint8_t *UART1_RecvBuff �������ݻ���
//                      uint32_t Len            �������ݳ���        
// �������           : ��
// ���ؽ��           : ��
// ע���˵��         : 
// �޸�����           :
//******************************************************************************
int32_t UART1_RecvData(uint8_t *UART1_RecvBuff, uint32_t Len)
{
    uint32_t i = 0;

    if ((0 == Len) || (((uint8_t*)NULL) == UART1_RecvBuff))
    {
        return 0;
    }

    if ((Bit_RESET == UartRecvFrameOK) || (0 == UART1_RXBuffLen))
    {
        return 0;
    }

    if (Len < UART1_RXBuffLen)
    {
        return -1;
    }

    Len = UART1_RXBuffLen;

    for (i = 0; i < Len; i++)
    {
        UART1_RecvBuff[i] = UART1_RXBuff[i];
    }

    UART1_RXBuffLen = 0;

    return Len;
}


//******************************************************************************
// ����               : UART1_SendData()
// ��������           : 2017-11-27
// ����               : ÷����
// ����               : UART1�������ݺ���
// �������           : uint8_t *UART1_SendBuff �������ݻ���
//                      uint32_t Len            �������ݳ���        
// �������           : ��
// ���ؽ��           : ��
// ע���˵��         : 
// �޸�����           :
//******************************************************************************
uint32_t UART1_SendData(uint8_t *UART1_SendBuff, uint32_t Len)
{
    uint32_t i = 0;

    if ((0 == Len) || (((uint8_t*)0) == UART1_SendBuff))
    {
        return 0;
    }
    if (u8SendNum != 0)
    {
        return 0;
    }

    if (Len > (sizeof(UART1_TXBuff) / sizeof(UART1_TXBuff[0])))
    {
        Len = (sizeof(UART1_TXBuff) / sizeof(UART1_TXBuff[0]));
    }

    for (i = 0; i < Len; i++)
    {
        UART1_TXBuff[i] = UART1_SendBuff[i];
    }
    TX_ON;
    LPUART1->TDR = UART1_TXBuff[0];
    u8SendIndex = 1;
    u8SendNum = Len;
    LPUART1_ENABLE_IT(LPUART1, UART_IT_TC);
    return Len;
}



//******************************************************************************
// ����               : Enable_Lpuart1()
// ��������           : 2017-11-27
// ����               : ÷����
// ����               : ʹ��LPUSART1
// �������           : ��     
// �������           : ��
// ���ؽ��           : ��
// ע���˵��         : 
// �޸�����           :
//******************************************************************************
void Enable_Lpuart1(void)
{
    LPUART1_ENABLE_IT(LPUART1, UART_IT_TC);                                     //���������ж�
    LPUART1_ENABLE_IT(LPUART1, UART_IT_RXNE);                                   //���������ж�
    LPUART1->CR1 |=  USART_CR1_UE;                                              //ʹ��LPUSART1
}


//******************************************************************************
// ����               : Disable_Lpuart1()
// ��������           : 2017-11-27
// ����               : ÷����
// ����               : �ر�LPUSART1
// �������           : ��     
// �������           : ��
// ���ؽ��           : ��
// ע���˵��         : 
// �޸�����           :
//******************************************************************************
void Disable_Lpuart1(void)
{
    LPUART1_DISABLE_IT(LPUART1, UART_IT_TC);
    LPUART1_DISABLE_IT(LPUART1, UART_IT_RXNE);   
    LPUART1->CR1 &=  ~USART_CR1_UE;
}

//******************************************************************************
// ����               : USART1_IRQHandler()
// ��������           : 2017-11-27
// ����               : ÷����
// ����               : UART1�����ж�
// �������           : ��
//                      ��       
// �������           : ��
// ���ؽ��           : ��
// ע���˵��         : 
// �޸�����           :
//******************************************************************************
void LPUART1_IRQHandler(void)
{
    uint8_t RecvByteData;

    if((LPUART1->ISR & ((uint32_t)1U << (UART_IT_RXNE >> 0x08U))) != RESET)
    {
        LPUART1->ICR = UART_IT_RXNE;
        
        RecvByteData = LPUART1->RDR;
                          
        MODBUS_ASCII_HandlRevData(RecvByteData); //����ASCII����     
    }

      
   if((LPUART1->ISR & ((uint32_t)1U << (UART_IT_TC >> 0x08U))) != RESET)
    {
       LPUART1->ICR = UART_IT_TC;


        if(u8SendIndex < u8SendNum)
        { 
          LPUART1->TDR = UART1_TXBuff[u8SendIndex++];         
        }
        else
        {
           u8SendNum = 0;
           LPUART1_DISABLE_IT(LPUART1, UART_IT_TC);
           TX_OFF;
        }
    }    
    if((LPUART1->ISR & ((uint32_t)1U << (UART_IT_ORE >> 0x08U))) != RESET)
    {
      LPUART1->ICR  = UART_CLEAR_OREF;
    }
}


//******************************************************************************
// ����         : uprintf(const char *fmt,...)
// ��������     : 2016-09-08
// ����         : ÷����
// ����         : �򴮿ڷ���ָ����ʽ������
// �������     : �룺fmt,...    ��������ָ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��usart��ӡ���ڵ����ݣ����ڵ���
// �޸�����     : 
//******************************************************************************

void uprintf(const char *fmt,...)
{
    va_list marker;
    char buff[32];
    memset(buff,0,sizeof(buff));
    va_start(marker, fmt);
    vsprintf(buff,fmt,marker);
    va_end(marker);
    UART1_SendData((uint8_t*)buff, strlen(buff));
    while(u8SendNum!=0);    
}

