
CC      = arm-none-eabi-gcc

CFLAGS  = -mcpu=cortex-m4 -mthumb -nostdlib -ffreestanding -O0 -g \
		  -I./hal \
		  -I./kernel

LDFLAGS = -T linker.ld -nostdlib -Wl,-Map=out/myRTOS.map

TARGET  = out/myRTOS.elf

SRCS    = src/main.c 	\
	src/startup.c 		\
	hal/systick.c 		\
	kernel/task.c 		\
	kernel/scheduler.c 	\
	kernel/context.s	\
	kernel/mutex.c		\
	kernel/timer.c 		\
	kernel/semaphore.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET) out/myRTOS.map
