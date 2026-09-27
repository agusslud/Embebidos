/*
 * uart_fsm.c
 *
 *  Created on: 24 sept 2026
 *      Author: agust
 */

#include "uart_fsm.h"
#include "display7seg.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RX_BUFFER_SIZE 64

static UART_State_t currentState = STATE_WAIT_START;
static char rxBuffer[RX_BUFFER_SIZE];
static uint8_t rxIndex = 0;

extern volatile ModoLectura_t modo_actual;
extern float temperature_c;

static uint8_t Calcular_Checksum_XOR(const char *p_inicio, size_t len) {
	uint8_t cs = 0;
	for (int i = 0; i < len; i++) {
		cs ^= (uint8_t) p_inicio[i];
	}
	return cs;
}

static void Enviar_Respuesta_UART(const char *tipo, const char *payload) {
	char msg_temp[64];
	char tx_buffer[80];

	// Formatear contenido
	sprintf(msg_temp, "%s,%s", tipo, payload);

	// Calcular checksum
	uint8_t cs_resp = Calcular_Checksum_XOR(msg_temp, strlen(msg_temp));

	// Formatear trama final
	sprintf(tx_buffer, "$%s*%02X\r\n", msg_temp, cs_resp);

	HAL_UART_Transmit(&huart2, (uint8_t*) tx_buffer, strlen(tx_buffer), 50);
}

void UART_FSM_Init(void) {
	currentState = STATE_WAIT_START;
	rxIndex = 0;
}

void UART_FSM_Update(char incomingByte) {
	switch (currentState) {
	case STATE_WAIT_START:
		if (incomingByte == '#' || incomingByte == '$') {
			rxIndex = 0;
			rxBuffer[rxIndex++] = incomingByte;
			currentState = STATE_ACCUMULATE;
		}
		break;

	case STATE_ACCUMULATE:
		if (incomingByte == '\n') {
			rxBuffer[rxIndex] = '\0';
			currentState = STATE_PROCESS;
		} else if (incomingByte != '\r') {
			if (rxIndex < (RX_BUFFER_SIZE - 1)) {
				rxBuffer[rxIndex++] = incomingByte;
			} else {
				currentState = STATE_WAIT_START;
				rxIndex = 0;
			}
		}
		break;

	case STATE_PROCESS:
		break;
	}
}

void UART_FSM_Process_Task(void) {
	if (currentState == STATE_PROCESS) {
		// PARTE 3: PROTOCOLO ROBUSTO CON CHECKSUM ($HEADER,COMANDO*CS)
		if (rxBuffer[0] == '$') {
			char *ptr_asterisco = strchr(rxBuffer, '*');

			if (ptr_asterisco != NULL) {
				// 1. Extraer el Checksum en Hexadecimal enviado por la PC
				unsigned int cs_recibido_temp = 0;
				sscanf(ptr_asterisco + 1, "%2X", &cs_recibido_temp);
				uint8_t cs_recibido = (uint8_t) cs_recibido_temp;

				// 2. Calcular Checksum del contenido entre '$' y '*'
				size_t longitud_payload = ptr_asterisco - (rxBuffer + 1);
				uint8_t cs_calculado = Calcular_Checksum_XOR(rxBuffer + 1, longitud_payload);

				// 3. Validar Integridad por Checksum
				if (cs_recibido != cs_calculado) {
					// Error de Checksum: Enviar NACK
					Enviar_Respuesta_UART("NACK", "ERR_CHECKSUM");
				} else {
					// Checksum Válido: Parsear comandos
					int led_num = 0, led_estado = 0;

					// Ejemplo: $SET,LED,2,1*3E
					if (sscanf(rxBuffer + 1, "SET,LED,%d,%d", &led_num, &led_estado) == 2) {
						if (led_num == 2) {
							HAL_GPIO_WritePin(GPIOD, LD4_Pin,
									(led_estado == 1) ?
											GPIO_PIN_SET : GPIO_PIN_RESET);
							Enviar_Respuesta_UART("ACK", "SET,LED,2");
						} else {
							Enviar_Respuesta_UART("NACK", "ERR_CMD_UNKNOWN");
						}
					} else {
						// Comando no reconocido en el diccionario
						Enviar_Respuesta_UART("NACK", "ERR_CMD_UNKNOWN");
					}
				}
			}
		}
		// PARTES 1 Y 2: COMANDOS SIMPLES (#COMANDO,PARAMETRO)
		else if (rxBuffer[0] == '#') {
			int param = 0;
			float val_disp = 0.0f;

			if (sscanf(rxBuffer, "#LED,%d", &param) == 1) {
				if (param == 1) {
					HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
				} else if (param == 0) {
					HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);
				}
			} else if (sscanf(rxBuffer, "#TOG,%d", &param) == 1) {
				if (param == 3) {
					HAL_GPIO_TogglePin(GPIOD, LD5_Pin);
				}
			} else if (sscanf(rxBuffer, "#DISP,SET,%f", &val_disp) == 1) {
				Display_SetNumberWithDP((uint16_t) (val_disp * 10.0f), 1);
				modo_actual = MODO_REMOTO_UART;
			} else if (strcmp(rxBuffer, "#GET,TEMP") == 0) {
				char tx_resp[50];

				sprintf(tx_resp, "#VAL,TEMP,%.1f\n", temperature_c);

				HAL_UART_Transmit(&huart2, (uint8_t*)tx_resp, strlen(tx_resp), 100);
			}
		}

		currentState = STATE_WAIT_START;
	}
}
