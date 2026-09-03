#include <REGX52.H>
#include "Delay.h"
#include "INTRINS.H"

//定义蜂鸣器端口
sbit Buzzer=P2^5;

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

/**
  * @brief  蜂鸣器发声
  * @param ms 发声时间
  * @retval 无返回值
  */
void Buzzer_Time(unsigned int ms){
	unsigned char i;
	//因为改成毫秒了，延时要×2
	for(i=0;i<ms*2;i++){
		Buzzer=~Buzzer;
		Buzzer_Delay500us();
	}
}