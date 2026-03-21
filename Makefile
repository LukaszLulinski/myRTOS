
CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m4 -mthumb -nostdlib -ffreestanding -O0 -g
LDFLAGS = -T linker.ld -nostdlib -Wl,-Map=out/myRTOS.map

TARGET  = out/myRTOS.elf
SRCS    = src/main.c src/startup.c hal/systick.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET) out/myRTOS.map
