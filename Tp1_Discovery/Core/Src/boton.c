/*
 * boton.c
 *
 *  Created on: 27 ago 2026
 *      Author: agust
 */

#include "boton.h"
#include "display7seg.h"

uint8_t modo_efecto_leds = 0;

void Read_Button_Task(void){
	static uint32_t t_debounce_s1 = 0, t_debounce_s2 = 0, t_debounce_s3 = 0;
	static GPIO_PinState ant_s1 = GPIO_PIN_SET, ant_s2 = GPIO_PIN_SET, ant_s3 = GPIO_PIN_SET;
	uint32_t now = HAL_GetTick();

	GPIO_PinState act_s1 = HAL_GPIO_ReadPin(GPIOC, BTN_1_Pin);
	if (act_s1 != ant_s1 && (now - t_debounce_s1) > 50) {
		t_debounce_s1 = now;
		ant_s1 = act_s1;

		if (act_s1 == GPIO_PIN_RESET) {
			Display_SetNumber(0);
		}
	}

	GPIO_PinState act_s2 = HAL_GPIO_ReadPin(GPIOC, BTN_2_Pin);
	if (act_s2 != ant_s2 && (now - t_debounce_s2) > 50) {
		t_debounce_s2 = now;
		ant_s2 = act_s2;

		if (act_s2 == GPIO_PIN_RESET) {
			modo_efecto_leds = 1;
		}
	}

	GPIO_PinState act_s3 = HAL_GPIO_ReadPin(GPIOA, BTN_3_Pin);
	if (act_s3 != ant_s3 && (now - t_debounce_s3) > 50) {
		t_debounce_s3 = now;
		ant_s3 = act_s3;
		if (act_s3 == GPIO_PIN_RESET) {
			modo_efecto_leds = 2;
		}
	}
}
