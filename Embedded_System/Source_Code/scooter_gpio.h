/******************************************************************************/
/*
 * File:   scooter_gpio.h
 * Author: Matt Kokshoorn
 *
 * Created on July 2, 2013, 10:21 PM
/******************************************************************************/

#ifndef SCOOTER_GPIO_H
#define	SCOOTER_GPIO_H

#define GPIO_PORTA 1
#define GPIO_PORTB 2
#define GPIO_PORTC 4


#define BIT(NUM) (1<<NUM)&(0xFF)

#define BIT0  1
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)


//ADC Pins
#define THROTTLE_IN_PORT    GPIO_PORTA
#define THROTTLE_IN_BIT     BIT3

#define CURRENT_IN_PORT     GPIO_PORTA
#define CURRENT_IN_BIT      BIT0

// Interupt Pins
#define SPEED_IN_PORT       GPIO_PORTB
#define SPEED_IN_BIT        BIT0

#define BUTTON_IN_PORT      GPIO_PORTB
#define BUTTON_IN_BIT       BIT1

//ADC GPIO
#define RS_PORT     GPIO_PORTC
#define RS_BIT      BIT6

#define RW_PORT     GPIO_PORTC
#define RW_BIT      BIT7

#define EN_PORT     GPIO_PORTA
#define EN_BIT      BIT7

#define D4_PORT     GPIO_PORTA
#define D4_BIT      BIT6

#define D5_PORT     GPIO_PORTC
#define D5_BIT      BIT1

#define D6_PORT     GPIO_PORTC
#define D6_BIT      BIT0

#define D7_PORT     GPIO_PORTC
#define D7_BIT      BIT2

//PWM Output
#define PWM_OUT_PORT       GPIO_PORTB
#define PWM_OUT_BIT        BIT4

//Remaining GPIO Inputs
#define BREAK_IN_PORT       GPIO_PORTB
#define BREAK_IN_BIT        BIT2

#define BOOST_IN_PORT       GPIO_PORTB
#define BOOST_IN_BIT        BIT3

//Remaining GPIO Outputs
#define BREAK_OUT_PORT       GPIO_PORTA
#define BREAK_OUT_BIT        BIT1



void gpio_init(void);

void gpio_deinit(void);

void gpio_on(unsigned char bitmask,unsigned char port);

void gpio_off(unsigned char bitmask,unsigned char port);

void gpio_toggle(unsigned char bitmask,unsigned char port);

unsigned char gpio_read(unsigned char bitmask,unsigned char port);

void gpio_set_output(unsigned char bitmask,unsigned char port);

void gpio_set_input(unsigned char bitmask,unsigned char port);



#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SCOOTER_GPIO_H */

