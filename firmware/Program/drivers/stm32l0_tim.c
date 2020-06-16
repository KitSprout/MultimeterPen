/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32l0_tim.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32l0_tim.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static TIM_HandleTypeDef htim_tim2;

TimHandle_st htim2 =
{
    .handle            = &htim_tim2,
    .tickEventCallback = NULL,
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  delay_nop
 */
void Timer2_Config( uint32_t prescaler, uint32_t period )
{
    /* TIMX Clk ******************************************************************/
    TIMER2_CLK_ENABLE();

    /* NVIC Config ***************************************************************/
    HAL_NVIC_SetPriority(TIMER2_IRQn, TIMER2_TIMx_IRQn_PREEMPT, TIMER2_TIMx_IRQn_SUB);
    HAL_NVIC_EnableIRQ(TIMER2_IRQn);

    /* TIM Base Config ************************************************************/
    htim2.handle->Instance                = TIMER2;
    htim2.handle->Init.Prescaler          = prescaler - 1;
    htim2.handle->Init.Period             = period - 1;
    htim2.handle->Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
    htim2.handle->Init.CounterMode        = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(htim2.handle);

    /* TIM Enable *****************************************************************/
//    HAL_TIM_Base_Start_IT(htim2.handle);
}

/**
 *  @brief  delay_nop
 */
void Timer2_Cmd( uint32_t status )
{
    if (status == ENABLE)
    {
        HAL_TIM_Base_Start_IT(htim2.handle);
    }
    else
    {
        HAL_TIM_Base_Stop_IT(htim2.handle);
    }
}

/*************************************** END OF FILE ****************************************/
