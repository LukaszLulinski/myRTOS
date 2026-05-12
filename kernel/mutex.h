/*------------------------------------------------------------------------------------*/
/*!
 * \file  mutex.h 
 * \brief Handling mutexes
 */
/*------------------------------------------------------------------------------------*/

#ifndef MUTEX_H
#define MUTEX_H

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stdint.h>
#include "task.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */
typedef struct
{
    uint32_t locked;
    task_control_block_t* owner;
    task_control_block_t* blocked_list;
} mutex_t;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void mutex_init(mutex_t* mutex);
void mutex_lock(mutex_t* mutex);
void mutex_unlock(mutex_t* mutex);

#endif /* MUTEX_H */
