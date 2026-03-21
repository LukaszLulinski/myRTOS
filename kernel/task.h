
#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef void (*task_func_t)(void);

typedef enum
{
    READY,
    RUNNING,
    BLOCKED,
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



void task_create(task_func_t task_function, uint32_t priority, uint32_t stack_size);
uint32_t task_get_tasks_counter(void);

#endif /* TASK_H */
