#include "stm32f10x.h"                  // Device header
#include "Delay.h" 

void Delay(uint32_t count){
	while(count--);
}

// GPIO初始化
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 启动时钟控制
	// 改成GPIOB控制蜂鸣器（因为使用的是GPIOB_12号引脚）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 需要进行蜂鸣器的控制，因为引脚号为Bx，因此使用GPIOB模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while(1)
	{
		
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
		// 设置为500则换算为100毫秒（芯片的时钟控制频率为72200，72200/500 = ）
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
		Delay_ms(700);
	}
}
