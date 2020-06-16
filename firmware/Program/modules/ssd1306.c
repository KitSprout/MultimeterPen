/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    ssd1306.c
 *  @author  KitSprout
 *  @date    Apr-2020
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32x_system.h"
#include "drivers\stm32l0_i2cSimu.h"
#include "modules\ssd1306.h"

/* Define ----------------------------------------------------------------------------------*/
#define SSD1306_Delay(__MS)     delay_ms(__MS)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static SimuI2C_InitTypeDef hi2c;
static uint8_t SSD1306_GRAM[SSD1306_HIGHT_BYTE][SSD1306_WIDTH]; // [y][x]
static uint8_t address8bit = SSD1306_I2C_ADDRESS << 1;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  SSD1306_WriteCmd
 */
void SSD1306_WriteCmd( const uint8_t writeCmd )
{
    SimuI2C_BeginTransmission(&hi2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(&hi2c, 0x00);
    SimuI2C_SendByteAck(&hi2c, writeCmd);
    SimuI2C_EndTransmission(&hi2c);
}

/**
 *  @brief  SSD1306_WriteData
 */
void SSD1306_WriteData( const uint8_t writeData )
{
    SimuI2C_BeginTransmission(&hi2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(&hi2c, 0x40);
    SimuI2C_SendByteAck(&hi2c, writeData);
    SimuI2C_EndTransmission(&hi2c);
}

/**
 *  @brief  SSD1306_WriteDatas
 */
void SSD1306_WriteDatas( const uint8_t *writeData, const uint32_t lens )
{
    SimuI2C_BeginTransmission(&hi2c, address8bit | SIMUI2C_WRITE);
    SimuI2C_SendByteAck(&hi2c, 0x40);
    for (uint32_t i = 0; i < lens; i++)
    {
        SimuI2C_SendByteAck(&hi2c, writeData[i]);
    }
    SimuI2C_EndTransmission(&hi2c);
}

/**
 *  @brief  SSD1306_Config
 */
void SSD1306_Config( void )
{
    hi2c.PinSCL    = OLED_SCL_PIN;
    hi2c.PortSCL   = OLED_SCL_PORT;
    hi2c.PinSDA    = OLED_SDA_PIN;
    hi2c.PortSDA   = OLED_SDA_PORT;
    hi2c.Frequency = 8;
    SimuI2C_Config(&hi2c);
}

/**
 *  @brief  SSD1306_Init
 */
void SSD1306_Init( void )
{
    /* Device Init */
    SSD1306_Display(DISABLE);   // Display Off
    SSD1306_WriteCmd(0xD5);     // Set osc division
    SSD1306_WriteCmd(0xF0);
    SSD1306_WriteCmd(0xA8);     // Multiplex ratio
    SSD1306_WriteCmd(0x27);     // Duty = 1/40
    SSD1306_WriteCmd(0xD3);     // Set display offset
    SSD1306_WriteCmd(0x00);
    SSD1306_WriteCmd(0x40);     // Set Display Start Line
    SSD1306_WriteCmd(0x8D);     // Set charge pump enable
    SSD1306_WriteCmd(0x14);
    SSD1306_WriteCmd(0x20);     // Set page address mode ... Vertical Addressing Mode
    SSD1306_WriteCmd(0x02);
    SSD1306_WriteCmd(0xA1);     // set segment remap
    SSD1306_WriteCmd(0xC8);     // Com scan direction
    SSD1306_WriteCmd(0xDA);     // set COM pins
    SSD1306_WriteCmd(0x12);
    SSD1306_WriteCmd(0xAD);     // Internal IREF Setting
    SSD1306_WriteCmd(0x30);
    SSD1306_WriteCmd(0x81);     // contract control
    SSD1306_WriteCmd(0xFF);     // 128
    SSD1306_WriteCmd(0xD9);     // set pre-charge period
    SSD1306_WriteCmd(0x22);
    SSD1306_WriteCmd(0xDB);     // set vcomh
    SSD1306_WriteCmd(0x20);
    SSD1306_WriteCmd(0xA4);     // Set Entire Display On/Off
    SSD1306_WriteCmd(0xA6);     // normal / reverse
    SSD1306_WriteCmd(0x0C);     // set lower column address
    SSD1306_WriteCmd(0x11);     // set higher column address
    SSD1306_Clear(0);
    SSD1306_Display(ENABLE);    // display on
}

/**
 *  @brief  SSD1306_Display
 */
void SSD1306_Display( uint8_t cmd )
{
    if (cmd == ENABLE)
    {
        SSD1306_WriteCmd(0xAF);
    }
    else
    {
        SSD1306_WriteCmd(0xAE);
    }
}

/**
 *  @brief  SSD1306_Refresh
 */
void SSD1306_Refresh( void )
{
    for (uint32_t i = 0; i < SSD1306_HIGHT_BYTE; i++)
    {
        SSD1306_WriteCmd(0xB0 + i);
        SSD1306_WriteCmd(0x0C);
        SSD1306_WriteCmd(0x11);
        SSD1306_WriteDatas(SSD1306_GRAM[i], SSD1306_WIDTH);
    }
}

/**
 *  @brief  SSD1306_Clear
 */
void SSD1306_Clear( const uint32_t color )
{
    uint32_t bit = (color) ? 0xFF : 0x00;
    for (uint32_t h = 0; h < SSD1306_HIGHT_BYTE; h++)
    {
        for (uint32_t w = 0; w < SSD1306_WIDTH; w++)
        {
            SSD1306_GRAM[h][w] = bit;
        }
    }
    SSD1306_Refresh();
}

/**
 *  @brief  SSD1306_DrawPixel
 */
void SSD1306_DrawPixel( const uint32_t posX, const uint32_t posY, const uint32_t color )
{
    uint32_t offsetX = posX + 0x1C;
    uint32_t pageY = posY >> 3;
    uint8_t bit = 1 << (posY % 8);
    if (color)
    {
        SSD1306_GRAM[pageY][posX] |= bit;
    }
    else
    {
        SSD1306_GRAM[pageY][posX] &= ~bit;
    }
    SSD1306_WriteCmd(0xB0 + pageY);
    SSD1306_WriteCmd(offsetX & 0x0F);
    SSD1306_WriteCmd(0x10 | ((offsetX & 0xF0) >> 4));
    SSD1306_WriteData(SSD1306_GRAM[pageY][posX]);
}

/**
 *  @brief  SSD1306_DrawLineX
 */
void SSD1306_DrawLineX( const uint32_t posX, const uint32_t posY, const uint32_t length, const uint32_t color )
{
    if ((posX + length) > SSD1306_WIDTH)
    {
        return;
    }

    for (uint32_t i = posX; i < posX + length; i++)
    {
        SSD1306_DrawPixel(i, posY, color);
    }
}

/**
 *  @brief  SSD1306_DrawLineY
 */
void SSD1306_DrawLineY( const uint32_t posX, const uint32_t posY, const uint32_t length, const uint32_t color )
{
    if ((posY + length) > SSD1306_HIGHT)
    {
        return;
    }

    for (uint32_t i = posY; i < posY + length; i++)
    {
        SSD1306_DrawPixel(posX, i, color);
    }
}

/**
 *  @brief  SSD1306_DrawRect
 */
void SSD1306_DrawRect( const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height, const uint32_t color )
{
    if ((width > SSD1306_WIDTH) || (height > SSD1306_HIGHT))
    {
        return;
    }
    if (((posX + width) > SSD1306_WIDTH) || ((posY + height) > SSD1306_HIGHT))
    {
        return;
    }
    SSD1306_DrawLineX(posX,             posY,              width,      color);
    SSD1306_DrawLineX(posX,             posY + height - 1, width,      color);
    SSD1306_DrawLineY(posX,             posY + 1,          height - 2, color);
    SSD1306_DrawLineY(posX + width - 1, posY + 1,          height - 2, color);
}

/**
 *  @brief  SSD1306_DrawRectFill
 */
void SSD1306_DrawRectFill( const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height, const uint32_t color )
{
    if ((width > SSD1306_WIDTH) || (height > SSD1306_HIGHT))
    {
        return;
    }
    if (((posX + width) > SSD1306_WIDTH) || ((posY + height) > SSD1306_HIGHT))
    {
        return;
    }

    for (uint32_t x = posX; x < posX + width; x++)
    {
        for (uint32_t y = posY; y < posY + height; y++)
        {
            SSD1306_DrawPixel(x, y, color);
        }
    }
}

/**
 *  @brief  SSD1306_PutFint
 */

#define FONT8x6_H       1
#define FONT8x6_W       6
extern const uint8_t font8x6[][FONT8x6_H*FONT8x6_W];

#define FONT16x11_H     2
#define FONT16x11_W     11
extern const uint8_t font16x11[][FONT16x11_H*FONT16x11_W];

#define FONT24x16_H     3
#define FONT24x16_W     16
extern const uint8_t font24x16[][FONT24x16_H*FONT24x16_W];

void SSD1306_PutFont8x6( const uint32_t posX, const uint32_t pageY, const uint8_t ch, const uint32_t color )
{
    uint8_t idx = ch;
    uint32_t offsetX = posX + 0x1C;

    switch (ch)
    {
        case '0':   idx = 0;    break;
        case '1':   idx = 1;    break;
        case '2':   idx = 2;    break;
        case '3':   idx = 3;    break;
        case '4':   idx = 4;    break;
        case '5':   idx = 5;    break;
        case '6':   idx = 6;    break;
        case '7':   idx = 7;    break;
        case '8':   idx = 8;    break;
        case '9':   idx = 9;    break;
        case 'A':   idx = 10;   break;
        case 'B':   idx = 11;   break;
        case 'E':   idx = 12;   break;
        case 'F':   idx = 13;   break;
        case 'R':   idx = 14;   break;
        case 'T':   idx = 15;   break;
        case 'V':   idx = 16;   break;
        case ' ':   idx = 17;   break;
        case '.':   idx = 18;   break;
        default:    return;
    }

    for (uint32_t y = 0; y < FONT8x6_H; y++)
    {
        for (uint32_t x = 0; x < FONT8x6_W; x++)
        {
            SSD1306_GRAM[pageY + y][posX + x] = (color) ? font8x6[idx][y * FONT8x6_W + x] : ~font8x6[idx][y * FONT8x6_W + x];
        }
        SSD1306_WriteCmd(0xB0 + pageY + y);
        SSD1306_WriteCmd(offsetX & 0x0F);
        SSD1306_WriteCmd(0x10 | ((offsetX & 0xF0) >> 4));
        SSD1306_WriteDatas(&SSD1306_GRAM[pageY + y][posX], FONT8x6_W);
    }
}

void SSD1306_PutFont16x11( const uint32_t posX, const uint32_t pageY, const uint8_t ch, const uint32_t color )
{
    uint8_t idx = ch;
    uint32_t offsetX = posX + 0x1C;

    switch (ch)
    {
        case '0':   idx = 0;    break;
        case '1':   idx = 1;    break;
        case '2':   idx = 2;    break;
        case '3':   idx = 3;    break;
        case '4':   idx = 4;    break;
        case '5':   idx = 5;    break;
        case '6':   idx = 6;    break;
        case '7':   idx = 7;    break;
        case '8':   idx = 8;    break;
        case '9':   idx = 9;    break;
        case 'v':   idx = 10;   break;
        case ' ':   idx = 11;   break;
        case '.':   idx = 12;   break;
        default:    return;
    }

    for (uint32_t y = 0; y < FONT16x11_H; y++)
    {
        for (uint32_t x = 0; x < FONT16x11_W; x++)
        {
            SSD1306_GRAM[pageY + y][posX + x] = (color) ? font16x11[idx][y * FONT16x11_W + x] : ~font16x11[idx][y * FONT16x11_W + x];
        }
        SSD1306_WriteCmd(0xB0 + pageY + y);
        SSD1306_WriteCmd(offsetX & 0x0F);
        SSD1306_WriteCmd(0x10 | ((offsetX & 0xF0) >> 4));
        SSD1306_WriteDatas(&SSD1306_GRAM[pageY + y][posX], FONT16x11_W);
    }
}

#include <stdio.h>
void SSD1306_PutVoltage( const uint32_t posX, const uint32_t pageY, const float voltage, const uint32_t color )
{
    uint32_t cnt = 0;
    char sbuf[16] = {0};
    cnt = sprintf(sbuf, "%5.2fv", voltage);
    for (uint32_t i = 0; i < cnt; i++)
    {
        SSD1306_PutFont16x11(posX + i * FONT16x11_W, pageY, sbuf[i], color);
    }
}

void SSD1306_PutBatteryVoltage( const uint32_t posX, const uint32_t pageY, const float battVoltage, const uint32_t color )
{
    uint32_t cnt = 0;
    char sbuf[16] = {0};
    cnt = sprintf(sbuf, "BAT %4.2fV", battVoltage);
    for (uint32_t i = 0; i < cnt; i++)
    {
        SSD1306_PutFont8x6(posX + i * FONT8x6_W, pageY, sbuf[i], color);
    }
}

void SSD1306_PutReferenceVoltage( const uint32_t posX, const uint32_t pageY, const float refVoltage, const uint32_t color )
{
    uint32_t cnt = 0;
    char sbuf[16] = {0};
    cnt = sprintf(sbuf, "REF %4.2fV", refVoltage);
    for (uint32_t i = 0; i < cnt; i++)
    {
        SSD1306_PutFont8x6(posX + i * FONT8x6_W, pageY, sbuf[i], color);
    }
}

//void SSD1306_DrawPixel( const uint32_t posX, const uint32_t posY, const uint32_t color )
//{
//    uint32_t offsetX = posX + 0x1C;
//    uint32_t pageY = posY >> 3;
//    uint8_t bit = 1 << (posY % 8);
//    if (color)
//    {
//        SSD1306_GRAM[pageY][posX] |= bit;
//    }
//    else
//    {
//        SSD1306_GRAM[pageY][posX] &= ~bit;
//    }
//    SSD1306_WriteCmd(0xB0 + pageY);
//    SSD1306_WriteCmd(offsetX & 0x0F);
//    SSD1306_WriteCmd(0x10 | ((offsetX & 0xF0) >> 4));
//    SSD1306_WriteData(SSD1306_GRAM[pageY][posX]);
//}
/*************************************** END OF FILE ****************************************/

//void SSD1306_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
//{
//uint8_t i,m,n;
//i=y/8;
//m=y%8;
//n=1<<m;
//if(t){SSD1306_GRAM[i][x]|=n;}
//else
//{
//SSD1306_GRAM[i][x]=~SSD1306_GRAM[i][x];
//SSD1306_GRAM[i][x]|=n;
//SSD1306_GRAM[i][x]=~SSD1306_GRAM[i][x];
//}
//}

void SSD1306_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
    uint16_t j=0;
    uint8_t i,n,temp,m;
    uint8_t x0=x,y0=y;
    sizey=sizey/8+((sizey%8)?1:0);
    for(n=0;n<sizey;n++)
    {
        for(i=0;i<sizex;i++)
        {
            temp=BMP[j];
            j++;
            for(m=0;m<8;m++)
            {
                if(temp&0x01)
                    SSD1306_DrawPixel(x,y,mode);
                else
                    SSD1306_DrawPixel(x,y,!mode);
                temp>>=1;
                y++;
            }
            x++;
            if((x-x0)==sizex)
            {
                x=x0;
                y0=y0+8;
            }
            y=y0;
        }
    }
}





















/*
void OLED_ColorTurn(u8 i)
{
if(i==0)
{
	OLED_WR_Byte(0xA6,OLED_CMD);//ֽӣДʾ
}
if(i==1)
{
	OLED_WR_Byte(0xA7,OLED_CMD);//״ɫДʾ
}
}

void OLED_DisplayTurn(u8 i)
{
if(i==0)
{
OLED_WR_Byte(0xC8,OLED_CMD);//ֽӣДʾ
OLED_WR_Byte(0xA1,OLED_CMD);
}
if(i==1)
{
OLED_WR_Byte(0xC0,OLED_CMD);//״תДʾ
OLED_WR_Byte(0xA0,OLED_CMD);
}
}

void OLED_DisPlay_On(void)
{
OLED_WR_Byte(0x8D,OLED_CMD);//֧ۉ҃ʹŜ
OLED_WR_Byte(0x14,OLED_CMD);//ߪǴ֧ۉ҃
OLED_WR_Byte(0xAF,OLED_CMD);//֣ǁĻ
}

void OLED_DisPlay_Off(void)
{
OLED_WR_Byte(0x8D,OLED_CMD);//֧ۉ҃ʹŜ
OLED_WR_Byte(0x10,OLED_CMD);//ژҕ֧ۉ҃
OLED_WR_Byte(0xAE,OLED_CMD);//ژҕǁĻ
}

void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
u8 i,m,n;
i=y/8;
m=y%8;
n=1<<m;
if(t){OLED_GRAM[x][i]|=n;}
else
{
OLED_GRAM[x][i]=~OLED_GRAM[x][i];
OLED_GRAM[x][i]|=n;
OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}
}

void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
u16 t; 
int xerr=0,yerr=0,delta_x,delta_y,distance;
int incx,incy,uRow,uCol;
delta_x=x2-x1; //݆̣ظҪն 
delta_y=y2-y1;
uRow=x1;//ۭПǰ֣ظҪ
uCol=y1;
if(delta_x>0)incx=1; //ʨ׃եҽ׽в 
else if (delta_x==0)incx=0;//ԹֱП 
else {incx=-1;delta_x=-delta_x;}
if(delta_y>0)incy=1;
else if (delta_y==0)incy=0;//ˮƽП 
else {incy=-1;delta_y=-delta_x;}
if(delta_x>delta_y)distance=delta_x; //ѡȡܹѾնظҪס 
else distance=delta_y;
for(t=0;t<distance+1;t++)
{
OLED_DrawPoint(uRow,uCol,mode);//ۭ֣
xerr+=delta_x;
yerr+=delta_y;
if(xerr>distance)
{
xerr-=distance;
uRow+=incx;
}
if(yerr>distance)
{
yerr-=distance;
uCol+=incy;
}
}
}

void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
int a, b,num;
a = 0;
b = r;
while(2 * b * b >= r * r)      
{
OLED_DrawPoint(x + a, y - b,1);
OLED_DrawPoint(x - a, y - b,1);
OLED_DrawPoint(x - a, y + b,1);
OLED_DrawPoint(x + a, y + b,1);

OLED_DrawPoint(x + b, y + a,1);
OLED_DrawPoint(x + b, y - a,1);
OLED_DrawPoint(x - b, y - a,1);
OLED_DrawPoint(x - b, y + a,1);

a++;
num = (a * a + b * b) - r*r;
if(num > 0)
{
b--;
a--;
}
}
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
u8 i,m,temp,size2,chr1;
u8 x0=x,y0=y;
if(size1==8)size2=6;
else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //փսؖͥһٶؖػהӦֳ֣ܯ̹ռքؖޚ˽
chr1=chr-' ';  //݆̣ƫӆ۳քֵ
for(i=0;i<size2;i++)
{
if(size1==8)
{temp=asc2_0806[chr1][i];} //ַԃ0806ؖͥ
else if(size1==12)
{temp=asc2_1206[chr1][i];} //ַԃ1206ؖͥ
else if(size1==16)
{temp=asc2_1608[chr1][i];} //ַԃ1608ؖͥ
else if(size1==24)
{temp=asc2_2412[chr1][i];} //ַԃ2412ؖͥ
else return;
for(m=0;m<8;m++)
{
if(temp&0x01)OLED_DrawPoint(x,y,mode);
else OLED_DrawPoint(x,y,!mode);
temp>>=1;
y++;
}
x++;
if((size1!=8)&&((x-x0)==size1/2))
{x=x0;y0=y0+8;}
y=y0;
}
}

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
while((*chr>=' ')&&(*chr<='~'))//Ɛ׏ˇһˇ؇רؖػ!
{
OLED_ShowChar(x,y,*chr,size1,mode);
if(size1==8)x+=6;
else x+=size1/2;
chr++;
}
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
u32 result=1;
while(n--)
{
result*=m;
}
return result;
}

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
u8 t,temp,m=0;
if(size1==8)m=2;
for(t=0;t<len;t++)
{
temp=(num/OLED_Pow(10,len-t-1))%10;
if(temp==0)
{
OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
}
else 
{
OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
}
}
}

void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
u8 m,temp;
u8 x0=x,y0=y;
u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //փսؖͥһٶؖػהӦֳ֣ܯ̹ռքؖޚ˽
for(i=0;i<size3;i++)
{
if(size1==16)
{temp=Hzk1[num][i];}//ַԃ16*16ؖͥ
else if(size1==24)
{temp=Hzk2[num][i];}//ַԃ24*24ؖͥ
else if(size1==32)       
{temp=Hzk3[num][i];}//ַԃ32*32ؖͥ
else if(size1==64)
{temp=Hzk4[num][i];}//ַԃ64*64ؖͥ
else return;
for(m=0;m<8;m++)
{
if(temp&0x01)OLED_DrawPoint(x,y,mode);
else OLED_DrawPoint(x,y,!mode);
temp>>=1;
y++;
}
x++;
if((x-x0)==size1)
{x=x0;y0=y0+8;}
y=y0;
}
}

void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
u8 i,n,t=0,m=0,r;
while(1)
{
if(m==0)
{
OLED_ShowChinese(72,13,t,16,mode); //дɫһٶںؖѣզ՚OLED_GRAM[][]˽ةא
t++;
}
if(t==num)
{
for(r=0;r<16*space;r++)      //Дʾݤٴ
{
for(i=1;i<88;i++)
{
for(n=0;n<5;n++)
{
OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
}
}
OLED_Refresh();
}
t=0;
}
m++;
if(m==16){m=0;}
for(i=1;i<88;i++)   //ʵЖسӆ
{
for(n=0;n<5;n++)
{
OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
}
}
OLED_Refresh();
}
}
*/
