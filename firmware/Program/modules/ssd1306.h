/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    ssd1306.h
 *  @author  KitSprout
 *  @date    Apr-2020
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __SSD1306_H
#define __SSD1306_H

#ifdef __cplusplus
extern "C" {
#endif

#define __SSD1306

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

/* Define ----------------------------------------------------------------------------------*/

#define SSD1306_I2C_ADDRESS         (0x3C)

#define SSD1306_WIDTH               (72U)
#define SSD1306_HIGHT               (40U)
#define SSD1306_HIGHT_BYTE          (SSD1306_HIGHT >> 3)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        SSD1306_Config( void );
void        SSD1306_Init( void );

void        SSD1306_WriteCmd( const uint8_t writeCmd );
void        SSD1306_WriteData( const uint8_t writeData );
void        SSD1306_WriteDatas( const uint8_t *writeData, const uint32_t lens );

void        SSD1306_Display( uint8_t cmd );
void        SSD1306_Clear( uint32_t color );
void        SSD1306_DrawPixel( const uint32_t posX, const uint32_t posY, const uint32_t color );
//void        SSD1306_SetWindow( int16_t posX1, int16_t posY1, int16_t posX2, int16_t posY2 );

void        SSD1306_Refresh( void );
void        SSD1306_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);

void SSD1306_DrawLineX( const uint32_t posX, const uint32_t posY, const uint32_t length, const uint32_t color );
void SSD1306_DrawLineY( const uint32_t posX, const uint32_t posY, const uint32_t length, const uint32_t color );
void SSD1306_DrawRect( const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height, const uint32_t color );
void SSD1306_DrawRectFill( const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height, const uint32_t color );

void SSD1306_PutFont( const uint32_t posX, const uint32_t pageY, const uint8_t ch, const uint32_t color );
void SSD1306_PutVoltage( const uint32_t posX, const uint32_t pageY, const float voltage, const uint32_t color );
void SSD1306_PutBatteryVoltage( const uint32_t posX, const uint32_t pageY, const float battVoltage, const uint32_t color );
void SSD1306_PutReferenceVoltage( const uint32_t posX, const uint32_t pageY, const float refVoltage, const uint32_t color );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
