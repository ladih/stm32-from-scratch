CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -nostdlib
LDFLAGS = -T linker_minimal.ld
SRCS = startup_minimal.s main.c uart.c led.c systick.c button.c adc.c tim2.c utils.c commands.c

all: blink.bin

blink.elf: $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o blink.elf

blink.bin: blink.elf
	arm-none-eabi-objcopy -O binary blink.elf blink.bin

flash: blink.bin
	st-flash write blink.bin 0x08004000

clean:
	rm blink.bin