#include <REGX52.H>
#include "LCD1602.h"
#include "DS18820.h"
#include "Delay.h"

unsigned char Ack;
float Temp;
int intPart;
unsigned long fracPart;

void main(){
	LCD_Init();
	LCD_ShowString(1,1,"Temperature:");
	
//	LCD_ShowNum(2,1,Ack,3);
	
	while(1){
		// 1.启动温度转换器
		DS18B20_ConvertT();
		
		// 2.等待转换完成（重要！）
		Delay(750);
		
		// 3.读取温度
		Temp=DS18B20_ReadT();
		
		// 4.显示正负号
		if(Temp<0){
			LCD_ShowChar(2,1,'-');
			Temp=-Temp;
		}
		else{
			LCD_ShowChar(2,1,'+');
		}
		
		// 5.分离整数和小数部分
		intPart=(int)Temp;
		fracPart = (unsigned long)((Temp-intPart)*10000);
		
		// 6.显示整数部分
		LCD_ShowNum(2,2,intPart,3);
		
		
		LCD_ShowChar(2,5,'.');
		
		// 8.显示小数部分
		LCD_ShowNum(2,6,fracPart,4);
		
		Delay(1000);
	}
}


