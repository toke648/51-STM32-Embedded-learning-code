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
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}


//由于寄存器地址是顺序的，也可以使用i传递地址快速实现
void MPU6050_GetData(
	int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
	int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ
	)
{
	uint8_t DataH, DataL;
	
	//分别读取寄存器的高8位和低8位
	//即为陀螺仪的数字模拟量（原理类似两侧感应弹簧）
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8 | DataL);
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8 | DataL);
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8 | DataL);
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8 | DataL);
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8 | DataL);
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8 | DataL);
	
}
