#include "LED_Control.h"
#include "stm32f10x_gpio.h"
#include "Host_Control.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
文件名		: LED_Control.c
作者	  	: 张博钊
版本	   	: V1.0
描述	   	: 上位机发送指令，由下位机控制LED
其他	   	: 无
网站 	   	: www.elsintech.com
日志	   	: 初版V1.0 2020/6/15 张博钊创建
***************************************************************/

u8 YLW_status = 0, GRN_status = 0, RED_status = 0;

void LED_Control_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能GPIOA 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	 //根据参数初始化GPIOA 1-3
	
	YLW = 1;
	GRN = 1;
	RED = 1;		//NPN三极管  发射极低电平时灯负极为高电平 灯灭
	
	
	
}

void LED_Control(u8 *get_data)
{
	if(get_data[1] == 0x00) YLW = 0;
	if(get_data[1] == 0x01)	YLW = 1;
	
	if(get_data[1] == 0x02)	GRN = 0;
	if(get_data[1] == 0x03)	GRN = 1;
	
	if(get_data[1] == 0x04)	RED = 0;
	if(get_data[1] == 0x05)	RED = 1;
}

