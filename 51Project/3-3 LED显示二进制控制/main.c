#include <REGX51.H>
#include <INTRINS.H>

void Dealy(unsigned int xms) {  //@12.000MHz
    unsigned char i, j;
    while(xms--){
		i = 2;
		j = 239;
		do{
			while(--j);
		}while(--i); // --j和j--的区别是 前置- 和后置-
	}
}

void main() {
//	P2_0=1; //灭
//	P2_0=0;//亮
	unsigned char led=0;
	while(1){
			if(P3_1==0){
				//清除抖动
				Dealy(20);
				while(P3_1==0);
				Dealy(20);
				
//				P2++;//P2从0自加到255
				//因为P2默认定义就是unsigned char类型，最大值为255，超过会自动重置
				
				//通过对正向传递取反实现倒置
				led++;
				P2=~led;
			}
		}
}
