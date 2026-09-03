#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0		//MPU6050的I2C从机地址

// 写寄存器
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);  // 设备地址 + 写
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);         // 寄存器地址
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data);               // 数据
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

//读
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);      // 写地址
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();
    
	MyI2C_Start();              // ✅ 重启动
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01);      // 读地址
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte(); // ✅ 读数据
    MyI2C_SendAck(1);           // 非应答
    
	MyI2C_Stop();
    
    return Data;
}

//读取多字节的方案

/*

┌─────────────────────────────────────┐
│         应用层 (main.c)              │
│   调用 MPU6050_GetData() 读取数据     │
└────────────────┬────────────────────┘
                 ▼
┌─────────────────────────────────────┐
│         驱动层 (MPU6050.c)           │
│   MPU6050_ReadRegs() 批量读取        │
│   MPU6050_GetData() 解析数据         │
└────────────────┬────────────────────┘
                 ▼
┌─────────────────────────────────────┐
│         底层 (MyI2C.c)              │
│    I2C 起始/停止/发送/接收/应答      │
└─────────────────────────────────────┘

旧方案问题： 每次 MPU6050_ReadReg 都要执行完整的 I2C 流程（起始+写地址+写寄存器+重起始+读地址+读数据+停止），12 次读取耗时过长。

新方案（批量读取）
优点： 一次 I2C 操作读完所有数据，速度快了 10 倍以上。




S  写地址  A  写寄存器  A  Sr  读地址  A  读数据1  A  读数据2  A  ...  读数据N  NA  P
│  └─0xD0─┘ │  └─0x3B─┘ │  └─0xD1─┘ │  └─Data0─┘ │  └─Data1─┘ │  └─DataN─┘ │
│            │           │           │            │            │            │
└────────────┴───────────┴───────────┴────────────┴────────────┴────────────┘
S  = Start (起始)
Sr = Restart (重起始)
A  = ACK (应答)
NA = NACK (非应答)
P  = Stop (停止)

*/
void MPU6050_ReadRegs(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count)
{
    uint8_t i;
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);      // 写地址
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();
    
	MyI2C_Start();              // ✅ 重启动
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01);      // 读地址
    MyI2C_ReceiveAck();

    for (i=0; i<Count; i++)
	{
		DataArray[i] = MyI2C_ReceiveByte(); // ✅ 读数据
		if(i < Count - 1)		//如果没有到最后一个
		{
			MyI2C_SendAck(0);
		}
		else
		{
			MyI2C_SendAck(1);
		}
//		MyI2C_SendAck(1);           // 非应答
	}
	MyI2C_Stop();
}

//void MPU6050_Init(void)
//{
//	MyI2C_Init();

//}

//初始寄存器配置(详细根据手册)
void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//GyroConfig 为0x18，标志寄存器两位均为1，+-2000°每秒
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}


//由于寄存器地址是顺序的，也可以使用i传递地址快速实现
//void MPU6050_GetData(
//	int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
//	int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ
//	)
//{
//	uint8_t DataH, DataL;
//	
//	//分别读取寄存器的高8位和低8位
//	//即为陀螺仪的数字模拟量（原理类似两侧感应弹簧）
//	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
//	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
//	*AccX = (DataH << 8 | DataL);
//	
//	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
//	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
//	*AccY = (DataH << 8 | DataL);
//	
//	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
//	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
//	*AccZ = (DataH << 8 | DataL);
//	
//	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
//	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
//	*GyroX = (DataH << 8 | DataL);
//	
//	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
//	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
//	*GyroY = (DataH << 8 | DataL);
//	
//	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
//	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
//	*GyroZ = (DataH << 8 | DataL);
//	
//}


void MPU6050_GetData(
	int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
	int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ
	)
{
	uint8_t Data[16];
	
	MPU6050_ReadRegs(MPU6050_ACCEL_XOUT_H, Data, 14);		//一次性读出14个字节的寄存器
	*AccX = (Data[0] << 8 | Data[1]);
	*AccY = (Data[2] << 8 | Data[3]);
	*AccZ = (Data[4] << 8 | Data[5]);
	
	*GyroX = (Data[8] << 8 | Data[9]);
	*GyroY = (Data[10] << 8 | Data[11]);
	*GyroZ = (Data[12] << 8 | Data[13]);

}
