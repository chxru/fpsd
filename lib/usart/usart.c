#include "usart.h"

// initialize usart
void UsartInit()
{

  UBRR1H = (BAUDRATE >> 8);               // set the upper UBRR value
  UBRR1L = BAUDRATE;                      // set the lower UBRR value
  UCSR1B = (1 << RXEN1) | (1 << TXEN1);   // enable receiver and transmitter
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); // set 8-bit data
}

// usart send a character
void UsartSendChar(uint8_t data)
{
  while (!(UCSR1A & (1 << UDRE1)))
    ;          // wait for empty transmit buffer
  UDR1 = data; // put data into buffer, sends the data
}

// usart send a string
void UsartSendString(char *str)
{
  while (*str != 0x00)
  {
    UsartSendChar(*str);
    str++;
  }
}

// usart receive a character
uint8_t UsartReceiveChar()
{
  while (!(UCSR1A & (1 << RXC1)))
    ;          // wait for data to be received
  return UDR1; // get and return received data from buffer
}

// usart receive a string
void UsartReceiveString(char *str)
{
  uint8_t c;
  uint8_t i = 0;
  while ((c = UsartReceiveChar()) != '\r')
  {
    str[i] = c;
    i++;
  }
  str[i] = '\0';
}
