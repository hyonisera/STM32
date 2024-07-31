/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
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
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart3;
uint8_t rx_data, rx_complete = 0;
uint8_t rx_index = 0, rx_buf[20];
uint8_t led[3] = {0,0,0};
uint32_t blink_time = 0;

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
  MX_USART3_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart3, &rx_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  cur_time = HAL_GetTick();
//
	  if(rx_complete == 1){
		  rx_complete = 0;

		  if(strstr((char*)rx_buf, "LED") != 0){
			  if(rx_buf[3] == '1'){
				  if(rx_buf[4] == '0') HAL_GPIO_WritePin(GPIOB, LD1_Pin, 0);
				  else if(rx_buf[4] == '1') HAL_GPIO_WritePin(GPIOB, LD1_Pin, 1);

				  led[0] = rx_buf[4] - '0';
			  }
			  else if(rx_buf[3] == '2') {
				  if(rx_buf[4] == '0') HAL_GPIO_WritePin(GPIOB, LD2_Pin, 0);
				  else if(rx_buf[4] == '1') HAL_GPIO_WritePin(GPIOB, LD2_Pin, 1);

				  led[1] = rx_buf[4] - '0';
			  }
			  else if(rx_buf[3] == '3') {
				  if(rx_buf[4] == '0') HAL_GPIO_WritePin(GPIOB, LD3_Pin, 0);
				  else if(rx_buf[4] == '1') HAL_GPIO_WritePin(GPIOB, LD3_Pin, 1);

				  led[2] = rx_buf[4] - '0';
			  }
			  else if(rx_buf[3] == 'A'){
				  if(rx_buf[4] == '0') HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD2_Pin | LD3_Pin, 0);
				  else if(rx_buf[4] == '1') HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD2_Pin | LD3_Pin, 1);
			  }
		  }
		  else if(strstr((char*)rx_buf, "BLK") != 0){
			  blink_time = (rx_buf[3] - '0') * 500;
		  }

	  }

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3){
		uint8_t ch;

		ch = rx_data;

		if(ch == '\n' || ch == '\r'){
			rx_buf[rx_index] = 0;
			rx_index = 0;
			rx_complete = 1;

			char *str = "\r\n>> ";
			HAL_UART_Transmit_IT(&huart3, (uint8_t *)str, strlen(str));
			//char *str1 = "aaaaaaaaaaaa";
			//HAL_UART_Transmit_IT(&huart3, (uint8_t *)str1, strlen(str1));
		}
		else if(ch == '\b'){
			if(rx_index != 0){
				rx_index--;
				char *str = "\b \b";
				HAL_UART_Transmit_IT(&huart3, (uint8_t *)str, strlen(str));
			}
		}
		else {
			rx_buf[rx_index++] = ch;
			HAL_UART_Transmit_IT(&huart3, &ch, 1);
		}


		HAL_UART_Receive_IT(&huart3, &rx_data, 1);
	}
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

#ifdef  USE_FULL_ASSERT
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
