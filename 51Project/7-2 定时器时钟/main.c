#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "Timer0.h"

 //计时器实现（时钟）
 
//unsigned char Sec;
//unsigned char Sec,Min,Hour;

// 倒计时
unsigned char Sec = 15,Min = 2,Hour = 0;

void main(){
	LCD_Init();
	Timer0Init();
	
	LCD_ShowString(1,1,"Clock!");

    while(1){
		LCD_ShowNum(2,1,Hour,2);
		LCD_ShowString(2,3,":");
		LCD_ShowNum(2,4,Min,2);
		LCD_ShowString(2,6,":");
		LCD_ShowNum(2,7,Sec,2);
    }
}


//unsigned int T0Count;
void Timer0_Routine() interrupt 1{
	static unsigned int T0Count;
	TL0=0x18;
	TH0=0xFC;
	T0Count++;
	
	// if(T0Count>=20){ 一秒钟实现
	if(T0Count>=1000){
		T0Count=0; //记得清空计时器！！！

//		Sec++;
//		if(Sec>=60){
//			Sec=0;
//			Min++;
//			if(Min>=60)
//			{	
//				Min=0;
//				Hour++;
//				if(Hour>=60);
//					Hour=0;
//			}
//		}
		
		// 倒计时
		Sec--;
		if(Sec<=0){
			Sec=59;
			Min--;
			if(Min<=0)
			{	
				Min=59;
				Hour--;
				if(Hour<=0);
					Hour=59;
			}
			if(Min<=2){
				T0Count=20;
			}
		}
	}

}