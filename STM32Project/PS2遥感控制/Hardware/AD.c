#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//72Mhz / 6 = 12Mhz
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//防断触模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 2, ADC_SampleTime_55Cycles5);		//通道设置
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          // 独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      // 右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
 	ADC_InitStructure.ADC_ScanConvMode = DISABLE;               // 非扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;                     // 转换通道数
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	
	ADC_ResetCalibration(ADC1);				//复位校准
	
	//set通常表示 “设置” 或 “使能” 状态。在逻辑上，它的值一般被定义为 1，用来表示某个功能或标志被激活、启用或处于高电平状态。
	//reset通常表示 “复位” 或 “清除” 状态。在逻辑上，它的值一般被定义为 0，用来表示某个功能或标志被禁用、清除或处于低电平状态。
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

//启动，等待，读取
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//1/12MHz	大约5.6us
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);

}
