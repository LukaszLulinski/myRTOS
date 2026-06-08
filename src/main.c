/*------------------------------------------------------------------------------------*/
/*!
 * \file  main.c 
 * \brief main component
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include "core.h"
#include "systick.h"
#include "scheduler.h"
#include "task.h"
#include "mutex.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */
static mutex_t mutex;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */
static void uart_init(void);
static void uart_print(const char *msg);
static void uart_print_uint(uint32_t n);
static void task1_handler(void);
static void task2_handler(void);

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void main(void)
{
    uart_init();
    
    task_create(task1_handler, 1u, 1024u);
    task_create(task2_handler, 1u, 1024u);

    mutex_init(&mutex);
    
    uart_print("Program started\n");
	
    /*! NOTE: Must be called after creating at least one task */
    scheduler_init();
    scheduler_start();
    
    systick_init(1000u);  // 1000 interrupts every second
	task1_handler();
	
    while (1)
    {
        // do nothing
    }
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
static void uart_init(void)
{
    UART_BAUDDIV = 16;
    UART_CTRL    = 0x1;
}

/* Print */
static void uart_print(const char *msg)
{
    while (*msg)
    {
        while (UART_STATE & 0x1);
        UART_DATA = *msg++;
    }
}

/* Print int */
static void uart_print_uint(uint32_t n)
{
    char buf[12];
    int i = 0;

    if (n == 0) { uart_print("0"); return; }

    while (n > 0)
    {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    /* flip */
    for (int j = i - 1; j >= 0; j--)
    {
        while (UART_STATE & 0x1);
        UART_DATA = buf[j];
    }
}

static void task1_handler(void)
{
    uint32_t last_wake_tick = systick_get_tick();
    
    while (1)
    {
        mutex_lock(&mutex);
        uart_print("task 1\n");
        mutex_unlock(&mutex);

        task_delay_until(&last_wake_tick, 1000u);
    }
}

static void task2_handler(void)
{
    uint32_t last_wake_tick = systick_get_tick();

    while (1)
    {
        mutex_lock(&mutex);
        uart_print("task 2\n");
        mutex_unlock(&mutex);

        task_delay_until(&last_wake_tick, 500u);
    }
}
