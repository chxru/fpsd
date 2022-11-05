#include <avr/io.h>
#include <string.h>
#include "lcd.h"

void cmdwriteLCD(char cmd) {
	LcdDataBus = (cmd & 0xF0); // Set upper 4 bits of the cmd
	LcdControlBus &= ~(1<<LCD_RS); // RS = 0
	LcdControlBus &= ~(1<<LCD_RW); // RW = 0 --> write operation
	LcdControlBus |= (1<<LCD_EN); // enable bit
	
	_delay_ms(10); // 10ms delay
	
	LcdControlBus &= ~(1<<LCD_EN); // finish write operation
	
	_delay_ms(10); // 10ms delay
	
	LcdDataBus = ((cmd<<4) & 0xF0); // Set lower 4 bits of the cmd
	LcdControlBus &= ~(1<<LCD_RS); // RS = 0
	LcdControlBus &= ~(1<<LCD_RW); // RW = 0 --> write operation
	LcdControlBus |= (1<<LCD_EN); // enable bit
	
	_delay_ms(10); // 10ms delay
	
	LcdControlBus &= ~(1<<LCD_EN); // finished write operation
	
	_delay_ms(10); // 10 ms delay
}

/*pass 1 byte of data into the data register*/
void datawriteLCD(char dat) {
	LcdDataBus = (dat & 0xF0); // Set lower 4 bits of the data
	LcdControlBus |= (1<<LCD_RS); // RS = 1
	LcdControlBus &= ~(1<<LCD_RW); // RW = 0 --> write operation
	LcdControlBus |= (1<<LCD_EN); // enable bit
	
	_delay_ms(10); // 10ms delay
	
	LcdControlBus &= ~(1<<LCD_EN); // finished write operation
	
	_delay_ms(10); // 10ms delay
	
	LcdDataBus = ((dat<<4) & 0xF0); //Set lower 4 bits of the data
	LcdControlBus |= (1<<LCD_RS); // RS = 1
	LcdControlBus &= ~(1<<LCD_RW); // RW = 0 --> write operation
	LcdControlBus |= (1<<LCD_EN); // enable bit
	
	_delay_ms(10); // 10ms delay
	
	LcdControlBus &= ~(1<<LCD_EN); // finished write operation
	
	_delay_ms(10); // 10ms delay
}

/*display string in LCD*/
void stringLCD(const unsigned char *str) {
	int length = strlen(str);
	for (int i=0; i<length; i++) {
		datawriteLCD(str[i]);
	}
}

/*clear display*/
void cleardisplay(void) {
	cmdwriteLCD(0x01); // clear display screen
}

/*cursor move to line 1*/
void line1(void) {
	cmdwriteLCD(0x80); // 1st line
}

/*cursor move to line 2*/
void line2(void) {
	cmdwriteLCD(0xC0); // 2nd line
}

/*initialize LCD*/
void initLCD(void) {
	cmdwriteLCD(0x02); // return home
	cmdwriteLCD(0x28); // 2 lines and 5x7 matrix
	cmdwriteLCD(0x0E); // Display on, cursor blinking
	cmdwriteLCD(0x01); // Clear Display
	cmdwriteLCD(0x80); // 1st line
}
