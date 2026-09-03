#include <REGX52.H>
#include "OneWire.h"
#include "LCD1602.h"

#define DS18B20_SKIP_ROM    0xCC
#define DS18B20_CONVERT_T   0x44
#define DS18B20_READ_ROM    0xBE  // 读取温度命令

/**
  * @brief 启动温度转换
  * @param 无
  * @retval 无
  */
void DS18B20_ConvertT(void){
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_CONVERT_T);
}

/**
  * @brief 读取温度
  * @param 无
  * @retval 温度值（摄氏度）
  */
float DS18B20_ReadT(void){
	unsigned char TLSB,TMSB;
	int Temp;
	float T;
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_ROM);  
	
    TLSB = OneWire_ReceiveByte();       // 读低8位
    TMSB = OneWire_ReceiveByte();       // 读高8位
	
	LCD_ShowBinNum(1,1,TMSB,8);
	LCD_ShowBinNum(1,9,TLSB,8);
	
    Temp = (TMSB << 8) | TLSB;          // 合并为16位
    T = Temp / 16.0;                    // 转换为实际温度
	
	return T;
}