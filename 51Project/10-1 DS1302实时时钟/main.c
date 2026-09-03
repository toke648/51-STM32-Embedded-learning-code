#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "DS1302.h"

void main(){
    LCD_Init();
    DS1302_Init();
    
    LCD_ShowString(1, 1, "Date:");
    LCD_ShowString(2, 1, "Time:");
    
    // 首次使用设置时间（设置一次后可以注释掉）
    DS1302_SetTime();
    
    while(1){	
        // 读取时间
        DS1302_ReadTime();
        
        // 显示日期：年-月-日
        LCD_ShowNum(1, 6, DS1302_Time[0], 2);  // 年
        LCD_ShowChar(1, 8, '-');
        LCD_ShowNum(1, 9, DS1302_Time[1], 2);  // 月
        LCD_ShowChar(1, 11, '-');
        LCD_ShowNum(1, 12, DS1302_Time[2], 2); // 日
        
        // 显示时间：时:分:秒
        LCD_ShowNum(2, 6, DS1302_Time[3], 2);  // 时
        LCD_ShowChar(2, 8, ':');
        LCD_ShowNum(2, 9, DS1302_Time[4], 2);  // 分
        LCD_ShowChar(2, 11, ':');
        LCD_ShowNum(2, 12, DS1302_Time[5], 2); // 秒
        
        Delay(100);
    }
}