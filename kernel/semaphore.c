/*------------------------------------------------------------------------------------*/
/*!
 * \file  semaphore.c 
 * \brief Handling semaphores
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stddef.h>
#include "semaphore.h"
#include "task.h"
#include "scheduler.h"
#include "core.h"

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
void semaphore_init(semaphore_t* semaphore, uint32_t initial_count)
{
    semaphore->count = initial_count;
    semaphore->blocked_list = NULL;
}

void semaphore_wait(semaphore_t* semaphore)
{
    if (semaphore->count > 0)
    {
        semaphore->count--;
    }
    else
    {
        /* Block the current task and add it to the blocked list */
        current_task->next = semaphore->blocked_list;
        semaphore->blocked_list = current_task;
        current_task->state = BLOCKED;

        /* Trigger PendSV interrupt */
        ICSR |= (1 << 28);
    }
}

void semaphore_signal(semaphore_t* semaphore)
{
    if (semaphore->blocked_list)
    {
        /* Unblock the first task in the blocked list */
        task_control_block_t* task_to_unblock = semaphore->blocked_list;
        semaphore->blocked_list = task_to_unblock->next;
        task_to_unblock->state = READY;
        task_to_unblock->next = NULL;

        /* Trigger PendSV interrupt */
        ICSR |= (1 << 28);
    }
    else
    {
        semaphore->count++;
    }    
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */
