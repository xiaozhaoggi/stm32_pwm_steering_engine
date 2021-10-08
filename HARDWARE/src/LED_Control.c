#include "LED_Control.h"
#include "stm32f10x_gpio.h"
#include "Host_Control.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
�ļ���		: LED_Control.c
����	  	: �Ų���
�汾	   	: V1.0
����	   	: ��λ������ָ�����λ������LED
����	   	: ��
��վ 	   	: www.elsintech.com
��־	   	: ����V1.0 2020/6/15 �Ų��ȴ���
***************************************************************/

u8 YLW_status = 0, GRN_status = 0, RED_status = 0;

void LED_Control_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOA �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	 //���ݲ�����ʼ��GPIOA 1-3
	
	YLW = 1;
	GRN = 1;
	RED = 1;		//NPN������  ���伫�͵�ƽʱ�Ƹ���Ϊ�ߵ�ƽ ����
	
	
	
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

