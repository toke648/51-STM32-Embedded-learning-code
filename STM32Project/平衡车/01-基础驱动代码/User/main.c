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

///*OLED LED 测试程序*/
//int main(void)
//{
//	OLED_Init();
//	LED_Init();
//	
//	OLED_Printf(0,0,OLED_8X16, "Hello Wrold!");
//	OLED_Printf(0,16,OLED_6X8, "Hello Wrold!");
//	OLED_Update();
//	
//	while(1)
//	{
//		LED_ON();
//		Delay_ms(500);
//		LED_OFF();
//		Delay_ms(500); 
//		LED_Turn();
//		Delay_ms(500);
//		LED_Turn();
//		Delay_ms(500);
//		
//	}
//}


/*定时器中断&非阻塞式按键测试*/
//uint16_t Count = 0;
//uint8_t KeyNum, Num;

//int main(void)
//{
//	OLED_Init();
//	Key_Init();
//	Timer_Init();
//	
//	while(1)
//	{
//		KeyNum = Key_GetNum();
//		if (KeyNum == 1)
//		{
//			Num ++;
//		}
//		if (KeyNum == 2)
//		{
//			Num --;
//		}
//		if (KeyNum == 3)
//		{
//			Num +=10;
//		}
//		if (KeyNum == 4)
//		{
//			Num -=10;
//		}
//		
//		OLED_Printf(0, 0, OLED_8X16, "Count:%05d", Count);
//		OLED_Printf(1, 16, OLED_8X16, "Num:%03d", Num);
//		OLED_Update();
//	}
//}

//void TIM1_UP_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//    {
//        Count++;
//		
//		Key_Tick();
//        
//        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//    }
//}


/*MPU6050测试*/

///*

//如果数据可能为负数，就用 int16_t。
//如果确定永远 ≥0，就用 uint16_t。
//*/
//int16_t AX, AY, AZ, GX, GY, GZ;
//uint8_t TimerErrorFlag;
//uint16_t TimerCount;

//int main(void)
//{
//	OLED_Init();
//	MPU6050_Init();
//	Timer_Init();
//	
//	while(1)
//	{
//		OLED_Printf(0, 0, OLED_8X16, "%+06d", AX);
//		OLED_Printf(0, 16, OLED_8X16, "%+06d", AY);
//		OLED_Printf(0, 32, OLED_8X16, "%+06d", AZ);
//		OLED_Printf(64, 0, OLED_8X16, "%+06d", GX);
//		OLED_Printf(64, 16, OLED_8X16, "%+06d", GY);
//		OLED_Printf(64, 32, OLED_8X16, "%+06d", GZ);
//		OLED_Printf(0, 48, OLED_8X16, "FLAG:%1d", TimerErrorFlag);
//		OLED_Printf(64, 48, OLED_8X16, "C:%05d", TimerCount);

//		OLED_Update();
//	}
//}

//void TIM1_UP_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//    {
//		//先清理标志位，再进入中断
//		//进入中断之前的时间，标志位都会被忽略
//        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//		
//		//由定时器中断控制，每隔1毫秒进行一次读取
//		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
//    
//		// 中断之前清理标志位，中断退出，再次判断标志位
//		if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//		{
//			TimerErrorFlag = 1;
//			//退出之前，再次清理标志位
//			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//			
//			//如果TimerErrorFlag=1，证明再次进入了中断
//		}
//		
//		TimerCount = TIM_GetCounter(TIM1);
//	
//		// 1 Time ——— 2 MPU6050
//		// 中断期间再次中断，代码现象：不显示
//		
//		// 1 MPU605 ——— 2 Time
//		// MPU6050没有读取完就被中断了
//		// 代码现象：显示缓慢
//		
//		/*
//		1. 进入 ISR，清标志位
//		2. 开始执行 MPU6050_GetData（耗时 >1ms）
//		3. 定时器溢出，硬件置标志位 1
//		4. MPU6050_GetData 返回
//		5. 检查 TIM_GetITStatus → 读到 1
//		6. TimerErrorFlag = 1
//		
//		这不是“中断重叠”，而是“中断溢出”——
//		你还没处理完当前中断，下一个中断已经发生了。
//		*/
//	}
//}



		
// 电机1 运行测试
/*

状态	PB12	PB13	电机1 表现
正转	1	0	正转
停止	0	0	停止
反转	0	1	反转
停止	0	0	停止


*/


/*直流电机和编码器测试*/
uint8_t KeyNum;
int8_t PWML = 0, PWMR = 0;
float SpeedL, SpeedR;
int main(void)
{
	OLED_Init();
	Key_Init();
	Motor_Init();
	Encoder_Init();
	
	Timer_Init();
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			PWML += 10;
		}
		if (KeyNum == 2)
		{
			PWML -= 10;
		}
		if (KeyNum == 3)
		{
			PWMR += 10;
		}
		if (KeyNum == 4)
		{
			PWMR -= 10;
		}
		
		Motor_SetPWM(1,PWML);
		Motor_SetPWM(2,PWMR);
		
		OLED_Printf(0, 0, OLED_8X16, "PWML:%+04d", PWML);
		OLED_Printf(1, 16, OLED_8X16, "PWMR:%+04d", PWMR);
		OLED_Printf(0, 32, OLED_8X16, "SpeedL:%+06.2f", SpeedL);
		OLED_Printf(1, 48, OLED_8X16, "SpeedR:%+06.2f", SpeedR);
		
		OLED_Update();
	}
}

void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {		
		Key_Tick();
        
		Count ++;
		//每50ms重置一次
		if (Count >= 50)							//如果改成10，0.05改成0.01
		{
			Count = 0;
			//电机磁铁旋转轴，
			//Encoder_Get(1) 获取转速比（PWM频率/增量值）瞬时变化率
			//1转/秒	Encoder_Get(1) / 44 / 0.05 / 9.27666
			//1分钟/转	Encoder_Get(1) / 44 / 0.05 / 9.27666 * 60
			SpeedL = Encoder_Get(1) / 44.0 / 0.05 / 9.27666;
			SpeedR = Encoder_Get(2) / 44.0 / 0.05 / 9.27666;
		}
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}


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



