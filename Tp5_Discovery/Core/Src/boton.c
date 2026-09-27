/*
 * boton.c
 *
 *  Created on: 11 sept 2026
 *      Author: aguss
 */

#include "boton.h"
#include "usart.h"
#include <string.h>

static ButtonCallback_t cb_s1 = NULL;
static ButtonCallback_t cb_s2 = NULL;
static ButtonCallback_t cb_s3 = NULL;

// mensajes
char msg_presionado[] = "Pulsador S3 presionado\r\n";
char msg_liberado[] = "Pulsador S3 liberado\r\n";

void Button_Init(void){
	cb_s1 = NULL;
	cb_s2 = NULL;
	cb_s3 = NULL;
}

void Button_SetAction_S1(ButtonCallback_t callback) { cb_s1 = callback; }
void Button_SetAction_S2(ButtonCallback_t callback) { cb_s2 = callback; }
void Button_SetAction_S3(ButtonCallback_t callback) { cb_s3 = callback; }

void Read_Button_Task(void){
	static uint32_t t_debounce_s1 = 0, t_debounce_s2 = 0, t_debounce_s3 = 0;
	static GPIO_PinState ant_s1 = GPIO_PIN_SET, ant_s2 = GPIO_PIN_SET, ant_s3 = GPIO_PIN_SET;
	uint32_t now = HAL_GetTick();

	// Boton S1
	GPIO_PinState act_s1 = HAL_GPIO_ReadPin(GPIOC, BTN_S1_Pin);
	if (act_s1 != ant_s1 && (now - t_debounce_s1) > 50) {
		t_debounce_s1 = now;
		ant_s1 = act_s1;

		if (act_s1 == GPIO_PIN_RESET) {
			if (cb_s1 != NULL) {
				cb_s1(); // Dispara la funcion del boton S1
			}
		}
	}

	// Boton S2
	GPIO_PinState act_s2 = HAL_GPIO_ReadPin(GPIOC, BTN_S2_Pin);
	if (act_s2 != ant_s2 && (now - t_debounce_s2) > 50) {
		t_debounce_s2 = now;
		ant_s2 = act_s2;

		if (act_s2 == GPIO_PIN_RESET) {
			if (cb_s2 != NULL) {
				cb_s2(); // Dispara la funcion del boton S2
			}
		}
	}

	// Boton S3
	GPIO_PinState act_s3 = HAL_GPIO_ReadPin(GPIOA, BTN_S3_Pin);
	if (act_s3 != ant_s3 && (now - t_debounce_s3) > 50) {
		t_debounce_s3 = now;
		ant_s3 = act_s3;

		if (act_s3 == GPIO_PIN_RESET) {
			HAL_UART_Transmit(&huart2, (uint8_t*)msg_presionado, strlen(msg_presionado), 100);

			 if (cb_s3 != NULL) {
				cb_s3(); // Dispara la funcion del boton S3
			}
		} else if (act_s3 == GPIO_PIN_SET) {
			HAL_UART_Transmit(&huart2, (uint8_t*)msg_liberado, strlen(msg_liberado), 100);
		}
	}
}
