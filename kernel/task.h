/*------------------------------------------------------------------------------------*/
/*!
 * \file  task.h 
 * \brief Handling tasks
 */
/*------------------------------------------------------------------------------------*/

#ifndef TASK_H
#define TASK_H

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stdint.h>

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */
typedef void (*task_func_t)(void);

typedef enum
{
    READY,
    RUNNING,
    BLOCKED,
    BLOCKED_DELAY,
    SUSPENDED
} task_state_t;

typedef struct TCB
{
    uint32_t*    stack_ptr;
    uint32_t     priority;
    task_state_t state;
    uint32_t     wake_tick;
    uint32_t     stack_size;
    struct TCB* next;
} task_control_block_t;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void task_init(void);
void task_create(task_func_t task_function, uint32_t priority, uint32_t stack_size);
uint32_t task_get_tasks_counter(void);
task_control_block_t* task_get_tcb(uint32_t index);
void task_delay(uint32_t ticks);
void task_delay_until(uint32_t* last_wake_tick, uint32_t ticks);
void task_delay_update(void);

#endif /* TASK_H */
