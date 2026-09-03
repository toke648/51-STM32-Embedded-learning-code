#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

unsigned int IR_Time;
unsigned char IR_State;

unsigned char IR_Data[4];
unsigned char IR_pData;

unsigned char IR_DataFlag;
unsigned char IR_RepeatFlag;
unsigned char IR_Address;
unsigned char IR_Command;

/*
状态机

0--空闲状态
1--触发读取
2--开始解码


晶振频率	机器周期	1us计数值	定时器1次计数时间
12MHz	1us	1	1us
11.0592MHz	1.085us	0.9216	1.085us


计算公式
实际时间(us) = 计数值 × (12 / 11.0592) = 计数值 × 1.085
或
所需计数值 = 时间(us) × (11.0592 / 12) = 时间(us) × 0.9216

修正对照表
事件	实际时间	12MHz计数值	11.0592MHz计数值	推荐范围
引导码	13.5ms	13500	12442	12000-12800
连发码	11.25ms	11250	10368	10000-10700
数据0	1.12ms	1120	1032	900-1100
数据1	2.25ms	2250	2074	2000-2150
*/


/**
  * @brief  红外遥控初始化
  * @param  无
  * @retval 无
  */
void IR_Init(void){
	Timer0Init();
	Int0_Init();
}

/**
  * @brief  红外遥控获取收到数据帧标志位
  * @param  无
  * @retval 是否收到数据帧，1为收到，0为未收到
  */
unsigned char IR_GetDataFlag(void){
	if(IR_DataFlag){
		IR_DataFlag=0;
		return 1;
	}
	return 0;
}

/**
  * @brief  红外遥控获取收到连发帧标志位
  * @param  无
  * @retval 是否收到连发帧，1为收到，0为未收到
  */
unsigned char IR_GetRepeatFlag(void){
	if(IR_RepeatFlag){
		IR_RepeatFlag=0;
		return 1;
	}
	return 0;
}


/**
  * @brief  红外遥控获取收到的地址数据
  * @param  无
  * @retval 收到的地址数据
  */
unsigned char IR_GetAddress(void){
	return IR_Address;
}

/**
  * @brief  红外遥控获取收到的命令数据
  * @param  无
  * @retval 收到的地址数据
  */
unsigned char IR_GetCommand(void){
	//单独定义一个函数，用于回传获得到的命令数据
	return IR_Command;
}

void Init0_Routine(void) interrupt 0{
	if(IR_State==0){				//状态0，空闲状态
		P2=0x01; // 测试点1：进入状态0
		Timer0_SetCounter(0);		//定时计数器清0
		Timer0_Run(1);				//定时器启动
		IR_State=1;					//置状态为1
	}
	else if(IR_State==1){			//状态1，等待等待start信号或repeat信号
		P2=0x02; // 测试点2：进入状态1
		IR_Time=Timer0_GetCounter();	//获取上一次中断到此次中断的时间
		Timer0_SetCounter(0);		//定时计数器清0
		//定时器运行1微妙计时检测
		//如果计时为13.5ms，则接受到了Start信号（判定值在12MHz晶振下为13500，在11.0592MHz晶振下为12442）		
		if(IR_Time>12442-500 && IR_Time<12442+500)
		{
			P2=0x03;  // 测试点3：收到引导码
			IR_State=2;				//置状态为2
		}
		else if(IR_Time>10368-500 && IR_Time<10368+500){
			P2=0x04;  // 测试点4：收到连发码
			IR_RepeatFlag=1;		//置收到连发帧标志位为1
			Timer0_Run(0);			//定时器停止
			IR_State=0;				//置状态为0
		}
		else						//接受出错
		{
			P2=0x05;  // 测试点5：无效信号
			IR_State=1;				//置状态为1（重新等待和接受）
		}
	}
	else if(IR_State==2)			//状态2，接受数据
	{
		P2=0x06; // 测试点6：进入状态2（接收数据）
		IR_Time=Timer0_GetCounter();	//获取上一次中断到此次中断的时间
		Timer0_SetCounter(0);		//定时计数器清0
		
		//如果计时为1120us，则接收到了数据0（）
		if(IR_Time>1032-500 && IR_Time<1032+500){
			P2=0x07;  // 测试点7：收到数据0
			//对接受到的数据处理，或等于
			IR_Data[IR_pData/8] &= ~(0x01<<(IR_pData%8));  //数据对应位置1
			IR_pData++;			//数据位置指针自增
		}
		//如果计时为2250us，则接收到了数据1（判定值在12MHz晶振下为2250，在11
		else if(IR_Time>2074-500 && IR_Time<2074+500){
			P2=0x08;  // 测试点8：收到数据1
			IR_Data[IR_pData/8] |= (0x01<<(IR_pData%8));  //数据对应位置1
			IR_pData++;			//数据位置指针自增
		}
		else{
			P2=0x09;  // 测试点9：数据错误
			//如果没有在以上两个范围内（接受出错）
			IR_pData=0;			//数据位置指针清0
			IR_State=1;			//重新开始
		}
		if(IR_pData>=32)		//如果接受到了32位数据
		{
			P2=0x0A;  // 测试点10：接收完成
			IR_pData=0;			//数据位置指针清0
			if((IR_Data[0]==~IR_Data[1]) && (IR_Data[2]==~IR_Data[3]))	//数据验证
			{
				P2 = 0x0B;  // 测试点11：验证通过
				IR_Address=IR_Data[0];	//转存数据
				IR_Command=IR_Data[2];	//转存指令
				IR_DataFlag=1;		//置收到连发帧标志位为1
			}
			Timer0_Run(0); 		//定时器停止
			IR_State=0;			//重置状态为0
		}
	}
}