# myRTOS

A lightweight preemptive RTOS kernel for ARM Cortex-M4, written from scratch in C and ARM assembly. Built as an educational project to understand the internals of real-time operating systems.

Tested on **QEMU MPS2-AN386** (Cortex-M4 emulation).

---

## Features

- **Preemptive scheduler** with priority-based task selection
- **Context switching** implemented in ARM assembly (PendSV handler)
- **Periodic and aperiodic tasks**
- **Mutexes** with FIFO blocking queue
- **Software timers** with one-shot and cyclic modes
- **task_delay** — non-blocking delay that yields CPU to other tasks
- **Idle task** with WFI (Wait For Interrupt) for low power consumption
- **SysTick** — 1ms system tick driving the scheduler and timers

---

## Architecture

### Task States

```
          task_create()
               │
               ▼
           ┌───────┐
    ┌──────│ READY │◄─────────────────────┐
    │      └───────┘                      │
    │          │ scheduler selects        │ mutex_unlock()
    │          ▼                          │ task_delay expires
    │      ┌─────────┐   mutex_lock()  ┌─────────┐
    │      │ RUNNING │────────────────►│ BLOCKED │
    │      └─────────┘   task_delay()  └─────────┘
    │          │
    └──────────┘
     preempted by SysTick
```

### Interrupt Flow

```
SysTick (1ms)
├── tick_count++
├── timer_update()     — fire expired software timers
├── task_delay_update() — wake up delayed tasks
└── trigger PendSV

PendSV (lowest priority)
├── save R4-R11 to current task stack (PSP)
├── save SP to current_task->stack_ptr
├── scheduler_run() — select next highest priority READY task
├── load SP from new task stack_ptr
└── restore R4-R11 from new task stack
```

### Memory Layout (MPS2-AN386)

```
FLASH: 0x00000000 — 0x003FFFFF (4MB) — code, rodata, vectors
RAM:   0x20000000 — 0x203FFFFF (4MB) — stacks, TCB pool, data, bss
```

---

## Project Structure

```
myRTOS/
├── hal/
│   ├── core.h            — core system defines
│   └── systick.c/h       — SysTick driver (1ms tick)
├── kernel/
│   ├── task.c/h          — TCB, task_create, task_delay
│   ├── scheduler.c/h     — priority scheduler, idle task
│   ├── context.s         — PendSV context switch (ARM assembly)
│   ├── mutex.c/h         — mutex with FIFO blocking queue
│   └── timer.c/h         — software timers
├── app/
│   └── main.c            — demo application
├── linker.ld             — memory layout for MPS2-AN386
└── Makefile
```

---

## API

### Tasks

```c
// Create a task
void task_create(task_func_t func, uint32_t priority, uint32_t stack_size);

// Block current task for given number of ticks (yields CPU)
void task_delay(uint32_t ticks);
// Block current task until given number of ticks from the last wake (yields CPU)
void task_delay_until(uint32_t* last_wake_tick, uint32_t ticks);
```

### Mutexes

```c
void mutex_init(mutex_t *mutex);
void mutex_lock(mutex_t *mutex);    // blocks if mutex is taken
void mutex_unlock(mutex_t *mutex);  // wakes first task in FIFO queue
```

### Software Timers

```c
// Start a timer — returns handle from internal pool
timer_t *timer_start(uint32_t delay_ticks, bool cyclic, timer_func_t func);

// Stop a timer
void timer_stop(timer_t *timer);
```

---

## Usage Example

```c
#include "task.h"
#include "mutex.h"

static mutex_t uart_mutex;

static void task1_handler(void)
{
    while (1)
    {
        mutex_lock(&uart_mutex);
        uart_print("task 1\n");
        mutex_unlock(&uart_mutex);
    }
}

static void task2_handler(void)
{
    while (1)
    {
        mutex_lock(&uart_mutex);
        uart_print("task 2\n");
        mutex_unlock(&uart_mutex);
    }
}

void main(void)
{
    uart_init();
    mutex_init(&uart_mutex);

    task_create(task1_handler, 1, 64);
    task_create(task2_handler, 2, 64);

    scheduler_init();
    systick_init(1000);  // 1ms tick
}
```

---

## Building and Running

### Prerequisites

- [MSYS2](https://www.msys2.org/) with UCRT64 terminal
- ARM GCC toolchain: `pacman -S mingw-w64-ucrt-x86_64-arm-none-eabi-gcc`
- QEMU: `pacman -S mingw-w64-ucrt-x86_64-qemu-system-arm`

### Build

```bash
make
```

### Run in QEMU

```bash
qemu-system-arm \
  -machine mps2-an386 \
  -kernel out/myRTOS.elf \
  -nographic \
  -serial mon:stdio
```

### Debug with GDB

```bash
# Terminal 1 — start QEMU with GDB server
qemu-system-arm -machine mps2-an386 -kernel out/myRTOS.elf -nographic -serial mon:stdio -s -S

# Terminal 2 — connect GDB
gdb-multiarch out/myRTOS.elf
(gdb) set architecture arm
(gdb) target remote :1234
(gdb) break main
(gdb) continue
```

---

## Known Limitations

- `mutex_lock` is not fully atomic — requires `LDREX/STREX` for production use
- Maximum tasks: configurable via `MAX_TASKS` in `task.c`
- Maximum timers: configurable via `MAX_TIMERS` in `timer.c`
- Single mutex blocked list entry per task (no nested blocking)

---

## References

- [ARM Cortex-M System Design Kit TRM (DDI0479)](https://developer.arm.com/documentation/ddi0479/latest)
- [ARMv7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest/)
- [Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/latest/)
- [FreeRTOS Kernel](https://www.freertos.org/Documentation/RTOS_book.html) — reference implementation
