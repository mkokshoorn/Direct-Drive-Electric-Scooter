/******************************************************************************/
/*
 * File:   scooter_pwm.h
 * Author: Matt Kokshoorn
 *
 * Created on July 4, 2013, 4:21 AM
 *
 * Notes:
 *
 *
/******************************************************************************/


#ifndef SCOOTER_PWM_H
#define	SCOOTER_PWM_H

#define PWM_PERIOD 0b11000000


void pwm_init(void);

void pwm_set_dc(long unsigned int D);

void pwm_deinit(void);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SCOOTER_PWM_H */

