#include <REGX52.H>
#include "Delay.h"
#include "INTRINS.H"

//定义蜂鸣器端口
sbit Buzzer=P2^5;

unsigned char BuzzerActive = 0;	//蜂鸣器是否激活
unsigned int BuzzerRemain  = 0;	//剩余时间
unsigned int BuzzerState = 0;  //目标时间

/**
  * @brief 蜂鸣器私有延时函数，延时500us
  * @param 无
  * @retval 无
  */
void Buzzer_Delay500us(){
	unsigned char i;

	_nop_();
	i = 247;
	while (--i);
}

void Buzzer_Init(void){
	Buzzer = 0;
	BuzzerActive = 0;
	BuzzerRemain = 0;
}

void Buzzer_Start(unsigned int ms){
	BuzzerActive = 1;
	BuzzerRemain = ms;
	Buzzer = 1;
}

void Buzzer_Stop(void){
	BuzzerActive = 0;
    BuzzerRemain = 0;
    Buzzer = 0;
}

void Buzzer_Loop(void){
	static unsigned char ms_counter = 0;
	
	if(BuzzerActive){
		ms_counter++;
		if(ms_counter >= 1){ //每1ms
			ms_counter = 0;
			BuzzerRemain--;
			if(BuzzerRemain == 0){
				BuzzerActive = 0;
				Buzzer = 0;   // 时间到，关闭蜂鸣器
			}
		}
	}
}


///**
//  * @brief  蜂鸣器发声
//  * @param ms 发声时间
//  * @retval 无返回值
//  */
//void Buzzer_Time(unsigned int ms){
//	unsigned char i;
//	//因为改成毫秒了，延时要×2
//	EA=0;
//	for(i=0;i<ms*2;i++){
//		Buzzer=~Buzzer;
//		Buzzer_Delay500us();
//	}
//	EA=1;
//}