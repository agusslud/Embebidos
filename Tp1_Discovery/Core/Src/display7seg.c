/*
 * display7seg.c
 *
 *  Created on: 25 ago 2026
 *      Author: aguss
 */

#include "display7seg.h"

static const uint8_t Tabla_7Seg[10] = {
		0x3F, // '0'
		0x06, // '1'
		0x5B, // '2'
		0x4F, // '3'
		0x66, // '4'
		0x6D, // '5'
		0x7D, // '6'
		0x07, // '7'
		0x7F, // '8'
		0x6F  // '9'
};

void ShiftRegister_SendByte(uint8_t byte_data){
	HAL_GPIO_WritePin(DISPLAY_LATCH_PORT, DISPLAY_LATCH_PIN, GPIO_PIN_RESET);

	for (uint8_t i = 7; i >= 0; i--) {
		HAL_GPIO_WritePin(DISPLAY_CLK_PORT, DISPLAY_CLK_PIN, GPIO_PIN_RESET);

		if ((byte_data >> i) & 0x01) {
			HAL_GPIO_WritePin(DISPLAY_DATA_PORT, DISPLAY_DATA_PIN, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(DISPLAY_DATA_PORT, DISPLAY_DATA_PIN, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(DISPLAY_CLK_PORT, DISPLAY_CLK_PIN, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(DISPLAY_LATCH_PORT, DISPLAY_LATCH_PIN, GPIO_PIN_SET);
}

void Display_Contador_Task(void){
	static uint8_t contador = 0;
	static uint32_t t_anterior = 0;
	uint32_t now = HAL_GetTick();

	if ((now - t_anterior) >= 1000) {
		t_anterior = now;

		ShiftRegister_SendByte(Tabla_7Seg[contador]);

		contador++;
		if (contador > 9) {
			contador = 0;
		}
	}
}
