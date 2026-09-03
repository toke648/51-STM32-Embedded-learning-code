#include "stm32f10x.h"                  // Device header


/*
什么是 ARR？（自动重装载寄存器）
ARR 决定了定时器"数到多少算一个周期"，也就是 PWM 的分辨率。
想象一个秒表：
从 0 开始向上数
数到 99（ARR的值），归零重来
每归零一次，就是一个 PWM 周期
CNT: 0→1→2→3→...→99→0→1→2→...  （无限循环）

什么是 CCR？（捕获/比较寄存器）
CCR 决定了"在这个周期的什么位置翻转电平"，也就是占空比。
ARR = 99，CCR = 50：

CNT: 0 ───────── 50 ───────── 99 → 0
PA15: ████████████▌──────────────  （高电平50个计数，低电平49个计数）
      ← 高电平 →  ←  低电平  →
      
占空比 = 50 / (99+1) = 50%


什么是 PSC？（预分频器）
PSC 决定了定时器"每秒钟数多少次"。

时钟频率 = 72MHz / (PSC + 1) = 72MHz / 720 = 100kHz
每个计数 = 1 / 100kHz = 0.01ms = 10μs

PWM频率 = 100kHz / (ARR + 1) = 100kHz / 100 = 1kHz
PWM周期 = 1ms



*/


void PWM_Init(void)
{
	//注意是定时器使能！不是定时器复位
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	//根据手册重定义TIM
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//输出模式改为复用推挽输出（使用定时控制片上外设）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 使用引脚（多引脚用或连接）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率50Hz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
	//使用内部时钟
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//检测模式
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR			//定时器溢出频率
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;	//PSC					//预分频和自动重装设置
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//高级定时器计数器，不使用0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;				//CCR占空比为50%
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);			
}

/*
调整占空比

*/
void PWM_SetCompare1(uint16_t Compare){
	TIM_SetCompare1(TIM2, Compare);
}

/*
重装载值，改变频率
*/
void PWM_SetPrescaler(uint16_t Perscaler)
{
	TIM_PrescalerConfig(TIM2, Perscaler, TIM_PSCReloadMode_Immediate);
}
