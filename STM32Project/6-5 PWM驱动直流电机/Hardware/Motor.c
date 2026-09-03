#include "stm32f10x.h"                  // Device header
#include "PWM.h"


/*
初始化用于驱动直流电机方向的引脚
1 0 正转
0 1 反转
0 0 静止
1 1 静止
*/
void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
    
    PWM_Init();
}

void Motor_SetSpeed(int8_t Speed)
{
    if (Speed == 0) {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        GPIO_ResetBits(GPIOB, GPIO_Pin_13);
        PWM_SetCompare1(0);
    }
    else if (Speed > 0) {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        GPIO_ResetBits(GPIOB, GPIO_Pin_13);
        PWM_SetCompare1(Speed);
    }
    else {
        GPIO_SetBits(GPIOB, GPIO_Pin_13);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        PWM_SetCompare1(-Speed);
    }
}
