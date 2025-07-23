/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>

static char buffer[IO_SIZE];

static uint8_t rx_data2;
static uint8_t rx_data3;

void SystemClock_Config(void);

void UART_motor_control(UART_HandleTypeDef *huart, uint8_t rx_data){
	 if (huart == &huart2){
    if (rx_data == 'B' || rx_data == 'b')  //'B' triggers bloom
    {
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 2650);  // Servo rotates to ~180°
    }
		else if (rx_data == 'C' || rx_data == 'c')  //'C' closes flower
    {
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 700);  // Servo back to ~0°
    }
		else
		{
				//print error message
				sprintf(buffer, "Command not recieved. Type 'b' or 'c'.\n");
				UART_print(buffer);
		}
		
    // Continue UART listening
		HAL_UART_Receive_IT(&huart2, &rx_data2, 1);
  }
	else if (huart == &huart3){
    if (rx_data3 == 'B' || rx_data3 == 'b')  //'B' triggers bloom
    {
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 2650);  // Servo rotates to ~180°
    }
		else if (rx_data3 == 'C' || rx_data3 == 'c')  //'C' closes flower
    {
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 700);  // Servo back to ~0°
    }
		else
		{
				//print error message
				sprintf(buffer, "Command not recieved. Type 'b' or 'c'.\n");
				UART_print(buffer);
		}
		
    // Continue UART listening
		HAL_UART_Receive_IT(&huart2, &rx_data3, 1);
  }
	else
	{
		//print error message
		sprintf(buffer, "Command not recieved. Use USART2/USART3.\n");
		UART_print(buffer);
		return;
	}
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  //MX_USART3_UART_Init();
	
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	
	// Start receiving via interrupt
  HAL_UART_Receive_IT(&huart2, &rx_data2, 1);
	//HAL_UART_Receive_IT(&huart3, &rx_data3, 1);
	
  while (1)
  {
		//check if termite is recieving
		sprintf(buffer, "termite recieving: %c\r\n", rx_data2);
		UART_print(buffer);
		
		//check if bluetooth is recieving
		//sprintf(buffer, "bluetooth recieving: %c\n", rx_data3);
		//UART_print(buffer);
		
		/*
		//writes to CCR1 and set the pulse width for the PWM signal
		//Set the compare value (pulse width) of TIM2, Channel 1 to 2650 timer ticks
		//if ticks = 2700, full 360 turn
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 2650); //full 180°
		//delay function
		HAL_Delay(3000);
		
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 700); //full 0°
		HAL_Delay(1000);
		*/
  }

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
