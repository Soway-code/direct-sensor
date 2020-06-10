/************************************************************************************************************************************************************************
** 版权：   2020-2030, 深圳市信为科技发展有限公司
** 文件名:  main.c
** 作者:    weijianx
** 版本:    V1.0.0
** 日期:    2020-06-09
** 描述:    
** 功能:    利用在不同状态(停车 怠速 工作)下电压不同来判断车辆的运行状态     
*************************************************************************************************************************************************************************
** 修改者:      No
** 版本:  		
** 修改内容:    No 
** 日期:        No
*************************************************************************************************************************************************************************/

#include "config.h"
#include "string.h"



//******************************************************************************
// 名称         : main()
// 创建日期     : 2020-06-09
// 作者         : weijianx
// 功能         : main函数
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************
int main(void)
{ 
	
	task_Init();
    
	while(1)
	{
	
		rt_thread_mdelay(1000);
	}
    
}                                       //end main



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

