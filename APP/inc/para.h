#ifndef _PARA_H
#define _PARA_H

#include "type.h"


#define RUN_ADDR_BASE				                                   0x00	                //参数初始化标志位，避免每次上电都写EEPROMP
#define SLAVE_ADDR					(RUN_ADDR_BASE		 + 0x01)                //从机地址
#define BAUDRATE		                        (SLAVE_ADDR	         + 0x01)                //波特率
#define PARITY			                        (BAUDRATE	         + 0x01)                //奇偶校验位

#define WORK_TIME_BASE                                  (PARITY                  + 0x01)                 //工作总时间基值
#define STOP_TIME_BASE                                  (WORK_TIME_BASE               + 0x04)                 //停止时间基值
#define POSITIVE_ROTATE_TIME_BASE                       (STOP_TIME_BASE               + 0x04)                 //正转时间基值
#define NEGATIVE_ROTATE_TIME_BASE                       (POSITIVE_ROTATE_TIME_BASE              + 0x04)      //反转时间基值
#define PULSE_TOTAL_BASE                                (NEGATIVE_ROTATE_TIME_BASE              + 0x04)       //脉冲数基值
#define DURATION_BASE                                  (PULSE_TOTAL_BASE              + 0x04)       //脉冲数基值

#define USER_DEFAULT_LEN				(DURATION_BASE                       + 0x04)


//---------------------------------------------------
#define COMPANY						(USER_DEFAULT_LEN       + 0x40) 
#define DEV_ENCODING				        (COMPANY		+ 0x40)
#define HWVERSION					(DEV_ENCODING		+ 0x40)
#define SFVERSION					(HWVERSION	        + 0x40)
#define DEV_ID						(SFVERSION		+ 0x40)
#define CUSTOMERCODE                                    (DEV_ID		        + 0x40)  


void ReadPara(void);
uint32_t GetDelExtremeAndAverage(uint32_t Array[], const uint32_t ArraySize,const uint32_t SortHeadSize, const uint32_t SortTailSize);
void Switch_Fiter(uint8_t value);

#endif