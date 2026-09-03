#include <REGX52.H>

sbit LED=P2^0;

void Delayms(unsigned int xms) {
    while(xms--);
}

void main(){
    unsigned char Time,i;
    while(1){
//		LED=0;
//		Delayms(5);
//		LED=1;
//		Delayms(95);
		
//		LED=0;
//		Delayms(95);
//		LED=1;
//		Delayms(5);
		for(Time=0;Time<100;Time++){
			//内部的i延时，是为了增加和延长Time的执行次数，无法直接使用Delay代替
			for(i=0;i<20;i++){
				LED=0;
				Delayms(Time);
				LED=1;
				//占空比逐渐由高到低
				Delayms(100-Time);
			}
		}
		for(Time=100;Time>0;Time--){
			//内部的i延时，是为了增加和延长Time的执行次数，无法直接使用Delay代替
			for(i=0;i<20;i++){
				LED=0;
				Delayms(Time);
				LED=1;
				//占空比逐渐由低到高
				Delayms(100-Time);
			}
		}
    }
}

