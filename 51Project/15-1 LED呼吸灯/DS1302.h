#ifndef __DS1302_H__
#define __DS1302_H__

// 外部变量声明（注意：不要在这里赋值！）
extern unsigned char DS1302_Time[7];

void Ds1302_Init(void);
void DS1302_WriteByte(unsigned char Command, Data);
unsigned char DS1302_ReadByte(unsigned char Command);
void DS1302_SetTime();
void DS1302_ReadTime();

#endif
