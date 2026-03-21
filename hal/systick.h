
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void systick_init(uint32_t ticks_per_second);
uint32_t systick_get_tick(void);

#endif /* SYSTICK_H */
