/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    stm32l0_i2cSimu.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __STM32L0_I2CSIMU_H
#define __STM32L0_I2CSIMU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32x_system.h"
#include "kitsprout/kStatus.h"

/* Define ----------------------------------------------------------------------------------*/

#define ENABLE_ACK_CHECK                                (1U)

#define SIMUI2C_DEFAULT_FREQ                            (8U)
#define SIMUI2C_ACK                                     (0U)
#define SIMUI2C_NACK                                    (1U)
#define SIMUI2C_WRITE                                   (0U)
#define SIMUI2C_READ                                    (1U)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct
{
    uint32_t      PinSDA;
    GPIO_TypeDef *PortSDA;
    uint32_t      PinSCL;
    GPIO_TypeDef *PortSCL;
    uint32_t      Frequency;

} SimuI2C_InitTypeDef;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        SimuI2C_Config( SimuI2C_InitTypeDef *pSimuI2c );

void        SimuI2C_Start( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_Stop( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_SendACK( SimuI2C_InitTypeDef *pSimuI2c, const uint32_t ack );
uint32_t    SimuI2C_RecvACK( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_SendByte( SimuI2C_InitTypeDef *pSimuI2c, uint8_t sendByte );
uint8_t     SimuI2C_RecvByte( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_SendBytes( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t *sendBytes, const uint32_t lens );
void        SimuI2C_RecvBytes( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *recvBytes, const uint32_t lens );

uint32_t    SimuI2C_BeginTransmission( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t sendbyte );
void        SimuI2C_EndTransmission( SimuI2C_InitTypeDef *pSimuI2c );
uint32_t    SimuI2C_SendByteAck( SimuI2C_InitTypeDef *pSimuI2c, uint8_t sendByte );
uint8_t     SimuI2C_RecvByteAck( SimuI2C_InitTypeDef *pSimuI2c, const uint32_t ack );
uint32_t    SimuI2C_SendBytesAck( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t *sendBytes, const uint32_t lens );
void        SimuI2C_RecvBytesAck( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *recvBytes, const uint32_t lens );

uint32_t    SimuI2C_WriteData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t writeData );
uint32_t    SimuI2C_WriteDatas( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t *writeData, const uint32_t lens );
uint32_t    SimuI2C_ReadData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *readData );
uint32_t    SimuI2C_ReadDatas( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *readData, const uint32_t lens );

uint32_t    SimuI2C_WriteReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t regData );
uint32_t    SimuI2C_ReadReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData );
uint32_t    SimuI2C_WriteRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t *regData, const uint32_t lens );
uint32_t    SimuI2C_ReadRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData, const uint32_t lens );

uint32_t    SimuI2C_ScanDevice( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *devAddress );
uint32_t    SimuI2C_ScanRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *regData );

uint32_t    SimuI2C_ScanDeviceGPIO( const uint8_t sclpin, GPIO_TypeDef *sclport, const uint8_t sdapin, GPIO_TypeDef *sdaport, uint8_t *devAddress );
uint32_t    SimuI2C_ScanRegsGPIO( const uint8_t sclpin, GPIO_TypeDef *sclport, const uint8_t sdapin, GPIO_TypeDef *sdaport, const uint8_t slaveAddr, uint8_t *regData );
void        SimuI2C_ScanAllGPIO( const uint8_t sclpin, GPIO_TypeDef *sclport, const uint8_t sdapin, GPIO_TypeDef *sdaport );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
