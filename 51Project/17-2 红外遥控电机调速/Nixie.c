#include <REGX52.H>
#include <INTRINS.H>

//缓存buffet。Nixie_Buffer
unsigned char Nixie_Buf[9]; //显像管最大值8位
unsigned char NixieTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x40};

/*
假如按键按下的是4，location给的值是1，那么
也就是把4这个值，赋给了SMG_Buf[]数组中的第一位，所以这时候SMG_Buf[]数组中第一位就是4，
SMG_Buf[location]=number; 也就变成了 SMG_Buf[1]=4;
所以在SMG_Scan();函数值中就变成了 SMG_Scan(1,4);即显示SMG_Scan()数组中第4位的值，即为4
*/
void Nixie_Scan(unsigned char Location,Number)
{
	Nixie_Buf[Location]=Number;
}

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
	//静态显示不用清零
//	Delay(1);
//	P0=0x00;
}

//显像管缓存区


//每隔2毫秒调用，刷新显像管缓存
void Nixie_Loop(void){
	//静态类型的值在内部不会因为跳出函数重置
	static unsigned char i;
	//循环遍历
	Nixie(i, Nixie_Buf[i]);
	i++;
	if(i>=9){i=1;}
//	if(i=3){P0=0x40;P2_4=0;P2_3=1;P2_2=0;}
}