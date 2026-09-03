#include <REGX51.H>
#include <INTRINS.H>

void Delay500ms() {
    unsigned char i, j, k;
    _nop_();
    i = 4;
    j = 205;
    k = 187;
    do {
        do {
            while (--k);
        } while (--j);
    } while (--i);
}

void main() {
    unsigned char a = 2;
	unsigned char count = 0;
	unsigned char result;
//    P2 = 3;
	
    while(1) {
		if(count < 10){
			result=a*count;
			
			P2 = result;
			
			Delay500ms();
			
			count++;
		}else{
			count=0; //重置计数器
		}
    }
}

//while(1) {
//	P2=0xFE; //1111 1110
//	Delay500ms();
//	P2=0xFD; //1111 1101
//	Delay500ms();
//	
//	P2=0xFB; //1111 1011
//	Delay500ms();
//	P2=0xF7; //1111 0111
//	Delay500ms();
//	
//	
//	P2=0xEF; //1110 1111
//	Delay500ms();
//	P2=0xDF; //1101 1111
//	Delay500ms();
//	
//	P2=0xBF; //1011 1111
//	Delay500ms();
//	P2=0x7F; //0111 1111
//	Delay500ms();
//	
//}

/// 2**n实现流水灯思路拆解

//void main() {
//    unsigned char num;
//	a = 2;
//	unsigned char count;
////    P2 = 3;
//	
//    while(1) {
//        P2 = 0;
//        Delay500ms();
//		P2 = 1;
//		Delay500ms();
//		P2 = 2;
//        Delay500ms();
//		P2 = 4;
//        Delay500ms();
//		P2 = 8;
//        Delay500ms();
//    }
//}


/// 算数运算效果实现


//void main() {
//    unsigned char counter = 0;
//    
//    while(1) {
//        // 直接输出数值到P2
//        P2 = counter;
//        
//        // 延时500ms
//        Delay500ms();
//        
//        // 数值递增（自动溢出：255→0）
//        counter++;
//        
//        // 可选：显示当前数值（通过其他方式）
//        // 数值范围：0→255，对应十六进制：0x00→0xFF
//    }
//}