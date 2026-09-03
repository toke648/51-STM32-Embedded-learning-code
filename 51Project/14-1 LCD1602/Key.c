#include <REGX52.H>
#include "Delay.h"

unsigned char Key_Key_Number;


unsigned char Key(void){
	unsigned char temp = Key_Key_Number;
	Key_Key_Number = 0;
	return temp;
}

/**
  * @brief  获取独立按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0~4，无按键按下时返回值为0
  */
/*
        硬件引脚
        P3_0, P3_1...
           ↓
    Key_GetState()    ← 读取硬件状态
           ↓
    NowState = 结果
           ↓
    Key_Loop()        ← 比较上次和这次
           ↓
    检测到松开事件
           ↓
    存入全局变量
*/
unsigned char Key_GetState()
{
	unsigned char KeyNumber=0;
	
	if(P3_1==0){Delay(20);KeyNumber=1;}
	if(P3_0==0){Delay(20);KeyNumber=2;}
	if(P3_2==0){Delay(20);KeyNumber=3;}
	if(P3_3==0){Delay(20);KeyNumber=4;}
	
	return KeyNumber;
}


/*
状态机工作原理图解
text
按键状态变化：
按下瞬间          松开瞬间
   ↓                ↓
[未按下] -> [按下] -> [未按下]
   0         1         0
   
检测逻辑：
LastState = 0, NowState = 0 → 无动作
LastState = 0, NowState = 1 → 按下（不处理）
LastState = 1, NowState = 0 → 松开（记录按键）✓

上一次状态 = 当前状态;      // 保存上一次的情况
当前状态 = Key_GetState();  // 看看现在谁被按着

如果(上一次有人按 && 现在没人按){
    // 有人松开了！记下来
    全局按键值 = 上一次是谁按的;
}
*/


unsigned char Key_Loop(void){
	static unsigned char NowState,LastState;
	LastState=NowState; //因为NowState是静态类型，因此会记录上一个状态的数据
	NowState=Key_GetState(); //判断是否有按键按下，如果有则返回被按下的按钮号
	/*
	松手检测，如果上个状态为1，并且现在的状态为0，
	if(LastState==1 && NowState==0)
	
	如果改为，上个状态为0，且如果现在的状态为1，则可以改为按下检测
	if(LastState==0 && NowState==1){
	*/
	if(LastState==1 && NowState==0){ //比如，如果上一个人进房间是开了灯的，并且你又点了一次灯，就表示灯开了，这部分主要用于判断之前的按钮是否是被按下
		Key_Key_Number=1;
	}
	if(LastState==2 && NowState==0){
		Key_Key_Number=2;
	}
	if(LastState==3 && NowState==0){
		Key_Key_Number=3;
	}
	if(LastState==4 && NowState==0){
		Key_Key_Number=4;
	}
	
	return Key_Key_Number;
}