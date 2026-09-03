#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "PWM.h"
#include "IC.h" 

uint16_t i;


/*
      ←   周期（通道1捕获）  →
      ┌─────────────────────┐
      │   高电平（通道2捕获） │              │
      └──────┘              └──────────────┘
      ↑上升沿               ↑下降沿        ↑上升沿
    
    占空比 = 高电平时间 / 周期 × 100%
           = CCR2 / CCR1 × 100%

*/
int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Duty:00%");			//显示占空比
	
	/*
	PWM 频率 = 72MHz / (50+1) / 100 = 72MHz / 51 / 100 ≈ 14117Hz  ← 不对

	ARR + 1 = 100
	PSC + 1 = 72MHz / 频率 / 100 = 72000000 / 1428 / 100 ≈ 504

	实际 PSC 约 503
	*/
	PWM_SetPrescaler(720 - 1);		// Freq = 72M / (PSC + 1) / ARR + 1
	PWM_SetCompare1(50);			//Duty = CCR / ARR + 1
	
	//80%
	PWM_SetPrescaler(7200 - 1);		// Freq = 72M / (PSC + 1) / ARR + 1
	PWM_SetCompare1(80);			//Duty = CCR / ARR + 1
	
	
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(2, 6, IC_GetDuty(), 2);

	}
}


