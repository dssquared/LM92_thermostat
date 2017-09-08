/*******************************************
****    header file/AVR library         ****
****  for use with LM92 temp sensor     ****
****    by: David Schwehr 2015          ****
*******************************************/
// test
#include <avr/io.h>

#ifndef LM92_H
#define LM92_H

#define LM92_ADDRESS_W          0b10010000
#define LM92_ADDRESS_R          0b10010001
#define LM92_TEMP_REGISTER      0b00000000
#define LM92_CONFIG_REGISTER    0b00000001
#define LM92_THYST_REGISTER     0b00000010
#define LM92_TCRIT_REGISTER     0b00000011
#define LM92_TLOW_REGISTER      0b00000100
#define LM92_THIGH_REGISTER     0b00000101
#define LM92_MFG_ID             0b00000111


#endif