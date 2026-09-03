#include <REGX52.H>

//多个中断控制，使用两个Time，另一个用Timer0，这个用Timer1
void Timer1Init(void)		//1毫秒@12.000MHz
{
//	TMOD &= 0xF0;		//设置定时器模式
//	TMOD |= 0x01;		//设置定时器模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x9C;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	
	TF1 = 0;		//清除TF0标志
	TR1 = 1;		//定时器0开始计时
	
	//不需要中断
	ET1 = 1;    //注意需要加上通道启动的控制
    EA = 1;     // ← 加上这行
	
	PT1=0;
}


////unsigned int T0Count;
//void Timer0_Routine() interrupt 1{
//	static unsigned int T0Count;
//	TL0=0x18;
//	TH0=0xFC;
//	T0Count++;
//	if(T0Count>=1000){
//		T0Count=0; // 定时器重置
//	}

//}
