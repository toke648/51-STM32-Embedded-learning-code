#include "stm32f10x.h"                  // Device header


/*
外部输入高电平，用上拉输入
外部输入低电平，用下拉输入
浮空输入，外部输入功率非常小（浮空输入没有上拉电阻和下拉电阻去影响外部信号）
缺点是外部引脚悬空时将会没有电平，输入会受到噪声的干扰，来回不断的跳变
（浮空输入，引脚是高阻态，电平不稳定，这很不好）
*/
void Encoder_Init(void)
{
	/*TIM3*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;        // 不分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    
    // 通道1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   // 必须写
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    // 通道2
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   // 必须写
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    // TI12 = 两个通道都计数，4倍频
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, 
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);	//反转极性
    
    TIM_Cmd(TIM3, ENABLE);
	

	/*TIM4*/
	/*
	重复的结构体定义可以复用
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;        // 不分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
    
    TIM_ICStructInit(&TIM_ICInitStructure);
    
    // 通道1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   // 必须写
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    
    // 通道2
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   // 必须写
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    
    // TI12 = 两个通道都计数，4倍频
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    TIM_Cmd(TIM4, ENABLE);
}

//通过将uint16 无符号整形，改成int16 有符号整形，可以实现正负的读取
int16_t Encoder_Get(uint8_t n)
{
	int16_t Temp;
	if (n == 1)
	{
		Temp = TIM_GetCounter(TIM3);
		TIM_SetCounter(TIM3, 0);		// 重置定时器
		return Temp;
	}
	else if (n == 2)
	{
		Temp = TIM_GetCounter(TIM4);
		TIM_SetCounter(TIM4, 0);		// 重置定时器
		return Temp;
	}
	return 0;
}


////通过将uint16 无符号整形，改成int16 有符号整形，可以实现正负的读取
//int16_t Encoder_Get(void)
//{
//	//TIM_GetCounter				读取TIM计数器
//	return TIM_GetCounter(TIM3);
//}
