#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "AD.h"
#include "PS2.h"

int main(void)
{
    uint16_t x, y;
    uint8_t z;
    
    OLED_Init();
    JoyStick_Init();
    
    OLED_ShowString(1, 1, "X:    Y:    Z:");
    
    while (1)
    {
        x = JoyStick_ReadX();
        y = JoyStick_ReadY();
        z = JoyStick_ReadZ();
        
        OLED_ShowNum(2, 1, x, 4);
        OLED_ShowNum(2, 8, y, 4);
        OLED_ShowNum(2, 15, z, 1);
        
        Delay_ms(50);
    }
}