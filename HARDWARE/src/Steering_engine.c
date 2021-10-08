#include "Steering_engine.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_spi.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
�ļ���		: Steering_engine.c
����	  	: �Ų���
�汾	   	: V1.0
����	   	: AD�ɼ���������λ������ָ�����PIDͨ��PWM�ٿض���ջ����ڵ�Ŀ���ѹ / ��λ������ָ�� ���ÿ�������
����	   	: ��
��վ 	   	: www.elsintech.com
��־	   	: ����V1.0 2020/6/15 �Ų��ȴ���
***************************************************************/

struct {
	u8 kp;
	u8 ki;
	u8 kd;	// p i d ����
	u16 error;//��ǰ���
  u16 last_error;//�ϴ����
	u16 pre_error;//���ϴ����
	u16 pwm;
} PID;

float pwm_accuracy = 0.000305; // 500-2500 ÿ1��pwmֵ��Ӧ�ĵ��ڵ�ѹ����ϵ��  

void Steering_engine_Init(void)
{
	TIM2_PWM_Init(19999,35);  //f = 36M / (19999+1) / (35 + 1)  = 50HZ , T = 20ms
	//ADS1118_Init();
}

int OpenLoop_Control(float in_voltage, float out_voltage)
{		
	int pwm_count = 0;
	
	pwm_count = (out_voltage / in_voltage - 0.509) / pwm_accuracy; //����pwm������pwm = 500 ����ѹ���Ϊ��׼��
	
	PID.pwm = 500 + pwm_count;
	
	if(PID.pwm < 500)	
		PID.pwm = 500;
	if(PID.pwm > 2500)
		PID.pwm = 2500;
	
	return PID.pwm;

}

void PID_Control(u16 goal_voltage)
{

	u16 cur_voltage = get_Voltage();	//��ȡ��ǰ��ѹ
	
	PID.error = goal_voltage - cur_voltage;	//���㵱ǰ���
	
	PID.kp = 1;
	PID.kd = 1;
	
	PID.pwm += PID.kp * PID.error + PID.kd * (PID.error - PID.last_error);//���pd����
	
	if(PID.pwm < 500)	
		PID.pwm = 500;
	if(PID.pwm > 2500)
		PID.pwm = 2500;
	
	TIM_SetCompare4(TIM2,PID.pwm);
		
	PID.pre_error = PID.last_error;
	PID.last_error = PID.error;
	
	
}

void TIM2_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE); //Timer2������ӳ��  TIM2_CH4->PB11  
 
   //���ø�����Ϊ�����������,���TIM2 CH4��PWM���岨��	GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�
	
	//��ʼ��TIM2 Channe4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1  count < ccr4 ֵʱ Ϊ�ߵ�ƽ 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2

}

void ADS1118_Init(void)
{
	//SPI ����
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SPI_Cmd(SPI1, DISABLE);//����֮ǰ�ȹر�SPI�ӿ�

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// CPOL=0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//CPHA=1
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//CS����Ϊ�������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;//ͨ������
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);//�������ʹ��SPI�ӿ�
	
	//GPIO����
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);
	
	//SPI1 NSS 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// Pin PB13 (SCLK) 50MHz push pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// Pin PB14 (MISO) input pull-up
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// Pin PB15 (MOSI) 50MHz push pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
}

u16 get_Voltage(void)
{	
	u16 ADS1118_Voltage;
	u16 Data_REG = SPI_read_write_Reg(ADS1118_CONFIG_VALUE);
	if(Data_REG>=0x8000)
	{
	Data_REG=0xFFFF-Data_REG;//��0xFFFF�ĳ�0x10000
	ADS1118_Voltage=(-1.0)*((Data_REG*2/0x8000)); //����ΪĬ�ϵ�2.048v ����Ϊ2v
	}
	else ADS1118_Voltage=(1.0)*((Data_REG*2/32768));
	
	return ADS1118_Voltage;
}

//��д�Ĵ���16bitsģʽ
u16 SPI_read_write_Reg(uint16_t CofigReg)
{
	u8 getdata = 0, getdata1 = 0, getdata2 = 0;
	getdata1 = SPI_send_Byte((uint8_t)(CofigReg>>8));
	getdata2 = SPI_send_Byte((uint8_t)CofigReg);

	getdata = (uint16_t)getdata2|((uint16_t)getdata1<<8);

	return getdata;
}
//����һ���ֽ�
u8 SPI_send_Byte(uint8_t byte)
{
 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
	
	SPI_I2S_SendData(SPI1, byte);
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
	
	return SPI_I2S_ReceiveData(SPI1);
}

