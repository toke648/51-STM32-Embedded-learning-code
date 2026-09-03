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

//如果有返回类型就不能设置成void空类型了
unsigned char mul(unsigned char nl){
	unsigned char count=1;
	int i;
	for(i=0;i<=nl;i++){
		count*=2;
	}
	return count;
}

void main() {
//	P2_0=1; //灭
//	P2_0=0;//亮
	
	unsigned char num=0;
	
	unsigned char led=0;
	while(1){
			if(P3_1==0){
				//清除抖动
				Dealy(20);
				while(P3_1==0);
				Dealy(20);
				
//				P2++;//P2从0自加到255
				//因为P2默认定义就是unsigned char类型，最大值为255，超过会自动重置
				
				
//				if(num<255){
					//通过对正向传递取反实现倒置
//					led++;
				
						
				if(num < 8) num++;else num=0;
				led=(0x01<<num);
				
				P2=~led;
			}
			if(P3_0==0){ //四个按钮的控制位
				//清除抖动
				Dealy(20);
				while(P3_0==0);
				Dealy(20);
				
//				P2++;//P2从0自加到255
				//因为P2默认定义就是unsigned char类型，最大值为255，超过会自动重置
				
				//通过对正向传递取反实现倒置
				
				if(num==0){
					num=7;
				}else{
					num--;
				}
				led=(0x01<<num);
				
				P2=~led;
			}
		}
}



//#include <REGX51.H>
//#include <INTRINS.H>

//void Dealy(unsigned int xms) {  //@12.000MHz
//    unsigned char i, j;
//    while(xms--){
//		i = 2;
//		j = 239;
//		do{
//			while(--j);
//		}while(--i); // --j和j--的区别是 前置- 和后置-
//	}
//}

////如果有返回类型就不能设置成void空类型了
//unsigned char mul(unsigned char nl){
//	unsigned char count=1;
//	int i;
//	for(i=0;i<=nl;i++){
//		count*=2;
//	}
//	return count;
//}

//void main() {
////	P2_0=1; //灭
////	P2_0=0;//亮
//	
//	unsigned char num=0;
//	
//	unsigned char led=0;
//	while(1){
//			if(P3_1==0){
//				//清除抖动
//				Dealy(20);
//				while(P3_1==0);
//				Dealy(20);
//				
////				P2++;//P2从0自加到255
//				//因为P2默认定义就是unsigned char类型，最大值为255，超过会自动重置
//				
//				
////				if(num<255){
//					//通过对正向传递取反实现倒置
////					led++;
//				
//						
//				if(num < 8) num++;else num=0;
//				led=mul(num);
//				
//				P2=~led;
//			}
//			if(P3_0==0){ //四个按钮的控制位
//				//清除抖动
//				Dealy(20);
//				while(P3_0==0);
//				Dealy(20);
//				
////				P2++;//P2从0自加到255
//				//因为P2默认定义就是unsigned char类型，最大值为255，超过会自动重置
//				
//				//通过对正向传递取反实现倒置
//				
//				if(num > 8) num--;
//				led=mul(num);
//				
//				P2=~led;
//			}
//		}
//}
