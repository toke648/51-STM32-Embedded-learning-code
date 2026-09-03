#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/*
step1 配置GPIO，开启时钟
step2 GPIO初始化，配置输入模式
step3 配置时基单元，让CNT计数器在内部时钟的驱动下自增运行
step4 配置输入捕获单元。滤波器、极性、直连通道/交叉通道、分频器
step5 选择从模式的触发源，TI1FP1

*/
void IC_Init(void)
{
	//注意是定时器使能！不是定时器复位
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//根据引脚时钟对应表，GPIOA定时器需要使用PIN6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	//根据手册重定义TIM
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//输出模式改为复用推挽输出（使用定时控制片上外设）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // 使用引脚（多引脚用或连接）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率50Hz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//使用内部时钟
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//检测模式
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR		//设置大一些，防止定时器溢出
	//提高测频上下限设置
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;	//PSC			//预分频和自动重装设置
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//高级定时器计数器，不使用0
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;				//滤波器，过滤噪声 0x0 0xF
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;				//极性，上升沿触发或下降沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					//分配器设置，不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;			//直连通道
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
	
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;				//滤波器，过滤噪声 0x0 0xF
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;				//极性，上升沿触发或下降沿触发
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					//分配器设置，不分频
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;			//直连通道
//	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);							//触发沿设置
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3, ENABLE);		
}


uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) * 100 / (TIM_GetCapture1(TIM3) + 1)) + 1;
}
