/******************************************************************************/
/*
 * File:   scooter_timer.h
 * Author: Matt Kokshoorn
 *
 * Created on July 2, 2013, 10:21 PM
/******************************************************************************/

#ifndef SCOOTER_TIMER_H
#define	SCOOTER_TIMER_H


void timer_init(void);

void timer_deinit(void);

void timer_delay(unsigned int ticks);

void timer_delay_ms(unsigned int ticks);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SCOOTER_TIMER_H */

