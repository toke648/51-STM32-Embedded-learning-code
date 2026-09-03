#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "Int0.h"
#include "Timer0.h"
#include "IR.h"

//unsigned char Num=255;
//char最大只能255，注意要改成int
//unsigned int Time;


/*
优先级顺序（默认）：

外部中断0 (INT0) - 最高

定时器0 (T0)

外部中断1 (INT1)

定时器1 (T1)

串口 (UART)
*/

unsigned char Num;
unsigned char Address;
unsigned char Command;

void main(){
	LCD_Init();
	LCD_ShowString(1,1,"ADDR  CMD  NUM");
	LCD_ShowString(2,1,"00    00   000");
	
	IR_Init();
	
    while(1){
		//if(IR_GetDataFlag())
		//实现重发获取，获取到数据 或 收到重复操作
		if(IR_GetDataFlag() || IR_GetRepeatFlag()){
			Address=IR_GetAddress();
			Command=IR_GetCommand();  //获取指令
			
			LCD_ShowHexNum(2,1,Address,2);
			LCD_ShowHexNum(2,7,Command,2);
			
			if(Command==IR_VOL_MINUS){		//即为VOL-键
				Num--;
			}
			if(Command==IR_VOL_ADD){		//即为VOL+键
				Num++;
			}
			
			LCD_ShowNum(2,12,Num,3);
		}
		
//		if(IR_GetDataFlag() || IR_GetRepeatFlag())  //如果检测到数据帧，或者收到连发帧
//		{
//			Address=IR_GetAddress();
//			Command=IR_GetCommand();
//			
//			LCD_ShowHexNum(2,1,Address,2);
//			LCD_ShowHexNum(2,7,Command,2);
//		}
    }
}

//void Init0_Routine(void) interrupt 0{
//	Num--;

//}

//Timer0Init();
//Timer0_SetCounter(0); //计时器清零
//Timer0_Run(1);
//Delay(10);

//Time=Timer0_GetCounter();
//LCD_ShowNum(2,1,Time,5);