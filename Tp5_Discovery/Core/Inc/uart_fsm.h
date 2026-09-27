/*
 * uart_fsm.h
 *
 *  Created on: 24 sept 2026
 *      Author: agust
 */

#ifndef INC_UART_FSM_H_
#define INC_UART_FSM_H_

#include "main.h"

typedef enum {
	STATE_WAIT_START, STATE_ACCUMULATE, STATE_PROCESS
} UART_State_t;

void UART_FSM_Init(void);
void UART_FSM_Update(char incomingByte);
void UART_FSM_Process_Task(void); // Se llama dentro del while(1)

#endif /* INC_UART_FSM_H_ */
