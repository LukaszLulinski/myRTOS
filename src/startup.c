/*------------------------------------------------------------------------------------*/
/*!
 * \file  startup.c 
 * \brief Initializing processor
 */
/*------------------------------------------------------------------------------------*/

#define UART0_BASE   0x40004000
#define UART_DATA    (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART_STATE   (*(volatile unsigned int *)(UART0_BASE + 0x04))

extern void main(void);
extern void systick_handler(void);
extern void PendSV_Handler(void);

#define STACK_TOP 0x20400000

void reset_handler(void)
{
    main();
    while (1);
}

void default_handler(void)
{
    const char* msg = "Fault\n";

    while (*msg)
    {
        while (UART_STATE & 0x1);
        UART_DATA = *msg++;
    }

    while (1);
}

__attribute__((section(".vectors")))
void (*vectors[])(void) =
{
    (void (*)(void))STACK_TOP,  // 0  - stack pointer
    reset_handler,              // 1  - reset
    default_handler,            // 2  - NMI
    default_handler,            // 3  - HardFault
    default_handler,            // 4  - MemManage
    default_handler,            // 5  - BusFault
    default_handler,            // 6  - UsageFault
    0, 0, 0, 0,                 // 7-10 - reserved
    default_handler,            // 11 - SVCall
    default_handler,            // 12 - DebugMon
    0,                          // 13 - reserved
    PendSV_Handler,             // 14 - PendSV
    systick_handler,            // 15 - SysTick
};
