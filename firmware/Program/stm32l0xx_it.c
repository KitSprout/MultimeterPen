/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32l0xx_it.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32x_system.h"
#include "drivers\stm32l0_adc.h"
#include "drivers\stm32l0_tim.h"
#include "modules\serial.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void NMI_Handler( void ) { while(1); }
void HardFault_Handler( void ) { while(1); }
void SVC_Handler( void ) { while(1); }
void PendSV_Handler( void ) { while(1); }
void SysTick_Handler( void ) { HAL_IncTick(); }

//void WWDG_IRQHandler( void )
//void PVD_IRQHandler( void )
//void RTC_IRQHandler( void )
//void FLASH_IRQHandler( void )
//void RCC_IRQHandler( void )
//void EXTI0_1_IRQHandler( void )
//void EXTI2_3_IRQHandler( void )
//void EXTI4_15_IRQHandler( void )

void DMA1_Channel1_IRQHandler( void )
{
    HAL_DMA_IRQHandler(hadc1.handle->DMA_Handle);
}

//void DMA1_Channel2_3_IRQHandler( void )
//void DMA1_Channel4_5_IRQHandler( void )
//void ADC1_COMP_IRQHandler( void )
//void LPTIM1_IRQHandler( void )

void TIM2_IRQHandler( void )
{
    if (__HAL_TIM_GET_FLAG(htim2.handle, TIM_FLAG_UPDATE) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(htim2.handle, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_IT(htim2.handle, TIM_IT_UPDATE);
            htim2.tickEventCallback();
        }
    }
}

//void TIM21_IRQHandler( void )
//void I2C1_IRQHandler( void )
//void SPI1_IRQHandler( void )

void USART2_IRQHandler( void )
{
#if KS_FW_UART_HAL_LIBRARY
    HAL_UART_IRQHandler(hSerial.handle);
#else
//    if (__HAL_UART_GET_IT_SOURCE(hserial.handle, UART_IT_TXE) != RESET)
//    {
//        __HAL_UART_GET_IT_SOURCE(hserial.handle, UART_IT_TXE);
//        HAL_UART_TxCpltCallback(hserial.handle);
//    }
    if (__HAL_UART_GET_IT_SOURCE(hserial.handle, UART_IT_RXNE) != RESET)
    {
        __HAL_UART_GET_IT_SOURCE(hserial.handle, UART_IT_RXNE);
        HAL_UART_RxCpltCallback(hserial.handle);
    }
#endif
}

//void LPUART1_IRQHandler( void )

/*************************************** END OF FILE ****************************************/
