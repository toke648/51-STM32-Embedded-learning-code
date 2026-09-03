#include <REGX52.H>

sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;


/**
  * @brief I2C启动
  * @param void
  * @retval void
  */
void I2C_Start(void){
	//默认总线空闲状态 高电平
	I2C_SDA=1;
	I2C_SCL=1;
	//再将总线拉低，相当于初始化
	I2C_SDA=0;
	I2C_SCL=0;

}

/**
  * @brief  I2C停止
  * @param 无
  * @retval 无
  */
void I2C_Stop(void){
	//确保默认状态为零再拉高
	I2C_SDA=0;
	
	//如果时钟频率过快，需要根据手册更改通信速率
	I2C_SCL=1;
	I2C_SDA=1;
	
}


/**
  * @brief I2C发送一个字节
  * @param 无
  * @retval 无
  */
void I2C_SendByte(unsigned char Byte){
	unsigned char i;
	//直接通过位移操作，更新内容
	for(i=0;i<8;i++){
		I2C_SDA = (Byte & (0x80>>i)) ? 1 : 0;
	//	I2C_SDA=Byte&0x80; // 1111 0000 将SDA状态改为最高位
		//置高，然后再置低
		I2C_SCL=1;
		I2C_SCL=0;
	}
}


/**
  * @brief I2C接受一个字节
  * @param 无
  * @retval 接受到的一个字节的数据
  */
unsigned char I2C_ReceiveByte(void)
{
	unsigned char i;
	 unsigned char Byte = 0x00;
	
	// 先置1释放总线
	I2C_SDA=1;
	
	for(i=0;i<8;i++){
		 Byte <<= 1;
		//读取前先拉高
		I2C_SCL=1;
		// 1010 0000 | 1111 0000 = 1111 0000
		if(I2C_SDA){Byte |= 0x01;}
		I2C_SCL=0;//拉高后状态改为等待状态
	}
	
	
	return Byte;
}

//变量bit 1位，只能存储0/1
//unsigned -127 —— 127
/**
  * @brief I2C发送应答
* @param AckBit 应答为，0为应答，1为非应答
  * @retval 无
  */
void I2C_SendAck(unsigned char AckBit){
	I2C_SDA=AckBit;
	I2C_SCL=1;
	I2C_SCL=0;
}

/*SDA控制权交给从机后，从机就会把SDA置1或0（从机发送应答，主机接受应答）
主机接受应答时，从机时自动发送应答的，不是外面操作从机做出什么应答

SCL为主机
SDA为从机

sda=1是主机释放控制权，运行到ackbit=sda的时候sda已经接收到设备应答回来的0了，已经变了
就是说数据一直在SDA上，应答数据也在SDA上，发送应答是主机去应答，接受是从机的应答读给ack给主机
要想明白这个总线不是一直在主机的控制，从机也可以控制的啊，所以这里只需要重置SDA，就相当于放手，把控制权交给从机，然后我们读SDA就行了
*/
/**
  * @brief I2C接受应答
  * @param void
  * @retval AckBit接受到的应答位，0为应答，1为非应答
  */
unsigned char I2C_ReceiveAck(void){
	unsigned char AckBit;
	I2C_SDA=1; //先释放
	I2C_SCL=1;//再拉高
	AckBit=I2C_SDA;
	I2C_SCL=0;
	return AckBit;
}