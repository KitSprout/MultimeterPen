/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32l0_uart.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32L0_UART_H
#define __STM32L0_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32x_system.h"
#include "kitsprout\kSystem.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct
{
    UART_HandleTypeDef *handle;

    const uint16_t txBufLens;
    const uint16_t rxBufLens;
    uint8_t *pTxBuf;
    uint8_t *pRxBuf;
    pFunc txEventCallback;
    pFunc rxEventCallback;

} __attribute__((aligned)) UartHandle_st;

/* Extern ----------------------------------------------------------------------------------*/

extern UartHandle_st hserial;

/* Functions -------------------------------------------------------------------------------*/

#if KS_FW_UART_HAL_LIBRARY

#define UART_SendByte( __H, __DAT )                 HAL_UART_Transmit(__H, __DAT, 1, HAL_MAX_DELAY)
#define UART_RecvByte( __H, __DAT )                 HAL_UART_Receive(__H, __DAT, 1, HAL_MAX_DELAY)
#define UART_SendData( __H, __DAT, __LEN, __TIME )  HAL_UART_Transmit(__H, __DAT, __LEN, __TIME)
#define UART_RecvData( __H, __DAT, __LEN, __TIME )  HAL_UART_Receive(__H, __DAT, __LEN, __TIME)
#define UART_SendDataIT( __H, __DAT, __LEN )        HAL_UART_Transmit_IT(__H, __DAT, __LEN)
#define UART_RecvDataIT( __H, __DAT, __LEN )        HAL_UART_Receive_IT(__H, __DAT, __LEN)
#define UART_SendDataDMA( __H, __DAT, __LEN )       HAL_UART_Transmit_DMA(__H, __DAT, __LEN)
#define UART_RecvDataDMA( __H, __DAT, __LEN )       HAL_UART_Receive_DMA(__H, __DAT, __LEN)

#else
void        UART_SendByte( UART_HandleTypeDef *huart, uint8_t *sendByte );
void        UART_RecvByte( UART_HandleTypeDef *huart, uint8_t *recvByte );
uint32_t    UART_RecvWait( UART_HandleTypeDef *huart, uint8_t *recvData );
uint32_t    UART_SendData( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens, uint32_t timeout );
uint32_t    UART_RecvData( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens, uint32_t timeout );
//uint32_t    UART_SendDataIT( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens );
//uint32_t    UART_RecvDataIT( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens );
//uint32_t    UART_SendDataDMA( UART_HandleTypeDef *huart, uint8_t *sendData, uint16_t lens );
//uint32_t    UART_RecvDataDMA( UART_HandleTypeDef *huart, uint8_t *recvData, uint16_t lens );

#endif

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
