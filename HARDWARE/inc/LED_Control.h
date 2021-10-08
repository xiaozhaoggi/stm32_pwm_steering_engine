#ifndef __LED_Control_H
#define __LED_Control_H

#include "stm32f10x.h"
#include "sys.h"


#define YLW 	PAout(1)
#define GRN		PAout(2)
#define RED		PAout(3)

void LED_Control_Init(void);
void LED_Control(u8 * get_data);

#endif

