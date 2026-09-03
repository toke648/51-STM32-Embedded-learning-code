#include <REGX52.H>
//#include "Delay.h"
//#include "LCD1602.h"
#include "Motor.h"
#include "IR.h"
#include "Key.h"
#include "Nixie.h"

unsigned char Num;
unsigned char Address;
unsigned char Command;

unsigned char KeyNum;
unsigned char Speed=0;

void main(){
	Motor_Init();
	IR_Init();
	
	Motor_SetSpeed(0);
	
    while(1){
		if(IR_GetDataFlag()){
			Address=IR_GetAddress();
			Command=IR_GetCommand();  //获取指令
			
//			LCD_ShowHexNum(2,1,Address,2);
//			LCD_ShowHexNum(2,7,Command,2);
			
			if(Command==IR_0){Motor_SetSpeed(0);Nixie(1,0);}
			if(Command==IR_1){Motor_SetSpeed(25);Nixie(1,1);}
			if(Command==IR_2){Motor_SetSpeed(50);Nixie(1,2);}
			if(Command==IR_3){Motor_SetSpeed(75);Nixie(1,3);}
			if(Command==IR_4){Motor_SetSpeed(100);Nixie(1,4);}
		}
			
    }
}
