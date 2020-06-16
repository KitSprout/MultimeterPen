/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32l0_adc.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32x_system.h"
#include "stm32l0_adc.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static DMA_HandleTypeDef hadc_dma;
static __IO uint16_t ADC_CONV_BUFF[ADC_MAX_SAMPLEBUF][ADC_MAX_CHANNEL + 1] = {0};
static ADC_HandleTypeDef huart_adc1;
AdcHandle_st hadc1 =
{
    .handle     = &huart_adc1,
    .convBuffer = (uint16_t*)ADC_CONV_BUFF,
};

/* Prototypes ------------------------------------------------------------------------------*/
/* MSP Functions ---------------------------------------------------------------------------*/

void HAL_ADC_MspInit( ADC_HandleTypeDef *hadc )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (hadc->Instance == ADCx)
    {
        /* ADC and DMA Clk ***********************************************************/
        ADCx_CLK_ENABLE();
        ADCx_DMA_CLK_ENABLE();

        /* ADC Pin *******************************************************************/
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;

        GPIO_InitStruct.Pin  = ADCx_PROBE_CHANNEL_PIN;
        HAL_GPIO_Init(ADCx_PROBE_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = ADCx_BATT_CHANNEL_PIN;
        HAL_GPIO_Init(ADCx_BATT_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

        /* DMA Init ******************************************************************/
        hadc_dma.Instance                 = ADCx_DMA_CHANNEL;
        hadc_dma.Init.Request             = DMA_REQUEST_0;
        hadc_dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hadc_dma.Init.PeriphInc           = DMA_PINC_DISABLE;
        hadc_dma.Init.MemInc              = DMA_MINC_ENABLE;
        hadc_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hadc_dma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hadc_dma.Init.Mode                = DMA_CIRCULAR;
        hadc_dma.Init.Priority            = DMA_PRIORITY_LOW;
        HAL_DMA_Init(&hadc_dma);

        __HAL_LINKDMA(hadc, DMA_Handle, hadc_dma);

        /* DMA IT ********************************************************************/
        HAL_NVIC_SetPriority(ADCx_DMA_IRQn, ADCx_DMA_IRQn_PREEMPT, ADCx_DMA_IRQn_SUB);
        HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
    }
}

/**
 *  @brief ADC MSP De-Initialization
 */
void HAL_ADC_MspDeInit( ADC_HandleTypeDef *hadc )
{
    if (hadc->Instance == ADCx)
    {
        ADCx_CLK_DISABLE();
        HAL_GPIO_DeInit(ADCx_PROBE_CHANNEL_GPIO_PORT, ADCx_PROBE_CHANNEL_PIN);
        HAL_GPIO_DeInit(ADCx_BATT_CHANNEL_GPIO_PORT, ADCx_BATT_CHANNEL_PIN);
    }
}

/**
 *  @brief  Regular conversion complete callback in non blocking mode 
 */
void HAL_ADC_ConvCpltCallback( ADC_HandleTypeDef* hadc )
{
    if (hadc->Instance == ADCx)
    {
        // ...
    }
}

/**
  * @brief  Regular conversion half DMA transfer callback in non blocking mode 
  */
void HAL_ADC_ConvHalfCpltCallback( ADC_HandleTypeDef* hadc )
{
    if (hadc->Instance == ADCx)
    {
        // ...
    }
}

/**
 *  @brief  Analog watchdog callback in non blocking mode 
 */
void HAL_ADC_LevelOutOfWindowCallback( ADC_HandleTypeDef* hadc )
{
    if (hadc->Instance == ADCx)
    {
        // ...
    }
}

/**
 *  @brief  Error ADC callback.
 */
void HAL_ADC_ErrorCallback( ADC_HandleTypeDef *hadc )
{
    if (hadc->Instance == ADCx)
    {
        // ...
    }
}

/**
 *  @brief  Injected conversion complete callback in non blocking mode 
 */
void HAL_ADCEx_InjectedConvCpltCallback( ADC_HandleTypeDef* hadc )
{
    if (hadc->Instance == ADCx)
    {
        // ...
    }
}

/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  ADC_Config
 */
void ADC_Config( void )
{
    ADC_ChannelConfTypeDef ADC_ChannelConfStruct;

    /* ADC Init ******************************************************************/
    huart_adc1.Instance                   = ADCx;
    huart_adc1.Init.OversamplingMode      = DISABLE;
    huart_adc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    huart_adc1.Init.Resolution            = ADC_RESOLUTION_12B;
    huart_adc1.Init.SamplingTime          = ADCx_SAMPLETIME;
    huart_adc1.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
    huart_adc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    huart_adc1.Init.ContinuousConvMode    = ENABLE;
    huart_adc1.Init.DiscontinuousConvMode = DISABLE;
    huart_adc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    huart_adc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    huart_adc1.Init.DMAContinuousRequests = ENABLE;
    huart_adc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    huart_adc1.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
    huart_adc1.Init.LowPowerAutoWait      = DISABLE;
    huart_adc1.Init.LowPowerFrequencyMode = DISABLE;
    huart_adc1.Init.LowPowerAutoPowerOff  = DISABLE;
    if (HAL_ADC_Init(&huart_adc1) != HAL_OK)
    {
        while (1) {;}
    }

    /* ADC Channel Init **********************************************************/
    ADC_ChannelConfStruct.Rank      = ADC_RANK_CHANNEL_NUMBER;

    ADC_ChannelConfStruct.Channel   = ADCx_BATT_CHANNEL;
    if (HAL_ADC_ConfigChannel(&huart_adc1, &ADC_ChannelConfStruct) != HAL_OK)
    {
        while (1) {;}
    }

    ADC_ChannelConfStruct.Channel   = ADCx_PROBE_CHANNEL;
    if (HAL_ADC_ConfigChannel(&huart_adc1, &ADC_ChannelConfStruct) != HAL_OK)
    {
        while (1) {;}
    }

    ADC_ChannelConfStruct.Channel   = ADC_CHANNEL_VREFINT;
    if (HAL_ADC_ConfigChannel(&huart_adc1, &ADC_ChannelConfStruct) != HAL_OK)
    {
        while (1) {;}
    }

    /* ADC Start *****************************************************************/
    if (HAL_ADC_Start_DMA(&huart_adc1, (uint32_t*)ADC_CONV_BUFF, (ADC_MAX_CHANNEL + 1) * ADC_MAX_SAMPLEBUF) != HAL_OK)
    {
        while (1) {;}
    }
}

/**
 *  @brief  ADC_GetValue
 */
uint16_t ADC_GetValue( uint8_t index )
{
    return ((uint16_t)(ADC_CONV_BUFF[0][index]));
}

/**
 *  @brief  ADC_GetAveValue
 */
void ADC_GetAveValue( uint16_t *aveAdc )
{
    for (uint32_t i = 0; i < ADC_MAX_CHANNEL; i++)
    {
        uint32_t tmp = 0;
        for (uint32_t j = 0; j < ADC_MAX_SAMPLEBUF; j++)
        {
            tmp += ADC_CONV_BUFF[j][i];
        }
        aveAdc[i] = tmp >> 3;
    }
}

/*************************************** END OF FILE ****************************************/
