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
#include "adc.h"
#include "scooter_adc.h"


char current_adc_channel=ADC_CH3;

void adc_init(void){
    OpenADC(ADC_FOSC_2 & ADC_LEFT_JUST & ADC_12_TAD,
                         ADC_CH3 & ADC_CH0 & ADC_INT_OFF & ADC_REF_VDD_VSS  ,
                         3);//ADC_1ANA

    SetChanADC(ADC_CH3);            //Swaps Channel
    SelChanConvADC( ADC_CH3 );
}

void adc_swap_channel(char adc_channel){
    switch(adc_channel){
        case ADC_CURRENT:
            SetChanADC(ADC_CH0);
            SelChanConvADC(ADC_CH0);
            break;
            
        case ADC_THROTTLE:
            SetChanADC(ADC_CH3);
            SelChanConvADC(ADC_CH3);
            break;
    }
}

void adc_deinit(void){
    CloseADC();
}

int adc_get_raw(void){
    ConvertADC();
    while(BusyADC());
    return ( ReadADC());
}









