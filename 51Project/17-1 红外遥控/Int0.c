#include <REGX52.H>

void Int0_Init(void){
	// 低电平触发，下降沿触发的区别
	// IT0=0; IT0=1;
	IT0=1; 		//下降触发使能（即 按下触发）
	IE0=0;
	EX0=1;		//外部中断0使能
	EA=1;		//总中断使能
	// 触发下降沿外部中断
	PX0=1; //高优先级中断
}

/*
void Init0_Routine(void) interrupt 0{

}
*/