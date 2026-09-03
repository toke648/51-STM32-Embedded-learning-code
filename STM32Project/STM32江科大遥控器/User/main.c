/**
  * 江科大STM32F103C8T6遥控器 - OLED显示测试程序（修正版）
  */

#include "stm32f10x.h"
#include "string.h"

// ========== 引脚配置区（修改这里即可更换引脚）==========
#define IIC_SCL_GPIO     GPIOB
#define IIC_SDA_GPIO     GPIOB
#define IIC_SCL_PIN      GPIO_Pin_14   // 可改为 PB6
#define IIC_SDA_PIN      GPIO_Pin_15   // 可改为 PB7
#define IIC_CLOCK        RCC_APB2Periph_GPIOB

// 引脚操作宏
#define SCL_HIGH()       GPIO_SetBits(IIC_SCL_GPIO, IIC_SCL_PIN)
#define SCL_LOW()        GPIO_ResetBits(IIC_SCL_GPIO, IIC_SCL_PIN)
#define SDA_HIGH()       GPIO_SetBits(IIC_SDA_GPIO, IIC_SDA_PIN)
#define SDA_LOW()        GPIO_ResetBits(IIC_SDA_GPIO, IIC_SDA_PIN)
#define READ_SDA()       GPIO_ReadInputDataBit(IIC_SDA_GPIO, IIC_SDA_PIN)

// OLED地址和命令
#define OLED_ADDR        0x78
#define OLED_CMD_MODE    0x00
#define OLED_DATA_MODE   0x40

// 简单延时（微秒级）
void delay_us(uint32_t us)
{
    uint32_t i;
    for(i = 0; i < us * 8; i++);
}

// ========== I2C底层函数（改名避免冲突）==========
void I2C_Soft_Init(void)      // 改名：避免与库函数 I2C_Init 冲突
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(IIC_CLOCK, ENABLE);
    
    // SCL和SDA都设为开漏输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
    GPIO_Init(IIC_SCL_GPIO, &GPIO_InitStructure);
    
    SCL_HIGH();
    SDA_HIGH();
}

void I2C_Soft_Start(void)     // 改名
{
    SDA_HIGH();
    SCL_HIGH();
    delay_us(5);
    SDA_LOW();
    delay_us(5);
    SCL_LOW();
}

void I2C_Soft_Stop(void)      // 改名
{
    SDA_LOW();
    SCL_HIGH();
    delay_us(5);
    SDA_HIGH();
    delay_us(5);
}

void I2C_Soft_SendByte(uint8_t byte)  // 改名
{
    uint8_t j;                // 变量名改为 j，避免与后面的 i 混淆
    for(j = 0; j < 8; j++)
    {
        if(byte & 0x80)
            SDA_HIGH();
        else
            SDA_LOW();
        byte <<= 1;
        delay_us(2);
        SCL_HIGH();
        delay_us(2);
        SCL_LOW();
        delay_us(2);
    }
    // 释放SDA，接收应答
    SDA_HIGH();
    SCL_HIGH();
    delay_us(2);
    SCL_LOW();
}

void OLED_WriteCmd(uint8_t cmd)
{
    I2C_Soft_Start();
    I2C_Soft_SendByte(OLED_ADDR);
    I2C_Soft_SendByte(OLED_CMD_MODE);
    I2C_Soft_SendByte(cmd);
    I2C_Soft_Stop();
}

void OLED_WriteData(uint8_t data)
{
    I2C_Soft_Start();
    I2C_Soft_SendByte(OLED_ADDR);
    I2C_Soft_SendByte(OLED_DATA_MODE);
    I2C_Soft_SendByte(data);
    I2C_Soft_Stop();
}

// ========== OLED屏幕初始化 ==========
void OLED_Init(void)
{
    delay_us(100000);  // 等待OLED上电稳定
    
    OLED_WriteCmd(0xAE);  // 关闭显示
    OLED_WriteCmd(0xD5);  // 设置显示时钟分频
    OLED_WriteCmd(0x80);
    OLED_WriteCmd(0xA8);  // 设置多路复用率
    OLED_WriteCmd(0x3F);
    OLED_WriteCmd(0xD3);  // 设置显示偏移
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(0x40);  // 设置起始行
    OLED_WriteCmd(0x8D);  // 电荷泵设置
    OLED_WriteCmd(0x14);  // 开启电荷泵
    OLED_WriteCmd(0x20);  // 设置内存寻址模式
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(0xA1);  // 段重映射
    OLED_WriteCmd(0xC8);  // 公共输出模式重映射
    OLED_WriteCmd(0xDA);  // 设置公共引脚配置
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0x81);  // 设置对比度
    OLED_WriteCmd(0xCF);
    OLED_WriteCmd(0xD9);  // 设置预充电周期
    OLED_WriteCmd(0xF1);
    OLED_WriteCmd(0xDB);  // 设置VCOMH
    OLED_WriteCmd(0x40);
    OLED_WriteCmd(0xA4);  // 关闭全局显示
    OLED_WriteCmd(0xA6);  // 设置正常显示
    OLED_WriteCmd(0xAF);  // 打开显示
    
    // 清屏
    uint8_t page, col;    // 添加变量声明
    for(page = 0; page < 8; page++)
    {
        OLED_WriteCmd(0xB0 + page);
        OLED_WriteCmd(0x00);
        OLED_WriteCmd(0x10);
        for(col = 0; col < 128; col++)
        {
            OLED_WriteData(0x00);
        }
    }
}

// 简单清屏并显示测试信息
void OLED_DisplayTest(void)
{
    uint8_t page, col;    // 添加变量声明
    
    // 清屏并显示条纹图案
    for(page = 0; page < 8; page++)
    {
        OLED_WriteCmd(0xB0 + page);
        OLED_WriteCmd(0x00);
        OLED_WriteCmd(0x10);
        for(col = 0; col < 128; col++)
        {
            if(page == 2 && col < 64)
                OLED_WriteData(0xFF);  // 第3行前半部分全亮
            else if(page == 4 && col < 64)
                OLED_WriteData(0xAA);  // 第5行前半部分交替点亮
            else
                OLED_WriteData(0x00);
        }
    }
}

int main(void)
{
    SystemInit();
    I2C_Soft_Init();      // 调用改过名的函数
    OLED_Init();
    
    while(1)
    {
        OLED_DisplayTest();
        delay_us(1000000);  // 延时约1秒
    }
}