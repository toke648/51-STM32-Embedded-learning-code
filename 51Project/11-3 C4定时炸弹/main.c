#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"

sbit Buzzer=P2^5;

unsigned char CountDown=60;
unsigned char DisplayBuff[8]={0,0,0,0,0,0,0,0};
unsigned char DisplayIndex=0;
unsigned char i,j,Speed=1000;
unsigned char MSCounter=60;
unsigned char BuzzerCount=0;
unsigned char Count=1000;

// 微秒级延时（12MHz晶振，约延时 us 微秒）
void Delay_us(unsigned int us) {
    while(us--) {
        // 空循环约 1μs（12MHz）
        unsigned char i = 1;
        while(i--);
    }
}

void main(){
	Timer0Init();
	while(1){
		
	}
}

void Timer0_Routine() interrupt 1 {
	//这里注意要用unsigned int 范围 0~65535
	//char 最大只能到 127
	static unsigned int SecondCounter=0;
	static unsigned int BeepFreq = 0;        // 蜂鸣器翻转计数器
	
    TL0 = 0x18;  // 重装初值（12MHz晶振定时1ms）
    TH0 = 0xFC;
	
	//数码管扫描（每1ms刷新一位）
	Nixie(DisplayIndex + 1, DisplayBuff[DisplayIndex]);
	DisplayIndex++;
	if(DisplayIndex >= 8) {            // 8位扫描完
        DisplayIndex = 0;              // 回到第0位
    }
	
	//当走完到%60
	if(CountDown%BuzzerCount==6){
		SecondCounter=500;
	}
	//当走完到%30
	if(CountDown%BuzzerCount==3){
		SecondCounter=250;
	}
	//当走完到%20
	if(CountDown%BuzzerCount==2){
		SecondCounter=100;
	}
	//当走完到%10
	if(CountDown%BuzzerCount==1){
		SecondCounter=10;
	}
	
	//倒计时（每秒减1）
	/*不用Delay实现，通过无延迟快速更新整个显像管*/
	SecondCounter++; //每1ms加一次
	
	if(SecondCounter>=1000){			//每1ms加1
		SecondCounter = 0;              // 清零，重新计数

		if(CountDown>0){
			CountDown--;
			
			DisplayBuff[4]=CountDown/10;
			DisplayBuff[5]=CountDown%10;
		}
		
//		if(MSCounter>0){
//			MSCounter--;
//			DisplayBuff[6]=MsCounter/100;
//			DisplayBuff[7]=(MsCounter%100)/10;
//		}

		for(i=0;i<160;i++) {
			Buzzer = !Buzzer;
			Delay_us(5);
		}
		Buzzer = 1; 
		BuzzerCount++;
	}
}