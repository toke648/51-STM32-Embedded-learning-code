//#include "stm32f10x.h"                  // Device header

//// 函数返回类型设置为空
//void Delay(uint32_t count){
//	while(count--);
//}

//int main(void)
//{
//	int i,j,TIME;
//	TIME=250;
//	
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	while(1)
//	{
//		// 类似PWD控制效果实现
//		
//		// 从暗到亮
//		for(i = 0; i < 2000; i++) {
//			GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 亮
//			Delay(i);                          // 亮的时间越来越长
//			GPIO_SetBits(GPIOA, GPIO_Pin_0);   // 灭
//			Delay(2000 - i);                    // 灭的时间越来越短
//		}
//		// 从亮到暗
//		for(i = 0; i < 2000; i++) {
//			GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 亮
//			Delay(2000 - i);                    // 亮的时间越来越短
//			GPIO_SetBits(GPIOA, GPIO_Pin_0);   // 灭
//			Delay(i);                          // 灭的时间越来越长
//		}
//		
//		
//		// 亮灯交替闪烁效果实现
//		
//		
//		// 呼吸灯效果实现
//		for(i=0;i<TIME;i++){
//			GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 亮
//			// 通过让i每次循环不断减去1
//			// 不断循环达到时钟时钟控制效果
//			Delay(i);
//			for(j=0;j<TIME;j++){
//				GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 亮
//				Delay(j);
//			}
//		}for(i=TIME;i>0;i--){
//			GPIO_SetBits(GPIOA, GPIO_Pin_0); // 亮
//			Delay(i);
//			for(j=TIME;j>0;j--){
//				GPIO_SetBits(GPIOA, GPIO_Pin_0); // 亮
//				Delay(j);
//			}
//		}
//	}
//}
