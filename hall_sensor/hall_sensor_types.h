/*
 * hall_sensor_types.h
 *
 *  Created on: Oct 29, 2024
 *      Author: Mahmoud
 */

#ifndef HALL_SENSOR_TYPES_H_
#define HALL_SENSOR_TYPES_H_
typedef struct{
	GPIO_TypeDef* GPIO_PORT;
	uint16_t PIN;
} HALL_SENSOR;
typedef union {
	struct {
		uint8_t h1 : 1;//LSB
		uint8_t h2 : 1;
		uint8_t h3 : 1;
	};
	uint8_t state;
}HALL_STATE;



#endif /* HALL_SENSOR_TYPES_H_ */
