#include "stm32f10x.h"                  // Device header


/*
引脚和通道必须匹配

PA0 ↔ TIM2_CH1 ↔ TIM_OC1Init + TIM_SetCompare1

PA1 ↔ TIM2_CH2 ↔ TIM_OC2Init + TIM_SetCompare2

避免特殊功能引脚

PA13( SWDIO)、PA14(SWCLK) → 调试用，别占

PA11(USB_DM)、PA12(USB_DP) → 有USB就别用

优先用 PB、PC 等普通 GPIO

PWM 参数设计要合理

直流电机：20kHz（无啸叫），ARR=99（占空比直观）

舵机：50Hz，ARR=19999（高精度）

分步排查法

先测方向引脚 → 再测 PWM 引脚 → 最后联调

不确定时用最简单代码独立测试每个模块
*/


/**
  * @brief PWM初始化
  * @param 无
  * @retval 无
  */
void PWM_Init(void)
{
	//注意是定时器使能！不是定时器复位
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    // 关键是这两行！启用AFIO时钟并关闭JTAG
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  // 关闭JTAG，保留SWD
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//输出模式改为复用推挽输出（使用定时控制片上外设）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 使用引脚（多引脚用或连接）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率50Hz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	
	//使用内部时钟
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//检测模式
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR			//定时器溢出频率
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;	//PSC					//预分频和自动重装设置
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//高级定时器计数器，不使用0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;				//CCR占空比为50%
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
//	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	//A0通道1 A1通道2 A2通道3 A3通道4

	TIM_Cmd(TIM2, ENABLE);			
}

/**
  * @brief PWM调整占空比
  * @param uint16_t Compare 		// 
  * @retval 无
  */
void PWM_SetCompare1(uint16_t Compare){
	TIM_SetCompare1(TIM2, Compare);
}
