#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>

#define LcdDataBus    PORTA
#define LcdControlBus PORTA

#define LcdDataBusDirnReg DDRA

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

void cmdwriteLCD(char cmd);
void datawriteLCD(char dat);
void initLCD(void);
void stringLCD(const unsigned char *str);
void cleardisplay(void);
void line1(void);
void line2(void);

#endif /* LCD_H_ */