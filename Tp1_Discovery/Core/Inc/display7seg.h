/*
 * display7seg.h
 *
 *  Created on: 25 ago 2026
 *      Author: aguss
 */

#ifndef INC_DISPLAY7SEG_H_
#define INC_DISPLAY7SEG_H_

#include "main.h"

#define DISPLAY_LATCH_PORT	GPIOD
#define DISPLAY_LATCH_PIN	GPIO_PIN_9

#define DISPLAY_CLK_PORT	GPIOD
#define DISPLAY_CLK_PIN	GPIO_PIN_10

#define DISPLAY_DATA_PORT	GPIOD
#define DISPLAY_DATA_PIN	GPIO_PIN_11

void ShiftRegister_SendByte(uint8_t byte_data);
void Display_Contador_Task(void);

#endif /* INC_DISPLAY7SEG_H_ */
