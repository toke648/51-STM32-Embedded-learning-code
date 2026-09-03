#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "MPU6050.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "BlueSerial.h"
#include <math.h>

int16_t AX, AY, AZ, GX, GY, GZ;
uint8_t TimerErrorFlag;
uint16_t TimerCount;

float AngleAcc;					//存储加速度计计算得到的俯仰角
float AngleGyro;
float Angle;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	BlueSerial_Init();
	
	Timer_Init();
	
	while(1)
	{
		OLED_Printf(0, 0, OLED_8X16, "%+06d", AX);
		OLED_Printf(0, 16, OLED_8X16, "%+06d", AY);
		OLED_Printf(0, 32, OLED_8X16, "%+06d", AZ);
		OLED_Printf(64, 0, OLED_8X16, "%+06d", GX);
		OLED_Printf(64, 16, OLED_8X16, "%+06d", GY);
		OLED_Printf(64, 32, OLED_8X16, "%+06d", GZ);
		OLED_Printf(0, 48, OLED_8X16, "FLAG:%1d", TimerErrorFlag);
		OLED_Printf(64, 48, OLED_8X16, "C:%05d", TimerCount);

		OLED_Update();
		
		//角度	AX, AY, AZ
//		BlueSerial_Printf("[plot,%d,%d,%d]", AX, AY, AZ);
		
		//对于当前角度值的积分，求得的相对于这个状态，改变了多少
//		BlueSerial_Printf("[plot,%d,%d,%d]", GX, GY, GZ);
		
		//俯仰角
//		BlueSerial_Printf("[plot,%f]", AngleAcc);
		
		//分别查看 加速度计 和 陀螺仪（角速度计）
//		BlueSerial_Printf("[plot,%f,%f]", AngleAcc, AngleGyro);

		//互补滤波
		//滤波后角度 = 陀螺仪角度 + a * （加速度计数器 - 陀螺仪角度）
		//加速度计角度 - 陀螺仪角度
		BlueSerial_Printf("[plot,%f,%f,%f]", AngleAcc, AngleGyro, Angle);
		
		
		/*
		互补滤波（Angle）
		陀螺仪角度（AngleGyro）
		加速度计角度（AngleAcc）
		
		
		修正项是：\(\boldsymbol{\alpha \cdot (AngleAcc - AngleGyro)}\)，分两种情况：情况 1：\(AngleGyro > AngleAcc\)\(AngleAcc - AngleGyro\) 结果为负数
		\(Angle = AngleGyro + 负数\)
		也就是：陀螺仪角度减去一个值，数值变小，向更小的 AngleAcc 靠拢。举个例子：
		\(AngleGyro=10°，AngleAcc=5°，\alpha=0.03\)
		\(Angle = 10 + 0.03\times(5-10) = 10 - 0.15 = 9.85°\)
		Gyro 数值被拉低，靠近 Acc。情况 2：\(AngleGyro < AngleAcc\)\(AngleAcc - AngleGyro\) 结果为正数
		\(Angle = AngleGyro + 正数\)
		陀螺仪角度加上一个值，数值变大，向更大的 AngleAcc 靠拢。例子：
		\(AngleGyro=3°，AngleAcc=8°，\alpha=0.03\)
		\(Angle = 3 + 0.03\times(8-3) = 3 + 0.15 = 3.15°\)
		Gyro 数值被拉高，靠近 Acc。三、总结正确理解
		\(\alpha\) 是固定比例系数（你代码里设的 0.001/0.03），不是通过三条角度实时算出来的；
		修正项符号完全由「加速度计角度 − 陀螺仪角度」决定：

		Gyro 比 Acc 大 → 修正项为负，Gyro 被往下拉；
		Gyro 比 Acc 小 → 修正项为正，Gyro 被往上抬；


		最后必须执行 AngleGyro = Angle，让下一轮积分以融合后的角度为基准，持续缓慢消除陀螺仪漂移，这也是课件里强调的关键一步。
		
		*/
		
		//另一种表达形式是
		//a = Angle - AngleGyro / (AngleAcc - AngleGyro)
		//a = 互补滤波 - 陀螺仪角度 / （加速度计角度 - 陀螺仪角度）
		//对于分子 Angle - AngleGyro
		//
		//对于分母 (AngleAcc - AngleGyro)
		//当 Gyro > Acc 时，
		//Gyro就会加一个正值，靠近Acc
	}
}

void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
		//先清理标志位，再进入中断
		//进入中断之前的时间，标志位都会被忽略
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		
		//由定时器中断控制，每隔1毫秒进行一次读取
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		
		//读取MPU6050原始数据后，计算俯仰角
		//AX / 1.0 / AZ 反正切函数九十俯仰角
		//角度转换为弧度
		
		//因为主要对平衡车平衡影响的关键是X轴和Z轴
		//因此通过对两个角度取反正切就可以得到
		
		//设为0，查看零飘误差程度，然后将误差加减回来
		GY -= 17;	//修补零飘的误差现象
		
		/*
		atan(AX/AZ) 不是简单的"标准化特征"，
		它是把两个力的比值通过三角函数映射成有物理意义的倾斜角度，
		排除了重力大小的干扰，让角度只取决于"方向"而非"重量"。
		*/
//		AngleAcc = atan(AX / 1.0 / AZ) / 3.14159 * 180;
		//由于超过180度会出现跳变，因此改用atan2
		AngleAcc = -atan2(AX, AZ) / 3.14159 * 180;

		// AngleGyro = AngleGyro + 角速度 * 时间
		//缩放为标准的单位
		//上面的不用缩放是因为，AX * AZ，只看大小关系，就可以得到角度，没有缩放的必要
		
		//Angle累加
		AngleGyro = Angle + GY / 32768.0 * 2000 * 0.001;
		
		/*
		// 俯仰角（绕X轴旋转）
		AngleGyro = Angle + GX / 32768.0 * 2000 * 0.001;

		// 或者如果你定义俯仰角为绕Y轴旋转
		AngleGyro = Angle + GY / 32768.0 * 2000 * 0.001
		*/
		
		float Alpha = 0.001;
		Angle = Alpha * AngleAcc + (1 - Alpha) * AngleGyro;
		
		// 中断之前清理标志位，中断退出，再次判断标志位
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
		{
			TimerErrorFlag = 1;
			//退出之前，再次清理标志位
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
			
			//如果TimerErrorFlag=1，证明再次进入了中断
		}
		
		TimerCount = TIM_GetCounter(TIM1);
	
		// 1 Time ——— 2 MPU6050
		// 中断期间再次中断，代码现象：不显示
		
		// 1 MPU605 ——— 2 Time
		// MPU6050没有读取完就被中断了
		// 代码现象：显示缓慢
		
		/*
		1. 进入 ISR，清标志位
		2. 开始执行 MPU6050_GetData（耗时 >1ms）
		3. 定时器溢出，硬件置标志位 1
		4. MPU6050_GetData 返回
		5. 检查 TIM_GetITStatus → 读到 1
		6. TimerErrorFlag = 1
		
		这不是“中断重叠”，而是“中断溢出”——
		你还没处理完当前中断，下一个中断已经发生了。
		*/
	}
}



		
// 电机1 运行测试
/*

状态	PB12	PB13	电机1 表现
正转	1	0	正转
停止	0	0	停止
反转	0	1	反转
停止	0	0	停止


*/


///*直流电机和编码器测试*/
//uint8_t KeyNum;
//int8_t PWML = 0, PWMR = 0;
//float SpeedL, SpeedR;
//int main(void)
//{
//	OLED_Init();
//	Key_Init();
//	Motor_Init();
//	Encoder_Init();
//	
//	Timer_Init();
//	
//	while(1)
//	{
//		KeyNum = Key_GetNum();
//		if (KeyNum == 1)
//		{
//			PWML += 10;
//		}
//		if (KeyNum == 2)
//		{
//			PWML -= 10;
//		}
//		if (KeyNum == 3)
//		{
//			PWMR += 10;
//		}
//		if (KeyNum == 4)
//		{
//			PWMR -= 10;
//		}
//		
//		Motor_SetPWM(1,PWML);
//		Motor_SetPWM(2,PWMR);
//		
//		OLED_Printf(0, 0, OLED_8X16, "PWML:%+04d", PWML);
//		OLED_Printf(1, 16, OLED_8X16, "PWMR:%+04d", PWMR);
//		OLED_Printf(0, 32, OLED_8X16, "SpeedL:%+06.2f", SpeedL);
//		OLED_Printf(1, 48, OLED_8X16, "SpeedR:%+06.2f", SpeedR);
//		
//		OLED_Update();
//	}
//}

//void TIM1_UP_IRQHandler(void)
//{
//	static uint16_t Count;
//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//    {		
//		Key_Tick();
//        
//		Count ++;
//		//每50ms重置一次
//		if (Count >= 50)							//如果改成10，0.05改成0.01
//		{
//			Count = 0;
//			//电机磁铁旋转轴，
//			//Encoder_Get(1) 获取转速比（PWM频率/增量值）瞬时变化率
//			//1转/秒	Encoder_Get(1) / 44 / 0.05 / 9.27666
//			//1分钟/转	Encoder_Get(1) / 44 / 0.05 / 9.27666 * 60
//			SpeedL = Encoder_Get(1) / 44.0 / 0.05 / 9.27666;
//			SpeedR = Encoder_Get(2) / 44.0 / 0.05 / 9.27666;
//		}
//        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//    }
//}


///*串口和蓝牙串口测试*/
//uint8_t Count;

//int main(void)
//{
//	OLED_Init();
//	Serial_Init();
//	BlueSerial_Init();
//	
//	OLED_Printf(0,0,OLED_8X16, "000000");
//	
//	Serial_SendString("Hello");
//	Serial_Printf("World");
//	
//	BlueSerial_SendString("Hello");
//	BlueSerial_Printf("World");
//	
//	OLED_Update();
//	
//	while (1)
//	{
//		if (Serial_GetRxFlag() == 1)
//		{
//			Count ++;
//			OLED_Printf(0, 16, OLED_8X16, "Count:%05d", Count);
//			
//			uint8_t RxData = Serial_GetRxData();
//			OLED_Printf(0, 0, OLED_8X16, "RxData:%02X", RxData);
//			
//			OLED_Update();
//		}
//		
//		if (BlueSerial_RxFlag == 1)
//		{
//			Count ++;
//			OLED_Printf(0, 16, OLED_8X16, "Count:%05d", Count);

//			OLED_Printf(0, 16, OLED_8X16, "%s", BlueSerial_RxPacket);
//			OLED_Update();
//			
//			BlueSerial_RxFlag = 0;
//		}
//	}
//}



