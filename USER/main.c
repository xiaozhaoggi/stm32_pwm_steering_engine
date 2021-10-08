#include "delay.h"
#include "sys.h"
#include "usart.h"	   
#include "Steering_engine.h"
#include "IO_Timer.h"
#include "LED_Control.h"
#include "Host_Control.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
文件名			: main.c
作者	  	: 张博钊
版本	   	: V1.0
描述	   	: 主函数
其他	   	: 无
网站 	   	: www.elsintech.com
日志	   	: 初版V1.0 2020/6/15 张博钊创建
***************************************************************/

extern u8 get_data[4];
extern u8 get_flag;
extern u8 get_flag1;
extern u8 get_flag2;
extern u8 send_data;
extern u8 stp_status;

extern struct {
	u8 kp;
	u8 ki;
	u8 kd;	// p i d 参数
	u16 error;//当前误差
  u16 last_error;//上次误差
	u16 pre_error;//上上次误差
	u16 pwm;
} PID;

 int main(void)
{
	u8 a = 0;
	u8 i = 0;
	SystemInit();
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	Steering_engine_Init();
	IO_Timer_Init();
	LED_Control_Init();
	PID.pwm = 500;
	
	while(1) 
	{
		
//		
//		if(get_flag == 1)
//		{
//			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//			USART_SendData(USART1, send_data);
//			get_flag = 0;
//			a = test_data;
//		}
//	

//		if(a >= 5 && a <= 25)
//		{
//			
//			PID.pwm = a * 100;
//			TIM_SetCompare4(TIM2, PID.pwm);
//		}
	
		if(get_flag)
		{
			if(get_flag1)
			{
				TIM_SetCompare4(TIM2, OpenLoop_Control(get_data[1], get_data[2]) );
				get_flag1 = 0;
			}
		
			if(get_flag2)
			{
				get_flag2 = 0;
			}
			
			for(i = 0; i< 4; i++)
			{
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
					USART_SendData(USART1, get_data[i]);
			}
			
			get_flag = 0;
		} 
		
//		if(!GPIO_ReadInputDataBit(GPIOA, STP) )  			stp_status = 1;
		
/*		if(stp_status)
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
				USART_SendData(USART1, send_data);
			
			delay_ms(1000);
		}
*/	

/*	if(send_time_flag)
		uart_time_send(&stpn_ms);
*/	
	}

}







	


	
	










