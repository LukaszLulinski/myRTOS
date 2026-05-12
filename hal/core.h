/*------------------------------------------------------------------------------------*/
/*!
 * \file  core.h 
 * \brief Core definitions
 */
/*------------------------------------------------------------------------------------*/

#ifndef CORE_H
#define CORE_H

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */
/* Interrupt control and state register */
#define ICSR (*(volatile uint32_t*)0xE000ED04)

/* Registers SysTick */
#define SYST_CSR  (*(volatile uint32_t *)0xE000E010) // control and status
#define SYST_RVR  (*(volatile uint32_t *)0xE000E014) // reload value
#define SYST_CVR  (*(volatile uint32_t *)0xE000E018) // current value

/* Frequency */
#define SYSTEM_CLOCK 25000000  // 25 MHz

#define UART0_BASE   0x40004000
#define UART_DATA    (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART_STATE   (*(volatile unsigned int *)(UART0_BASE + 0x04))
#define UART_CTRL    (*(volatile unsigned int *)(UART0_BASE + 0x08))
#define UART_BAUDDIV (*(volatile unsigned int *)(UART0_BASE + 0x10))

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */

#endif /* CORE_H */
