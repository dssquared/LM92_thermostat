/*
 * LM92_thermostat.c
 *
 * Created: 9/7/2017 11:34:44 PM
 * Author : David Schwehr
 * GitHub: dssquared
 *
 * LM92_thermostat - written for atmega328p, can substitute pinDefines.h for different microcontrollers
 *                   Utilizes LM92 temperature sensor via I2C, LCD with 44780 controller, and Power Switch Tail connected to heater
 *                   Need i2c.h, lm92.h, and lcd.h along with pinDifines.h
 *                   Monitor temperature and activates Power Switch Tail via NPN transistor. If temp is below
 *                   lower threshold turn heater on, above upper threshold turn heater off. 
 */ 


#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "lcd.h"
#include "i2c.h"
#include "lm92.h"

//  ----- defines/macros  -----  //
typedef uint8_t byte;
#define DEG             0XDF       // LCD char for degrees
#define PST_PORT        PORTB
#define PST_DDR         DDRB
#define PSTENABLEPIN    2
#define UPPERTHRESHOLD  86.0
#define LOWERTHRESHOLD  84.0


//  ----- function prototypes  -----  //
double getCtemp(void);
double getFtemp(void);
void displayTemp(byte temp, char unit);
void displayHeaterState(void);
void turnHeaterOn(void);
void turnHeaterOff(void);


//  -----  global variables  -----  //
uint8_t heaterOn = 0;    // flag for heater state

int main(void)
{
    byte integerTemp;      // integer to be passed to displayTemp()
	double calculatedTemp; // double to store return of getFTemp()
	
	//  ----- inits  -----  //
	initI2C();
	initLCD();
	LCD_string("LM92 Thermostat...");
	PST_DDR |= (1 << PSTENABLEPIN);     // set enable pin to output
	_delay_ms(3000);
	
    while (1) 
    {
		calculatedTemp = getFtemp();
		integerTemp = (byte)calculatedTemp;
		displayTemp(integerTemp, 'F');
		//displayHeaterState();
		if (calculatedTemp > UPPERTHRESHOLD && heaterOn)
		{
			turnHeaterOff();
		}
		if (calculatedTemp < LOWERTHRESHOLD && !heaterOn)
		{
			turnHeaterOn();
		}
		LCD_goto(0,1);
		displayHeaterState();  // be aware displayHeaterState() will not clear LCD first, displayTemp() clears LCD first.
		_delay_ms(260);
		_delay_ms(260);
		_delay_ms(262);
		_delay_ms(262);
		
    }  // end while(1)
}  // end main()


double getCtemp(){
	byte tempHighByte, tempLowByte;
	uint16_t tempWord;
	double cTemp;
	
	// read temperature register from LM92
	i2cStart();
	i2cSend(LM92_ADDRESS_W);
	i2cSend(LM92_TEMP_REGISTER);
	i2cStart();
	i2cSend(LM92_ADDRESS_R);
	tempHighByte = i2cReadAck();
	tempLowByte = i2cReadNoAck();
	i2cStop();
	
	// assemble 2 8-bit readings to 16-bit word
	tempWord = 0x00;
	tempWord = (uint16_t)tempHighByte << 8;
	tempWord |= tempLowByte;
	
	cTemp = (tempWord >> 3) * 0.0625;
	
	return cTemp;
}  // end getCtemp()

double getFtemp(){
	double fTemp;
	
	fTemp = (getCtemp() * 1.80) + 32.00;  // could do all of this in one line with return
	
	return fTemp;
}  // end getFtemp()

void displayTemp(byte temp, char unit){
	LCD_clear();
	LCD_string("Temp: ");
	LCD_integer(temp);
	LCD_char(DEG);
	LCD_char(unit);
}  // end displayTemp()

// displayHeaterState() will display at current cursor position, or clear LCD to start at top left of LCD
void displayHeaterState(){
	
	if (heaterOn)
	{
		//LCD_clear();
		LCD_string("Heater is ON");
	} else{
		//LCD_clear();
		LCD_string("Heater is OFF");
	}
	
}  // end displayHeaterState()

void turnHeaterOn(){
	PST_PORT |= (1 << PSTENABLEPIN);
	heaterOn = 1;
}  // end turnHeaterOn()

void turnHeaterOff(){
	PST_PORT &= ~(1 << PSTENABLEPIN);
	heaterOn = 0;
}  // end turnHeaterOff()

