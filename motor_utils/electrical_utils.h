/*
 * electrical_utils.h
 *	types to represents the electrical phases and motor utils
 *	along with utility functions to domesticate the electrical details to abstract functions
 *      Author: princ
 */

#ifndef ELECTRICAL_UTILS_H_
#define ELECTRICAL_UTILS_H_
/*
 * Variety of
 * Electrical status
 * notes:
 * 	GPIO is the generalized state of LOW/HIGH whenver we don't know..
 */
typedef enum{
	GPIO,
	OFF,
	ON,
	PWM,
	NA
}E_STATUS;

/*
 * Representation of a mosfet gate (low side or high side)
 */
typedef struct {
	GPIO_TypeDef* GPIO_PORT;
	uint16_t PIN;
	E_STATUS state;
}MOS_gate;
/*
 * Motor Electrical phase of an inverter , hence it has High and low side mosfet gates to control..
 */
typedef struct{
	MOS_gate *high_side;
	MOS_gate *low_side;
	//timer details
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	E_STATUS status;
}Phase;

#endif /* ELECTRICAL_UTILS_H_ */
