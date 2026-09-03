#ifndef __LED_H
#define __LED_H

// void 指返回值为空
// warning: #1295-D: Deprecated declaration LED0_ON - give arg types
// 默认赋值不能为空，设置void位空值
void LED_Init(unsigned int rcc, uint16_t pin); // 模块外部声明，这个函数是可以被外部调用的函数
void LED0_ON(void); // 每次新建外部函数，都需要重新声明
void LED0_OFF(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED0_Turn(void);
void LED1_Turn(void);

// 注意要留空结尾
# endif
