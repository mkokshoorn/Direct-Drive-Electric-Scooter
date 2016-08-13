/******************************************************************************/
/* System Files to Include                                                    */
/******************************************************************************/

#include <p18cxxx.h>   /* C18 General Include File */
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Fundamental Constants                                                      */
/******************************************************************************/
#define RESET_THRESHOLD 10
#define INIT_DELAY      400

/******************************************************************************/
/* Import Modules                                                             */
/******************************************************************************/

#include "scooter_timer.h"
#include "scooter_gpio.h"
#include "scooter_adc.h"
#include "scooter_interrupts.h"
#include "scooter_pwm.h"
#include "scooter_lcd.h"

#include "scooter_tasks.h"


#define BAT_SYM 219


/******************************************************************************/
/* Internal Prototypes                                                        */
/******************************************************************************/
void init_all(void);
void deinit_all(void);
void reinit_all(void);
void sleep_routine(void);

/******************************************************************************/
/* Globals                                                                    */
/******************************************************************************/

//timing globals
unsigned int miliseconds=0;
unsigned int seconds=0;
unsigned int current_time=0;
unsigned int sleeping=0;


//Speed Signal
long unsigned int signalPeriod=0;
char speedUpdated=0;

//Button Reset Variable
unsigned int holdCount=0;
char buttonReleasedFromReset=0;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main () {
    int adc_val=0;
    long unsigned int throttle=0;
    long unsigned int batteryPercentage=0;
    long unsigned int boostPercentage=0;
    long unsigned int current=0;
    long unsigned int scooterSpeed=0;
    long unsigned int scooterPWM=SCOOTER_IDLE_PWM;
    
    init_all();
    
    
    while(1){

       

        // ADC CURRENT GET
        adc_swap_channel(ADC_CURRENT);
        adc_val=adc_get_raw();
        current=(unsigned int)adc_val;
        current = current*5000;
        current=  current/65535;
        


        // ADC THROTTLE GET
        adc_swap_channel(ADC_THROTTLE);
        adc_val=adc_get_raw();
        throttle=(unsigned int)adc_val;
        throttle = throttle*5000;
        throttle=  throttle/65535;


        //Check Break and Update PWM
        scooterPWM=SCOOTER_IDLE_PWM;
        //Check for break
        if(!check_break()){
            if((throttle>SCOOTER_IDLE_PWM) && (throttle < SCOOTER_MAX_PWM)){
                scooterPWM=throttle;
            }

        }

        //Update Motor Speed Control
        pwm_set_dc((long unsigned int)scooterPWM*20); //i.e. out of 100000


        //Get Scooter Speed
        scooterSpeed=0;
        if(signalPeriod>0 && speedUpdated==1) scooterSpeed=(33800)/(8*signalPeriod); //r=0.94 , speed= (m/s to km/hr)* r * (signal frequency to second conversion) /(sensors per revoloution * period)
        speedUpdated=0;

       
        //Display two values
        if(scooterSpeed<10){
            lcd_print_str((rom char *)" ");
            lcd_print_int(scooterSpeed,1);
        }
        else{
            lcd_print_int(scooterSpeed,2);
        }
        
        lcd_print_str((rom char *)"km/hr");

        if(gpio_read(BREAK_IN_BIT,BREAK_IN_PORT)){
            lcd_command_8bit(DATA,WRITE,'|');
        }
        else if(gpio_read(BOOST_IN_BIT,BOOST_IN_PORT)){
            lcd_command_8bit(DATA,WRITE,'^');
        }
        else{
            lcd_command_8bit(DATA,WRITE,' ');
        }

        lcd_go_to_line(LCD_LINE_2);


        /*
       batteryPercentage= 40;
        
       
       lcd_command_8bit(DATA,WRITE,BAT_SYM);
       if( batteryPercentage>=100){
           lcd_print_str((rom char *)"FUL");
       }
       else{
           if (batteryPercentage<10){
               lcd_command_8bit(DATA,WRITE,' ');
               lcd_print_int(batteryPercentage,1);
           }
           else{
               lcd_print_int(batteryPercentage,2);
           }
           lcd_command_8bit(DATA,WRITE,'%');
       }
       */



        //Boost Display
        lcd_print_str((rom char *)":");
        lcd_print_int(scooterPWM,4);

        //End boost Display
        lcd_go_to_line(LCD_LINE_1);
        


        // Check Sleep and Reset Conditions
        if(sleeping){
            sleep_routine();
        }
        else{
            if((gpio_read(BUTTON_IN_BIT,BUTTON_IN_PORT))==0){
                holdCount++;
                if(holdCount>=RESET_THRESHOLD) Reset();
            }
            else{
                buttonReleasedFromReset=1;
                holdCount=0;
            }
            
        }
    }
}


/******************************************************************************/
/* Initilisation                                                                 */
/******************************************************************************/

void init_all(void){

    OSCCONbits.IRCF = 7;

    timer_init();
    timer_delay_ms(INIT_DELAY);
    
    gpio_init();
    lcd_init();
    
    adc_init();
    interrupts_init();
    pwm_init();

   INTCONbits.GIEH = 1;          //enable interrupts
}

/******************************************************************************/
/* Re/Deinitilisation                                                            */
/******************************************************************************/
void reinit_all(void){

    timer_init();
    gpio_init();
    lcd_reinit();
    adc_init();
    interrupts_init();
    pwm_init();

    INTCONbits.GIEH = 1;          //Enable interrupts  
}


void deinit_all(void){
    INTCONbits.GIEH = 0;          //Disable interrupts
    lcd_deinit();
    timer_deinit();
    gpio_deinit();
    adc_deinit();
    pwm_deinit();
}

void sleep_routine(void){
    // Suss low power stuff and LCD off -(still need wake up ? adc sampling ? and duty cycle to zero)
    holdCount=0;
    deinit_all();
    Sleep();
    reinit_all();
    sleeping=0;
}
