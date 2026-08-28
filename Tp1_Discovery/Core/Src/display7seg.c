/*
 * display7seg.c
 *
 *  Created on: 25 ago 2026
 *      Author: aguss
 */

#include "display7seg.h"

static const uint8_t Tabla_7Seg[10] = {
		~0x3F, // '0'
		~0x06, // '1'
		~0x5B, // '2'
		~0x4F, // '3'
		~0x66, // '4'
		~0x6D, // '5'
		~0x7D, // '6'
		~0x07, // '7'
		~0x7F, // '8'
		~0x6F  // '9'
};

static const uint8_t Mascara_Digitos[4] = {
		0x08, // Derecha
		0x04,
		0x02,
		0x01 // Izquierda
};

static uint16_t numero_actual = 0;

void Display_Send16Bits(uint8_t byte_segmentos, uint8_t byte_digito){
	HAL_GPIO_WritePin(DISPLAY_LATCH_PORT, DISPLAY_LATCH_PIN, GPIO_PIN_RESET);

	for (int8_t i = 7; i >= 0; i--) {
		HAL_GPIO_WritePin(DISPLAY_CLK_PORT, DISPLAY_CLK_PIN, GPIO_PIN_RESET);

		GPIO_PinState bit_state = ((byte_segmentos >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
		HAL_GPIO_WritePin(DISPLAY_DATA_PORT, DISPLAY_DATA_PIN, bit_state);

		HAL_GPIO_WritePin(DISPLAY_CLK_PORT, DISPLAY_CLK_PIN, GPIO_PIN_SET);
	}

	for (int8_t i = 7; i >= 0; i--) {
		HAL_GPIO_WritePin(DISPLAY_CLK_PORT, DISPLAY_CLK_PIN, GPIO_PIN_RESET);

		GPIO_PinState bit_state = ((byte_digito >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
		HAL_GPIO_WritePin(DISPLAY_DATA_PORT, DISPLAY_DATA_PIN, bit_state);

		HAL_GPIO_WritePin(DISPLAY_CLK_PORT, DISPLAY_CLK_PIN, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(DISPLAY_LATCH_PORT, DISPLAY_LATCH_PIN, GPIO_PIN_SET);
}

void Display_SetNumber(uint16_t valor){
	numero_actual = valor % 10000;
}

void Display_Refresh_Task(void){
	static uint8_t digito_activo = 0;
	static uint32_t t_refresco = 0;
	uint32_t now = HAL_GetTick();

	if ((now - t_refresco) >= 2) {
		t_refresco += 2;

		uint8_t digito_val = 0;

		switch (digito_activo) {
			case 0: digito_val = (numero_actual / 1) % 10; break;
			case 1: digito_val = (numero_actual / 10) % 10; break;
			case 2: digito_val = (numero_actual / 100) % 10; break;
			case 3: digito_val = (numero_actual / 1000) % 10; break;
			default: break;
		}

		Display_Send16Bits(Tabla_7Seg[digito_val], Mascara_Digitos[digito_activo]);

		digito_activo = (digito_activo + 1) % 4;
	}
}

void Display_Contador_Task(void){
	static uint16_t contador = 0;
	static uint32_t t_segundo = 0;
	uint32_t now = HAL_GetTick();

	if ((now - t_segundo) >= 1000) {
		t_segundo += 1000;

		contador++;
		if (contador > 9999) contador = 0;

		Display_SetNumber(contador);
	}
}
