/*------------------------------------------------------------------------------------*/
/*!
 * \file  semaphore.h 
 * \brief Handling semaphores
 */
/*------------------------------------------------------------------------------------*/

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

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
    uint32_t count;
    task_control_block_t* blocked_list;
} semaphore_t;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void semaphore_init(semaphore_t* semaphore, uint32_t initial_count);
void semaphore_wait(semaphore_t* semaphore);
void semaphore_signal(semaphore_t* semaphore);

#endif /* SEMAPHORE_H */
