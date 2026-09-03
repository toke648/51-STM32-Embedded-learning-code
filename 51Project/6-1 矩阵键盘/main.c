#include <REGX52.H>
#include <INTRINS.H>
#include "Delay.h"
#include "Nixie.h"
#include "LCD1602.h"
#include "MatrixKey.h"

//	LCD_ShowChar(1,1,"A");
//	LCD_ShowString(1,1,"HelloWorld!");
//	LCD_ShowNum(1,9,123,3);
//	LCD_ShowSignedNum(1,13,-66,2);
//	LCD_ShowHexNum(2,1,0xA8,2);
//	LCD_ShowBinNum(2,4,0xAA,8);

void main() {
	int num=0;
	unsigned char KeyNum;
	
	// 先初始化
	LCD_Init();
//	LCD_ShowString(1,1,"HelloWorld!");

    while(1) {
		KeyNum=MatrixKey();
		if(KeyNum){
			LCD_ShowNum(1,1,KeyNum,2);
		}

    }
}

