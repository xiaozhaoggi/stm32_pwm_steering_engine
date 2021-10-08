#include "IO_Timer.h"
#include "stm32f10x_tim.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
�ļ���		: IO_Timer.c
����	  	: �Ų���
�汾	   	: V1.0
����	   	: ������λ�����͵���ɢ��
����	   	: ��
��վ 	   	: www.elsintech.com
��־	   	: ����V1.0 2020/6/15 �Ų��ȴ���
***************************************************************/

int ms = 0;
u8 stp0_status = 0, stp1_status = 0, stp2_status = 0, stp3_status = 0, stp4_status = 0, stp5_status = 0, 
	stp6_status = 0, stp7_status = 0, stp8_status = 0, stp9_status = 0, stp10_status = 0, stp_status = 0;//�˿�״̬����
int stpn_secs = 0;//�˿ڼ�ʱʱ�������
int stpn_ms = 0;//�˿ڼ�ʱʱ�����  
u8 send_time_flag = 0; //���Ͷ�ʱʱ���־λ


//stp 0 - 6 Ϊ��ʼλ  stp 7 -10 Ϊֹͣλ
void IO_Timer_Init(void) //��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOA B �˿�ʱ��
	
	
	//GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin  = TM_STP0 | TM_STP1 | TM_STP2 | TM_STP3 | TM_STP4 | TM_STP5 | TM_STP6 | TM_STP7 | TM_STP8 | TM_STP9 | TM_STP10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB 0-10
	
	GPIO_InitStructure.GPIO_Pin  = STP;//PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA 11
	

//	//GPIOB 0-10 �ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0 | GPIO_PinSource1 | GPIO_PinSource2 | GPIO_PinSource3 | GPIO_PinSource4 
																					| GPIO_PinSource5 | GPIO_PinSource6 | GPIO_PinSource7 | GPIO_PinSource8 | GPIO_PinSource9 | GPIO_PinSource10);
	//GPIOA 11
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	
	EXTI_InitStructure.EXTI_Line =EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4 
																| EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9 | EXTI_Line10 | EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	
	NVIC_InitStructure.NVIC_IRQChannel =EXTI0_IRQn;			//ʹ��IO���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ��IO���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ��IO���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ��IO���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ��IO���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ��IO���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ��IO���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	TIM3_Int_Init(49, 3599);		//10ms ��һ���ж�
}
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

void get_stp_status(void)	//��ȡ�˿�״̬
{
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP0) ) 	stp0_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP1) ) 	stp1_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP2) ) 	stp2_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP3) ) 	stp3_status = 0;
		
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP4) ) 	stp4_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP5) ) 	stp5_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP6) ) 	stp6_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP7) ) 	stp7_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP8) ) 	stp8_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP9) ) 	stp9_status = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, TM_STP10) ) 	stp10_status = 0;
	
	
	if(!GPIO_ReadInputDataBit(GPIOA, STP) )  			stp_status = 0;
			
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{   
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		
		if(stp0_status || stp1_status || stp2_status
			|| stp3_status || stp4_status || stp5_status || stp6_status)	
			stpn_ms += 5;
		
		if(stp7_status || stp8_status || stp9_status || stp10_status)	
		{	
			uart_time_send(&stpn_ms);
			
			stpn_ms = 0;
			
			stp0_status = 0;
			stp1_status = 0;
			stp2_status = 0;
			stp3_status = 0;
			stp4_status = 0;
			stp5_status = 0;
			stp6_status = 0;
			stp7_status = 0;
			stp8_status = 0;
			stp9_status = 0;
			stp10_status = 0;
		}
			
		
		//stpn_secs = stpn_ms / 1000;
		
			
				
	}
}

//GPIO�ⲿ�ж�
void EXTI0_IRQHandler(void)
{
	stp0_status = 1;
	EXTI_ClearITPendingBit(EXTI_Line0);  
}

void EXTI1_IRQHandler(void)
{
	stp1_status = 1;
	EXTI_ClearITPendingBit(EXTI_Line1);  
}

void EXTI2_IRQHandler(void)
{
	stp2_status = 1;
	EXTI_ClearITPendingBit(EXTI_Line2);  
}

void EXTI3_IRQHandler(void)
{
	stp3_status = 1;
	EXTI_ClearITPendingBit(EXTI_Line3);  
}

void EXTI4_IRQHandler(void)
{
	stp4_status = 1;
	EXTI_ClearITPendingBit(EXTI_Line4); 
}

void EXTI9_5_IRQHandler(void)
{	
	if(EXTI_GetFlagStatus(EXTI_Line5) )
	{
		stp5_status = 1;
		EXTI_ClearITPendingBit(EXTI_Line5);  
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line6) )
	{
		stp6_status = 1;
		EXTI_ClearITPendingBit(EXTI_Line6);  
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line7) )
	{
		stp7_status = 1;
		EXTI_ClearITPendingBit(EXTI_Line7);  
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line8) )
	{
		stp8_status = 1;
		EXTI_ClearITPendingBit(EXTI_Line8);  
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line9) )
	{
		stp9_status = 1;
		EXTI_ClearITPendingBit(EXTI_Line9);  
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line10) )
	{
		stp10_status = 1;
		EXTI_ClearITPendingBit(EXTI_Line10);  
	} 
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		stp_status = 1;
		EXTI_ClearITPendingBit(EXTI_Line11);  
	}
}

