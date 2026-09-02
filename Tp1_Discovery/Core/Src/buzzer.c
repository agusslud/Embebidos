/*
 * buzzer.c
 *
 *  Created on: 28 ago 2026
 *      Author: aguss
 */

#include "buzzer.h"

void Buzzer_Init(void){
	HAL_GPIO_WritePin(GPIOD, Buzzer_Pin, GPIO_PIN_SET);
}

void Buzzer_Alarm_Task(void){
	static uint32_t t_alarma = 0;
	static uint8_t buzzer_activo = 0;
	uint32_t now = HAL_GetTick();

	if ((now - t_alarma) > 5000) {
		t_alarma += 5000;
		buzzer_activo = 1;
		HAL_GPIO_WritePin(GPIOD, Buzzer_Pin, GPIO_PIN_RESET);
	}

	if (buzzer_activo && (now - t_alarma) >= 100) {
		buzzer_activo = 0;
		HAL_GPIO_WritePin(GPIOD, Buzzer_Pin, GPIO_PIN_SET);
	}
}
