/*------------------------------------------------------------------------------------*/
/*!
 * \file  systick.c 
 * \brief Handling SysTick
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include "systick.h"
#include "core.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */
static volatile uint32_t tick_count = 0;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

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
    /* Trigger PendSV interrupt */
    ICSR |= (1 << 28);

    tick_count++;
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
