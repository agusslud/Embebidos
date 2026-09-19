/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "boton.h"
#include "display7seg.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	MODO_POTENCIOMETRO = 0,
	MODO_TEMPERATURA = 1
} ModoLectura_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MUESTRAS_FILTRO	16
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile ModoLectura_t modo_actual = MODO_POTENCIOMETRO;
volatile uint8_t filtro_activo = 0; // 0 = Medicion directa, 1 = Promedio (16 muestras)

// Variables para ADC1 (Potenciometro)
volatile uint8_t adc1_ready = 0;
uint32_t adc1_value = 0;
uint32_t adc1_acumulador = 0;
uint8_t adc1_muestras = 0;

// Variables para ADC2 (Sensor de temperatura)
volatile uint8_t adc2_ready = 0;
uint32_t adc2_value = 0;
uint32_t adc2_acumulador = 0;
uint8_t adc2_muestras = 0;

// Variables procesadas
float voltage_pot = 0.0f;
float voltage_temp = 0.0f;
float temperature_c = 0.0f;

uint8_t rx_data = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Accion_Boton_S1(void);
void Accion_Boton_S2(void);
void Accion_Boton_S3(void);
void Muestra_Periodica(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  Button_Init();
  Button_SetAction_S1(Accion_Boton_S1);
  Button_SetAction_S2(Accion_Boton_S2);
  Button_SetAction_S3(Accion_Boton_S3);

  HAL_TIM_Base_Start_IT(&htim2);

  HAL_UART_Receive_IT(&huart2, &rx_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Read_Button_Task();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  static uint32_t t_telemetria = 0;
	  uint32_t now = HAL_GetTick();

	  // Potenciometro
	  if (adc1_ready) {
		  adc1_ready = 0;

		  if (filtro_activo) {
			  adc1_acumulador += adc1_value;
			  adc1_muestras++;

			  if (adc1_muestras >= MUESTRAS_FILTRO) {
				  uint32_t prom = adc1_acumulador / MUESTRAS_FILTRO;
				  adc1_acumulador = 0;
				  adc1_muestras = 0;

				  voltage_pot = ((float)prom * 3.3f) / 4095.0f;
			  }
		  } else {
			  voltage_pot = ((float)adc1_value * 3.3f) / 4095.0f;
		  }
	  }

	  // Sensor de temperatura
	  if (adc2_ready) {
		  adc2_ready = 0;

		  if (filtro_activo) {
			  adc2_acumulador += adc2_value;
			  adc2_muestras++;

			  if (adc2_muestras >= MUESTRAS_FILTRO) {
				  uint32_t prom = adc2_acumulador / MUESTRAS_FILTRO;
				  adc2_acumulador = 0;
				  adc2_muestras = 0;

				  voltage_temp = ((float)prom * 3.3f) / 4095.0f;
				  // temperature_c = (voltage_temp) * 100.0f; // Formula LM35
				  temperature_c = (voltage_temp - 0.6f) * 100.0f; // Formula MCP9700
			}
		} else {
			voltage_temp = ((float)adc2_value * 3.3f) / 4095.0f;
			// temperature_c = (voltage_temp) * 100.0f; // Formula LM35
			temperature_c = (voltage_temp - 0.6f) * 100.0f; // Formula MCP9700
		}
	}

	  if (modo_actual == MODO_POTENCIOMETRO) {
		  Display_SetNumberWithDP((uint16_t)(voltage_pot * 1000.0f), 3);
	} else {
		Display_SetNumberWithDP((uint16_t)(temperature_c * 10.0f), 1);
	}

	  if ((now - t_telemetria) >= 2000) {
		  t_telemetria = now;

		  Muestra_Periodica();
	  }

	  Display_Refresh_Task();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// Esta interrupcion ocurre cada 100 ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2) {
		HAL_ADC_Start_IT(&hadc1); // Potenciometro (PA0)
		HAL_ADC_Start_IT(&hadc2); // Sensor de Temperatura (PA1)
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	if (hadc->Instance == ADC1) {
		adc1_value = HAL_ADC_GetValue(hadc);
		adc1_ready = 1;
	} else if (hadc->Instance == ADC2) {
		adc2_value = HAL_ADC_GetValue(hadc);
		adc2_ready = 1;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART2) {
		switch (rx_data){
		case '1': HAL_GPIO_TogglePin(GPIOD, LD3_Pin); break;
		case '2': HAL_GPIO_TogglePin(GPIOD, LD4_Pin); break;
		case '3': HAL_GPIO_TogglePin(GPIOD, LD5_Pin); break;
		case '4': HAL_GPIO_TogglePin(GPIOD, LD6_Pin); break;
		default: break;
		}

		HAL_UART_Receive_IT(&huart2, &rx_data, 1);
	}
}

void Accion_Boton_S1(void){ modo_actual = MODO_POTENCIOMETRO; }
void Accion_Boton_S2(void){ modo_actual = MODO_TEMPERATURA; }
void Accion_Boton_S3(void){ filtro_activo = !filtro_activo; }

void Muestra_Periodica(void){
	static char tx_buffer[50];
	sprintf(tx_buffer, "Temperatura actual: %.1f C\r\n", temperature_c);

	HAL_UART_Transmit_IT(&huart2, (uint8_t*)tx_buffer, strlen(tx_buffer));
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
