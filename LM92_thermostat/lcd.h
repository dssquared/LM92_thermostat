/***********************************************************
****  Title: C include file for LCD library (lcd.c)     ****
****  Author: David Schwehr 2015                        ****
****  File:                                             ****
****  Software:   AVR-GCC                               ****
****  Target:     AVR-micros with proper pin defines    ****
************************************************************/
/**
@mainpage
@author       David Schwehr  github: dssquared
@file         lcd.h
@code         AVR C GCC
@brief        include file for LCD library for use with 2x16 to 4x20
                 LCDs with 44780 controller
@see          Hitachi, Optrex data sheets
@version      1.0
@copyright  (c)  2015 David Schwehr
 */

#ifndef LCD_H
#define LCD_H

/**
* @name definitions
*/
#ifndef LCD_CONTROL                  // LCD control port of MCU //
#define LCD_CONTROL     PORTB
#endif
#ifndef LCD_DATA                        // LCD data port of MCU //
#define LCD_DATA        PORTD
#endif
// need to find more elegant way of doing DDRx and PORTx
// if not using the same ports, maybe function that takes
// ports as argument?? 
#ifndef DIRECTION_LCD_CONTROL // Data direction LCD control port//
#define DIRECTION_LCD_CONTROL     DDRB
#endif
#ifndef DIRECTION_LCD_DATA      // Data direction LCD data port //
#define DIRECTION_LCD_DATA        DDRD
#endif
#ifndef LCD_RS                       // LCD register select pin //
#define LCD_RS          0
#endif
#ifndef LCD_ENABLE                            // LCD enable pin //
#define LCD_ENABLE      1
#endif
#ifndef D4                                  // LCD data line D4 //
#define D4              4
#endif
#ifndef D5                                  // LCD data line D5 //
#define D5              5
#endif
#ifndef D6                                  // LCD data line D6 //
#define D6              6
#endif
#ifndef D7                                  // LCD data line D7 //
#define D7              7
#endif

#define CLEARDISPLAY   0X01
#define SETCURSOR      0X80

typedef uint8_t byte;      // byte is easier to type and I like it 

// if macros.h is included from any other file this is not needed
// ?? maybe just include macros.h ??
#define BV(bit)               (1 << bit)
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))  // old sbi()
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit)) // old cbi()
#define toggle_bit(sfr, bit)  (_SFR_BYTE(sfr) ^= BV(bit))  


/**
* @name functions
*/

/**
 @brief    initialize ports and appropriate pins for output
              for LCD data and LCD control ports
 @param    none
 @return   none
 @see      
*/
 void setupLCDPorts(void);


/**
 @brief    pulse enable line to "clock" data from LCD data port
             to LCD memory/display
 @param    none
 @return   none
 @see      
*/
void pulseEnable(void);


/**
 @brief    send 4-bits of data, mainly for LCD in 4-bit mode
             would have to add functionality for 8-bit mode
 @param    byte/nibble, 4-bits of data to be sent
 @return   none
 @see      
*/
void sendNibble(byte data);


/**
 @brief    send 8-bits of data to LCD, used for 8-bit commands
		      assuming LCD is being used in 4-bit mode
 @param    byte, 8-bits of data to be sent
 @return   none
 @see      
*/
void sendByte(byte data);


/**
 @brief    sends byte to be written to LCD
			 such as register value or memory address
 @param    byte to be written to LCD
 @return   none
 @see      
*/
void LCD_cmd(byte cmd);

/**
 @brief    display single character on LCD
 @param    byte ascii value
 @return   none
 @see      
*/
void LCD_char(byte ch);

/**
 @brief    initialization of LCD, needs to be called
              before LCD can be used, contains all settings
			  such as matrix size, cursor on/off blink etc.
 @param    none
 @return   none
 @see      hitachi data sheet pg. 46
           optrex data sheet pg. 33
*/
void initLCD(void);

/**
 @brief
 @param    none
 @return   none
 @see    
*/
void LCD_clear(void);

/**
 @brief    clears display and moves cursor position to 0,0
 @param    none
 @return   none
 @see      
*/
void LCD_home(void);

/**
 @brief    places cursor at home position without clearing the display
             just as a carriage return, CR
 @param    x,y location as bytes of desired cursor position
 @return   none
 @see      optrex data sheet page 18
*/
void LCD_goto(byte x, byte y);

/**
 @brief   moves cursor to specified line, no CR
           use in conjunction with LCD_home()   ***  need to check this ***
		   to put cursor at beginning of specified line
 @param    byte, number of row 0-3
 @return   none
 @see      
*/
void LCD_line(byte row);

/**
 @brief    display string of text on LCD at current cursor position
 @param    pointer to char array
 @return   none
 @see      
*/
void LCD_string(const char *text);

/**
 @brief    display hex value of data at current cursor position
 @param    int data to be displayed as hex
 @return   none
 @see      
*/
void LCD_hex(int data);

/**
 @brief    display integer value of data at current cursor position
 @param    int data to be displayed as integer
 @return   none
 @see      
*/
void LCD_integer(int data);


#endif  // LCD_H