#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define SCL_PORT		GPIOB
#define SCL_PIN			GPIO_Pin_10

void MyI2C_W_SCL(uint8_t BitValue)
{
	//BitAction通常是一个用于位操作的枚举类型或数据类型
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	//Sets or clears the selected data port bit.
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

//11是数据线，10是时钟线
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;		//返回读到SDA线的电平
}

void My12C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		// 1.开启时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;						// 2.直接引用GPIO_InitTypeDef typedef定义的类型
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;			// 3.启动模式使用开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 	// 使用引脚（多引脚用或连接）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			// 输出频率50Hz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
	
}

void MyI2C_Start(void)
{
	//先释放SCL	SDA
	MyI2C_W_SDA(1);			//通信
	MyI2C_W_SCL(1);			//时钟
	//再拉低
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
	
	//Sr确保先释放SDA再释放SCL
	
	
//	GPIO_SetBits(SCL_PORT, SCL_PIN);
//	
//	MyI2C_R_SDA(SCL_PORT, SCL_PIN);
}


//SCL不用拉低的因为接受应答之后SCL默认低电平
void MyI2C_Stop(void)
{
	//开始停止的波形按着输入就行了
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

//数据为从高位到低位>>i
void MyI2C_SendAck(uint8_t Ackbit)
{
	MyI2C_W_SDA(Ackbit); 
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

//数据为从高位到低位>>i
void MyI2C_SendByte(uint8_t Byte)
{
	//好比喻，如果破坏了规则，那要么就是开始前还没这个规则，要么就是不玩了
	uint8_t i;
	//	//要么你接受了对方还没有发送，要么对方发送了你没有接受，由于发送接受位的差异，因此不会对数据位造成干扰
	for (i=0;i<8;i++)
	{
		//按位与取出最高位
		MyI2C_W_SDA(Byte & (0x80 >> i)); //0x80 1000 0000
		//释放，拉低SCL
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

//数据为从高位到低位>>i
uint8_t MyI2C_ReceiveAck(void)
{
//	uint8_t i, Byte = 0x00;
	uint8_t AckBit;
	
	//我去，我就是隔着懵了，其实就是这里sda置1代表释放主机sda，你一释放那从机需要用的话，它会马上在sda上写入数据，且是在scl进行读取数据之前
	MyI2C_W_SDA(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
	return AckBit;

}

//数据为从高位到低位>>i
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	
	MyI2C_W_SDA(1);			//拉低等待
	for (i = 0; i < 8; i++){
		MyI2C_W_SCL(1);
		//从高位到低位接受
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}
