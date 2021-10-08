#ifndef __Host_Control_H
#define __Host_Control_H

#include "stm32f10x.h"

void uart_irq(void);
void uart_time_send(int *stp_ms);
	
#endif

