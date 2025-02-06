/*
 * motor_driver.c
 *
 *  Created on: Feb 3, 2025
 *      Author: princ
 */
#include "motor_driver.h"

extern TIM_HandleTypeDef htim1;

ErrorStatus init_mosfet(MOS_gate *gate , GPIO_TypeDef* GPIO_PORT , uint16_t PIN){
	if(gate == NULL)return ERROR;
	gate->GPIO_PORT = GPIO_PORT;
	gate->PIN = PIN;
	gate->state = NA;
	return SUCCESS;
}

ErrorStatus init_phase(Phase *phase , MOS_gate *high , MOS_gate *low , TIM_HandleTypeDef *htim, uint32_t channel){
	if(phase == NULL || high == NULL || low == NULL || htim == NULL )return ERROR;
	phase->htim = htim;
	phase->channel = channel;
	phase->high_side = high;
	phase->low_side = low;
	return SUCCESS;
}

static ErrorStatus set_mosfet_to_gpio(MOS_gate *gate){
	if(gate == NULL)return ERROR;
	if(gate->state == ON || gate->state == OFF || gate->state == GPIO)return SUCCESS;
	uint32_t pin_pos = __builtin_ctz(gate->PIN); // Find the pin position (0-15)

	// Disable the timer output
	//__HAL_TIM_DISABLE(&htim1);
	// Determine if it's in CRL (pins 0-7) or CRH (pins 8-15)
	if (pin_pos < 8)
	{
		gate->GPIO_PORT->CRL &= ~(0xF << (pin_pos * 4)); // Clear the 4-bit field
		gate->GPIO_PORT->CRL |= (0x3 << (pin_pos * 4)); // Set MODE=11 (50 MHz), CNF=00 (Push-Pull)
	}
	else
	{
		pin_pos -= 8;
		gate->GPIO_PORT->CRH &= ~(0xF << (pin_pos * 4)); // Clear the 4-bit field
		gate->GPIO_PORT->CRH |= (0x3 << (pin_pos * 4)); // Set MODE=11 (50 MHz), CNF=00 (Push-Pull)
	}
	return SUCCESS;
}
static ErrorStatus set_mosfet_to_pwm(MOS_gate *gate){
	if(gate == NULL)return ERROR;
	if(gate->state == PWM)return SUCCESS;

	uint32_t pin_pos = __builtin_ctz(gate->PIN); // Find the pin position (0-15)

	// Disable the timer output
	//__HAL_TIM_DISABLE(&htim1);

	// Determine if it's in CRL (pins 0-7) or CRH (pins 8-15)
	if (pin_pos < 8)
	{
		gate->GPIO_PORT->CRL &= ~(0xF << (pin_pos * 4)); // Clear the 4-bit field
		gate->GPIO_PORT->CRL |= (0xB << (pin_pos * 4)); // Set MODE=10 (Input mode), CNF=11 (Alternate Function Push-Pull)
	}
	else
	{
		pin_pos -= 8;
		gate->GPIO_PORT->CRH &= ~(0xF << (pin_pos * 4)); // Clear the 4-bit field
		gate->GPIO_PORT->CRH |= (0xB << (pin_pos * 4)); // Set MODE=10 (Input mode), CNF=11 (Alternate Function Push-Pull)
	}

	// Re-enable the timer output
	//__HAL_TIM_ENABLE(&htim1);
	return SUCCESS;
}

ErrorStatus set_phase_to_complementary_pwm(Phase *phase){
	if(phase == NULL)return ERROR;
	ErrorStatus status = SUCCESS;
	status |= set_mosfet_to_pwm(phase->high_side);
	status |= set_mosfet_to_pwm(phase->low_side);
	return status;
}
void start_pwm(Phase *phase){
	if(phase == NULL)return;
	HAL_TIM_PWM_Start(phase->htim, phase->channel);
	HAL_TIMEx_PWMN_Start(phase->htim, phase->channel);
	phase->status = PWM;
}
void stop_pwm(Phase *phase){
	if(phase == NULL)return;
	HAL_TIM_PWM_Stop(phase->htim , phase->channel);
	HAL_TIMEx_PWMN_Stop(phase->htim, phase->channel);
	phase->status = NA;
}
ErrorStatus set_mosfet_to(MOS_gate *gate ,E_STATUS state){
	if(gate == NULL)return ERROR;
	ErrorStatus status = SUCCESS;
	if(gate->state == ON || gate->state == OFF || gate->state == GPIO){
		status |= set_mosfet_to_gpio(gate);
	}
	if(state == ON){
		HAL_GPIO_WritePin(gate->GPIO_PORT, gate->PIN, GPIO_PIN_SET);
	}
	else if(state == OFF){
		HAL_GPIO_WritePin(gate->GPIO_PORT, gate->PIN, GPIO_PIN_RESET);
	}
	else if(state == PWM){
		set_mosfet_to_pwm(gate);
	}
	else{

	}
	return status;
}

ErrorStatus set_phase_pwm_DC(Phase *phase , uint8_t DC){
	if(phase == NULL || phase->htim == NULL)return ERROR;
	if(phase->status != PWM){
		start_pwm(phase);
	}
	uint16_t cc_val = ((uint32_t) (phase->htim->Instance->ARR - 1) * DC) / 100;
	if(phase->channel == TIM_CHANNEL_1){
		phase->htim->Instance->CCR1 = cc_val;
	}
	else if(phase->channel == TIM_CHANNEL_2){
		phase->htim->Instance->CCR2 = cc_val;
	}else if(phase->channel == TIM_CHANNEL_3){
		phase->htim->Instance->CCR3 = cc_val;
	}else{

	}
	return SUCCESS;
}
