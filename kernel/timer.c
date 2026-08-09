/*------------------------------------------------------------------------------------*/
/*!
 * \file  timer.c 
 * \brief Handling timers
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stddef.h>
#include "timer.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void timer_init(void)
{
    for (uint32_t timer_id = 0; timer_id < MAX_TIMERS; timer_id++)
    {
        timers_pool[timer_id].active = false;
    }
}

timer_t* timer_start(uint32_t delay_ticks, bool cyclic, timer_func_t func)
{
    for (uint32_t timer_id = 0; timer_id < MAX_TIMERS; timer_id++)
    {
        timer_t* timer = &timers_pool[timer_id];
        
        if (timer->active == false)
        {
            timer->expire_tick = delay_ticks;
            timer->interval_ticks = delay_ticks;
            timer->func = func;
            timer->cyclic = cyclic;
            timer->active = true;
            return timer;
        }
    }

    return NULL;
}

void timer_stop(timer_t* timer)
{
    timer->active = false;
}

void timer_update(void)
{
    for (uint32_t timer_id = 0; timer_id < MAX_TIMERS; timer_id++)
    {
        timer_t* timer = &timers_pool[timer_id];
        
        if (timer->active)
        {
            if (timer->expire_tick > 0)
            {
                timer->expire_tick--;
            }
            else
            {
                if (timer->func)
                {
                    timer->func();
                }

                if (timer->cyclic)
                {
                    timer->expire_tick = timer->interval_ticks;
                }
                else
                {
                    timer->active = false;
                }
            }
        }
    }
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */
