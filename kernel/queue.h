/*------------------------------------------------------------------------------------*/
/*!
 * \file  queue.h 
 * \brief Handling queues
 */
/*------------------------------------------------------------------------------------*/

#ifndef QUEUE_H
#define QUEUE_H

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stdint.h>
#include "semaphore.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */
#define MAX_QUEUE_SIZE (10)
#define MAX_ITEM_SIZE  (8)

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */
typedef struct
{
    uint8_t data[MAX_QUEUE_SIZE * MAX_ITEM_SIZE];
    uint32_t item_size;
    uint32_t head;
    uint32_t tail;
    semaphore_t free_slots;
    semaphore_t used_slots;
} queue_t;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void queue_init(queue_t* q, uint32_t item_size);
void queue_push(queue_t* q, const void* item);
void queue_pop(queue_t* q, void* item);

#endif /* QUEUE_H */
