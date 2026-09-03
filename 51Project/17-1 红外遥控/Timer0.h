#ifndef __TIMER_H__
#define __TIMER_H__

void Timer0Init(void);
void Timer0_SetCounter(unsigned int Value);
unsigned int Timer0_GetCounter(void);
void Timer0_Run(unsigned char Flag);

#endif