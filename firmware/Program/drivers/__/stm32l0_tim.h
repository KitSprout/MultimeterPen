/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32l4_tim.h
 *  @author  KitSprout
 *  @date    Apr-2019
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32L4_TIM_H
#define __STM32L4_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define TIMx_CLOCK( __PRES, __PERIOD )    (SystemCoreClock / (__PRES * __PERIOD))

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct
{
    TIM_HandleTypeDef *handle;
    pFunc tickEventCallback;

} TimHandle_st;

/* Variables -------------------------------------------------------------------------------*/
extern TimHandle_st htim2;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        Timer2_Config( uint32_t prescaler, uint32_t period );
void        Timer2_Cmd( uint32_t status );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
