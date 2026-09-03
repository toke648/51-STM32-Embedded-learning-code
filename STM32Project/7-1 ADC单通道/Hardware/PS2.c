#include "stm32f10x.h"

// 摇杆引脚定义
#define JOY_X_PIN   GPIO_Pin_0   // PA0
#define JOY_Y_PIN   GPIO_Pin_1   // PA1
#define JOY_Z_PIN   GPIO_Pin_2   // PA2（按键）

void JoyStick_Init(void)
{
    // 1. ADC 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // 12MHz

    // 2. GPIO 配置（X 和 Y 轴用 ADC，Z 轴用普通输入）
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // X、Y 轴：模拟输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = JOY_X_PIN | JOY_Y_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // Z 轴：上拉输入（按键）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = JOY_Z_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ADC 配置
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // 单通道
    ADC_Init(ADC1, &ADC_InitStructure);

    // 4. 校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

    ADC_Cmd(ADC1, ENABLE);
}

// 读取 X 轴（PA0）
uint16_t JoyStick_ReadX(void)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}

// 读取 Y 轴（PA1）
uint16_t JoyStick_ReadY(void)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}

// 读取 Z 轴（按键） 1=未按下，0=按下
uint8_t JoyStick_ReadZ(void)
{
    return GPIO_ReadInputDataBit(GPIOA, JOY_Z_PIN);
}
