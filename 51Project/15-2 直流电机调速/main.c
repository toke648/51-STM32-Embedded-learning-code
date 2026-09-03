#include <REGX52.H>
#include "Timer0.h"
#include "Key.h"
#include "Nixie.h"

sbit LED=P2^0;
sbit Motor=P1^0;

//分别定义定时器和比较值
//定时器为0-100
//通过定义Compare的比较值，即可实现PWM频率控制
unsigned char Counter,Compare;
unsigned char KeyNum;
unsigned char Speed;


void main(){
	Timer0Init();
	Compare=50;
	
    while(1){
		KeyNum=Key();
		Key_Loop();
		
		if(KeyNum==1){
			Speed++;
			if(Speed==0){Compare=10;}
			if(Speed==1){Compare=25;}
			if(Speed==2){Compare=50;}
			if(Speed==3){Compare=100;}
			if(Speed==4){Compare=0;Speed=0;}
		}
		Nixie(1, Speed);
		
//		if(KeyNum==1){
//			Compare=5;
//		}
//		if(KeyNum==2){
//			Compare=10;
//		}
//		if(KeyNum==3){
//			Compare=50;
//		}
//		if(KeyNum==4){
//			Compare=100;
//		}
    }
}

//unsigned int T0Count;
void Timer0_Routine() interrupt 1{
	//定时器每隔100微妙执行一次中断
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	
	//定时器定时自增
//	Counter++;
//	if(Counter>=100){
//		Counter=0;
//	}
	
	//另一种实现方法
	Counter++;
	// if 99%100=99  if 100%100=0 
	Counter%=100;
	
	//Motor和LED的极性相反
	if(Counter<Compare){
		Motor=1;
		LED=0;
	}else{
		Motor=0;
		LED=1;
	}
	
}