/*
 * boton.h
 *
 *  Created on: 11 sept 2026
 *      Author: aguss
 */

#ifndef INC_BOTON_H_
#define INC_BOTON_H_

#include "main.h"

typedef void (*ButtonCallback_t)(void);

// Inicializador y asignador de acciones
void Button_Init(void);
void Button_SetAction_S1(ButtonCallback_t callback);
void Button_SetAction_S2(ButtonCallback_t callback);
void Button_SetAction_S3(ButtonCallback_t callback);

// Tarea no bloqueante para procesar rebotes
void Read_Button_Task(void);

#endif /* INC_BOTON_H_ */
