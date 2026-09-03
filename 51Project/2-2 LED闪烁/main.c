#include <REGX51.H>
#include <INTRINS.H> //
void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 205;
	k = 187;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main(){
//	P2=10101010;
//	P2=0x55; // 0101 0101
	
//	P2=0xFE; // 1111 1110
//	P2=0xFD; // 1111 1101
//	P2=0xFC; // 1111 1100
//	P2=0xFB; // 1111 1101
//	P2=0xFA; // 1111 1100
//	P2=0xF9; // 1111 1100
//	P2=0xF8; // 1111 1100
	
	while(1){
//		P2=0xFF; // 0101 0101
//		Delay500ms();
//		P2=0xFE; // 1010 1010
//		Delay500ms();
		
		P2=0xFF; // 0101 0101
		Delay500ms();
		P2=0xFE; // 1111 1110
		Delay500ms();
		P2=0xFD; // 1111 1101
		Delay500ms();
		P2=0xFC; // 1111 1100
		Delay500ms();
		P2=0xFB; // 1111 1101
		Delay500ms();
		P2=0xFA; // 1111 1100
		Delay500ms();
		
		P2=0xF9; // 1111 1100
		Delay500ms();
		P2=0xF8; // 1111 1100
		Delay500ms();
		P2=0xF7; // 1111 1110
		Delay500ms();
		P2=0xF6; // 1111 1101
		Delay500ms();
		P2=0xF5; // 1111 1100
		Delay500ms();
		P2=0xF4; // 1111 1101
		Delay500ms();
		P2=0xF3; // 1111 1100
		Delay500ms();
		P2=0xF2; // 1111 1100
		Delay500ms();
		P2=0xF1; // 1111 1100
		Delay500ms();
		
		P2=0xFF; // 0101 0101
		Delay500ms();
		P2=0xEE; // 1111 1110
		Delay500ms();
		P2=0xDD; // 1111 1101
		Delay500ms();
		P2=0xCC; // 1111 1100
		Delay500ms();
		P2=0xBB; // 1111 1101
		Delay500ms();
		P2=0xAA; // 1111 1100
		Delay500ms();
		
		P2=0x99; // 1111 1100
		Delay500ms();
		P2=0x88; // 1111 1100
		Delay500ms();
		P2=0x77; // 1111 1110
		Delay500ms();
		P2=0x66; // 1111 1101
		Delay500ms();
		P2=0x55; // 1111 1100
		Delay500ms();
		P2=0x44; // 1111 1101
		Delay500ms();
		P2=0x33; // 1111 1100
		Delay500ms();
		P2=0x22; // 1111 1100
		Delay500ms();
		P2=0x11; // 1111 1100
		Delay500ms();
	}
}


//void main(){
////	P2=10101010;
////	P2=0x55; // 0101 0101
////	P2=0xAA; // 1010 1010
//	
//	while(1){
//		P2=0xFF; // 0101 0101
//		Delay500ms();
//		P2=0xFE; // 1010 1010
//		Delay500ms();
//	}
//}