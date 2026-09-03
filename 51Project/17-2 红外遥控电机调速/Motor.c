#include <REGX52.H>
#include "Timer1.h"

sbit LED = P2^0;
sbit Motor = P1^0;

unsigned char Counter = 0;   // 初始化为0
unsigned char Compare = 0;   // 初始化为0

void Motor_Init(void){
    Timer1Init();
	
    TR1 = 1;  // 添加这一行！
}

void Motor_SetSpeed(unsigned char Speed){
    Compare = Speed;  // 设置占空比 0-100
}

void Timer1_Routine() interrupt 3{
    TL1 = 0x18;
    TH1 = 0xFC;  // 1ms
    
    Counter++;
    if(Counter >= 100) Counter = 0;  // 0-99
    
    if(Counter < Compare){
        Motor = 1;
        LED = 0;
    } else {
        Motor = 0;
        LED = 1;
    }
}