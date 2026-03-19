

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -nostdlib
LDFLAGS = -T linker_minimal.ld

all: blink.bin

blink.elf: main.c startup_minimal.s
	$(CC) $(CFLAGS) $(LDFLAGS) startup_minimal.s main.c -o blink.elf

blink.bin: blink.elf
	$(OBJCOPY) -O binary blink.elf blink.bin

flash: blink.bin
	st-flash write blink.bin 0x08000000

clean:
	rm -f *.elf *.bin