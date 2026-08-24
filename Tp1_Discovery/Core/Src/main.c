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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Leds_Temporizados(void);
void Auto_Fantastico(void);
void Contador_Binario(void);
void Read_Button_Task(void);
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
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  /* USER CODE BEGIN 3 */
/* Parte A ------------------------------------------------------------------*/
	  /* HAL_GPIO_TogglePin(GPIOA, LD1_Pin);
	  	  HAL_GPIO_TogglePin(GPIOA, LD2_Pin);
	  	  HAL_GPIO_TogglePin(GPIOA, LD3_Pin);
	  	  HAL_GPIO_TogglePin(GPIOB, LD4_Pin);

	  	  HAL_Delay(200);*/
/* Parte B ------------------------------------------------------------------*/
	  // Leds_Temporizados();

/* Parte C ------------------------------------------------------------------*/
	  // Auto_Fantastico();
	  // Contador_Binario();

/* Modulo 3 ------------------------------------------------------------------*/
	  Read_Button_Task();
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
void Leds_Temporizados(void){
	static uint32_t tLed1 = 0, tLed2 = 0, tLed3 = 0, tLed4 = 0;

	uint32_t now = HAL_GetTick();
	if ((now - tLed1) >= 250) {
		tLed1 = now;
		HAL_GPIO_TogglePin(GPIOD, LD6_Pin);
	}
	if ((now - tLed2) >= 500) {
		tLed2 = now;
		HAL_GPIO_TogglePin(GPIOD, LD4_Pin);
	}
	if ((now - tLed3) >= 750) {
		tLed3 = now;
		HAL_GPIO_TogglePin(GPIOD, LD3_Pin);
	}
	if ((now - tLed4) >= 1000) {
		tLed4 = now;
		HAL_GPIO_TogglePin(GPIOD, LD5_Pin);
	}
}
void Auto_Fantastico(void){
	static int8_t AfPos = 0;
	static int8_t AfDir = 1;
	static uint32_t AutoFantastico = 0;

	uint32_t now = HAL_GetTick();
	if (now - AutoFantastico >= 500) {
		AutoFantastico = now;

		HAL_GPIO_WritePin(GPIOD, LD6_Pin, (AfPos == 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, (AfPos == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, (AfPos == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, (AfPos == 3) ? GPIO_PIN_SET : GPIO_PIN_RESET);

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

		HAL_GPIO_WritePin(GPIOD, LD6_Pin, (binCounter & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, (binCounter & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, (binCounter & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, (binCounter & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

		binCounter = (binCounter + 1) & 0xF;
	}
}
void Read_Button_Task(void){
	GPIO_PinState estado_actual = HAL_GPIO_ReadPin(GPIOA, B1_Pin);

	static GPIO_PinState estado_anterior = GPIO_PIN_RESET;
	static GPIO_PinState estado_valido = GPIO_PIN_RESET;
	static uint32_t tiempo_ultimo_cambio = 0;

	if (estado_actual != estado_anterior) {
		tiempo_ultimo_cambio = HAL_GetTick();
	}
	if ((HAL_GetTick() - tiempo_ultimo_cambio) > 50) {
		if (estado_actual != estado_valido) {
			estado_valido = estado_actual;
		}
	}

	if (estado_valido == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_SET);
	}else {
		HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_RESET);
	}
	estado_anterior = estado_actual;
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
