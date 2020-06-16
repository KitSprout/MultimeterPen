/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    serial.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32x_system.h"
#include "modules\serial.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static uint8_t TX_BUFFER[SERIAL_MAX_TXBUF] = {0};
static uint8_t RX_BUFFER[SERIAL_MAX_RXBUF] = {0};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  Serial_Config
 */
void Serial_Config( void )
{
    hserial.pTxBuf = TX_BUFFER;
    hserial.pRxBuf = RX_BUFFER;

    /* UART Init *****************************************************************/
    hserial.handle->Instance          = SERIAL_UARTx;
    hserial.handle->Init.BaudRate     = SERIAL_BAUDRATE;
    hserial.handle->Init.WordLength   = SERIAL_BYTESIZE;
    hserial.handle->Init.StopBits     = SERIAL_STOPBITS;
    hserial.handle->Init.Parity       = SERIAL_PARITY;
    hserial.handle->Init.HwFlowCtl    = SERIAL_HARDWARECTRL;
    hserial.handle->Init.Mode         = SERIAL_MODE;
    hserial.handle->Init.OverSampling = SERIAL_OVERSAMPLE;
    HAL_UART_Init(hserial.handle);

#if KS_FW_UART_HAL_LIBRARY

#else
    /* UART IT *******************************************************************/
    if (hserial.txEventCallback != NULL)
    {
        __HAL_UART_ENABLE_IT(hserial.handle, UART_IT_TXE);
    }
    else
    {
        __HAL_UART_DISABLE_IT(hserial.handle, UART_IT_TXE);
    }

    if (hserial.rxEventCallback != NULL)
    {
        __HAL_UART_ENABLE_IT(hserial.handle, UART_IT_RXNE);
    }
    else
    {
        __HAL_UART_DISABLE_IT(hserial.handle, UART_IT_RXNE);
    }

    /* UART Enable ***************************************************************/
    __HAL_UART_ENABLE(hserial.handle);
    __HAL_UART_CLEAR_FLAG(hserial.handle, UART_FLAG_TC);

#endif
}

/**
 *  @brief  Serial_SendByte
 */
__INLINE void Serial_SendByte( uint8_t sendByte )
{
    UART_SendByte(hserial.handle, &sendByte);
}

/**
 *  @brief  Serial_RecvByte
 */
__INLINE uint8_t Serial_RecvByte( void )
{
    uint8_t recvByte = 0;
    UART_RecvByte(hserial.handle, &recvByte);
    return recvByte;
}

#if !KS_FW_UART_HAL_LIBRARY
/**
 *  @brief  Serial Recv Wait
 */
__INLINE uint32_t Serial_RecvWait( uint8_t *recvData )
{
    return UART_RecvWait(hserial.handle, recvData);
}
#endif

/**
 *  @brief  Serial Send Data
 */
__INLINE uint32_t Serial_SendData( uint8_t *sendData, uint16_t lens, uint32_t timuout )
{
    return UART_SendData(hserial.handle, sendData, lens, timuout);
}

/**
 *  @brief  Serial Recv Data
 */
__INLINE uint32_t Serial_RecvData( uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
    return UART_RecvData(hserial.handle, recvData, lens, timeout);
}

#if KS_FW_UART_HAL_LIBRARY

/**
 *  @brief  Serial Send Data IT
 */
__INLINE uint32_t Serial_SendDataIT( uint8_t *sendData, uint16_t lens)
{
    return HAL_UART_Transmit_IT(hserial.handle, sendData, lens);
}

/**
 *  @brief  Serial Recv Data IT
 */
__INLINE uint32_t Serial_RecvDataIT( uint8_t *recvData, uint16_t lens )
{
    return HAL_UART_Receive_IT(hserial.handle, recvData, lens);
}

/**
 *  @brief  Serial Send Data DMA
 */
__INLINE uint32_t Serial_SendDataDMA( uint8_t *sendData, uint16_t lens )
{
    return HAL_UART_Transmit_DMA(hserial.handle, sendData, lens);
}

/**
 *  @brief  Serial Recv Data DMA
 */
__INLINE uint32_t Serial_RecvDataDMA( uint8_t *recvData, uint16_t lens )
{
    return HAL_UART_Receive_DMA(hserial.handle, recvData, lens);
}
#endif

/**
 *  @brief  fputc
 */
int fputc( int ch, FILE *f )
{
    hserial.handle->Instance->TDR = ((uint8_t)ch & (uint16_t)0x01FF);
    while (!(hserial.handle->Instance->ISR & UART_FLAG_TXE));
    return (ch);
}

/**
 *  @brief  fgetc
 */
int fgetc( FILE *f )
{
    while (!(hserial.handle->Instance->ISR & UART_FLAG_RXNE));
    return (hserial.handle->Instance->RDR & (uint16_t)0x01FF);
}

/*************************************** END OF FILE ****************************************/
