#include <REGX52.H>
#include <INTRINS.H>

void Delay(unsigned int xms) {  //@12.000MHz
    unsigned char i, j;
    while(xms--){
		i = 2;
		j = 239;
		do{
			while(--j);
		}while(--i); // --j和j--的区别是
	}
}