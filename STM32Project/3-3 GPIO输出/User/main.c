#include "stm32f10x.h"                  // Device header

// 函数返回类型设置为空
void Delay(uint32_t count){
	while(count--);
}

int main(void)
{
//	int i,j,TIME;
	int TIME=250;
//	int a = ENABLE;	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	

//	FunctionalState a;
//	a = ENABLE;
//	a = DISABLE;
//	a = 100;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 	// PB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 引脚初始化结构体 GPIO_InitStructure，引脚号 GPIOA
	// GPIOA：一组独立的16个引脚（PA0-PA15），GPIOB：另一组独立的16个引脚（PB0-PB15），分别被分成两个主要区域
	// GPIOB的宏定义
	// #define RCC_APB2Periph_GPIOB           ((uint32_t)0x00000008)
	// RCC_APB2Periph 为可控制的引脚寄存器口
	// currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
	// 这里的GPIO_InitTypeDef*就是用的typedef struct引出的三个可用参数
	// uint16_t GPIO_Pin; GPIOSpeed_TypeDef GPIO_Speed; GPIOMode_TypeDef GPIO_Mode;
	// 但需要&GPIO_InitStructure取指针操作取出参数的物理地址
	
	// 初始化GPIOB
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);

    while(1){
        GPIO_SetBits(GPIOB, GPIO_Pin_0);   // PB0输出高电平
        Delay(TIME);
        GPIO_ResetBits(GPIOB, GPIO_Pin_0); // PB0输出低电平
        Delay(TIME);
    }
}
