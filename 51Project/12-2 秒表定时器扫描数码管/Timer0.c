#include <REGX52.H>

void Timer0Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0 = 1;    //注意需要加上通道启动的控制
    EA = 1;     // ← 加上这行
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
