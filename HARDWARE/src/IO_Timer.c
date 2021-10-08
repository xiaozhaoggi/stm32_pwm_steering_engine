#include "IO_Timer.h"
#include "stm32f10x_tim.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
文件名		: IO_Timer.c
作者	  	: 张博钊
版本	   	: V1.0
描述	   	: 处理上位机发送的离散量
其他	   	: 无
网站 	   	: www.elsintech.com
日志	   	: 初版V1.0 2020/6/15 张博钊创建
***************************************************************/

int ms = 0;
u8 stp0_status = 0, stp1_status = 0, stp2_status = 0, stp3_status = 0, stp4_status = 0, stp5_status = 0, 
	stp6_status = 0, stp7_status = 0, stp8_status = 0, stp9_status = 0, stp10_status = 0, stp_status = 0;//端口状态变量
int stpn_secs = 0;//端口计时时间秒变量
int stpn_ms = 0;//端口计时时间变量  
u8 send_time_flag = 0; //发送定时时间标志位


//stp 0 - 6 为开始位  stp 7 -10 为停止位
void IO_Timer_Init(void) //初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOA B 端口时钟
	
	
	//GPIO初始化
	GPIO_InitStructure.GPIO_Pin  = TM_STP0 | TM_STP1 | TM_STP2 | TM_STP3 | TM_STP4 | TM_STP5 | TM_STP6 | TM_STP7 | TM_STP8 | TM_STP9 | TM_STP10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB 0-10
	
	GPIO_InitStructure.GPIO_Pin  = STP;//PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA 11
	

//	//GPIOB 0-10 中断线以及中断初始化配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0 | GPIO_PinSource1 | GPIO_PinSource2 | GPIO_PinSource3 | GPIO_PinSource4 
																					| GPIO_PinSource5 | GPIO_PinSource6 | GPIO_PinSource7 | GPIO_PinSource8 | GPIO_PinSource9 | GPIO_PinSource10);
	//GPIOA 11
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	
	EXTI_InitStructure.EXTI_Line =EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4 
																| EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9 | EXTI_Line10 | EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	
	NVIC_InitStructure.NVIC_IRQChannel =EXTI0_IRQn;			//使能IO所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能IO所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能IO所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能IO所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能IO所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能IO所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能IO所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	TIM3_Int_Init(49, 3599);		//10ms 进一次中断
}
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

void get_stp_status(void)	//获取端口状态
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

void TIM3_IRQHandler(void)   //TIM3中断
{   
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
		
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

//GPIO外部中断
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

