#include <REGX52.H>

sbit XPT2046_CS = P3^5;    // CS: 片选信号（选中芯片）
sbit XPT2046_DCLK = P3^6;  // DCLK: 时钟信号
sbit XPT2046_DIN = P3^4;   // DIN: 数据输入（单片机→芯片）
sbit XPT2046_DOUT = P3^7;  // DOUT: 数据输出（芯片→单片机）

/*
位:    D7  D6  D5  D4  D3  D2  D1  D0
      S   A2  A1  A0  MODE SER/DFR PD1 PD0


常用命令：
0x94 = 1001 0100 → 读取X坐标
0xD4 = 1101 0100 → 读取Y坐标

1. CS拉低 → 选中芯片
2. 发送8位命令 → 告诉芯片要读X还是Y
3. 芯片转换 → 需要等待（代码中没有）
4. 读取16位数据 → 得到原始AD值
5. 右移4位 → 得到12位有效数据（0-4095）
6. CS拉高 → 结束通信
*/
unsigned int XPT2046_Read(unsigned char Command){
	unsigned char i;
	unsigned int ADVAlue=0; // ✅ 初始化为0

    XPT2046_CS = 0;        // 选中芯片
    XPT2046_DCLK = 0;      // 时钟初始为低

    // 发送命令（8位）
	for(i=0;i<8;i++){
       // 发送数据位（从高位开始）
		XPT2046_DIN=Command & (0x80 >> i); // 1111 0000 对高位置1
		XPT2046_DCLK = 1;  // 时钟上升沿，芯片读取数据位
		XPT2046_DCLK = 0;  // 时钟恢复低电平
	}
	//读取数据部分
	for(i=0;i<16;i++){
		XPT2046_DCLK = 1;   // 时钟上升沿，芯片输出数据
		XPT2046_DCLK = 0;   // 时钟下降沿
		if(XPT2046_DOUT){ADVAlue |= (0x8000 >> i);}
	}
	
	XPT2046_CS=1;  //CS拉高 → 结束通信
	
	if(Command & 0x08){     // 检查命令的第3位（D3）
		return ADVAlue >> 8;  // 12位模式：右移8位，保留高4位？可能有误
	} else {
		return ADVAlue >> 4;  // 12位模式：右移4位，得到12位有效数据
	}
	
}