#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "Timer0.h"
#include "UART.h"

unsigned char Sec;

void main(){
	UART_Init();
	
    while(1){
		Delay(1000); //改为延时1秒
		UART_SendByte(Sec);
		Sec++;
    }
}
