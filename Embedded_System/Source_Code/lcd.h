/**
 * Header file for driving a HD44780 controlled LCD.
 * @date 25 July 2012
 * @author Campbell Sinclair
 * cls76@uclive.ac.nz
 */

#include "scooter_gpio.h"

#ifndef LCD_H
#define LCD_H

/* --- Public Constants --- */
/* LCD to Atmega8 pin connections */
#define RS_PORT     GPIO_PORTC
#define RS_BIT      BIT6

#define RW_PORT     GPIO_PORTC
#define RW_BIT      BIT7

#define EN_PORT     GPIO_PORTA
#define EN_BIT      BIT7

#define D4_PORT     GPIO_PORTA 
#define D4_BIT      BIT6    

#define D5_PORT     GPIO_PORTC
#define D5_BIT      BIT1

#define D6_PORT     GPIO_PORTC
#define D6_BIT      BIT0

#define D7_PORT     GPIO_PORTC
#define D7_BIT      BIT2

/* --- Public Functions --- */
/**
 * Initialise the LCD.
 */
void init_LCD(void);

/**
 * Draw a character to the display.
 * @param character The character you want to display, in ASCII form.
 */
void drawChar_LCD(unsigned char character);

/**
 * Draw a string to the display.
 * Note that if string is longer than 20 characters, the excess will not be displayed.
 * @param string The string you want to display, in ASCII form.
 */
void drawString_LCD(unsigned char *string);

/**
 * Draws a string to a specific line.
 * Note that if string is longer than 20 characters, the excess will not be displayed.
 * @param lineNumber The line number to draw to.
 * @param string The string to draw.
 */
void drawLine_LCD(unsigned char *string, unsigned char lineNumber);

#endif
