#include "Steering_engine.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_spi.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
文件名		: Steering_engine.c
作者	  	: 张博钊
版本	   	: V1.0
描述	   	: AD采集交流，上位机发送指令，采用PID通过PWM操控舵机闭环调节到目标电压 / 上位机发送指令 采用开环调节
其他	   	: 无
网站 	   	: www.elsintech.com
日志	   	: 初版V1.0 2020/6/15 张博钊创建
***************************************************************/

struct {
	u8 kp;
	u8 ki;
	u8 kd;	// p i d 参数
	u16 error;//当前误差
  u16 last_error;//上次误差
	u16 pre_error;//上上次误差
	u16 pwm;
} PID;

float pwm_accuracy = 0.000305; // 500-2500 每1个pwm值对应的调节电压倍率系数  

void Steering_engine_Init(void)
{
	TIM2_PWM_Init(19999,35);  //f = 36M / (19999+1) / (35 + 1)  = 50HZ , T = 20ms
	//ADS1118_Init();
}

int OpenLoop_Control(float in_voltage, float out_voltage)
{		
	int pwm_count = 0;
	
	pwm_count = (out_voltage / in_voltage - 0.509) / pwm_accuracy; //所需pwm数（以pwm = 500 即降压最低为基准）
	
	PID.pwm = 500 + pwm_count;
	
	if(PID.pwm < 500)	
		PID.pwm = 500;
	if(PID.pwm > 2500)
		PID.pwm = 2500;
	
	return PID.pwm;

}

void PID_Control(u16 goal_voltage)
{

	u16 cur_voltage = get_Voltage();	//获取当前电压
	
	PID.error = goal_voltage - cur_voltage;	//计算当前误差
	
	PID.kp = 1;
	PID.kd = 1;
	
	PID.pwm += PID.kp * PID.error + PID.kd * (PID.error - PID.last_error);//舵机pd控制
	
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
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE); //Timer2部分重映射  TIM2_CH4->PB11  
 
   //设置该引脚为复用输出功能,输出TIM2 CH4的PWM脉冲波形	GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断
	
	//初始化TIM2 Channe4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1  count < ccr4 值时 为高电平 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2

}

void ADS1118_Init(void)
{
	//SPI 设置
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SPI_Cmd(SPI1, DISABLE);//配置之前先关闭SPI接口

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// CPOL=0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//CPHA=1
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//CS设置为软件配置
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;//通信速率
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);//配置完成使能SPI接口
	
	//GPIO设置
	

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
	Data_REG=0xFFFF-Data_REG;//把0xFFFF改成0x10000
	ADS1118_Voltage=(-1.0)*((Data_REG*2/0x8000)); //量程为默认的2.048v 近似为2v
	}
	else ADS1118_Voltage=(1.0)*((Data_REG*2/32768));
	
	return ADS1118_Voltage;
}

//读写寄存器16bits模式
u16 SPI_read_write_Reg(uint16_t CofigReg)
{
	u8 getdata = 0, getdata1 = 0, getdata2 = 0;
	getdata1 = SPI_send_Byte((uint8_t)(CofigReg>>8));
	getdata2 = SPI_send_Byte((uint8_t)CofigReg);

	getdata = (uint16_t)getdata2|((uint16_t)getdata1<<8);

	return getdata;
}
//发送一个字节
u8 SPI_send_Byte(uint8_t byte)
{
 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
	
	SPI_I2S_SendData(SPI1, byte);
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
	
	return SPI_I2S_ReceiveData(SPI1);
}

