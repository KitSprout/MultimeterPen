/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32x_bsp.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32L0XX_BSP_H
#define __STM32L0XX_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32x_system.h"
#include "kitsprout\kSystem.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        bsp_gpio_init( void );
void        bsp_probe_init( void );
void        bsp_timer_init( pFunc event, uint32_t freq );
void        bsp_timer_enable( uint32_t state );
void        bsp_uart_serial_init( pFunc event );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
