#include <REGX52.H>
#include "Delay.h"
#include "UART.h"

unsigned char Sec;

//当传入 "B" 时，编译器会警告，但实际传入的是指针值（2-4字节），函数只取低8位发送，导致乱码。
//记住：
//单引号 'B' = 字符
//双引号 "B" = 字符串（指针/地址）
void main(){
	UART_Init();
	UART_SendByte('B');
//	UART_SendByte(0x30); //单片机向电脑发送数据
    while(1){
//		Delay(1000); //改为延时1秒
//		UART_SendByte(Sec);
//		Sec++;
    }
}

// 将函数应用为中断子函数，需要设置interrupt x
//当中断发生时，interrupt 4将会跳转到函数执行
void UART_Routine() interrupt 4
{	
	// 判断，当有数据进入
	if(RI==1){
		//因为发送会触发中断，接受也会触发中断
		//只需要对这两个做出判断即可
		P2=SBUF;
		//需要注意，一个函数不能在主函数里出现
		//又在中断函数中出现
//		UART_SendByte(SBUF);
		RI=0;
	}
}