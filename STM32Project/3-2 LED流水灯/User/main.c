#include "stm32f10x.h"                  // Device header
#include "Delay.h" 

// 延时
void Delay(uint32_t count){
	while(count--);
}

// GPIO初始化
int main(void)
{
	int i,j,TIME;
	TIME=250;
	
	GPIO_InitTypeDef GPIO_InitStructure; // GPIO初始化结构体地址
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 启动时钟控制
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; // 引脚设置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率控制
	GPIO_Init(GPIOA, &GPIO_InitStructure); // 对GPIO结构体地址进行指针调取操作&GPIO_InitStructure
	
	while(1)
	{
		// Pin_0 的16进制的二进制表示，为 0000 0000 0000 0001
		// 转换为十进制表示，即为 0x0001
		GPIO_WriteBit(GPIOA, 0x0001, Bit_RESET);
		Delay_ms(250);
		GPIO_WriteBit(GPIOA, 0x0001, Bit_SET);
		Delay_ms(500);
		
		// Pin_1 的16进制的二进制表示，为 0000 0000 0000 0010
		// 转换为十进制表示，即为 0x0002
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
		Delay_ms(500);
		
		// Pin_2 的16进制的二进制表示，为 0000 0000 0000 0011
		// 转换为十进制表示，即为 0x0003
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		Delay_ms(500);
		
		// 以此类推，这些就是引脚在寄存器中的地址
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
		Delay_ms(500);
		
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
		Delay_ms(500);
		
	}
}
