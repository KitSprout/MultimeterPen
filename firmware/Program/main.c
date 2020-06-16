/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    main.c
 *  @author  KitSprout
 *  @brief   main function
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "kitsprout\kSystem.h"
#include "drivers\stm32l0_adc.h"
#include "modules\serial.h"
#include "modules\ssd1306.h"

#include <stdio.h>
#include "stm32x_bsp.h"

/* Define ----------------------------------------------------------------------------------*/

#define TIMER_TICK_FREQ     (25000)     // 25kHz, 40us
#define UPDATE_FREQ         (500)       // 50Hz

#define MA_WEIGHTING_H      (0.2f)
#define MA_WEIGHTING        (0.5f)

#define SHORT_THRESHOLD     (0.5f)
#define DIODE_THRESHOLD     (1.0f)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

enum
{
    MODE_VOLTAGE    = 0,
    MODE_SHORT      = 1,
    MODE_DIODE      = 2,
    MODE_COUNT,

} measureMode;

/* Variables -------------------------------------------------------------------------------*/

uint32_t tick = 0;
uint32_t beepcmd = 0;
uint32_t beepcnt = 0;

/* Prototypes ------------------------------------------------------------------------------*/
void event_timer_tick( void );

/* Functions -------------------------------------------------------------------------------*/

void bsp_init( void )
{
    bsp_gpio_init();
    bsp_timer_init(event_timer_tick, TIMER_TICK_FREQ);
    bsp_uart_serial_init(NULL);
    bsp_probe_init();

    delay_ms(200);
    SSD1306_Config();
    delay_ms(200);
    SSD1306_Init();

    bsp_timer_enable(ENABLE);
}

void beep( uint32_t cmd )
{
    beepcmd = cmd;
}

int main( void )
{
    bsp_init();

    uint32_t mode = MODE_VOLTAGE;

    uint16_t vprobeLsb = ADC_GetValue(0);
    uint16_t vbattLsb = ADC_GetValue(1);
    uint16_t VREFINT_DATA = ADC_GetValue(2);
    float vref = 3.0f * VREFINT_CAL / VREFINT_DATA;
    float vbatt = (vbattLsb / 4096.0f) * vref * 2;
    float vprobe = ((vprobeLsb - 50) / 4096.0f) * vref * (17/2.0f);
    float f_vref = vref, f_vbatt = vbatt, f_vprobe = vprobe;

    while (1)
    {
        if (tick >= (TIMER_TICK_FREQ / UPDATE_FREQ))
        {
            tick = 0;

            vprobeLsb = ADC_GetValue(0);
            vbattLsb = ADC_GetValue(1);
            VREFINT_DATA = ADC_GetValue(2);
            vref = 3.0f * VREFINT_CAL / VREFINT_DATA;
            vbatt = (vbattLsb / 4096.0f) * vref * 2;
            vprobe = ((vprobeLsb - 50) / 4096.0f) * vref * (17/2.0f);

            f_vref = (MA_WEIGHTING_H) * vref + (1.0f - MA_WEIGHTING_H) * f_vref;
            f_vbatt = (MA_WEIGHTING_H) * vbatt + (1.0f - MA_WEIGHTING_H) * f_vbatt;
            f_vprobe = (MA_WEIGHTING) * vprobe + (1.0f - MA_WEIGHTING) * f_vprobe;

            if (KEY_READ())
            {
                if (++mode == MODE_COUNT)
                {
                    mode = MODE_VOLTAGE;
                }

                beep(1);
                switch (mode)
                {
                    case MODE_VOLTAGE:
                    {
                        LED_On();
                        delay_ms(250);
                        LED_Off();
                        PROBE_OUT_Off();
                        break;
                    }
                    case MODE_SHORT:
                    {
                        LED_On();
                        delay_ms(250);
                        LED_Off();
                        PROBE_OUT_On();
                        vprobe = 5;
                        break;
                    }
                    case MODE_DIODE:
                    {
                        LED_On();
                        delay_ms(250);
                        LED_Off();
                        PROBE_OUT_On();
                        vprobe = 5;
                        break;
                    }
                }
                beep(0);
            }

            switch (mode)
            {
                case MODE_VOLTAGE:
                {
                    SSD1306_PutVoltage(5, 0, f_vprobe, 1);
                    SSD1306_PutBatteryVoltage(17, 3, f_vbatt, 1);
                    SSD1306_PutReferenceVoltage(17, 4, f_vref, 1);
//                    printf("vin = %.2fv (%6d), probe = %.2fv (%6d), vref = %.2fv (cal %6d, data %6d)\r\n",
//                        vin, vinLsb, probe, probeLsb, vref, VREFINT_CAL, VREFINT_DATA);
                    break;
                }
                case MODE_SHORT:
                {
                    beep(vprobe < SHORT_THRESHOLD);

                    SSD1306_PutVoltage(5, 0, f_vprobe, 1);
                    SSD1306_PutBatteryVoltage(17, 3, f_vbatt, 1);
                    SSD1306_PutReferenceVoltage(17, 4, SHORT_THRESHOLD, 1);
                    break;
                }
                case MODE_DIODE:
                {
                    beep(vprobe < DIODE_THRESHOLD);

                    SSD1306_PutVoltage(5, 0, f_vprobe, 1);
                    SSD1306_PutBatteryVoltage(17, 3, f_vbatt, 1);
                    SSD1306_PutReferenceVoltage(17, 4, DIODE_THRESHOLD, 1);
                    break;
                }
            }
        }
    }
}

void event_timer_tick( void )
{
    tick++;

    if (beepcmd)
    {
        if (++beepcnt > 5)
        {
            beepcnt = 0;
            BUZ_Off();
        }
        else
        {
            BUZ_On();
        }
    }
    else
    {
        beepcnt = 0;
        BUZ_Off();
    }
}

/*************************************** END OF FILE ****************************************/
