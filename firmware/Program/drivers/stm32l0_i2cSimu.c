/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 *  
 *  @file    stm32f0_i2cSimu.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32l0_i2cSimu.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/

#define I2CxS_SCL_L()               __GPIO_RST(pSimuI2c->PortSCL, pSimuI2c->PinSCL)
#define I2CxS_SCL_H()               __GPIO_SET(pSimuI2c->PortSCL, pSimuI2c->PinSCL)
#define I2CxS_SCL_READ()            (__GPIO_READ(pSimuI2c->PortSCL, pSimuI2c->PinSCL) == pSimuI2c->PinSCL)

#define I2CxS_SDA_L()               __GPIO_RST(pSimuI2c->PortSDA, pSimuI2c->PinSDA)
#define I2CxS_SDA_H()               __GPIO_SET(pSimuI2c->PortSDA, pSimuI2c->PinSDA)
#define I2CxS_SDA_READ()            (__GPIO_READ(pSimuI2c->PortSDA, pSimuI2c->PinSDA) == pSimuI2c->PinSDA)

/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  SimuI2C_Config
 */
void SimuI2C_Config( SimuI2C_InitTypeDef *pSimuI2c )
{
    GPIO_InitTypeDef GPIO_InitStruct; 

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin   = pSimuI2c->PinSCL;
    HAL_GPIO_Init(pSimuI2c->PortSCL, &GPIO_InitStruct);

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin   = pSimuI2c->PinSDA;
    HAL_GPIO_Init(pSimuI2c->PortSDA, &GPIO_InitStruct);

    I2CxS_SCL_H();
    I2CxS_SDA_H();
}

/**
 *  @brief  SimuI2C_Delay
 */
static void SimuI2C_Delay( SimuI2C_InitTypeDef *pSimuI2c )
{
    uint32_t count = pSimuI2c->Frequency;

    while (count--)
    {
        __nop();
    }
}

/**
 *  @brief  SimuI2C_Start
 */
void SimuI2C_Start( SimuI2C_InitTypeDef *pSimuI2c )
{
    I2CxS_SDA_H();
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SDA_L();
    SimuI2C_Delay(pSimuI2c);
}

/**
 *  @brief  SimuI2C_Stop
 */
void SimuI2C_Stop( SimuI2C_InitTypeDef *pSimuI2c )
{
    I2CxS_SDA_L();
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SDA_H();
    SimuI2C_Delay(pSimuI2c);
}

/**
 *  @brief  SimuI2C_SendACK
 */
void SimuI2C_SendACK( SimuI2C_InitTypeDef *pSimuI2c, const uint32_t ack )
{
    I2CxS_SCL_L();
    if (ack)
    {
        I2CxS_SDA_H();
    }
    else
    {
        I2CxS_SDA_L();
    }
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_L();
    SimuI2C_Delay(pSimuI2c);
}

/**
 *  @brief  SimuI2C_RecvACK
 */
uint32_t SimuI2C_RecvACK( SimuI2C_InitTypeDef *pSimuI2c )
{
    uint32_t ack;

    I2CxS_SCL_L();
    I2CxS_SDA_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    ack = I2CxS_SDA_READ();
    I2CxS_SCL_L();
    SimuI2C_Delay(pSimuI2c);

    return ack;
}

/**
 *  @brief  SimuI2C_SendByte
 */
void SimuI2C_SendByte( SimuI2C_InitTypeDef *pSimuI2c, uint8_t sendByte )
{
    uint32_t cnt = 8;

    while (cnt--)
    {
        I2CxS_SCL_L();
        SimuI2C_Delay(pSimuI2c);
        if (sendByte & 0x80)  // MSB First
        {
            I2CxS_SDA_H();
        }
        else
        {
            I2CxS_SDA_L();
        }
        sendByte = sendByte << 1;
        SimuI2C_Delay(pSimuI2c);
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
    }
    I2CxS_SCL_L();
}

/**
 *  @brief  SimuI2C_RecvByte
 */
uint8_t SimuI2C_RecvByte( SimuI2C_InitTypeDef *pSimuI2c )
{
    uint32_t cnt = 8;
    uint8_t recvByte = 0;

    I2CxS_SDA_H();
    while (cnt--)
    {
        recvByte = recvByte << 1;
        I2CxS_SCL_L();
        SimuI2C_Delay(pSimuI2c);
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
        if (I2CxS_SDA_READ())
        {
            recvByte |= 0x01;
        }
    }
    I2CxS_SCL_L();

    return recvByte;
}

/**
 *  @brief  SimuI2C_SendBytes
 */
void SimuI2C_SendBytes( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t *sendBytes, const uint32_t lens )
{
    uint32_t i, cnt;
    uint8_t sendByte;

    for (i = 0; i < lens; i++)
    {
        sendByte = *(sendBytes++);
        cnt = 8;
        while (cnt--)
        {
            I2CxS_SCL_L();
            SimuI2C_Delay(pSimuI2c);
            if (sendByte & 0x80)  // MSB First
            {
                I2CxS_SDA_H();
            }
            else
            {
                I2CxS_SDA_L();
            }
            sendByte = sendByte << 1;
            SimuI2C_Delay(pSimuI2c);
            I2CxS_SCL_H();
            SimuI2C_Delay(pSimuI2c);
        }
    }
    I2CxS_SCL_L();
}

/**
 *  @brief  SimuI2C_RecvBytes
 */
void SimuI2C_RecvBytes( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *recvBytes, const uint32_t lens )
{
    uint32_t i, cnt;
    uint8_t recvByte;

    I2CxS_SDA_H();
    for (i = 0; i < lens; i++)
    {
        cnt = 8;
        recvByte = 0;
        while (cnt--)
        {
            recvByte = recvByte << 1;
            I2CxS_SCL_L();
            SimuI2C_Delay(pSimuI2c);
            I2CxS_SCL_H();
            SimuI2C_Delay(pSimuI2c);
            if (I2CxS_SDA_READ())
            {
                recvByte |= 0x01;
            }
        }
        *(recvBytes++) = recvByte;
    }

    I2CxS_SCL_L();
}

/**
 *  @brief  SimuI2C_BeginTransmission
 */
uint32_t SimuI2C_BeginTransmission( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t sendbyte )
{
    SimuI2C_Start(pSimuI2c);
    return (SimuI2C_SendByteAck(pSimuI2c, sendbyte) != SIMUI2C_ACK) ? KS_ERROR : KS_OK;
}

/**
 *  @brief  SimuI2C_EndTransmission
 */
void SimuI2C_EndTransmission( SimuI2C_InitTypeDef *pSimuI2c )
{
    I2CxS_SDA_L();
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SDA_H();
    SimuI2C_Delay(pSimuI2c);
}

/**
 *  @brief  SimuI2C_SendByteAck
 */
uint32_t SimuI2C_SendByteAck( SimuI2C_InitTypeDef *pSimuI2c, uint8_t sendByte )
{
    uint32_t cnt = 8;
    uint32_t ack;

    while (cnt--)
    {
        I2CxS_SCL_L();
        SimuI2C_Delay(pSimuI2c);
        if (sendByte & 0x80)  // MSB First
        {
            I2CxS_SDA_H();
        }
        else
        {
            I2CxS_SDA_L();
        }
        sendByte = sendByte << 1;
        SimuI2C_Delay(pSimuI2c);
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
    }
    I2CxS_SCL_L();
    I2CxS_SDA_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    ack = I2CxS_SDA_READ();
    I2CxS_SCL_L();
    SimuI2C_Delay(pSimuI2c);

    return ack;
}

/**
 *  @brief  SimuI2C_RecvByteAck
 */
uint8_t SimuI2C_RecvByteAck( SimuI2C_InitTypeDef *pSimuI2c, const uint32_t ack )
{
    uint32_t cnt = 8;
    uint8_t recvByte = 0;

    I2CxS_SDA_H();
    while (cnt--)
    {
        I2CxS_SCL_L();
        SimuI2C_Delay(pSimuI2c);
        recvByte = recvByte << 1;
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
        if (I2CxS_SDA_READ())
        {
            recvByte |= 0x01;
        }
    }
    I2CxS_SCL_L();
    if (ack)
    {
        I2CxS_SDA_H();
    }
    else
    {
        I2CxS_SDA_L();
    }
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_L();
    SimuI2C_Delay(pSimuI2c);

    return recvByte;
}

/**
 *  @brief  SimuI2C_SendBytesAck
 */
uint32_t SimuI2C_SendBytesAck( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t *sendBytes, const uint32_t lens )
{
    uint32_t i, cnt;
    uint32_t ack;
    uint8_t sendByte;

    for (i = 0; i < lens; i++)
    {
        sendByte = *(sendBytes++);
        cnt = 8;
        while (cnt--)
        {
            I2CxS_SCL_L();
            SimuI2C_Delay(pSimuI2c);
            if (sendByte & 0x80)  // MSB First
            {
                I2CxS_SDA_H();
            }
            else
            {
                I2CxS_SDA_L();
            }
            sendByte = sendByte << 1;
            SimuI2C_Delay(pSimuI2c);
            I2CxS_SCL_H();
            SimuI2C_Delay(pSimuI2c);
        }
        I2CxS_SCL_L();
        I2CxS_SDA_H();
        SimuI2C_Delay(pSimuI2c);
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
        ack = I2CxS_SDA_READ();
        if (ack != SIMUI2C_ACK)
        {
            return KS_ERROR;
        }
    }
    I2CxS_SCL_L();
    return KS_OK;
}

/**
 *  @brief  SimuI2C_RecvBytesAck
 */
void SimuI2C_RecvBytesAck( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *recvBytes, const uint32_t lens )
{
    uint32_t i, cnt;
    uint8_t recvByte;

    for (i = 0; i < lens; i++)
    {
        I2CxS_SDA_H();
        cnt = 8;
        recvByte = 0;
        while (cnt--)
        {
            I2CxS_SCL_L();
            SimuI2C_Delay(pSimuI2c);
            recvByte = recvByte << 1;
            I2CxS_SCL_H();
            SimuI2C_Delay(pSimuI2c);
            if (I2CxS_SDA_READ())
            {
                recvByte |= 0x01;
            }
        }
        I2CxS_SCL_L();
        *(recvBytes++) = recvByte;
        if (i == (lens - 1))    // last data send NACK
        {
            I2CxS_SDA_H();
        }
        else
        {
            I2CxS_SDA_L();
        }
        SimuI2C_Delay(pSimuI2c);
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
    }
    I2CxS_SCL_L();
}

/**
 *  @brief  SimuI2C_WriteData
 */
uint32_t SimuI2C_WriteData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t writeData )
{
    uint8_t address8bit = slaveAddr << 1;

#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendByteAck(pSimuI2c, writeData) != SIMUI2C_ACK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(pSimuI2c, writeData);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, writeData);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_ReadData
 */
uint32_t SimuI2C_ReadData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *readData )
{
    uint8_t address8bit = slaveAddr << 1;
#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    *readData = SimuI2C_RecvByteAck(pSimuI2c, SIMUI2C_NACK);
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ);
    *readData = SimuI2C_RecvByteAck(pSimuI2c, SIMUI2C_NACK);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_READ);
    SimuI2C_RecvACK(pSimuI2c);
    *readData = SimuI2C_RecvByte(pSimuI2c);
    SimuI2C_SendACK(pSimuI2c, SIMUI2C_NACK);
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_WriteDatas
 */
uint32_t SimuI2C_WriteDatas( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t *writeData, const uint32_t lens )
{
    uint8_t address8bit = slaveAddr << 1;
#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendBytesAck(pSimuI2c, writeData, lens) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendBytesAck(pSimuI2c, writeData, lens);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    uint32_t cnt = lens;
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_RecvACK(pSimuI2c);
    while (cnt--)
    {
        SimuI2C_SendByte(pSimuI2c, *(writeData++));
        SimuI2C_RecvACK(pSimuI2c);
    }
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_ReadDatas
 */
uint32_t SimuI2C_ReadDatas( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *readData, const uint32_t lens )
{
    uint8_t address8bit = slaveAddr << 1;
#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    SimuI2C_RecvBytesAck(pSimuI2c, readData, lens);
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ);
    SimuI2C_RecvBytesAck(pSimuI2c, readData, lens);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    uint32_t cnt = lens;
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_READ);
    SimuI2C_RecvACK(pSimuI2c);
    while (cnt--)
    {
        *(readData++) = SimuI2C_RecvByte(pSimuI2c);
        if (cnt)
        {
            SimuI2C_SendACK(pSimuI2c, SIMUI2C_ACK);
        }
        else
        {
            SimuI2C_SendACK(pSimuI2c, SIMUI2C_NACK);
        }
    }
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_WriteReg
 */
uint32_t SimuI2C_WriteReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t regData )
{
    uint8_t address8bit = slaveAddr << 1;
#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendByteAck(pSimuI2c, regAddr) != SIMUI2C_ACK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendByteAck(pSimuI2c, regData) != SIMUI2C_ACK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(pSimuI2c, regAddr);
    SimuI2C_SendByteAck(pSimuI2c, regData);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regData);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_ReadReg
 */
uint32_t SimuI2C_ReadReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData )
{
    uint8_t address8bit = slaveAddr << 1;
#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendByteAck(pSimuI2c, regAddr) != SIMUI2C_ACK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    *regData = SimuI2C_RecvByteAck(pSimuI2c, SIMUI2C_NACK);
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(pSimuI2c, regAddr);
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ);
    *regData = SimuI2C_RecvByteAck(pSimuI2c, SIMUI2C_NACK);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_READ);
    SimuI2C_RecvACK(pSimuI2c);
    *regData = SimuI2C_RecvByte(pSimuI2c);
    SimuI2C_SendACK(pSimuI2c, NACK);
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_WriteRegs
 */
uint32_t SimuI2C_WriteRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t *regData, const uint32_t lens )
{
    uint8_t address8bit = slaveAddr << 1;
#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendByteAck(pSimuI2c, regAddr) != SIMUI2C_ACK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendBytesAck(pSimuI2c, regData, lens) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(pSimuI2c, regAddr);
    SimuI2C_SendBytesAck(pSimuI2c, regData, lens);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    uint32_t cnt = lens;
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    while (cnt--)
    {
        SimuI2C_SendByte(pSimuI2c, *(regData++));
        SimuI2C_RecvACK(pSimuI2c);
    }
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_ReadRegs
 */
uint32_t SimuI2C_ReadRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData, const uint32_t lens )
{
    uint8_t address8bit = slaveAddr << 1;
#if 1
#if ENABLE_ACK_CHECK
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_SendByteAck(pSimuI2c, regAddr) != SIMUI2C_ACK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    if (SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ) != KS_OK)
    {
        SimuI2C_EndTransmission(pSimuI2c);
        return KS_ERROR;
    }
    SimuI2C_RecvBytesAck(pSimuI2c, regData, lens);
    SimuI2C_EndTransmission(pSimuI2c);
#else
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(pSimuI2c, regAddr);
    SimuI2C_BeginTransmission(pSimuI2c, address8bit | SIMUI2C_READ);
    SimuI2C_RecvBytesAck(pSimuI2c, regData, lens);
    SimuI2C_EndTransmission(pSimuI2c);
#endif
#else
    uint32_t cnt = lens;
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, address8bit | SIMUI2C_READ);
    SimuI2C_RecvACK(pSimuI2c);
    while (cnt--)
    {
        *(regData++) = SimuI2C_RecvByte(pSimuI2c);
        if (cnt)
        {
            SimuI2C_SendACK(pSimuI2c, SIMUI2C_ACK);
        }
        else
        {
            SimuI2C_SendACK(pSimuI2c, SIMUI2C_NACK);
        }
    }
    SimuI2C_Stop(pSimuI2c);
#endif
    return KS_OK;
}

/**
 *  @brief  SimuI2C_ScanDevice
 */
uint32_t SimuI2C_ScanDevice( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *devAddress )
{
    uint32_t status;
    uint32_t idx = 0;

    SimuI2C_EndTransmission(pSimuI2c);
    for (uint32_t i = 0; i < 128; i++)
    {
        status = SimuI2C_BeginTransmission(pSimuI2c, i << 1);
        SimuI2C_EndTransmission(pSimuI2c);
        if (status == KS_OK)
        {
            devAddress[idx++] = i;
        }
        else
        {
            devAddress[i] = 0;
        }
    }

#if 0
    klogc("\n");
    klogc(" >>  i2c device scanner (found %d)\n\n", idx);
    klogc("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    idx = 0;
    for (uint32_t i = 0; i < 128; i++)
    {
        if (i % 16 == 0)
        {
           klogc("\n %02X:", i);
        }
        if (i == devAddress[idx])
        {
            idx++;
            klogc(" %02X", i);
        }
        else
        {
            klogc(" --");
        }
    }
    klogc("\n\n");
    klogc(NULL);
#endif

    return idx;
}

/**
 *  @brief  SimuI2C_ScanRegs
 */
#define MULTIBYTEREAD    (8U)
uint32_t SimuI2C_ScanRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *regData )
{
    uint32_t status;

    SimuI2C_EndTransmission(pSimuI2c);
    status = SimuI2C_BeginTransmission(pSimuI2c, slaveAddr << 1);
    SimuI2C_EndTransmission(pSimuI2c);

    if (status == KS_OK)
    {
        for (uint32_t i = 0; i < 256; i = i + MULTIBYTEREAD)
        {
            SimuI2C_ReadRegs(pSimuI2c, slaveAddr, i, &regData[i], MULTIBYTEREAD);
        }
    }

#if 0
    klogc("\n");
    klogc(" >>  i2c device register (address 0x%02X)\n\n", slaveAddr);
    klogc("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    for (uint32_t i = 0; i < 256; i++)
    {
        if (i % 16 == 0)
        {
            klogc("\n %02X:", i);
        }
        klogc(" %02X", regData[i]);
    }
    klogc("\n\n");
    klogc(NULL);
#endif

    return status;
}

#if 1
/**
 *  @brief  SimuI2C_ScanDeviceGPIO
 */
uint32_t SimuI2C_ScanDeviceGPIO( const uint8_t sclpin, GPIO_TypeDef *sclport, const uint8_t sdapin, GPIO_TypeDef *sdaport, uint8_t *devAddress )
{
    uint8_t devCount;
    SimuI2C_InitTypeDef i2c =
    {
        .PinSCL     = sclpin,
        .PortSCL    = sclport,
        .PinSDA     = sdapin,
        .PortSDA    = sdaport,
        .Frequency  = SIMUI2C_DEFAULT_FREQ
    };
    SimuI2C_Config(&i2c);
    devCount = SimuI2C_ScanDevice(&i2c, devAddress);

#if 0
    klogc("\n");
    klogc(" >>  i2c device scanner (found %d)\n\n", devCount);
    klogc("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    devCount = 0;
    for (uint32_t i = 0; i < 128; i++)
    {
        if (i % 16 == 0)
        {
           klogc("\n %02X:", i);
        }
        if (i == devAddress[devCount])
        {
            devCount++;
            klogc(" %02X", i);
        }
        else
        {
            klogc(" --");
        }
    }
    klogc("\n\n");
    klogc(NULL);
#endif

    return devCount;
}

/**
 *  @brief  SimuI2C_ScanRegsGPIO
 */
uint32_t SimuI2C_ScanRegsGPIO( const uint8_t sclpin, GPIO_TypeDef *sclport, const uint8_t sdapin, GPIO_TypeDef *sdaport, const uint8_t slaveAddr, uint8_t *regData )
{
    uint8_t status;
    SimuI2C_InitTypeDef i2c =
    {
        .PinSCL     = sclpin,
        .PortSCL    = sclport,
        .PinSDA     = sdapin,
        .PortSDA    = sdaport,
        .Frequency  = SIMUI2C_DEFAULT_FREQ
    };
    SimuI2C_Config(&i2c);
    status = SimuI2C_ScanRegs(&i2c, slaveAddr, regData);

#if 0
    klogc("\n");
    klogc(" >>  i2c device register (address 0x%02X)\n\n", slaveAddr);
    klogc("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    for (uint32_t i = 0; i < 256; i++)
    {
        if (i % 16 == 0)
        {
            klogc("\n %02X:", i);
        }
        klogc(" %02X", regData[i]);
    }
    klogc("\n\n");
    klogc(NULL);
#endif

    return status;
}

/**
 *  @brief  SimuI2C_ScanAllGPIO
 */
void SimuI2C_ScanAllGPIO( const uint8_t sclpin, GPIO_TypeDef *sclport, const uint8_t sdapin, GPIO_TypeDef *sdaport )
{
    uint8_t slaveAddr[128] = {0};
    uint8_t regData[256] = {0};
    uint8_t devCount;

    SimuI2C_InitTypeDef i2c =
    {
        .PinSCL     = sclpin,
        .PortSCL    = sclport,
        .PinSDA     = sdapin,
        .PortSDA    = sdaport,
        .Frequency  = SIMUI2C_DEFAULT_FREQ
    };
    SimuI2C_Config(&i2c);

    devCount = SimuI2C_ScanDevice(&i2c, slaveAddr);
#if 0
    klogc("\n");
    klogc(" >>  i2c device scanner (found %d)\n\n", devCount);
    klogc("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    devCount = 0;
    for (uint32_t i = 0; i < 128; i++)
    {
        if (i % 16 == 0)
        {
           klogc("\n %02X:", i);
        }
        if (i == devAddress[devCount])
        {
            devCount++;
            klogc(" %02X", i);
        }
        else
        {
            klogc(" --");
        }
    }
    klogc("\n\n");
    klogc(NULL);
#endif
    for (uint32_t i = 0; i < devCount; i++)
    {
        SimuI2C_ScanRegs(&i2c, slaveAddr[i], regData);
#if 0
        klogc("\n");
        klogc(" >>  i2c device register (address 0x%02X)\n\n", slaveAddr);
        klogc("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
        for (uint32_t i = 0; i < 256; i++)
        {
            if (i % 16 == 0)
            {
                klogc("\n %02X:", i);
            }
            klogc(" %02X", regData[i]);
        }
        klogc("\n\n");
        klogc(NULL);
#endif
    }
}
#endif

/*************************************** END OF FILE ****************************************/
