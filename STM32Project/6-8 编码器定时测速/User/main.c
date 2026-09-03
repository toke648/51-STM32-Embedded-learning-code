#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Speed;

int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "Speed:");		//CNT 计数器
	
	while(1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);
		Delay_ms(100);
	}
}

/*

使用定时器定时读取时钟
*/
void TIM2_IRQHandler(void)
{
	//如果TIM当前状态为使能，就清空TIM计时器
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encoder_Get();
//		TIM_SetCounter(TIM3, 0);       // 清零编码器计数器，下一周期重新算
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

//int main(void)
//{
//	OLED_Init();
//	Encoder_Init();
//	
//	OLED_ShowString(1, 1, "CNT:");		//CNT 计数器
//	
//	while(1)
//	{
//		OLED_ShowSignedNum(1, 5, Encoder_Get(), 5);
//		Delay_ms(1000);
//	}
//}


/*

int main(void)
{
    OLED_Init();
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    OLED_ShowString(1, 1, "PA6: PA7:");
    
    while(1)
    {
        OLED_ShowNum(2, 1, GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6), 1);
        OLED_ShowNum(2, 8, GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7), 1);
        Delay_ms(100);
    }
}

*/