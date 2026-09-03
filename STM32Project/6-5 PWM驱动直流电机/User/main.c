#include "stm32f10x.h"
#include "Delay.h" 
#include "OLED.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum;
float Angle;
int8_t Speed;

int main(void)
{
    OLED_Init();
	Motor_Init();
	Key_Init();
    
    OLED_ShowString(1, 1, "Motor Speed");
    Motor_SetSpeed(0);
	
//	GPIO_SetBits(GPIOB, GPIO_Pin_12);
//	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	
    while(1)
    {
		// 检测引脚是否触发下降沿（即 高电平接地）
		KeyNum = Key_GetNum();
		if (KeyNum == 1){
			Speed += 20;
			if (Speed > 100)
			{
				Speed = -100;
			}
		}Motor_SetSpeed(Speed);
		
		OLED_ShowSignedNum(2, 1, Speed, 3);
		Delay_ms(10);    // 关键！给一点延时，不要疯狂循环
	}
}




//#include "stm32f10x.h"
//#include "Delay.h" 
//#include "OLED.h"
//#include "Motor.h"

//uint8_t KeyNum;
//float Angle;

//int main(void)
//{
//    // 初始化方向引脚 PB12, PB13
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    
//    GPIO_SetBits(GPIOB, GPIO_Pin_12);    // IN1 高
//    GPIO_ResetBits(GPIOB, GPIO_Pin_13);  // IN2 低 → 正转方向
//    
//    // 初始化 PWM - PA0, 50%占空比
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//    
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
//    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
//    
//    TIM_OCInitTypeDef TIM_OCInitStructure;
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 0;    // 80%占空比，力度大容易看出来
//    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//    
//    TIM_Cmd(TIM2, ENABLE);
//    
//    while(1);
//}
