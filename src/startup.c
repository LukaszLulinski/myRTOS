
extern void main(void);
extern void systick_handler(void);

#define STACK_TOP 0x20400000

void reset_handler(void)
{
    main();
    while (1);
}

void default_handler(void)
{
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
    default_handler,            // 14 - PendSV
    systick_handler,            // 15 - SysTick
};
