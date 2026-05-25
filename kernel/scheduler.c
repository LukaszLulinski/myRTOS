/*------------------------------------------------------------------------------------*/
/*!
 * \file  scheduler.c 
 * \brief Scheduling tasks
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stddef.h>
#include "scheduler.h"
#include "task.h"

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
task_control_block_t* current_task;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */
static void idle_task(void);

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void scheduler_init(void)
{
    task_create(idle_task, 0u, 64u);

    if (task_get_tasks_counter())
    {
        current_task = task_get_tcb(0u); 
        current_task->state = RUNNING;
    }
}

void scheduler_run(void)
{
    uint32_t tasks_counter = task_get_tasks_counter();
    task_control_block_t* best_candidate = NULL;

    for (uint32_t task_id = 0u; task_id < tasks_counter; task_id++)
    {
        task_control_block_t* task_candidate = task_get_tcb(task_id);
        
        if (READY == task_candidate->state)
        {
            if (best_candidate)
            {
                if (task_candidate->priority > best_candidate->priority)
                {
                    best_candidate = task_candidate;
                }
            }
            else
            {
                best_candidate = task_candidate;
            }
        }
    }

    if (best_candidate)
    {
        current_task->state = (RUNNING == current_task->state) ? READY : current_task->state;
        current_task = best_candidate;
        current_task->state = RUNNING;
    }
    else
    {
        /* Going to idle task */
    }
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
static void idle_task(void)
{
    while (1)
    {
        /* do nothing - wait for interrupt, going to sleep to save energy */
        __asm volatile ("WFI");
    }
}
