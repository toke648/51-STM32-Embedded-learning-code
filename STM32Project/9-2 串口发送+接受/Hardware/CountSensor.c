#include "stm32f10x.h"                  // Device header

// 配置中断函数
// 如果不打开时钟控制是没法工作的
// 1.RCC

// 2.配置GPIO，端口选择为输入模式

// 3.配置AFIO,选择我们这一路GPIO，连接到后面的EXTI

// 4.配置EXTI边缘触发方式，比如上升沿、下降沿或者双边沿
// 以及选择触发方式（中断响应/触发响应）
// 一般来讲都是终端响应

// 5.配置NVIC，中断选择一个适合的优先级
// 最后，通过NVIC外部中断信号就能进入CPU了

//void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
//void GPIO_EventOutputCmd(FunctionalState NewState);
//void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);
//void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
//void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface);


// GPIO_PinLockConfig 用于锁定GPIO配置

// GPIO_EventOutputConfig 用于配置AFIO的实践输出功能
// GPIO_EventOutputCmd

// GPIO_PinRemapConfig 用来进行引脚重映射
// GPIO_EXTILineConfig 外部中断需要用的函数 —— 本节课的

// GPIO_ETH_MediaInterfaceConfig 以太网有关

uint16_t CountSensor_Count;			//中断计次

/**
  * @brief  
  * @param 
  * @param 
  * @retval 
  */
void CountSensor_Init(void)
{
	// 1.配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 此处设置为开启AFIO2开启时钟控制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//结构体定义
	GPIO_InitTypeDef GPIO_StructInit;
	GPIO_StructInit.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_StructInit.GPIO_Pin = GPIO_Pin_14; // 使用引脚（多引脚用或连接）
	GPIO_StructInit.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率50Hz
	GPIO_Init(GPIOB, &GPIO_StructInit);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	//结构体定义
	EXTI_InitTypeDef EXTI_StructIniture;
	EXTI_StructIniture.EXTI_Line=EXTI_Line14;				//中断通道引脚
	EXTI_StructIniture.EXTI_LineCmd=ENABLE;					//启动中断
	EXTI_StructIniture.EXTI_Mode=EXTI_Mode_Interrupt;		//使用中断模式
//	EXTI_StructIniture.EXTI_Trigger=EXTI_Trigger_Falling;	//下降沿触发
	EXTI_StructIniture.EXTI_Trigger=EXTI_Trigger_Rising;	//上升沿触发
	EXTI_StructIniture.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//下降+上升沿触发
	//取地址+初始化赋值
	EXTI_Init(&EXTI_StructIniture);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//两位抢占
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;		//使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级配置，优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void){
	return CountSensor_Count;
}

//中断函数
void EXTI15_10_IRQHandler(void){
	//只要中断标志位置1，就启动中断
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count++;		//中断计次
		EXTI_ClearITPendingBit(EXTI_Line14); //清除中断标志位
	}
}
