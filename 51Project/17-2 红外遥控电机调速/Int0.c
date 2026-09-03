#include <REGX52.H>

void Int0_Init(void){
	// 低电平触发，下降沿触发的区别
	// IT0=0; IT0=1;
	IT0=1;
	IE0=0;
	EX0=1;
	EA=1;
	// 触发下降沿外部中断
	PX0=1; //高优先级中断
}

/*
void Init0_Routine(void) interrupt 0{

}
*/