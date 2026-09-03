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
//	P2_0=1; //灭
//	P2_0=0;//亮
	
	while(1){
		if(P3_1==0){ // 如果被按下
			P2_0=0;
		}else{
			P2_0=1;
		}
	}
}
