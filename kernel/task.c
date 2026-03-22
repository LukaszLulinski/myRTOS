/*------------------------------------------------------------------------------------*/
/*!
 * \file  task.c 
 * \brief Handling tasks
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stddef.h>
#include "task.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */
#define MAX_TASK    (3)
#define MAX_STACK_SIZE  (100)

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */
static uint32_t tasks_counter;
static uint32_t stacks[MAX_TASK][MAX_STACK_SIZE];
task_control_block_t tcb_pool[MAX_TASK];

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void task_create(task_func_t task_function, uint32_t priority, uint32_t stack_size)
{
    if (MAX_TASK > tasks_counter)
    {
        uint32_t st_size = (MAX_STACK_SIZE >= stack_size) ? stack_size : MAX_STACK_SIZE;
        task_control_block_t* tcb = &tcb_pool[tasks_counter];
        
        tcb->priority   = priority;
        tcb->state      = READY;
        tcb->wake_tick  = 0;
        tcb->next       = NULL;
        tcb->stack_size = st_size;
        tcb->stack_ptr  = &stacks[tasks_counter][st_size - 8];

        stacks[tasks_counter][st_size - 1] = 0x01000000;                 // xPSR
        stacks[tasks_counter][st_size - 2] = (uint32_t)task_function;    // PC
        stacks[tasks_counter][st_size - 3] = 0x00000000;                 // LR
        stacks[tasks_counter][st_size - 4] = 0x00000000;                 // R12
        stacks[tasks_counter][st_size - 5] = 0x00000000;                 // R3
        stacks[tasks_counter][st_size - 6] = 0x00000000;                 // R2
        stacks[tasks_counter][st_size - 7] = 0x00000000;                 // R1
        stacks[tasks_counter][st_size - 8] = 0x00000000;                 // R0
        
        tasks_counter++;
    }
}

uint32_t task_get_tasks_counter(void)
{
    return tasks_counter;
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
