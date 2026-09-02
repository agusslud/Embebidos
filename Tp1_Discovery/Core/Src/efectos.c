/*
 * efectos.c
 *
 *  Created on: 27 ago 2026
 *      Author: agust
 */

#include "efectos.h"

void Leds_Temporizados(void){
	static uint32_t tLed1 = 0, tLed2 = 0, tLed3 = 0, tLed4 = 0;

	uint32_t now = HAL_GetTick();
	if ((now - tLed1) >= 250) {
		tLed1 = now;
		HAL_GPIO_TogglePin(GPIOD, Led1_Pin);
	}
	if ((now - tLed2) >= 500) {
		tLed2 = now;
		HAL_GPIO_TogglePin(GPIOD, Led2_Pin);
	}
	if ((now - tLed3) >= 750) {
		tLed3 = now;
		HAL_GPIO_TogglePin(GPIOD, Led3_Pin);
	}
	if ((now - tLed4) >= 1000) {
		tLed4 = now;
		HAL_GPIO_TogglePin(GPIOD, Led4_Pin);
	}
}

void Auto_Fantastico(void){
	static int8_t AfPos = 0;
	static int8_t AfDir = 1;
	static uint32_t AutoFantastico = 0;

	uint32_t now = HAL_GetTick();
	if (now - AutoFantastico >= 500) {
		AutoFantastico = now;

		HAL_GPIO_WritePin(GPIOD, Led1_Pin, (AfPos == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Led2_Pin, (AfPos == 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Led3_Pin, (AfPos == 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Led4_Pin, (AfPos == 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);

		AfPos += AfDir;
		if (AfPos == 3) {
			AfDir = -1;
		} else if (AfPos == 0) {
			AfDir = 1;
		}
	}
}

void Contador_Binario(void){
	static int8_t binCounter = 0;
	static uint32_t tBinCounter = 0;

	uint32_t now = HAL_GetTick();
	if (now - tBinCounter >= 1000) {
		tBinCounter = now;

		HAL_GPIO_WritePin(GPIOD, Led4_Pin, (binCounter & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Led3_Pin, (binCounter & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Led2_Pin, (binCounter & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, Led1_Pin, (binCounter & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);

		binCounter = (binCounter + 1) & 0xF;
	}
}
