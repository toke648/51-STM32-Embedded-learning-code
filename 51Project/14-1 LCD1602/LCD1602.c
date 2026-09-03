#include <REGX52.H>

sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_E=P2^7;

#define LCD_DataPort P0

void LCD_Delay()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}


/**
  * @brief  
  * @param Line 行
  * @param Column 列
  * @param Char 字符
  * @retval 
  */
void LCD_WriteCommand(unsigned char Command){
	LCD_RS=0;
	LCD_RW=0;
	LCD_DataPort=Command;
	LCD_E=1;
	LCD_Delay();
	LCD_E=0;
	LCD_Delay();
}

//写数据
void LCD_WriteData(unsigned char Data){
	LCD_RS=1;
	LCD_RW=0;
	LCD_DataPort=Data;

	LCD_E=1; //运行写入
	LCD_Delay();
	LCD_E=0;
	LCD_Delay();
	
}

/**
  * @brief  LCD初始化
  * @retval void
  */
void LCD_Init(void){
	//初始化
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x0C);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x01);
}

void LCD_SetCursor(unsigned char Line,unsigned char Column){
	if(Line==1){									//第一行
		LCD_WriteCommand(0x80 | (Column-1));
	}
	else{											//第二行
		LCD_WriteCommand(0x80 | (Column-1)+0x40);
	}

}

/**
  * @brief LCD显示数据
  * @param Line 行
  * @param Column 列
  * @param Char 字符
  * @retval 
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,unsigned char Char){
	//因为LCD1602的显示地址是由一位数据位控制的，因此通过运算做拆分
//	if(Line==1){									//第一行
//		LCD_WriteCommand(0x80 | (Column-1));
//	}
//	else{											//第二行
//		LCD_WriteCommand(0x80 | (Column-1)+0x40);
//	}
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}


void LCD_ShowString(unsigned char Line,unsigned char Column,unsigned char *String){  //String[]/*String 传递多个字符地址
	unsigned char i;
	
	LCD_SetCursor(Line,Column);
	
	//相当于计数器，遍历数组
	//最常见的遍历数组方法
	for(i=0;String[i]!='\0';i++){
		LCD_WriteData(String[i]);
	}
}


int LCD_Pow(unsigned char X,unsigned char Y){
	unsigned char i;
	unsigned int Result = 1;
	for(i=0;i<Y;i++){
		Result*=X;
	}
	return Result;
}

/*
789

789/100=7.89
7.89%10=7
提出每一位

6789/1000%10=6
789/100%10=7	//以100为一个整数，多的算余数
789/10%10=8		//以10为一个整数，多的算余数
789/1%10=9 

*/
/**
  * @brief LCD显示数字（支持0-255）
  * @param Line 行(1-2)
  * @param Column 列(1-16)
  * @param Number 要显示的数字(0-255)
  * @param Length 显示长度(1-3)
  * @retval 无
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned char Number, unsigned char Length){
	unsigned char i;
	unsigned char temp;
	
	LCD_SetCursor(Line,Column);
	
//	for(i=0;i<Length;i++){
	
//	for(i=Length;i>0;i--){
//		LCD_WriteData((Number/(i*10))%10);
//	}
	
	for(i=Length;i>0;i--){
		temp = Number/LCD_Pow(10, i-1)%10;
		LCD_WriteData('0' + temp);
	}
}

void LCD_ShowSignedNum(unsigned char Line,unsigned char Column, int Number, unsigned char Length)
{
	unsigned char i;
	unsigned char temp;
	unsigned int Number1;
	
	LCD_SetCursor(Line,Column);
	
	if(Number>=0){
		LCD_WriteData('+');
		Number1=Number;
	}
	else{											//第二行
		LCD_WriteData('-');
		//直接取反会有一点问题
		//如果传递一个-32768赋值给一个int类型的值，会超出范围
		Number1=-Number;
	}
	for(i=Length;i>0;i--){
		temp = Number1/LCD_Pow(10, i-1)%10;
		LCD_WriteData('0' + temp);
	}
}


void LCD_ShowHexNum(unsigned char Line,unsigned char Column, int Number, unsigned char Length){
	unsigned char i;
	unsigned char SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--){
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10){
			LCD_WriteData('0'+ SingleNumber);
		}
		else{
			LCD_WriteData('A'+ SingleNumber - 10);
		}
	}

}

/*
数字 13 的二进制：1 1 0 1
                  ↑ ↑ ↑ ↑
                  │ │ │ └── i=1: 2^0=1   → (13/1)%2 = 1
                  │ │ └──── i=2: 2^1=2   → (13/2)%2 = 0
                  │ └────── i=3: 2^2=4   → (13/4)%2 = 1
                  └──────── i=4: 2^3=8   → (13/8)%2 = 1
*/
/**
  * @brief  显示二进制数值
  * @param 
  * @param 
  * @param 
  * @param
  * @retval 
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column, int Number, unsigned char Length){
	unsigned char i;
	unsigned char BinNum;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--){
		BinNum='0'+Number/LCD_Pow(2,i-1)%2;
		LCD_WriteData(BinNum);
	}
}