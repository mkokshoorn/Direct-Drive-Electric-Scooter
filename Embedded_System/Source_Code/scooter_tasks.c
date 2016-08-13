/******************************************************************************/
/*
 * File:   scooter_tasks.c
 * Author: Matt Kokshoorn
 *
 * Created on August 7, 2013, 10:21 PM
 *
 * Notes:
 * 
 *
/******************************************************************************/


#include <p18cxxx.h>
#include "scooter_tasks.h"
#include "scooter_gpio.h"
#include "scooter_pwm.h"

char check_break(void){
        char breaking=0;
        breaking = gpio_read(BREAK_IN_BIT,BREAK_IN_PORT);
        if(!breaking) gpio_on(BREAK_OUT_BIT,BREAK_OUT_PORT);

        //Turn Break on and reduce PWM
        if(breaking)  gpio_off(BREAK_OUT_BIT,BREAK_OUT_PORT);
        

        return breaking;

}

