/*------------------------------------------------------------------------------------*/
/*!
 * \file  timer.h 
 * \brief Handling timers
 */
/*------------------------------------------------------------------------------------*/

#ifndef TIMER_H
#define TIMER_H

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stdint.h>
#include <stdbool.h>

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */
#define MAX_TIMERS (10)

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */
typedef void (*timer_func_t)(void);

typedef struct timer_t
{
    uint32_t     interval_ticks;
    uint32_t     expire_tick;
    timer_func_t func;
    bool         cyclic;
    bool         active;
} timer_t;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */
static timer_t timers_pool[MAX_TIMERS];

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void timer_init(void);
timer_t* timer_start(uint32_t delay_ticks, bool cyclic, timer_func_t func);
void timer_stop(timer_t* timer);
void timer_update(void);

#endif /* TIMER_H */
