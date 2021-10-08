#ifndef __IO_Timer_H
#define __IO_Timer_H

#include "stm32f10x.h"
#include "sys.h"
//PB 0-10
#define	TM_STP0		GPIO_Pin_0   //°´Å¥Î»
#define	TM_STP1		GPIO_Pin_1
#define	TM_STP2		GPIO_Pin_2
#define	TM_STP3		GPIO_Pin_3
#define	TM_STP4		GPIO_Pin_4
#define	TM_STP5		GPIO_Pin_5
#define	TM_STP6		GPIO_Pin_6
#define	TM_STP7		GPIO_Pin_7
#define	TM_STP8		GPIO_Pin_8
#define	TM_STP9		GPIO_Pin_9
#define	TM_STP10	GPIO_Pin_10
//PA
#define STP				GPIO_Pin_11		

void IO_Timer_Init(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void get_stp_status(void);
//void TIM3_IRQHandler(void);

//void EXTI0_IRQHandler(void);
//void EXTI1_IRQHandler(void);
//void EXTI2_IRQHandler(void);
//void EXTI3_IRQHandler(void);
//void EXTI4_IRQHandler(void);
//void EXTI9_5_IRQHandler(void);
//void EXTI15_10_IRQHandler(void);




#endif

