#include <stdlib.h>
#include <avr/io.h>
#include <avr/delay.h>

#include "fps.h"
#include "lcd.h"
#include "usart.h"

#define F_CPU 16000000UL
#define BAUD 57600
#define UBRR_VALUE ((F_CPU) / (BAUD * 16UL) - 1)

int main()
{
  LCD_Init();

  LCD_Message("USART", "Initializing");
  UsartInit();
  LCD_Message("USART", "Initialized");

  _delay_ms(500);

  LCD_Message("FPS", "Initializing");
  FPSSendCommand(FPS_CMD_VERIFYPASSWORD, NULL, 0);
  LCD_Message("FPS", "Initialized");

  while (1)
  {
  }

  return 0;
}