/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32l0_adc.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32L0_ADC_H
#define __STM32L0_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32x_system.h"

/* Define ----------------------------------------------------------------------------------*/

#define ADC_CONV        (0.5f)  // R = R2/(R1+R2) = 0.5, 3300mV / (2^12 * R)
#define VREFINT_CAL     *(__IO uint16_t *)(0x1FF80078)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct
{
    ADC_HandleTypeDef *handle;
    uint16_t *convBuffer;

} __attribute__((aligned)) AdcHandle_st;

/* Extern ----------------------------------------------------------------------------------*/
extern AdcHandle_st hadc1;

/* Functions -------------------------------------------------------------------------------*/

void        ADC_Config( void );
uint16_t    ADC_GetValue( uint8_t index );
void        ADC_GetAveValue( uint16_t *aveAdc );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
