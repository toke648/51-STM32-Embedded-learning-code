#include <REGX52.H>

void Timer0Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0;		//设置定时初值
	TH0 = 0;		//设置定时初值
	
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
	
	//不需要中断
//	ET0 = 1;    //注意需要加上通道启动的控制
//    EA = 1;     // ← 加上这行
}

void Timer0_SetCounter(unsigned int Value){
	TH0=Value/256;  //取余
	TL0=Value%256;  //取余
}

unsigned int Timer0_GetCounter(void){
	return (TH0<<8) | TL0;
}

void Timer0_Run(unsigned char Flag){
	TR0=Flag;
}