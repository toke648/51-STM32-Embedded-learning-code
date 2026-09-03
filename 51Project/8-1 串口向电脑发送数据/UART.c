#include <REGX52.H>

/**
  * @brief  串口初始化 //4800bps@12.000MHz
  * @param  无
  * @param  无
  * @retval 
  */

void UART_Init(){
	// 40是使能，50是不使能
	SCON = 0x40;		//8位数据,可变波特率
	PCON |= 0x80;		//使能波特率倍速位SMOD
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0); //判断是否为零（即是否有数据）
	TI=0;
}