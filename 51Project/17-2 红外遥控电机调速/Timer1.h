#ifndef __TIMER1_H__
#define __TIMER1_H__

void Timer1Init(void);
void Timer1_SetCounter(unsigned int Value);
unsigned int Timer1_GetCounter(void);
void Timer1_Run(unsigned char Flag);

#endif