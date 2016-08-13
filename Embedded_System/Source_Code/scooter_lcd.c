/******************************************************************************/
/*
 * File:   scooter_gpio.c
 * Author: Matt Kokshoorn
 *
 * Created on July 22, 2013, 3:45 PM
 *
 * Notes:
 *
 *
/******************************************************************************/


#include <p18cxxx.h>
#include "scooter_gpio.h"
#include "scooter_lcd.h"
#include "scooter_timer.h"
#include <delays.h>

#define D7_INDENTIFER 0b1000
#define D6_INDENTIFER 0b0100
#define D5_INDENTIFER 0b0010
#define D4_INDENTIFER 0b0001

#define EN_PULSE_DELAY 2
#define WAKE_UP_DELAY 10
#define BIT4_MODE_DELAY 10

#define LCD_CHAR_WIDTH 8

#define DATA 1
#define INSTRUCTION 0

#define LCD_OFF 0
#define LCD_ON 1

#define READ 1
#define WRITE 0

#define LOWER_4BIT_MASK 0x0F
#define NULL 0

#define LOGICAL_CHARS_PER_LINE    40
#define CURSOR_INC_INSTRUCTION    0b00010100
#define CURSOR_RETURN_INSTRUCTION 0b00000010


//Special Init Commands
#define CMD_WAKE_UP         0b0011
#define CMD_FUCNTION_SET_1  0b0010  // 0 0 1 ,0 - LOW  for 4 bit (FIRST HALF FUCNTION SET)
#define CMD_FUCNTION_SET_2  0b1000  // (N,F,X,X)  N - 1/2 Line  F- 5x8/5x11 (2ND HALF FUCNTION SET)
#define CMD_PRE_EMPTY       0b0000

//Next three require CMD_PRE_EMPTY as MSB's (Pre 4bits)
#define CMD_DISPLAY_OPTS    0b1011  // (1,D,C,B)  D-Display , C-Cursor , B-Blink
#define CMD_DISPLAY_CLEAR   0b0001
#define CMD_CURSOR_OPTS     0b0110   // (0,1,I/D, SH)  I/D - H = Cursor Blinks and Moves to Right
                                     //                 SH - H = Shift of entire Display
//Changeable options
#define CMD_DISPLAY_ON  0b1111  // (1,D,C,B)  D-Display , C-Cursor , B-Blink
#define CMD_DISPLAY_OFF 0b1000  // (1,D,C,B)  D-Display , C-Cursor , B-Blink

//Local Prototypes
void int_to_string(unsigned long int i, char buf[]);


void lcd_init(void){

    int Count=0;

    //Step 2,3 & 4
    for (Count=0;Count<3;Count++){
        lcd_command_4bit(INSTRUCTION,WRITE,CMD_WAKE_UP);
        timer_delay_ms(WAKE_UP_DELAY);
    }


    //Step 5 - Send fist half of fucntion set to indicate 4 bit mode.
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_FUCNTION_SET_1);
    timer_delay_ms(BIT4_MODE_DELAY);

    //Step 6 - Resend the (real) first half followed by the secocnd
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_FUCNTION_SET_1);
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_FUCNTION_SET_2);
    lcd_wait_busy_flag();

    //Step 7
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_PRE_EMPTY);
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_DISPLAY_OPTS);
    lcd_wait_busy_flag();
    
    //step 8
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_PRE_EMPTY);
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_DISPLAY_CLEAR);
    lcd_wait_busy_flag();

    //Step 9
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_PRE_EMPTY);
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_CURSOR_OPTS);
    lcd_wait_busy_flag();

    //Display Control (Step 11)
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_PRE_EMPTY);
    lcd_command_4bit(INSTRUCTION,WRITE,CMD_DISPLAY_ON); 
    lcd_wait_busy_flag();

}

void lcd_display_control(char State){
    switch(State){
        case LCD_ON:
                //Display OFF (B=1,C=1,D=1)
                lcd_command_4bit(INSTRUCTION,WRITE,CMD_PRE_EMPTY);
                lcd_command_4bit(INSTRUCTION,WRITE,CMD_DISPLAY_ON);
                lcd_wait_busy_flag();
                break;
                
        case LCD_OFF:
            
                //Display OFF (B=0,C=0,D=0)
                lcd_command_4bit(INSTRUCTION,WRITE,CMD_PRE_EMPTY);
                lcd_command_4bit(INSTRUCTION,WRITE,CMD_DISPLAY_OFF);
                break;
    }
}





void int_to_string(unsigned long int i, char buf[])
{
	unsigned char temp;
	unsigned char s = 0,t = 0;
        if(i==0){
            buf[0]='0';
        }
        else{
            while(i) {
                    buf[s++] = i % 10 + '0';
                    i /= 10;
            }
            buf[s] = 0;
            s-=1;
            //Reverse Order
            for(;t<s;t++,s--) {
                    temp = buf[s];
                    buf[s]=buf[t];
                    buf[t] = temp;
            }
        }
}


void lcd_print_int(unsigned int Num , unsigned int max_digits){
    
    char* intStr;
    char  buffer[50];
    int   Count=0;
    int digitCount=0;

    if(Num==0){
        lcd_command_8bit(DATA,WRITE,'0');
    }
    else{

        //Convert Int to String
        int_to_string(Num, buffer);

        //Ensure Digits are Valid
        for(Count=0;Count<LCD_CHAR_WIDTH;Count++){
            if(buffer[Count]>='0'&&buffer[Count]<='9'){
                intStr[Count]=buffer[Count];
            }
            else{
                intStr[Count]=' ';
            }
        }
        intStr[Count+1]=0;

        //Loop through string pointer unitl empty or char width has been exceeded.
        while((*intStr!=NULL)&&(digitCount<max_digits)){
            lcd_command_8bit(DATA,WRITE,*(intStr++));
            digitCount++;
        }
    }
}

void lcd_print_str(rom char *Message){
    rom char *pos=Message;
    //Loop through string pointer unitl empty or char width has been exceeded.
    while((*pos!=NULL)&&pos<(Message+LCD_CHAR_WIDTH)){
        lcd_command_8bit(DATA,WRITE,(*pos++));
    }
}

void lcd_return_home(void){
    //Return Cursor Home
    lcd_command_8bit(INSTRUCTION,WRITE,CURSOR_RETURN_INSTRUCTION);
    lcd_wait_busy_flag();
}

void lcd_go_to_line(unsigned short lineNum){
    int i=0;
    switch(lineNum){
        case LCD_LINE_1:
            //Return Cursor Home
            lcd_return_home();
            break;
        case LCD_LINE_2:
            //Return Cursor Home
            lcd_return_home();
            //Increment Line Increase by moving vursor forward 40 chars.
            for (i=0;i<LOGICAL_CHARS_PER_LINE;i++){
                //Increment Cursor
                lcd_command_8bit(INSTRUCTION,WRITE,CURSOR_INC_INSTRUCTION);
                lcd_wait_busy_flag();
            }
            break;
    }
}


void lcd_command_8bit(char rs,char rw, char data){

    //Split Bits int upper and lower four bits
    char upper=(data>>4)&LOWER_4BIT_MASK;
    char lower= data&LOWER_4BIT_MASK;

    //Ensure Controller is free before continuing
    lcd_wait_busy_flag();
    
    //Send upper four bits folled by the lower four.
    lcd_command_4bit(rs, rw, upper);
    lcd_command_4bit(rs, rw, lower);
}

void lcd_command_4bit(char rs,char rw, char data){
    
    // Set RS and RW
    if(rs)gpio_on (RS_BIT,RS_PORT);
    else gpio_off(RS_BIT,RS_PORT);

    if(rw)gpio_on (RW_BIT,RW_PORT);
    else  gpio_off(RW_BIT,RW_PORT);

    //Set Data
    if(data&(D7_INDENTIFER)) gpio_on (D7_BIT,D7_PORT);
    else  gpio_off(D7_BIT,D7_PORT);

    if(data&(D6_INDENTIFER)) gpio_on (D6_BIT,D6_PORT);
    else  gpio_off(D6_BIT,D6_PORT);

    if(data&(D5_INDENTIFER)) gpio_on (D5_BIT,D5_PORT);
    else  gpio_off(D5_BIT,D5_PORT);

    if(data&(D4_INDENTIFER)) gpio_on (D4_BIT,D4_PORT);
    else  gpio_off(D4_BIT,D4_PORT);

    //Enable High
    gpio_on(EN_BIT,EN_PORT);

    //Alow enable oin time to be recongined
    timer_delay_ms(EN_PULSE_DELAY);

    //Enable Low
    gpio_off(EN_BIT,EN_PORT);
}

void lcd_wait_busy_flag(void){

    //Enable Read Pin
    gpio_on (RW_BIT,RW_PORT);

    //Configure D7 as input and wait unitl low.
    gpio_set_input(D7_BIT,D7_PORT);
    while(gpio_read(D7_BIT,D7_PORT));

    //Disable Read Pin (i.e. go abck to writing)
    gpio_off(RW_BIT,RW_PORT);

    //Return D7 to being an output.
    gpio_set_output(D7_BIT,D7_PORT);
 
}

void lcd_deinit(void){
    lcd_display_control(LCD_OFF);
}

void lcd_reinit(void){
    lcd_display_control(LCD_ON);
}