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
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  // 改成 TIM4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);  // TIM4
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);              // TIM4
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);              // TIM4
    
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,  // TIM4
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    TIM_Cmd(TIM4, ENABLE);                               // TIM4
}

int16_t Encoder_Get(void)
{
    int16_t Temp;
    Temp = TIM_GetCounter(TIM4);     // TIM4
    TIM_SetCounter(TIM4, 0);         // TIM4
    return Temp;
}