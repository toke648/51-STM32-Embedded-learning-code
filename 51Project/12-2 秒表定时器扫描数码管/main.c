#include <REGX52.H>
#include "Key.h"
#include "Nixie.h"
#include "Delay.h"
#include "Timer0.h"
#include "AT24C02.h"

unsigned char KeyNum;
unsigned char Temp;
unsigned char Sec=0,Min=0,Hour=0,MinSec;
unsigned char RunFlag;

void Time_Update(){

}

void main(){
	Timer0Init();
	
	while(1){
		KeyNum=Key();
		//主函数里的代码会优先执行，除非定时器中断
		if(KeyNum){
			//只加速控制秒的更改，这样可以操控更多的位
			if(KeyNum==1){
				//启动秒表计时
				RunFlag=!RunFlag;
			}
			if(KeyNum==2){
				//计时清零
				Min=0;
				Sec=0;
				Hour=0;
			}
			if(KeyNum==3){
				AT24C02_WriteByte(0,Sec);
				Delay(5);
				AT24C02_WriteByte(1,Min);
				Delay(5);
				AT24C02_WriteByte(2,Hour);
				Delay(5);
			}
			if(KeyNum==4){
				Sec=AT24C02_ReadByte(0);
				Min=AT24C02_ReadByte(1);
				Hour=AT24C02_ReadByte(2);
			}
		}
		
		//状态更新
		Nixie_Scan(7, Sec/10); //个位
		Nixie_Scan(8, Sec%10); //十位
		Nixie_Scan(4, Min/10); //个位
		Nixie_Scan(5, Min%10); //十位
		Nixie_Scan(1, Hour/10); //个位
		Nixie_Scan(2, Hour%10); //十位	
	}
}


void Sec_Loop(){
	if(RunFlag){
		Sec++;
		if(Sec>60){
			Sec=0;
			Min++;
			if(Min>60){
				Min=0;
				Hour++;
				if(Hour>60){
					Hour=0;
				}
			}
		}
	}
}

//unsigned int T0Count;
void Timer0_Routine() interrupt 1{
	static unsigned int T0Count0,T0Count1,T0Count2;
	TL0=0x18;
	TH0=0xFC;
	
	//横杠的初始化
	Nixie_Scan(3,11);
	Nixie_Scan(6,11);
	
	T0Count0++;
	if(T0Count0>=60){ // 每隔1秒检测一次 //改成秒表功能测试，因此速度调成0.06秒了
		T0Count0=0; // 定时器重置
		Sec_Loop();
	}
	
	T0Count1++;
	if(T0Count1>=20){ // 每隔20毫秒检测一次
		T0Count1=0; // 定时器重置
//		P2=~P2; //测试
		Key_Loop(); //代替延时效果
	}
	
	T0Count2++;
	if(T0Count2>=2){ // 每隔2毫秒检测一次
		T0Count2=0; // 定时器重置
		Nixie_Loop(); //代替延时效果
	}
}