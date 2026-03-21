
#include "systick.h"

#define UART0_BASE   0x40004000
#define UART_DATA    (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART_STATE   (*(volatile unsigned int *)(UART0_BASE + 0x04))
#define UART_CTRL    (*(volatile unsigned int *)(UART0_BASE + 0x08))
#define UART_BAUDDIV (*(volatile unsigned int *)(UART0_BASE + 0x10))

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

void main(void)
{
    uint32_t last = 0;

    uart_init();
    systick_init(1000);  // 1000 interrupts every second

    uart_print("SysTick test start\n");

    while (1)
    {
        uint32_t now = systick_get_tick();

        if (now - last >= 1000) // every 1000ms
        {
            uart_print("tick: ");
            uart_print_uint(now);
            uart_print("\r");
            last = now;
        }
    }
}
