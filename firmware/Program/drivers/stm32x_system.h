/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32x_system.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32X_SYSTEM_H
#define __STM32X_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/

#include "stm32l0xx_hal.h"
#include "stm32l011xx.h"

#include "boardConfig.h"
#include "stm32l0_delay.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/

#define __GPIO_SET(__PORT, __PIN)           ((__PORT)->BSRR = (__PIN))
#define __GPIO_RST(__PORT, __PIN)           ((__PORT)->BRR = (__PIN))
#define __GPIO_TOG(__PORT, __PIN)           ((__PORT)->ODR ^= (__PIN))
#define __GPIO_READ(__PORT, __PIN)          ((__PORT)->IDR & (__PIN))

/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/

extern void SystemClock_Config( void );

/* Functions -------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
