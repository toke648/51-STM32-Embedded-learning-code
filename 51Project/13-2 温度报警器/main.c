#include <REGX52.H>
#include "LCD1602.h"
#include "DS18820.h"
#include "Delay.h"
#include "AT24C02.h"
#include "Key.h"
#include "Timer0.h"
#include "Buzzer.h"

float Temp;          // 原始温度（用于比较）
float TShow;         // 显示用温度（取绝对值）
unsigned char TLow = 29;
unsigned char THigh = 30;
unsigned char KeyNum;

void main(){
    Timer0Init();
	Buzzer_Init();
    
    // 读取EEPROM存储的阈值
    THigh = AT24C02_ReadByte(0);
    Delay(5);
    TLow = AT24C02_ReadByte(1);
    
    LCD_Init();
    LCD_ShowString(1, 1, "T:");
    LCD_ShowString(2, 1, "TH:");
    LCD_ShowString(2, 9, "TL:");
    
    while(1){
        /* 温度读取显示 */
        DS18B20_ConvertT();
        Delay(750);                     // 等待转换完成
        Temp = DS18B20_ReadT();
        
        // 显示温度（取绝对值）
        TShow = Temp;
        if(TShow < 0){
            LCD_ShowChar(1, 3, '-');    // ✅ 用 ShowChar
            TShow = -TShow;
        } else {
            LCD_ShowChar(1, 3, '+');
        }
        LCD_ShowNum(1, 4, (int)TShow, 2);
        LCD_ShowChar(1, 6, '.');
        LCD_ShowNum(1, 7, (unsigned int)(TShow * 100) % 100, 2);
        LCD_ShowChar(1, 9, 'C');
        
        /* 获取按键值 */
        KeyNum = Key();                  // ✅ 直接获取，Key_Loop 在中断中运行
        
        /* 阈值调整 */
        if(KeyNum == 1){
            THigh++;
            if(THigh > 125) THigh = 125;
            if(THigh <= TLow) THigh = TLow + 1;
            // 保存到EEPROM
            AT24C02_WriteByte(0, THigh);
            Delay(5);
        }
        if(KeyNum == 2){
            if(THigh > TLow + 1) THigh--;
            AT24C02_WriteByte(0, THigh);
            Delay(5);
        }
        if(KeyNum == 3){
            if(TLow < THigh - 1) TLow++;
            AT24C02_WriteByte(1, TLow);
            Delay(5);
        }
        if(KeyNum == 4){
            if(TLow > 0) TLow--;
            AT24C02_WriteByte(1, TLow);
            Delay(5);
        }
        
        /* 显示阈值 */
        LCD_ShowNum(2, 4, THigh, 2);
        LCD_ShowNum(2, 12, TLow, 2);
        
        /* 温度报警判断（使用原始 Temp）*/
        if(Temp > THigh){
            LCD_ShowString(1, 11, "OV:H");
            Buzzer_Start(500);
        } else if(Temp < TLow){
            LCD_ShowString(1, 11, "OV:L");
            Buzzer_Start(500);
        } else {
            LCD_ShowString(1, 11, "    ");
        }
        
        Delay(200);
    }
}

// 定时器中断（每1ms）
void Timer0_Routine() interrupt 1{
    static unsigned int T0Count = 0;  // ✅ 初始化
    TL0 = 0x18;
    TH0 = 0xFC;
    
    T0Count++;
	
	Buzzer_Loop();
	
    if(T0Count >= 20){  // 每20ms执行一次
        T0Count = 0;
        Key_Loop();     // 更新按键状态
    }
}