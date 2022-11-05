#ifndef USART_H
#define USART_H

#include <avr/io.h>

#ifndef BAUD
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)
#endif

void UsartInit();
void UsartSendChar(uint8_t data);
void UsartSendString(char *str);
uint8_t UsartReceiveChar();
void UsartReceiveString(char *str);

#endif
