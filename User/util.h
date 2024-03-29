#ifndef __UTIL_H__
#define __UTIL_H__

#include "stm32f4xx.h"

#define range(x, a, b)		(min(max(x, a), b))
#define exchange(a, b, tmp) (tmp=a, a=b, b=tmp)

void rcc_clock_enable(void);
void print_clock_freq(void);
void nvic_config(uint8_t irq_channel, uint8_t priority);

#endif
