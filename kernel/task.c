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
#define DUMMY_XPSR	    (0x01000000)
#define MAX_TASKS       (3)
#define MAX_STACK_SIZE  (100)

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */
static uint32_t tasks_counter = 0;
static uint32_t stacks[MAX_TASKS][MAX_STACK_SIZE];
static task_control_block_t tcb_pool[MAX_TASKS];

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void task_create(task_func_t task_function, uint32_t priority, uint32_t stack_size)
{
    if (MAX_TASKS > tasks_counter)
    {
        uint32_t st_size = (MAX_STACK_SIZE >= stack_size) ? stack_size : MAX_STACK_SIZE;
        task_control_block_t* tcb = &tcb_pool[tasks_counter];
        
        tcb->priority   = priority;
        tcb->state      = READY;
        tcb->wake_tick  = 0;
        tcb->next       = NULL;
        tcb->stack_size = st_size;
        tcb->stack_ptr  = &stacks[tasks_counter][st_size - 16];

        stacks[tasks_counter][st_size - 1] = DUMMY_XPSR;                 // xPSR (bit Thumb)
        stacks[tasks_counter][st_size - 2] = (uint32_t)task_function;    // PC
        stacks[tasks_counter][st_size - 3] = 0xFFFFFFFD;                 // LR
        stacks[tasks_counter][st_size - 4] = 0x00000000;                 // R12
        stacks[tasks_counter][st_size - 5] = 0x00000000;                 // R3
        stacks[tasks_counter][st_size - 6] = 0x00000000;                 // R2
        stacks[tasks_counter][st_size - 7] = 0x00000000;                 // R1
        stacks[tasks_counter][st_size - 8] = 0x00000000;                 // R0

        stacks[tasks_counter][st_size - 9]  = 0x00000000;                 // R11
        stacks[tasks_counter][st_size - 10] = 0x00000000;                 // R10
        stacks[tasks_counter][st_size - 11] = 0x00000000;                 // R9
        stacks[tasks_counter][st_size - 12] = 0x00000000;                 // R8
        stacks[tasks_counter][st_size - 13] = 0x00000000;                 // R7
        stacks[tasks_counter][st_size - 14] = 0x00000000;                 // R6
        stacks[tasks_counter][st_size - 15] = 0x00000000;                 // R5
        stacks[tasks_counter][st_size - 16] = 0x00000000;                 // R4
        
        tasks_counter++;
    }
}

uint32_t task_get_tasks_counter(void)
{
    return tasks_counter;
}

task_control_block_t* task_get_tcb(uint32_t index)
{
    return &tcb_pool[index];
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
