/*------------------------------------------------------------------------------------*/
/*!
 * \file  main.c 
 * \brief main component
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include "systick.h"
#include "scheduler.h"
#include "task.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */
#define UART0_BASE   0x40004000
#define UART_DATA    (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART_STATE   (*(volatile unsigned int *)(UART0_BASE + 0x04))
#define UART_CTRL    (*(volatile unsigned int *)(UART0_BASE + 0x08))
#define UART_BAUDDIV (*(volatile unsigned int *)(UART0_BASE + 0x10))

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
static void uart_init(void);
static void uart_print(const char *msg);
static void uart_print_uint(uint32_t n);
static void task1_handler(void);
static void task2_handler(void);

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void main(void)
{
    task_func_t task1_fun = task1_handler;
    task_func_t task2_fun = task2_handler;

    uart_init();
    
    task_create(task1_fun, 1, 1024);
    task_create(task2_fun, 1, 1024);
    
    uart_print("Program started\n");
	
    /*! NOTE: Must be called after creating at least one task */
    scheduler_init();
    scheduler_start();
    
    systick_init(1000);  // 1000 interrupts every second
	task1_fun();
	
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
    while (1)
    {
        uart_print("task 1");
    }
}

static void task2_handler(void)
{
    while (1)
    {
        uart_print("task 2");
    }
}
