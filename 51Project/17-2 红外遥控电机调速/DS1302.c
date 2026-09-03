#include <REGX52.H>

sbit DS1302_SCLK=P3^6;
sbit DS1302_IO=P3^4;
sbit DS1302_CE=P3^5;

// 初始化
/**
  * @brief  
  * @param 
  * @param 
  * @retval 
  */
void Ds1302_Init(void){
	DS1302_CE=0; //使能初始为0（定义具体去看官方手册）
	DS1302_SCLK=0;
	
}

// 写
/**
  * @brief  
  * @param 
  * @param 
  * @retval 
  */
void DS1302_WriteByte(unsigned char Command, Data){
	unsigned char i;
	DS1302_CE=1;
	
//	// 将第一位 与 操作出来
//	DS1302_IO=Command&0x01;
//	DS1302_SCLK=1; //启动1微秒后关闭
//	DS1302_SCLK=0;
//	
//	// 写入第二位
//	DS1302_IO=Command&0x02;//0000 0010
//	DS1302_SCLK=1; //启动1微秒后关闭
//	DS1302_SCLK=0;
	
	//8位写入
	for(i=0;i<8;i++){
		//通过位移操作实现批量操作
		DS1302_IO=Command&(0x01<<i);
		DS1302_SCLK=1; //启动1微秒后关闭
		DS1302_SCLK=0;
	}
	
	for(i=0;i<8;i++){
		//通过位移操作实现批量操作
		DS1302_IO=Data&(0x01<<i);
		DS1302_SCLK=1; //启动1微秒后关闭
		DS1302_SCLK=0;
	}
	DS1302_CE=0;
}

// 读
/**
  * @brief  
  * @param 
  * @param 
  * @retval 
  */
unsigned char DS1302_ReadByte(unsigned char Command){
	unsigned char i, Data=0x00;
	Command|=0x01; //最低为为1
	DS1302_CE=1;
	for(i=0;i<8;i++){
		DS1302_IO=Command&(0x01<<i);
		DS1302_SCLK=0; //先给0，再给1
		DS1302_SCLK=1;
	}
//	DS1302_SCLK=0;
//	DS1302_SCLK=1;//启动时钟，改为输入内容
//	if(DS1302_IO){Data=Data|0x01;} //给最低位置1
//	DS1302_SCLK=0;
//	DS1302_SCLK=1;//启动时钟，改为输入内容
//	if(DS1302_IO){Data=Data|0x02;} //给最低位置1
	//循环实现
	
//	DS1302_IO=0; // 在读出前，先将IO口置零
	for(i=0;i<8;i++){
		DS1302_SCLK=0; //根据手册，输入需要先置0，再改为1		
		DS1302_SCLK=1;//启动时钟，改为输入内容
		if(DS1302_IO){Data=Data|(0x01<<i);} //给最低位置1
	}
	DS1302_CE=0; //使能为0
	DS1302_IO=0;
	return Data;
}

#define DS1302_SECOND		0x80
#define DS1302_MINUTE		0x82
#define DS1302_HOUR		0x84
#define DS1302_DATE		0x86
#define DS1302_MONTH		0x88
#define DS1302_DAY		0x8A
#define DS1302_YEAR		0x8C
#define DS1302_WP		0x8E

unsigned char DS1302_Time[7]={24, 6, 6, 12, 0, 0, 5};

// 设置时间
void DS1302_SetTime(void){
    // 关闭写保护
    DS1302_WriteByte(DS1302_WP, 0x00);
    
    // 十进制转BCD码写入
    DS1302_WriteByte(DS1302_YEAR,   (DS1302_Time[0] / 10 * 16) + (DS1302_Time[0] % 10));
    DS1302_WriteByte(DS1302_MONTH,  (DS1302_Time[1] / 10 * 16) + (DS1302_Time[1] % 10));
    DS1302_WriteByte(DS1302_DATE,   (DS1302_Time[2] / 10 * 16) + (DS1302_Time[2] % 10));
    DS1302_WriteByte(DS1302_HOUR,   (DS1302_Time[3] / 10 * 16) + (DS1302_Time[3] % 10));
    DS1302_WriteByte(DS1302_MINUTE, (DS1302_Time[4] / 10 * 16) + (DS1302_Time[4] % 10));
    DS1302_WriteByte(DS1302_SECOND, (DS1302_Time[5] / 10 * 16) + (DS1302_Time[5] % 10));
    DS1302_WriteByte(DS1302_DAY,    (DS1302_Time[6] / 10 * 16) + (DS1302_Time[6] % 10));
    
    // 开启写保护（可选）
    // DS1302_WriteByte(DS1302_WP, 0x80);
}

// 读取时间
void DS1302_ReadTime(void){
    unsigned char Temp;
    
    Temp = DS1302_ReadByte(DS1302_YEAR);
    DS1302_Time[0] = (Temp / 16 * 10) + (Temp % 16);
    Temp = DS1302_ReadByte(DS1302_MONTH);
    DS1302_Time[1] = (Temp / 16 * 10) + (Temp % 16);
    Temp = DS1302_ReadByte(DS1302_DATE);
    DS1302_Time[2] = (Temp / 16 * 10) + (Temp % 16);
    Temp = DS1302_ReadByte(DS1302_HOUR);
    DS1302_Time[3] = (Temp / 16 * 10) + (Temp % 16);
    Temp = DS1302_ReadByte(DS1302_MINUTE);
    DS1302_Time[4] = (Temp / 16 * 10) + (Temp % 16);
    Temp = DS1302_ReadByte(DS1302_SECOND);
    DS1302_Time[5] = (Temp / 16 * 10) + (Temp % 16);
    Temp = DS1302_ReadByte(DS1302_DAY);
    DS1302_Time[6] = (Temp / 16 * 10) + (Temp % 16);
}