#include "stm32f10x.h"                  // Device header

int mian(void)
{
	
	// 这是一个被封装好了的函数，通过函数封装的库寄存器操作
	// 可以更加便捷的管理，而不用查找手册对寄存器直接操作了
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	// 查看函数定义，发现该函数具有三种方法
	// GPIO_Mode，GPIO_Pin，GPIO_Speed
	// 分别对这三种方法，使用搜索查看函数定义，得到参数
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// 根据GPIO库文件定义，GPIO_InitTypeDef为一个指针操作
	// 因此对函数进行取针操作&
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 设置端口高低电平实现电灯操作
	GPIO_SetBits(GPIOC, GPIO_Pin_13); // 对GPIO13号引脚设置高电平
	//GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 将Pin13设置为低电平
	while(1)
	{
		
	}
}
