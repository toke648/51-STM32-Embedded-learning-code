#include "stm32f10x.h"                  // Device header

extern uint16_t Num;					//使用外部变量

/*


定时器溢出频率 = TIMxCLK / (Prescaler + 1) / (Period + 1)

关键设计思想：

预分频器 7200 产生约 10kHz 的计数频率

自动重载 9999（10000个脉冲）正好得到 1Hz

两个参数相乘 ≈ 72,000,000，正好是 72MHz，利用了整数倍关系


72,000,000 = 1Hz × 7201 × 10000
          ↑        ↑      ↑
      目标频率   PSC+1   ARR+1
*/

void Timer_Init(void)
{
	//注意是定时器使能！不是定时器复位
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//使用内部时钟
//	TIM_InternalClockConfig(TIM2);
	
	GPIO_InitTypeDef GPIO_InitTypeStructure;
	GPIO_InitTypeStructure.GPIO_Mode = GPIO_Mode_IPU;			//上拉输入
	GPIO_InitTypeStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitTypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitTypeStructure);
	
	
	//改用外部时钟
	//外部时钟模式1和外部时钟模式2的区别在于，
	//外部时钟模式1使用外部输入脚(TIx)作为计数器时钟的时钟源，
	//而外部时钟模式2使用外部触发输入(ETR)或内部触发输入(ITRx)作为计数器时钟的时钟源1。
	//大家注意，0x00无滤波 波动大，up文件中还是用的0xFF
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//检测模式
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;					//定时器溢出频率
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;						//预分频和自动重装设置
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//高级定时器计数器，不使用0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_IT_Update);						//清空定时器中断标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 				//定时器使能中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);									//定时器使能
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}

//void TIM2_IRQHandler(void)
//{
//	//如果TIM当前状态为使能，就清空TIM计时器
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Num++;
//		
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}
