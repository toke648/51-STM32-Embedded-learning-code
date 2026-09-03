#include <REGX51.H>
#include <INTRINS.H>

void Delaylms(unsigned int xms) {  //@12.000MHz
    unsigned char i, j;
    while(xms--){
		i = 2;
		j = 239;
		do{
			
			while(--j);
		}while(--i); // --j和j--的区别是
	}
}

void main() {
	
	while(1) {
		P2=0xFE; //1111 1110
		Delaylms(20);
		P2=0xFD; //1111 1101
		Delaylms(20);
		
		P2=0xFB; //1111 1011
		Delaylms(20);
		P2=0xF7; //1111 0111
		Delaylms(20);
		
		
		P2=0xEF; //1110 1111
		Delaylms(20);
		P2=0xDF; //1101 1111
		Delaylms(20);
		
		P2=0xBF; //1011 1111
		Delaylms(20);
		P2=0x7F; //0111 1111
		Delaylms(20);
		
	}
}


