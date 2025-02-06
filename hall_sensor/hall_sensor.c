/*
 * hall_sensor.c
 *
 *  Created on: Oct 29, 2024
 *      Author: princ
 */

#include "hall_sensor.h"

void init_hall_sensor(HALL_SENSOR *sensor , GPIO_TypeDef* GPIO_PORT , uint16_t PIN){
	if(sensor == NULL)return;
	sensor->GPIO_PORT = GPIO_PORT;
	sensor->PIN = PIN;
}

uint8_t read_hall_sensor(HALL_SENSOR *sensor){
	if(sensor == NULL)return 0;
	return HAL_GPIO_ReadPin(sensor->GPIO_PORT, sensor->PIN);
}

void update_hall_state(HALL_STATE *hstate ,HALL_SENSOR *hs1 , HALL_SENSOR *hs2 , HALL_SENSOR *hs3 ){

	hstate->h1 = read_hall_sensor(hs1);
	hstate->h2 = read_hall_sensor(hs2);
	hstate->h3 = read_hall_sensor(hs3);
}
