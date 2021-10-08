#include "Host_Control.h"
#include "LED_Control.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
�ļ���		: Host_Control.c
����	  	: �Ų���
�汾	   	: V1.0
����	   	: �����жϺ���
����	   	: ��
��վ 	   	: www.elsintech.com
��־	   	: ����V1.0 2020/6/15 �Ų��ȴ���
***************************************************************/

u8 	get_data[4];
u16 send_data = 0x1;
u8 	get_flag  = 0; //������ɱ�־
u8 	get_flag1 = 0;	//���յ�ѹ��������־
u8 	get_flag2 = 0;	//������ɢ����־

void uart_irq(void)
{
	static u8 receive_index = 1;
				 u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		Res =USART_ReceiveData(USART1);

	
	switch(receive_index)
		{
		case 1: 
				if(Res == 0xAA)   
					receive_index++;
				get_data[0] = Res;
				get_flag = 1;		
				break;		
				
		case 2:
				get_data[1] = Res;
				receive_index++;
		
				if(get_data[1] != 0)
					get_flag1 = 1;  //����ڶ�λ������0 ������͵��ǿ��Ƶ�ѹ����֡
				else
					get_flag2 = 1;	//����ڶ�λ����0 	������͵�����ɢ������֡
				
				break;
				
		case 3:
				get_data[2] = Res;
				receive_index++;
		
				break;
		
		case 4:
			if(Res == 0xBB)
				get_flag = 1;			
			get_data[3] = Res;
			receive_index = 1;
			
				break;             //��������֡ AA xx xx BB
	
		}
	}

	
		
}

void uart_time_send(int *stp_ms)
{
	int send_data[4] = {0xCC, 0, 0, 0xDD };
	
	int i = 0;
	int p_time  = *stp_ms;
	u8  time_h, time_l;
	
	time_h = p_time >> 8;
	time_l = (p_time | 0xFF00) ^ 0xFF00; 
	
	send_data[1] = time_h;
	send_data[2] = time_l;

	for(i = 0; i < 4; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_SendData(USART1, send_data[i]);
	}
	
	
	 
	
		
}

