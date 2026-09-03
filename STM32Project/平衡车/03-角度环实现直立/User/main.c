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
#include "PID.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>



int16_t AX, AY, AZ, GX, GY, GZ;
uint8_t TimerErrorFlag;
uint16_t TimerCount;

float AngleAcc;										//存储加速度计计算得到的俯仰角
float AngleGyro;
float Angle;

uint8_t KeyNum, RunFlag;
//		左轮	   右轮
int16_t LeftPWM, RightPWM;
//	   均分PWM  差分PWM
int16_t AvePWM, DifPWM;


/*
关于PID调参的思想



*/
PID_t AnglePID = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,
	
	.OutMax = 100,
	.OutMin = -100,
};

int main(void)
{
	LED_Init();
	OLED_Init();
	MPU6050_Init();
	BlueSerial_Init();
	Key_Init();
	Motor_Init();
	Encoder_Init();
	Serial_Init();
	
	Timer_Init();
		
	while(1)
	{
		if (RunFlag){LED_ON();} else {LED_OFF();}		//PID启停指示灯
		
		KeyNum = Key_GetNum();
		if (KeyNum == 1)								//GPIOB1
		{
			RunFlag = !RunFlag;							//控制启停
		}
		
//		OLED_Printf(0, 0, OLED_8X16, "%+06d", AX);
//		OLED_Printf(0, 16, OLED_8X16, "%+06d", AY);
//		OLED_Printf(0, 32, OLED_8X16, "%+06d", AZ);
//		OLED_Printf(64, 0, OLED_8X16, "%+06d", GX);
//		OLED_Printf(64, 16, OLED_8X16, "%+06d", GY);
//		OLED_Printf(64, 32, OLED_8X16, "%+06d", GZ);
//		OLED_Printf(0, 48, OLED_8X16, "FLAG:%1d", TimerErrorFlag);
//		OLED_Printf(64, 48, OLED_8X16, "C:%05d", TimerCount);

		OLED_Clear();
		OLED_Printf(0, 0, OLED_6X8, "  Angle");
		OLED_Printf(0, 8, OLED_6X8, "  P:%05.2f", AnglePID.Kp);
		OLED_Printf(0, 16, OLED_6X8, "  I:%05.2f", AnglePID.Ki);
		OLED_Printf(0, 24, OLED_6X8, "  D:%05.2f", AnglePID.Kd);
//		OLED_Printf(0, 32, OLED_6X8, "  T:%05.2f", AnglePID.Target);	//目标值
		OLED_Printf(0, 32, OLED_6X8, "  T:%05.2f", Angle);	//目标值
		OLED_Printf(0, 40, OLED_6X8, "  A:%05.2f", AnglePID.Actual);		//正确值
		OLED_Printf(0, 48, OLED_6X8, "  O:%05.2f", AnglePID.Out);		//输出值
		
		OLED_Update();
		
		//使用蓝牙调参
		if (BlueSerial_RxFlag == 1)
		{
			//按键数据包
			char *Tag = strtok(BlueSerial_RxPacket, ",");
			if (strcmp(Tag, "key") == 0)
			{
				char *Name = strtok(NULL, ",");
				char *Action = strtok(NULL, ",");
				
			}
			//滑杆数据包
			else if (strcmp(Tag, "slider") == 0)
			{
				char *Name = strtok(NULL, ",");
				char *Value = strtok(NULL, ",");
				
				if (strcmp(Name, "AngleKp") == 0)
				{
					//转换Value为浮点数
					AnglePID.Kp = atof(Value);	//使用滑杆调整Kp的PID值
				}
				else if (strcmp(Name, "AngleKi") == 0)
				{
					AnglePID.Ki = atof(Value);
				}
				else if (strcmp(Name, "AngleKd") == 0)
				{
					AnglePID.Kd = atof(Value);
				}
			}
			//遥感数据包
			else if (strcmp(Tag, "joystick") == 0)
			{
				int8_t LH = atoi(strtok(NULL, ","));
				int8_t LV = atoi(strtok(NULL, ","));
				int8_t RH = atoi(strtok(NULL, ","));
				int8_t RV = atoi(strtok(NULL, ","));
				
			}
			
			BlueSerial_RxFlag = 0;
		}

		
		BlueSerial_Printf("[plot,%f,%f,%f]", AngleAcc, AngleGyro, Angle);
	}
}

void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count0;
	
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
		Key_Tick();
		
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		
		/*
		定时器资源是有限的，基础分频1ms，通过分频得到其他定时时间，实现一个分频器做更多事情
		
		角度环PID控制流程
		1.读取和计算俯仰角
		2.PID计算
		3.把PID输出的控制量，作用于电机
		*/
		Count0 ++;
		if (Count0 >= 10)								//（时钟周期）每10ms进行一次
		{
			Count0 = 0;
			
			MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
			
			GY -= 17;									//修补零飘的误差现象
			
//			AngleAcc = -atan2(AX, AZ) / 3.14159 * 180;
			AngleAcc = -atan2(AX, AZ) / 3.14159 * 180 - 0.6	;

			AngleGyro = Angle + GY / 32768.0 * 2000 * 0.01;		//0.001是周期，原本是1ms，现在需要改成10ms，即0.01
			
			//互补滤波在每次计算俯仰角后加一个偏移
			//原来1ms计算一次，那么1ms就会加1000次偏移，现在改成10ms，100次偏移
			float Alpha = 0.01;
			Angle = Alpha * AngleAcc + (1 - Alpha) * AngleGyro;
			
			if (Angle > 50 || Angle < -50)
			{
				RunFlag = 0;							//如果俯仰角超过阈值，停止
			}
			
			//在RunFlag时触发，防止失控
			if (RunFlag)				//Key可以控制RunFlag，只有当RunFlag==1时启动 否则执行PID调控代码
			{
				//(俯仰角 ——> 角度环)
				AnglePID.Actual = Angle;
				PID_Update(&AnglePID);
				//取出PID输出，作用于平均PWM控制
				AvePWM = -AnglePID.Out;				//极性反了，反转一下			//作用于平均PWM，转换并得到左轮PWM和右轮PWM，再输出给电机调整小车姿态
				
				LeftPWM = AvePWM + DifPWM / 2;
				RightPWM = AvePWM - DifPWM / 2;
				
				//阈值限制
				if (LeftPWM > 100) {LeftPWM = 100;} else if (LeftPWM < -100) {LeftPWM = -100;}
				if (RightPWM > 100) {RightPWM = 100;} else if (RightPWM < -100) {RightPWM = -100;}

				Motor_SetPWM(1, LeftPWM);
				Motor_SetPWM(2, RightPWM);
			
			}
			else
			{
				Motor_SetPWM(1, 0);
				Motor_SetPWM(2, 0);
			}
		}
		
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
		{
			TimerErrorFlag = 1;
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		}
		
		TimerCount = TIM_GetCounter(TIM1);

	}
}
