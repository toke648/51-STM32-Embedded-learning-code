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

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
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
	int i;
	
//	Nixie(7, 9);

    while(1) {
		for(i=0;i<=9;i++){
			Nixie(7, i);
			Delay100ms();
		}
    }
}