#include "stm32f10x.h"                  // Device header

// 头文件第8295行的
// #ifdef USE_STDPERIPH_DRIVER
//  #include "stm32f10x_conf.h"
// #endif
// 表示头文件编译
// 如果定义了USE_STDPERIPH_DRIVER这个字符串
// 下面的include定义才有效

int main(void)
{
	// 输入输出寄存器配置
	// 时钟控制RCC的配置（APB2外设时钟）
	
	// 转化为16进制 每四个为一位
	// 根据参数手册说明，第7位写1为打开时钟，既0x00000010，全零为关闭时钟
	RCC->APB2ENR = 0x00000010; 

	// 端口配置高寄存器（GPIO x CRH）CNF13 Mode13 为0011
	// 根据手册说明，需要将配置为CNF通用推挽输出模式，也就是00
	// 即为00 00 00 00 00 11 00 00 00 00 00 00 00 00 00 00 00 00
	// 四个为一位转换为二进制为，0030 0000
	GPIOC->CRH = 0x00300000;
	
	// 端口输出数据寄存器（GPIOx_ODR）（x=A..E）
	// ODR13 写1，13号口就是高电平，写0就是低电平
	// ODR 0x00002000就是灭
	// ODR 0x00000000就是亮
	GPIOC->ODR = 0x00000000; // ODR13写1高电平，写0低电平
	while(1)
	{
		
	}
}
