#include <REGX52.H>
#include "LCD1602.h"
#include "Key.h"
#include "Delay.h"
#include "AT24C02.h"
#include "Key.h"

unsigned char Data,Write=123;
unsigned int KeyNum,Num=0;

void main(){
	LCD_Init();
	
//	LCD_ShowString(1,1,"HelloWorld!");
	
//	//在第一个位置写入内容66
//	
//	//多地址存储W
//	AT24C02_WriteByte(0,147);
//	Delay(5);
//	AT24C02_WriteByte(1,147);
//	Delay(5);
//	AT24C02_WriteByte(2,147);
//	
//	Delay(5); //根据数据手册，周期时间从Stop到Start需要有一定时间间隔
//	//读取第一个数据位
//	
//	Data = AT24C02_ReadByte(0x01);//获取第一位数据
//	
//	if(Data==Write){P2=0x00;}else{P2=0xFF;}
//	
//	LCD_ShowNum(2,1,Data,3);
	
	while(1){
		KeyNum=Key();
		//加
		if(KeyNum==1){
			Num++;
			LCD_ShowNum(1,1,Num,5);
			LCD_ShowString(2, 1, "     ");
		}
		//减
		if(KeyNum==2){
			Num--;
			LCD_ShowNum(1,1,Num,5);
			LCD_ShowString(2, 1, "     ");
		}
		//写
		/*
		因为16进制不能直接读取，因此拆成两部分
		高八位  jnm
		*/
		
		/*
		像这样 54321
		54321 = 1101 0100 0011 0001
		54321%256=212
		212=1101 0100
		54321/256=49
		49=0011 0001
		212<<8=1101 0100 0000 0000
		(212<<8)|49=1101 0100 0011 0001
		*/
		if(KeyNum==3){
			AT24C02_WriteByte(0,Num%256);
			Delay(10);
			AT24C02_WriteByte(1,Num/256);
			Delay(10);
			LCD_ShowString(2,1,"Saved");
		}
		//读
		if(KeyNum==4){
			Num=AT24C02_ReadByte(0);
			Num|=AT24C02_ReadByte(1)<<8;
			LCD_ShowNum(1,1,Num,5);
			LCD_ShowString(2,1,"Loaded");
		}
	}
}
