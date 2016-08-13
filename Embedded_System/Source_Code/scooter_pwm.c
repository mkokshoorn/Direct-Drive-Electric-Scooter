/******************************************************************************/
/*
 * File:   scooter_pwm.c
 * Author: Matt Kokshoorn
 *
 * Created on July 4, 2013, 4:21 AM
 *
 * Notes:
 * 
 *
/******************************************************************************/


#include <p18cxxx.h>
#include "scooter_pwm.h"
#include "scooter_gpio.h"
#include "pwm.h"
#include "timers.h"


void pwm_init(void){


    //PWM Init
    OpenTimer2(TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_4);
    CCP1CON = 0;
    PR2 = PWM_PERIOD; /* Set Time Period Value */
    TRISCbits.TRISC2=0; /* Set RC2 as an output pin */
    T2CON = 0x00; /* No Prescalers Timer 2 OFF */
    CCP1CON = 0b00001100; /* PWM Mode */
    CCPR1L = 0b01000011;
    TMR2 = 0; /* Reset Timer 2 */
    T2CONbits.TMR2ON = 1; /* Timer 2 ON */

    SetOutputEPWM1(SINGLE_OUT, PWM_MODE_1);
    PSTR1CONbits.STR1A=0;
    PSTR1CONbits.STR1D=1;
    gpio_set_output (PWM_OUT_BIT,PWM_OUT_PORT);  //Set PB4 as ouput
    
}

//Takes value between 0 and 10000
void pwm_set_dc(long unsigned int D){
    CCPR1L=(D*PWM_PERIOD)/100000;
}



void pwm_deinit(void){
    pwm_set_dc(0);
}