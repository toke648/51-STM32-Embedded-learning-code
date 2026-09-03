#include "stm32f10x.h"                  // Device header
#include "Delay.h" 

#include "OLED.h"

// OLED_Init(); // 初始化
// OLED_Clear();  // 清屏
// OLED_ShowChar(1, 1, 'A') // 显示一个字符
// OLED_ShowString(1, 3, "HelloWorld"); // 显示一个字符
// OLED_ShowNum(2, 1, 12345, 5); 显示十进制数字
// OLED_ShowSignedNuM(2, 7, -66, 2); // 显示有符号十进制数字
// OLED_ShowHexNum(3, 1, 0xAA55, 4); // 显示十六进制数字
// OLED_ShowBinNum(4, 1, 0xAA55, 16); // 显示二进制数字

int main(void)
{
	
	OLED_Init();
	
	while(1)
	{
		OLED_ShowChar(1, 1, 'A');
		OLED_ShowString(1, 3, "HelloWorld");
		OLED_ShowNum(2, 1, 12345, 5);
		OLED_ShowHexNum(3, 1, 0xAA55, 4);
		OLED_ShowBinNum(4, 1, 0xAA55, 16); 

	}
}
