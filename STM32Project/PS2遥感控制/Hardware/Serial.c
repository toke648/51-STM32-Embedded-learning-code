#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include <stdarg.h>				/// C 语言中可变参数（可变参数）相关的宏，需要包含 <stdarg.h> 头文件。

//定义封装，方便管理和阅读
//uint8_t Serial_RxData;
//uint8_t Serial_TxPacket[4];
char Serial_RxPacket[100];		//接受缓存区
uint8_t Serial_RxFlag;

void Serial_Init(void){
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		//开启USART时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOB的时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
	
	/*GPIO初始化 用于接受*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PB0和PB1引脚初始化为上拉输入
	
	/*GPIO初始化 用于发送*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PB0和PB1引脚初始化为上拉输入
	
	/*初始化USART*/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;		//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//不使用流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//同时开启接受和发送
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//停止位bit
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字节长度（最后一位用于校验）
	USART_Init(USART1, &USART_InitStructure);
	
	//开启中断,RXNE标志位到NVIC的输出
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//优先分组，传递数组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//USART_IT_RXNE置1，就会申请中断
	
	USART_Cmd(USART1, ENABLE);
	
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//Byte写入到TXD
	//等待标志位(发送寄存器空)
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//发送数据标志位
}

//发送数组
//传递数组需要指针,由于无法判断数组是否结束，需要再传递一个Length
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i=0; i<Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}

//传递的是指针
/*
用内存地址来理解
假设字符串 "ABC" 在内存中是这样存储的：

text
内存地址:  0x100   0x101   0x102   0x103
           ┌──────┬──────┬──────┬──────┐
           │  'A'  │  'B'  │  'C'  │  '\0' │
           └──────┴──────┴──────┴──────┘
              ↑
           String 指向这里 (0x100)

char *String = "ABC";

// 初始状态
String 指向 0x100 → 内容是 'A'

// 第一次后移 String++
String 指向 0x101 → 内容是 'B'

// 第二次后移 String++
String 指向 0x102 → 内容是 'C'

// 第三次后移 String++
String 指向 0x103 → 内容是 '\0' (结束符)
*/
void Serial_SendString(char *String)
{
	while(*String != '\0')  // 判断指针指向的字符
    {
        Serial_SendByte(*String);
        String++;  // 指针后移
    }
	
//	//另一种实现方法
//	int i = 0;
//    while(String[i] != '\0')  // 用下标访问
//    {
//        Serial_SendByte(String[i]);
//        i++;  // 下标递增，相当于指针后移
//    }
	
//	uint16_t i;
//	//只要不是字符的终止标志位，常用于字符串的遍历
//	for(i=0; String[i] != '\0'; i++)
//	{
//		Serial_SendByte(String[i]);
//	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	//从高位到低位发送，
	for(i=0; i<Length; i++)
	{
		//需要显示字符还要加偏移，即0
		//否则就会从ASCII的第一位开始算了
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

//串口重定向，将printf输入的内容重定向到串口发送
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

// ...用于接受可变参数列表，类似python
void Serial_Printf(char *format, ...)
{
	//相当于把打了压缩包的内容在内部打包+解包
	char String[100];				//定义参数列表
	va_list arg;					//var_list类型
	va_start(arg, format);			//接受到的内容保存在format（格式化字符串）
	vsprintf(String, format, arg);	//vsprintf接受封装参数
	va_end(arg);					//释放参数表
	Serial_SendString(String);
}

////读取标志位后立刻清零
//uint8_t Serial_GetRxFlag(void)
//{
//	if(Serial_RxFlag == 1)
//	{
//		Serial_RxFlag = 0;
//		return 1;
//	}
//	return 0;
//}

////自动发送数据配置
//void Serial_SendPacket(void)
//{
//	Serial_SendByte(0xFF);
//	Serial_SendArray(Serial_TxPacket, 4);
//	Serial_SendByte(0xFE);
//}
//	

//uint8_t Serial_GetRxData(void)
//{
//	return Serial_RxData;
//}


// ✅ 取消注释这个函数
uint8_t Serial_GetRxFlag(void)
{
    if(Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}

// 中断函数修改
void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;
    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);
        
        if (RxState == 0)
        {
            if (RxData == '@' && Serial_RxFlag == 0)
            {
                RxState = 1;
                pRxPacket = 0;        // ✅ 重置指针
            }
        }
        else if (RxState == 1)
        {
            if (RxData == '\r')
            {
                RxState = 2;
            }
            else
            {
                if (pRxPacket < 99)   // ✅ 防止数组越界
                {
                    Serial_RxPacket[pRxPacket] = RxData;
                    pRxPacket++;
                }
            }
        }
        else if (RxState == 2)
        {
            if (RxData == '\n')
            {
                RxState = 0;
                Serial_RxPacket[pRxPacket] = '\0';
                Serial_RxFlag = 1;
                pRxPacket = 0;        // ✅ 重置指针
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}