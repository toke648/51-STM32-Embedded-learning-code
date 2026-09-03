#include <REGX51.H>
#include <INTRINS.H>

unsigned char NixieTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	
void Nixie(unsigned char Location, unsigned char Number) {
    switch(Location) {
        // 8 control bits
        case 1: P2_4=1; P2_3=1; P2_2=1; break;
        case 2: P2_4=1; P2_3=1; P2_2=0; break;
        case 3: P2_4=1; P2_3=0; P2_2=1; break;
        case 4: P2_4=1; P2_3=0; P2_2=0; break;
        case 5: P2_4=0; P2_3=1; P2_2=1; break;
        case 6: P2_4=0; P2_3=1; P2_2=0; break;
        case 7: P2_4=0; P2_3=0; P2_2=1; break;
        case 8: P2_4=0; P2_3=0; P2_2=0; break;
    }
    P0 = NixieTable[Number];
}

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

void Display3DigitWithBlank(unsigned int number) {
    unsigned char hundreds, tens, units;
    
    if(number > 999) number = 999;
    
    hundreds = number / 100;
    tens = (number % 100) / 10;
    units = number % 10;
    
    // 百位：如果为0且是三位数范围，则消隐
    if(hundreds == 0) {
        // 不显示百位，但需要延时保持扫描周期
        Delay(5);
    } else {
        Nixie(6, hundreds);
        Delay(5);
    }
    P0 = 0x00;
    
    // 十位：如果百位为0且十位为0，也消隐
    if(hundreds == 0 && tens == 0) {
        Delay(5);
    } else {
        Nixie(7, tens);
        Delay(5);
    }
    P0 = 0x00;
    
    // 个位：总是显示
    Nixie(8, units);
    Delay(5);
    P0 = 0x00;
}


void main() {
	//第一个值为显示位置
	//第二个值为显示内容，P0=0x7D先作为测试
    //输入2，则表示为1111 1110
	//0xFF，表示1111 1111 所有段熄灭（共阴极数码管）
	//如果要表示3，段码通常为0x4F/0x03，即0100 1111
	//前8位决定像二进制一样叠加亮，后8位点亮右下角的.
	// 1110 1111 9. 0x6F
	// 0111 1101 6 0x7D
	// 7 0x07 0000 0110
	// 5 0x6D 0101 
	// 1 0x06 0000 0101
	// 0 0x3F 0011 1111
	int i,j,carry;
	carry=8;
	
//	Nixie(7, 9);

    while(1) {
		// 多LED常亮测试
//		Nixie(1, 1);
//		Delay20ms();
//		Nixie(2, 0);
//		Delay20ms();

//		// 自己实现的
//		for(i=1;i<=9;i++){ //i用于表示十进制位
//			for(j=1;j<=9;j++){ //j用于表示单加
//				Nixie(carry, i); //i的位置比j要提前一个位置
//				Delay(250); //动态更新
//				
//				if(carry!=8){ //如果当前位置不等于8位（最大位）则不用进位
//					Nixie(carry+1, j);
//					Delay(250);
//				}
//			}
//		}
		
//		//更好的思路
//		for(i=1;i<=99;i++){
//			Nixie(6, (i / 10) % 10);
//			Delay(20);
//			Nixie(6, i / 10);
//			Delay(20);
//			Nixie(7, i % 10);
//			Delay(20);
//		}

		Display3DigitWithBlank(123);
		
//		//跑马灯效果
//		for(i=0;i<=8;i++){
//			Nixie(i, i);
//			Delay(100);
//		}

		
//		for(i=0;i<=9;i++){
//			Nixie(1, i);
//			Delay100ms();
//			Nixie(2, i+1);
//			Delay100ms();
//			Nixie(3, i+2);
//			Delay100ms();
//			Nixie(4, i+3);
//			Delay100ms();
//		}
    }
}




//#include <REGX51.H>
//#include <INTRINS.H>

//unsigned char code NixieTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

//unsigned int displayValue = 123;  // 要显示的数字
//unsigned char displayBuff[3];      // 显示缓冲区 {百位,十位,个位}
//unsigned char scanIndex = 0;       // 当前扫描的位置

//void Nixie(unsigned char Location, unsigned char Number) {
//    if(Number > 9) Number = 0;
//    
//    switch(Location) {
//        case 1: P2_4=1; P2_3=1; P2_2=1; break;
//        case 2: P2_4=1; P2_3=1; P2_2=0; break;
//        case 3: P2_4=1; P2_3=0; P2_2=1; break;
//        case 4: P2_4=1; P2_3=0; P2_2=0; break;
//        case 5: P2_4=0; P2_3=1; P2_2=1; break;
//        case 6: P2_4=0; P2_3=1; P2_2=0; break;
//        case 7: P2_4=0; P2_3=0; P2_2=1; break;
//        case 8: P2_4=0; P2_3=0; P2_2=0; break;
//        default: return;
//    }
//    P0 = NixieTable[Number];
//}

//// 更新显示缓冲区
//void UpdateDisplay(unsigned int num) {
//    if(num > 999) num = 999;
//    displayBuff[0] = num / 100;       // 百位
//    displayBuff[1] = (num % 100) / 10; // 十位
//    displayBuff[2] = num % 10;         // 个位
//}

//// 定时器0初始化 (每2ms中断一次)
//void Timer0_Init() {
//    TMOD &= 0xF0;
//    TMOD |= 0x01;  // 模式1,16位定时器
//    TH0 = 0xF8;    // 2ms @12MHz
//    TL0 = 0x30;
//    EA = 1;        // 开总中断
//    ET0 = 1;       // 开定时器0中断
//    TR0 = 1;       // 启动定时器
//}

//// 定时器中断服务函数
//void Timer0_Isr() interrupt 1 {
//    TH0 = 0xF8;
//    TL0 = 0x30;
//    
//    // 动态扫描：轮流显示百十个位
//    switch(scanIndex) {
//        case 0: Nixie(6, displayBuff[0]); break;  // 百位
//        case 1: Nixie(7, displayBuff[1]); break;  // 十位
//        case 2: Nixie(8, displayBuff[2]); break;  // 个位
//    }
//    
//    scanIndex++;
//    if(scanIndex >= 3) scanIndex = 0;
//}

//void Delay(unsigned int xms) {  //@12.000MHz
//    unsigned char i, j;
//    while(xms--){
//		i = 2;
//		j = 239;
//		do{
//			while(--j);
//		}while(--i); // --j和j--的区别是
//	}
//}

//void main() {
//    Timer0_Init();     // 初始化定时器
//    UpdateDisplay(123); // 显示123
//    
//    while(1) {
//        // 主循环可以做其他事情
//        // 显示会自动刷新
//        
//        // 示例：计数器
//        static unsigned int cnt = 0;
//        Delay(500);
//        cnt++;
//        if(cnt > 999) cnt = 0;
//        UpdateDisplay(cnt);
//    }
//}