/******************************************************************************/
/*
 * File:   scooter_interrupts.c
 * Author: Matt Kokshoorn
 *
 * Created on July 2, 2013, 10:21 PM
 *
 * Notes:
 * 
 *
/******************************************************************************/


#include <p18cxxx.h>
#include "scooter_interrupts.h"
#include "portb.h"

void interrupts_init(void){
    ADCON1 = 0x03;
    //**************** configure INT0 with pullups enabled, falling edge *********************************
    OpenRB0INT(PORTB_CHANGE_INT_ON | FALLING_EDGE_INT | PORTB_PULLUPS_ON);  //configures INT0 & enables it
    OpenRB1INT(PORTB_CHANGE_INT_ON | RISING_EDGE_INT | PORTB_PULLUPS_OFF);  //configures INT1 & enables it
    //OpenPORTB(PORTB_CHANGE_INT_ON | PORTB_PULLUPS_ON);//configures and enables change notification in PORTB
}
