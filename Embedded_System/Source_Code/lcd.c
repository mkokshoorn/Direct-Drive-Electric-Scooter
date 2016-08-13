/**
 * Source file for driving a HD44780 controlled LCD.
 * @date 25 July 2012
 * @author Campbell Sinclair
 * cls76@uclive.ac.nz
 */

#include "lcd.h"
#include "scooter_gpio.h"

/* --- Private Constants --- */
#define NUM_CHAR_WIDE_DISPLAY 8
#define LOW 0
#define HIGH 1
#define OFF 0
#define ON 1
#define INPUT 1
#define OUTPUT 0
#define COMMAND 0
#define DATA 1
#define READY 0
#define BUSY 1

/* --- Macro Functions --- */
#define SET(PORT, BITNUM)  gpio_on(BITNUM,PORT)
#define CLEAR(PORT, BITNUM) gpio_off(BITNUM,PORT)
#define SET_INPUT(PORT, BITNUM) gpio_set_input(BITNUM,PORT)
#define SET_OUTPUT(PORT,BITNUM) gpio_set_output(BITNUM,PORT)

/* LCD pin: set high, set low, set input and set output "functions" */
#define RS_SET      SET(RS_PORT, RS_BIT)
#define RS_CLEAR    CLEAR(RS_PORT, RS_BIT)
#define RS_INPUT    SET_INPUT(RS_PORT, RS_BIT);
#define RS_OUTPUT   SET_OUTPUT(RS_PORT, RS_BIT);

#define RW_SET      SET(RW_PORT, RW_BIT)
#define RW_CLEAR    CLEAR(RW_PORT, RW_BIT)
#define RW_INPUT    SET_INPUT(RW_PORT, RW_BIT);
#define RW_OUTPUT   SET_OUTPUT(RW_PORT, RW_BIT);

#define EN_SET      SET(EN_PORT, EN_BIT)
#define EN_CLEAR    CLEAR(EN_PORT, EN_BIT)
#define EN_INPUT    SET_INPUT(EN_PORT, EN_BIT);
#define EN_OUTPUT   SET_OUTPUT(EN_PORT, EN_BIT);

#define D4_SET      SET(D4_PORT, D4_BIT)
#define D4_CLEAR    CLEAR(D4_PORT, D4_BIT)
#define D4_INPUT    SET_INPUT(D4_PORT, D4_BIT);
#define D4_OUTPUT   SET_OUTPUT(D4_PORT, D4_BIT);

#define D5_SET      SET(D5_PORT, D5_BIT)
#define D5_CLEAR    CLEAR(D5_PORT, D5_BIT)
#define D5_INPUT    SET_INPUT(D5_PORT, D5_BIT);
#define D5_OUTPUT   SET_OUTPUT(D5_PORT, D5_BIT);

#define D6_SET      SET(D6_PORT, D6_BIT)
#define D6_CLEAR    CLEAR(D6_PORT, D6_BIT)
#define D6_INPUT    SET_INPUT(D6_PORT, D6_BIT);
#define D6_OUTPUT   SET_OUTPUT(D6_PORT, D6_BIT);

#define D7_SET      SET(D7_PORT, D7_BIT)
#define D7_CLEAR    CLEAR(D7_PORT, D7_BIT)
#define D7_INPUT    SET_INPUT(D7_PORT, D7_BIT);
#define D7_OUTPUT   SET_OUTPUT(D7_PORT, D7_BIT);

/* --- Private Data Types --- */
typedef char bit;
typedef char byte;

/* --- Private Function Prototypes --- */
void hexWriteLCD(bit rs, byte data);
void binWriteLCD(bit rs, bit d7, bit d6, bit d5, bit d4, bit d3, bit d2, bit d1, bit d0);
void displayClear(void);
void returnHome(void);
void entryModeSet(bit cursorMoveDirection, bit displayShift);
void displayOnOff(bit display, bit cursor, bit blinking);
void shift(bit anchor, bit direction);
void setFunction(bit interface, bit lines, bit dots);
void waitWhileBusy(void);
void moveCursorToLine(unsigned char lineNumber);
void incrementCursor(unsigned char numOfCharacters);

/**
 * Write to the LCD, specifying data as a hexadecimal number.
 * @param rs Register select, as COMMAND or DATA.
 * @param data Data bits 7-0, in hexadecimal form.
 */
void hexWriteLCD(bit rs, byte data) {
    /* Make Sure "EN" is 0 */
    EN_CLEAR;
    
    /* Set "R/S" to 0 for a command, or 1 for data/characters */
    if (rs == COMMAND) { RS_CLEAR; }
    else { RS_SET; }
    
    /* Put the data/command on D7-0 */
    if ((data& (1<<7)) == 0) { D7_CLEAR; }
    else { D7_SET; }
    
    if ((data& (1<<6)) == 0) { D6_CLEAR; }
    else { D6_SET; }
    
    if ((data& (1<<5)) == 0) { D5_CLEAR; }
    else { D5_SET; }
    
    if ((data& (1<<4)) == 0) { D4_CLEAR; }
    else { D4_SET; }
    
    /* Set "EN" */
    EN_SET;
    
    /* Clear "EN" */
    EN_CLEAR;
    
    /* Wait for LCD to finish processing the write */
    waitWhileBusy();


    //Send Lower 4 Bits


    /* Make Sure "EN" is 0 */
    EN_CLEAR;

    /* Set "R/S" to 0 for a command, or 1 for data/characters */
    if (rs == COMMAND) { RS_CLEAR; }
    else { RS_SET; }

    /* Put the data/command on D7-0 */
    if ((data& (1<<3)) == 0) { D7_CLEAR; }
    else { D7_SET; }

    if ((data& (1<<2)) == 0) { D6_CLEAR; }
    else { D6_SET; }

    if ((data& (1<<1)) == 0) { D5_CLEAR; }
    else { D5_SET; }

    if ((data& (1)) == 0) { D4_CLEAR; }
    else { D4_SET; }

    /* Set "EN" */
    EN_SET;

    /* Clear "EN" */
    EN_CLEAR;

    /* Wait for LCD to finish processing the write */
    waitWhileBusy();
}

/**
 * Write to the LCD, specifying each bit individually.
 * @param rs Register select, as COMMAND or DATA.
 * @param d7 Data bit 7, as HIGH or LOW.
 * @param d6 Data bit 6, as HIGH or LOW.
 * @param d5 Data bit 5, as HIGH or LOW.
 * @param d4 Data bit 4, as HIGH or LOW.
 * @param d3 Data bit 3, as HIGH or LOW.
 * @param d2 Data bit 2, as HIGH or LOW.
 * @param d1 Data bit 1, as HIGH or LOW.
 * @param d0 Data bit 0, as HIGH or LOW.
 */
void binWriteLCD(bit rs, bit d7, bit d6, bit d5, bit d4, bit d3, bit d2, bit d1, bit d0) {
    /* Make Sure "EN" is 0 */
    EN_CLEAR;
    
    /* Set "R/S" to 0 for a command, or 1 for data/characters */
    if (rs == COMMAND) { RS_CLEAR; }
    else { RS_SET; }
    
    /* Put the data/command on D7-0 */
    if (d7 == LOW) { D7_CLEAR; }
    else { D7_SET; }
    
    if (d6 == LOW) { D6_CLEAR; }
    else { D6_SET; }
    
    if (d5 == LOW) { D5_CLEAR; }
    else { D5_SET; }
    
    if (d4 == LOW) { D4_CLEAR; }
    else { D4_SET; }
    
    
    /* Set "EN" */
    EN_SET;
    
    /* Clear "EN" */
    EN_CLEAR;
    
    /* Wait for LCD to finish processing the write */
    waitWhileBusy();
}

/* --- LCD Commands --- */

/**
 * Clear the display.
 */
void displayClear(void) {
    binWriteLCD(COMMAND, 0, 0, 0, 0, 0, 0, 0, 1);
}

/**
 * Move cursor to first digit.
 */
void returnHome(void) {
    binWriteLCD(COMMAND, 0, 0, 0, 0, 0, 0, 1, 0);
}

/**
 * Select whether the cursor moves left or right when characters are added.
 * displayShift is not understood, but it does make the display shift when characters are added.
 * @param cursorMoveDirection Set cursor move direction (H = increase; L = decrease).
 * @param displayShift Specifies the shift of display (H = display is shifted; L = display is not shifted).
 */
void entryModeSet(bit cursorMoveDirection, bit displayShift) {
    binWriteLCD(COMMAND, 0, 0, 0, 0, 0, 1, cursorMoveDirection, displayShift);
}

/**
 * Turn the display, cursor and blinking on or off.
 * @param display Display on or off (H = display on, L = display off).
 * @param cursor Cursor on or off (H = cursor on, L = cursor off).
 * @param blinking Cursor blinking on or off (H = blinking on, L = blinking off).
 */
void displayOnOff(bit display, bit cursor, bit blinking) {
    binWriteLCD(COMMAND, 0, 0, 0, 0, 1, display, cursor, blinking);
}

/**
 * Either shift all of the text on the display, left or right, by one character;
 * or move the position of the cursor, left or right, by one character.
 * @param anchor Sets whether the display or cursor shifts (H = display shift, L = cursor move).
 * @param direction Direction of shift (H = right, L = left).
 */
void shift(bit anchor, bit direction) {
    binWriteLCD(COMMAND, 0, 0, 0, 1, anchor, direction, 0, 0);
}

/**
 * Set the configuration of the LCD.
 * @param interface Number of data bits used for interface (H = 8-bit, L = 4-bit).
 * @param lines Number of lines on display (H = 2 lines, L = 1 line).
 * @param dots Height of characters on display (H = 5x10 dots, L = 5x7 dots).
 */
void setFunction(bit interface, bit lines, bit dots) {
    binWriteLCD(COMMAND, 0, 0, 1, interface, lines, dots, 0, 0);
}

/**
 * Wait until the LCD is no longer busy.
 */
void waitWhileBusy() {
    // "D7" as input
    D7_INPUT;
    // command
    RS_CLEAR;
    // reading
    RW_SET;
    // set enable for reading busy flag (is like AND gate - do not have to constantly poll enable H->L)
    EN_SET;
    // do nothing while busy
    while(gpio_read(D7_BIT,D7_PORT)) {}
    // return enable to 0
    EN_CLEAR;
    // return "D7" as an output
    D7_OUTPUT;
    // back to writing
    RW_CLEAR;
}

/**
 * Initialise the LCD.
 */
void init_LCD() {
    /* Set pins used by LCD as outputs */
    char test=0;

    RS_OUTPUT;
    RW_OUTPUT;
    EN_OUTPUT;

    D4_OUTPUT;
    D5_OUTPUT;
    D6_OUTPUT;
    D7_OUTPUT;

    
    /* Set "R/W" low for writing to the LCD */
    RW_CLEAR;
    
    /* Wait for LCD to power up */
    waitWhileBusy();
    
    /* ---- START OF DATA SHEET INITIALISATION COMMANDS ----*/
        /* Write command D7-0 = 0x00110000, 3 times (as per data sheet) */
        binWriteLCD(COMMAND, 0, 0, 1, 1, 0, 0, 0, 0);
      
        binWriteLCD(COMMAND, 0, 0, 1, 1, 0, 0, 0, 0);
        binWriteLCD(COMMAND, 0, 0, 1, 1, 0, 0, 0, 0);
    
        /* 4-bit interface, 2 lines (used for 4 line displays), 5x7 pixel characters */
        /* This cannot be changed afterwards */
        binWriteLCD(COMMAND, 0, 0, 1, 0,       0, 0, 0, 0); //  0 , 0 , 4 Bit, 0
        binWriteLCD(COMMAND, 0, 0, 1, 0,       0, 0, 0, 0); //  0 , 0 , 4 Bit, 0
        
        binWriteLCD(COMMAND, 1, 0, 0, 0,       0, 0, 0, 0); //  N , F , X , X
        
        binWriteLCD(COMMAND, 0, 0, 0, 0,       0, 0, 0, 0); //
        
        binWriteLCD(COMMAND, 0, 0, 0, 1,       0, 0, 0, 0); //
       
        binWriteLCD(COMMAND, 0, 0, 0, 0,       0, 0, 0, 0); //
        binWriteLCD(COMMAND, 0, 1, 1, 0,       0, 0, 0, 0); // 0, 1 , I/D, S  (Increment and Shift)

        //Turn Display On ./off
        binWriteLCD(COMMAND, 0, 0, 0, 0,       0, 0, 0, 0); // 
        binWriteLCD(COMMAND, 1, 0, 0, 0,       0, 0, 0, 0); // 1, D , C, B  (Display Cursor and Increment)

        //setFunction(LOW, HIGH, LOW);

        hexWriteLCD(DATA,"Hello");

        while(1);
        /* Display off */
        displayOnOff(LOW, LOW, LOW);
    
        /* Display clear */
        displayClear();
    
        /* Set entry mode to move cursor right and do not shift display */
        entryModeSet(HIGH, LOW);
    /* ---- END OF DATA SHEET INITIALISATION COMMANDS ----*/
    
    /* Display ON, cursor OFF, blinking OFF */
    displayOnOff(HIGH, LOW, LOW);
    
    /* Display is ready to accept data */
}

/**
 * Write a character to the display.
 * @param character The character you want to display, in ASCII form.
 */
void drawChar_LCD(unsigned char character) {
    hexWriteLCD(DATA, character);
}

/**
 * Write a string to the display.
 * Note that if string is longer than 20 characters, the excess will not be displayed.
 * @param string The string you want to display, in ASCII form.
 */
void drawString_LCD(unsigned char *string) {
    unsigned char *stringStart = string;
    
    while ((*string != 0) && (string < (stringStart + 20))) {
        hexWriteLCD(DATA, *string);
        string++;
    }
}

/**
 * Move cursor to start of a line.
 * @param lineNumber The line number to move the cursor to (1, 2, 3 or 4).
 */
void moveCursorToLine(unsigned char lineNumber) {
    /* Return cursor to home */
    returnHome();
    
    switch (lineNumber) {
        case 1:
            break;
        case 2:
            incrementCursor(2 * NUM_CHAR_WIDE_DISPLAY);
            break;
        case 3:
            incrementCursor(NUM_CHAR_WIDE_DISPLAY);
            break;
        case 4:
            incrementCursor(3 * NUM_CHAR_WIDE_DISPLAY);
            break;
    }
}

/**
 * Increment cursor by specified number of characters.
 * Note that this is more efficient than multiple calls of the shift() function
 * @param numOfCharacters The number of characters to shift by.
 */
void incrementCursor(unsigned char numOfCharacters) {
    int i=0;
    /* Make Sure "EN" is 0 */
    EN_CLEAR;
    
    /* Set "R/S" to 0 for command */
    RS_CLEAR;
    
    /* Put the command 0b00010100 on "D7-0" */
    D7_CLEAR;
    D6_CLEAR;
    D5_CLEAR;
    D4_SET;

    for ( i = 0; i < numOfCharacters; i++) {
        /* Clear "D7" since used in waitWhileBusy() */
        D7_CLEAR;
        
        /* Set "EN" */
        EN_SET;
        
        /* Clear "EN" */
        EN_CLEAR;
        
        /* Wait for LCD to finish processing the write */
        waitWhileBusy();
    }
}

/**
 * Draws a string to a specific line.
 * Note that if string is longer than 20 characters, the excess will not be displayed.
 * @param lineNumber The line number to draw to.
 * @param string The string to draw.
 */
void drawLine_LCD(unsigned char *string, unsigned char lineNumber) {
    moveCursorToLine(lineNumber);
    drawString_LCD(string);
}
