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
#include "semaphore.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */
static mutex_t mutex;
static semaphore_t semaphore;
static uint32_t shared_counter = 0;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
static void uart_init(void);
static void uart_print(const char *msg);
static void uart_print_uint(uint32_t n);
static void producer(void);
static void consumer(void);

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void main(void)
{
    uart_init();
    
    task_create(producer, 1u, 128u);
    task_create(consumer, 1u, 128u);

    mutex_init(&mutex);
    semaphore_init(&semaphore, 0u);

    uart_print("Program started\n");
	
    systick_init(1000u); // 1000 interrupts every second

    /*! NOTE: Must be called after creating at least one task */
    scheduler_init();
	
    while (1)
    {
        // do nothing
    }
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */
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

static void producer(void)
{
    uint32_t last_wake = systick_get_tick();
    while (1)
    {
        shared_counter++;
        uart_print("produced: ");
        uart_print_uint(shared_counter);
        uart_print("\n");
        semaphore_signal(&semaphore);

        task_delay_until(&last_wake, 1000);
    }
}

static void consumer(void)
{
    while (1)
    {
        semaphore_wait(&semaphore);
        uart_print("consumed: ");
        uart_print_uint(shared_counter);
        uart_print("\n");
    }
}
