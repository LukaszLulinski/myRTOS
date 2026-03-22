/*------------------------------------------------------------------------------------*/
/*!
 * \file  systick.c 
 * \brief Handling SysTick
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include "systick.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */
/* Registers SysTick */
#define SYST_CSR  (*(volatile uint32_t *)0xE000E010) // control and status
#define SYST_RVR  (*(volatile uint32_t *)0xE000E014) // reload value
#define SYST_CVR  (*(volatile uint32_t *)0xE000E018) // current value

/* Frequency */
#define SYSTEM_CLOCK 25000000  // 25 MHz

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */
static volatile uint32_t tick_count = 0;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void systick_init(uint32_t ticks_per_second)
{
    SYST_RVR = (SYSTEM_CLOCK / ticks_per_second) - 1;
    SYST_CVR = 0;
    /* bit 2 = clksource (core clock), bit 1 = tickint (interrupt), bit 0 = enable */
    SYST_CSR = 0x7;
}

uint32_t systick_get_tick(void)
{
    return tick_count;
}

/* Interrupt handler */
void systick_handler(void)
{
    tick_count++;
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
