#ifndef USART_H
#define USART_H

#include <avr/io.h>

void UsartInit();
void UsartSendChar(uint8_t data);
void UsartSendString(char *str);
uint8_t UsartReceiveChar();
void UsartReceiveString(char *str);

#endif
