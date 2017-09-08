/*****************************************************
****  Title: lcd.c                                ****
****  Author: David Schwehr 2015                  ****
****  File:                                       ****
****  Software:                                   ****
****  Target:                                     ****

 DESCRIPTION
 USAGE
******************************************************/
 

/**
* includes
*/
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"

/**
* constants/macros
*/
#define _BV(bit) (1 << (bit))

/**
* function prototypes
*/

/**
* local functions
*/
void setupLCDPorts(){
	DIRECTION_LCD_CONTROL |= ((1 << LCD_RS) | (1 << LCD_ENABLE));          // LCD control lines as output
	DIRECTION_LCD_DATA |= ((1 << D7) | (1 << D6) | (1 << D5) | (1 << D4)); // LCD data lines as output
}  // end setupLCDPorts()

void pulseEnable(){
	set_bit(LCD_CONTROL, LCD_ENABLE);
	_delay_us(40);
	clear_bit(LCD_CONTROL, LCD_ENABLE);
}  // end pulseEnable()

void sendNibble(byte data){
	LCD_DATA &= ~((1 << D7) | (1 << D6) | (1 << D5) | (1 << D4)); // clear only data lines w/o messing with rest of port
	if(data & _BV(4)) set_bit(LCD_DATA, D4);                      // or just use (1 << 4) etc. instead of _BV
	if(data & _BV(5)) set_bit(LCD_DATA, D5);
	if(data & _BV(6)) set_bit(LCD_DATA, D6);
	if(data & _BV(7)) set_bit(LCD_DATA, D7);
	pulseEnable();
}  // end sendNibble()

void sendByte(byte data){
	sendNibble(data);                  // send upper 4-bits, this is assuming LCD is being used in 4-bit mode
	sendNibble(data << 4);             // send lower 4-bits
}  // end sendByte()

void LCD_cmd(byte cmd){
	clear_bit(LCD_CONTROL, LCD_RS);    // R/S line 0 -> command data
	sendByte(cmd);
}  // end LCD_cmd()

void LCD_char(byte ch){
	set_bit(LCD_CONTROL, LCD_RS);     // R/S line 1 -> character data
	sendByte(ch);
}  // end LCD_char()

void initLCD(){
	setupLCDPorts();                  // set proper data directions and pins to outputs
	_delay_ms(15);
	LCD_cmd(0b00110000);              // see data sheet for timings and initialization sequence for 4-bit mode
	_delay_ms(5);                     // optrex data sheet pg. 33
	LCD_cmd(0b00110000);              // hitachi data sheet pg. 46
	_delay_us(150);                   // more info in lcd.h
	LCD_cmd(0b00110000);
	_delay_us(50);
	LCD_cmd(0x33);                    // initialize LCD controller
	LCD_cmd(0x32);                    // set to 4-bit mode
	LCD_cmd(0x28);                    // two line, 5x7 matrix
	LCD_cmd(0x0E);                    // cursor on/off (0x0E cursor on)(0x0C cursor off)
	LCD_cmd(0x06);                    // cursor direction (0x06 cursor right)
	LCD_cmd(0x01);                    // clear display
	_delay_ms(3);                     // wait for LCD to initialize
	
}  // end LCD_init()

void LCD_clear(){
	LCD_cmd(CLEARDISPLAY);
	_delay_ms(3);
}  // end LCD_clear()

void LCD_home(){                      // far left, CR
	LCD_cmd(SETCURSOR);
}  // end LCD_home()

void LCD_goto(byte x, byte y){
	byte line = 0x00;                 // line 0 at address 0x00 see optrex data sheet page 18
	switch(y){
		case 1: line = 0x40; break;
		case 2: line = 0x14; break;   // line 3 of 4 line display
		case 3: line = 0x54; break;   // line 4 of 4 line display
	}
	LCD_cmd(SETCURSOR+line+x);        // update cursor to x,y
}  // end LCD_goto()

void LCD_line(byte row){              // moves cursor to specified line, no CR
	LCD_goto(0, row);                 // use in conjunction with LCD_home to put at beginning of specified line
}  // end LCD_line()

void LCD_string(const char *text){
	while (*text)                     // until /0 character
	LCD_char(*text++);                // increment after to update pointer
}  // end LCD_string()

void LCD_hex(int data){
	char ahex[8] = "";
	itoa(data, ahex, 16);             // convert to ascii hex
	//LCD_string("0x");               // add hex prefix 0x
	LCD_string(ahex);
}  // end LCD_hex()

void LCD_integer(int data){
	char ahex[8] = "";
	itoa(data, ahex, 10);             // convert to ascii
	LCD_string(ahex);
}  // end LCD_integer()


/**
* public functions
*/