#include <REGX52.H>
#include "Delay.h"


sbit RCK = P3^5;   // 锁存时钟 (RCLK) - 数据锁存引脚
sbit SCK = P3^6;   // 移位时钟 (SRCLK) - 移位时钟引脚  
sbit SER = P3^4;   // 串行数据 (SER) - 数据输入引脚

/*
三个引脚的作用（重点！）
引脚	功能	通俗比喻
SER	数据输入线	像一根水管，一次只能流1滴水（1个bit）
SCK	移位时钟	像水管的阀门，每开一次就推进1滴
RCK	锁存时钟	像蓄水池的闸门，打开后8滴水一起流出

数据输入过程（8次循环）：
┌─────────────────────────────────────────┐
│ SER: 1 → 0 → 1 → 1 → 0 → 1 → 0 → 0    │ 串行输入
│ SCK: ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑          │ 每来一个上升沿
│      把1滴 把0滴 把1滴... 推进移位寄存器    │
└─────────────────────────────────────────┘
                    ↓
              【移位寄存器】内部存储8个bit
                    ↓
              RCK给出上升沿 ↓
                    ↓
         ┌─────────┴─────────┐
         Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7  (8个并行输出)
         1  0  1  1  0  1  0  0   同时输出到LED


硬件连接示意


单片机                    74HC595
┌─────┐                 ┌──────────┐
│P3.4 ├────────────────►│SER   Q0  ├──► LED0
│P3.6 ├────────────────►│SCK   Q1  ├──► LED1
│P3.5 ├────────────────►│RCK   Q2  ├──► LED2
└─────┘                 │      Q3  ├──► LED3
                        │      Q4  ├──► LED4
                        │      Q5  ├──► LED5
                        │      Q6  ├──► LED6
                        │      Q7  ├──► LED7
                        └──────────┘

总结
这个程序是用3个IO口控制8个LED的经典例子。核心思想是：

把1个字节（8位）拆成8个bit，一个一个从SER送进去

每送1个bit，SCK给一个脉冲，让它往里走一格

8个bit都送完后，RCK给一个脉冲，8个输出引脚同时更新

代码中的问题主要是SER赋值语句写错了，需要修改为正确的位提取方式。



没有灯的话可以在74HC595的函数里面加一句P2=~Byte
*/


/**
  * @brief  点阵屏电路启动设置
  * @param unsigned char Byte
  * @retval 无
  
//	_74HC595_WriteByte(0xCD);
//	MatrixLED_ShowColumn(7,0xA0);
  */
void _74HC595_WriteByte(unsigned char Byte){
	unsigned char i;
	P2=~Byte;
	for(i=0;i<8;i++){
//		SER=Byte&0x80;
		//P2=~Byte; //这行放在循环里，每次移位都会刷新P2口8次，应该删除或移到循环外。
//		SER=Byte&(0x80>>i); //每次循环移位一位
		SER = (Byte >> (7 - i)) & 0x01;       // 错误！这样取出的不是0就是非0
		// 位移读取改为从高位先读取，而不是从地位先读取，否则是倒过来的
		
		SCK = 1;                       // 移位时钟上升沿
        SCK = 0;                       // 拉低，准备下次
		
		// 正确的写法应该是：
        // SER = (Byte >> (7 - i)) & 0x01;  // 从高位到低位依次发送
        // 或者：
        // SER = (Byte >> i) & 0x01;        // 从低位到高位发送
	}
    RCK = 1;                           // 锁存时钟上升沿
    RCK = 0;                           // 拉低，数据锁存完成
	
//	if(Byte0x80)
//	SER=0x66; //如果后面的不是0，就是1
}

/**
  * @brief  介绍 点阵屏初始化
  * @param 参数 无
  * @retval 返回值 无
  */
void MatrixLED_Init(){
	SCK = 0;                       // 移位时钟上升沿
    RCK = 0;                       // 拉低，数据锁存完成
}

/**
  * @brief  位移操作，令寄存器中的状态输出
  * @param unsigned char Column,Data
  * @param 
  * @retval void
  */
void MatrixLED_ShowColumn(unsigned char Column,Data){
	_74HC595_WriteByte(Data);
	P0=~(0x80>>Column);
	// 段选 位选 延时 位清零 段选
	Delay(1);
	P0=0xFF;
}