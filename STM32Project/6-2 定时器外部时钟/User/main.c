#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "Timer.h"

uint16_t Num;

int main(void)
{
	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "Num");
	OLED_ShowString(1, 1, "CNT");
	
	while(1)
	{
		OLED_ShowNum(1,5,Num,5);
		//更改TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;					//定时器溢出频率
		//可以改变定时器计数速度
		OLED_ShowNum(2,5,Timer_GetCounter(),5);
	}
}

void TIM2_IRQHandler(void)
{
	//如果TIM当前状态为使能，就清空TIM计时器
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num++;
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
