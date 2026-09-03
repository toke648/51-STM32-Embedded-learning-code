#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"

void main(){
	LCD_Init();
	LCD_ShowChar(1,1,'\'');
	LCD_ShowChar(1,2,'\"');
	LCD_ShowChar(1,3,0x41);
	LCD_ShowChar(1,4,'A');
	LCD_ShowString(5,1,"HelloWrold!");
	LCD_ShowNum(1,13,255,3);
	LCD_ShowSignedNum(1,14,-88,3);
	LCD_ShowHexNum(2,7,0xA5,2);
	LCD_ShowBinNum(2,8,0xA5,8);
	LCD_ShowBinNum(2,9,26,8);
	
	
	//1101 0110 ヨ 1101 1001 ル 1011 1100 シ 1011 0110カ
	LCD_ShowChar(2,11,0xD6);
	LCD_ShowChar(2,12,0xD9);
	LCD_ShowChar(2,13,0xBC);
	LCD_ShowChar(2,14,0xB6);
	
	LCD_ShowChar(2,1,0xDF);
	LCD_ShowChar(2,2,'C');
	
	LCD_ShowString(1,1,"HelloWrold!");
	while(1){
		Delay(500);
		LCD_WriteCommand(0x18);
    }
}
