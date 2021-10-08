#include "delay.h"
#include "sys.h"
#include "usart.h"	   
#include "Steering_engine.h"
#include "IO_Timer.h"
#include "LED_Control.h"
#include "Host_Control.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
�ļ���			: main.c
����	  	: �Ų���
�汾	   	: V1.0
����	   	: ������
����	   	: ��
��վ 	   	: www.elsintech.com
��־	   	: ����V1.0 2020/6/15 �Ų��ȴ���
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
	u8 kd;	// p i d ����
	u16 error;//��ǰ���
  u16 last_error;//�ϴ����
	u16 pre_error;//���ϴ����
	u16 pwm;
} PID;

 int main(void)
{
	u8 a = 0;
	u8 i = 0;
	SystemInit();
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
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







	


	
	










