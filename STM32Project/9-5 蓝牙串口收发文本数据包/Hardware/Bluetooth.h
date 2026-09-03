#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include <stdio.h>

//声明到外部，如果要封装就用setxxx
//extern uint8_t Bluetooth_TxPacket[];
extern char Bluetooth_RxPacket[];
extern uint8_t Bluetooth_RxFlag;

void Bluetooth_Init(void);
void Bluetooth_SendByte(uint8_t Byte);
void Bluetooth_SendArray(uint8_t *Array, uint16_t Length);
void Bluetooth_SendString(char *String);
void Bluetooth_SendNumber(uint32_t Number, uint8_t Length);
void Bluetooth_Printf(char *format, ...);

//void Bluetooth_SendPacket(void);

uint8_t Bluetooth_GetRxFlag(void);
//uint8_t Bluetooth_GetRxData(void);

#endif
