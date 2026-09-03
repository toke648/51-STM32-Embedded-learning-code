#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Buzzer.h"

sbit Buzzer=P2^5;

// 演奏速度定义
#define SPEED		125

unsigned int FreqTable[]={
	63628,63731,63835,63928,64021,
	64103,64185,64260,64331,64400,
	64463,64524,64580,64633,64684,
	64732,64777,64820,64860,64898,
	64934,64968,65000,65030,65058,
	65085,65110,65134,65157,65178,
	65198,65217,65235,65252,65268,
	65283,
};

// 乐谱：{音调索引, 时值} 交替存储
unsigned char Music[] = {
    12,4,
    12,4,
    19,4,
    19,4,
	0,4,
    21,4,
    21,4,
    19,8,
    17,4,
    17,4,
	0,4,
    16,4,
    16,4,
    14,4,
    14,4,
    12,8,
	
	0XFF
};


unsigned char FreqSelect,MusicSelect;

void main(){
	Timer0Init(); //忘了初始化了
    while(1)
	{
		//如果不是终止，才执行
		if(Music[MusicSelect]!=0xFF){
			
			FreqSelect=Music[MusicSelect];
			MusicSelect++;
	//		FreqSelect++;
			
			//根据音符，将4定义为500毫秒
			Delay(SPEED*Music[MusicSelect]); //每隔500毫秒切换
			
			//注意要在结束后再++
			MusicSelect++;
			
			
			// 因为音和音没有间隔，因此停顿5毫秒
			TR0 = 0;
			Delay(5);
			TR0 = 1;
		}else{
			TR0=0;
			while(1);
		}
		
					// 循环播放
//			if(MusicSelect >= sizeof(Music)) {
//				MusicSelect = 0;
//			}
    }
}

//一毫秒计时一次，因此产生了500毫秒hz的频率
void Timer0_Routine() interrupt 1
{
	//通过更改TL0 TH0初值，即可调整不同的蜂鸣器频率
	//通过赫兹的转位成十进制即可发出不同的音
	
	//因为TL0 TH0组合成了一个16位的
	//因此需要拆开分别赋值
	
	// 休止符定义，如果没有值就无定义
	if(FreqTable[FreqSelect]){
		TL0=FreqTable[FreqSelect]%256;	//设置定时初值
		
		//1111 1100,十进制374
		TH0=FreqTable[FreqSelect]/256;	//设置定时初值
		
		Buzzer=!Buzzer; //每次溢出产生中断，重置
	}
}




//#include <REGX52.H>
//#include "Delay.h"
//#include "Timer0.h"

//sbit Buzzer = P2^5;

//// 频率表
//unsigned int code FreqTable[] = {
//    63628, 63731, 63835, 63928, 64021,
//    64103, 64185, 64260, 64331, 64400,
//    64463, 64524, 64580, 64633, 64684,
//    64732, 64777, 64820, 64860, 64898,
//    64934, 64968, 65000, 65030, 65058,
//    65085, 65110, 65134, 65157, 65178,
//    65198, 65217, 65235, 65252, 65268,
//    65283
//};

//// 乐谱（《小星星》片段）
//unsigned char code Music[] = {
//    12, 4,   // 1
//    12, 4,   // 1
//    19, 4,   // 5
//    19, 4,   // 5
//    21, 4,   // 6
//    21, 4,   // 6
//    19, 8,   // 5---
//    17, 4,   // 4
//    17, 4,   // 4
//    16, 4,   // 3
//    16, 4,   // 3
//    14, 4,   // 2
//    14, 4,   // 2
//    12, 8    // 1---
//};

//unsigned char FreqSelect, MusicSelect;

//void main() {
//    Timer0Init();
//    
//    while(1) {
//        // 播放一个音符
//        FreqSelect = Music[MusicSelect++];
//        TR0 = 1;  // 开启定时器，开始发声
//        Delay(125 * Music[MusicSelect++]);  // 延时音符时值
//        TR0 = 0;  // 关闭定时器，停止发声
//        
//        // 音符间停顿
//        Delay(5);
//        
//        // 循环播放
//        if(MusicSelect >= sizeof(Music)) {
//            MusicSelect = 0;
//        }
//    }
//}

//void Timer0_Routine() interrupt 1 {
//    TL0 = FreqTable[FreqSelect] % 256;
//    TH0 = FreqTable[FreqSelect] / 256;
//    Buzzer = !Buzzer;
//}