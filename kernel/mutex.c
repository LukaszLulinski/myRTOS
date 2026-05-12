/*------------------------------------------------------------------------------------*/
/*!
 * \file  mutex.c 
 * \brief Handling mutexes
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stddef.h>
#include "mutex.h"
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
/* Static functions definitions                                                       */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void mutex_init(mutex_t* mutex)
{
    mutex->locked       = 0u;
    mutex->owner        = NULL;
    mutex->blocked_list = NULL;
}

void mutex_lock(mutex_t* mutex)
{
    if (!mutex->locked)
    {
        mutex->locked = 1u;
        mutex->owner  = current_task;
    }
    else
    {
        current_task->state = BLOCKED;

        if (!mutex->blocked_list)
        {
            mutex->blocked_list = current_task;
        }
        else
        {
            /* Add to the end of blocked linked list */
            task_control_block_t* task = mutex->blocked_list;

            while (task->next)
            {
                task = task->next;
            }

            task->next = current_task;
        }

        /* Trigger PendSV interrupt */
        ICSR |= (1 << 28);
    }
}

void mutex_unlock(mutex_t* mutex)
{
    mutex->locked = 0u;
    mutex->owner  = NULL;

    if (mutex->blocked_list)
    {
        /* It's FIFO, so remove the first task from the blocked list */
        task_control_block_t* task = mutex->blocked_list;

        if (mutex->blocked_list->next)
        {
            mutex->blocked_list = mutex->blocked_list->next;
        }
        else
        {
            mutex->blocked_list = NULL;
        }

        task->state = READY;
        task->next = NULL;
        
        /* Trigger PendSV interrupt */
        ICSR |= (1 << 28);
    }
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
