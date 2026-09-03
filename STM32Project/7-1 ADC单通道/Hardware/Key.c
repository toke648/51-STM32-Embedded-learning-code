#include "stm32f10x.h"
#include "Delay.h"

static uint8_t Key_LastState = 1;  // 静态变量，只在本文件可见

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief 获取按键值（下降沿触发，按下瞬间返回1）
  * @retval 1: 按键被按下，0: 无按键
  */
uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;
    uint8_t current = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
    
    // 检测下降沿：上次高 → 这次低
    if(Key_LastState == 1 && current == 0)
    {
        Delay_ms(20);
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
        {
            KeyNum = 1;
        }
    }
    
    Key_LastState = current;
    return KeyNum;
}
