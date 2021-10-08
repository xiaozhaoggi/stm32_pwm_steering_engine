#ifndef __Steering_engine_H
#define __Steering_engine_H

#include "stm32f10x.h"

#define ADS1118_CONFIG_VALUE	0x8583 



void Steering_engine_Init(void);
void TIM2_PWM_Init(u16 arr,u16 psc);
void ADS1118_Init(void);
void PID_Control(u16 goal_voltage);
int OpenLoop_Control(float in_voltage, float out_voltage);
u16 get_Voltage(void);
u8 SPI_send_Byte(uint8_t byte);
u16 SPI_read_write_Reg(uint16_t CofigReg);

#endif

