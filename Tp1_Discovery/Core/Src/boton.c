/*
 * boton.c
 *
 *  Created on: 27 ago 2026
 *      Author: agust
 */

#include "boton.h"

void Read_Button_Task(void){
	GPIO_PinState estado_actual = HAL_GPIO_ReadPin(GPIOA, B1_Pin);

	static GPIO_PinState estado_anterior = GPIO_PIN_RESET;
	static GPIO_PinState estado_valido = GPIO_PIN_RESET;
	static uint32_t tiempo_ultimo_cambio = 0;

	if (estado_actual != estado_anterior) {
		tiempo_ultimo_cambio = HAL_GetTick();
	}
	if ((HAL_GetTick() - tiempo_ultimo_cambio) > 50) {
		if (estado_actual != estado_valido) {
			estado_valido = estado_actual;
		}
	}

	if (estado_valido == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_SET);
	}else {
		HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_RESET);
	}
	estado_anterior = estado_actual;
}
