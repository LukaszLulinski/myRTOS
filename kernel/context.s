/*
 * \file    context.s
 * \brief   Context switching and starting scheduler
 * \details Context is switched in PendSV_Handler, starting scheduler by starting using PSP
 */
    
    .syntax unified
    .cpu cortex-m4
    .thumb

    .global PendSV_Handler
    .type PendSV_Handler, %function
    .global scheduler_start
    .type scheduler_start, %function

    .extern scheduler_run
    .extern current_task

PendSV_Handler:
    CPSID   I @ Switch off interrupts

    @ Save context of the current task
    MRS     R0, PSP
    STMDB   R0!, {R4-R11}

    PUSH    {LR}
    SUB     SP, SP, #4
    @ Save stack pointer of the current task
    LDR     R1, =current_task
    LDR     R1, [R1]
    STR     R0, [R1]
    @ Run scheduler
    BL      scheduler_run
    @ Load stack pointer of the new task
    LDR     R1, =current_task
    LDR     R0, [R1]
    LDR     R0, [R0]

    ADD     SP, SP, #4
    POP     {LR}
    LDMIA   R0!, {R4-R11}
    MSR     PSP, R0

    CPSIE   I @ Switch on interrupts

    BX      LR

scheduler_start:
    @ Get stack pointer of the current task and store to PSP
    LDR     R1, =current_task
    LDR     R0, [R1]
    LDR     R0, [R0]
    MSR     PSP, R0
    @ Use PSP as a SP
    MOV     R0, #0x02
    MSR     CONTROL, R0
    ISB
    
    BX      LR
