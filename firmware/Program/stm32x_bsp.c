/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32x_bsp.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "kitsprout\kSystem.h"
#include "drivers\stm32l0_adc.h"
#include "drivers\stm32l0_tim.h"
#include "modules\serial.h"
#include "stm32x_bsp.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void bsp_gpio_init( void )
{
    HAL_Init();
    SystemClock_Config();

    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable all GPIO Clk *******************************************************/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* GPIO all analog input *****************************************************/
    GPIO_InitStruct.Pin   = GPIO_PIN_All;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin   = GPIO_PIN_All & (~(GPIO_PIN_13 | GPIO_PIN_14));
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* GPIO Pin ******************************************************************/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin   = LED_PIN;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = BUZ_PIN;
    HAL_GPIO_Init(BUZ_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin   = KEY_PIN;
    HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

    LED_Off();
}

void bsp_probe_init( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin   = PROBE_OUT_PIN;
    HAL_GPIO_Init(PROBE_OUT_GPIO_PORT, &GPIO_InitStruct);
    PROBE_OUT_Off();

    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin   = PROBE_EXT1_PIN;
    HAL_GPIO_Init(PROBE_EXT1_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = PROBE_EXT2_PIN;
    HAL_GPIO_Init(PROBE_EXT2_GPIO_PORT, &GPIO_InitStruct);

    ADC_Config();
}

void bsp_timer_init( pFunc event, uint32_t freq )
{
    htim2.tickEventCallback = event;
    Timer2_Config(TIMx_CLOCK(freq, 1000), 1000);
}

void bsp_timer_enable( uint32_t state )
{
    Timer2_Cmd(state);
}

void bsp_uart_serial_init( pFunc event )
{
    hserial.rxEventCallback = event;

    Serial_Config();
    printf("\r\n\f\r\n");
}

/*************************************** END OF FILE ****************************************/
