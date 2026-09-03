#include <REGX52.H>
#include <INTRINS.H>
#include "Timer0.h"
#include "MatrixKey.h"

/*
外部引脚控制时钟相当于计数器

0000 是前四位时钟控制，即为T1 
0001 为启用后四位时钟控制，即为T2

因为定时器是不可赋值的（查看官方文档）
TMOD=0x01; // 0000 0001 是赋值操作
因此需要改用 &| 与或式赋值法

TMOD=TMOD&0xF0; //把TMOD的低四位清零，高四位保持不变
1010 0011
1010 0011 & 1111 0000 = 1010 0000 （都1才1）

1010 0000
1010 0000 | 0000 0001 = 1010 0001 （有1就1）



//	TL0=0x18;TH0=0xFC;将65536转化成16进制的方法也可以，但是这种方法初始值要设置成65536



0~65535
每隔1us计数+1
总共定时时间65535us
11.0952(晶振频率)的溢出值应该是1085，从64450开始计时
初值 = 65536 - （定格时间 * 晶振频率 / 12）

同时计时器在超过最大值1111 1111
会溢出重置，并且变成0000 0000

a=_crol_(a,1); //0x01 << >>

*/

unsigned char KeyNum,LEDMode;	

void main(){
	P2=0xFE;
	Timer0Init();
	
    while(1){
		KeyNum=MatrixKey();
		if(KeyNum==1){
			LEDMode++;
			if(LEDMode>=2)LEDMode=0;
		}
    }
}

//unsigned int T0Count;
void Timer0_Routine() interrupt 1{
	static unsigned int T0Count;
	TL0=0x18;
	TH0=0xFC;
	T0Count++;
//	if(T0Count>=500){
//		T0Count=0;
//		P2_0=~P2_0; // 取反
//	}
	if(T0Count>=500){
		T0Count=0; // 定时器重置
		if(LEDMode==0) // _cror_ 移位操作
			P2=_cror_(P2, 1); // 左移一位
		if(LEDMode==1)
			P2=_cror_(P2, 1); // 右移一位
	}

}

