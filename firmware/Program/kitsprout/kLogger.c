/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kLogger.c
 *  @author  KitSprout
 *  @date    Mar-2020
 *  @brief   
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>

#include "SEGGER_RTT_Conf.h"
#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/

#define ENABLE_SEGGER_RTT   (1U)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static char klogdBuf[BUFFER_SIZE_UP];
static char klogcBuf[BUFFER_SIZE_UP];
static uint32_t idx;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  klogd
 */
int klogd( const char *fmt, ... )
{
    int lens;
    va_list aptr;
    va_start(aptr, fmt);
    lens = vsprintf(klogdBuf, fmt, aptr);
    va_end(aptr);
    klog_write(klogdBuf, lens);
    return lens;
}

/**
 *  @brief  klogc
 */
int klogc( const char *fmt, ... )
{
    int lens;
    if (fmt != NULL)
    {
        va_list aptr;
        va_start(aptr, fmt);
        idx += vsprintf(&klogcBuf[idx], fmt, aptr);
        va_end(aptr);
        lens = idx;
    }
    else
    {
        lens = idx;
        klogcBuf[idx] = 0;
        klog_write(klogcBuf, idx);
        idx = 0;
    }

    return lens;
}

/**
 *  @brief  klog_init
 */
int klog_init( const int mode )
{
    // default mode: SEGGER_RTT_MODE_NO_BLOCK_SKIP
    return SEGGER_RTT_ConfigUpBuffer(KLOGGER_BUFFER_INDEX, NULL, NULL, 0, mode);
}

/**
 *  @brief  klog_scope_init
 *
 *  t4: timestamp (32bit, us)
 *  i1,i2,i4: 8,16,32bits signed
 *  u1,u2,u4: 8,16,32bits unsigned
 *
 */
int klog_scope_init( uint8_t *buf, const uint32_t lens, const char *format )
{
    return SEGGER_RTT_ConfigUpBuffer(KLOGGER_JSCOPE_BUFFER_INDEX, format, buf, lens, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

/**
 *  @brief  klog_scope_send
 */
int klog_scope_send( const void *buf, const uint32_t lens )
{
    return SEGGER_RTT_Write(KLOGGER_JSCOPE_BUFFER_INDEX, buf, lens);
}

#if KLOGGER_PRINTF_SUPPORT
/**
 *  @brief  fputc
 */
int fputc( int ch, FILE *f )
{
    char ch8 = ch;
    klog_write(&ch8, 1);
    return (ch);
}

/**
 *  @brief  fgetc
 */
int fgetc( FILE *f )
{
    return klog_waitkey();
}
#endif

/*************************************** END OF FILE ****************************************/
