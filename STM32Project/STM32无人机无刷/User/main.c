/*****************************************************************************
 * 文件：ESC_Control.c
 * 功能：无刷电机电调控制（50Hz PWM，支持双电调）
 * 硬件：STM32F103C8T6
 * 引脚：PA8  → 电调1信号线（TIM1_CH1）
 *       PA10 → 电调2信号线（TIM1_CH3）
 *       PA11 → LED指示（解锁成功亮起）
 *       PA9  → 备用LED或按键（可选）
 *****************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"  // 请确保有准确的毫秒延时函数

// ===================== 电调参数定义 =====================
#define ESC_MIN_PULSE      1000    // 1.00ms 最小油门（停止）
#define ESC_MAX_PULSE      2000    // 2.00ms 最大油门
#define ESC_START_PULSE    1120    // 1.12ms 启动油门（避开死区）
#define ESC_MID_PULSE      1500    // 1.50ms 中速
#define ESC_LOW_PULSE      1100    // 1.10ms 低速
#define ESC_HIGH_PULSE     1800    // 1.80ms 高速

// ===================== 全局变量 =====================
volatile uint8_t esc1_ready = 0;   // 电调1解锁标志
volatile uint8_t esc2_ready = 0;   // 电调2解锁标志

// ===================== 函数声明 =====================
void System_Init(void);
void GPIO_InitConfig(void);
void TIM1_PWM_Init(void);
void Set_ESC1_Pulse(uint16_t pulse_us);
void Set_ESC2_Pulse(uint16_t pulse_us);
void ESC_Calibrate(void);
void Motor_Test(void);
void Delay_Ms(uint32_t ms);

// ===================== 系统初始化 =====================
void System_Init(void) {
    // 设置中断优先级分组（如果需要）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // 初始化系统时钟（默认72MHz，如果使用外部晶振需配置）
    // 这里假设已经通过启动文件或SystemInit配置好了
}

// ===================== GPIO 初始化 =====================
void GPIO_InitConfig(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // -------- PA8, PA10 配置为复用推挽（TIM1 PWM输出） --------
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // -------- PA9, PA11 配置为普通推挽输出（LED指示） --------
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 初始状态：所有LED熄灭
    GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);
}

// ===================== TIM1 PWM 初始化 =====================
void TIM1_PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 使能 TIM1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // -------- 定时器基本配置 --------
    // 时钟 = 72MHz / 7200 = 10KHz，周期 = 200 / 10KHz = 20ms（50Hz）
    TIM_TimeBaseStructure.TIM_Period = 200 - 1;         // ARR = 199
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;     // PSC = 7199
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // -------- PWM 通道1（PA8）配置 --------
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 10;                 // 初始1.0ms（停止信号）
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    // -------- PWM 通道3（PA10）配置 --------
    TIM_OCInitStructure.TIM_Pulse = 10;                 // 初始1.0ms（停止信号）
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    // -------- 使能定时器 --------
    TIM_ARRPreloadConfig(TIM1, ENABLE);                 // 使能自动重装载预装载
    TIM_CtrlPWMOutputs(TIM1, ENABLE);                   // 高级定时器PWM输出使能
    TIM_Cmd(TIM1, ENABLE);                              // 启动定时器
}

// ===================== 设置电调1脉宽（PA8） =====================
void Set_ESC1_Pulse(uint16_t pulse_us) {
    uint16_t compare_value;
    
    // 脉宽限幅保护
    if (pulse_us < ESC_MIN_PULSE) pulse_us = ESC_MIN_PULSE;
    if (pulse_us > ESC_MAX_PULSE) pulse_us = ESC_MAX_PULSE;
    
    // 计算计数值：脉宽(us) / (20000us/200) = 脉宽(us) / 100
    // 使用四舍五入避免精度丢失
    compare_value = (pulse_us + 50) / 100;
    
    // 再次限幅（安全保护）
    if (compare_value < 10) compare_value = 10;
    if (compare_value > 200) compare_value = 200;
    
    TIM_SetCompare1(TIM1, compare_value);
}

// ===================== 设置电调2脉宽（PA10） =====================
void Set_ESC2_Pulse(uint16_t pulse_us) {
    uint16_t compare_value;
    
    // 脉宽限幅保护
    if (pulse_us < ESC_MIN_PULSE) pulse_us = ESC_MIN_PULSE;
    if (pulse_us > ESC_MAX_PULSE) pulse_us = ESC_MAX_PULSE;
    
    // 计算计数值
    compare_value = (pulse_us + 50) / 100;
    
    // 限幅保护
    if (compare_value < 10) compare_value = 10;
    if (compare_value > 200) compare_value = 200;
    
    TIM_SetCompare3(TIM1, compare_value);
}

// ===================== 电调解锁流程 =====================
void ESC_Calibrate(void) {
    // ====== 重要：确保上电时已经输出停止信号 ======
    Set_ESC1_Pulse(ESC_MIN_PULSE);
    Set_ESC2_Pulse(ESC_MIN_PULSE);
    Delay_Ms(200);  // 稳定200ms
    
    // ====== 第一步：发送最大油门（2ms），持续4秒 ======
    // 电调会发出 "哔哔" 两声（检测到最高点）
    Set_ESC1_Pulse(ESC_MAX_PULSE);
    Set_ESC2_Pulse(ESC_MAX_PULSE);
    Delay_Ms(4000);
    
    // ====== 第二步：发送最小油门（1ms），持续4秒 ======
    // 电调会发出 "哔哔哔" 三声（检测到最低点，解锁完成）
    Set_ESC1_Pulse(ESC_MIN_PULSE);
    Set_ESC2_Pulse(ESC_MIN_PULSE);
    Delay_Ms(4000);
    
    // 解锁成功，点亮LED指示
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
    
    // 解锁标志置位
    esc1_ready = 1;
    esc2_ready = 1;
}

// ===================== 电机运转测试 =====================
void Motor_Test(void) {
    // 等待解锁完成
    if (!esc1_ready || !esc2_ready) {
        return;
    }
    
    // ====== 启动电机（逐渐加速） ======
    // 1. 低速启动（1.10ms）
    Set_ESC1_Pulse(ESC_LOW_PULSE);
    Set_ESC2_Pulse(ESC_LOW_PULSE);
    Delay_Ms(2000);
    
    // 2. 中速（1.50ms）
    Set_ESC1_Pulse(ESC_MID_PULSE);
    Set_ESC2_Pulse(ESC_MID_PULSE);
    Delay_Ms(3000);
    
    // 3. 高速（1.80ms）
    Set_ESC1_Pulse(ESC_HIGH_PULSE);
    Set_ESC2_Pulse(ESC_HIGH_PULSE);
    Delay_Ms(3000);
    
    // 4. 回到中速
    Set_ESC1_Pulse(ESC_MID_PULSE);
    Set_ESC2_Pulse(ESC_MID_PULSE);
    Delay_Ms(3000);
    
    // 5. 停止
    Set_ESC1_Pulse(ESC_MIN_PULSE);
    Set_ESC2_Pulse(ESC_MIN_PULSE);
    Delay_Ms(2000);
}

// ===================== 毫秒延时（如果 delay.h 没有提供） =====================
void Delay_Ms(uint32_t ms) {
    // 如果已有 delay.h 中的 Delay_ms，可以替换为此函数
    // 这里用简单循环实现（不精确，仅用于演示）
    // 实际项目请使用定时器或 systick 延时
    while (ms--) {
        volatile uint32_t count = 8000;  // 72MHz 下约 1ms（需根据实际调整）
        while (count--);
    }
}

// ===================== 主函数 =====================
int main(void) {
    // -------- 1. 系统初始化 --------
    System_Init();
    GPIO_InitConfig();
    TIM1_PWM_Init();
    
    // -------- 2. 等待系统稳定 --------
    Delay_Ms(500);
    
    // -------- 3. 输出停止信号（上电前确保安全） --------
    Set_ESC1_Pulse(ESC_MIN_PULSE);
    Set_ESC2_Pulse(ESC_MIN_PULSE);
    Delay_Ms(100);
    
    // =========================================================
    // 重要操作顺序：
    // 1. 烧录此程序到 STM32
    // 2. STM32 上电运行（此时 PA8, PA10 输出 1ms 停止信号）
    // 3. 给电调主电源通电（连接电池）
    // 4. 程序自动执行解锁流程
    // =========================================================
    
    // -------- 4. 电调解锁 --------
    ESC_Calibrate();
    
    // -------- 5. 等待用户准备 --------
    Delay_Ms(1000);
    
    // -------- 6. 主循环：循环运转测试 --------
    while (1) {
        Motor_Test();
        
        // 每次测试后暂停 3 秒
        Delay_Ms(3000);
    }
}

