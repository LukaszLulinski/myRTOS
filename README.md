# myRTOS

A small educational RTOS for ARM Cortex-M4, written in C and ARM assembly. It demonstrates task scheduling, context switching, synchronization, queues, and tick-based delays on QEMU's MPS2-AN386 board model.

## Features

- Priority-based preemptive scheduling. The scheduler selects the highest-priority task in `READY` state.
- PendSV context switching, with task register state saved on each task's stack.
- SysTick-driven timekeeping and task delays. `systick_init(1000u)` configures a 1 ms tick for the 25 MHz board clock.
- Tasks with relative (`task_delay`) and periodic (`task_delay_until`) delays.
- Mutexes with a FIFO list of blocked tasks.
- Counting semaphores. Blocked tasks are stored LIFO.
- Fixed-size queues built from semaphores, with up to 10 items and 8 bytes per item.
- Software timers with one-shot and cyclic modes.
- An idle task that waits using `WFI`.

## Project layout

```text
myRTOS/
├── hal/
│   ├── core.h              Cortex-M registers and MPS2-AN386 clock definition
│   └── systick.c/h         SysTick setup and tick counter
├── kernel/
│   ├── context.s           SVC startup and PendSV context switching
│   ├── mutex.c/h           Mutexes
│   ├── queue.c/h           Fixed-size queues
│   ├── scheduler.c/h       Priority scheduler and idle task
│   ├── semaphore.c/h       Counting semaphores
│   ├── task.c/h            Task control blocks, creation, and delays
│   └── timer.c/h           Software timers
├── src/
│   ├── main.c              Producer/consumer demo using a queue and UART
│   └── startup.c           Vector table and reset/fault handlers
│
├── linker.ld               MPS2-AN386 memory layout
└── Makefile                ARM cross-compilation build
```

## Build and run

### Requirements

- `arm-none-eabi-gcc` (ARM GNU toolchain)
- `qemu-system-arm`
- GNU Make

On MSYS2 UCRT64, the packages used for these tools are `mingw-w64-ucrt-x86_64-arm-none-eabi-gcc` and `mingw-w64-ucrt-x86_64-qemu-system-arm`.

Build from the project directory:

```sh
make
```

Run the firmware on the MPS2-AN386 model:

```sh
qemu-system-arm \
  -machine mps2-an386 \
  -kernel out/myRTOS.elf \
  -nographic \
  -serial mon:stdio
```

Use `Ctrl-A`, then `X` to exit QEMU's serial monitor.

### Debug with GDB

Start QEMU in one terminal:

```sh
qemu-system-arm -machine mps2-an386 -kernel out/myRTOS.elf \
  -nographic -serial mon:stdio -s -S
```

Connect from another terminal:

```sh
arm-none-eabi-gdb out/myRTOS.elf
(gdb) set architecture arm
(gdb) target remote :1234
(gdb) break main
(gdb) continue
```

## Current limitations

The current implementation has fixed task, stack, queue, and timer capacities. A task has only one `next` link for blocking lists, so it cannot safely wait on multiple synchronization objects at once. Synchronization operations do not yet provide full interrupt-safe atomicity, and semaphore wakeup currently assumes the released count is handed to the woken task.
