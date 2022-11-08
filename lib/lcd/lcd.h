#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>

#define LcdDataBus PORTA
#define LcdControlBus PORTA

#define LcdDataBusDirnReg DDRA

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_HOME 0x02
#define LCD_CMD_ENTRY_MODE 0x06
#define LCD_CMD_DISPLAY_ON 0x0C
#define LCD_CMD_DISPLAY_OFF 0x08
#define LCD_CMD_CURSOR_ON 0x0E
#define LCD_CMD_4BIT_2LINE 0x28
#define LCD_CMD_LINE1 0x80
#define LCD_CMD_LINE2 0xC0

void LCD_CmdWrite(char cmd);
void LCD_DataWrite(char dat);
void LCD_Init(void);
void LCD_Message(char *title, char *msg);
void LCD_String(char *str);

#endif /* LCD_H_ */