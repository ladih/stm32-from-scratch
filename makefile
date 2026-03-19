CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -nostdlib
LDFLAGS = -T linker.ld
SRCS = startup_minimal.s main.c uart.c led.c systick.c

all: blink.bin

blink.elf: $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o blink.elf

blink.bin: blink.elf
	arm-none-eabi-objcopy -O binary blink.elf blink.bin

flash: blink.bin
	st-flash write blink.bin 0x08000000

clean:
	rm -f *.elf *.bin

