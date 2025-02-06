/*
 * motor_driver.h
 *		Abstract motor functions to control the phases..
 *		Intended to be used to with Timer1 (mainly as it is intended for motors )
 *      Author: princ
 */

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include "main.h"
#include "electrical_utils.h"
/*
//maximum number of complementary channels of Timer1
#define TIM1_CHANNELS_MAX 4
*/

/**
 * Assigns a pin to the mosfet gate
 * With any suitable initialization
 */
ErrorStatus init_mosfet(MOS_gate *gate , GPIO_TypeDef* GPIO_PORT , uint16_t PIN);
/**
 * Assigns low and high side mosfets to a phase
 * With any suitable initialization
 */
ErrorStatus init_phase(Phase *phase , MOS_gate *high , MOS_gate *low , TIM_HandleTypeDef *htim, uint32_t channel);
/**
 * Setup the mosfet to gpio high or low if not
 */
static ErrorStatus set_mosfet_to_gpio(MOS_gate *gate);
/*
 * Setup the mosfet to PWM
 */
static ErrorStatus set_mosfet_to_pwm(MOS_gate *gate);
/**
 * Starts the complimentary timer PWM on corresponding channel and its complementary
 */
void start_pwm(Phase *phase);
/**
 * Stops timer PWM
 */
void stop_pwm(Phase *phase);
/**
 * Set Mosfet mode (on,off,PWM switching , just GPIO..)
 */
ErrorStatus set_mosfet_to(MOS_gate *gate ,E_STATUS state);

/**
 * sets Duty cycle of a phase switching pwm
 * DC is duty cycle % (max 100)
 */
ErrorStatus set_phase_pwm_DC(Phase *phase , uint8_t DC);
/*
 * Set a whole phase to complementary switching PWM
 * must use start_pwm to start it..
 */
ErrorStatus set_phase_to_complementary_pwm(Phase *phase);
#endif /* MOTOR_DRIVER_H_ */
