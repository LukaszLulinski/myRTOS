/*------------------------------------------------------------------------------------*/
/*!
 * \file  queue.c 
 * \brief Handling queues
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include <stddef.h>
#include "queue.h"

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
void* memcpy(void* dest, const void* src, uint32_t n);

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void queue_init(queue_t* q, uint32_t item_size)
{
    if ((item_size == 0) || (item_size > MAX_ITEM_SIZE))
    {
        item_size = MAX_ITEM_SIZE;
    }

    q->item_size = item_size;
    q->head = 0;
    q->tail = 0;
    semaphore_init(&q->free_slots, MAX_QUEUE_SIZE);
    semaphore_init(&q->used_slots, 0u);
}

void queue_push(queue_t* q, const void* item)
{
    semaphore_wait(&q->free_slots);
    
    memcpy(&q->data[q->tail * q->item_size], item, q->item_size);
    q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
    
    semaphore_signal(&q->used_slots);
}

void queue_pop(queue_t* q, void* item)
{
    semaphore_wait(&q->used_slots);
    
    memcpy(item, &q->data[q->head * q->item_size], q->item_size);
    q->head = (q->head + 1) % MAX_QUEUE_SIZE;
    
    semaphore_signal(&q->free_slots);
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */
void* memcpy(void* dest, const void* src, uint32_t n)
{
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    
    for (uint32_t i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
    
    return dest;
}
