/******************************************************************************/
/*
 * File:   scooter_gpio.c
 * Author: Matt Kokshoorn
 *
 * Created on July 2, 2013, 10:21 PM
 *
 * Notes:
 * Currently Uses GPIO PB2
 *
/******************************************************************************/


#include <p18cxxx.h>
#include "scooter_gpio.h"

void gpio_init(void){

    //LCD Pins Note: D7 Briefly becomes input
    gpio_set_output(EN_BIT,EN_PORT);
    gpio_set_output(RS_BIT,RS_PORT);
    gpio_set_output(RW_BIT,RW_PORT);
    gpio_set_output(D4_BIT,D4_PORT);
    gpio_set_output(D5_BIT,D5_PORT);
    gpio_set_output(D6_BIT,D6_PORT);
    gpio_set_output(D7_BIT,D7_PORT);

    //Interupt Pins
    gpio_set_input(SPEED_IN_BIT,SPEED_IN_PORT);
    gpio_set_input(BUTTON_IN_BIT,BUTTON_IN_PORT);

    //Disable Interupt pins as Analog
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;

    //Disable PWM as analog.
    ANSELBbits.ANSB4=0;


    //Remaining GPIO Inputs
    gpio_set_input(BREAK_IN_BIT,BREAK_IN_PORT);
    gpio_set_input(BOOST_IN_BIT,BOOST_IN_PORT);
    //Disable Boost and break pins as Analog
    ANSELBbits.ANSB2=0;
    ANSELBbits.ANSB3=0;

    //Remaining GPIO Outputs
    gpio_set_output(BREAK_OUT_BIT,BREAK_OUT_PORT);
    //Disable Break output as Analog
    ANSELAbits.ANSA1=0;




}

void gpio_on(unsigned char bitmask,unsigned char port){
    switch(port){
        case GPIO_PORTA:
            PORTA=LATA|(bitmask);      // Turn On PORTA bits
            break;
        case GPIO_PORTB:
            PORTB=LATB|(bitmask);      // Turn On PORTB bits
            break;
        case GPIO_PORTC:
            PORTC=LATC|(bitmask);      // Turn On PORTC bits
            break;
    }
}


void gpio_off(unsigned char bitmask,unsigned char port){
    switch(port){
        case GPIO_PORTA:
            PORTA=LATA&(~(bitmask));      // Turn Off PORTA bits
            break;
        case GPIO_PORTB:
            PORTB=LATB&(~(bitmask));      // Turn Off PORTB bits
            break;
        case GPIO_PORTC:
            PORTC=LATC&(~(bitmask));      // Turn Off PORTC bits
            break;
    }
}


void gpio_toggle(unsigned char bitmask,unsigned char port){
    switch(port){
        case GPIO_PORTA:
            PORTA=(LATA)^(bitmask);      // Toggle PORTA Bits
            break;
        case GPIO_PORTB:
            PORTB=(LATB)^(bitmask);      // Toggle PORTB Bits
            break;
        case GPIO_PORTC:
            PORTC=(LATC)^(bitmask);      // Toggle PORTC Bits
            break;
    }
}


unsigned char gpio_read(unsigned char bitmask,unsigned char port){


    switch(port){
        case GPIO_PORTA:
            return (PORTA&bitmask);       // Read PORTA Bits
        case GPIO_PORTB:
            return (PORTB&bitmask);       // Read PORTB Bits
        case GPIO_PORTC:
            return (PORTC&bitmask);       // Read PORTC Bits
    }
}

void gpio_set_output(unsigned char bitmask,unsigned char port){

    switch(port){
        case GPIO_PORTA:
             TRISA = TRISA&(~(bitmask));   // Set Port A Bit x to 0 (output)
             break;
        case GPIO_PORTB:
             TRISB = TRISB&(~(bitmask));   // Set Port B Bit x to 0 (output)
             break;
        case GPIO_PORTC:
             TRISC = TRISC&(~(bitmask));   // Set Port C Bit x to 0 (output)
             break;
    }

}

void gpio_set_input(unsigned char bitmask,unsigned char port){
    
    switch(port){
        case GPIO_PORTA:
             TRISA = TRISA|((bitmask));   // Set Port A Bit x to 1 (input)
             break;
        case GPIO_PORTB:
             TRISB = TRISB|((bitmask));   // Set Port B Bit x to 1 (input)
             break;
        case GPIO_PORTC:
             TRISC = TRISC|((bitmask));   // Set Port C Bit x to 1 (input)
             break;
    }
    
}


void gpio_deinit(void){

}


