#include <avr/io.h>
#include <string.h>

#include "lcd.h"

// Send command to LCD display
void LCD_CmdWrite(char cmd)
{
	LcdDataBus = (cmd & 0xF0);			 // Set upper 4 bits of the cmd
	LcdControlBus &= ~(1 << LCD_RS); // RS = 0
	LcdControlBus &= ~(1 << LCD_RW); // RW = 0 --> write operation
	LcdControlBus |= (1 << LCD_EN);	 // enable bit

	_delay_ms(10); // 10ms delay

	LcdControlBus &= ~(1 << LCD_EN); // finish write operation

	_delay_ms(10); // 10ms delay

	LcdDataBus = ((cmd << 4) & 0xF0); // Set lower 4 bits of the cmd
	LcdControlBus &= ~(1 << LCD_RS);	// RS = 0
	LcdControlBus &= ~(1 << LCD_RW);	// RW = 0 --> write operation
	LcdControlBus |= (1 << LCD_EN);		// enable bit

	_delay_ms(10); // 10ms delay

	LcdControlBus &= ~(1 << LCD_EN); // finished write operation

	_delay_ms(10); // 10 ms delay
}

// Pass 1 byte of data into the LCD data register
void LCD_DataWrite(char dat)
{
	LcdDataBus = (dat & 0xF0);			 // Set lower 4 bits of the data
	LcdControlBus |= (1 << LCD_RS);	 // RS = 1
	LcdControlBus &= ~(1 << LCD_RW); // RW = 0 --> write operation
	LcdControlBus |= (1 << LCD_EN);	 // enable bit

	_delay_ms(10); // 10ms delay

	LcdControlBus &= ~(1 << LCD_EN); // finished write operation

	_delay_ms(10); // 10ms delay

	LcdDataBus = ((dat << 4) & 0xF0); // Set lower 4 bits of the data
	LcdControlBus |= (1 << LCD_RS);		// RS = 1
	LcdControlBus &= ~(1 << LCD_RW);	// RW = 0 --> write operation
	LcdControlBus |= (1 << LCD_EN);		// enable bit

	_delay_ms(10); // 10ms delay

	LcdControlBus &= ~(1 << LCD_EN); // finished write operation

	_delay_ms(10); // 10ms delay
}

// Display string in LCD
void LCD_String(char *str)
{
	int length = strlen(str);
	for (int i = 0; i < length; i++)
	{
		LCD_DataWrite(str[i]);
	}
}

// Display two line message
void LCD_Message(char *title, char *msg)
{
	LCD_CmdWrite(LCD_CMD_CLEAR);
	LCD_CmdWrite(LCD_CMD_LINE1);
	LCD_String(title);
	LCD_CmdWrite(LCD_CMD_LINE2);
	LCD_String(msg);
}

/*initialize LCD*/
void LCD_Init(void)
{
	LcdDataBusDirnReg = 0xFF;					// Set all pins of PORTA as output
	LCD_CmdWrite(LCD_CMD_HOME);				// return home
	LCD_CmdWrite(LCD_CMD_4BIT_2LINE); // 2 lines and 5x7 matrix
	LCD_CmdWrite(LCD_CMD_CURSOR_ON);	// Display on, cursor blinking
	LCD_CmdWrite(LCD_CMD_CLEAR);			// Clear Display
	LCD_CmdWrite(LCD_CMD_LINE1);			// 1st line
}
