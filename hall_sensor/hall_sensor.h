/*
 * hall_sensor.h
 *
 *  Created on: Oct 29, 2024
 *      Author: Mahmoud
 *  A hall sensors representation and interface
 */

#ifndef HALL_SENSOR_H_
#define HALL_SENSOR_H_
#include "main.h"

#include "hall_sensor_types.h"

void init_hall_sensor(HALL_SENSOR *sensor , GPIO_TypeDef* GPIO_PORT , uint16_t PIN);
uint8_t read_hall_sensor(HALL_SENSOR *sensor);

/*
 * Updates the hall sensor state reading the related GPIO pins , usually three hall sensors..
 * @param  hstate  a pointer to a HALL_STATE object
 * @param hs1 first hall sensor , pointer to HALL_SENSOR object
 * @param hs2 second hall sensor , pointer to HALL_SENSOR object
 * @param hs3 third hall sensor , pointer to HALL_SENSOR object
 *
 */
void update_hall_state(HALL_STATE *hstate ,HALL_SENSOR *hs1 , HALL_SENSOR *hs2 , HALL_SENSOR *hs3);
#endif /* HALL_SENSOR_H_ */
