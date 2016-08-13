/******************************************************************************/
/*
 * File:   scooter_timer.c
 * Author: Matt Kokshoorn
 * Created on July 2, 2013, 10:21 PM
 *
 * Notes:
 * Currenlty uses timer0 and timer 1
 *
/******************************************************************************/

#include <p18cxxx.h>
#include "timers.h"
#include "scooter_timer.h"
#include "system.h"

extern unsigned int miliseconds;
extern unsigned int seconds;
extern unsigned int current_time;


void timer_init(void){

    //Timer 0 - Used for timekeeping

    INTCONbits.TMR0IE=1;
    INTCON2bits.TMR0IP=1;

    RCONbits.IPEN = 1;            //enable priority levels

    TMR0H = 0;                    //clear timer
    TMR0L = 0;                    //clear timer
    OpenTimer0((TIMER_INT_ON&T0_16BIT&T0_SOURCE_INT&T0_PS_1_1));

    //Timer 1 - Used for delays

    OpenTimer1((TIMER_INT_OFF&T1_16BIT_RW&T1_SOURCE_FOSC_4&T1_PS_1_1&T1_OSC1EN_OFF&T1_SYNC_EXT_OFF ),0);
}

void timer_deinit(void){
    //Timer 0 - Used for timekeeping
    CloseTimer0();

    //Timer 1 - Used for delays
    CloseTimer1();
}


void timer_delay(unsigned int ticks){
    WriteTimer1(0);
    while(ReadTimer1()<ticks);
}


void timer_delay_ms(unsigned int miliseconds){

    float ticks =(FCY*miliseconds/1000);///1000/256)* miliseconds;  //Convert ms to ticks  (Tick per ms *ms)   -> Ticks/ms =FCY/1000/2/ prescaler
    while(ticks>40000){
        timer_delay(40000);
        ticks=ticks-40000;
    }
    timer_delay(ticks);
}


