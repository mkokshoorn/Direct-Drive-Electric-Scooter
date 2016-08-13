/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/
#include "system.h"
#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "scooter_gpio.h"

#define MS_PER_S 1000
#define INT_PER_MS 1000


// Timing Variables
extern unsigned int miliseconds;
extern unsigned int seconds;
unsigned int int_counter=0;
// Sleep Boolean
extern unsigned int sleeping;

//Speed Signal Variables
extern long unsigned int signalPeriod;
extern char speedUpdated;
unsigned int previousTime_s=0;
unsigned int currentTime_s=0;
unsigned int previousTime_ms=0;
unsigned int currentTime_ms=0;
unsigned int previousTime_ints=0;
unsigned int currentTime_ints=0;

//Button Reset Var
extern char buttonReleasedFromReset;

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 1
  //Time Keeping Interupt
  if (INTCONbits.TMR0IF){   //check for TMR0 overflow
        int_counter++;
        INTCONbits.TMR0IF = 0;                        //clear interrupt flag

        TMR0H=((0xFFFF-(FCY/INT_PER_MS)>>8)&0x00FF);      // Reset Timer
        TMR0L=(0xFFFF-(FCY/INT_PER_MS))&0x00FF;           // Reset Timer
        
        if (int_counter>=(INT_PER_MS/MS_PER_S)){
            miliseconds++;
            int_counter=0;
        }

        if(miliseconds>MS_PER_S){
            miliseconds=0;
            seconds++;
        }
    }

    //Check for INT0 (Speed Signal)
    else if( INTCONbits.INT0IF == 1){
        currentTime_ints=int_counter;
        currentTime_ms=miliseconds;
        currentTime_s =seconds;

        signalPeriod = (currentTime_ints-previousTime_ints);
        signalPeriod = signalPeriod +(currentTime_s-previousTime_s)*MS_PER_S*10;
        signalPeriod = signalPeriod+(currentTime_ms-previousTime_ms)*10;

        speedUpdated=1;

        previousTime_ints=currentTime_ints;
        previousTime_ms=currentTime_ms;
        previousTime_s=currentTime_s;
        INTCONbits.INT0IF = 0;      // Clear INT0
    }

    else if(INTCON3bits.INT1IF == 1)     //Check for INT0
    {
        if((!sleeping)&&(buttonReleasedFromReset))sleeping=1;
        INTCON3bits.INT1IF = 0;     // Clear INT1
    }

    // Unknown Interupt
    else{
        while(1);
    }

#endif
}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0xFA8
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0




#endif

}
