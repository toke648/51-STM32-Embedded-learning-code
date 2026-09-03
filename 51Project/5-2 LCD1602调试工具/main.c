#include <REGX52.H>
#include <INTRINS.H>
#include "Delay.h"
#include "Nixie.h"
#include "LCD1602.h"

void main() {
	int num=0;
	// 先初始化
	LCD_Init();
	
//	LCD_ShowChar(1,1,"A");
//	LCD_ShowString(1,1,"HelloWorld!");
//	LCD_ShowNum(1,9,123,3);
//	LCD_ShowSignedNum(1,13,-66,2);
//	LCD_ShowHexNum(2,1,0xA8,2);
//	LCD_ShowBinNum(2,4,0xAA,8);

    while(1) {
		num++;
		
		LCD_ShowNum(1,9,num,3);
		Delay(100);

    }
}

