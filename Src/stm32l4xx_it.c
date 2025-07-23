/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32l4xx_it.h"
#include "usart.h"

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
   while (1)
  {
  }

}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{

  while (1)
  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{

  while (1)
  {
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{

  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
	
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

/**
  * @brief This function handles DMA1 channel7 global interrupt.
  */
void DMA1_Channel7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
	// When complete sending data, invoke on_complete_transfer
	if ((USART2->ISR & USART_ISR_RXNE) || (USART2->ISR & USART_ISR_ORE)){
		USART2->ICR |= USART_ICR_ORECF;  // Clear overrun
    uint8_t ch = USART2->RDR;
		
		// trigger motor
		UART_motor_control(&huart2, ch);
	} 
	uint8_t ch = USART2->RDR;
	
	HAL_UART_IRQHandler(&huart2);
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart3);
	
	if (((USART3->ISR) & USART_ISR_RXNE) | (USART3->ISR & USART_ISR_ORE)){
		USART3->ICR |= USART_ICR_ORECF;
		uint8_t ch = USART3->RDR;
		
		// trigger motor
		UART_motor_control(&huart3, ch);
	}
	
	// When complete sending data, invoke on_complete_transfer
	if ((USART3->ISR)&USART_ISR_TC){
		USART3->ICR |= USART_ICR_TCCF;  					// Clear the TC flag
		DMA1_Channel2->CCR &=~ DMA_CCR_EN; 				// Clearing the flag
		
		// clear pending register
		on_complete_transfer();
	}
	
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(B1_Pin);
}
