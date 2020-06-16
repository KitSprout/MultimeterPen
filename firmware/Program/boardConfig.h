/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    boardConfig.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Define ----------------------------------------------------------------------------------*/

#define KS_HW_BOARD_NAME                    "NULL"
#define KS_HW_MCU_NAME                      "STM32L011F4"

#define KS_HW_USE_CLOCK_SOUCE_INT           (0U)
#define KS_HW_USE_CLOCK_SOUCE_EXT           (1U)
#define KS_HW_HCLOCK_SOUCE                  KS_HW_USE_CLOCK_SOUCE_INT
#define KS_HW_LCLOCK_SOUCE                  KS_HW_USE_CLOCK_SOUCE_INT

#define KS_FW_UART_HAL_LIBRARY              (0U)
#define KS_FW_I2C_HAL_LIBRARY               (0U)


/* -------- LED and KEY */

#define LED_PIN                             GPIO_PIN_14
#define LED_GPIO_PORT                       GPIOC
#define LED_Set()                           __GPIO_SET(LED_GPIO_PORT, LED_PIN)
#define LED_Reset()                         __GPIO_RST(LED_GPIO_PORT, LED_PIN)
#define LED_Toggle()                        __GPIO_TOG(LED_GPIO_PORT, LED_PIN)
#define LED_On()                            LED_Set()
#define LED_Off()                           LED_Reset()

#define BUZ_PIN                             GPIO_PIN_7
#define BUZ_GPIO_PORT                       GPIOB
#define BUZ_Set()                           __GPIO_SET(BUZ_GPIO_PORT, BUZ_PIN)
#define BUZ_Reset()                         __GPIO_RST(BUZ_GPIO_PORT, BUZ_PIN)
#define BUZ_Toggle()                        __GPIO_TOG(BUZ_GPIO_PORT, BUZ_PIN)
#define BUZ_On()                            BUZ_Set()
#define BUZ_Off()                           BUZ_Reset()

#define KEY_PIN                             GPIO_PIN_9
#define KEY_GPIO_PORT                       GPIOB
#define KEY_READ()                          (__GPIO_READ(KEY_GPIO_PORT, KEY_PIN) == KEY_PIN)


/* -------- TIM Timer */

#define TIMER2                              TIM2
#define TIMER2_CLK_ENABLE()                 __HAL_RCC_TIM2_CLK_ENABLE()
#define TIMER2_IRQn                         TIM2_IRQn
#define TIMER2_TIMx_IRQHandler              TIM2_IRQHandler
#define TIMER2_TIMx_IRQn_PREEMPT            0U
#define TIMER2_TIMx_IRQn_SUB                1U


/* -------- PROBE */

#define ADC_MAX_CHANNEL                     (2U)
#define ADC_MAX_SAMPLEBUF                   (1U)

#define ADCx                                ADC1
#define ADCx_CLK_ENABLE()                   __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCx_DMA_CLK_ENABLE()               __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx_DMA_CHANNEL                    DMA1_Channel1
#define ADCx_DMA_IRQn                       DMA1_Channel1_IRQn
#define ADCx_DMA_IRQn_PREEMPT               (0U)
#define ADCx_DMA_IRQn_SUB                   (0U)

#define ADCx_CLK_DISABLE()                  __HAL_RCC_ADC1_CLK_DISABLE()
#define ADCx_SAMPLETIME                     ADC_SAMPLETIME_79CYCLES_5

#define ADCx_PROBE_RANK                     1
#define ADCx_PROBE_CHANNEL                  ADC_CHANNEL_6
#define ADCx_PROBE_CHANNEL_PIN              GPIO_PIN_6
#define ADCx_PROBE_CHANNEL_GPIO_PORT        GPIOA

#define ADCx_BATT_RANK                      2
#define ADCx_BATT_CHANNEL                   ADC_CHANNEL_9
#define ADCx_BATT_CHANNEL_PIN               GPIO_PIN_1
#define ADCx_BATT_CHANNEL_GPIO_PORT         GPIOB

#define PROBE_OUT_PIN                       GPIO_PIN_7
#define PROBE_OUT_GPIO_PORT                 GPIOA
#define PROBE_OUT_Set()                     __GPIO_SET(PROBE_OUT_GPIO_PORT, PROBE_OUT_PIN)
#define PROBE_OUT_Reset()                   __GPIO_RST(PROBE_OUT_GPIO_PORT, PROBE_OUT_PIN)
#define PROBE_OUT_Toggle()                  __GPIO_TOG(PROBE_OUT_GPIO_PORT, PROBE_OUT_PIN)
#define PROBE_OUT_On()                      PROBE_OUT_Reset()
#define PROBE_OUT_Off()                     PROBE_OUT_Set()

#define PROBE_EXT1_PIN                      GPIO_PIN_1
#define PROBE_EXT1_GPIO_PORT                GPIOA
#define PROBE_EXT1_Set()                    __GPIO_SET(PROBE_EXT1_GPIO_PORT, PROBE_EXT1_PIN)
#define PROBE_EXT1_Reset()                  __GPIO_RST(PROBE_EXT1_GPIO_PORT, PROBE_EXT1_PIN)
#define PROBE_EXT1_Toggle()                 __GPIO_TOG(PROBE_EXT1_GPIO_PORT, PROBE_EXT1_PIN)
#define PROBE_EXT1_On()                     PROBE_EXT1_Set()
#define PROBE_EXT1_Off()                    PROBE_EXT1_Reset()
#define PROBE_EXT1_READ()                   (__GPIO_READ(PROBE_EXT1_GPIO_PORT, PROBE_EXT1_PIN) == PROBE_EXT1_PIN)

#define PROBE_EXT2_PIN                      GPIO_PIN_0
#define PROBE_EXT2_GPIO_PORT                GPIOA
#define PROBE_EXT2_Set()                    __GPIO_SET(PROBE_EXT2_GPIO_PORT, PROBE_EXT2_PIN)
#define PROBE_EXT2_Reset()                  __GPIO_RST(PROBE_EXT2_GPIO_PORT, PROBE_EXT2_PIN)
#define PROBE_EXT2_Toggle()                 __GPIO_TOG(PROBE_EXT2_GPIO_PORT, PROBE_EXT2_PIN)
#define PROBE_EXT2_On()                     PROBE_EXT2_Set()
#define PROBE_EXT2_Off()                    PROBE_EXT2_Reset()
#define PROBE_EXT2_READ()                   (__GPIO_READ(PROBE_EXT2_GPIO_PORT, PROBE_EXT2_PIN) == PROBE_EXT2_PIN)


/* -------- UART Serial */

#define SERIAL_MAX_TXBUF                    8
#define SERIAL_MAX_RXBUF                    8

#define SERIAL_UARTx                        USART2
#define SERIAL_UARTx_CLK_ENABLE()           __HAL_RCC_USART2_CLK_ENABLE()
#define SERIAL_UARTx_CLK_DISABLE()          __HAL_RCC_USART2_CLK_DISABLE()
#define SERIAL_UARTx_IRQn                   USART2_IRQn
#define SERIAL_UARTx_IRQHandler             USART2_IRQHandler
#define SERIAL_UARTx_IRQn_PREEMPT           0U
#define SERIAL_UARTx_IRQn_SUB               1U

#define SERIAL_TX_PIN                       GPIO_PIN_9
#define SERIAL_TX_GPIO_PORT                 GPIOA
#define SERIAL_TX_AF                        GPIO_AF4_USART2

#define SERIAL_RX_PIN                       GPIO_PIN_10
#define SERIAL_RX_GPIO_PORT                 GPIOA
#define SERIAL_RX_AF                        GPIO_AF4_USART2

#define SERIAL_BAUDRATE                     115200
#define SERIAL_BYTESIZE                     UART_WORDLENGTH_8B
#define SERIAL_STOPBITS                     UART_STOPBITS_1
#define SERIAL_PARITY                       UART_PARITY_NONE
#define SERIAL_HARDWARECTRL                 UART_HWCONTROL_NONE
#define SERIAL_MODE                         UART_MODE_TX_RX
#define SERIAL_OVERSAMPLE                   UART_OVERSAMPLING_16


/* -------- OLED */

#define OLED_SCL_PIN                        GPIO_PIN_5
#define OLED_SCL_PORT                       GPIOA
#define OLED_SCL_H()                        __GPIO_SET(OLED_SCL_PORT, OLED_SCL_PIN)
#define OLED_SCL_L()                        __GPIO_RST(OLED_SCL_PORT, OLED_SCL_PIN)
#define OLED_SCL_READ()                     (__GPIO_READ(OLED_SCL_PORT, OLED_SCL_PIN) == OLED_SCL_PIN)

#define OLED_SDA_PIN                        GPIO_PIN_4
#define OLED_SDA_PORT                       GPIOA
#define OLED_SDA_H()                        __GPIO_SET(OLED_SDA_PORT, OLED_SDA_PIN)
#define OLED_SDA_L()                        __GPIO_RST(OLED_SDA_PORT, OLED_SDA_PIN)
#define OLED_SDA_READ()                     (__GPIO_READ(OLED_SDA_PORT, OLED_SDA_PIN) == OLED_SDA_PIN)


/* Macro -----------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
