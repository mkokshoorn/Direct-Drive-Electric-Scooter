/******************************************************************************/
/*
 * File:   scooter_adc.h
 * Author: Matt Kokshoorn
 *
 * Created on July 3, 2013, 10:21 PM
/******************************************************************************/

#ifndef SCOOTER_ADC_H
#define	SCOOTER_ADC_H


void adc_init(void);

void adc_deinit(void);

int adc_get_raw(void);

void adc_swap_channel(char adc_channel);

#define ADC_CURRENT  0
#define ADC_THROTTLE 1



#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SCOOTER_ADC_H */

